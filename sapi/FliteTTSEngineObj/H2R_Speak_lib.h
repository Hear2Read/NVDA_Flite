#ifndef H2R_SPEAK_LIB_H
#define H2R_SPEAK_LIB_H
/***************************************************************************
 *   Copyright (C) 2021 by Hear2Read                                       *
 *   email: info@hear2read.org                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see:                                 *
 *               <http://www.gnu.org/licenses/>.                           *
 ***************************************************************************/


/****************************************************************/
/* This is the header file for the library version of H2R_Speak */
/*                                                              */
/****************************************************************/

#include <stdio.h>
#include <stddef.h>

#define H2R_SPEAK_API_REVISION  1
/*	Version 1 -- initial code
*/

#if defined(_WIN32) || defined(_WIN64)
#define H2R_SPEAK_API extern __declspec(dllexport)
#else
#define H2R_SPEAK_API
#endif

typedef enum {
	espeakEVENT_LIST_TERMINATED = 0, // Retrieval mode: terminates the event list.
	espeakEVENT_WORD = 1,            // Start of word
	espeakEVENT_SENTENCE = 2,        // Start of sentence
	espeakEVENT_MARK = 3,            // Mark
	espeakEVENT_PLAY = 4,            // Audio element
	espeakEVENT_END = 5,             // End of sentence or clause
	espeakEVENT_MSG_TERMINATED = 6,  // End of message
	espeakEVENT_PHONEME = 7,         // Phoneme, if enabled in espeak_Initialize()
	espeakEVENT_SAMPLERATE = 8       // internal use, set sample rate
} H2R_Speak_EVENT_TYPE;



typedef struct {
	H2R_Speak_EVENT_TYPE type;
	unsigned int unique_identifier; // message identifier (or 0 for key or character)
	int text_position;    // the number of characters from the start of the text
	int length;           // word length, in characters (for espeakEVENT_WORD)
	int audio_position;   // the time in mS within the generated speech output data
	int sample;           // sample id (internal use)
	void* user_data;      // pointer supplied by the calling program
	union {
		int number;        // used for WORD and SENTENCE events.
		const char *name;  // used for MARK and PLAY events.  UTF8 string
		char string[8];    // used for phoneme names (UTF8). Terminated by a zero byte unless the name needs the full 8 bytes.
	} id;
} H2R_Speak_EVENT;
/*
   When a message is supplied to speak the request is buffered and speak returns. When the message is really processed, the callback function will be repetedly called.


   In RETRIEVAL mode, the callback function supplies to the calling program the audio data and an event list terminated by 0 (LIST_TERMINATED).

   In PLAYBACK mode, the callback function is called as soon as an event happens.

   For example suppose that the following message is supplied to espeak_Synth:
   "hello, hello."


   * Once processed in RETRIEVAL mode, it could lead to 3 calls of the callback function :

   ** Block 1:
   <audio data> +
   List of events: SENTENCE + WORD + LIST_TERMINATED

   ** Block 2:
   <audio data> +
   List of events: WORD + END + LIST_TERMINATED

   ** Block 3:
   no audio data
   List of events: MSG_TERMINATED + LIST_TERMINATED


   * Once processed in PLAYBACK mode, it could lead to 5 calls of the callback function:

   ** SENTENCE
   ** WORD (call when the sounds are actually played)
   ** WORD
   ** END (call when the end of sentence is actually played.)
   ** MSG_TERMINATED


   The MSG_TERMINATED event is the last event. It can inform the calling program to clear the user data related to the message.
   So if the synthesis must be stopped, the callback function is called for each pending message with the MSG_TERMINATED event.

   A MARK event indicates a <mark> element in the text.
   A PLAY event indicates an <audio> element in the text, for which the calling program should play the named sound file.
*/



typedef enum {
	POS_CHARACTER = 1,
	POS_WORD,
	POS_SENTENCE
} H2R_Speak_POSITION_TYPE;


typedef enum {
	/* PLAYBACK mode: plays the audio data, supplies events to the calling program*/
	AUDIO_OUTPUT_PLAYBACK,

	/* RETRIEVAL mode: supplies audio data and events to the calling program */
	AUDIO_OUTPUT_RETRIEVAL,

	/* SYNCHRONOUS mode: as RETRIEVAL but doesn't return until synthesis is completed */
	AUDIO_OUTPUT_SYNCHRONOUS,

	/* Synchronous playback */
	AUDIO_OUTPUT_SYNCH_PLAYBACK

} H2R_Speak_AUDIO_OUTPUT;


typedef enum {
	EE_OK = 0,
	EE_INTERNAL_ERROR = -1,
	EE_BUFFER_FULL = 1,
	EE_NOT_FOUND = 2
} H2R_Speak_ERROR;

// voice table
typedef struct {
	const char *name;      // a given name for this voice. UTF8 string.
	const char *languages;       // list of pairs of (byte) priority + (string) language (and dialect qualifier)
	const char *identifier;      // the filename for this voice within espeak-ng-data/voices
	unsigned char gender;  // 0=none 1=male, 2=female,
	unsigned char age;     // 0=not specified, or age in years
	unsigned char variant; // only used when passed as a parameter to espeak_SetVoiceByProperties
	unsigned char xx1;     // for internal use
	int score;       // for internal use
	void *spare;     // for internal use
} H2R_Speak_VOICE;

typedef enum {
	H2R_SpeakSILENCE = 0, /* internal use */
	H2R_SpeakRATE = 1,
	H2R_SpeakVOLUME = 2,
	H2R_SpeakPITCH = 3,
	H2R_SpeakRANGE = 4,
	H2R_SpeakPUNCTUATION = 5,
	H2R_SpeakCAPITALS = 6,
	H2R_SpeakWORDGAP = 7,
	H2R_SpeakOPTIONS = 8,   // reserved for misc. options.  not yet used
	H2R_SpeakINTONATION = 9,
	H2R_SpeakRESERVED1 = 10,
	H2R_SpeakRESERVED2 = 11,

//	espeakEMPHASIS,   /* internal use */
//	espeakLINELENGTH, /* internal use */
//	espeakVOICETYPE,  // internal, 1=mbrola
	N_SPEECH_PARAM    /* last enum */
} H2R_Speak_PARAMETER;


// embedded command numbers
#define EMBED_P     1 // pitch
#define EMBED_S     2 // speed (used in setlengths)
#define EMBED_A     3 // amplitude/volume
#define EMBED_R     4 // pitch range/expression
#define EMBED_H     5 // echo/reverberation
#define EMBED_T     6 // different tone for announcing punctuation (not used)
#define EMBED_I     7 // sound icon
#define EMBED_S2    8 // speed (used in synthesize)
#define EMBED_Y     9 // say-as commands
#define EMBED_M    10 // mark name
#define EMBED_U    11 // audio uri
#define EMBED_B    12 // break
#define EMBED_F    13 // emphasis
#define EMBED_C    14 // capital letter indication

#define N_EMBEDDED_VALUES    15
extern int embedded_value[N_EMBEDDED_VALUES];
extern int embedded_default[N_EMBEDDED_VALUES];


#endif