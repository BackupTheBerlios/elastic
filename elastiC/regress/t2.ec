package t2;

import basic;

local i;

i = 5;

public class myclass extends basic.Object {
	method hello()
	{
		basic.print( "Hello. How are you ?\n" );
		basic.print( "i: ", i, "\n" );
	}
}

local o = [myclass new];
[o hello];
