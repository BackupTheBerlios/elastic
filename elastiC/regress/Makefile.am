# ==========================================================================
# Makefile.am
# ==========================================================================
# Automake makefile template for elastiC.
# --------------------------------------------------------------------------
#   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
#
#   Created: 15 Nov 1999
#
#   $Id: Makefile.am,v 1.3 2005/03/22 10:09:33 panta Exp $
# --------------------------------------------------------------------------
#    Copyright (C) 1999-2000 Marco Pantaleoni. All rights reserved.
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

# Directory where sources are, from where we are.
srcdir = @srcdir@
VPATH = @srcdir@

ECC  = $(top_builddir)/src/ecc$(EXEEXT)
EC   = $(top_builddir)/src/ec$(EXEEXT)
ECPP = $(srcdir)/ecpp.pl
DIFF = diff

OBJDIR      = ./obj/
ELASTICPATH = ./obj

EcSrcSuffixes = .ec
SRCS          = $(foreach s,$(EcSrcSuffixes),$(wildcard $(srcdir)/*$(s)))
OBJS          = $(addsuffix .ecc,$(addprefix $(OBJDIR),$(basename $(notdir $(SRCS)))))

EXTRA_DIST = ecpp.pl regress.sh tests .depend $(SRCS)

#DISTCLEANFILES       = .depend
MAINTAINERCLEANFILES = tests

include $(top_srcdir)/build/rules.mk

TESTS = regress.sh

noinst_DATA = $(OBJS)

regress.sh: $(noinst_DATA) .depend tests

tests:
	@echo $(SRCS) >tests
	perl -i -p -e 's/([a-zA-Z0-9]+)\.ec\s*/$$1\n/g;' tests

depfile .depend:
	for i in $(SRCS); do $(ECPP) $$i; done >.depend
	perl -i.bak -p -e 's/([^\s]*\.ecc)/obj\/$$1/g;' .depend
	perl -i.bak -p -e 's/\/\.\//\//g;' .depend
include .depend

clean-local:
	-rm -f $(OBJDIR)/*.ecc

dist-hook:
	mkdir -p $(distdir)/input
	mkdir -p $(distdir)/stdout
	mkdir -p $(distdir)/stderr
	mkdir -p $(distdir)/expectout
	mkdir -p $(distdir)/expecterr
	mkdir -p $(distdir)/files
	mkdir -p $(distdir)/obj
	cp -a input/* $(distdir)/input
	cp -a expectout/* $(distdir)/expectout
	cp -a expecterr/* $(distdir)/expecterr
	cp -a files/* $(distdir)/files
