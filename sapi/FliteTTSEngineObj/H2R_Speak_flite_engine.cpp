/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                         Copyright (c) 2010                            */
/*                        All Rights Reserved.                           */
/*                                                                       */
/*  Permission is hereby granted, free of charge, to use and distribute  */
/*  this software and its documentation without restriction, including   */
/*  without limitation the rights to use, copy, modify, merge, publish,  */
/*  distribute, sublicense, and/or sell copies of this work, and to      */
/*  permit persons to whom this work is furnished to do so, subject to   */
/*  the following conditions:                                            */
/*   1. The code must retain the above copyright notice, this list of    */
/*      conditions and the following disclaimer.                         */
/*   2. Any modifications must be clearly marked as such.                */
/*   3. Original authors' names are not deleted.                         */
/*   4. The authors' names are not used to endorse or promote products   */
/*      derived from this software without specific prior written        */
/*      permission.                                                      */
/*                                                                       */
/*  CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK         */
/*  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING      */
/*  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   */
/*  SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE      */
/*  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES    */
/*  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN   */
/*  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,          */
/*  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF       */
/*  THIS SOFTWARE.                                                       */
/*                                                                       */
/*************************************************************************/
/*             Author:  Alok Parlikar (aup@cs.cmu.edu)                   */
/*               Date:  March 2010                                       */
/*************************************************************************/
/*                                                                       */
/*  Interface to the eyes-free project for Flite (www.cmuflite.org)      */
/*                                                                       */
/*************************************************************************/

// Standard headers
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include<windows.h>

// Flite headers
extern "C" {
#include "flite.h"
};

extern void compress(short *samples, int num_samples, float mu);


// Local headers
#include "H2R_Speak_Flite_engine.h"
#include "H2R_Speak_flite_voice.h"
#include "H2R_Speak_lib.h"

extern "C" cst_voice *REGISTER_VOX(const char *voxdir);
extern "C" void UNREGISTER_VOX(cst_voice *vox);

//int *ttsSynthDoneCBPointer;
int (*ttsSynthDoneCBPointer)(short *wav, int numSamples); 
static int ttsAbort = 0;
static int ttsStream = 1;
extern char* flite_voxdir_path;
FliteEngine::Voices* loadedVoices = NULL;
FliteEngine::Voice* currentVoice;

/* BEGIN VOICE SPECIFIC CODE */

// Declarations
extern "C" void usenglish_init(cst_voice *v);
extern "C" cst_lexicon *cmulex_init(void);

extern "C" void cmu_indic_lang_init(cst_voice *v);
extern "C" cst_lexicon *cmu_indic_lex_init(void);

// extern cst_voice* REGISTER_VOX(const char*);
// extern void UNREGISTER_VOX(cst_voice *);


void setVoiceList() {
	if (loadedVoices != NULL)
	{
		OutputDebugString("[TRW] setVoiceList: loadedVoices not null");
		return;
	} else if (flite_lang_list_length == 0) {
		flite_add_lang("eng", usenglish_init, cmulex_init);
		flite_add_lang("cmu_indic_lang", cmu_indic_lang_init, cmu_indic_lex_init);
	}
	else {
	}
	loadedVoices = new FliteEngine::Voices(20, FliteEngine::ONLY_ONE_VOICE_REGISTERED); // Max number of voices is the first argument.
	if (loadedVoices == NULL)
	{
		OutputDebugString("[TRW] setVoiceList: setting loadedVoices FAILED");
		return;
	}
}

/* END VOICE SPECIFIC CODE */

void wave_rescale(short *samples, int num_samples, float factor)
{
		for (int i = 0; i < num_samples; ++i) {
			samples[i] = (INT)(samples[i] * factor / 50);
	}
}

