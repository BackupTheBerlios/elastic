package testnamedprintf;

//import basic;

from basic import print, sprintf;

// simple example
print( sprintf("%d, %s, %f, %(var1)s, %(var2)g\n",
			   12, "a string", 5.5,
			   %[#var2, 6.2,
				 #var1, "another string"]) );

// now using strings
print( sprintf("%d, %s, %f, %(var1)s, %(var2)g\n",
			   12, "a string", 5.5,
			   %["var2", 6.2,
				 "var1", "another string"]) );

// ... or both
print( sprintf("%d, %s, %f, %(var1)s, %(var2)g\n",
			   12, "a string", 5.5,
			   %["var2", 6.2,
				 #var1, "another string"]) );

// using other args after the sequence
print( sprintf("%d, %s, %f, %(var1)s, %(var2)g, %g\n",
			   12, "a string", 5.5,
			   %["var2", 6.2,
				 #var1, "another string"], 3.3) );

// let's generate an exception
print( sprintf("%d, %s, %f, %(var1)s, %(var2)g, %g\n",
			   12, "a string", 5.5,
			   %["var2", 6.2,
				 #var1, "another string"], "sdsda") );
