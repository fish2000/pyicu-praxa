 # ====================================================================
 # Copyright (c) 2004-2006 Open Source Applications Foundation.
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


class ICUError(Exception):
    messages = {}

    def __str__(self):
        return "%s, error code: %d" %(self.args[1], self.args[0])

    def getErrorCode(self):
        return self.args[0]


class InvalidArgsError(Exception):
    pass


from _PyICU import *


from datetime import tzinfo, timedelta
FLOATING_TZNAME = "World/Floating"

class ICUtzinfo(tzinfo):

    instances = {}

    def _resetDefault(cls):
        cls.default = ICUtzinfo(TimeZone.createDefault())
    _resetDefault = classmethod(_resetDefault)

    def getInstance(cls, id):
        try:
            return cls.instances[id]
        except KeyError:
            if id == FLOATING_TZNAME:
                instance = cls.floating
            else:
                instance = cls(TimeZone.createTimeZone(id))
            cls.instances[id] = instance
            return instance
    getInstance = classmethod(getInstance)

    def getDefault(cls):
        return cls.default
    getDefault = classmethod(getDefault)

    def getFloating(cls):
        return cls.floating
    getFloating = classmethod(getFloating)

    def __init__(self, timezone):
        if not isinstance(timezone, TimeZone):
            raise TypeError, timezone
        super(ICUtzinfo, self).__init__()
        self._timezone = timezone

    def __repr__(self):
        return "<ICUtzinfo: %s>" %(self._timezone.getID())

    def __str__(self):
        return str(self._timezone.getID())

    def __eq__(self, other):
        if isinstance(other, ICUtzinfo):
            return str(self) == str(other)
        return False

    def __ne__(self, other):
        if isinstance(other, ICUtzinfo):
            return str(self) != str(other)
        return True

    def __hash__(self):
        return hash(self.tzid)

    def _notzsecs(self, dt):
        return ((dt.toordinal() - 719163) * 86400.0 +
                dt.hour * 3600.0 + dt.minute * 60.0 +
                float(dt.second) + dt.microsecond / 1e6)

    def utcoffset(self, dt):
        raw, dst = self._timezone.getOffset(self._notzsecs(dt), True)
        return timedelta(seconds = (raw + dst) / 1000)

    def dst(self, dt):
        raw, dst = self._timezone.getOffset(self._notzsecs(dt), True)
        return timedelta(seconds = dst / 1000)

    def tzname(self, dt):
        return str(self._timezone.getID())

    def _getTimezone(self):
        return TimeZone.createTimeZone(self._timezone.getID())

    tzid = property(__str__)
    timezone = property(_getTimezone)


class FloatingTZ(ICUtzinfo):

    def __init__(self):
        pass

    def __repr__(self):
        return "<FloatingTZ: %s>" %(ICUtzinfo.default._timezone.getID())

    def __str__(self):
        return FLOATING_TZNAME

    def __hash__(self):
        return hash(FLOATING_TZNAME)

    def utcoffset(self, dt):
        tz = ICUtzinfo.default._timezone
        raw, dst = tz.getOffset(self._notzsecs(dt), True)
        return timedelta(seconds = (raw + dst) / 1000)

    def dst(self, dt):
        tz = ICUtzinfo.default._timezone
        raw, dst = tz.getOffset(self._notzsecs(dt), True)
        return timedelta(seconds = dst / 1000)

    def _getTimezone(self):
        return TimeZone.createTimeZone(ICUtzinfo.default._timezone.getID())

    def __getTimezone(self):
        return ICUtzinfo.default._timezone

    def tzname(self, dt):
        return FLOATING_TZNAME
    
    tzid = FLOATING_TZNAME
    _timezone = property(__getTimezone)


ICUtzinfo.default = ICUtzinfo(TimeZone.createDefault())
ICUtzinfo.floating = FloatingTZ()
