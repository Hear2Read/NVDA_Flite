/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute              */
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
#ifndef H2R_Speak_FLITE_VOICE_H_
#define H2R_Speak_FLITE_VOICE_H_


#include "flite.h"
#include <sys/stat.h>
#include <fcntl.h>

extern char* flite_voxdir_path;  // Path to flite-data directory

typedef enum {
	H2R_SPEAK_TTS_SUCCESS = 0,
	H2R_SPEAK_TTS_FAILURE = -1,
	H2R_SPEAK_TTS_FEATURE_UNSUPPORTED = -2,
	H2R_SPEAK_TTS_VALUE_INVALID = -3,
	H2R_SPEAK_TTS_PROPERTY_UNSUPPORTED = -4,
	H2R_SPEAK_TTS_PROPERTY_SIZE_TOO_SMALL = -5,
	H2R_SPEAK_TTS_MISSING_RESOURCES = -6
} H2R_Speak_tts_result_t;

typedef enum {
	H2R_SPEAK_TTS_SYNTH_DONE = 0,
	H2R_SPEAK_TTS_SYNTH_PENDING = 1
} H2R_Speak_tts_synth_status_t;

typedef enum {
	H2R_SPEAK_TTS_AUDIO_FORMAT_INVALID = -1,
	H2R_SPEAK_TTS_AUDIO_FORMAT_DEFAULT = 0,
	H2R_SPEAK_TTS_AUDIO_FORMAT_PCM_16_BIT = 1,
	H2R_SPEAK_TTS_AUDIO_FORMAT_PCM_8_BIT = 2,
} android_tts_audio_format_t;

typedef enum {
	H2R_SPEAK_TTS_LANG_COUNTRY_VAR_AVAILABLE = 2,
	H2R_SPEAK_TTS_LANG_COUNTRY_AVAILABLE = 1,
	H2R_SPEAK_TTS_LANG_AVAILABLE = 0,
	H2R_SPEAK_TTS_LANG_MISSING_DATA = -1,
	H2R_SPEAK_TTS_LANG_NOT_SUPPORTED = -2
} H2R_Speak_tts_support_result_t;

//typedef enum {
//	H2R_SPEAK_TTS_SYNTH_DONE = 0,
//	H2R_SPEAK_TTS_SYNTH_PENDING = 1
//} H2R_Speak_tts_synth_status_t;

namespace FliteEngine {

		// Function pointer type for flite_register_voice
		typedef cst_voice* (*t_voice_register_function)(const char*);

		// Function pointer type for flite_unregister_voice
		typedef void(*t_voice_unregister_function)(cst_voice* v);

		enum VoiceRegistrationMode {
			ONLY_ONE_VOICE_REGISTERED = 0,
			ALL_VOICES_REGISTERED = 1
		};

		class String {
		private:
			char* str_data_;
		public:
			String(const char* str = NULL);  // NOLINT : Explicit constructor okay.
			String(const String& rhs);
			~String();

			String& operator=(const String &rhs);
			bool operator==(const String &other) const;
			String operator+(const String &other);
			const char* c_str();
		};

		class Voice {
		protected:
			String language_;  // ISO3 language
			String country_;   // ISO3 country
			String variant_;   // Short name of the variant
			float wav_rescale_; // Support for vol increase -Shyam 2018.06.20
			cst_voice*  flite_voice_;  // Pointer to registered flite voice

		public:
			virtual ~Voice() {}
			const char* GetLanguage();
			const char* GetCountry();
			const char* GetVariant();
			const int GetSampleRate();
			const float GetWavRescale();
			// Returns the currently set flite voice.
			// WARNING: This will *NOT* register the voice.
			cst_voice* GetFliteVoice();
			void SetFliteVoice(cst_voice* FliteVoice) { flite_voice_ = FliteVoice; }

			bool IsSameLocaleAs(String flang, String fcountry, String fvar);

			// Returns how far the language request is supported.
			virtual H2R_Speak_tts_support_result_t GetLocaleSupport(String flang,const char *flitevoxFileName) {
				return H2R_SPEAK_TTS_LANG_NOT_SUPPORTED;
			}
	
			virtual void UnregisterVoice() {};
//			virtual cst_voice *RegisterVoice() {};
		};

		class LinkedVoice : public Voice {
		private:
			// Flite function: register voice
			t_voice_register_function voice_register_function_;

			// Flite function: unregister voice
			t_voice_unregister_function voice_unregister_function_;

		public:
			LinkedVoice(String flang, String fcountry, String fvar,
				t_voice_register_function freg,
				t_voice_unregister_function funreg);

			~LinkedVoice();

			cst_voice* RegisterVoice();
			void UnregisterVoice();

			H2R_Speak_tts_support_result_t GetLocaleSupport(String flang, const char *flitevoxFileName);
		};

		class ClustergenVoice : public Voice {
		public:
			ClustergenVoice();
			~ClustergenVoice();

			H2R_Speak_tts_support_result_t GetLocaleSupport(String flang, const char *filtevoxFileName);
			H2R_Speak_tts_result_t SetLanguage(String flang, const char *filtevoxFileName);
			void UnregisterVoice();
		};

		class Voices {
		private:
			LinkedVoice** voice_list_;
			Voice* current_voice_;
			ClustergenVoice clustergen_voice_;
			VoiceRegistrationMode voice_registration_mode_;
			int max_voice_count_;  // Maximum voice list size
			int current_voice_count_;  // Current occupancy of voice list

		public:
			Voices(int max_count, VoiceRegistrationMode registration_mode);
			~Voices();

			Voice* GetCurrentVoice();
			int voiceExists(const char *flang, const char *fcountry, const char *fvar);

			void AddLinkedVoice(String flang, String fcountry, String fvar,
				t_voice_register_function freg,
				t_voice_unregister_function funreg);

			void SetDefaultVoice();

			// Find out if a particular locale is available.
			// H2R_Speak_tts_support_result_t IsLocaleAvailable(String flang,
			//	String fcountry,
			//	String fvar);

			// Register and set the current voice to the one asked for
			Voice* GetVoiceForLocale(String flang, const char *flitevoxFileName);
		};
	};
#endif 