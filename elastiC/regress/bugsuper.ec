package bugsuper;

import basic;


public class Symbolic extends basic.Object
{
	local name;

	local name2value;
	local value2name;

	method init( desc )
	{
		name = desc;
	}

	method getName()
	{
		return name;
	}
}

public class Enum extends Symbolic
{
	method init( desc )
	{
		[super init desc];
		basic.print( "Enum constructed\n" );
	}

	method _print( detailed )
	{
		return basic.sprintf( "<enum %r>", [super getName] );
		//return basic.sprintf( "<enum %r>", 1 );
	}
}

private el;
el = [Enum new 10];
basic.print( el );
basic.print( "\n" );
