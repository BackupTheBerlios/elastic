# ==========================================================================
# Makefile.am
# ==========================================================================
# Automake makefile template for elastiC.
# --------------------------------------------------------------------------
#   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
#
#   Created: 15 Nov 1999
#
#   $Id: Makefile.am,v 1.1 2002/05/23 21:15:48 panta Exp $
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

EXTRA_DIST = LICENSE ACKNOWLEDGEMENTS elastic.m4 makemodule mkwin32.bat mingw32-makedll cygwin-makedll
SUBDIRS = src aclocal . doc regress

aclocaldir = $(datadir)/aclocal
aclocal_DATA = elastic.m4

elasticincludedir = $(includedir)/elastic

elasticinclude_HEADERS = \
	config.h

.PHONY: prepare

config.h: elastic
elastic: prepare
prepare:
	if [ ! -d elastic ] ; then \
		ln -s src elastic; \
	fi
#	touch $(srcdir)/doc/INSTALL.sdf

ChangeLog.html: ChangeLog
	enscript -o $@ -W html -Echangelog --color -J ChangeLog $<

if WIN32
install-exec-local:
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	$(INSTALL_SCRIPT) $(srcdir)/makemodule $(DESTDIR)$(bindir)
	$(INSTALL_SCRIPT) $(srcdir)/mingw32-makedll $(DESTDIR)$(bindir)
else
install-exec-local:
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	$(INSTALL_SCRIPT) $(srcdir)/makemodule $(DESTDIR)$(bindir)
endif

dist-hook:
	mkdir -p $(distdir)/build
	cp -p $(srcdir)/build/* $(distdir)/build
	mkdir -p $(distdir)/tools
	cp -p $(srcdir)/tools/* $(distdir)/tools
	mkdir -p $(distdir)/misc
	cp -p $(srcdir)/misc/* $(distdir)/misc
	mkdir -p $(distdir)/win32
	cp -a $(srcdir)/win32/* $(distdir)/win32
