/*************************************************************************/
/*                                                                       */
/*                           Cepstral, LLC                               */
/*                        Copyright (c) 2001                             */
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
/*  CEPSTRAL, LLC AND THE CONTRIBUTORS TO THIS WORK DISCLAIM ALL         */
/*  WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED       */
/*  WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL         */
/*  CEPSTRAL, LLC NOR THE CONTRIBUTORS BE LIABLE FOR ANY SPECIAL,        */
/*  INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER          */
/*  RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION    */
/*  OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR  */
/*  IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.          */
/*                                                                       */
/*************************************************************************/
/*             Author:  David Huggins-Daines (dhd@cepstral.com)          */
/*               Date:  December 2001                                    */
/*************************************************************************/
/*                                                                       */
/*  FliteTTSEngineObj.cpp: implementation of SAPI interface to Flite     */
/*                                                                       */
/*************************************************************************/
/* Updated December 2015: Alok Parlikar <alok@cobaltspeech.com>          */
/*   to work with Flite-2.0.0.                                           */
/*************************************************************************/

#include "FliteTTSEngineObj.h"
#include "H2R_Speak_lib.h"
#include "../FliteCMUGenericCG/FliteCMUGenericCGObj.h"
#include "H2R_Speak_flite_voice.h"
#include "H2R_Speak_Flite_engine.h"

extern "C" {
#include "flite.h"
#include "cst_tokenstream.h"
};
extern int(*ttsSynthDoneCBPointer)(short *wav, int numSamples);
extern int ttsAbort = 0;
extern int ttsStream = 1;
extern FliteEngine::Voices* loadedVoices;
extern FliteEngine::Voice* currentVoice;
extern H2R_Speak_tts_result_t setSpeechRate(int rate);
int param_defaults[N_SPEECH_PARAM];
int embedded_value[N_EMBEDDED_VALUES];
int embedded_default[N_EMBEDDED_VALUES];

//extern  H2R_Speak_tts_result_t init(H2R_Speak_tts_synth_cb_t *synthDoneCBPtr, const char *H2R_SpeakPath);
extern H2R_Speak_tts_result_t H2RSpeak_init(const char *H2R_SpeakPath, int(*synthDoneCBPtr)(short*, int) );
static cst_val *sapi_tokentowords(cst_item *i);
cst_voice *curr_vox = NULL;
char* flite_voxdir_path;
int fliteCallback(const cst_wave *w, int start, int size,
	int last, cst_audio_streaming_info_struct *asi);
// FliteEngine::Voices* loadedVoices;

#define sgn(x) (x>0?1:x?-1:0)

void compress(short *samples, int num_samples, float mu) {
	int i = 0;
	short limit = 30000;
	short x;
	for (i = 0; i < num_samples; i++)
	{
		x = samples[i];
		samples[i] = limit * (sgn(x)*(log(1 + (mu / limit)*abs(x)) / log(1 + mu)));
	}
}

int audioStreamChunk(const cst_wave *w, int start, int size, int last, cst_audio_streaming_info *asi)
{
	CFliteTTSEngineObj *speechObject = (CFliteTTSEngineObj*)asi->userdata;
	ULONG b;
	speechObject->get_actions_and_do_them();
	if (speechObject->aborted) 
		return CST_AUDIO_STREAM_STOP;

		short *waveSamples = (short *)&w->samples[start];
		compress(waveSamples, size, 5);


	cst_wave *wav = copy_wave(w);
	if (speechObject->volume != 100)
		cst_wave_rescale(wav, speechObject->volume * 65536 / 100);
	speechObject->site->Write(&wav->samples[start],
		size * sizeof(*wav->samples),
		&b);
	speechObject->bcount += size * sizeof(*wav->samples);

	delete_wave(wav);
	return CST_AUDIO_STREAM_CONT;
}

STDMETHODIMP
CFliteTTSEngineObj::SetObjectToken(ISpObjectToken* pToken)
{
	USES_CONVERSION;
	CSpDynamicString voxdir;
	char *avoxdir;
	HRESULT hr;
	const cst_val *ttwv;

	if (!SUCCEEDED(hr = SpGenericSetObjectToken(pToken, vox_token)))
		return hr;
	if (!SUCCEEDED(vox_token->GetStringValue(L"voxdir", &voxdir)))
		avoxdir = NULL; /* It isn't always necessary */
	else
		avoxdir = W2A(voxdir);

	if (curr_vox)
		(*unregfunc)(curr_vox);
	if ((curr_vox = (*regfunc)(avoxdir)) == NULL)
		return E_INVALIDARG; /* or something */

	if ((ttwv = feat_val(curr_vox->features, "tokentowords_func"))) {
		feat_set(curr_vox->features, "old_tokentowords_func", ttwv);
		feat_set(curr_vox->features, "tokentowords_func",
			 itemfunc_val(sapi_tokentowords));
	}

	return hr;
}