/* Callback from flite. Should call back the TTS API */
int fliteCallback(const cst_wave *w, int start, int size, int last, cst_audio_streaming_info_struct *asi)
{

	short *waveSamples = (short *)&w->samples[start];

	char OutputString[500];

	// Change volume if NVDA has asked us to
	if (embedded_value[EMBED_A] != 50) //50% is the default rendered volume
	{
		wave_rescale(waveSamples, size, embedded_value[EMBED_A]);
	}
	// char *castedWave = (char *)&w->samples[start];
	// size_t bufferSize = size * sizeof(short);
	int num_channels = w->num_channels;
	int sample_rate = w->sample_rate;

	//The following two lines are needed to prevent and infinite loop in NVDA --THIS NEEDS TO BE EXPLORED
	sprintf_s(OutputString, 498, "[TRW] flite callback received! Start: %d. Size: %d. Last: %d. Channels: %d. Rate: %d.", start, size, last, num_channels, sample_rate );
//	OutputDebugString(OutputString);

//	Sleep(250);

	if (ttsSynthDoneCBPointer != NULL)
	{
		if (last == 1)
		{
			/* Bug in audio rendering: Short utterances are not played. Fix it by playing silence in addition. */
			float dur = (start + size) / sample_rate;
			if (size != 0 && dur < .8) // Minimum buffer the NVDA interface plays
			{
				OutputDebugString(" flitecallback  sending padding here");
				// create padding
				size_t padding_length = num_channels * (sample_rate / 3);
//				size_t padding_length = 1000/300; // Minimum buffer the NVDA interface plays
				char* paddingWave = new char[padding_length * 2]; //samples are 2 bytes (16 bits)

				char OutputString[502];
				sprintf_s(OutputString, 500, "[TRW] fliteCallback creating padding length = %d", padding_length);
				OutputDebugString(OutputString);

				for (int i = 0; i < (int)padding_length * 2; i++)
					paddingWave[i] = 0;
				sprintf_s(OutputString, 498, "[TRW] fliteCallback last == 1 calling ttsSynthDoneCBPointer with length %d", size);
				OutputDebugString(OutputString);
				ttsSynthDoneCBPointer(waveSamples, size); // Output Last set of samples, and then
				sprintf_s(OutputString, 498, "[TRW] fliteCallback last == 1 calling ttsSynthDoneCBPointer with padding length %d", padding_length);
				OutputDebugString(OutputString);
				ttsSynthDoneCBPointer((short *)paddingWave, padding_length); //Output padding to flush the audio buffer

				delete[] paddingWave;
			}
			else {
				// See comment above on why this has been changed.
				/* ttsSynthDoneCBPointer(&asi->userdata, sample_rate, ANDROID_TTS_AUDIO_FORMAT_PCM_16_BIT, num_channels, &castedWave, &bufferSize,
			    ANDROID_TTS_S];YNTH_DONE); */
//				OutputDebugString("[TRW] fliteCallback last == 1 but duration is long enough calling ttsSynthDoneCBPointer");
				ttsSynthDoneCBPointer(waveSamples, size);
			}
		}
		else {
//			OutputDebugString("[TRW] fliteCallback last != 1 calling ttsSynthDoneCBPointer");
			ttsSynthDoneCBPointer(waveSamples, size);
		}
	}
	else
	{
		OutputDebugString("[TRW] fliteCallback ttsSynthDoneCBPointer = NULL aborting  SHOULD NOT GET HERE!!!");
		ttsAbort = 1;
	}

	if (ttsAbort == 1) {
		OutputDebugString("[TRW] fliteCallback tsAbort == 1 returning CST_AUDIO_STREAM_STOP");
		return CST_AUDIO_STREAM_STOP;
	}

	return CST_AUDIO_STREAM_CONT;
}



// Shutsdown the TTS engine. Unload all voices
H2R_Speak_tts_result_t shutdown(void* engine)
{
	if (flite_voxdir_path != NULL) {
		free(flite_voxdir_path);
		flite_voxdir_path = NULL;
	}

	if (loadedVoices != NULL)
		delete loadedVoices;
	loadedVoices = NULL;
	return H2R_SPEAK_TTS_SUCCESS;
}

extern "C" {
	__declspec(dllexport) H2R_Speak_ERROR H2R_Speak_Terminate() {
		if (flite_voxdir_path != NULL) {
			free(flite_voxdir_path);
			flite_voxdir_path = NULL;
		}
		if (loadedVoices != NULL) {
			delete loadedVoices;
			loadedVoices = NULL;
		}
		return EE_OK;
	}
}


// We load language when we set the language. Thus, this function always succeeds.
H2R_Speak_tts_result_t loadLanguage(void* engine, const char *lang, const char *country, const char *variant)
{
	return H2R_SPEAK_TTS_SUCCESS;
}

