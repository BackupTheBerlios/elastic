package exc4;
import basic;

/*
 * Re-raise exception test
 */

public class IOError extends basic.Exception {
	method init() {
		basic.print( "IOError INIT\n" );
	}
	method out( errmsg ) {
		basic.print( errmsg, '\n' );
	}
}

public class FileError extends IOError {
	method out( errmsg ) {
		basic.print( errmsg, '\n' );
	}
}

public class MemoryError extends basic.Object {
	method init() {
		basic.print( "MEMORY INIT\n" );
	}

	method out( errmsg ) {
		basic.print( errmsg, '\n' );
	}
}

function square(a)
{
	local b;

	b = 5;
	try {
		local d;
		d = 8;
		basic.print( "Going to throw an EXCEPTION !\n" );
		basic.print( "a: ", a, '\n' );
		try {
			basic.print( b+b/2, '\n' );
			basic.print( d, '\n' );
			throw [IOError new];
		} catch (FileError) {
			basic.print( "ARGH 2!\n" );
		} catch (IOError e) {
			basic.print( b+b/3, '\n' );
			basic.print( d, '\n' );
			basic.print( "ARGH IOError!\n" );
			// Re-raise exception
			throw e;
		} catch (MemoryError) {
			basic.print( "ARGH MemoryError!\n" );
		}
		b = a*a;
//		return b;
		throw [IOError new];
		basic.print( b, '\n' );
		basic.print( "WE MUST NOT COME HERE\n" );
	} catch (IOError) {
		basic.print( "ARGH !\n" );
	}

	basic.print( "OUT\n" );
	return b;
}

basic.print( "*** Re-raise exception test ***\n" );
square( 6 );
