package t3;

import basic;

local i;

i = 5;

private function tf()
{
	local j;

	j = 6;

	try {
		basic.print( "HELLO\n" );
		basic.print( "i: ", i, "\n" );
		basic.print( "j: ", j, "\n" );
	} catch (basic.UnknownMethodError e) {
	}
}

tf();