// Set the language based on locale. We use our voices manager to do this job.
//H2R_Speak_tts_result_t setLanguage(const char * lang, const char * flitevoxFileName)
//{
//	// We don't support country or variant so just set by language
//
//	// Request the voice to voice-manager
//	currentVoice = loadedVoices->GetVoiceForLocale(lang, flitevoxFileName);
//	if (currentVoice == NULL)
//	{
//		return H2R_SPEAK_TTS_FAILURE;
//	}
//
//	if (currentVoice->GetFliteVoice() == NULL) {
//		("[TRW] setLanguage : currentVoice  = NULL");
//		return H2R_SPEAK_TTS_FAILURE;
//	}
//	else {
//		return H2R_SPEAK_TTS_SUCCESS;
//	}
//}

H2R_Speak_tts_result_t setSpeechRate(int rate) {
	cst_voice* flite_voice;
	//LOGI("TtsEngine::setSpeechRate : Attempting to set rate %d", rate);
	char OutputString[500];
	sprintf_s(OutputString, 490, "[TRW] setSpeechRate: rate = %d", rate);
	OutputDebugStringA( OutputString );
	if (currentVoice == NULL)
	{
		OutputDebugStringA( "[TRW] setSpeechRate: currVoice is null returning failure");
		return H2R_SPEAK_TTS_FAILURE;
	}

	flite_voice= currentVoice->GetFliteVoice();
	if (flite_voice == NULL)
	{
		OutputDebugStringA("[TRW] setSpeechRate: flite_voice is null returning failure");
		return H2R_SPEAK_TTS_FAILURE;
	}

	// Set duration_stretch parameter on the voice. Rate is given as a percentage.
	if (rate == 0) {
		OutputDebugStringA("[TRW] setSpeechRate: rate is 0 returning failure");
		return H2R_SPEAK_TTS_FAILURE;
	}

	// Each voice has its own default duration stretch parameter. We should not discard this original value.
	if (!feat_present(flite_voice->features, "orig_duration_stretch")) {

		float orig_dur_stretch = get_param_float(flite_voice->features, "duration_stretch", 1.0);
		sprintf_s(OutputString, 490, "[TRW] setSpeechRate: setting orig_duration_stretch to %d", orig_dur_stretch);
		OutputDebugStringA(OutputString);
		feat_set_float(flite_voice->features, "orig_duration_stretch", orig_dur_stretch);
	}

	// Now update the actual duration stretch as a percentage of the default value.
	// AEW: default value for multiplier was 100; have changed the value such that the
	// new rate is 90% of the original (default) rate.
	float temp = feat_float(flite_voice->features, "orig_duration_stretch") * 111.11 / (rate + 50);
	sprintf_s(OutputString, 490, "[TRW] setSpeechRate: setting orig_duration_stretch to %f", temp);
	OutputDebugStringA(OutputString);
	feat_set_float(flite_voice->features, "duration_stretch", temp);
	OutputDebugStringA("[TRW] setSpeechRate: currVoice returning success");
	return H2R_SPEAK_TTS_SUCCESS;
}

// LanguageListItem availability check does not use the "streaming" byte, as in setLanguage
// Also, check is made against the entire locale.
H2R_Speak_tts_support_result_t isLanguageAvailable(void* engine, const char *lang, const char *country,
	const char *variant)
{
	// The hack for java to make sure flite is available.
	// Only flite can respond "YES" to the language "eng-USA-is_flite_available"
	if ((strcmp(lang, "eng") == 0) &&
		(strcmp(country, "USA") == 0) &&
		(strcmp(variant, "is_flite_available") == 0)) {
		return H2R_SPEAK_TTS_LANG_COUNTRY_VAR_AVAILABLE;
	}

	// The hack to set streaming:
	// If language and country are not set, then variant will be
	// interpreted as being "stream" or "nostream" to set the appropriate parameters.
	// The default is to stream.
//	if ((strcmp(lang, "") == 0) && (strcmp(country, "") == 0))
//	{
		if (strcmp(variant, "stream") == 0)
		{
			ttsStream = 1;
			return H2R_SPEAK_TTS_LANG_COUNTRY_VAR_AVAILABLE;
		}
		else if (strcmp(variant, "nostream") == 0)
		{
			ttsStream = 0;
			return H2R_SPEAK_TTS_LANG_COUNTRY_VAR_AVAILABLE;
		}
		else
		{
			return H2R_SPEAK_TTS_LANG_NOT_SUPPORTED;
		}
//	}
//
//	return loadedVoices->IsLocaleAvailable(lang, country, variant);
}

