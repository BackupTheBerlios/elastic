/*
 * A package.
 */
package test1;

import util;
from util import *;
import basic;

public b;

b = 7;

private function addsquare5(a)
{
	// A one line comment
	return square(a)+5;
}

private function addcube5(c)
{
	/* A one line comment */
	return cube(c)+5;
}

private function cycle(a,b)
{
	local i;

	/*
	 * A more elaborate comment.
	 */
	for (i = a; i <= b; i = i + 1)
		basic.print( "i: ", i, '\n' );

	for (i = 0; i < 10; i = i + 1);
}

basic.print( "IN PACKAGE test1\n" );

basic.print( addsquare5(8), '\n' );
basic.print( addcube5(1), '\n' );
cycle(1, 10);

basic.print( "OUT PACKAGE test1\n" );
