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

%module PyICU_currency

%{

#include "common.h"

%}

%include "common.i"
%import "bases.i"
%import "string.i"
%import "format.i"


namespace icu {

    class MeasureFormat : public Format {
    public:
        static _MeasureFormat *createCurrencyFormat(Locale &, UErrorCode);
        static _MeasureFormat *createCurrencyFormat(UErrorCode);
    };

    class CurrencyUnit : public MeasureUnit {
    public:
        CurrencyUnit(ISO3Code, UErrorCode);
        ISO3Code getISOCurrency();

        %extend {
            PyObject *__repr__()
            {
                UnicodeString u(self->getISOCurrency());
                PyObject *string = PyUnicode_FromUnicodeString(&u);
                PyObject *format = PyString_FromString("<CurrencyUnit: %s>");
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

    class CurrencyAmount : public Measure {
    public:
        CurrencyAmount(Formattable &, ISO3Code, UErrorCode);
        CurrencyAmount(double, ISO3Code, UErrorCode);
        CurrencyUnit getCurrency();
        ISO3Code getISOCurrency();

        %extend {
            PyObject *__repr__()
            {
                Formattable number = self->getNumber();
                PyObject *amount = PyFloat_FromDouble(number.getDouble());

                UnicodeString u(self->getISOCurrency());
                PyObject *currency = PyUnicode_FromUnicodeString(&u);

                PyObject *format = PyString_FromString("<CurrencyAmount: %0.2f %s>");
                PyObject *tuple = PyTuple_New(2);
                PyObject *repr;

                PyTuple_SET_ITEM(tuple, 0, amount);
                PyTuple_SET_ITEM(tuple, 1, currency);
                repr = PyString_Format(format, tuple);
                Py_DECREF(format);
                Py_DECREF(tuple);

                return repr;
            }
        }
    };
}
