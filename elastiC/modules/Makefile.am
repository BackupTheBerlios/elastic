# ==========================================================================
# Makefile.am
# ==========================================================================
# Automake makefile template for elastiC.
# --------------------------------------------------------------------------
#   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
#
#   Created: Wed Jun 12 17:23:08 CEST 2002
#
#   $Id: Makefile.am,v 1.1 2002/06/12 16:00:05 panta Exp $
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

CLEANFILES = $(wildcard $.o)  $(wildcard $.dll) $(wildcard $.ecc)

elasticlibdir = $(prefix)/lib/elastic-$(EC_API_VERSION)
elasticlib_DATA = file.ec file.ecc os.ec os.ecc

EXTRA_DIST = Re.ec

ECC  = $(top_builddir)/src/ecc$(EXEEXT)
EC   = $(top_builddir)/src/ec$(EXEEXT)

%.ecc: %.ec
	ELASTICPATH="$(ELASTICPATH):$(top_builddir)/src/modules-dynamic/re:$(top_builddir)/src/modules:$(top_builddir)/modules" $(ECC) -o $@ $<
