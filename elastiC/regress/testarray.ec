package testarray;

//import basic;
import string;

from basic import print, sprintf;

private a, i, s;

a = basic.array( 20 );
a[0] = 5;
a[2] = 2;
a[1] = 3;
a[5] = 8;
a[3] = 4;
a[4] = 9;
a[6] = 12;
a[7] = 15;

for (i = 0; i <= 7; i = i + 1)
{
	print( sprintf( "a[%d] = %w\n", i, a[i] ) );
}

s = "Hello Goofy !\nHow are you ?";
s[2] = 'L';
basic.print( s + " Fine, thanks !", '\n' );
basic.print( s[3] );

s = 'n';
basic.print( s, '\n' );

basic.print( "This is an integer " + basic.str(12), '\n' );
basic.print( "This is a floating point " + basic.str(34.5), '\n' );

s = "Hello World ! In elastiC this time";
basic.print( "len: ", string.length( s ), '\n' );
basic.print( s, '\n' );
basic.print( string.sub( s, 6, 7 ), '\n' );

private function makearray( u )
{
	local a, i;

	a = basic.array( 10 );
	for (i = 0; i < 10; i = i + 1)
		a[i] = u - i;
	return a;
}

basic.print( makearray( 10 )[2], '\n' );

s = "A multi-pieced" " string.\n"
    "It can span multiple lines also !";
basic.print( s, '\n' );

private jjk, k = 9, jj;

private function double( n )
{
	return n * 2;
}

basic.print( k, '\n' );
k = #[ 1, 2, 3, "Hello", 5.5, 4 + 2 * 3, double(5.2), makearray, 'P' ];
basic.print( k, '\n' );

i = 5;
basic.print( i < 3 ? i + 2 : -i, '\n' );

basic.print( i++ * 3, '\n' );
basic.print( i, '\n' );
basic.print( ++i * 3, '\n' );

basic.print( k[2], '\n' );
k[2]++;
basic.print( k[2], '\n' );
++k[2];
basic.print( k[2], '\n' );
basic.print( k[2]++ * 3, '\n' );
basic.print( k[2], '\n' );
basic.print( ++k[2] * 3, '\n' );
basic.print( k[2], '\n' );

for (i in k)
	basic.print( i, ' ' );
basic.print( '\n' );

for (i in "Hello " + "Sushi")
	basic.print( i );
basic.print( '\n' );

print( sprintf( "*%13s*,*%12.4f*,*0x%x*,%w\n", "elastiC", 15.5, 121, #[ 1, 2, basic.print, double, 5.6] ) );
