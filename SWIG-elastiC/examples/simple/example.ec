from basic import *;
import example;

print( "get_time: ", example.get_time(), "\n" );
print( "My_Variable = ", example.get_My_variable(), "\n" );

local i, j, n;
for (i = 0; i < 14; i++)
{
    n = example.fact( i );
    print( "fact(", i, ") = ", n, "\n" );
}


for (i = 1; i < 250; i++) {
    for (j = 1; j < 250; j++) {
		n = example.mod( i, j );
		example.set_My_variable( example.get_My_variable() + n );
    }
}

print( "My_variable = ", example.get_My_variable(), "\n" );
