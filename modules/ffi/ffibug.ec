package ffibug;

from basic import *;
import ffi;

local dlh;

local strlen_func;
local res;

dlh = ffi.open( @nil );

strlen_func = ffi.declare(dlh, "strlen", #int, #[#string]);

print( "***\n" );
res = sprintf( "strlen(\"mickey\") = %d\n", strlen_func("mickey") );
print( res );
print( "***\n" );

print( "strlen(\"mickey\") = ", strlen_func("mickey"), "\n" );
print( "***\n" );
