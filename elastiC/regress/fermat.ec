package fermat;

import basic;

private function issqr( n )
{
	local a, b, c;

	if (n == 0) return 0;

	a = 1;
	b = n / 2;

	while (1)
	{
		if (n == a*a) return a;
		if (n == b*b) return b;

		c = (a + b) / 2;

		if (n == c*c) return c;

		if (c == a) return -1;
		if (c == b) return -1;

		if (c*c < n)
			a = c;
		else
			b = c;
	}

	return -1;
}

private function mcd( a, b )
{
	local r1, r2, tmp;

	if (a < b)
	{
		tmp = a;
		a   = b;
		b   = tmp;
	}

	r1 = b;
	r2 = a % b;

	while (r2 != 0)
	{
		tmp = r1 % r2;

		r1 = r2;
		r2 = tmp;
	}
	return r1;
}

private function main()
{
	local a, b, c;

	for (c = 2; c < 400; c++)
	{
		for (b = c/2; b < c; b++)
		{
			if (b*b < (c*c)/2)
				continue;

			if (mcd(c, b) != 1)
				continue;

			a = issqr(c*c - b*b);

			if ((a > 0) && (a <= b))
				basic.print( basic.sprintf( "%u\t%u\t%u\n", a, b, c) );
		}
	}
}

main();
