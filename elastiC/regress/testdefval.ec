package testdefval;

import basic;
import string;

private m = 8;

private function f1(a, b, c = a * b + m)
{
	return a + b + c;
}

basic.printf( "Result: %s %.3f\n" % #[1, 2.0] );
basic.printf( "Result: %s\n" % f1(2, 3) );
basic.printf( "Result: %s\n" % f1(2, 3, 1) );
