package sm;
import basic;

public class myclass extends basic.Object {
	local a;
	local b;
	local c;

	static s_j = #[15, 12, 14];
	local i;

	method print_s( )
	{
		basic.print( "Self: ", self, "   s_j: ", s_j, '\n' );
	}
}

public class myclass2 extends myclass {
	method print( )
	{
		[super print_s];
	}
}

private obj;

obj = [myclass2 new];
[obj print];

//private obj;

//obj = [myclass new];
//[obj print_s];
