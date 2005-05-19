# Makefile for building PyICU
#
# Supported operating systems: Linux, Mac OS X and Windows.
# See INSTALL file for requirements.
# 
# Steps to build
#   1. Edit the sections below as documented
#   2. make all
#   3. make install
#
# The install target installs the PyICU python extension in python's
# site-packages directory. 
# To successfully import the PyICU extension into Python, all required
# libraries need to be found. If the locations you chose are non-standard,
# the relevant DYLD_LIBRARY_PATH (Mac OS X), LD_LIBRARY_PATH (Linux), or 
# PATH (Windows) need to be set accordingly.
# 

# 
# You need to verify that the version of python below is correct.
# 

VERSION=0.1
ICU_VER=3.2
PYTHON_VER=2.4

# 
# You need to uncomment and edit the variables below in the section
# corresponding to your operating system.
#
# Windows drive-absolute paths need to be expressed cygwin style.
#
# PREFIX: where programs are normally installed on your system (Unix).
# PREFIX_PYTHON: where your version of python is installed.
#

# Mac OS X (Darwin)
#PREFIX=/usr/local
#PREFIX_PYTHON=/Library/Frameworks/Python.framework/Versions/$(PYTHON_VER)
#PREFIX_ICU=/usr/local/icu-$(ICU_VER)
#SWIG=$(PREFIX)/bin/swig

# Linux
#PREFIX=/usr/local
#PREFIX_PYTHON=$(PREFIX)
#PREFIX_ICU=/usr/local/icu-$(ICU_VER)
#SWIG=$(PREFIX)/bin/swig

# Windows
#PREFIX_PYTHON=/cygdrive/o/Python-2.4.1
#PREFIX_ICU=/cygdrive/o/icu-$(ICU_VER)
#SWIG=/cygdrive/c/utils/bin/swig.exe

#
# No edits required below
#

OS=$(shell uname)
ifeq ($(findstring CYGWIN,$(OS)),CYGWIN)
OS=Cygwin
endif
ifeq ($(findstring WINNT,$(OS)),WINNT)
OS=Cygwin
endif

ifeq ($(DEBUG),1)
COMP_OPT=DEBUG=1
SUFFIX=d
_SUFFIX=_d
BINDIR=debug
else
COMP_OPT=
SUFFIX=
_SUFFIX=
BINDIR=release
endif

SWIG_OPT=-DSWIG_COBJECT_TYPES -DSWIG_COBJECT_PYTHON -DPYICU_VER="'$(VERSION)'" -DICU_VER="'$(ICU_VER)'"

MODULES=errors bases string locale format dateformat currency numberformat \
        calendar

ifeq ($(OS),Darwin)
PYTHON_SITE=$(PREFIX_PYTHON)/lib/python$(PYTHON_VER)/site-packages
PYTHON_INC=$(PREFIX_PYTHON)/include/python$(PYTHON_VER)
PYICU_LIB=$(BINDIR)/_PyICU.so
PYICU_COMMON_LIB=$(BINDIR)/libPyICU.dylib
PYICU_MODULE_LIBS:=$(MODULES:%=$(BINDIR)/_PyICU_%.so)
ICU_INC=$(PREFIX_ICU)/include
ICU_LIB=$(PREFIX_ICU)/lib
ifeq ($(DEBUG),1)
CCFLAGS=-O0 -g
LDFLAGS=-g
else
CCFLAGS=-O2
LDFLAGS=
endif
else

ifeq ($(OS),Linux)
PYTHON_SITE=$(PREFIX_PYTHON)/lib/python$(PYTHON_VER)/site-packages
PYTHON_INC=$(PREFIX_PYTHON)/include/python$(PYTHON_VER)
PYICU_LIB=$(BINDIR)/_PyICU.so
PYICU_COMMON_LIB=$(BINDIR)/libPyICU.so
PYICU_MODULE_LIBS:=$(MODULES:%=$(BINDIR)/_PyICU_%.so)
ICU_INC=$(PREFIX_ICU)/include
ICU_LIB=$(PREFIX_ICU)/lib
ifeq ($(DEBUG),1)
CCFLAGS=-O0 -g -fPIC
LDFLAGS=-g
else
CCFLAGS=-O2 -fPIC
LDFLAGS=
endif
else

