package outofmem;

import basic;
import string;

// Array manipulation

private dd;

dd = #[ 1, 2, 3 ];
basic.print( basic.str( dd ), "\n" );

private anArray, el;

anArray = basic.array( 200000000 );

anArray[0] = 5;
anArray[1] = 4;

basic.print( "An array:\n" );
for (el in anArray)
    basic.print( el, "\n" );
