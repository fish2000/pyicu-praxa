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

%module PyICU_collator

%{

#include "common.h"

%}

%include "common.i"
%import "bases.i"
%import "locale.i"

enum UCollationResult {
    UCOL_LESS    = -1,
    UCOL_EQUAL   = 0,
    UCOL_GREATER = 1
};

namespace icu {

    class CollationKey : public UObject {
    public:

        CollationKey();

        UBool operator==(CollationKey &);
        UBool operator!=(CollationKey &);
        UBool isBogus();
        UCollationResult compareTo(CollationKey &, UErrorCode);

        %extend {

            PyObject *getByteArray()
            {
                int32_t count;
                const uint8_t *array = self->getByteArray(count);

                return PyString_FromStringAndSize((char *) array, count);
            }
        }
    };

    class Collator : public UObject {
    public:
        enum ECollationStrength {
            PRIMARY    = 0, 
            SECONDARY  = 1,
            TERTIARY   = 2,
            QUATERNARY = 3,
            IDENTICAL  = 15
        };

        UBool operator==(Collator &);
        UBool operator!=(Collator &);
        
        UCollationResult compare(UnicodeString &, UnicodeString &, UErrorCode);
        UCollationResult compare(_PyString, _PyString, UErrorCode);

        UCollationResult compare(UnicodeString &, UnicodeString &, int32_t, UErrorCode);
        UCollationResult compare(_PyString, _PyString, int32_t, UErrorCode);
        
        CollationKey2 &getCollationKey(UnicodeString &, CollationKey &, UErrorCode);
        CollationKey2 &getCollationKey(_PyString, CollationKey &, UErrorCode);

        UBool greater(UnicodeString &, UnicodeString &);
        UBool greaterOrEqual(UnicodeString &, UnicodeString &);
        UBool equals(UnicodeString &, UnicodeString &);

        UBool greater(_PyString, _PyString);
        UBool greaterOrEqual(_PyString, _PyString);
        UBool equals(_PyString, _PyString);

        ECollationStrength getStrength();
        void setStrength(ECollationStrength);

        static _Collator *createInstance(UErrorCode);
        static _Collator *createInstance(Locale, UErrorCode);

        static LocaleDict1 getAvailableLocales(_int32_t);
        static _StringEnumeration *getKeywords(UErrorCode);
        static _StringEnumeration *getKeywordValues(char *, UErrorCode);

        %extend {
            const Locale getLocale(ULocDataLocaleType type=ULOC_VALID_LOCALE)
            {
                UErrorCode status = U_ZERO_ERROR;
                Locale locale = self->getLocale(type, status);

                if (U_FAILURE(status))
                    throw ICUException(status);

                return locale;
            }

            static PyObject *getFunctionalEquivalent(char *keyword, Locale &locale)
            {
                UErrorCode status = U_ZERO_ERROR;
                UBool isAvailable;

                Locale equiv = Collator::getFunctionalEquivalent(keyword, locale, isAvailable, status);
                
                if (U_FAILURE(status))
                    throw ICUException(status);

                PyObject *tuple = PyTuple_New(2);
                PyTuple_SET_ITEM(tuple, 0, SWIG_NewPointerObj(new Locale(equiv), SWIGTYPE_p_icu__Locale, 1));
                if (isAvailable)
                    Py_INCREF(Py_True);
                else
                    Py_INCREF(Py_False);
                PyTuple_SET_ITEM(tuple, 1, isAvailable ? Py_True : Py_False);

                return tuple;
            }
        }
    };

    class RuleBasedCollator : public Collator {
    public:

        RuleBasedCollator(UnicodeString &, UErrorCode);
        RuleBasedCollator(UnicodeString &, ECollationStrength, UErrorCode);
        RuleBasedCollator(_PyString, UErrorCode);
        RuleBasedCollator(_PyString, ECollationStrength, UErrorCode);

        UnicodeString getRules();
    };
}