ifeq ($(OS),Cygwin)
PYTHON_SITE=`cygpath -aw $(PREFIX_PYTHON)/Lib/site-packages`
PYTHON_INC=`cygpath -aw $(PREFIX_PYTHON)/Include`
PYTHON_PC=`cygpath -aw $(PREFIX_PYTHON)/PC`
ICU_INC=`cygpath -aw $(PREFIX_ICU)/include`
ICU_LIB=`cygpath -aw $(PREFIX_ICU)/lib`
PYICU_LIB=$(BINDIR)/_PyICU(_SUFFIX).pyd
ifeq ($(DEBUG),1)
CCFLAGS=-O -g
LDFLAGS=-g
else
CCFLAGS=-O2
LDFLAGS=
endif
else

PYTHON=unknown
PYTHON_SITE=unknown
endif
endif
endif

DISTRIB=PyICU-$(VERSION)
LIBS=$(PYICU_COMMON_LIB) $(PYICU_LIB) $(PYICU_MODULE_LIBS)

default: all

env:
ifndef PREFIX_PYTHON
	@echo Operating system is $(OS)
	@echo You need to edit that section of the Makefile
	@false
else
	@true
endif


$(BINDIR):
	mkdir -p $(BINDIR)

%_wrap.cxx: %.i
	$(SWIG) $(SWIG_OPT) -I$(ICU_INC) -c++ -nodefault -python -modern $<


ifeq ($(OS),Darwin)

$(PYICU_COMMON_LIB): common.cpp common.h
	$(CXX) -dynamiclib -o $@ $(CCFLAGS) $(PYDBG) $(SWIG_OPT) -I$(PYTHON_INC) -I$(ICU_INC) common.cpp -undefined dynamic_lookup

$(PYICU_MODULE_LIBS): $(BINDIR)/_PyICU_%.so: %_wrap.cxx common.h
	$(CXX) -dynamic -bundle -o $@ $(CCFLAGS) $(PYDBG) $(SWIG_OPT) -I$(PYTHON_INC) -I$(ICU_INC) $< -dylib_file libPyICU.dylib:$(PYICU_COMMON_LIB) -L$(BINDIR) -lPyICU -L$(ICU_LIB) -licui18n -licuuc -licudata -undefined dynamic_lookup	

$(PYICU_LIB): PyICU_wrap.cxx $(PYICU_COMMON_LIB)
	$(CXX) -dynamic -bundle -o $@ $(CCFLAGS) $(PYDBG) $(SWIG_OPT) -I$(PYTHON_INC) -I$(ICU_INC) PyICU_wrap.cxx -dylib_file libPyICU.dylib:$(PYICU_COMMON_LIB) -L$(BINDIR) -lPyICU -L$(ICU_LIB) -licui18n -licuuc -licudata -undefined dynamic_lookup
else

ifeq ($(OS),Linux)

$(PYICU_COMMON_LIB): common.cpp common.h
	$(CXX) -shared -o $@ $(CCFLAGS) $(PYDBG) $(SWIG_OPT) -I$(PYTHON_INC) -I$(ICU_INC) common.cpp

$(PYICU_MODULE_LIBS): $(BINDIR)/_PyICU_%.so: %_wrap.cxx common.h
	$(CXX) -shared -o $@ $(CCFLAGS) $(PYDBG) $(SWIG_OPT) -I$(PYTHON_INC) -I$(ICU_INC) $< -L$(BINDIR) -lPyICU -L$(ICU_LIB) -licui18n -licuuc -licudata

$(PYICU_LIB): PyICU_wrap.cxx $(PYICU_COMMON_LIB)
	$(CXX) -shared -o $@ $(CCFLAGS) $(PYDBG) $(SWIG_OPT) -I$(PYTHON_INC) -I$(ICU_INC) PyICU_wrap.cxx -L$(BINDIR) -lPyICU -L$(ICU_LIB) -licui18n -licuuc -licudata
else

ifeq ($(OS),Cygwin)
endif
endif
endif


all: env $(BINDIR) $(LIBS)
	@echo build of $(BINDIR) complete

install: all
	mkdir -p $(PYTHON_SITE)
	install PyICU.py $(MODULES:%=PyICU_%.py) $(PYTHON_SITE)
	install $(PYICU_LIB) $(PYICU_MODULE_LIBS) $(PYTHON_SITE)
	install $(PYICU_COMMON_LIB) $(PREFIX)/lib

clean:
	rm -rf $(BINDIR)
	rm -f PyICU.py* PyICU_wrap.cxx
	rm -f $(MODULES:%=%_wrap.cxx) $(MODULES:%=PyICU_%.py*)
	rm -f $(PYICU_MODULE_LIBS)