STDMETHODIMP
CFliteTTSEngineObj::GetOutputFormat(const GUID * pTargetFormatId,
				    const WAVEFORMATEX * pTargetWaveFormatEx,
				    GUID * pDesiredFormatId,
				    WAVEFORMATEX ** ppCoMemDesiredWaveFormatEx)
{
	WAVEFORMATEX *wfx;

	if (curr_vox == NULL)
		return SpConvertStreamFormatEnum(SPSF_16kHz16BitMono,
						 pDesiredFormatId,
						 ppCoMemDesiredWaveFormatEx);

	/* Just return the default format for the voice, since SAPI
           can do all the necessary conversion.  (our resampling
           algorithm is a lot better than SAPI's, though...) */
	if ((wfx = (WAVEFORMATEX *)CoTaskMemAlloc(sizeof(*wfx))) == NULL)
		return E_OUTOFMEMORY;
	memset(wfx, 0, sizeof(*wfx));
	wfx->nChannels = 1;
	wfx->nSamplesPerSec = get_param_int(curr_vox->features, "sample_rate", 16000);
	wfx->wFormatTag = WAVE_FORMAT_PCM;
	wfx->wBitsPerSample = 16;
	wfx->nBlockAlign = wfx->nChannels*wfx->wBitsPerSample/8;
	wfx->nAvgBytesPerSec = wfx->nSamplesPerSec*wfx->nBlockAlign;

	*pDesiredFormatId = SPDFID_WaveFormatEx;
	*ppCoMemDesiredWaveFormatEx = wfx;

	return S_OK;
}

int
CFliteTTSEngineObj::phoneme_len(cst_item *s)
{
    return (short) ((item_feat_float(s, "end")
                     - ffeature_float(s, "p.end")) * 1000);
}

int
CFliteTTSEngineObj::viseme_len(cst_item *s)
{
	int len, vis, v;

	if (!visemefunc)
		return 0;

	vis = (*visemefunc)(s);
	len = phoneme_len(s);
	for (s = item_next(s); s; s = item_next(s)) {
		v = (*visemefunc)(s);
		if (v != vis)
			break;
		len += phoneme_len(s);
	}

	return len;
}

int
CFliteTTSEngineObj::next_viseme(cst_item *s)
{
	int vis, v = 0;

	if (!visemefunc)
		return 0;

	vis = (*visemefunc)(s);
	for (s = item_next(s); s; s = item_next(s)) {
		v = (*visemefunc)(s);
		if (v != vis)
			break;
	}

	return v;
}

void
CFliteTTSEngineObj::send_item_events()
{
	USES_CONVERSION;
	cst_item *i;
	int sps;

	sps = get_param_int(curr_vox->features, "sample_rate", 16000);
	for (i = relation_head(utt_relation(curr_utt, "Segment"));
	     i; i = item_next(i))
	{
		int offset;
		SPEVENT evt;
		cst_item *token;
		const cst_val *bmark;

		offset = (int) (ffeature_float(i, "p.end") * sps * sizeof(short));
		token = path_to_item(i, "R:SylStructure.parent.parent.R:Token.parent");

		/* Word boundaries */
		if (token
		    && item_parent(item_as(i, "SylStructure"))
		    && item_prev(item_as(i, "SylStructure")) == NULL
		    && item_prev(item_parent(item_as(i, "SylStructure"))) == NULL
		    && (token !=
			path_to_item(i, "R:SylStructure.parent.parent.p."
				     "R:Token.parent")))
		{
			cst_item *prev;

			if ((prev = item_prev(token))
			    && (bmark = item_feat(prev, "bookmark")))
			{
				SPEVENT fooevt; /* SpClearEvent will segfault... */
				fooevt.eEventId = SPEI_TTS_BOOKMARK;
				fooevt.elParamType = SPET_LPARAM_IS_STRING;
				fooevt.ullAudioStreamOffset = bcount + offset;
				fooevt.wParam = atol(val_string(bmark));
				fooevt.lParam = (LPARAM) A2W(val_string(bmark));
				site->AddEvents(&fooevt, 1);
			}
			SpClearEvent(&evt);
			evt.eEventId = SPEI_WORD_BOUNDARY;
			evt.elParamType = SPET_LPARAM_IS_UNDEFINED;
			evt.ullAudioStreamOffset = bcount + offset;
			evt.wParam = item_feat_int(token, "token_length");
			evt.lParam = item_feat_int(token, "token_pos");
			site->AddEvents(&evt, 1);
		}

		/* Visemes */
		if (visemefunc
		    && ((*visemefunc)(i)
			!= (*visemefunc)(item_prev(i)))) {
			SpClearEvent(&evt);
			evt.eEventId = SPEI_VISEME;
			evt.elParamType = SPET_LPARAM_IS_UNDEFINED;
			evt.ullAudioStreamOffset = bcount + offset;
			evt.wParam = MAKEWPARAM(next_viseme(i), viseme_len(i));
			evt.lParam = MAKELPARAM((*visemefunc)(i), (*featurefunc)(i));
			site->AddEvents(&evt, 1);
		}

		/* Bookmarks:

		   If the previous token is a bookmark and this is the
		   first segment in the first word of the current
		   token, then send an event. (we did that already)

		   If this is the last segment in the utterance
		   (probably a pause) and the last token in the
		   utterance is a bookmark, then send an event. */

		if (item_next(i) == NULL
		    && (bmark = item_feat(relation_tail(utt_relation(curr_utt, "Token")),
					  "bookmark")))
		{
			SPEVENT fooevt; /* SpClearEvent will segfault... */
			fooevt.eEventId = SPEI_TTS_BOOKMARK;
			fooevt.elParamType = SPET_LPARAM_IS_STRING;
			fooevt.ullAudioStreamOffset = bcount + offset;
			fooevt.wParam = atol(val_string(bmark));
			fooevt.lParam = (LPARAM) A2W(val_string(bmark));
			site->AddEvents(&fooevt, 1);
		}

		/* Phonemes */
		if (phonemefunc) {
			SpClearEvent(&evt);
			evt.eEventId = SPEI_PHONEME;
			evt.elParamType = SPET_LPARAM_IS_UNDEFINED;
			evt.ullAudioStreamOffset = bcount + offset;
			evt.wParam = MAKEWPARAM((*phonemefunc)(item_next(i)),
						phoneme_len(i));
			evt.lParam = MAKELPARAM((*phonemefunc)(i),
						(*featurefunc)(i));
			site->AddEvents(&evt, 1);
		}
	}
}

