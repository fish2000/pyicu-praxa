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

VERSION=0.5
ICU_VER=3.4
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
#PREFIX_FRAMEWORKS=/Library/Frameworks
#PREFIX_PYTHON=$(PREFIX_FRAMEWORKS)/Python.framework/Versions/$(PYTHON_VER)
#PREFIX_ICU=/usr/local/icu-$(ICU_VER)

# Linux
#PREFIX=/usr/local
#PREFIX_PYTHON=$(PREFIX)
#PREFIX_ICU=/usr/local/icu-$(ICU_VER)

# Windows
#PREFIX_PYTHON=/cygdrive/o/osaf/release/bin
#PREFIX_ICU=/cygdrive/o/icu-$(ICU_VER)

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

MODULES=common \
        errors bases locale iterators format dateformat numberformat \
        calendar collator

ifeq ($(OS),Darwin)
PYTHON_SITE=$(PREFIX_PYTHON)/lib/python$(PYTHON_VER)/site-packages
PYTHON_INC=$(PREFIX_PYTHON)/include/python$(PYTHON_VER)
PYICU_LIB=$(BINDIR)/_PyICU.so
ICU_INC=$(PREFIX_ICU)/include
ICU_LIB=$(PREFIX_ICU)/lib
ifeq ($(DEBUG),1)
CCFLAGS=-O0 -g
LDFLAGS=-g
else
CCFLAGS=-O2
LDFLAGS=
endif
OBJS=$(MODULES:%=$(BINDIR)/%.o)
else

ifeq ($(OS),Linux)
PYTHON_SITE=$(PREFIX_PYTHON)/lib/python$(PYTHON_VER)/site-packages
PYTHON_INC=$(PREFIX_PYTHON)/include/python$(PYTHON_VER)
PYICU_LIB=$(BINDIR)/_PyICU.so
ICU_INC=$(PREFIX_ICU)/include
ICU_LIB=$(PREFIX_ICU)/lib
ifeq ($(DEBUG),1)
CCFLAGS=-O0 -g -fPIC
LDFLAGS=-g
else
CCFLAGS=-O2 -fPIC
LDFLAGS=
endif
OBJS=$(MODULES:%=$(BINDIR)/%.o)
else

ifeq ($(OS),Cygwin)
PYTHON_SITE=`cygpath -aw $(PREFIX_PYTHON)/Lib/site-packages`
PYTHON_INC=`cygpath -aw $(PREFIX_PYTHON)/Include`
PYTHON_PC=`cygpath -aw $(PREFIX_PYTHON)/PC`
PYICU_LIB=$(BINDIR)/_PyICU$(_SUFFIX).pyd
ICU_INC=`cygpath -aw $(PREFIX_ICU)/include`
ICU_LIB=`cygpath -aw $(PREFIX_ICU)/lib`
CC=cl
CXX=cl
LD=link
ifeq ($(DEBUG),1)
CCFLAGS=/nologo /GX /Od /Zi /LDd /MDd /D_DEBUG
LDFLAGS=/INCREMENTAL:no /OPT:noref /DEBUG
else
CCFLAGS=/nologo /GX /Ox /LD /MD
LDFLAGS=/INCREMENTAL:no /OPT:noref
endif
OBJS=$(MODULES:%=$(BINDIR)/%.obj)
else

PYTHON=unknown
PYTHON_SITE=unknown
endif
endif
endif

DISTRIB=PyICU-$(VERSION)
LIBS=$(PYICU_LIB)

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

ifeq ($(OS),Darwin)

$(OBJS): $(BINDIR)/%.o: %.cpp
	$(CXX) -c -o $@ $(CCFLAGS) $(PYDBG) -I$(PYTHON_INC) -I$(ICU_INC) $<

$(PYICU_LIB): $(OBJS) _PyICU.cpp
	$(CXX) -dynamic -bundle -o $@ $(CCFLAGS) $(PYDBG) -I$(PYTHON_INC) -I$(ICU_INC) _PyICU.cpp $(OBJS) -L$(ICU_LIB) -licui18n -licuuc -licudata -F$(PREFIX_FRAMEWORKS) -framework Python
else

ifeq ($(OS),Linux)

$(OBJS): $(BINDIR)/%.o: %.cpp
	$(CXX) -c -o $@ $(CCFLAGS) $(PYDBG) -I$(PYTHON_INC) -I$(ICU_INC) $<

$(PYICU_LIB): $(OBJS) PyICU.cpp
	$(CXX) -shared -o $@ $(CCFLAGS) $(PYDBG) -I$(PYTHON_INC) -I$(ICU_INC) _PyICU.cpp $(OBJS) -L$(ICU_LIB) -licui18n -licuuc -licudata
else

ifeq ($(OS),Cygwin)

$(OBJS): $(BINDIR)/%.obj: %.cpp
	$(CXX) $(CCFLAGS) /I $(PYTHON_INC) /I $(ICU_INC) $(PYDBG) /Tp$< /Fe$@

$(PYICU_LIB): $(OBJS) _PyICU.cpp
	$(CXX) $(CCFLAGS) /I $(PYTHON_INC) /I $(ICU_INC) $(PYDBG) /Tp_PyICU.cpp /Fe$@ /link $(LDFLAGS) $(OBJS) /LIBPATH:`cygpath -aw $(PREFIX_PYTHON)/libs` /LIBPATH:`cygpath -aw $(PREFIX_ICU)/lib` icuuc$(SUFFIX).lib icuin$(SUFFIX).lib icudt.lib

endif
endif
endif


all: env $(BINDIR) $(LIBS)
	@echo build of $(BINDIR) complete

install: all
	mkdir -p $(PYTHON_SITE)
	install PyICU.py $(PYTHON_SITE)
	install $(PYICU_LIB) $(PYTHON_SITE)

clean:
	rm -rf $(BINDIR)

realclean: clean
	rm -rf $(DISTRIB)

distrib: all
	mkdir -p $(DISTRIB)/python
	install PyICU.py $(DISTRIB)/python
	install $(PYICU_LIB) $(DISTRIB)/python
	tar --exclude .svn -cf - test/*.py | tar -C $(DISTRIB) -xf -
	install README $(DISTRIB)
	install CHANGES $(DISTRIB)
	install CREDITS $(DISTRIB)
	tar -cvzf $(DISTRIB).tar.gz $(DISTRIB)
