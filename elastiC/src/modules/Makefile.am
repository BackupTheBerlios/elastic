# ==========================================================================
# Makefile.am
# ==========================================================================
# Automake makefile template for elastiC.
# --------------------------------------------------------------------------
#   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
#
#   Created: 15 Nov 1999
#
#   $Id: Makefile.am,v 1.8 2005/03/22 10:09:34 panta Exp $
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

SUBDIRS = xml

CLEANFILES = $(DYNAMIC_MODULES) $(wildcard $.o)  $(wildcard $.dll)

elasticlibdir = $(prefix)/lib/elastic-$(EC_API_VERSION)
elasticlib_DATA = $(DYNAMIC_MODULES)

EXTRA_DIST = $(SOURCES_DYNAMIC_MODULES)

noinst_LTLIBRARIES = libmodules.la

CFLAGS = $(CORE_CFLAGS)

# `stream' module

if ECMODULE_STREAM_STATIC
stream_static = stream_m.c
else
stream_static =
endif

if ECMODULE_STREAM_DYNAMIC
stream_dynamic                 = stream_m.c
dynamic_modules_stream         = stream$(SO)
sources_dynamic_modules_stream = $(stream_dynamic)
else
stream_dynamic                 =
dynamic_modules_stream         =
sources_dynamic_modules_stream = $(stream_dynamic)
endif

# `filestream' module

if ECMODULE_FILESTREAM_STATIC
filestream_static = filestream_m.c
else
filestream_static =
endif

if ECMODULE_FILESTREAM_DYNAMIC
filestream_dynamic                 = filestream_m.c
dynamic_modules_filestream         = filestream$(SO)
sources_dynamic_modules_filestream = $(filestream_dynamic)
else
filestream_dynamic                 =
dynamic_modules_filestream         =
sources_dynamic_modules_filestream = $(filestream_dynamic)
endif

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

# `xml' module

if ECMODULE_XML_STATIC
xml_static =
else
xml_static =
endif

if ECMODULE_XML_DYNAMIC
xml_dynamic                   = 
dynamic_modules_xml           = xml/xml$(SO)
sources_dynamic_modules_xml   = $(xml_dynamic)
libraries_static_modules_xml  = 
else
xml_dynamic                   =
dynamic_modules_xml           =
sources_dynamic_modules_xml   = $(xml_dynamic)
libraries_static_modules_xml  = xml/libxml.a
endif

DYNAMIC_MODULES = \
	$(dynamic_modules_stream)	\
	$(dynamic_modules_filestream)	\
	$(dynamic_modules_posix)	\
	$(dynamic_modules_errno)	\
	$(dynamic_modules_xml)
SOURCES_DYNAMIC_MODULES = \
	$(sources_dynamic_modules_stream)	\
	$(sources_dynamic_modules_filestream)	\
	$(sources_dynamic_modules_posix)	\
	$(sources_dynamic_modules_errno)	\
	$(sources_dynamic_modules_xml)

static_sources = \
	list_m.c		\
	string_m.c		\
	string_m.h		\
	array_m.c		\
	array_m.h		\
	$(stream_static) 	\
	$(filestream_static)	\
	$(posix_static) 	\
	$(errno_static)		\
	$(xml_static)		\
	errno_m_body.c.incl

static_libraries =
	$(libraries_static_modules_xml)

libmodules_la_SOURCES = \
	$(static_sources)
libmodules_la_LIBADD = \
	$(static_libraries)

CFLAGS_DYNAMIC = $(MODULE_CFLAGS) -DELASTIC_EXPORTS
COMPILE_DYNAMIC = $(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS_DYNAMIC) $(CCSHARED)

if ECMODULE_STREAM_DYNAMIC
stream_m.o : stream_m.c
	$(COMPILE_DYNAMIC) -c $< -o $@

$(dynamic_modules_stream): stream_m.o
	$(MKMODULE) $(MKMODULEFLAGS) $@ $^ $(MKMODULELIBS) $(LIBS)
endif

if ECMODULE_FILESTREAM_DYNAMIC
filestream_m.o : filestream_m.c
	$(COMPILE_DYNAMIC) -c $< -o $@

$(dynamic_modules_filestream): filestream_m.o
	$(MKMODULE) $(MKMODULEFLAGS) $@ $^ $(MKMODULELIBS) $(LIBS)
endif

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