void
CFliteTTSEngineObj::send_sentence_event(int fpos)
{
	SPEVENT evt;

	SpClearEvent(&evt);

	evt.eEventId = SPEI_SENTENCE_BOUNDARY;
	evt.elParamType = SPET_LPARAM_IS_UNDEFINED;
	evt.ullAudioStreamOffset = bcount;
	evt.lParam = sentence_start;        // character position within the current text input stream of the sentence being synthesized.	
	evt.wParam = fpos - sentence_start; // character length of the sentence including punctuation in the current input stream being synthesized.
	site->AddEvents(&evt, 1);

	sentence_start = fpos;
}

void
CFliteTTSEngineObj::synth_one_utt()
{
	cst_wave *wav;
	ULONG b;
	if (!(curr_utt && tok_rel && relation_head(tok_rel)))
		return;

	cst_audio_streaming_info *asi;
	asi = new_audio_streaming_info();
	asi->asc = audioStreamChunk;
	asi->userdata = this;
	/* Link the vox features into the utterance features so the voice  */
	/* features will be searched too (after the utt ones)              */
	feat_link_into(curr_vox->features, curr_utt->features);
	feat_link_into(curr_vox->ffunctions, curr_utt->ffunctions);

	feat_set(curr_utt->features, "streaming_info", audio_streaming_info_val(asi));

	if (curr_vox->utt_init)
		curr_vox->utt_init(curr_utt, curr_vox);
	utt_synth_tokens(curr_utt);
	
	send_item_events();
	delete_utterance(curr_utt);
	asi = NULL;
	curr_utt = NULL;
	tok_rel = NULL;
}

void
CFliteTTSEngineObj::start_new_utt()
{
	curr_utt = new_utterance();
	tok_rel = utt_relation_create(curr_utt, "Token");
}

