# ==========================================================================
# Makefile.am
# ==========================================================================
# Automake makefile template for elastiC.
# --------------------------------------------------------------------------
#   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
#
#   Created: 23 Jan 2000
#
#   $Id: Makefile.am,v 1.1 2002/05/23 21:20:24 panta Exp $
# --------------------------------------------------------------------------
#    Copyright (C) 2001 Marco Pantaleoni. All rights reserved.
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

noinst_LTLIBRARIES = libos.la

CFLAGS = $(CORE_CFLAGS)

static_sources = \
	os.c

libos_la_SOURCES = \
	$(static_sources)
libos_la_LIBADD =

INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/src
