package break1;

import basic;

local l, n, num;
l = 0;
n = 1;
num = 1024;
while (@true)
{
	if (n >= num) break;

	l = l + 1;
	n = n * 2;
}
basic.print( "l = ", l, '\n' );
