package break2;

import basic;

local i, j;
for (i = 0; i < 10; i = i + 1)
{
	for (j = 0; j < 10; j = j + 1)
	{
		if (i == 5) break 2;
	}
}

basic.print( "i = ", i, ", j = ", j, '\n' );
