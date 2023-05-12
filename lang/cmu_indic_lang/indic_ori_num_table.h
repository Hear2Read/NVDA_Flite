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

#ifndef _indic_ori_num_table_h_
#define _indic_ori_num_table_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "indic_num_table.h"
static const char * const indic_ori_digit[11][2] =
{
    { "୦", "ଶୂନ୍" },
    { "୧", "ଏକ" },
    { "୨", "ଦୁଇ" },
    { "୩", "ତିନି" },
    { "୪", "ଚାରି" },
    { "୫", "ପାଂଚ" },
    { "୬", "ଛଅ" },
    { "୭", "ସାତ" },
    { "୮", "ଆଠ" },
    { "୯", "ନଅ" },
    { NULL, NULL },
};

static const char * const indic_ori_two_digit[101][4] =
{
    { "୧", "୦", "ଦଶ", NULL },
    { "୧", "୧", "ଏଗାର", NULL },
    { "୧", "୨", "ବାର", NULL },
    { "୧", "୩", "ତେର", NULL },
    { "୧", "୪", "ଚଉଦ", NULL },
    { "୧", "୫", "ପନ୍ଦର", NULL },
    { "୧", "୬", "ଷୋଳ", NULL },
    { "୧", "୭", "ସତର", NULL },
    { "୧", "୮", "ଅଠର", NULL },
    { "୧", "୯", "ଉଣେଇଶ୍", NULL },
    { "୨", "୦", "କୋଡ଼ିଏ", NULL },
    { "୨", "୧", "ଏକୋଇଶ୍", NULL },
    { "୨", "୨", "ବାଇଶ୍", NULL },
    { "୨", "୩", "ତେଇଶ୍", NULL },
    { "୨", "୪", "ଚବିଶ୍", NULL },
    { "୨", "୫", "ପଚିଶ୍", NULL },
    { "୨", "୬", "ଛବିଶ୍", NULL },
    { "୨", "୭", "ସତେଇଶ୍", NULL },
    { "୨", "୮", "ଅଠେଇଶ୍", NULL },
    { "୨", "୯", "ଅଣତିରିଶ୍", NULL },
    { "୩", "୦", "ତିରିଶ୍", NULL },
    { "୩", "୧", "ଏକ୍ତିରିଶ୍", NULL },
    { "୩", "୨", "ବତିଶ୍", NULL },
    { "୩", "୩", "ତେତିଶ୍", NULL },
    { "୩", "୪", "ଚଉତିରିଶ୍", NULL },
    { "୩", "୫", "ପଇଁତିରିଶ୍", NULL },
    { "୩", "୬", "ଛତିଶ୍", NULL },
    { "୩", "୭", "ସଇଁତିରିଶ୍", NULL },
    { "୩", "୮", "ଅଡ଼ତିରିଶ୍", NULL },
    { "୩", "୯", "ଅଣଚାଳିଶ୍", NULL },
    { "୪", "୦", "ଚାଳିଶ୍", NULL },
    { "୪", "୧", "ଏକ୍ଚାଳିଶ୍", NULL },
    { "୪", "୨", "ବୟାଳିଶ୍", NULL },
    { "୪", "୩", "ତେୟାଳିଶ୍", NULL },
    { "୪", "୪", "ଚଉରାଳିଶ୍", NULL },
    { "୪", "୫", "ପଇଁଚାଳିଶ୍", NULL },
    { "୪", "୬", "ଛୟାଳିଶ୍", NULL },
    { "୪", "୭", "ସତଚାଳିଶ୍", NULL },
    { "୪", "୮", "ଅଡ଼ଚାଳିଶ୍", NULL },
    { "୪", "୯", "ଅଣଞ୍ଚାଶ୍", NULL },
    { "୫", "୦", "ପଚାଶ୍", NULL },
    { "୫", "୧", "ଏକାବନ୍", NULL },
    { "୫", "୨", "ବାବନ୍", NULL },
    { "୫", "୩", "ତେପନ୍", NULL },
    { "୫", "୪", "ଚଉବନ୍", NULL },
    { "୫", "୫", "ପଂଚାବନ୍", NULL },
    { "୫", "୬", "ଛପନ୍", NULL },
    { "୫", "୭", "ସତାବନ୍", NULL },
    { "୫", "୮", "ଅଠାବନ୍", NULL }, 
    { "୫", "୯", "ଅଣଷଠି", NULL },
    { "୬", "୦", "ଷାଠିଏ", NULL },
    { "୬", "୧", "ଏକଷଠି", NULL },
    { "୬", "୨", "ବାଷଠି", NULL },
    { "୬", "୩", "ତେଷଠି", NULL },
    { "୬", "୪", "ଚଉଷଠି", NULL },
    { "୬", "୫", "ପଅଁଷଠି", NULL },
    { "୬", "୬", "ଛଅଷଠି", NULL },
    { "୬", "୭", "ସତଷଠି", NULL },
    { "୬", "୮", "ଅଡ଼ଷଠି", NULL },
    { "୬", "୯", "ଅଣସ୍ତରି", NULL },
    { "୭", "୦", "ସତୁରି", NULL },
    { "୭", "୧", "ଏକସ୍ତରି", NULL },
    { "୭", "୨", "ବାସ୍ତରି", NULL },
    { "୭", "୩", "ତେସ୍ତରି", NULL },
    { "୭", "୪", "ଛଉସ୍ତରି", NULL },
    { "୭", "୫", "ପଞ୍ଚସ୍ତରି", NULL },
    { "୭", "୬", "ଛଅସ୍ତରି", NULL },
    { "୭", "୭", "ସତସ୍ତରି", NULL },
    { "୭", "୮", "ଅଠସ୍ତରି", NULL },
    { "୭", "୯", "ଅଣାଅଶୀ", NULL },
    { "୮", "୦", "ଅଶୀ", NULL },
    { "୮", "୧", "ଏକାଅଶୀ", NULL },
    { "୮", "୨", "ବୟାଅଶୀ", NULL },
    { "୮", "୩", "ତେୟାଅଶୀ", NULL },
    { "୮", "୪", "ଚଉରାଅଶୀ", NULL },
    { "୮", "୫", "ପଞ୍ଚାଅଶୀ", NULL },
    { "୮", "୬", "ଛୟାଅଶୀ", NULL },
    { "୮", "୭", "ସତାଅଶୀ", NULL },
    { "୮", "୮", "ଅଠାଅଶୀ", NULL },
    { "୮", "୯", "ଅଣାନବେ", NULL },
    { "୯", "୦", "ନବେ", NULL },
    { "୯", "୧", "ଏକାନବେ", NULL },
    { "୯", "୨", "ବୟାନବେ", NULL },
    { "୯", "୩", "ତେୟାନବେ", NULL },
    { "୯", "୪", "ଚଉରାନବେ", NULL },
    { "୯", "୫", "ପଞ୍ଚାନବେ", NULL },
    { "୯", "୬", "ଛୟାନବେ", NULL },
    { "୯", "୭", "ସତାନବେ", NULL },
    { "୯", "୮", "ଅଠାନବେ", NULL },
    { "୯", "୯", "ଅନେଶତ", NULL },  
    { NULL, NULL },
};

const static indic_num_table ori_num_table = {
    "ori",
    &indic_ori_digit,
    &indic_ori_two_digit,
    "ଶହ",   /* hundred */
    "ହଜାର", /* thousand */
    "ଲକ୍ଷ",  /* lakh */
    "କୋଟି", /* crore */
};


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif


