## ==========================================================================
## acinclude.m4
## ==========================================================================
## Autoconf macros for elastiC.
## --------------------------------------------------------------------------
##   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
##
##   Created: 18 Nov 1999
##
##   $Id: acinclude.m4,v 1.1 2002/06/01 17:10:07 panta Exp $
## --------------------------------------------------------------------------
##    Copyright (C) 1999-2002 Marco Pantaleoni. All rights reserved.
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

dnl ELASTIC_CHECK_TYPE - check for typedef-ined symbols
dnl
dnl Check for types (eventually typedef'd), allowing to specify
dnl the header files to check (in addition to the default ones).
dnl 
dnl ELASTIC_CHECK_TYPE(TYPE, HEADERS, DEFAULT[, VALUE-IF-FOUND])
AC_DEFUN(ELASTIC_CHECK_TYPE,
[AC_REQUIRE([AC_HEADER_STDC])dnl
AC_MSG_CHECKING(for $1)
AC_CACHE_VAL(elastic_cv_type_$1,
[AC_EGREP_CPP($1, [#include <sys/types.h>
#if STDC_HEADERS
#include <stdlib.h>
#endif
$2
], elastic_cv_type_$1=yes, elastic_cv_type_$1=no)])
AC_MSG_RESULT($elastic_cv_type_$1)
ifelse($#, 4, [if test $elastic_cv_type_$1 = yes; then
        AC_DEFINE($4)
        fi])
if test $elastic_cv_type_$1 = no; then
  AC_DEFINE($1, $3)
fi
])

dnl
dnl A signed 16-bit integer quantity
dnl
AC_DEFUN(ELASTIC_TYPE_BITS16_T,
[
if test "$ac_cv_sizeof_short" = 2; then
  AC_CHECK_TYPE(bits16_t, short)
elif test "$ac_cv_sizeof_char" = 2; then
  AC_CHECK_TYPE(bits16_t, char)
else
  AC_CHECK_TYPE(bits16_t, short)
fi
])

dnl
dnl An unsigned 16-bit integer quantity
dnl
AC_DEFUN(ELASTIC_TYPE_U_BITS16_T,
[
if test "$ac_cv_sizeof_short" = 2; then
  AC_CHECK_TYPE(u_bits16_t, unsigned short)
elif test "$ac_cv_sizeof_char" = 2; then
  AC_CHECK_TYPE(u_bits16_t, unsigned char)
else
  AC_CHECK_TYPE(u_bits16_t, unsigned short)
fi
])

dnl
dnl A signed 32-bit integer quantity
dnl
AC_DEFUN(ELASTIC_TYPE_BITS32_T,
[
if test "$ac_cv_sizeof_int" = 4; then
  AC_CHECK_TYPE(bits32_t, int)
elif test "$ac_cv_sizeof_long" = 4; then
  AC_CHECK_TYPE(bits32_t, long)
else
  AC_CHECK_TYPE(bits32_t, int)
fi
])

dnl
dnl An unsigned 32-bit integer quantity
dnl
AC_DEFUN(ELASTIC_TYPE_U_BITS32_T,
[
if test "$ac_cv_sizeof_int" = 4; then
  AC_CHECK_TYPE(u_bits32_t, unsigned int)
elif test "$ac_cv_sizeof_long" = 4; then
  AC_CHECK_TYPE(u_bits32_t, unsigned long)
else
  AC_CHECK_TYPE(u_bits32_t, unsigned int)
fi
])

AC_DEFUN(ELASTIC_TYPE_PTRDIFF_T,
[
if test "$ac_cv_sizeof_int" = "$ac_cv_sizeof_char_p"; then
  AC_CHECK_TYPE(ptrdiff_t, int)
elif test "$ac_cv_sizeof_long" = "$ac_cv_sizeof_char_p"; then
  AC_CHECK_TYPE(ptrdiff_t, long)
else
  AC_CHECK_TYPE(ptrdiff_t, int)
fi
])

dnl
dnl A signed 64-bit quantity
dnl
AC_DEFUN(ELASTIC_TYPE_BITS64_T,
[
if test "$ac_sv_sizeof_char_p" = 8; then
  AC_CHECK_TYPE(bits64_t, char *)
elif test "$ac_cv_sizeof_double" = 8; then
  AC_CHECK_TYPE(bits64_t, double)
elif test "$ac_cv_sizeof_long" = 8; then
  AC_CHECK_TYPE(bits64_t, long)
else
  AC_CHECK_TYPE(bits64_t, double)
fi
])


