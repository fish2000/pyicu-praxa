
import os, sys

try:
    from setuptools import setup, Extension
except ImportError:
    from distutils.core import setup, Extension


VERSION = '0.8'
ICU_VERSION = '3.8'

INCLUDES = {
    'darwin': ['/usr/local/icu-%s/include' %(ICU_VERSION)],
    'linux2': [],
    'win32': [],
}

LFLAGS = {
    'darwin': ['-L/usr/local/icu-%s/lib' %(ICU_VERSION)],
    'linux2': [],
    'win32': []
}


if 'PYICU_INCLUDES' in os.environ:
    _includes = os.environ['PYICU_INCLUDES'].split(os.pathsep)
else:
    _includes = INCLUDES[sys.platform]

if 'PYICU_LFLAGS' in os.environ:
    _lflags = os.environ['PYICU_LFLAGS'].split(os.pathsep)
else:
    _lflags = LFLAGS[sys.platform]


setup(name="PyICU",
      description='Python extension wrapping the ICU C++ API',
      version=VERSION,
      test_suite="test",
      url='http://pyicu.osafoundation.org/',
      author='Open Source Applications Foundation',
      ext_modules=[Extension('_PyICU',
                             [filename for filename in os.listdir(os.curdir)
                              if filename.endswith('.cpp')],
                             include_dirs=_includes,
                             extra_link_args=_lflags,
                             libraries=['icui18n', 'icuuc', 'icudata'],
                             define_macros=[('PYICU_VER', '"%s"' %(VERSION))])
                   ],
      py_modules=['PyICU'])
