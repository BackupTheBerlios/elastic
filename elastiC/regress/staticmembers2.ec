package staticmembers2;

import basic;
import staticmembers;

public class myclass2 extends staticmembers.myclass {
	method print( )
	{
		[super print_s];
	}
}

[[staticmembers.myclass new] print_s];
[[myclass2 new] print];
