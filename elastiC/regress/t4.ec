package t4;

import basic;

local i;

i = 5;

private function tf()
{
	local k;
	local j;

	j = 6;
	k = 7;

	basic.print( "i: ", i, "  k: ", k, "\n" );

	if (j < 5)
	{
		local k = 12;
		local n;

		n = 3;
		basic.print( "i: ", i, "  k: ", k, "  n: ", n, "\n" );
	} else
	{
		local k = 15;
		local n;

		n = 4;
		basic.print( "i: ", i, "  k: ", k, "  n: ", n, "\n" );
	}

	basic.print( "i: ", i, "  k: ", k, "\n" );

	local n = 9;
	basic.print( "n: ", n, "\n" );
}

tf();
