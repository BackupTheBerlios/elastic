# ==========================================================================
# Makefile.am
# ==========================================================================
# Automake makefile template for the 'inet' elastiC module.
# --------------------------------------------------------------------------
#   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
#
#   Created: 25 Nov 1999
#
#   $Id: Makefile.am,v 1.1 2002/05/30 13:23:05 panta Exp $
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
	$(module_sources)			\
	$(netfile_sources) $(geturl_sources)	\
	$(example_sources)
MODULE = inet$(SO)

CLEANFILES = $(MODULE) $(wildcard *.o) $(wildcard *.ecc) \
	inet.def inet.dll libinet.a

EC  = ec
ECC = ecc

CFLAGS = $(MODULE_CFLAGS) -DINET_MODULE_EXPORTS
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $(CCSHARED)
ECCOMPILE = $(ECC)

if WIN32
MLIBS = $(MKMODULELIBS) -lwsock32
else
MLIBS = $(MKMODULELIBS)
endif

netfile_module = netfile.ec netfile.ecc
geturl_module  = geturl.ec geturl.ecc

elasticlibdir = $(prefix)/lib/elastic
elasticlib_DATA = $(MODULE) $(netfile_module) $(geturl_module)

inet_sources = \
	inet.c		\
	inet.h		\
	nwlib.h		\
	nwlib.c		\
	error.c		\
	inet.c		\
	socket.c	\
	unix.c

module_sources = $(inet_sources)

netfile_sources = \
	netfile.ec

geturl_sources = \
	geturl.ec

example_sources = \
	testinet.ec	\
	client.ec	\
	server.ec	\
	testnetfile.ec	\
	stockticker.ec

%.o : %.c
	$(COMPILE) -c $< -o $@

%.ecc : %.ec
	$(ECCOMPILE) -o $@ $<

$(MODULE): inet.o nwlib.o error.o inet.o socket.o unix.o
	$(MKMODULE) $(MKMODULEFLAGS) $@ $^ $(MLIBS) $(LIBS)

# Explicit elastiC module dependencies (no automake subject *sigh*)

geturl.ecc: geturl.ec netfile.ecc

netfile.ecc: netfile.ec $(MODULE)
