package arity;
import basic;

function f0()                   { return 6; }
function f1(a)                  { return a; }
function f2(a, b)               { return a + b; }
function f3(a, b, c)            { return a + b + c; }
function f3v(a, b, c, args ...) { return a + b + c; }

basic.print( basic.arity( f0 ), "\n" );
basic.print( basic.arity( f1 ), "\n" );
basic.print( basic.arity( f2 ), "\n" );
basic.print( basic.arity( f3 ), "\n" );
basic.print( basic.arity( f3v ), "\n" );
