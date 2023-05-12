/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                         Copyright (c) 2015                            */
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
/*  Number pronunciation for (Odia) Indic                             */
/*************************************************************************/

#ifndef _indic_nep_num_table_h_
#define _indic_nep_num_table_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "indic_num_table.h"
static const char * const indic_nep_digit[11][2] =
{
    { "०", "शुन्य" },
    { "१", "एक" },
    { "२", "दुई" },
    { "३", "तीन" },
    { "४", "चार" },
    { "५", "पाँच" },
    { "६", "छ" },
    { "७", "सात" },
    { "८", "आठ" },
    { "९", "नौ" },
    { NULL, NULL },
};

static const char * const indic_nep_two_digit[101][4] =
{
    { "१", "०", "दस", NULL },
    { "१", "१", "एघार", NULL },
    { "१", "२", "बाह्र", NULL },
    { "१", "३", "तेह्र", NULL },
    { "१", "४", "चौध", NULL },
    { "१", "५", "पन्ध्र", NULL },
    { "१", "६", "सोह्र", NULL },
    { "१", "७", "सत्र", NULL },
    { "१", "८", "अठार", NULL },
    { "१", "९", "उन्नाइस", NULL },
    { "२", "०", "बिस", NULL },
    { "२", "१", "एक्काइस", NULL },
    { "२", "२", "बाइस", NULL },
    { "२", "३", "तेइस", NULL },
    { "२", "४", "चौबिस", NULL },
    { "२", "५", "पच्चिस", NULL },
    { "२", "६", "छब्बिस", NULL },
    { "२", "७", "सत्ताइस", NULL },
    { "२", "८", "अट्ठाइस", NULL },
    { "२", "९", "उनन्तिस", NULL },
    { "३", "०", "तिस", NULL },
    { "३", "१", "एकतिस", NULL },
    { "३", "२", "बत्तिस", NULL },
    { "३", "३", "तेत्तिस", NULL },
    { "३", "४", "चौतिस", NULL },
    { "३", "५", "पैँतिस", NULL },
    { "३", "६", "छत्तिस", NULL },
    { "३", "७", "सैँतिस", NULL },
    { "३", "८", "अठतिस", NULL },
    { "३", "९", "उनन्चालिस", NULL },
    { "४", "०", "चालिस", NULL },
    { "४", "१", "एकचालिस", NULL },
    { "४", "२", "बयालिस", NULL },
    { "४", "३", "त्रिचालिस", NULL },
    { "४", "४", "चवालिस", NULL },
    { "४", "५", "पैँतालिस", NULL },
    { "४", "६", "छयालिस", NULL },
    { "४", "७", "सतचालिस", NULL },
    { "४", "८", "अठचालिस", NULL },
    { "४", "९", "उनन्चास", NULL },
    { "५", "०", "पचास", NULL },
    { "५", "१", "एकाउन्न", NULL },
    { "५", "२", "बाउन्न", NULL },
    { "५", "३", "त्रिपन्न", NULL },
    { "५", "४", "चवन्न", NULL },
    { "५", "५", "पचपन्न", NULL },
    { "५", "६", "छपन्न", NULL },
    { "५", "७", "सन्ताउन्न", NULL },
    { "५", "८", "अन्ठाउन्न", NULL }, 
    { "५", "९", "उनसट्ठी", NULL },
    { "६", "०", "साठी", NULL },
    { "६", "१", "एकसट्ठी", NULL },
    { "६", "२", "बयसट्ठी", NULL },
    { "६", "३", "त्रिसट्ठी", NULL },
    { "६", "४", "चौसट्ठी", NULL },
    { "६", "५", "पैँसट्ठी", NULL },
    { "६", "६", "छयसट्ठी", NULL },
    { "६", "७", "सतसट्ठी", NULL },
    { "६", "८", "अठसट्ठी", NULL },
    { "६", "९", "उनन्सत्तरी", NULL },
    { "७", "०", "सत्तरी", NULL },
    { "७", "१", "एकहत्तर", NULL },
    { "७", "२", "बहत्तर", NULL },
    { "७", "३", "त्रिहत्तर", NULL },
    { "७", "४", "चौहत्तर", NULL },
    { "७", "५", "पचहत्तर", NULL },
    { "७", "६", "छयहत्तर", NULL },
    { "७", "७", "सतहत्तर", NULL },
    { "७", "८", "अठहत्तर", NULL },
    { "७", "९", "उनासी", NULL },
    { "८", "०", "असी", NULL },
    { "८", "१", "एकासी", NULL },
    { "८", "२", "बयासी", NULL },
    { "८", "३", "त्रियासी", NULL },
    { "८", "४", "चौरासी", NULL },
    { "८", "५", "पचासी", NULL },
    { "८", "६", "छयासी", NULL },
    { "८", "७", "सतासी", NULL },
    { "८", "८", "अठासी", NULL },
    { "८", "९", "उनान्नब्बे", NULL },
    { "९", "०", "नब्बे", NULL },
    { "९", "१", "एकान्नब्बे", NULL },
    { "९", "२", "बयान्नब्बे", NULL },
    { "९", "३", "त्रियान्नब्बे", NULL },
    { "९", "४", "चौरान्नब्बे", NULL },
    { "९", "५", "पन्चान्नब्बे", NULL },
    { "९", "६", "छयान्नब्बे", NULL },
    { "९", "७", "सन्तान्नब्बे", NULL },
    { "९", "८", "अन्ठान्नब्बे", NULL },
    { "९", "९", "उनान्सय", NULL },  
    { NULL, NULL },
};

const static indic_num_table nep_num_table = {
    "nep",
    &indic_nep_digit,
    &indic_nep_two_digit,
    "सय",   /* hundred */
    "हजार", /* thousand */
    "लाख",  /* lakh */
    "करोड", /* crore */
};


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif


