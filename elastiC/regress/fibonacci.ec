package fibonacci;

import basic;
import string;

private function fibonacci_r(n)
{
	if (n == 0)
		return 0;
	if (n == 1)
		return 1;
	return fibonacci_r(n - 1) + fibonacci_r(n - 2);
}

private function fibonacci_i(n)
{
	if (n == 0)
		return 0;
	if (n == 1)
		return 1;

	local f_2 = 0, f_1 = 1, f_0;
	local i = 2;

 repeat:
	f_0 = f_1 + f_2;
	if (i == n) return f_0;
	f_2 = f_1;
	f_1 = f_0;
	i++;
	goto repeat;
}

private function rabbit(n)
{
	if (n == 0)
	{
		return "0";
	}
	if (n == 1)
	{
		return "1";
	}
	return rabbit(n-1) + rabbit(n-2);
}

for (local i = 0; i < 8; i++)
	basic.printnl( "fib(", i, ") = ", fibonacci_r(i) );
for (local i = 0; i < 8; i++)
	basic.printnl( "i fib(", i, ") = ", fibonacci_i(i) );
for (local i = 0; i < 8; i++)
	basic.printnl( "rabbit(", i, ") = ", rabbit(i) );
