package sum_to;

import basic;

public function sum_to( upper )
{
	local i, result;

	i      = 0;
	result = 0;

	while (i <= upper)
	{
		result = result + i;
		i = i + 1;
	}

	return result;
}
basic.print( "1 + 2 + ... + 10 = ", sum_to( 10 ), '\n' );
