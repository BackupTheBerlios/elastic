package testgoto;

from   basic  import *;
import sys;

function test()
{
	print( "POINT A\n" );

	goto label_A;
	print( "POINT B\n" );
 label_A:
	print( "POINT C\n" );
}

test();
