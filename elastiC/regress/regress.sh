#!/bin/bash
# ==========================================================================
# regress.sh
# ==========================================================================
# Regression test script for elastiC.
# --------------------------------------------------------------------------
#   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
#
#   Created: 1998
#
#   $Id: regress.sh,v 1.1 2002/05/23 21:16:31 panta Exp $
# --------------------------------------------------------------------------
#    Copyright (C) 1998-2001 Marco Pantaleoni. All rights reserved.
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

ERRORPRONE="./testenv"

LIBPATH="${srcdir}/obj:${srcdir}/../src/modules-dynamic/re"
#LIBPATH=${top_srcdir}/regress/obj:${top_builddir}/src/modules-dynamic/re

if echo '\c' | grep -s c >/dev/null 2>&1
then
        ECHO_N="echo -n"
        ECHO_C=""
else
        ECHO_N="echo"
        ECHO_C='\c'
fi

FRONTEND="${srcdir}/../src/ec"

mode="test"
if test $# -eq 1; then
	mode=$1
fi

echo "=============== Notes...                              ================="
echo "For regression tests to succed, binary tools (ecc, ec, ...) must"
echo "already be present in the src directory."
echo "For more information see the INSTALL file in top-level directory."
echo ""

# Delete previous regression results
rm -f ${srcdir}/stdout/*
rm -f ${srcdir}/stderr/*
#rm -f ${srcdir}/obj/*

if test "x$mode" != "xmake" ; then
	msg="=============== running regression tests...           ================="
	dstout=${srcdir}/stdout
	dsterr=${srcdir}/stderr
else
	msg="=============== building reference values...          ================="
	dstout=${srcdir}/expectout
	dsterr=${srcdir}/expecterr
fi

numtests=0
passed=0

# all is ok by default
result=1

echo "$msg"
for i in `cat ${srcdir}/tests`
do
        numtests=`expr $numtests + 1`;

        $ECHO_N "${i} .. " $ECHO_C;
	ELASTICPATH="$LIBPATH";
	export ELASTICPATH;
	#export ELASTICPATH="$LIBPATH";
	if test -f ${srcdir}/input/${i}.in ; then
		$FRONTEND ${srcdir}/obj/${i}.ecc < ${srcdir}/input/${i}.in > ${dstout}/${i}.out 2> ${dsterr}/${i}.err ;
	else
		$FRONTEND ${srcdir}/obj/${i}.ecc > ${dstout}/${i}.out 2> ${dsterr}/${i}.err ;
	fi
	if test "x$mode" != "xmake" ; then
		diffout_n=`diff ${srcdir}/expectout/${i}.out ${dstout}/${i}.out | wc -l`;
		differr_n=`diff ${srcdir}/expecterr/${i}.err ${dsterr}/${i}.err | wc -l`;
		if test $diffout_n -ne 0 -o $differr_n -ne 0; then
			ign=0;
			for j in $ERRORPRONE ; do
			    if test "x$i" = "x$j"; then
				echo "failed (ignored)";
				ign=1;
				break
			    fi
			done
			if test $ign -eq 0 ; then
			    echo "failed";
			    result=0;
			fi
	        else
	                echo "ok";
			passed=`expr $passed + 1`;
	        fi
	else
	        echo "done";
	fi
done
echo "=============== done                                  ================="

if test $result -ne 1 ; then
        failed=`expr $numtests - $passed`;
	echo "REGRESSION TESTS FAILED ! (passed: ${passed}/${numtests}, failed: ${failed})";
	exit 1
fi
if test $numtests -eq $passed ; then
        echo "ALL TESTS PASSED";
	exit 0
fi

exit
