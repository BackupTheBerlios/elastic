package testbinding;

import basic;

private function makefunc1(a)
{
	local i;

	i = 6;

	return function() {
		return i * a;
	};
}

private function makefunc2(a)
{
	local i;

	i = 5;

	return function() {
		return i * a;
	};
}

private f1;
private f2;

f1 = makefunc1(2);
f2 = makefunc2(2);

basic.print( f1(), '\n' );
basic.print( f2(), '\n' );
