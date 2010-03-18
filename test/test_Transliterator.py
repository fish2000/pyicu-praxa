# -*- coding: utf-8 -*-
# ====================================================================
# Copyright (c) 2009-2010 Open Source Applications Foundation.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions: 
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software. 
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
# ====================================================================
#

import sys, os

from unittest import TestCase, main
from PyICU import *


class TestTransliterator(TestCase):

    def testTransliterate(self):

        trans = Transliterator.createInstance('Accents-Any',
                                              UTransDirection.UTRANS_FORWARD)
        inverse = trans.createInverse()

        string = u'\xe9v\xe9nement'
        if ICU_VERSION < '4.0':
            result = u"e<'>ve<'>nement"
        else:
            result = u"e\u2190'\u2192ve\u2190'\u2192nement"

        self.assert_(trans.transliterate(string) == result)
        self.assert_(inverse.transliterate(result) == string)

    def testUnicodeString(self):

        trans = Transliterator.createInstance('NumericPinyin-Latin',
                                              UTransDirection.UTRANS_FORWARD)
        string = UnicodeString("Shang4hai3 zi4lai2shui3 lai2 zi4 hai3 shang4")
        result = u'Sh\xe0ngh\u01cei z\xecl\xe1ishu\u01d0 l\xe1i z\xec h\u01cei sh\xe0ng'

        self.assert_(trans.transliterate(unicode(string)) == result)
        self.assert_(trans.transliterate(string) == result)
        self.assert_(string == result)

    def testPythonTransliterator(self):

        class vowelSubst(Transliterator):
            def __init__(self, char=u'i'):
                super(vowelSubst, self).__init__("vowel")
                self.char = char
            def handleTransliterate(self, text, pos, incremental):
                for i in xrange(pos.start, pos.limit):
                    if text[i] in u"aeiouüöä":
                        text[i] = self.char
                pos.start = pos.limit

        trans = vowelSubst()
        result = trans.transliterate(u"Drei Chinesen mit dem Kontrabass")
        self.assert_(result == u'Drii Chinisin mit dim Kintribiss')
        

if __name__ == "__main__":
    main()
