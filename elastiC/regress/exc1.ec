package exc1;
import basic;

public class IOError extends basic.Exception {
	method shout( ) {
		basic.print( "CAUGHT EXCEPTION: ", [self getFullMessage], '\n' );
	}
}

function square(a)
{
	local b;

	try {
		basic.print( "Going to throw an EXCEPTION !\n" );
		basic.print( "a: ", a, '\n' );
		b = a*a;
//		return b;
		throw [IOError new:"Test exception. Don't worry !"];
		basic.print( "WE MUST NOT COME HERE\n" );
	} catch (IOError e) {
		[e shout];
	}

	basic.print( "OUT\n" );
	return b;
}

square( 6 );
