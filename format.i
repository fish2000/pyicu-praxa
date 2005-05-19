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

%module PyICU_format

%{

#include "common.h"

%}

%include "common.i"
%import "bases.i"
%import "string.i"

namespace icu {

    class FieldPosition : public UObject {
    public:
        enum {
            DONT_CARE = -1
        };

        FieldPosition();
        FieldPosition(int32_t);
        UBool operator==(FieldPosition &);
        UBool operator!=(FieldPosition &);
        int32_t getField();
        int32_t getBeginIndex();
        int32_t getEndIndex();
        void setField(int32_t);
        void setBeginIndex(int32_t);
        void setEndIndex(int32_t);
    };

    class ParsePosition : public UObject {
    public:
        ParsePosition();
        ParsePosition(int32_t);
        UBool operator==(ParsePosition &);
        UBool operator!=(ParsePosition &);
        int32_t getIndex();
        void setIndex(int32_t);
        void setErrorIndex(int32_t);
        int32_t getErrorIndex();
    };

    class Format : public UObject {
    public:
        UBool operator==(Format &);
        UBool operator!=(Format &);
        UnicodeString2 &format(Formattable &, UnicodeString &, UErrorCode);
        UnicodeString2 &format(Formattable &, UnicodeString &, FieldPosition &, UErrorCode);
        UnicodeString format(Formattable &, _UnicodeString, UErrorCode);
        UnicodeString format(Formattable &, _UnicodeString, FieldPosition &, UErrorCode);
        void parseObject(UnicodeString &, Formattable &, ParsePosition &);
        void parseObject(UnicodeString &, Formattable &, UErrorCode);
        void parseObject(_PyString, Formattable &, ParsePosition &);
        void parseObject(_PyString, Formattable &, UErrorCode);

        %extend {
            const Locale getLocale(ULocDataLocaleType type=ULOC_VALID_LOCALE)
            {
                UErrorCode status = U_ZERO_ERROR;
                Locale locale = self->getLocale(type, status);

                if (U_FAILURE(status))
                    throw ICUException(status, "error");

                return locale;
            }

            const char *getLocaleID(ULocDataLocaleType type=ULOC_VALID_LOCALE)
            {
                UErrorCode status = U_ZERO_ERROR;
                const char *localeID = self->getLocaleID(type, status);

                if (U_FAILURE(status))
                    throw ICUException(status, "error");

                return localeID;
            }
        }
    };

    class MeasureFormat : public Format {
    public:
        static _MeasureFormat *createCurrencyFormat(Locale &, UErrorCode);
        static _MeasureFormat *createCurrencyFormat(UErrorCode);
    };

    class MessageFormat : public Format {
    public:
        MessageFormat(UnicodeString &, UErrorCode);
        MessageFormat(UnicodeString &, Locale &, UParseError, UErrorCode);
        MessageFormat(_PyString, UErrorCode);
        MessageFormat(_PyString, Locale &, UParseError, UErrorCode);

        void setLocale(Locale &);
        Locale getLocale();

        void applyPattern(UnicodeString &, UParseError, UErrorCode);
        void applyPattern(_PyString, UParseError, UErrorCode);

        UnicodeString1 &toPattern(UnicodeString &);
        UnicodeString toPattern(_UnicodeString);

        void setFormat(int32_t, Format &);
    };
}