dnl ========================================================================
dnl ELASTIC_VA_COPY -  check for va_copy
dnl
dnl we currently check for all three va_copy possibilities, so we get
dnl all results in config.log for bug reports.
AC_DEFUN(ELASTIC_VA_COPY,
[
AC_CACHE_CHECK([for an implementation of va_copy()],elastic_cv_va_copy,[
	AC_TRY_RUN([
	#include <stdarg.h>
	void f (int i, ...) {
	va_list args1, args2;
	va_start (args1, i);
	va_copy (args2, args1);
	if (va_arg (args2, int) != 42 || va_arg (args1, int) != 42)
	  exit (1);
	va_end (args1); va_end (args2);
	}
	int main() {
	  f (0, 42);
	  return 0;
	}],
	[elastic_cv_va_copy=yes],
	[elastic_cv_va_copy=no],
	[elastic_cv_va_copy=no])
])
AC_CACHE_CHECK([for an implementation of __va_copy()],elastic_cv___va_copy,[
	AC_TRY_RUN([
	#include <stdarg.h>
	void f (int i, ...) {
	va_list args1, args2;
	va_start (args1, i);
	__va_copy (args2, args1);
	if (va_arg (args2, int) != 42 || va_arg (args1, int) != 42)
	  exit (1);
	va_end (args1); va_end (args2);
	}
	int main() {
	  f (0, 42);
	  return 0;
	}],
	[elastic_cv___va_copy=yes],
	[elastic_cv___va_copy=no],
	[elastic_cv___va_copy=no])
])

if test "x$elastic_cv_va_copy" = "xyes"; then
  ec_va_copy_func=va_copy
else if test "x$elastic_cv___va_copy" = "xyes"; then
  ec_va_copy_func=__va_copy
fi
fi

if test -n "$ec_va_copy_func"; then
  AC_DEFINE_UNQUOTED(EC_VA_COPY,$ec_va_copy_func,[A 'va_copy' style function])
fi

AC_CACHE_CHECK([whether va_lists can be copied by value],elastic_cv_va_val_copy,[
	AC_TRY_RUN([
	#include <stdarg.h>
	void f (int i, ...) {
	va_list args1, args2;
	va_start (args1, i);
	args2 = args1;
	if (va_arg (args2, int) != 42 || va_arg (args1, int) != 42)
	  exit (1);
	va_end (args1); va_end (args2);
	}
	int main() {
	  f (0, 42);
	  return 0;
	}],
	[elastic_cv_va_val_copy=yes],
	[elastic_cv_va_val_copy=no],
	[elastic_cv_va_val_copy=no])
])

if test "x$elastic_cv_va_val_copy" = "xno"; then
  AC_DEFINE(EC_VA_COPY_AS_ARRAY,1, ['va_lists' cannot be copies as values])
fi
])

dnl End of ELASTIC_VA_COPY
dnl ------------------------------------------------------------------------

AC_PROG_SDF - find the path to SDF
AC_DEFUN(AC_PROG_SDF,
[AC_MSG_CHECKING([for SDF])
AC_CACHE_VAL(ac_cv_path_SDF,
[if test -z "$SDF"; then
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:"
  ac_prog="sdf"
  for ac_dir in $PATH; do
    test -z "$ac_dir" && ac_dir=.
    if test -f "$ac_dir/$ac_prog"; then
      ac_cv_path_SDF="$ac_dir/$ac_prog"
      break
    fi
  done
  IFS="$ac_save_ifs"
else
  ac_cv_path_SDF="$SDF" # Let the user override the test with a path.
fi])
SDF="$ac_cv_path_SDF"
AC_MSG_RESULT([$SDF])
AC_SUBST(SDF)
])

AC_MODULE_BMON - find the path to bmon.o
AC_DEFUN(AC_MODULE_BMON,
[AC_MSG_CHECKING([for bmon.o])
AC_CACHE_VAL(ac_cv_path_BMON,
[if test -z "$BMON"; then
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:"
  ac_module="bmon.o"
  for ac_dir in /lib /usr/lib /usr/local/lib; do
    test -z "$ac_dir" && ac_dir=.
    if test -f "$ac_dir/$ac_module"; then
      ac_cv_path_BMON="$ac_dir/$ac_module"
      break
    fi
  done
  IFS="$ac_save_ifs"
else
  ac_cv_path_BMON="$BMON" # Let the user override the test with a path.
fi])
BMON="$ac_cv_path_BMON"
AC_MSG_RESULT([$BMON])
AC_SUBST(BMON)
])
