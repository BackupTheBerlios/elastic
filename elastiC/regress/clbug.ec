package clbug;

from   basic  import *;
import sys;

public class Function extends basic.Object
{
	method getName()
	{
		return "fdesc";
	}
}

public class Module extends Object
{
	local defdata;

	method run()
	{
		[self _parse];
		return self;
	}

	method _parse()
	{

		defdata = #[ [Function new], 5 ];

		print( "DATA : ", defdata, "\n" );
		print( "FDEF : ", [defdata[0] getName], "\n" );
		print( "****\n" );
		print( "DATA : ", defdata, "\n" );
	}
}

private module;
module = [Module new];
[module run];
