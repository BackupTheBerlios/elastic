#!/bin/sh
# Run this to generate all the initial makefiles, etc.

# $Id: autogen.sh,v 1.2 2005/03/22 10:09:32 panta Exp $

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

PKG_NAME="elastic"

export SED=sed

(test -f $srcdir/configure.in \
  && test -f $srcdir/README \
  && test -f $srcdir/src/elastic.c) || {
    echo -n "**Error**: Directory "\`$srcdir\'" does not look like the"
    echo " top-level $PKG_NAME directory"
    exit 1
}

[ -e autom4te.cache ] && rm -rf autom4te.cache
find . -name "config.cache" | xargs rm -f
find . -name "config.log" | xargs rm -f
find . -name "config.status" | xargs rm -f

$srcdir/tools/symlinks
. $srcdir/subautogen.sh
