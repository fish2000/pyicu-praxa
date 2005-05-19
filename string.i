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

%module PyICU_string

%{

#include <unicode/ucnv.h>
#include "common.h"

%}

%include "common.i"
%import "bases.i"

namespace icu {

    class UnicodeString : public Replaceable {
    public:
        UnicodeString();
        UnicodeString(UnicodeString &);
        UnicodeString(_PyString);
        UBool operator==(UnicodeString &);
        UBool operator!=(UnicodeString &);
        UBool operator>(UnicodeString &);
        UBool operator<(UnicodeString &);
        UBool operator>=(UnicodeString &);
        UBool operator<=(UnicodeString &);
        UBool operator==(_PyString);
        UBool operator!=(_PyString);
        UBool operator>(_PyString);
        UBool operator<(_PyString);
        UBool operator>=(_PyString);
        UBool operator<=(_PyString);
        UnicodeString0 &operator+=(UnicodeString &);
        UnicodeString0 &operator+=(_PyString);
        UnicodeString0 &append(UnicodeString &);
        UnicodeString0 &append(UnicodeString &, int32_t, int32_t);
        UnicodeString0 &append(_PyString);
        UnicodeString0 &append(_PyString, int32_t, int32_t);
        UnicodeString0 &append(UChar);

        %extend {
            _PyString *toUnicode()
            {
                return self;
            }

            UChar __getitem__(int32_t index)
            {
                if (index < 0)
                    index += self->length();

                return self->charAt(index);
            }

            int32_t __len__()
            {
                return self->length();
            }

            void __setitem__(int32_t index, UChar c)
            {
                if (index < 0)
                    index += self->length();

                self->setCharAt(index, c);
            }

            _UnicodeString *__getslice__(int32_t start, int32_t end)
            {
                if (start < 0)
                    start += self->length();
                if (end < 0)
                    end += self->length();

                UnicodeString *string = new UnicodeString();
                self->extractBetween(start, end, *string);

                return string;
            }

            void __setslice__(int32_t start, int32_t end, UnicodeString &string)
            {
                if (start < 0)
                    start += self->length();
                if (end < 0)
                    end += self->length();

                self->replaceBetween(start, end, string);
            }

            void __setslice__(int32_t start, int32_t end, _PyString string)
            {
                if (start < 0)
                    start += self->length();
                if (end < 0)
                    end += self->length();

                self->replaceBetween(start, end, string);
            }

            PyObject *__repr__()
            {
                PyObject *string = PyUnicode_FromUnicodeString(self);
                PyObject *format = PyString_FromString("<UnicodeString: %s>");
                PyObject *tuple = PyTuple_New(1);
                PyObject *repr;

                PyTuple_SET_ITEM(tuple, 0, string);
                repr = PyString_Format(format, tuple);
                Py_DECREF(format);
                Py_DECREF(tuple);

                return repr;
            }

            PyObject *__str__()
            {
                PyObject *string = PyUnicode_FromUnicodeString(self);
                PyObject *str = PyObject_Str(string);

                Py_DECREF(string);

                return str;
            }
        }
    };
}
