package testnamedprintf;

//import basic;

from basic import print, sprintf;

// simple example
print( "%d, %s, %f\n" % #[ 12, "a string", 5.5 ] );

print( "%(var1)s, %(var2)g\n" % %[#var2, 6.2, #var1, "another string"] );
