package littletour;

import basic;
import string;

// Array manipulation

private anArray, el;

anArray = #[ 12, "Hello world !", 5.5, 4 * 3, "ABC" + "def" ];

basic.print( "An array:\n" );
for (el in anArray)
    basic.print( el, "\n" );

// String handling

private s;

s = "thks is a string";         // oops !
s[2] = 'i';                     // Ok, better
basic.print( "And the string is:\n" + s, "\n" );

s = "A multi-pieced" " string.\n"
    "It can span multiple lines also !\n";
basic.print( s );

/*
 * A little function
 *
 * Here we demonstrate how you can create
 * objects inside functions and pass around
 * them without memory handling concerns.
 */
private function makearray( u )
{
    local a, i;

    a = #[ ];               // an empty array
    for (i = 0; i < u; i++)
        a[i] = i;

    return a;
}

basic.print( makearray( 10 )[2], '\n' );
