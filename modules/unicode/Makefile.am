# ==========================================================================
# Makefile.am
# ==========================================================================
# Automake makefile template for the 'unicode' elastiC module.
# --------------------------------------------------------------------------
#   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
#
#   Created: 18 Oct 2001
#
#   $Id: Makefile.am,v 1.1 2002/05/30 18:39:45 panta Exp $
# --------------------------------------------------------------------------
#    Copyright (C) 2001-2002 Marco Pantaleoni. All rights reserved.
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

EXTRA_DIST = LICENSE README.CVS autogen.sh aclocal-flags \
	$(module_sources) testunicode.ec
MODULE = unicode$(SO)

CLEANFILES = $(MODULE) $(wildcard *.o) $(wildcard *.ecc) \
	unicode.def unicode.dll libunicode.a

EC  = ec
ECC = ecc

CFLAGS = $(MODULE_CFLAGS) -DUNICODE_MODULE_EXPORTS
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $(CCSHARED)

if WIN32
MLIBS = $(MKMODULELIBS) -lunicode
else
MLIBS = $(MKMODULELIBS) -lunicode
endif

elasticlibdir = $(prefix)/lib/elastic
elasticlib_DATA = $(MODULE)

unicode_sources = \
	unicode.c	\
	unicode.h

module_sources = $(unicode_sources)

%.o : %.c
	$(COMPILE) -c $< -o $@

%.ecc : %.ec
	$(ECCOMPILE) -o $@ $<

$(MODULE): unicode.o
	$(MKMODULE) $(MKMODULEFLAGS) $@ $^ $(MLIBS) $(LIBS)
