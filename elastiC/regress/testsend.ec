package testsend;

from   basic  import *;
import sys;

public class TestClass extends basic.Object
{
	method callMe(arg1, arg2, arg3)
	{
		print( "ARG1: ", arg1, "  ARG2: ", arg2, "  ARG3: ", arg3, "\n" );
	}

	method callMeToo(arg)
	{
		print( "ARG: ", arg, "\n" );
	}
}

private obj;
obj = [TestClass new];
basic.send( obj, #callMe, 1, 2, 3 );
basic.sendwith( obj, #callMe, #[1, 2, 3] );
basic.send( obj, #callMeToo, 1 );
