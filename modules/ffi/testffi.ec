package testffi;

from basic import *;
import ffi;

private function test()
{
	local dlh;

	local strlen_func, strcmp_func, sin_func;

	dlh = ffi.open( @nil );										// the parameter is the name of a shared object
																// or @nil for the interpreter executable
	print( dlh, "\n" );

	strlen_func = ffi.declare(dlh, "strlen", #int, #[#string]);
	strcmp_func = ffi.declare(dlh, "strcmp", #int, #[#string, #string]);
	sin_func    = ffi.declare(dlh, "sin",    #double, #[#double]);

	print( sprintf( "%r\n", strlen_func) );
	print( sprintf( "%r\n", strcmp_func) );
	print( sprintf( "%r\n", sin_func) );

	local res;

	res = strlen_func("mickey");
	print( sprintf( "strlen(\"mickey\") = %d\n", res ) );

	res = strcmp_func("mickey", "goofy");
	print( sprintf( "strcmp(\"mickey\", \"goofy\") = %d\n", res ) );

	res = strcmp_func("goofy", "mickey");
	print( sprintf( "strcmp(\"goofy\", \"mickey\") = %d\n", res ) );

	res = strcmp_func("mickey", "mickey");
	print( sprintf( "strcmp(\"mickey\", \"mickey\") = %d\n", res ) );

	res = sin_func(3.141592 / 4.0);
	print( sprintf( "sin(3.141592 / 4.0) = %f\n", res ) );

	ffi.close( dlh );
}

test();
