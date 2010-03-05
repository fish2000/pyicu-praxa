/* ====================================================================
 * Copyright (c) 2010-2010 Open Source Applications Foundation.
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

#include "common.h"
#include "structmember.h"

#include "bases.h"
#include "iterators.h"
#include "normalizer.h"
#include "macros.h"

DECLARE_CONSTANTS_TYPE(UNormalizationMode);
DECLARE_CONSTANTS_TYPE(UNormalizationCheckResult);


/* RegexPattern */

class t_normalizer : public _wrapper {
public:
    Normalizer *object;
};

static int t_normalizer_init(t_normalizer *self,
                             PyObject *args, PyObject *kwds);
static PyObject *t_normalizer_current(t_normalizer *self);
static PyObject *t_normalizer_first(t_normalizer *self);
static PyObject *t_normalizer_last(t_normalizer *self);
static PyObject *t_normalizer_next(t_normalizer *self);
static PyObject *t_normalizer_previous(t_normalizer *self);
static PyObject *t_normalizer_reset(t_normalizer *self);
static PyObject *t_normalizer_setIndexOnly(t_normalizer *self, PyObject *arg);
static PyObject *t_normalizer_getIndex(t_normalizer *self);
static PyObject *t_normalizer_startIndex(t_normalizer *self);
static PyObject *t_normalizer_endIndex(t_normalizer *self);
static PyObject *t_normalizer_setMode(t_normalizer *self, PyObject *arg);
static PyObject *t_normalizer_getUMode(t_normalizer *self);
static PyObject *t_normalizer_getText(t_normalizer *self);
static PyObject *t_normalizer_setText(t_normalizer *self, PyObject *arg);
static PyObject *t_normalizer_normalize(PyTypeObject *type, PyObject *args);
static PyObject *t_normalizer_compose(PyTypeObject *type, PyObject *args);
static PyObject *t_normalizer_decompose(PyTypeObject *type, PyObject *args);
static PyObject *t_normalizer_quickCheck(PyTypeObject *type, PyObject *args);
static PyObject *t_normalizer_isNormalized(PyTypeObject *type, PyObject *args);
static PyObject *t_normalizer_concatenate(PyTypeObject *type, PyObject *args);
static PyObject *t_normalizer_compare(PyTypeObject *type, PyObject *args);

static PyMethodDef t_normalizer_methods[] = {
    DECLARE_METHOD(t_normalizer, current, METH_NOARGS),
    DECLARE_METHOD(t_normalizer, first, METH_NOARGS),
    DECLARE_METHOD(t_normalizer, last, METH_NOARGS),
    DECLARE_METHOD(t_normalizer, next, METH_NOARGS),
    DECLARE_METHOD(t_normalizer, previous, METH_NOARGS),
    DECLARE_METHOD(t_normalizer, reset, METH_NOARGS),
    DECLARE_METHOD(t_normalizer, setIndexOnly, METH_O),
    DECLARE_METHOD(t_normalizer, getIndex, METH_NOARGS),
    DECLARE_METHOD(t_normalizer, startIndex, METH_NOARGS),
    DECLARE_METHOD(t_normalizer, endIndex, METH_NOARGS),
    DECLARE_METHOD(t_normalizer, setMode, METH_O),
    DECLARE_METHOD(t_normalizer, getUMode, METH_NOARGS),
    DECLARE_METHOD(t_normalizer, getText, METH_NOARGS),
    DECLARE_METHOD(t_normalizer, setText, METH_O),
    DECLARE_METHOD(t_normalizer, normalize, METH_VARARGS | METH_CLASS),
    DECLARE_METHOD(t_normalizer, compose, METH_VARARGS | METH_CLASS),
    DECLARE_METHOD(t_normalizer, decompose, METH_VARARGS | METH_CLASS),
    DECLARE_METHOD(t_normalizer, quickCheck, METH_VARARGS | METH_CLASS),
    DECLARE_METHOD(t_normalizer, isNormalized, METH_VARARGS | METH_CLASS),
    DECLARE_METHOD(t_normalizer, concatenate, METH_VARARGS | METH_CLASS),
    DECLARE_METHOD(t_normalizer, compare, METH_VARARGS | METH_CLASS),
    { NULL, NULL, 0, NULL }
};

DECLARE_TYPE(Normalizer, t_normalizer, UObject, Normalizer, t_normalizer_init);


/* Normalizer */