/* snarfed from flite.c */
static int
utt_break(cst_tokenstream *ts, const char *token, cst_relation *tokens)
{
    /* This is English (and some other latin based languages) */
    /* so it shouldn't be here                                */
    const char *postpunct;
    const char *ltoken;

	cst_item *tail = relation_tail(tokens);

    if (!item_feat_present(tail, "punc"))
	    return FALSE;

    ltoken = item_name(tail);
    postpunct = item_feat_string(tail, "punc");

	if (strchr(ts->whitespace,'\n') != cst_strrchr(ts->whitespace,'\n'))
	 /* contains two new lines */
	 return TRUE;
    else if (strchr(postpunct,':') ||
	     strchr(postpunct,'?') ||
	     strchr(postpunct,'!'))
	return TRUE;
	else if (strchr(postpunct, '.') &&
		(strlen(ts->whitespace) > 1) ) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

static cst_val *
sapi_tokentowords(cst_item *i)
{
	const cst_val *ttwv;

	if (item_feat_present(i, "phones"))
		return default_tokentowords(i);
	if (item_feat_present(i, "bookmark"))
		return NULL;

	ttwv = utt_feat_val(item_utt(i), "old_tokentowords_func");
	if (ttwv)
		return (*val_itemfunc(ttwv))(i);
	else
		return default_tokentowords(i);
}

static cst_tokenstream *
my_ts_open_string(cst_utterance *utt, const char *text)
{
	cst_tokenstream *ts;

	ts = ts_open_string(text,
		get_param_string(utt->features,"text_whitespace", cst_ts_default_whitespacesymbols),
		get_param_string(utt->features, "text_singlecharsymbols",cst_ts_default_singlecharsymbols),
		get_param_string(utt->features, "text_prepunctuation",cst_ts_default_prepunctuationsymbols),
		get_param_string(utt->features, "text_postpunctuation", cst_ts_default_postpunctuationsymbols)
		);


	return ts;
}

static cst_item *
append_new_token(cst_tokenstream *ts, const char *token, cst_relation *rel)
{
	cst_item *t;

	t = relation_append(rel, NULL);
	item_set_string(t, "name", token);
	if (ts) {
		item_set_string(t, "whitespace", ts->whitespace);
		item_set_string(t, "prepunctuation", ts->prepunctuation);
		item_set_string(t, "punc", ts->postpunctuation);
	}
	return t;
}

/* Each step is prev * (10**(1/10)) */
static const double sapi_ratetab_foo[21] = {
	3,
	2.68787537952229,
	2.40822468528069,
	2.15766927997459,
	1.93318204493176,
	1.73205080756888,
	1.55184557391536,
	1.39038917031591,
	1.24573093961552,
	1.11612317403391,
	1,
	0.895958459840763,
	0.802741561760231,
	0.719223093324865,
	0.644394014977255,
	0.577350269189626,
	0.517281857971787,
	0.46346305677197,
	0.415243646538506,
	0.372041058011302,
	0.333333333333333
};
static const double *sapi_ratetab = sapi_ratetab_foo + 10;

/* Each step is prev * (2**(1/24)) */
static const double sapi_pitchtab_foo[21] = {
	1.33333333333333,
	1.29537592153814,
	1.25849908357559,
	1.22267205760623,
	1.18786495752045,
	1.15404874800819,
	1.12119522033829,
	1.0892769688274,
	1.0582673679788,
	1.02814055027196,
	1.0,
	0.970435455228124,
	0.942809041582063,
	0.915969098305392,
	0.889893236113355,
	0.864559703100672,
	0.839947366596581,
	0.816035695536436,
	0.792804743335147,
	0.770235131248181,
	0.75
};
static const double *sapi_pitchtab = sapi_pitchtab_foo + 10;

static double
convert_sapi_rate(int r)
{
	if (r < -10)
		r = -10;
	else if (r > 10)
		r = 10;
	return sapi_ratetab[r];
}

static double
convert_sapi_pitch(int p)
{
	if (p == -24)
		return 0.5;
	else if (p == 24)
		return 2.0;
	else {
		if (p < -10)
			p = -10;
		else if (p > 10)
			p = 10;
		return sapi_pitchtab[p];
	}
}

static void
set_local_prosody(cst_item *t, const SPVSTATE *st)
{
	int r, p;

	if (st->EmphAdj) /* This doesn't really work, though */
		item_set_int(t, "EMPH", st->EmphAdj);
	if ((r = st->RateAdj))
		item_set_float(t, "local_duration_stretch",
			       convert_sapi_rate(r));
	if ((p = st->PitchAdj.MiddleAdj))
		item_set_float(t, "local_f0_shift",
			       convert_sapi_pitch(p));
	/* We could do something with RangeAdj, but SAPI apparently
           doesn't use it yet and the documentation doesn't say what
           the value passed in it would mean, anyway. */
}

void
CFliteTTSEngineObj::speak_frag()
{
	cst_tokenstream *ts;
	const char *token;
	cst_item *t;
	char *text;
	size_t len;
	// Dinesh 28 December 2017
	// this should be removed once Kannada voice mahaprana is fixed
	wchar_t * tempText = new wchar_t[curr_frag->ulTextLen +2];
	ULONG i = 0;

	for (; i < curr_frag->ulTextLen; i++)
	{
		switch (curr_frag->pTextStart[i]) {
		case 0XCA0:
			tempText[i] = 0XC9F;
			break;

		case 0XCA2:
			tempText[i] = 0XCA1;
			break;

		case 0XC9D:
			tempText[i] = 0XC9C;
			break;
		
		case 0x0964:  // Danda
			tempText[i] = 0X002E; // replace with a period
			// Make sure there are no proceeding spaces
			int j;
			for (j = i - 1; j > 0 && tempText[j] == 0x0020; j--) {
				tempText[j] = tempText[j + 1];
				tempText[j + 1] = 0x0020;
			}
			break;

		default:
			tempText[i] = curr_frag->pTextStart[i];
			break;
		}
	}
	tempText[i+1] = 0x0000;

        // Sai Krishna + Tim 13 March 2017

	//len = wcstombs(NULL, curr_frag->pTextStart, curr_frag->ulTextLen);
	//text = cst_alloc(char, len+1);
	//wcstombs(text, curr_frag->pTextStart, curr_frag->ulTextLen);

        len = WideCharToMultiByte(CP_UTF8, 0, tempText, curr_frag->ulTextLen, NULL, 0, NULL, NULL);
        text = cst_alloc(char, len + 1);
        WideCharToMultiByte(CP_UTF8, 0, tempText , curr_frag->ulTextLen, text, len, NULL, NULL);
        text[len] = 0x00;

        /////////////////////////
 
		delete[] tempText;
		tempText = NULL;
	
		ts = my_ts_open_string(curr_utt, text);
	cst_free(text);

	while (!ts_eof(ts)) {
                // Sai Krishna 14 June 2017. This needs to be modified. Earlier change to ts_getc() didnt help. 
		token = ts_get(ts);
		if (relation_head(tok_rel) && utt_break(ts, token, tok_rel)) {
			get_actions_and_do_them();
			if (aborted)
				goto pod_bay_doors;
			if (sentence_skip == 0) {
				send_sentence_event(ts->token_pos
						    + curr_frag->ulTextSrcOffset);
				synth_one_utt();
				start_new_utt();
			} else {
				--sentence_skip;
				site->CompleteSkip(1);
				start_new_utt();
			}
		}

		t = append_new_token(ts, token, tok_rel);
		item_set_int(t, "token_pos",
			     ts->token_pos + curr_frag->ulTextSrcOffset);
		item_set_int(t, "token_length", strlen(token));
		set_local_prosody(t, &curr_frag->State);
	}
pod_bay_doors: /* Open the POD bay doors, KAL... */
	ts_close(ts);
}

void
CFliteTTSEngineObj::spell_frag()
{
	int i;

	for (i = 0; i < curr_frag->ulTextLen; ++i) {
		cst_item *t;
		char *c;
		int len;

		if (!iswspace(curr_frag->pTextStart[i])) {
						len = WideCharToMultiByte(CP_UTF8, 0, curr_frag->pTextStart + i, 1, NULL, 0, NULL, NULL);
					c= cst_alloc(char, len + 1);
					WideCharToMultiByte(CP_UTF8, 0, curr_frag->pTextStart + i , 1, c, len, NULL, NULL);
					c[len] = 0x00;

		t = append_new_token(NULL, c, tok_rel);
			item_set_int(t, "token_pos", curr_frag->ulTextSrcOffset + i);
			item_set_int(t, "token_length", 1 );
			set_local_prosody(t, &curr_frag->State);
			cst_free(c);
		}
	}
}

void
CFliteTTSEngineObj::pronounce_frag()
{
	cst_val *phones;
	cst_item *t;

	if (!pronouncefunc
	    || (phones = (*pronouncefunc)(curr_frag->State.pPhoneIds)) == NULL)
		return;

	t = append_new_token(NULL, "", tok_rel);
	item_set_int(t, "token_pos", curr_frag->ulTextSrcOffset);
	item_set_int(t, "token_length", curr_frag->ulTextLen);
	set_local_prosody(t, &curr_frag->State);
	item_set(t, "phones", phones);
}

void
CFliteTTSEngineObj::silence_frag()
{
	int sps, silence;
	short *buf;
	ULONG b;

	/* Force an utterance break */
	synth_one_utt();
	start_new_utt();

	sps = get_param_int(curr_vox->features, "sample_rate", 16000);
	silence = sps * curr_frag->State.SilenceMSecs / 1000;
	buf = cst_alloc(short, 512);
	while (silence > 0) {
		b = ((silence < 512) ? silence : 512) * sizeof(*buf);
		site->Write(buf, b, &b);
		bcount += b;
		silence -= (b / sizeof(*buf));
	}
}

void
CFliteTTSEngineObj::set_bookmark()
{
	size_t len;
	char *bookmark;
	cst_item *t;

	len = wcstombs(NULL, curr_frag->pTextStart, curr_frag->ulTextLen);
	bookmark = cst_alloc(char, len+1);
	wcstombs(bookmark, curr_frag->pTextStart, curr_frag->ulTextLen);

	t = relation_append(tok_rel, NULL);
	item_set_string(t, "name", "");
	item_set_string(t, "bookmark", bookmark);
	item_set_int(t, "token_pos", curr_frag->ulTextSrcOffset);
	item_set_int(t, "token_length", curr_frag->ulTextLen);
	cst_free(bookmark);
}

void
CFliteTTSEngineObj::get_actions_and_do_them()
{
	DWORD actions;

	actions = site->GetActions();
	if (actions & SPVES_ABORT) {
		aborted = 1;
		return;
	}
	if (actions & SPVES_SKIP) {
		SPVSKIPTYPE stype;
		long count;

		site->GetSkipInfo(&stype, &count);
		if (stype == SPVST_SENTENCE) {
			sentence_skip += count;
		}
	}
	if (actions & SPVES_RATE) {
		long adj;

		site->GetRate(&adj);
		utt_set_feat_float(curr_utt, "duration_stretch",
				   convert_sapi_rate(adj));
		feat_set_float(curr_vox->features, "duration_stretch",
			       convert_sapi_rate(adj));
	}
	if (actions & SPVES_VOLUME) {
		USHORT adj;

		site->GetVolume(&adj);
		volume = adj;
	}
}

STDMETHODIMP
CFliteTTSEngineObj::Speak(DWORD dwSpeakFlags,
			  REFGUID rguidFormatId,
			  const WAVEFORMATEX * pWaveFormatEx,
			  const SPVTEXTFRAG* pTextFragList,
			  ISpTTSEngineSite* pOutputSite)
{
	char OutputString[500];
	if (curr_vox == NULL)
		return E_POINTER;

	start_new_utt();
	bcount = 0;
	sentence_start = 0;
	sentence_skip = 0;
	aborted = 0;
	volume = 100;
	site = pOutputSite;

	for (curr_frag = pTextFragList; curr_frag && !aborted;
	     curr_frag = curr_frag->pNext) {
		get_actions_and_do_them();

		switch (curr_frag->State.eAction) {
		case SPVA_Speak:
			speak_frag();
			break;
		case SPVA_SpellOut:
			spell_frag();
			break;
		case SPVA_Pronounce:
			pronounce_frag();
			break;
		case SPVA_Silence:
			silence_frag();
			break;
		case SPVA_Bookmark:
			set_bookmark();
			break;
		default:
			break;
		}
	}

	if (tok_rel && relation_tail(tok_rel)) {
		send_sentence_event(item_feat_int(relation_tail(tok_rel), "token_pos")
			              + item_feat_int(relation_tail(tok_rel), "token_length"));
	}

	synth_one_utt();

	delete_utterance(curr_utt);
	curr_utt = NULL;
	tok_rel = NULL;
	site = NULL;

	return S_OK;
}

const char *version_string = "1.0.0.1";

/* extern "C" {
	// Initializes the TTS engine and returns whether initialization succeeded
	__declspec(dllexport) H2R_Speak_ERROR init(int *synthDoneCBPtr, char* voxdir )
	{

		// First make sure we receive the data directory. That's very crucial.
		flite_voxdir_path = voxdir;

		ttsSynthDoneCBPointer = synthDoneCBPtr;
		flite_init();
//		setVoiceList();
//		if (loadedVoices == NULL)
//		{
//			LOGE("TTSEngine::init Could not load voice list");
//			return ANDROID_TTS_FAILURE;
//		}
//		currentVoice = loadedVoices->GetCurrentVoice();
//		if (currentVoice == NULL)
//		{
//			LOGE("TTSEngine::init Voice list error");
//			return ANDROID_TTS_FAILURE;
//		}
//		if (currentVoice->GetFliteVoice() == NULL) {
//			return ANDROID_TTS_FAILURE;
//		}
//		ttsAbort = 0;
//		LOGI("TtsEngine::init done");
		return EE_OK;
	}

*/
extern "C" {
	__declspec(dllexport) H2R_Speak_ERROR H2R_Speak_synthesizeText(char *text)
	{
		char OutputString[2000];
		sprintf_s(OutputString, 1900, "[TRW] H2R_Speak_synthesizeText text = %s", text);
		OutputDebugString(OutputString);
		cst_voice* flite_voice;
		if (currentVoice == NULL)
		{
			OutputDebugStringA("[TRW] H2R_Speak_synthesizeText: currentVoice = NULL");
			return EE_INTERNAL_ERROR;
		}
		flite_voice = curr_vox;
		if (flite_voice == NULL)
		{
			OutputDebugStringA("[TRW] H2R_Speak_synthesizeText: flite_voice = NULL");
			return EE_INTERNAL_ERROR;
		}
		OutputDebugString("[TRW] H2R_Speak_synthesizeText: found currentVoice flite_voice  setting ttsAbort = 0");
			ttsAbort = 0;
			cst_audio_streaming_info *asi;
			asi = new_audio_streaming_info();
			asi->min_buffsize = 100;
			asi->asc = fliteCallback;
			asi->userdata = NULL;
			feat_set(flite_voice->features,
				"streaming_info",
				audio_streaming_info_val(asi));

			// SSML support 
			if (!strncmp(text, "<?xml version=\"1.0\"?>", 21))
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

			char* padded_text = reinterpret_cast<char*>(malloc(strlen(text) + 2));
			snprintf(padded_text, strlen(text) + 2, "%s\n\n", text);

			if ((ts = ts_open_string(padded_text,
				get_param_string(flite_voice->features, "text_whitespace", cst_ts_default_whitespacesymbols),
				get_param_string(flite_voice->features, "text_singlecharsymbols", cst_ts_default_singlecharsymbols),
				get_param_string(flite_voice->features, "text_prepunctuation", cst_ts_default_prepunctuationsymbols),
				get_param_string(flite_voice->features, "text_postpunctuation", cst_ts_default_postpunctuationsymbols))) == NULL) {

				free(padded_text);
				OutputDebugStringA("[TRW] H2R_Speak_synthesizeText: ts_open_string returned NULL");
				return EE_INTERNAL_ERROR;
			}

			free(padded_text);
			OutputDebugString("[TRW] H2R_Speak_synthesizeText: calling flite_ts_to_speech");
			flite_ts_to_speech(ts,
				flite_voice,
				"stream");

				// cst_utterance *u = flite_synth_text(text,flite_voice);
				// delete_utterance(u);

			feat_remove(flite_voice->features, "streaming_info");

			// // Mark synthesis as done
			// size_t padding_length = 8000;
			// int8_t* paddingWave = new int8_t[padding_length]; // Half a second
			// for(int i=0;i<(int)padding_length;i++)
			//   paddingWave[i] = 0;
			// LOGE("Finalizing TTS");
			// uint32_t rate = getSampleRate(NULL);
			// ttsSynthDoneCBPointer(&asi->userdata, rate, ANDROID_TTS_AUDIO_FORMAT_PCM_16_BIT, 1, &paddingWave, &padding_length,
			// 		      ANDROID_TTS_SYNTH_DONE);
			
			OutputDebugStringA("[TRW] H2R_Speak_synthesizeText: return SUCCESS");
			return EE_OK;
	}

//	__declspec(dllexport) H2R_Speak_ERROR H2R_Speak_Synth(const void *text, size_t size,
//		unsigned int position,
//		H2R_Speak_POSITION_TYPE position_type,
//		unsigned int end_position, unsigned int flags,
//		unsigned int *unique_identifier, void *user_data)
//	{
//		return status_to_espeak_error(Speak(dwSpeakFlags, unique_identifier, pWaveFormatEX, pTextFragList, pOutputSite)
//			text, size, position, position_type, end_position, flags, unique_identifier, user_data));
//		return EE_OK;
//	}

	__declspec(dllexport) const char *H2R_Speak_Info(const char **ptr)
	{
		if (ptr != NULL)
			*ptr = "";
		return version_string;
	}
}
extern "C" {
	__declspec(dllexport) H2R_Speak_ERROR H2R_Speak_SetVoice(char *lang, char *voxfile)
	{
		H2R_Speak_ERROR hr;

		CSpDynamicString voxdir;
		const char *avoxdir;
		const cst_val *ttwv;
		char OutputString[500];
		sprintf_s(OutputString, 490, "[TRW] H2R_Speak_SetVoice entered:  lang = %s  voxfile = %s ", lang, voxfile);
		OutputDebugStringA(OutputString);
		avoxdir = voxfile;


		if (loadedVoices == NULL) {
			OutputDebugStringA("[TRW] H2R_Speak_SetVoice voices not loaded returning error");
			return EE_INTERNAL_ERROR;
		}

		currentVoice = loadedVoices->GetVoiceForLocale(lang, voxfile);
		if (currentVoice  == NULL) {
			OutputDebugStringA("[TRW] H2R_Speak_SetVoice can not find voice returning error");
			return EE_INTERNAL_ERROR; // or something 
		}

		if ((ttwv = feat_val(currentVoice->GetFliteVoice()->features, "tokentowords_func"))) {
			OutputDebugStringA("[TRW] H2R_Speak_SetVoice ttwv is not false");
			feat_set(currentVoice->GetFliteVoice()->features, "old_tokentowords_func", ttwv);
			feat_set(currentVoice->GetFliteVoice()->features, "tokentowords_func", itemfunc_val(sapi_tokentowords));
		}
//		loadedVoices->GetCurrentVoice()->SetFliteVoice(curr_vox);
		curr_vox = currentVoice->GetFliteVoice();

		OutputDebugStringA("[TRW] H2R_Speak_SetVoice returning EE_OK");
		return EE_OK;
	}
}
/*
	__declspec(dllexport) H2R_Speak_ERROR (H2R_Speak_VOICE *voice_spec)
	{
		// An espeak_VOICE structure is used to pass criteria to select a voice.  Any of the following
		//	fields may be set:
		//		name     NULL, or a voice name
		//		languages  NULL, or a single language string (with optional dialect), eg. "en-uk", or "en"
		//		gender   0=not specified, 1=male, 2=female
		//		age      0=not specified, or an age in years
		//		variant  After a list of candidates is produced, scored and sorted, "variant" is used to index
		//			that list and choose a voice.
		//			variant=0 takes the top voice (i.e. best match). variant=1 takes the next voice, etc
		//
		return(EE_INTERNAL_ERROR);
	}
*/
extern "C" {
	__declspec(dllexport) int H2R_Speak_GetParameter(const char parameter, int current)
	{
		int value;
		char OutputString[500];
		sprintf_s(OutputString, 490, "[TRW] H2R_Speak_GetParametern called:  Parameter = %d current = %d", parameter, current);
		OutputDebugStringA(OutputString);
		
		//!current == default
		if (!current && parameter < 5) {
			return (param_defaults[parameter]);
		}
		switch (parameter)
		{
		case H2R_SpeakRATE:
			value = embedded_value[EMBED_S];
//			embedded_value[EMBED_S2] = new_value;
			break;
		case H2R_SpeakVOLUME:
			value = embedded_value[EMBED_A];
			break;
		case H2R_SpeakPITCH:
			value = embedded_value[EMBED_P];
			break;
		case H2R_SpeakRANGE:
			value = embedded_value[EMBED_R];
			break;
			//		case H2R_SpeakLINELENGTH:
			//			value = option_linelength;
			//			break;
//		case H2R_SpeakWORDGAP:
			//			value = option_wordgap;
//			break;
//		case H2R_SpeakINTONATION:
			//			if ((new_value & 0xff) != 0)
			//			translator->langopts.intonation_group = new_value & 0xff;
			//			option_tone_flags = new_value;
//			break;
		default:
			OutputDebugStringA("[TRW] H2R_Speak_GetParameter returning EE_INTERNAL ERROR");
			return EE_INTERNAL_ERROR;
		}
		sprintf_s(OutputString, 490, "[TRW] H2R_Speak_GetParameter returning %d", value);
		OutputDebugStringA(OutputString);
		return value;
	}
}

extern "C" {
	__declspec(dllexport) H2R_Speak_ERROR H2R_Speak_SetParameter(const char parameter, int value, int relative)
	{
		OutputDebugStringA("[TRW] H2R_Speak_SetParameter called");
		// parameter: reset-all, amp, pitch, speed, linelength, expression, capitals, number grouping
		// relative 0=absolute  1=relative
			int new_value = value;
		int default_value;
		char OutputString[500];
		if (relative) {
			OutputDebugString("[TRW] H2R_Speak_SetParameter relative is TRUE");
			if (parameter < 5) {
			default_value = param_defaults[parameter];
			sprintf_s(OutputString, 490, "[TRW] H2R_Speak_SetParameter setting parameter %d, %d to %d", parameter, value, default_value + (default_value * value) / 100);
			OutputDebugStringA(OutputString);
				new_value = default_value + (default_value * value) / 100;
			}
		}
//		param_stack[0].parameter[parameter] = new_value;
//		saved_parameters[parameter] = new_value;

		switch (parameter)
		{
		case H2R_SpeakRATE:
			sprintf_s(OutputString, 490, "[TRW] H2R_Speak_setParameter H2R_SpeakRATE = %d", new_value);
			OutputDebugStringA(OutputString);
			embedded_value[EMBED_S] = new_value;
			embedded_value[EMBED_S2] = new_value;
			setSpeechRate(new_value);
//			SetSpeed(3);
			break;
		case H2R_SpeakVOLUME:
			sprintf_s(OutputString, 490, "[TRW] H2R_Speak_setParameter H2R_SpeakVOLUME = %d", new_value);
			OutputDebugStringA(OutputString);
			embedded_value[EMBED_A] = new_value;
//			GetAmplitude();
			break;
		case H2R_SpeakPITCH:
			if (new_value > 99) new_value = 99;
			if (new_value < 0) new_value = 0;
			sprintf_s(OutputString, 490, "[TRW] H2R_Speak_setParameter H2R_SpeakPITCH = %d", new_value);
			OutputDebugStringA(OutputString);
			embedded_value[EMBED_P] = new_value;
			break;
		case H2R_SpeakRANGE:
			if (new_value > 99) new_value = 99;
			embedded_value[EMBED_R] = new_value;
			break; 
		case H2R_SpeakINTONATION:
//			if ((new_value & 0xff) != 0)
//			translator->langopts.intonation_group = new_value & 0xff;
//			option_tone_flags = new_value;
		break;
		default:
			return EE_INTERNAL_ERROR;
		}
		return EE_OK;
	}
}
/*
	__declspec(dllexport) H2R_Speak_ERROR H2R_Speak_Terminate()
	{
		return(EE_INTERNAL_ERROR);
	}
	__declspec(dllexport) H2R_Speak_ERROR H2R_Speak_ListVoices()
	{
		return(EE_INTERNAL_ERROR);
	}
	__declspec(dllexport) H2R_Speak_ERROR H2R_Speak_GetCurrentVoice()
	{
		return(EE_INTERNAL_ERROR);
	}
*/


// Initializes the TTS engine and returns whether initialization succeeded
extern "C" { 
	__declspec(dllexport) H2R_Speak_tts_result_t H2R_Speak_init(const char *H2R_Speak_Path, int (*synthDoneCBPtr)(short *, int)) {
		OutputDebugString("[TRW] H2R_Speak_init entered");
		return(H2RSpeak_init(H2R_Speak_Path, synthDoneCBPtr));
	}

}
