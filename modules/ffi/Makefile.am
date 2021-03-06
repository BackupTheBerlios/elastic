# ==========================================================================
# Makefile.am
# ==========================================================================
# Automake makefile template for the 'ffi' elastiC module.
# --------------------------------------------------------------------------
#   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
#
#   Created: 27 Sep 2001
#
#   $Id: Makefile.am,v 1.1 2002/05/30 17:00:14 panta Exp $
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

EXTRA_DIST = LICENSE TODO README.CVS autogen.sh aclocal-flags \
	$(module_sources)		\
	$(example_sources)
MODULE = ffi$(SO)

CLEANFILES = $(MODULE) $(wildcard *.o) $(wildcard *.ecc) \
	ffi.def ffi.dll libffi.a

EC  = ec
ECC = ecc

CFLAGS = $(MODULE_CFLAGS) -DFFI_MODULE_EXPORTS
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $(CCSHARED)
ECCOMPILE = $(ECC)

if WIN32
MLIBS = $(MKMODULELIBS) -lffi
else
MLIBS = $(MKMODULELIBS) -lffi
endif

elasticlibdir = $(prefix)/lib/elastic
elasticlib_DATA = $(MODULE)

ffi_sources = \
	ffi.c		\
	ffi.h

module_sources = $(ffi_sources)

example_sources = \
	testffi.ec

%.o : %.c
	$(COMPILE) -c $< -o $@

%.ecc : %.ec
	$(ECCOMPILE) -o $@ $<

$(MODULE): ffi.o
	$(MKMODULE) $(MKMODULEFLAGS) $@ $^ $(MLIBS) $(LIBS)

# Explicit elastiC module dependencies (no automake subject *sigh*)

testffi.ecc: testffi.ec $(MODULE)
