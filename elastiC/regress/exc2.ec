package exc2;
import basic;

/*
 * Inlined exception return test
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

	basic.print( "A1\n" );

	/* Level 1 try block */
	try {
		basic.print( "B1\n" );

		/* Level 2 try block #1 */
		try {
			basic.print( "BT\n" );
			// inlined return from try
		} catch (FileError) {
			basic.print( "BC1\n" );
		} catch (IOError e) {
			basic.print( "BC2\n" );
		} catch (MemoryError) {
			basic.print( "BC3\n" );
		}
		basic.print( "MARK1\n" );


		basic.print( "B2\n" );

		/* Level 2 try block #1 */
		try {
			basic.print( "BT\n" );
			throw [IOError new];
		} catch (FileError) {
			basic.print( "BC1\n" );
		} catch (IOError e) {
			basic.print( "BC2\n" );
			// inlined return from catch
		} catch (MemoryError) {
			basic.print( "BC3\n" );
		}

		basic.print( "MARK2\n" );

		basic.print( "B3\n" );
		throw [IOError new];
	} catch (IOError) {
		basic.print( "ARGH !\n" );
	}

	basic.print( "OUT\n" );
	return b;
}

basic.print( "*** Exception inlined-return test ***\n" );

square( 6 );
