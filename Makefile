#  $Id$
#  -----------------------------------------------------------------------
#  Copyright  (c) Joerg Desch <jd@die-deschs.de>
#  -----------------------------------------------------------------------
#  PROJECT.: ezV24 -- easy RS232/V24 access
#  AUTHOR..: Joerg Desch <jd@die-deschs.de>
#  COMPILER: g++ 2.95.x / Linux
#
#

# the version of the library
VERSION = 0.1
# the release of the library; a change here means, that the API has
# changes. This number is the major number of the above version
SORELEASE = 0
# the patchlevel is the lowest release information. It is incremented
# with each released bugfix.
PATCHLEVEL = 1
# the base name of the library
SOBASE = ezV24

# define the destination OS (currently only linux)
PLATFORM=__LINUX__

# the base path where the file should be installed to.
PREFIX = /usr/local

# an additional prefix for building RPM packages. NOTE: don't forget to add a
# trailing slash!
DESTDIR =

# generate the name of the output file in dependence of the development state.
#
ifeq "${RELEASE}" "DEBUG"
NAME = lib$(SOBASE).so.$(SORELEASE).dbg
else
NAME = lib$(SOBASE).so.$(VERSION)
endif
SONAME = lib$(SOBASE).so.$(SORELEASE)
LIBNAME = lib$(SOBASE)-$(SORELEASE)_s.a
PLAINNAME = lib$(SOBASE).so

# basename of the project
PROJECTNAME = libezV24-$(VERSION).$(PATCHLEVEL)

OBJS = ezV24.o snprintf.o
LIBS = 


ifeq "${RELEASE}" "DEBUG"
C_OPT  = -O2
C_FLAG = -c -Wall -fPIC $(C_OPT) -D$(PLATFORM) $(INCDIR)
C_DEFS = -DDEBUG -DBETA
LFLAGS = $(LIBDIR)
else 
ifeq "${RELEASE}" "BETA"
C_OPT  = -O2
C_FLAG = -c -Wall -fPIC $(C_OPT) -D$(PLATFORM) $(INCDIR)
C_DEFS = -DBETA
LFLAGS = $(LIBDIR)
else
C_OPT  = -O2
C_FLAG = -c -Wall -fPIC $(C_OPT) -D$(PLATFORM) $(INCDIR)
C_DEFS = -DFINAL
LFLAGS = -s $(LIBDIR)
endif
endif

# tools to build the static library
ARFLAGS = cru
AR = ar
RANLIB = ranlib

# concatinate the compile flags
CFLAGS = $(C_FLAG) $(C_DEFS)



# ------------------------------------------------------------------------
# AUTOMATISCHE COMPILE-ANWEISUNGEN
# ------------------------------------------------------------------------

.c.o:
		gcc $(CFLAGS) $<


# --------------------------------------------------------------------------
# ANHÄNGIGKEITEN
# --------------------------------------------------------------------------

all:		shared static test-v24

shared:		$(NAME)

static:		$(LIBNAME)



$(NAME):	$(OBJS)
		gcc -shared -W1,soname,$(SONAME) -o $(NAME) $(OBJS)

$(LIBNAME):	$(OBJS)
		$(AR) $(ARFLAGS) $(LIBNAME) $(OBJS)
		$(RANLIB) $(LIBNAME)


# Abhängigkeiten des Source, jedoch dann ohne Generierungsanweisung, wenn
# die Extension durch obigen Automatismus abgedeckt wird!
#

ezV24.o:	ezV24.c ezV24.h ezV24_config.h snprintf.h

snprintf.o:	snprintf.c snprintf.h



# This install / uninstall the library into the given directories.
#

install:
		install -d -m 755 $(DESTDIR)$(PREFIX)/include/$(SOBASE)/
		install -d -m 755 $(DESTDIR)$(PREFIX)/lib/
		install -m 644 ezV24.h $(DESTDIR)$(PREFIX)/include/$(SOBASE)/
		install -m 644 -s $(LIBNAME) $(DESTDIR)$(PREFIX)/lib/$(LIBNAME)
		install -m 755 -s $(NAME) $(DESTDIR)$(PREFIX)/lib/$(NAME)
		rm -f $(DESTDIR)$(PREFIX)/lib/$(SONAME) $(DESTDIR)$(PREFIX)/lib/$(PLAINNAME)
		ln -s $(PREFIX)/lib/$(NAME) $(DESTDIR)$(PREFIX)/lib/$(SONAME);\
		ln -s $(PREFIX)/lib/$(SONAME) $(DESTDIR)$(PREFIX)/lib/$(PLAINNAME);\

uninstall:
		rm -f $(PREFIX)/include/ezV24/*
		rmdir $(PREFIX)/include/ezV24
		rm -f $(PREFIX)/lib/$(LIBNAME)
		rm -f $(PREFIX)/lib/$(NAME)
		rm -f $(PREFIX)/lib/$(SONAME) $(PREFIX)/lib/$(PLAINNAME)
		ldconfig


# This entry is for packing a distribution tarball
#
tarball:	api-ref
		if test -d $(PROJECTNAME); then\
		  rm -fR $(PROJECTNAME)/*;\
		  rmdir $(PROJECTNAME);\
		fi
		mkdir $(PROJECTNAME)
		cp ezV24.h ezV24_config.h ezV24.c $(PROJECTNAME)/
		cp snprintf.h snprintf.c test-v24.c $(PROJECTNAME)/
		cp Makefile Makefile.cygwin README $(PROJECTNAME)/
		cp AUTHORS HISTORY COPY* BUGS ChangeLog $(PROJECTNAME)/
		cp doc++.conf manual.dxx $(PROJECTNAME)/
		cp -r --parents api-html $(PROJECTNAME)/
		tar cfz $(PROJECTNAME).tar.gz $(PROJECTNAME)
		rm -fR $(PROJECTNAME)/*
		rmdir $(PROJECTNAME)

# build the api reference
#
api-ref:	doc++.conf manual.dxx ezV24.h
		doc++

# The ezV24-Test program. To compile the dynamic link version, the
# library must be installed first! To avoid this, i use the static lib!
#	gcc -o test-v24 -Wall test-v24.c -l$(SOBASE)
#
test-v24:	test-v24.c ezV24.h $(LIBNAME)
		gcc -o test-v24 -Wall test-v24.c -L./ $(LIBNAME)


# --------------------------------------------------------------------------
# ANDERE AUFGABEN
# --------------------------------------------------------------------------

clean:
		rm -f *.o core

clean-all:
		rm -f *.o core test-v24 $(NAME) $(LIBNAME)
		rm -f $(PROJECTNAME).tar.gz
		rm -f api-html/*
		rmdir api-html


# --[end of file]-----------------------------------------------------------

