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
/*  Library classes to manage available flite voices                     */
/*                                                                       */
/*************************************************************************/

#include "H2R_Speak_flite_voice.h"
#include <stdint.h>

extern "C" cst_voice *REGISTER_VOX(const char *);
extern FliteEngine::Voice* currentVoice;

namespace FliteEngine {

	String::String(const char* str) {
		if (str != NULL) {
			str_data_ = new char[strlen(str) + 1];
			if (str_data_ != NULL)
				snprintf(str_data_, strlen(str) + 1, "%s", str);
		}
		else {
			str_data_ = NULL;
		}
	}

	String::String(const String& rhs) {
		if (rhs.str_data_ == NULL) {
			str_data_ = NULL;
		}
		else {
			str_data_ = new char[strlen(rhs.str_data_) + 1];
			if (str_data_ != NULL)
				snprintf(str_data_, strlen(rhs.str_data_) + 1, "%s", rhs.str_data_);
		}
	}

	String::~String() {
		if (str_data_ != NULL) {
			delete[] str_data_;
		}
	}

	String& String::operator=(const String &rhs) {
		// Don't assign if rhs is the same object!
		if (this != &rhs) {
			if (str_data_ != NULL)
				delete[] str_data_;
			str_data_ = new char[strlen(rhs.str_data_) + 1];
			if (str_data_ != NULL)
				snprintf(str_data_, strlen(rhs.str_data_) + 1, "%s", rhs.str_data_);
		}
		return *this;
	}

	String String::operator+(const String &other) {
		int newlength;
		char* s;

		if (str_data_ == NULL)
			return other;

		if (other.str_data_ == NULL)
			return *this;

		newlength = strlen(str_data_) + strlen(other.str_data_) + 1;

		s = new char[newlength];
		snprintf(s, newlength, "%s%s", str_data_, other.str_data_);

		String addedStr = String(s);
		delete[] s;
		return addedStr;
	}

	bool String::operator==(const String &other) const {
		if ((str_data_ == NULL) || (other.str_data_ == NULL))
			return false;

		if (strcmp(str_data_, other.str_data_) == 0)
			return true;
		else
			return false;
	}

	const char* String::c_str() {
		return str_data_;
	}


	const char* Voice::GetLanguage() {
		return language_.c_str();
	}

	const char* Voice::GetCountry() {
		return country_.c_str();
	}

	const char* Voice::GetVariant() {
		return variant_.c_str();
	}

	const int Voice::GetSampleRate() {
		int rate = 16000;
		if (flite_voice_ != NULL) {
			rate = flite_get_param_int(flite_voice_->features, "sample_rate", rate);
		}

		return rate;
	}
	const float Voice::GetWavRescale() {
		return wav_rescale_;
	}

	cst_voice* Voice::GetFliteVoice() {
		return flite_voice_;
	}

	bool Voice::IsSameLocaleAs(String flang, String fcountry, String fvar) {
		return (language_ == flang) &&
			(country_ == fcountry) &&
			(variant_ == fvar);
	}

	LinkedVoice::LinkedVoice(const String flang, const String fcountry, const String fvar,
							 t_voice_register_function freg, t_voice_unregister_function funreg) {
		language_ = flang;
		country_ = fcountry;
		variant_ = fvar;
		flite_voice_ = NULL;
		voice_register_function_ = freg;
		voice_unregister_function_ = funreg;
		char OutputString[500];
		sprintf_s(OutputString, 498, "[TRW] LinkedVoice creating enry for lang %s country %s", flang, fcountry);
		OutputDebugStringA(OutputString);
	}

	LinkedVoice::~LinkedVoice() {
		UnregisterVoice();
	}

	H2R_Speak_tts_support_result_t LinkedVoice::GetLocaleSupport(String flang, const char *flitevoxFileName) {
		H2R_Speak_tts_support_result_t support = H2R_SPEAK_TTS_LANG_NOT_SUPPORTED;
		if (language_ == flang) {

			support = H2R_SPEAK_TTS_LANG_COUNTRY_VAR_AVAILABLE;
		}
		char OutputString[500];
		sprintf_s(OutputString, 498, "[TRW] GetLocalSupport: flang = %s language_ = %s returning %d", flang, language_, support);
		OutputDebugStringA(OutputString);
		return support;
	}


