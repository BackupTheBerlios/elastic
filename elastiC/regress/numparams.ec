package numparams;
import basic;

public class cl1 extends basic.Object {
	static j = 15;
	local i;

	class method printJ( )
	{
		basic.print( "Self: ", self, "   J: ", j, '\n' );
	}

	method out( val ) {
		basic.print( "j  : ", j, '\n' );
		i = val;
		basic.print( "val: ", val, '\n' );

		j = val;

		for (; i < 20; i = i + 1) {
			basic.print( "i: ", i, '\n' );
		}
		basic.print( "val: ", val, '\n' );

		if (i >= 50)
			basic.print( "BOO !\n" );
		else if (i < 20)
			basic.print( "UHU !\n" );
		else
			basic.print( "OINC !\n" );

		[self print];
	}

	method mickey: a with: b and: c {
		basic.print( "a: ", a, "  b: ", b, "  c: ", c, '\n' );
	}

	method print( ) {
		basic.print( "i: ", i, '\n' );
	}
}

public class cl2 extends cl1 {
/*
	class method name( aString ) {
		return "*" + aString + "*";
	}
*/

	method goofy( val ) {
		basic.print( "goofy val: ", val, '\n' );
	}

	method print( ) {
		basic.print( "CL2 PRINT ", '\n' );
	}
}

private o2;

o2 = [cl2 new];
[cl2 printJ];
[o2 out 3];
[cl2 printJ];
try {
	[o2 gosfy 4];
} catch (basic.UnknownMethodError e) {
	basic.print( "UNKNOWN METHOD: ", [e getMethod], " in target: ", [e getTarget], '\n' );
	basic.print( "       message: ", [e getFullMessage], '\n' );
	//throw e;
	[[e getTarget] goofy 4];
}
[o2 print];
[o2 mickey:3
    with:  2
    and:   1];

basic.print( "o2 class: ", [[o2 isA] name "pluto"], '\n' );
