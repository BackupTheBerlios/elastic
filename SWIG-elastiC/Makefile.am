# ==========================================================================
# Makefile.am
# ==========================================================================
# Automake makefile template for SWIG for elastiC.
# --------------------------------------------------------------------------
#   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
#
#   Created: 29 Dec 1999
#
#   $Id: Makefile.am,v 1.1 2002/05/31 12:29:59 panta Exp $
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

EXTRA_DIST = LICENSE autogen.sh aclocal-flags \
	$(ecswig_sources)
#SUBDIRS = examples

CLEANFILES = $(wildcard *.o) $(wildcard *_wrap.*) $(wildcard *.ecc) \
	$(SWIG_PROG)

bin_PROGRAMS = $(SWIG_PROG)

COMPILE    = $(CC)  $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $(SWIG_CFLAGS)
CXXCOMPILE = $(CXX) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $(SWIG_CFLAGS)
MODCOMPILE = $(CC)  $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $(CCSHARED)

SWIG_PROG    = ecswig

swiglibdir   = $(SWIG_LIB)/elastic
swiglib_DATA = elastic.swg header.swg

ecswig_sources = \
	elastic.h	\
	main.cxx	\
	elastic.cxx	\
	elastic.swg	\
	header.swg

ecswig_SOURCES = \
	$(ecswig_sources)
ecswig_LDADD = $(SWIG_LIBS)

%.o : %.c
	$(COMPILE) -c $< -o $@

ecswig.o: main.o elastic.o
main.o: main.cxx
elastic.o: elastic.cxx

examples::
	cd examples && $(MAKE) all

dist-hook:
	cd examples && $(MAKE) distclean
	cd $(top_srcdir)
	cp -a examples $(distdir)

clean-local:
	cd examples && $(MAKE) clean
	cd $(top_srcdir)
