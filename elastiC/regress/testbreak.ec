package testbreak;

import basic;

private function cycle(a,b)
{
	local i;
	local j;

	for (j = 1; j <= 10; j = j + 1)
	{
		basic.print( "j: ", j, '\n' );
		basic.print( "i:  " );
		for (i = a; i <= b; i = i + 1)
		{
//			if (j == 2) continue;
			basic.print( i, ' ' );
			if (j == 5) break 2;
		}
		basic.print( '\n' );
		basic.print( "END I\n" );
	}
	basic.print( "END E\n" );
}

cycle(1, 10);
