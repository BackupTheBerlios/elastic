## ==========================================================================
## elasticdir.m4
## ==========================================================================
## elastiC file handling
## --------------------------------------------------------------------------
##   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
##
##   Created: 19 Nov 1999
##
##   $Id: elasticdir.m4,v 1.1 2002/05/23 21:15:54 panta Exp $
## --------------------------------------------------------------------------
##    Copyright (C) 1999-2000 Marco Pantaleoni. All rights reserved.
##
##  The contents of this file are subject to the elastiC License version 1.0
##  (the "elastiC License"); you may not use this file except in compliance
##  with the elastiC License. You may obtain a copy of the elastiC License at
##  http://www.elasticworld.org/LICENSE
##
##  IN NO EVENT SHALL THE AUTHOR OR DISTRIBUTORS BE LIABLE TO ANY PARTY
##  FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
##  ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
##  DERIVATIVES THEREOF, EVEN IF THE AUTHOR HAS BEEN ADVISED OF THE
##  POSSIBILITY OF SUCH DAMAGE.
##
##  THE AUTHOR AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
##  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
##  FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE
##  IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHOR AND DISTRIBUTORS HAVE
##  NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
##  MODIFICATIONS.
##
##  See the elastiC License for the specific language governing rights and
##  limitations under the elastiC License.
## ==========================================================================

# serial 1

AC_DEFUN(AM_PATH_ELASTICDIR,
 [AC_PATH_PROG(ECC, ecc, no)
  if test $EMACS != "no"; then
    AC_MSG_CHECKING([where .ecc files should go])
    dnl Set default value
    elasticdir="\$(datadir)/elastic/modules"
    if test "x$prefix" = "xNONE"; then
      if test -d $ac_default_prefix/share/elastic/modules; then
	elasticdir="\$(prefix)/share/elastic/modules"
      else
	if test -d $ac_default_prefix/lib/elastic/modules; then
	  elasticdir="\$(prefix)/lib/elastic/modules"
	fi
      fi
    else
      if test -d $prefix/share/elastic/modules; then
	elasticdir="\$(prefix)/share/elastic/modules"
      else
	if test -d $prefix/lib/elastic/modules; then
	  elasticdir="\$(prefix)/lib/elastic/modules"
	fi
      fi
    fi
    AC_MSG_RESULT($elasticdir)
  fi
  AC_SUBST(elasticdir)])
