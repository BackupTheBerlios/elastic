# ==========================================================================
# Makefile.am
# ==========================================================================
# Automake makefile template for elastiC.
# --------------------------------------------------------------------------
#   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
#
#   Created: 15 Nov 1999
#
#   $Id: Makefile.am,v 1.6 2002/06/12 16:00:05 panta Exp $
# --------------------------------------------------------------------------
#    Copyright (C) 1999-2002 Marco Pantaleoni. All rights reserved.
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

#SUBDIRS =

# Directory where sources are, from where we are.
srcdir = @srcdir@
VPATH = @srcdir@

CLEANFILES = $(DYNAMIC_MODULES) $(wildcard $.o)  $(wildcard $.dll)

elasticlibdir = $(prefix)/lib/elastic-$(EC_API_VERSION)
elasticlib_DATA = $(DYNAMIC_MODULES)

EXTRA_DIST = $(SOURCES_DYNAMIC_MODULES)

noinst_LTLIBRARIES = libmodules.la

CFLAGS = $(CORE_CFLAGS)

# `posix' module

if ECMODULE_POSIX_STATIC
posix_static = posix_m.c
else
posix_static =
endif

if ECMODULE_POSIX_DYNAMIC
posix_dynamic                 = posix_m.c
dynamic_modules_posix         = posix$(SO)
sources_dynamic_modules_posix = $(posix_dynamic)
else
posix_dynamic                 =
dynamic_modules_posix         =
sources_dynamic_modules_posix = $(posix_dynamic)
endif

# `errno' module

if ECMODULE_ERRNO_STATIC
errno_static = errno_m.c
else
errno_static =
endif

if ECMODULE_ERRNO_DYNAMIC
errno_dynamic                 = errno_m.c
dynamic_modules_errno         = errno$(SO)
sources_dynamic_modules_errno = $(errno_dynamic)
else
errno_dynamic                 =
dynamic_modules_errno         =
sources_dynamic_modules_errno = $(errno_dynamic)
endif

DYNAMIC_MODULES = $(dynamic_modules_posix) $(dynamic_modules_errno)
SOURCES_DYNAMIC_MODULES = $(sources_dynamic_modules_posix) $(sources_dynamic_modules_errno)

static_sources = \
	list_m.c	\
	string_m.c	\
	string_m.h	\
	array_m.c	\
	array_m.h	\
	$(posix_static) \
	$(errno_static)	\
	errno_m_body.c.incl	\
	stream_m.c

libmodules_la_SOURCES = \
	$(static_sources)
libmodules_la_LIBADD =

CFLAGS_DYNAMIC = $(MODULE_CFLAGS) -DELASTIC_EXPORTS
COMPILE_DYNAMIC = $(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS_DYNAMIC) $(CCSHARED)

if ECMODULE_POSIX_DYNAMIC
posix_m.o : posix_m.c
	$(COMPILE_DYNAMIC) -c $< -o $@

$(dynamic_modules_posix): posix_m.o
	$(MKMODULE) $(MKMODULEFLAGS) $@ $^ $(MKMODULELIBS) $(LIBS)
endif

if ECMODULE_ERRNO_DYNAMIC
errno_m.o : errno_m.c
	$(COMPILE_DYNAMIC) -c $< -o $@

$(dynamic_modules_errno): errno_m.o
	$(MKMODULE) $(MKMODULEFLAGS) $@ $^ $(MKMODULELIBS) $(LIBS)
endif

INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/src