static int t_normalizer_init(t_normalizer *self,
                             PyObject *args, PyObject *kwds)
{
    Normalizer *normalizer;
    UnicodeString *u, _u;
    UNormalizationMode mode;
    CharacterIterator *iterator;

    switch (PyTuple_Size(args)) {
      case 1:
        if (!parseArgs(args, "P", TYPE_CLASSID(Normalizer), &normalizer))
        {
            self->object = new Normalizer(*normalizer);
            self->flags = T_OWNED;
            break;
        }
        PyErr_SetArgsError((PyObject *) self, "__init__", args);
        return -1;
      case 2:
        if (!parseArgs(args, "Si", &u, &_u, &mode))
        {
            self->object = new Normalizer(*u, mode);
            self->flags = T_OWNED;
            break;
        }
        if (!parseArgs(args, "Pi", TYPE_ID(CharacterIterator),
                       &iterator, &mode))
        {
            self->object = new Normalizer(*iterator, mode);
            self->flags = T_OWNED;
            break;
        }
        PyErr_SetArgsError((PyObject *) self, "__init__", args);
        return -1;
      default:
        PyErr_SetArgsError((PyObject *) self, "__init__", args);
        return -1;
    }
        
    if (self->object)
        return 0;

    return -1;
}

static PyObject * t_normalizer_current(t_normalizer *self)
{
    UChar32 c = self->object->current();
    return PyInt_FromLong(c);
}

static PyObject * t_normalizer_first(t_normalizer *self)
{
    UChar32 c = self->object->first();
    return PyInt_FromLong(c);
}

static PyObject * t_normalizer_last(t_normalizer *self)
{
    UChar32 c = self->object->last();
    return PyInt_FromLong(c);
}

static PyObject * t_normalizer_next(t_normalizer *self)
{
    UChar32 c = self->object->next();
    return PyInt_FromLong(c);
}

static PyObject * t_normalizer_previous(t_normalizer *self)
{
    UChar32 c = self->object->previous();
    return PyInt_FromLong(c);
}

static PyObject *t_normalizer_reset(t_normalizer *self)
{
    self->object->reset();
    Py_RETURN_NONE;
}

static PyObject *t_normalizer_setIndexOnly(t_normalizer *self, PyObject *arg)
{
    int32_t index;

    if (!parseArg(arg, "i", &index))
    {
        self->object->setIndexOnly(index);
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "setIndexOnly", arg);
}

static PyObject *t_normalizer_getIndex(t_normalizer *self)
{
    int32_t index = self->object->getIndex();
    return PyInt_FromLong(index);
}

static PyObject *t_normalizer_startIndex(t_normalizer *self)
{
    int32_t index = self->object->startIndex();
    return PyInt_FromLong(index);
}

static PyObject *t_normalizer_endIndex(t_normalizer *self)
{
    int32_t index = self->object->endIndex();
    return PyInt_FromLong(index);
}

static PyObject *t_normalizer_setMode(t_normalizer *self, PyObject *arg)
{
    UNormalizationMode mode;

    if (!parseArg(arg, "i", &mode) &&
        mode >= UNORM_NONE && mode < UNORM_MODE_COUNT)
    {
        self->object->setMode(mode);
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "setMode", arg);
}

static PyObject *t_normalizer_getUMode(t_normalizer *self)
{
    UNormalizationMode mode = self->object->getUMode();
    return PyInt_FromLong(mode);
}

static PyObject *t_normalizer_getText(t_normalizer *self)
{
    UnicodeString u;
    
    self->object->getText(u);
    return PyUnicode_FromUnicodeString(&u);
}

static PyObject *t_normalizer_setText(t_normalizer *self, PyObject *arg)
{
    UnicodeString *u, _u;
    CharacterIterator *iterator;

    if (!parseArg(arg, "S", &u, &_u))
    {
        STATUS_CALL(self->object->setText(*u, status));
        Py_RETURN_NONE;
    }
    
    if (!parseArg(arg, "P", TYPE_ID(CharacterIterator), &iterator))
    {
        STATUS_CALL(self->object->setText(*iterator, status));
        Py_RETURN_NONE;
    }
     
    return PyErr_SetArgsError((PyObject *) self, "setText", arg);
}

static PyObject *t_normalizer_normalize(PyTypeObject *type, PyObject *args)
{
    UnicodeString *u, _u, target;
    UNormalizationMode mode;
    int32_t options;

    if (!parseArgs(args, "Sii", &u, &_u, &mode, &options))
    {
        STATUS_CALL(Normalizer::normalize(*u, mode, options, target, status));
        return PyUnicode_FromUnicodeString(&target);
    }

    return PyErr_SetArgsError(type, "normalize", args);
}

static PyObject *t_normalizer_compose(PyTypeObject *type, PyObject *args)
{
    UnicodeString *u, _u, target;
    UBool compat;
    int32_t options;

    if (!parseArgs(args, "SBi", &u, &_u, &compat, &options))
    {
        STATUS_CALL(Normalizer::compose(*u, compat, options, target, status));
        return PyUnicode_FromUnicodeString(&target);
    }

    return PyErr_SetArgsError(type, "compose", args);
}

static PyObject *t_normalizer_decompose(PyTypeObject *type, PyObject *args)
{
    UnicodeString *u, _u, target;
    UBool compat;
    int32_t options;

    if (!parseArgs(args, "SBi", &u, &_u, &compat, &options))
    {
        STATUS_CALL(Normalizer::decompose(*u, compat, options, target, status));
        return PyUnicode_FromUnicodeString(&target);
    }

    return PyErr_SetArgsError(type, "decompose", args);
}

