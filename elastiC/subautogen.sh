#!/bin/sh
# Run this to generate all the initial makefiles, etc.

# $Id: subautogen.sh,v 1.1 2002/06/01 17:10:07 panta Exp $

DIE=0

REAL=1

(autoconf --version) < /dev/null > /dev/null 2>&1 || {
  echo
  echo "**Error**: You must have \`autoconf' installed to compile ${PKG_NAME}."
  echo "Download the appropriate package for your distribution,"
  echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/"
  DIE=1
}

(grep "^AM_PROG_LIBTOOL" $srcdir/configure.in >/dev/null) && {
  (libtool --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "**Error**: You must have \`libtool' installed to compile ${PKG_NAME}."
    echo "Get ftp://ftp.gnu.org/pub/gnu/libtool-1.4.2.tar.gz"
    echo "(or a newer version if it is available)"
    DIE=1
  }
}

#grep "^AM_GNU_GETTEXT" $srcdir/configure.in >/dev/null && {
#  grep "sed.*POTFILES" $srcdir/configure.in >/dev/null || \
#  (gettext --version) < /dev/null > /dev/null 2>&1 || {
#    echo
#    echo "**Error**: You must have \`gettext' installed to compile ${PKG_NAME}."
#    echo "Get ftp://alpha.gnu.org/gnu/gettext-0.10.35.tar.gz"
#    echo "(or a newer version if it is available)"
#    DIE=1
#  }
#}

(automake --version) < /dev/null > /dev/null 2>&1 || {
  echo
  echo "**Error**: You must have \`automake' installed to compile ${PKG_NAME}."
  echo "Get ftp://ftp.gnu.org/pub/gnu/automake-1.6.tar.gz"
  echo "(or a newer version if it is available)"
  DIE=1
  NO_AUTOMAKE=yes
}


# if no automake, don't bother testing for aclocal
test -n "$NO_AUTOMAKE" || (aclocal --version) < /dev/null > /dev/null 2>&1 || {
  echo
  echo "**Error**: Missing \`aclocal'.  The version of \`automake'"
  echo "installed doesn't appear recent enough."
  echo "Get ftp://ftp.gnu.org/pub/gnu/automake-1.6.tar.gz"
  echo "(or a newer version if it is available)"
  DIE=1
}

if test "$DIE" -eq 1; then
  exit 1
fi

if test -z "$*"; then
  echo "**Warning**: I am going to run \`configure' with no arguments."
  echo "If you wish to pass any to it, please specify them on the"
  echo \`$0\'" command line."
  echo
fi

case $CC in
*xlc | *xlc\ * | *lcc | *lcc\ *)
  am_opt=--include-deps;;
esac

for coin in `find $srcdir -name configure.in -print`
do 
  dr=`dirname $coin`
  if test -f $dr/NO-AUTO-GEN; then
    echo skipping $dr -- flagged as no auto-gen
  else
    echo
    echo Processing $dr
    ( cd $dr
      if test -f "./aclocal-flags"; then
        aclocal_flags_s=`./aclocal-flags`
      else
        aclocal_flags_s=""
      fi
      #echo "aclocal_flags_s: $aclocal_flags_s"
      aclocal_flags="$ACLOCAL_FLAGS $aclocal_flags_s"
      #echo "ACLOCAL_FLAGS: $ACLOCAL_FLAGS"
      #echo "aclocal_flags: \"$aclocal_flags\""
      aclocal_flags_ss=`echo $aclocal_flags | sed 's/^ //' | sed 's/ $//'`
      aclocal_flags=$aclocal_flags_ss
      if test -n "$aclocal_flags"; then
        aclocaldirs=`echo $aclocal_flags | sed -n -e 's/-I / /gp'`
      else
        aclocaldirs=`aclocal --print-ac-dir`
      fi
      #echo "aclocaldirs: $aclocaldirs"
      m4list="elastic.m4 elasticdir.m4"
      acfound=0
      for file in $m4list ; do
        for acdir in $aclocaldirs; do
          if [ -f "$acdir/$file" ]; then
            acfound=1
          fi
        done
        if test "$acfound" -eq 0; then
          echo "WARNING: aclocal's directories are \"$aclocaldirs\", but..."
          echo "         no file $file"
          echo "         You may see fatal macro warnings below."
          echo "         If these files are installed in /some/dir, set the ACLOCAL_FLAGS "
          echo "         environment variable to \"-I /some/dir\", or install"
          echo "         $file in one of \"$aclocaldirs\"."
          echo ""
        fi
      done
#      aclocalinclude="$ACLOCAL_FLAGS"
#      for k in $macrodirs; do
#  	if test -d $k; then
#          aclocalinclude="$aclocalinclude -I $k"
#  	##else 
#	##  echo "**Warning**: No such directory \`$k'.  Ignored."
#        fi
#      done
      aclocalinclude="${aclocal_flags}"
      #echo "aclocalinclude: $aclocalinclude"
      if grep "^AM_GNU_GETTEXT" configure.in >/dev/null; then
	if grep "sed.*POTFILES" configure.in >/dev/null; then
	  : do nothing -- we still have an old unmodified configure.in
	else
	  echo "Creating $dr/aclocal.m4 ..."
	  test -r $dr/aclocal.m4 || touch $dr/aclocal.m4
	  echo "Running gettextize  Ignore non-fatal messages."
	  test "$REAL" -eq 1 && ( echo "no" | gettextize --force --copy )
	  echo "Making $dr/aclocal.m4 writable ..."
	  test -r $dr/aclocal.m4 && chmod u+w $dr/aclocal.m4
        fi
      fi
      if grep "^AM_PROG_LIBTOOL" configure.in >/dev/null; then
	echo "Running libtoolize --force --copy"
	test "$REAL" -eq 1 && ( libtoolize --force --copy )
      fi
      echo "Running aclocal $aclocalinclude"
      test "$REAL" -eq 1 && ( aclocal $aclocalinclude )
      if grep "^AM_CONFIG_HEADER" configure.in >/dev/null; then
        if test -f $dr/NO-AUTOHEADER; then
          echo skipping autoheader -- flagged as no-autoheader
        else
	  echo "Running autoheader"
	  test "$REAL" -eq 1 && ( autoheader )
        fi
      fi
      echo "Running automake --add-missing --gnu $am_opt"
      test "$REAL" -eq 1 && ( automake --add-missing --gnu $am_opt )
      echo "Running autoconf"
      test "$REAL" -eq 1 && ( autoconf )
    )
  fi
done

conf_flags="--enable-maintainer-mode --enable-compile-warnings" #--enable-iso-c

if test x$NOCONFIGURE = x; then
  echo
  echo Running $srcdir/configure $conf_flags "$@"
  test "$REAL" -eq 1 && ( $srcdir/configure $conf_flags "$@" \
  && echo Now type \`make\' to compile $PKG_NAME || exit 1 )
else
  echo Skipping configure process.
fi
