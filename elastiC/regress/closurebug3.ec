package bug3;

import basic;

public function create( a )
{
	return function( ) {
		return a;
	};
}

/*
 *  Here we start to execute package code
 */

local func1, func2;
local r1,    r2;

func1 = create( 2 );
func2 = create( 3 );

basic.print( "func1( 2 ) = ", func1(), "\n" );
basic.print( "func2( 3 ) = ", func2(), "\n" );
