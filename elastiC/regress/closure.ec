package closure;

// Import the `basic' package
import basic;

// A function creating other functions !
function create( a )
{
	local b = 10;
	local func;

	// create a function bound to current lexical environment
	func = function( c ) {
		// note that `a' is a parameter passed to create()
		// and 'b' is local to create()
		return a * b * c;
	};

	return func;
}

/*
 *  Here we start to execute package code
 */

local func1, func2;
local r1,    r2;

func1 = create( 2 );
func2 = create( 5 );

r1 = func1( 3 );
r2 = func2( 3 );

basic.print( "func1( 3 ) = ", r1, "\n" );
basic.print( "func2( 3 ) = ", r2, "\n" );
