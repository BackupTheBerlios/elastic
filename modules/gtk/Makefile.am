# ==========================================================================
# Makefile.am
# ==========================================================================
# Automake makefile template for 'gdk'/'gtk' elastiC modules.
# --------------------------------------------------------------------------
#   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
#
#   Created: 26 Dec 1999
#
#   $Id: Makefile.am,v 1.1 2002/05/30 12:41:45 panta Exp $
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
	$(modules_sources) $(interfacegen_sources)			\
	gdk.defs gtk.defs 						\
	$(allbuilt_sources)						\
	testgtk.ec testgdk.ec fileview.ec

GDKMODULE = gdk$(SO)
GTKMODULE = gtk$(SO)
MODULES = $(GDKMODULE) $(GTKMODULE)

CLEANFILES = $(MODULES) $(wildcard *.o) $(wildcard *.ecc)

EC  = ec
ECC = ecc

COMPILE   = $(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $(CCSHARED)
ECCOMPILE = $(ECC)

elasticlibdir = $(prefix)/lib/elastic
elasticlib_DATA = $(MODULES)

interfacegen_sources = \
	scmparse.ec		\
	InterfaceGen.ec		\
	Util.ec			\
	Data.ec			\
	CGenerator.ec		\
	CodeEmitter.ec

interfacegen_OBJECTS = \
	scmparse.ecc		\
	InterfaceGen.ecc	\
	Util.ecc		\
	Data.ecc		\
	CGenerator.ecc		\
	CodeEmitter.ecc

gdkbuilt_sources = \
	gdkglue.h		\
	gdkglue.c

gtkbuilt_sources = \
	gdkglue.h		\
	gtkglue.h		\
	gtkglue.c

allbuilt_sources = \
	gdkglue.h		\
	gtkglue.h		\
	gdkglue.c		\
	gtkglue.c

gdk_sources = \
	ecgtk.h			\
	ecgtk.c			\
	gdk_m.c			\
	$(gdkbuilt_sources)

gtk_sources = \
	ecgtk.h			\
	ecgtk.c			\
	gtk_m.c			\
	$(gtkbuilt_sources)

modules_sources = \
	$(allbuilt_sources)	\
	ecgtk.h			\
	ecgtk.c			\
	gdk_m.c			\
	gtk_m.c

%.o : %.c
	$(COMPILE) -c $< -o $@

%.ecc : %.ec
	$(ECCOMPILE) -o $@ $<

$(GDKMODULE): gdk_m.o
	$(LDSHARED) $(CFLAGS) $(LINKFORSHARED) -o $@ $^ $(LIBS)

$(GTKMODULE): gtk_m.o
	$(LDSHARED) $(CFLAGS) $(LINKFORSHARED) -o $@ $^ $(LIBS)

gdk_m.o: $(gdk_sources)
gtk_m.o: $(gtk_sources)

$(allbuilt_sources): $(interfacegen_OBJECTS)
	$(EC) InterfaceGen.ecc gtk gtk.defs
	@echo -e '\007'

# Explicit elastiC module dependencies (no automake support *sigh*)

InterfaceGen.ecc: InterfaceGen.ec scmparse.ecc Data.ecc CGenerator.ecc Util.ecc CodeEmitter.ecc

Data.ecc: Data.ec Util.ecc

scmparse.ecc: scmparse.ec

CGenerator.ecc: CGenerator.ec Util.ecc CodeEmitter.ecc

Util.ecc: Util.ec

CodeEmitter.ecc: CodeEmitter.ec
