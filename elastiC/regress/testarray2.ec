package testarray2;

import basic;

public class ClArray extends basic.Object {
	local arr;

	method init() {
		arr = basic.array( 100 );
	}

	method _getitem( idx ) {
		if (idx == "Test")
			return "Good.";
		return arr[ idx ];
	}

	method _setitem( idx, val ) {
		arr[ idx ] = val;
	}

	method doesNotUnderstand( methodId, args ) {
		basic.print( "HEY ! I don't understand method: ", methodId, '\n' );

		basic.print( "ARGS: " );

		local i;
		for (i in args)
			basic.print( i, ' ' );
		basic.print( '\n' );
	}
}

private a;

a = [ClArray new];
a[10] = 6;
a[8]  = 'm';
a[5]  = 5.5;
a[6]  = 'ThisIsASymbol;
a[7]  = 'This:Is:Another-symbol;

basic.print( basic.typeof(basic.print), '\n' );
[a ciccio 5, 6, 7];
basic.print( a[8], '\n' );
basic.print( a[5], '\n' );
basic.print( a[10], '\n' );
basic.print( a["Test"], '\n' );
basic.print( a[6], '\n' );
basic.print( a[7], '\n' );
basic.print( a[120], '\n' );
