package mutual;

import basic;
import string;

// forward declare f2()
local f2;

function f1(i)
{
	local v;

	if (i == 0)
		return 5;

	v = i * f2(0);
	basic.print( "f1(): \n", v, "\n" );
	return v; 
}

function f2(j)
{
	local v;

	if (j == 0)
		return 12;

	v = j * f1(0);
	basic.print( "f2(): \n", v, "\n" );
	return v; 
}

f2(3);
f1(3);
