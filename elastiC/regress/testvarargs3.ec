package testvarargs3;

import basic;
import string;

public class Test extends basic.Object
{
	method showargs(format, args...)
	{
		basic.print( "FORMAT: ", format, "  ARGS: ", args, "\n" );
	}
}

private obj = [Test new];
[obj showargs "testString", 1, 2, 3, 4, 5];
