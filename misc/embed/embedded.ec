package embedded;

//import basic;
import string;

from basic import print, sprintf;

print("embedded module loaded.\n");

public myvar1, myvar2;

public function runme( anInt, aString, aBool )
{
	print( sprintf( "myvar1: %r\n", myvar1 ) );
	print( sprintf( "myvar2: %r\n", myvar2 ) );

	print( sprintf( "parameter 1: %r\n", anInt ) );
	print( sprintf( "parameter 2: %r\n", aString ) );
	print( sprintf( "parameter 3: %r\n", aBool ) );

	return #[anInt, aString, aBool];
}
