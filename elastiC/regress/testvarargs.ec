package testvarargs;

import basic;
import string;

private function tt( a, b, c, args ... )
{
	basic.print( "a: ", a, "  b: ", b, "  c: ", c, '\n' );

	local i;
	basic.print( "varargs: " );
	for (i in args)
		basic.print( i, ' ' );
	basic.print( '\n' );
}

tt( 8, 7, 6, "Goofy", 5, 'N', 'Y', 8.423 );
