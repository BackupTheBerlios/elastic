package testnewsyn;

//import basic;

from basic import print, sprintf;

// new hash syntax
private v = { #var1 : "hello", #var2 : 6.2, "var3": "world" };

print( v[#var2], "\n" );
print( v[#var1], "\n" );
print( v["var3"], "\n" );

// new array syntax

v = (,);
print( v, "\n" );

v = (1,);
print( v, "\n" );

v = (1,2);
print( v, "\n" );

v = (1,2,);
print( v, "\n" );

v = (1,2,3);
print( v, "\n" );

v = (1,2,3,);
print( v, "\n" );
