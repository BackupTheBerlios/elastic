# ==========================================================================
# Makefile.am
# ==========================================================================
# Automake makefile template for elastiC.
# --------------------------------------------------------------------------
#   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
#
#   Created : 24 Nov 1999
#   Modified: 19 Oct 2001
#
#   $Id: Makefile.am,v 1.1 2002/05/23 21:19:34 panta Exp $
# --------------------------------------------------------------------------
#    Copyright (C) 1999-2001 Marco Pantaleoni. All rights reserved.
#
#  The contents of this file are subject to the elastiC License version 1.0
#  (the "elastiC License"); you may not use this file except in compliance
#  with the elastiC License. You may obtain a copy of the elastiC License at
#  http://www.elasticworld.org/LICENSE
#
#  IN NO EVENT SHALL THE AUTHOR OR DISTRIBUTORS BE LIABLE TO ANY PARTY
#  FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
#  ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
#  DERIVATIVES THEREOF, EVEN IF THE AUTHOR HAS BEEN ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
#
#  THE AUTHOR AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE
#  IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHOR AND DISTRIBUTORS HAVE
#  NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
#  MODIFICATIONS.
#
#  See the elastiC License for the specific language governing rights and
#  limitations under the elastiC License.
# ==========================================================================

# Directory where sources are, from where we are.
srcdir = @srcdir@
VPATH = @srcdir@

EXTRA_DIST = AUTHORS COPYING ChangeLog INSTALL LICENCE NEWS NON-UNIX-USE README \
	Makefile-libtool.am Makefile.in.orig configure.in configure \
	pcre.in pcre.h pcre-config.in pcre-config RunTest.in RunTest dll.mk \
	perltest perltest8 pcregrep.c pcredemo.c

noinst_LIBRARIES = libpcre.a libpcreposix.a
#noinst_LTLIBRARIES = libpcre.la libpcreposix.la
if CROSSCOMPILING
else
noinst_PROGRAMS = pcretest dftables
endif

#UTF8    = @UTF8@
#NEWLINE = @NEWLINE@
UTF8    = -DSUPPORT_UTF8
#NEWLINE = -DNEWLINE=13
#NEWLINE = -DNEWLINE=10

if WIN32
# don't compile as a DLL (-DSTATIC)
CFLAGS  = $(MODULE_CFLAGS) -D_WIN32 -DSTATIC
else
CFLAGS  = $(MODULE_CFLAGS)
endif
COMPILE = $(CC) $(CFLAGS) $(UTF8) $(NEWLINE) $(CCSHARED)

# These are the version numbers for the shared libraries

PCRELIBVERSION = @PCRE_LIB_VERSION@
PCREPOSIXLIBVERSION = @PCRE_POSIXLIB_VERSION@

static_sources = \
	maketables.c	\
	get.c		\
	study.c		\
	pcre.c		\
	chartables.c	\
	pcre.h		\
	internal.h

posix_static_sources = \
	pcreposix.c	\
	pcreposix.h

libpcre_a_SOURCES = \
	$(static_sources)
#libpcre_la_LDFLAGS = \
#        -version-info $(PCRELIBVERSION)         \
#        -export-dynamic                         \
#        -rpath $(LIBDIR)

libpcreposix_a_SOURCES = \
	$(posix_static_sources)
#libpcreposix_la_LDFLAGS = \
#        -version-info $(PCREPOSIXLIBVERSION)    \
#        -export-dynamic                         \
#        -rpath $(LIBDIR)

if CROSSCOMPILING
else
pcretest_SOURCES = \
	pcretest.c
pcretest_LDADD = libpcre.a libpcreposix.a

dftables_SOURCES = \
	dftables.c 	\
	config.h	\
	pcre.h		\
	internal.h
dftables_LDADD =
endif

BUILT_SOURCES = \
	chartables.c

if CROSSCOMPILING
else
chartables.c: dftables
	./dftables >chartables.c
endif

%.o: %.c
	$(COMPILE) -c $< -o $@

dist-hook:
	mkdir -p $(distdir)/doc
	cp -p $(srcdir)/doc/* $(distdir)/doc
	mkdir -p $(distdir)/testdata
	cp -p $(srcdir)/testdata/* $(distdir)/testdata
