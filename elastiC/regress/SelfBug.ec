package SelfBug;

import basic;

public class MyClass extends basic.Object
{
	method init()
	{
		basic.print( self, "\n" );
	}
}

private obj;
obj = [MyClass new];