	cst_voice* LinkedVoice::RegisterVoice() {
		flite_voice_ = voice_register_function_(flite_voxdir_path);
		return flite_voice_;
	}

	void LinkedVoice::UnregisterVoice() {
		if (flite_voice_ == NULL) return;  // Voice not registered
		voice_unregister_function_(flite_voice_);
		flite_voice_ = NULL;
	}

	ClustergenVoice::ClustergenVoice() {
		flite_voice_ = NULL;
	}

	ClustergenVoice::~ClustergenVoice() {

		if (flite_voice_ != NULL) {
			// We have something loaded in there. Let's unregister it.
			UnregisterVoice();
		}
	}

	void ClustergenVoice::UnregisterVoice() {
		if (flite_voice_ != NULL) {
			// TODO(aup): Flite 1.5.6 does not support unregistering a linked voice.
			// We do nothing here, but there is potential memory issue.

			// cst_cg_unload_voice(flite_voice_);
			// LOGD("Flite voice unregistered.");
			flite_voice_ = NULL;
			language_ = "";
			country_ = "";
			variant_ = "";
		}
	}

	//	String GetDefaultVariantInCountryDirectory(String dirname) {
	//		int return_code;
	//		DIR *dir;
	//		struct dirent *entry;
	//		struct dirent *result;
	//
	//		if ((dir = opendir(dirname.c_str())) == NULL) {
	//			return "";
	//		}
	//		else {
	//			entry = (struct dirent *) malloc(
	//				pathconf(dirname.c_str(), _PC_NAME_MAX) + 1);
	//			for (return_code = readdir_r(dir, entry, &result); //TODO: readdir deprecated
	//				result != NULL && return_code == 0;
	//				return_code = readdir_r(dir, entry, &result)) {
	//				if (entry->d_type == DT_REG) {
	//					if (strstr(entry->d_name, "cg.flitevox") ==
	//						(entry->d_name + strlen(entry->d_name) - 11)) {
	//						char *tmp = new char[strlen(entry->d_name) - 11];
	//						strncpy(tmp, entry->d_name, strlen(entry->d_name) - 12);
	//						tmp[strlen(entry->d_name) - 12] = '\0';
	//						String ret = String(tmp);
	//						delete[] tmp;
	//						closedir(dir);
	//						return ret;
	//					}
	//				}
	//			}
	//		}
	//		return "";
	//	}

	//	String GetDefaultCountryInLanguageDirectory(String dirname) {
	//		int return_code;
	//		DIR *dir;
	//		struct dirent *entry;
	//		struct dirent *result;
	//		String default_voice;
	//
	//		if ((dir = opendir(dirname.c_str())) == NULL) {
	//			return "";
	//		}
	//		else {
	//			entry = (struct dirent *) malloc(
	//				pathconf(dirname.c_str(), _PC_NAME_MAX) + 1);
	//			for (return_code = readdir_r(dir, entry, &result);
	//				result != NULL && return_code == 0;
	//				return_code = readdir_r(dir, entry, &result)) {
	//				if (strcmp(entry->d_name, ".") == 0 ||
	//					strcmp(entry->d_name, "..") == 0)
	//					continue;
	//				if (entry->d_type == DT_DIR) {
	//					String newdir = dirname + "/" + String(entry->d_name);
	//					default_voice = GetDefaultVariantInCountryDirectory(newdir);
	//					if (!(default_voice == "")) {
	//						String ret = String(entry->d_name);
	//						closedir(dir);
	//						return ret;
	//					}
	//				}
	//			}
	//		}
	//		return "";
	//	}

	bool FileExists(String filename) {
		FILE *fd;
		fd = fopen(filename.c_str(), O_RDONLY);
		if (fd < 0)
			return false;
		fclose(fd);
		return true;
	}

	// Check that the required clustergen file is present on disk and
// return the information.

	H2R_Speak_tts_support_result_t ClustergenVoice::GetLocaleSupport(String flang, const char *flitevoxFileName) {

		H2R_Speak_tts_support_result_t
			language_support = H2R_SPEAK_TTS_LANG_NOT_SUPPORTED;

		String path;
		path = flitevoxFileName;
		if (FileExists(path)) {
			language_support = H2R_SPEAK_TTS_LANG_COUNTRY_VAR_AVAILABLE;
		}
		return language_support;
	}

