package exc5;
import basic;

/*
 * Inter-functional catch test
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

function func(a, b)
{
	basic.print( "IN 1\n" );
	try {
		basic.print( "IN 2\n" );
		throw [IOError new];
	} catch (MemoryError e) {
		basic.print( "MemoryError: ", [e getFullMessage], '\n' );
	}

	basic.print( "OUT\n" );
	return a * b;
}

basic.print( "*** Inter-functional catch exception test ***\n" );
try {
	func( 6, 3 );
} catch (IOError e) {
	basic.print( "IOError: ", [e getFullMessage], '\n' );
}
