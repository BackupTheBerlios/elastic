package test_assign;

from basic import print, sprintf;
import string;

private a, b;

a = 1;

print( sprintf( "1. a = %w\n", a ) );
a += 5;
print( sprintf( "2. a = %w\n", a ) );

b = #[ 1, 2, 3, 4, 5.0, 6.1, 7, "a string" ];
print( sprintf( "1. b = %w\n", b ) );
b[0] += 1;
b[1] *= 2;
b[2] **= 2;
b[3] -= 1;
b[4] /= 2;
b[7] += " a longer";
print( sprintf( "2. b = %w\n", b ) );
