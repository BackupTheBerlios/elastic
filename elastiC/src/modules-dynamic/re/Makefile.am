# ==========================================================================
# Makefile.am
# ==========================================================================
# Automake makefile template for elastiC.
# --------------------------------------------------------------------------
#   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
#
#   Created: 24 Nov 1999
#
#   $Id: Makefile.am,v 1.2 2002/06/04 20:08:27 panta Exp $
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

EXTRA_DIST = $(module_sources)
MODULE = re$(SO)

SUBDIRS = pcre

CLEANFILES = re.def re.dll libre.a $(MODULE) $(wildcard $.o)

elasticlibdir = $(prefix)/lib/elastic-$(EC_API_VERSION)
elasticlib_DATA = $(MODULE)

# dummy target to force proper automake behavior
noinst_LIBRARIES = libre.a

CFLAGS = $(MODULE_CFLAGS) -DRE_MODULE_EXPORTS
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(MODULE_CFLAGS) $(CCSHARED)

.PHONY: prepare

static_sources = \
	re_m.c		\
	re_m.h

module_sources = $(static_sources)

pcre_objects = \
	pcre/maketables.o	\
	pcre/get.o		\
	pcre/study.o		\
	pcre/pcre.o

libre_a_SOURCES = \
	$(static_sources)

%.o : %.c
	$(COMPILE) -c $< -o $@

##$(MODULE): re_m.o pcre/libpcre.a
##	$(LDSHARED) $(CFLAGS) $(LINKFORSHARED) -o $@ $^ $(LIBS) -L$(top_builddir)/elastic/.libs -lelastic
#if WIN32
#$(MODULE): re_m.o pcre/libpcre.a
#	$(MAKEDLL) $@ "" "$(LIBS) $(top_builddir)/elastic/libelastic.a" $^
#else
#$(MODULE): re_m.o pcre/libpcre.a
#	$(LDSHARED) $(LINKFORSHARED) -o $@ $^ $(LIBS)
#endif

$(MODULE): re_m.o pcre/libpcre.a
	$(MKMODULE) $(MKMODULEFLAGS) $@ $^ $(MKMODULELIBS) $(LIBS)

INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/src
