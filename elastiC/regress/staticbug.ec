package staticbug;

import basic;

function count()
{
	static val = 0;

	val = val + 1;
	basic.print( val, '\n' );
}

count();
count();
count();
