package apply;
import basic;

function testf(a, b, c)
{
	return a + b * c;
}

basic.print(basic.apply(testf, #[1, 2, 3]), "\n");
basic.print(basic.apply(testf, #[2, 2, 3]), "\n");
basic.print(basic.apply(testf, #[2, 4, 5]), "\n");

function mul(a, b) { return a * b; }
basic.print( basic.apply(mul, #[2, 3]), "\n" );
