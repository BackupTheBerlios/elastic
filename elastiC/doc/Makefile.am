# ==========================================================================
# Makefile.am
# ==========================================================================
# Automake makefile template for elastiC.
# --------------------------------------------------------------------------
#   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
#
#   Created: 15 Nov 1999
#
#   $Id: Makefile.am,v 1.1 2002/05/23 21:16:19 panta Exp $
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

EXTRA_DIST = TUTORIAL.struct document.reg elastic.sdm			\
	ec.sdf ecc.sdf ecdump.sdf elastic.sdf elastic-config.sdf	\
	quickref.sdf internal-architecture.sdf				\
	$(man_MANS)							\
	$(html_DATA)							\
	$(sgml_DATA)							\
	$(pod_DATA)							\
	$(txt_DATA)							\
	mingw.sh

include $(top_srcdir)/build/rules.mk

elasticdir = $(datadir)/elastic
htmldir    = $(datadir)/html
imagesdir  = $(htmldir)/images
sgmldir    = $(datadir)/sgml
poddir     = $(datadir)/pod
txtdir     = $(datadir)/txt
exampledir = $(elasticdir)/examples

man_MANS  = ec.1    ecc.1    ecdump.1    elastic.1    elastic-config.1    quickref.1    internal-architecture.1
html_DATA = ec.html ecc.html ecdump.html elastic.html elastic-config.html quickref.html internal-architecture.html
sgml_DATA = ec.sgml ecc.sgml ecdump.sgml elastic.sgml elastic-config.sgml quickref.sgml internal-architecture.sgml
pod_DATA  = ec.pod  ecc.pod  ecdump.pod  elastic.pod  elastic-config.pod  quickref.pod  internal-architecture.pod
txt_DATA  = ec.txt  ecc.txt  ecdump.txt  elastic.txt  elastic-config.txt  quickref.txt  internal-architecture.txt  INSTALL.txt

##info_TEXINFOS =

all-local:
	cp -p INSTALL.txt $(top_builddir)/INSTALL

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(imagesdir)
	for p in $(srcdir)/images/*; do \
	  $(INSTALL_DATA) $$p $(DESTDIR)$(imagesdir)/`basename $$p`; \
	done
	$(mkinstalldirs) $(DESTDIR)$(exampledir)
	for p in $(srcdir)/examples/*; do \
	  $(INSTALL_DATA) $$p $(DESTDIR)$(exampledir)/`basename $$p`; \
	done

dist-hook:
	cp -p INSTALL.txt $(distdir)/../INSTALL
	mkdir -p $(distdir)/images
	cp -p $(srcdir)/images/* $(distdir)/images
	mkdir -p $(distdir)/examples
	cp -p $(srcdir)/examples/* $(distdir)/examples
	mkdir -p $(distdir)/devel
	cp -p $(srcdir)/devel/* $(distdir)/devel
