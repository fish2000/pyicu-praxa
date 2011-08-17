
import os, sys

try:
    from setuptools import setup, Extension
except ImportError:
    from distutils.core import setup, Extension


VERSION = '1.2'

INCLUDES = {
    'darwin': ['/usr/local/include'],
    'linux': [],
    'freebsd7': ['/usr/local/include'],
    'win32': ['c:/icu/include'],
    'sunos5': [],
}

CFLAGS = {
    'darwin': ['-Wno-write-strings', '-DPYICU_VER="%s"' %(VERSION)],
    'linux': ['-DPYICU_VER="%s"' %(VERSION)],
    'freebsd7': ['-DPYICU_VER="%s"' %(VERSION)],
    'win32': ['/Zc:wchar_t', '/EHsc', '/DPYICU_VER=\\"%s\\"' %(VERSION)],
    'sunos5': ['-DPYICU_VER="%s"' %(VERSION)],
}

LFLAGS = {
    'darwin': ['-L/usr/local/lib'],
    'linux': [],
    'freebsd7': ['-L/usr/local/lib'],
    'win32': ['/LIBPATH:c:/icu/lib'],
    'sunos5': [],
}

LIBRARIES = {
    'darwin': ['icui18n', 'icuuc', 'icudata'],
    'linux': ['icui18n', 'icuuc', 'icudata'],
    'freebsd7': ['icui18n', 'icuuc', 'icudata'],
    'win32': ['icuin', 'icuuc', 'icudt'],
    'sunos5': ['icui18n', 'icuuc', 'icudata'],
}

platform = sys.platform
if platform.startswith('linux'):
    platform = 'linux'

if 'PYICU_INCLUDES' in os.environ:
    _includes = os.environ['PYICU_INCLUDES'].split(os.pathsep)
else:
    _includes = INCLUDES[platform]

if 'PYICU_CFLAGS' in os.environ:
    _cflags = os.environ['PYICU_CFLAGS'].split(os.pathsep)
else:
    _cflags = CFLAGS[platform]

if 'PYICU_LFLAGS' in os.environ:
    _lflags = os.environ['PYICU_LFLAGS'].split(os.pathsep)
else:
    _lflags = LFLAGS[platform]

if 'PYICU_LIBRARIES' in os.environ:
    _libraries = os.environ['PYICU_LIBRARIES'].split(os.pathsep)
else:
    _libraries = LIBRARIES[platform]


setup(name="PyICU",
      description='Python extension wrapping the ICU C++ API',
      long_description=open('README').read(),
      version=VERSION,
      test_suite="test",
      url='http://pyicu.osafoundation.org/',
      author='Open Source Applications Foundation',
      ext_modules=[Extension('_icu',
                             [filename for filename in os.listdir(os.curdir)
                              if filename.endswith('.cpp')],
                             include_dirs=_includes,
                             extra_compile_args=_cflags,
                             extra_link_args=_lflags,
                             libraries=_libraries)],
      py_modules=['icu', 'PyICU', 'docs'],
)
