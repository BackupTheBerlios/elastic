package hashtry2;

import basic;
import string;
import hash;

private limit = 70, incr = 1;
private d, ht, i;

ht = %[ ];


for (d = 1; d < limit; d = d + incr)
{
	basic.print( "D: ", d, '\n' );
	for (i = 0; i < d; i++)
	{
		ht[i] = i * 5 - 2;
		ht[i] = i * 5 - 2;
		ht[i] = i * 5 - 2;
		ht[i] = i * 5 - 2;
	}

	for (i = d - 1; i >= 0; i--)
	{
		if (ht[i] != i * 5 - 2)
			basic.print( "D: ", d, "  ERROR: ht[", i, "] = ", ht[i], "  expected: ", i * 5 - 2, '\n' );
	}
}