// Provide information about the currently set language.
H2R_Speak_tts_result_t getLanguage(void* engine, char *language, char *country, char *variant)
{
	if (currentVoice == NULL)
		return H2R_SPEAK_TTS_FAILURE;

	strcpy(language, currentVoice->GetLanguage());
	strcpy(country, currentVoice->GetCountry());
	strcpy(variant, currentVoice->GetVariant());
	return H2R_SPEAK_TTS_SUCCESS;
}

// Provide Sample rate of current Voice
const int getSampleRate(void* engine)
{
	int rate = 16000;
	if (currentVoice != NULL)
	{
		rate = currentVoice->GetSampleRate();
	}

	//LOGV("getSampleRate: %d", rate);
	return rate;
}



// Properties are not yet implemented.
H2R_Speak_tts_result_t setProperty(void* engine, const char *property, const char *value,
	const size_t size)
{
	return H2R_SPEAK_TTS_PROPERTY_UNSUPPORTED;
	// Other values hint: TTS_VALUE_INVALID, TTS_SUCCESS
}

//Properties are not yet implemented.
H2R_Speak_tts_result_t getProperty(void* engine, const char *property, char *value, size_t *iosize)
{
	return H2R_SPEAK_TTS_PROPERTY_UNSUPPORTED;
}

// Support for synthesizing IPA text is not implemented.
H2R_Speak_tts_result_t synthesizeIpa(void* engine, const char * ipa, int * buffer, size_t bufferSize, void * userdata)
{
	return H2R_SPEAK_TTS_FAILURE;
}

extern "C" {
	// Interrupts synthesis.
	__declspec(dllexport) H2R_Speak_tts_result_t H2R_Speak_stop()
	{
		OutputDebugString("[TRW] H2R_Speak_stop called setting ttsAbort = 1");
		ttsAbort = 1;
		return H2R_SPEAK_TTS_SUCCESS;
	}
}

// Synthesize Text. Check if streaming is requested, and stream iff so.
/*H2R_Speak_tts_result_t synthesizeText(void* engine, const char * text, int *buffer, size_t bufferSize, void * userdata)
{
	cst_voice* flite_voice;
	if (currentVoice == NULL)
	{
		return H2R_SPEAK_TTS_FAILURE;
	}
	flite_voice = currentVoice->GetFliteVoice();
	if (flite_voice == NULL)
	{
		return H2R_SPEAK_TTS_FAILURE;
	}

	if (ttsStream)
	{
		ttsAbort = 0;
		cst_audio_streaming_info *asi;
		asi = new_audio_streaming_info();
		asi->min_buffsize = bufferSize;
		asi->asc = fliteCallback;
		asi->userdata = userdata;
		feat_set(flite_voice->features,
			"streaming_info",
			audio_streaming_info_val(asi));

		/* SSML support */
