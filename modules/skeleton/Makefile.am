# ==========================================================================
# Makefile.am
# ==========================================================================
# Automake makefile template for a 'skeleton' elastiC module.
# --------------------------------------------------------------------------
#   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
#
#   Created: 25 Nov 1999
#
#   $Id: Makefile.am,v 1.1 2002/05/30 17:12:07 panta Exp $
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

EXTRA_DIST = LICENSE README.CVS autogen.sh aclocal-flags \
	$(module_sources) testskeleton.ec
MODULE = skeleton$(SO)

CLEANFILES = $(MODULE) $(wildcard *.o) $(wildcard *.ecc) \
	skeleton.def skeleton.dll libskeleton.a

CFLAGS = $(MODULE_CFLAGS)
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $(CCSHARED)

if WIN32
MLIBS = $(MKMODULELIBS)
else
MLIBS = $(MKMODULELIBS)
endif

elasticlibdir = $(prefix)/lib/elastic
elasticlib_DATA = $(MODULE)

skeleton_sources = \
	skeleton.c	\
	skeleton.h

module_sources = $(skeleton_sources)

%.o : %.c
	$(COMPILE) -c $< -o $@

$(MODULE): skeleton.o
	$(MKMODULE) $(MKMODULEFLAGS) $@ $^ $(MLIBS) $(LIBS)
