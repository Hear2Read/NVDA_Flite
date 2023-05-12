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
/*  Number pronunciation for (asmarati) Indic                             */
/*************************************************************************/

#ifndef _indic_asm_num_table_h_
#define _indic_asm_num_table_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "indic_num_table.h"

static const char * const indic_asm_digit[11][2] =
{
    { "০", "শূণ্য" },
    { "১", "এক" },
    { "২", "দুই" },
    { "৩", "তিনি" },
    { "৪", "চাৰি" },
    { "৫", "পাঁচ" },
    { "৬", "ছয়" },
    { "৭", "সাত" },
    { "৮", "আঠ" },
    { "৯", "ন" },
    { NULL, NULL },
};

static const char * const indic_asm_two_digit[101][4] =
{
    { "১", "০", "দহ", NULL },
    { "১", "১", "এঘাৰ", NULL },
    { "১", "২", "বাৰ", NULL },
    { "১", "৩", "তেৰ", NULL },
    { "১", "৪", "চৈধ্য", NULL },
    { "১", "৫", "পোন্ধৰ", NULL },
    { "১", "৬", "ষোল্ল", NULL },
    { "১", "৭", "সোতৰ", NULL },
    { "১", "৮", "ওঁঠৰ", NULL },
    { "১", "৯", "ঊনৈছ", NULL },
    { "২", "০", "বিছ", NULL },
    { "২", "১", "একৈছ", NULL },
    { "২", "২", "বাইছ", NULL },
    { "২", "৩", "তেইছ", NULL },
    { "২", "৪", "চৌব্বিছ", NULL },
    { "২", "৫", "পঁচিছ", NULL },
    { "২", "৬", "ছাব্বিছ", NULL },
    { "২", "৭", "সাতাইছ", NULL },
    { "২", "৮", "আঁঠাইছ", NULL },
    { "২", "৯", "উনত্ৰিছ", NULL },
    { "৩", "০", "ত্ৰিছ", NULL },
    { "৩", "১", "একত্ৰিছ", NULL },
    { "৩", "২", "বত্ৰিছ", NULL },
    { "৩", "৩", "তেত্ৰিছ", NULL },
    { "৩", "৪", "চৌত্ৰিছ", NULL },
    { "৩", "৫", "পয়ত্ৰিছ", NULL },
    { "৩", "৬", "ছয়ত্ৰিছ", NULL },
    { "৩", "৭", "সাতত্ৰিছ", NULL },
    { "৩", "৮", "আঠত্ৰিছ", NULL },
    { "৩", "৯", "ঊনচল্লিছ", NULL },
    { "৪", "০", "চল্লিছ", NULL },
    { "৪", "১", "একচল্লিছ", NULL },
    { "৪", "২", "বিয়ালিছ", NULL },
    { "৪", "৩", "তিয়াল্লিছ", NULL },
    { "৪", "৪", "চৌচল্লিছ", NULL },
    { "৪", "৫", "পঞ্চল্লিছ", NULL },
    { "৪", "৬", "ছয়চল্লিছ", NULL },
    { "৪", "৭", "সাতচল্লিছ", NULL },
    { "৪", "৮", "আঠচল্লিছ", NULL },
    { "৪", "৯", "ঊনপঞ্চাছ", NULL },
    { "৫", "০", "পঞ্চাছ", NULL },
    { "৫", "১", "একাৱন্ন", NULL },
    { "৫", "২", "বাৱন্ন", NULL },
    { "৫", "৩", "তেৱন্ন", NULL },
    { "৫", "৪", "চৌৱন্ন", NULL },
    { "৫", "৫", "পঁচপন্ন", NULL },
    { "৫", "৬", "ছাপন্ন", NULL },
    { "৫", "৭", "সাতাৱন্ন", NULL },
    { "৫", "৮", "আঠাৱন্ন", NULL },
    { "৫", "৯", "উন্নষাঠী", NULL },
    { "৬", "০", "ষাঠি", NULL },
    { "৬", "১", "এষষ্ঠি", NULL },
    { "৬", "২", "বাষষ্ঠি", NULL },
    { "৬", "৩", "তেষষ্ঠি", NULL },
    { "৬", "৪", "চৌষষ্ঠি", NULL },
    { "৬", "৫", "পঁষষ্ঠি", NULL },
    { "৬", "৬", "ছয়ষষ্ঠি", NULL },
    { "৬", "৭", "সাতষষ্ঠি", NULL },
    { "৬", "৮", "আঠষষ্ঠি", NULL },
    { "৬", "৯", "ঊনসত্তৰ", NULL },
    { "৭", "০", "সত্তৰ", NULL },
    { "৭", "১", "এসত্তৰ", NULL },
    { "৭", "২", "বাসত্তৰ", NULL },
    { "৭", "৩", "তেসত্তৰ", NULL },
    { "৭", "৪", "চৌসত্তৰ", NULL },
    { "৭", "৫", "পঁসত্তৰ", NULL },
    { "৭", "৬", "ছয়সত্তৰ", NULL },
    { "৭", "৭", "সাতসত্তৰ", NULL },
    { "৭", "৮", "আঠসত্তৰ", NULL },
    { "৭", "৯", "ঊনাশী", NULL },
    { "৮", "০", "আশী", NULL },
    { "৮", "১", "একাশী", NULL },
    { "৮", "২", "বিয়াশী", NULL },
    { "৮", "৩", "তিৰাশী", NULL },
    { "৮", "৪", "চৌৰাশী", NULL },
    { "৮", "৫", "পঁচাশী", NULL },
    { "৮", "৬", "ছিয়াশী", NULL },
    { "৮", "৭", "সাতাশী", NULL },
    { "৮", "৮", "আঠাশী", NULL },
    { "৮", "৯", "ঊননবৈ", NULL },
    { "৯", "০", "নব্বৈ", NULL },
    { "৯", "১", "একান্নব্বৈ", NULL },
    { "৯", "২", "বিয়ান্নব্বৈ", NULL },
    { "৯", "৩", "তিৰান্নব্বৈ", NULL },
    { "৯", "৪", "চৌৰান্নব্বৈ", NULL },
    { "৯", "৫", "পঁচানব্বৈ", NULL },
    { "৯", "৬", "ছয়ান্নব্বৈ", NULL },
    { "৯", "৭", "সাতান্নব্বৈ", NULL },
    { "৯", "৮", "আঠান্নব্বৈ", NULL },
    { "৯", "৯", "নিৰান্নব্বৈ", NULL },
    { NULL, NULL },
};

const static indic_num_table asm_num_table = {
    "asm",
    &indic_asm_digit,
    &indic_asm_two_digit,
    "শ",   /* hundred */
    "হাজাৰ", /* thousand */
    "লাখ",  /* lakh */
    "কোটি", /* crore */
};


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif

    
