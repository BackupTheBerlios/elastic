package test_mulassign;

from basic import print, sprintf;
import string;

private a, b, c, d;

a = 1;
c = 3;
d = 4;

print( "Before assignment:\n" );
print( "a = ", a, "\n" );
print( "b = ", b, "\n" );
print( "c = ", c, "\n" );
print( "d = ", d, "\n" );

[a, b, c, d] = [d, c, b, a];

print( "\nAfter assignment:\n" );
print( "a = ", a, "\n" );
print( "b = ", b, "\n" );
print( "c = ", c, "\n" );
print( "d = ", d, "\n" );

[a, b, c, d] = [d, c, b, a];

print( "\nAfter 2nd assignment:\n" );
print( "a = ", a, "\n" );
print( "b = ", b, "\n" );
print( "c = ", c, "\n" );
print( "d = ", d, "\n" );

private arr = #[6.5, "middle", 4];
[a, b, c] = arr;

print( "\nAfter array assignment:\n" );
print( "a = ", a, "\n" );
print( "b = ", b, "\n" );
print( "c = ", c, "\n" );
