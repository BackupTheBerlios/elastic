import basic;

public class cladd extends basic.Object {
	local a;

	method init( ) {
		a = #[];
	}

	method _add( other ) {
		local l;

		l = basic.length( a );
		a[l] = other;
	}

	method _radd( other ) {
		local i, l;

		l = basic.length( a );
		for (i = l-1; i >= 0; i--)
			a[i+1] = a[i];
		a[0] = other;
	}

	method show() {
		basic.print( "HERE IT IS: ", a, "\n" );
	}
}

basic.print( 10.2 + 4, "\n" );

basic.print( 4 + 10.2, "\n" );

basic.print( 4 + 2, "\n" );

basic.print( 4.1 + 5.1, "\n" );

basic.print( "Hello" + " World", "\n" );
basic.print( "Result: " + basic.str(10.1), "\n" );

local obj = [cladd new];
obj = obj + 1;
obj = obj + 2;
obj = "f" + obj;
obj = obj + "Ok";

[obj show];

local ar1, ar2;

ar1 = #[1, 2, 3];
ar2 = #["four", "five", #six];
basic.print( ar1 + ar2, "\n" );
//basic.print( ar1 + 2, "\n" );

basic.print( "abc" + 'd', "\n" );
basic.print( 'a' + "bcd", "\n" );
basic.print( 'a' + 4, "\n" );

local i;
local ar;

for (i = 0; i < 5; i++)
{
	ar = #[ 1, 2, 3, 4, 5 ];

	//ar[basic.length(ar)] = i;
	ar[i] = i;
	basic.print( "I: ", i, "  Array: ", ar, "\n" );
}

local st;

for (i = 0; i < 5; i++)
{
	st = "Hello";

	st[i] = 'a';
	basic.print( "I: ", i, "  String: ", st, "\n" );
}
