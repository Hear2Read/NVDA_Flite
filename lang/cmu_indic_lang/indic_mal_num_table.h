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
/*  Number pronunciation for (Malayalam) Indic                             */
/*************************************************************************/

#ifndef _indic_mal_num_table_h_
#define _indic_mal_num_table_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "indic_num_table.h"

static const char * const indic_mal_digit[11][2] =
{
    { "0", "പൂജ്യം" },
    { "1", "ഒന്ന്" },
    { "2", "രണ്ട്" },
    { "3", "മുന്ന്" },
    { "4", "നാല്‌" },
    { "5", "അഞ്ച്" },
    { "6", "അറ്" },
    { "7", "ഏഴ്" },
    { "8", "എട്ട്" },
    { "9", "ഒന്‍പത്" },
    { NULL, NULL },
};

static const char * const indic_mal_two_digit[101][4] =
{
    { "1", "0", "പത്ത്",  NULL },
    { "1", "1", "പതിനൊന്ന്",  NULL },
    { "1", "2", "പന്ത്രണ്ട്",  NULL },
    { "1", "3", "പതിമുന്നു",  NULL },
    { "1", "4", "പതിനാല്",  NULL },
    { "1", "5", "പതിനഞ്ച്",  NULL },
    { "1", "6", "പതിനാറ്",  NULL },
    { "1", "7", "പതിനേഴ്",  NULL },
    { "1", "8", "പതിനെട്ട്",  NULL },
    { "1", "9", "പത്തൊമ്പതു",  NULL },
    { "2", "0", "ഇരുപത്",  NULL },
    { "2", "1", "ഇരുപത്തി ഒന്ന്",  NULL },
    { "2", "2", "ഇരുപത്തി രണ്ട്‌",  NULL },
    { "2", "3", "ഇരുപത്തി മൂന്ന്",  NULL },
    { "2", "4", "ഇരുപത്തി നാല്",  NULL },
    { "2", "5", "ഇരുപത്തി അഞ്ചു",  NULL },
    { "2", "6", "ഇരുപത്തി ആറ്",  NULL },
    { "2", "7", "ഇരുപത്തി ഏഴ്",  NULL },
    { "2", "8", "ഇരുപത്തി എട്ടു",  NULL },
    { "2", "9", "ഇരുപത്തി ഒന്‍പത്",  NULL },
    { "3", "0", "മുപ്പത്",  NULL },
    { "3", "1", "മുപ്പത്തി ഒന്ന്",  NULL },
    { "3", "2", "മുപ്പത്തി രണ്ട്",  NULL },
    { "3", "3", "മുപ്പത്തി മൂന്ന്",  NULL },
    { "3", "4", "മുപ്പത്തി നാല്",  NULL },
    { "3", "5", "മുപ്പത്തി അഞ്ചു",  NULL },
    { "3", "6", "മുപ്പത്തി ആറ്",  NULL },
    { "3", "7", "മുപ്പത്തി ഏഴ്",  NULL },
    { "3", "8", "മുപ്പത്തി എട്ട്",  NULL },
    { "3", "9", "മുപ്പത്തി ഒന്‍പതു",  NULL },
    { "4", "0", "നാല്‍പത്‌",  NULL },
    { "4", "1", "നാപ്പത്തി ഒന്ന്",  NULL },
    { "4", "2", "നാപ്പത്തി രണ്ട്",  NULL },
    { "4", "3", "നാപ്പത്തി മൂന്ന്",  NULL },
    { "4", "4", "നാപ്പത്തി നാല്",  NULL },
    { "4", "5", "നാപ്പത്തി അഞ്ച്",  NULL },
    { "4", "6", "നാപ്പത്തി ആറ്",  NULL },
    { "4", "7", "നാപ്പത്തി ഏഴ്",  NULL },
    { "4", "8", "നാപ്പത്തി എട്ട്",  NULL },
    { "4", "9", "നാപ്പത്തി ഒന്‍പത്",  NULL },
    { "5", "0", "അന്‍പത് ",  NULL },
    { "5", "1", "അന്‍പത്തി ഒന്ന്",  NULL },
    { "5", "2", "അന്‍പത്തി രണ്ട്",  NULL },
    { "5", "3", "അന്‍പത്തി മൂന്ന്",  NULL },
    { "5", "4", "അന്‍പത്തി നാല്",  NULL },
    { "5", "5", "അന്‍പത്തി അഞ്ച്",  NULL },
    { "5", "6", "അന്‍പത്തി ആറ്",  NULL },
    { "5", "7", "അന്‍പത്തി ഏഴ്",  NULL },
    { "5", "8", "അന്‍പത്തി എട്ട്",  NULL },
    { "5", "9", "അന്‍പത്തി ഒന്‍പത്",  NULL },
    { "6", "0", "അറുപത്",  NULL },
    { "6", "1", "അറുപത്തി ഒന്ന്",  NULL },
    { "6", "2", "അറുപത്തി രണ്ട്",  NULL },
    { "6", "3", "അറുപത്തി മൂന്ന്",  NULL },
    { "6", "4", "അറുപത്തി നാല്",  NULL },
    { "6", "5", "അറുപത്തി അഞ്ച്",  NULL },
    { "6", "6", "അറുപത്തി ആറ്",  NULL },
    { "6", "7", "അറുപത്തി ഏഴ്",  NULL },
    { "6", "8", "അറുപത്തി എട്ട്",  NULL },
    { "6", "9", "അറുപത്തി ഒന്‍പത്",  NULL },
    { "7", "0", "എഴുപത്",  NULL },
    { "7", "1", "എഴുപത്തി ഒന്ന്",  NULL },
    { "7", "2", "എഴുപത്തി രണ്ട്",  NULL },
    { "7", "3", "എഴുപത്തി മൂന്ന്",  NULL },
    { "7", "4", "എഴുപത്തി നാല്",  NULL },
    { "7", "5", "എഴുപത്തി അഞ്ച്",  NULL },
    { "7", "6", "എഴുപത്തി ആറ്",  NULL },
    { "7", "7", "എഴുപത്തി ഏഴ്",  NULL },
    { "7", "8", "എഴുപത്തി എട്ട്",  NULL },
    { "7", "9", "एकोणऐंशी",  NULL },
    { "8", "0", "എണ്‍പത്",  NULL },
    { "8", "1", "എണ്‍പത്തി ഒന്ന്",  NULL },
    { "8", "2", "എണ്‍പത്തി രണ്ട്",  NULL },
    { "8", "3", "എണ്‍പത്തി മൂന്ന്",  NULL },
    { "8", "4", "എണ്‍പത്തി നാല്",  NULL },
    { "8", "5", "എണ്‍പത്തി അഞ്ച്",  NULL },
    { "8", "6", "എണ്‍പത്തി ആറ്",  NULL },
    { "8", "7", "എണ്‍പത്തി ഏഴ്",  NULL },
    { "8", "8", "എണ്‍പത്തി എട്ട്",  NULL },
    { "8", "9", "എണ്‍പത്തി ഒന്‍പത്",  NULL },
    { "9", "0", "തൊണ്ണൂറ്",  NULL },
    { "9", "1", "തൊണ്ണൂറ്റി ഒന്ന്",  NULL },
    { "9", "2", "തൊണ്ണൂറ്റി രണ്ട്",  NULL },
    { "9", "3", "തൊണ്ണൂറ്റി മൂന്ന്",  NULL },
    { "9", "4", "തൊണ്ണൂറ്റി നാല്",  NULL },
    { "9", "5", "തൊണ്ണൂറ്റി അഞ്ച്",  NULL },
    { "9", "6", "തൊണ്ണൂറ്റി ആറ്",  NULL },
    { "9", "7", "തൊണ്ണൂറ്റി ഏഴ്",  NULL },
    { "9", "8", "തൊണ്ണൂറ്റി എട്ട്",  NULL },
    { "9", "9", "തൊണ്ണൂറ്റി ഒന്‍പത്",  NULL },
    { NULL, NULL },
};

const static indic_num_table mal_num_table = {
    "mal",
    &indic_mal_digit,
    &indic_mal_two_digit,
    "നുറ്",   /* hundred */
    "ആയിരം", /* thousand */
    "ലക്ഷം",  /* lakh */
    "കോടി", /* crore */
};


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif

    
