# ==========================================================================
# Makefile.am
# ==========================================================================
# Automake makefile template for byacc/elastiC examples.
# --------------------------------------------------------------------------
#   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
#
#   Created: Tue Oct  9 17:06:50 CEST 2001
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

EXTRA_DIST = LICENSE \
	cal.y					\
	$(cal_sources)				\
	$(allbuilt_sources)

CLEANFILES = $(wildcard *.o) $(wildcard *.ecc) $(wildcard *.output)

EC  = ec
ECC = ecc
BYACC_EC = ${top_builddir}/byacc-ec

ECCOMPILE = $(ECC)

cal_sources = \
	cal_driver.ec		\
	cal.y			\
	Token.ec		\
	cal.ec

allbuilt_sources = \
	cal.ec			\
	Token.ec

%.ecc : %.ec
	$(ECCOMPILE) -o $@ $<

all-local: cal_driver.ecc

cal.ec Token.ec: cal.y
	$(BYACC_EC) -Epackage=cal -b cal -v $<

# Explicit elastiC module dependencies (no automake subject *sigh*)

cal_driver.ecc: Token.ecc cal.ecc

cal.ecc: Token.ecc
