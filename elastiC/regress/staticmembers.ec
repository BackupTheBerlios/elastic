package staticmembers;
import basic;

public class myclass extends basic.Object {
	local a;
	local b;
	static s_j1 = #staticvalue1;
	static s_j2 = #staticvalue2;
	static s_j3 = #staticvalue3;
	local i;

	method print_s( )
	{
		basic.print( "s:", s_j2, "\n" );
	}
}

[[myclass new] print_s];