static PyObject *t_normalizer_quickCheck(PyTypeObject *type, PyObject *args)
{
    UnicodeString *u, *_u;
    UNormalizationMode mode;
    int32_t options;

    switch (PyTuple_Size(args)) {
      case 2:
        if (!parseArgs(args, "Si", &u, &_u, &mode))
        {
            UNormalizationCheckResult uncr;
            
            STATUS_CALL(uncr = Normalizer::quickCheck(*u, mode, status));
            return PyInt_FromLong(uncr);
        }
        break;
      case 3:
        if (!parseArgs(args, "Sii", &u, &_u, &mode, &options))
        {
            UNormalizationCheckResult uncr;
            
            STATUS_CALL(uncr = Normalizer::quickCheck(*u, mode, options,
                                                      status));
            return PyInt_FromLong(uncr);
        }
        break;
    }

    return PyErr_SetArgsError(type, "quickCheck", args);
}

static PyObject *t_normalizer_isNormalized(PyTypeObject *type, PyObject *args)
{
    UnicodeString *u, *_u;
    UNormalizationMode mode;
    int32_t options;
    UBool b;

    switch (PyTuple_Size(args)) {
      case 2:
        if (!parseArgs(args, "Si", &u, &_u, &mode))
        {
            STATUS_CALL(b = Normalizer::isNormalized(*u, mode, status));
            Py_RETURN_BOOL(b);
        }
        break;
      case 3:
        if (!parseArgs(args, "Sii", &u, &_u, &mode, &options))
        {
            STATUS_CALL(b = Normalizer::isNormalized(*u, mode, options,
                                                     status));
            Py_RETURN_BOOL(b);
        }
        break;
    }

    return PyErr_SetArgsError(type, "isNormalized", args);
}

static PyObject *t_normalizer_concatenate(PyTypeObject *type, PyObject *args)
{
    UnicodeString *u0, *_u0;
    UnicodeString *u1, *_u1;
    UnicodeString u;
    UNormalizationMode mode;
    int32_t options;

    if (!parseArgs(args, "SSii", &u0, &_u0, &u1, &_u1, &mode, &options))
    {
        STATUS_CALL(Normalizer::concatenate(*u0, *u1, u,
                                            mode, options, status));
        return PyUnicode_FromUnicodeString(&u);

    }

    return PyErr_SetArgsError(type, "concatenate", args);
}

static PyObject *t_normalizer_compare(PyTypeObject *type, PyObject *args)
{
    UnicodeString *u0, *_u0;
    UnicodeString *u1, *_u1;
    int32_t options, n;

    if (!parseArgs(args, "SSi", &u0, &_u0, &u1, &_u1, &options))
    {
        STATUS_CALL(n = Normalizer::compare(*u0, *u1, options, status));
        return PyInt_FromLong(n);
    }

    return PyErr_SetArgsError(type, "compare", args);
}


static PyObject *t_normalizer_richcmp(t_normalizer *self, PyObject *arg, int op)
{
    Normalizer *normalizer;
    int b = 0;

    if (!parseArg(arg, "P", TYPE_CLASSID(Normalizer), &normalizer))
    {
        switch (op) {
          case Py_EQ:
          case Py_NE:
            b = *self->object == *normalizer;
            if (op == Py_EQ)
                Py_RETURN_BOOL(b);
            Py_RETURN_BOOL(!b);
          case Py_LT:
          case Py_LE:
          case Py_GT:
          case Py_GE:
            PyErr_SetNone(PyExc_NotImplementedError);
            return NULL;
        }
    }

    return PyErr_SetArgsError((PyObject *) self, "__richcmp__", arg);
}

static int t_normalizer_hash(t_normalizer *self)
{
    return self->object->hashCode();
}


void _init_normalizer(PyObject *m)
{
    NormalizerType.tp_richcompare = (richcmpfunc) t_normalizer_richcmp;
    NormalizerType.tp_hash = (hashfunc) t_normalizer_hash;

    REGISTER_TYPE(Normalizer, m);

    INSTALL_CONSTANTS_TYPE(UNormalizationMode, m);
    INSTALL_CONSTANTS_TYPE(UNormalizationCheckResult, m);

    INSTALL_ENUM(UNormalizationMode, UNORM_NONE);
    INSTALL_ENUM(UNormalizationMode, UNORM_NFD);
    INSTALL_ENUM(UNormalizationMode, UNORM_NFKD);
    INSTALL_ENUM(UNormalizationMode, UNORM_NFC);
    INSTALL_ENUM(UNormalizationMode, UNORM_DEFAULT);
    INSTALL_ENUM(UNormalizationMode, UNORM_NFKC);
    INSTALL_ENUM(UNormalizationMode, UNORM_FCD);

    INSTALL_ENUM(UNormalizationCheckResult, UNORM_NO);
    INSTALL_ENUM(UNormalizationCheckResult, UNORM_YES);
    INSTALL_ENUM(UNormalizationCheckResult, UNORM_MAYBE);
}
