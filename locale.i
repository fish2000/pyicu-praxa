/* ====================================================================
 * Copyright (c) 2005 Open Source Applications Foundation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software. 
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 * ====================================================================
 */

%module PyICU_locale

%{

#include <unicode/locid.h>
#include "common.h"

%}

%include "common.i"
%import "bases.i"
%import "string.i"

namespace icu {

    class Locale : public UObject {
    public:
        Locale();
        Locale(char *language, char *country=0, char *variant=0);
        char *getLanguage();
        char *getScript();
        char *getCountry();
        char *getVariant();
        char *getName();
        char *getBaseName();
        char *getISO3Language();
        char *getISO3Country();
        int32_t getLCID();
        UnicodeString1 &getDisplayLanguage(UnicodeString &);
        UnicodeString2 &getDisplayLanguage(Locale &, UnicodeString &);
        UnicodeString1 &getDisplayScript(UnicodeString &);
        UnicodeString2 &getDisplayScript(Locale &, UnicodeString &);
        UnicodeString1 &getDisplayCountry(UnicodeString &);
        UnicodeString2 &getDisplayCountry(Locale &, UnicodeString &);
        UnicodeString1 &getDisplayVariant(UnicodeString &);
        UnicodeString2 &getDisplayVariant(Locale &, UnicodeString &);
        UnicodeString1 &getDisplayName(UnicodeString &);
        UnicodeString2 &getDisplayName(Locale &, UnicodeString &);
        UnicodeString getDisplayLanguage(_UnicodeString);
        UnicodeString getDisplayLanguage(Locale &, _UnicodeString);
        UnicodeString getDisplayScript(_UnicodeString);
        UnicodeString getDisplayScript(Locale &, _UnicodeString);
        UnicodeString getDisplayCountry(_UnicodeString);
        UnicodeString getDisplayCountry(Locale &, _UnicodeString);
        UnicodeString getDisplayVariant(_UnicodeString);
        UnicodeString getDisplayVariant(Locale &, _UnicodeString);
        UnicodeString getDisplayName(_UnicodeString);
        UnicodeString getDisplayName(Locale &, _UnicodeString);

        static Locale getEnglish();
        static Locale getFrench();
        static Locale getGerman();
        static Locale getItalian();
        static Locale getJapanese();
        static Locale getKorean();
        static Locale getChinese();
        static Locale getSimplifiedChinese();
        static Locale getTraditionalChinese();
        static Locale getFrance();
        static Locale getGermany();
        static Locale getItaly();
        static Locale getJapan();
        static Locale getKorea();
        static Locale getChina();
        static Locale getPRC();
        static Locale getTaiwan();
        static Locale getUK();
        static Locale getUS();
        static Locale getCanada();
        static Locale getCanadaFrench();
        static Locale getDefault();

        %extend {
            PyObject *__repr__()
            {
                PyObject *string = PyString_FromString(self->getName());
                PyObject *format = PyString_FromString("<Locale: %s>");
                PyObject *tuple = PyTuple_New(1);
                PyObject *repr;

                PyTuple_SET_ITEM(tuple, 0, string);
                repr = PyString_Format(format, tuple);
                Py_DECREF(format);
                Py_DECREF(tuple);

                return repr;
            }
        }
    };
}