	H2R_Speak_tts_result_t ClustergenVoice::SetLanguage(String flang, const char *flitevoxFileName) {
		// But check that the current voice itself isn't being requested.
		if (language_ == flang) {
			return H2R_SPEAK_TTS_SUCCESS;
		}

		// If some voice is already loaded, unload it.
		UnregisterVoice();

		H2R_Speak_tts_support_result_t language_support;
		language_support = H2R_SPEAK_TTS_LANG_COUNTRY_VAR_AVAILABLE;
		String path = flite_voxdir_path;

		//		if (languageSupported(flang)) {
		//			path = path + "/cg/" + flang + "/" + fcountry + "/" + fvar + ".cg.flitevox";
		//			language_ = flang;
		//			country_ = fcountry;
		//			variant_ = fvar;
		//
		//		}
		//		else if (language_support == H2R_SPEAK_TTS_LANG_COUNTRY_AVAILABLE) {
		//			path = path + "/cg/" + flang + "/" + fcountry;
		//			String var = GetDefaultVariantInCountryDirectory(path);
		//			path = path + "/" + var + ".cg.flitevox";
		//
		//			language_ = flang;
		//			country_ = fcountry;
		//			variant_ = var;
		//		}
		//		else if (language_support == ANDROID_TTS_LANG_AVAILABLE) {
		//			path = path + "/cg/" + flang;
		//			String country = "";
		//			path = path + "/" + country;
		//			String var = "";
		//			path = path + "/" + var + ".cg.flitevox";
		language_ = flang;
		country_ = "";
		variant_ = "";
		//		}
		//		else {
		//			return H2R_SPEAK_TTS_FAILURE;
		//		}

				// Try to load the flite voice given the voxdata file

				// new since fLite 1.5.6
		flite_voice_ = flite_voice_load(path.c_str());

		if (flite_voice_ == NULL) {
			return H2R_SPEAK_TTS_FAILURE;
		}
		else {
		}
		language_ = flang;
		country_ = NULL;
		variant_ = NULL;

		// Print out voice information from the meta-data.
		const char* lang, *country, *gender, *age, *build_date, *desc;

		lang = flite_get_param_string(flite_voice_->features, "language", "");
		country = flite_get_param_string(flite_voice_->features, "country", "");
		gender = flite_get_param_string(flite_voice_->features, "gender", "");
		age = flite_get_param_string(flite_voice_->features, "age", "");
		build_date = flite_get_param_string(flite_voice_->features, "build_date", "");
		desc = flite_get_param_string(flite_voice_->features, "desc", "");

		//		LOGV("      Clustergen voice: Speaker Country: %s", country);
		//		LOGV("      Clustergen voice: Speaker Gender: %s", gender);
		//		LOGV("      Clustergen voice: Speaker Age: %s", age);
		//		LOGV("      Clustergen voice: Voice Build Date: %s", build_date);
		//		LOGV("      Clustergen voice: Voice Description: %s", desc);

		return H2R_SPEAK_TTS_SUCCESS;
	}

	Voices::Voices(int max_count, VoiceRegistrationMode registration_mode) {
		voice_registration_mode_ = registration_mode;
		current_voice_ = NULL;
		voice_list_ = new LinkedVoice*[max_count];
		max_voice_count_ = max_count;
		for (int i = 0; i < max_voice_count_; i++)
			voice_list_[i] = NULL;
		current_voice_count_ = 0;
	}

	Voices::~Voices() {
		if (voice_list_ != NULL) {
			for (int i = 0; i < current_voice_count_; i++)
				if (voice_list_[i] != NULL)
					delete voice_list_[i];  // Delete the individual voices
			delete[] voice_list_;
			voice_list_ = NULL;
		}
	}

	Voice* Voices::GetCurrentVoice() {
		return current_voice_;
	}

	int Voices::voiceExists(const char *flang, const char *fcountry, const char *fvar) {
		for (int i = 0; i < current_voice_count_; i++) {
			if  (voice_list_[i]->GetLanguage() == flang && voice_list_[i]->GetCountry() == fcountry && voice_list_[i]->GetVariant() == fvar) {
				return (TRUE);
			}
		}
		return(FALSE);
	}

