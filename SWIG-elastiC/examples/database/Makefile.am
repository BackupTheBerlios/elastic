# ==========================================================================
# Makefile.am
# ==========================================================================
# Automake makefile template for SWIG for elastiC.
# --------------------------------------------------------------------------
#   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
#
#   Created: 29 Dec 1999
#
#   $Id: Makefile.am,v 1.1 2002/05/31 12:30:00 panta Exp $
# --------------------------------------------------------------------------
#    Copyright (C) 1999 Marco Pantaleoni. All rights reserved.
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

EXTRA_DIST = README $(example_sources)

CLEANFILES = $(wildcard *.o) $(wildcard *_wrap.*) $(wildcard *.ecc) \
	$(EXAMPLE_MODULE)

COMPILE    = $(CC)  $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $(SWIG_CFLAGS)
CXXCOMPILE = $(CXX) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $(SWIG_CFLAGS)
MODCOMPILE = $(CC)  $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $(CCSHARED)

SWIG_PROG      = $(top_srcdir)/ecswig
SWIG_FLAGS     = -I$(top_srcdir) -v
EXAMPLE_MODULE = db$(SO)

#elasticlibdir = $(prefix)/lib/elastic
#elasticlib_DATA = $(EXAMPLE_MODULE)
elasticlibdir = ./dummy
elasticlib_DATA = $(EXAMPLE_MODULE)

example_sources = \
	test.ec		\
	db.h		\
	db.c		\
	db.i

%.o : %.c
	$(COMPILE) -c $< -o $@

$(EXAMPLE_MODULE): db.o db_wrap.o
	$(LDSHARED) $(CFLAGS) $(LINKFORSHARED) -o $@ $^ $(LIBS)

.PHONY: example
example: $(EXAMPLE_MODULE)
db.o: db.c db.i
	$(MODCOMPILE) -c db.c
db_wrap.o: db_wrap.c

db_wrap.c: db.i $(SWIG_PROG)
	$(SWIG_PROG) $(SWIG_FLAGS) db.i
