/* ====================================================================
 * Copyright (c) 2004-2005 Open Source Applications Foundation.
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

%module PyICU_bases

%{

#include "common.h"

%}

%include "common.i"

typedef enum {
    ULOC_ACTUAL_LOCALE = 0,
    ULOC_VALID_LOCALE  = 1,
} icu::ULocDataLocaleType;

namespace icu {

    class UMemory {
    public:
    };

    class UObject : public UMemory {
    public:
    };

    class Replaceable : public UObject {
    public:
        int32_t length(void);
        UChar charAt(int32_t);
        UBool hasMetaData();
    };

    class Formattable : public UObject {
    public:
        enum ISDATE {
            kIsDate
        };

        enum Type {
            kDate,
            kDouble,
            kLong,
            kString,
            kArray,
            kInt64,
            kObject
        };

        Formattable();
        Formattable(UDate, ISDATE);
        Formattable(double);
        Formattable(int32_t);
        Formattable(int64_t);
        Formattable(char *);
        Formattable(UnicodeString &);
        UBool operator==(Formattable &);
        UBool operator!=(Formattable &);
        Type getType();
        UBool isNumeric();
        double getDouble(UErrorCode);
        int32_t getLong(UErrorCode);
        int64_t getInt64(UErrorCode);
        UDate getDate(UErrorCode);
        UnicodeString1 &getString(UnicodeString &, UErrorCode);
        UnicodeString getString(_UnicodeString, UErrorCode);
        void setDouble(double);
        void setLong(int32_t);
        void setInt64(int64_t);
        void setDate(UDate);
        void setString(UnicodeString &);
        void setString(_PyString);
    };

    class MeasureUnit : public UObject {
    public:
        UBool operator==(UObject &);
    };

    class Measure : public UObject {
    public:
        UBool operator==(UObject &);
        Formattable getNumber();
    };
}
