package squarecount;

import basic;

function squarecount()
{
	static val = 0;    // a static variable
	local  v;          // a local variable

	val = val + 1;
	v = val * val;
	basic.print( v, '\n' );
}

squarecount();
squarecount();
squarecount();