	void Voices::AddLinkedVoice(String flang, String fcountry, String fvar,
								t_voice_register_function freg, t_voice_unregister_function funreg) {
		if (current_voice_count_ >= max_voice_count_) {
			return;
		}

		LinkedVoice* v = new LinkedVoice(flang, fcountry, fvar, freg, funreg);
		
		/* We must register this voice if the registration mode
		   so dictates.
		*/
		if (voice_registration_mode_ == ALL_VOICES_REGISTERED)
			v->RegisterVoice();
		voice_list_[current_voice_count_] = v;
		current_voice_count_++;
	}

	void Voices::SetDefaultVoice() {
		if (current_voice_ != NULL)
			if (voice_registration_mode_ == ONLY_ONE_VOICE_REGISTERED) {
				current_voice_->UnregisterVoice();
				current_voice_ = NULL;
			}

		// Try to load CMU_US_RMS. If it doesn't exist,
		// then pick the first linked voice, whichever it is.

//		H2R_Speak_tts_result_t result = clustergen_voice_.SetLanguage("eng",
//			"USA",
//			"male,rms");
//		if (result == H2R_SPEAK_TTS_SUCCESS) {
//			current_voice_ = &clustergen_voice_;
//			return;
//		}
	}

	//	H2R_Speak_tts_support_result_t Voices::IsLocaleAvailable(String flang,
	//		String fcountry,
	//		String fvar) {
	//
	//		H2R_Speak_tts_support_result_t
	//			language_support = H2R_SPEAK_TTS_LANG_NOT_SUPPORTED;
	//
	//		H2R_Speak_tts_support_result_t current_support;
	//
	//		// We need to also look through the cg voices to see if better
	//		// support available there.
	//
	//		current_support = clustergen_voice_.GetLocaleSupport(flang, fcountry, fvar);
	//		if (language_support < current_support)
	//			// we found a better support than we previously knew
	//			language_support = current_support;
	//		return language_support;
	//	}

	Voice* Voices::GetVoiceForLocale(String flang, const char *flitevoxFileName) {

		/* Check that the voice we currently have set doesn't already
		   provide what is requested.
		*/
		if ((current_voice_ != NULL)
			&& (current_voice_->IsSameLocaleAs(flang, NULL, NULL))) {
			OutputDebugStringA("[TRW] GetVoiceForLocal: returing current voice");
			return current_voice_;
		}
		/* If registration mode dictatas that only one voice can be set,
		   this is the right time to unregister currently loaded voice.
		*/
		if ((current_voice_ != NULL)
			&& (voice_registration_mode_ == ONLY_ONE_VOICE_REGISTERED)) {
			current_voice_->UnregisterVoice();
		}
		current_voice_ = NULL;

		Voice* newVoice = NULL;
		H2R_Speak_tts_support_result_t
			language_support = H2R_SPEAK_TTS_LANG_NOT_SUPPORTED;

		H2R_Speak_tts_support_result_t current_support;

		/* First loop over the linked-in voices to gather best available voice. */
		for (int i = 0; i < current_voice_count_; i++) {
			if (voice_list_[i] == NULL) continue;
			current_support = voice_list_[i]->GetLocaleSupport(flang, flitevoxFileName);
			if (language_support < current_support) {
				// We found a better support for language than we previously had.
				newVoice = voice_list_[i];
				language_support = current_support;
			}
			if (language_support == H2R_SPEAK_TTS_LANG_COUNTRY_VAR_AVAILABLE)
				break;  // No point in continuing search if best support is found.
		}

		if (newVoice != NULL) {
			// Something was found in the linked voices.

			current_voice_ = newVoice;
			if (voice_registration_mode_ == ONLY_ONE_VOICE_REGISTERED) {
				cst_voice *v;
				v = REGISTER_VOX(flitevoxFileName);
				current_voice_->SetFliteVoice(v);
			}
			OutputDebugStringA("[TRW] GetVoiceForLocale returning voice");
			return current_voice_;
		}
		else {
			current_voice_ = NULL;
			OutputDebugStringA("[TRW] GetVoiceForLocale returning NULL");
			return current_voice_;
		}
	}
}
