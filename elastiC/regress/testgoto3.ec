package testgoto3;

from   basic  import *;
import sys;

function test()
{
	print( "POINT A\n" );

	goto label_A;
	print( "POINT B\n" );
	local v;
	v = 10;
 label_A:
	print( "POINT C\n" );
	print( "v: ", v, "\n" );
}

test();
