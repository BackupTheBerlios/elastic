package simple;

// Import the `basic' package
import basic;

// Define a simple function with two parameters
function multiply(a, b)
{
	// return the product of a and b
	return a * b;
}

/*
 *  Here we start to execute package code
 */

local w1, w2, r;	// Define some local variables

w1 = 10.1;
w2 = 5;
r = multiply(w1, w2);
basic.print( w1, " * ", w2, " = ", r, "\n" );
