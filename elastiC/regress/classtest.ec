package classtest;

import basic;

public class One extends basic.Object
{
	method test()
	{
		return 1;
	}

	method result1()
	{
		return [self test];
	}
}

public class Two extends One
{
	method test()
	{
		return 2;
	}
}

public class Three extends Two
{
	method result2()
	{
		return [self result1];
	}
	method result3()
	{
		return [super test];
	}
}

public class Four extends Three
{
	method test()
	{
		return 4;
	}
}

private function show(s1, s2)
{
	basic.print( basic.sprintf( "%-36s%-36s\n", s1, s2 ) );
}

private example1 = [One new];
private example2 = [Two new];
private example3 = [Three new];
private example4 = [Four new];

// First table

basic.print( "-" * 72, "\n" );
show( "EXPRESSION", "RESULT" );
basic.print( "-" * 72, "\n" );

show( "[example1 test]",    basic.repr([example1 test]) );
show( "[example1 result1]", basic.repr([example1 result1]) );
show( "[example2 test]",    basic.repr([example2 test]) );
show( "[example2 result1]", basic.repr([example2 result1]) );

basic.print( "\n" );

// Second table

basic.print( "-" * 72, "\n" );
show( "EXPRESSION", "RESULT" );
basic.print( "-" * 72, "\n" );

show( "[example3 test]",    basic.repr([example3 test]) );
show( "[example4 result1]", basic.repr([example4 result1]) );
show( "[example3 result2]", basic.repr([example3 result2]) );
show( "[example4 result2]", basic.repr([example4 result2]) );
show( "[example3 result3]", basic.repr([example3 result3]) );
show( "[example4 result3]", basic.repr([example4 result3]) );

basic.print( "\n" );