/*		if (!strncmp(text, "<?xml version=\"1.0\"?>", 21))
		{
			// XML string given. Parse as SSML.

			// For now, just strip off all tags and say whatever remains.
			int pos1 = 0, pos2 = 0;
			int maxpos = strlen(text);

			char* textOverride = (char*)text;

			while (pos2 < maxpos)
			{
				while ((pos2 < maxpos) && (textOverride[pos2] != '<'))
				{
					textOverride[pos1] = textOverride[pos2];
					pos1++;
					pos2++;
				}
				while ((pos2 < maxpos) && (textOverride[pos2] != '>'))
				{
					pos2++;
				}
			}
			textOverride[pos1] = '\0';

		}

		cst_tokenstream *ts;
*/
/*		char* padded_text = reinterpret_cast<char*>(malloc(strlen(text) + 4));
		snprintf(padded_text, strlen(text) + 4, "%s\n\n", text);

		if ((ts = ts_open_string(padded_text,
			get_param_string(flite_voice->features, "text_whitespace", cst_ts_default_whitespacesymbols),
			get_param_string(flite_voice->features, "text_singlecharsymbols", cst_ts_default_singlecharsymbols),
			get_param_string(flite_voice->features, "text_prepunctuation", cst_ts_default_prepunctuationsymbols),
			get_param_string(flite_voice->features, "text_postpunctuation", cst_ts_default_postpunctuationsymbols))) == NULL) {

			free(padded_text);
			padded_text = NULL;
			return H2R_SPEAK_TTS_FAILURE;
		}

		free(padded_text);
*/ /*		padded_text = NULL;
		flite_ts_to_speech(ts,
			flite_voice,
			"stream");

		//	cst_utterance *u = flite_synth_text(text,flite_voice);
		//	delete_utterance(u);

		feat_remove(flite_voice->features, "streaming_info");

		// Mark synthesis as done
		if (ttsSynthDoneCBPointer != NULL) {
//			int rate = getSampleRate(NULL);
//			ttsSynthDoneCBPointer(&userdata, rate, H2R_SPEAK_TTS_AUDIO_FORMAT_PCM_16_BIT, 1,
//				NULL, NULL,
//				H2R_SPEAK_TTS_SYNTH_DONE);
		}

		return H2R_SPEAK_TTS_SUCCESS;
	}
	return H2R_SPEAK_TTS_FAILURE;

}
*/
extern "C" {

//	__declspec(dllexport) H2R_Speak_ERROR H2R_Speak_SetVoice(char *lang, char *voxfile) {
//		if (setLanguage(lang, voxfile) == H2R_SPEAK_TTS_SUCCESS) {
//			return(EE_OK);
//		}
//		else {
//			return(EE_INTERNAL_ERROR);
//		}
//	}
}
//	__declspec(dllexport) H2R_Speak_tts_result_t init(H2R_Speak_tts_synth_cb_t *synthDoneCBPtr, const char *H2R_SpeakPath)

H2R_Speak_tts_result_t H2RSpeak_init(const char *H2R_SpeakPath, int (*synthDoneCBPtr)(short *wav, int numSamples) )
{
	OutputDebugString("[TRW] H2RSPEAK_init entered");
		// First make sure we receives the data directory. That's very crucial.
	if (H2R_SpeakPath != NULL) {
		flite_voxdir_path = cst_alloc(char, strlen(H2R_SpeakPath)+ 2);
		snprintf(flite_voxdir_path, strlen(H2R_SpeakPath) + 1, "%s", H2R_SpeakPath);
	}
	else {
		return H2R_SPEAK_TTS_FAILURE;
	}
	char OutputString[502];
	sprintf_s(OutputString, 500, "[TRW] H2RSpeak_init: flite_voxdir_path = %s", flite_voxdir_path);
	OutputDebugString(OutputString);

	ttsSynthDoneCBPointer = synthDoneCBPtr;
	flite_init();
	setVoiceList();
	if (loadedVoices == NULL)
	{
		OutputDebugString("[TRW] H2RSPEAK_init loadedVoices = null");
		return H2R_SPEAK_TTS_FAILURE;
	}
	currentVoice = loadedVoices->GetCurrentVoice();
	if (currentVoice) {
		if (currentVoice->GetFliteVoice() == NULL) {
			OutputDebugString("[TRW] H2RSPEAK_init fliteVoice = null");
			return H2R_SPEAK_TTS_FAILURE;
		}
	}

	ttsAbort = 0;
	OutputDebugString("[TRW] H2RSPEAK_init returning success");
	return H2R_SPEAK_TTS_SUCCESS;
}

extern "C" {
	__declspec(dllexport) void H2R_Speak_Add_Voice(char *lang) {
		char OutputString[500];
		sprintf_s(OutputString, 498, "[TRW] H2R_Speak_Add_Voice Entered lang = %s", lang);
		OutputDebugStringA(OutputString);
		if (loadedVoices != NULL) {
			if (loadedVoices->voiceExists(lang, NULL, NULL) == FALSE) {
				OutputDebugStringA("[TRW] H2R_Speak_Add_Voice calling AddLinkedVoice");
				loadedVoices->AddLinkedVoice(lang, "", "", REGISTER_VOX, UNREGISTER_VOX);

			} 
			else {
				OutputDebugStringA("[TRW] H2R_Speak_Add_Voice voice Exists doing nothing");
			}
		}
		else {
			OutputDebugStringA("[TRW] H2R_Speak_Add_Voice loadedVoices = NULL doing nothing");
		}
	}
}
