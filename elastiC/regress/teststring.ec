package teststring;

import string;
from basic import print, sprintf;

local s = "   ABCabcDEFdef   ";

print( sprintf( "s        = '%s'\n", s ) );
print( sprintf( "ltrim(s) = '%s'\n", string.ltrim( s ) ) );
print( sprintf( "rtrim(s) = '%s'\n", string.rtrim( s ) ) );
print( sprintf( "trim(s)  = '%s'\n", string.trim( s ) ) );

print( "\n" );

private function find(s, sub)
{
	local pos;

	pos = string.find(s, sub);
	print( sprintf( "find(s, %-8s) = %d\n", sprintf( "\"%s\"", sub ), pos ) );
}

s = "Nel mezzo del cammin di nostra vita, mi ritrovai per una selva oscura...";
print( sprintf( "s = '%s'\n   (Dante, \"Divina Commedia\")\n\n", s ) );
find(s, "mezzo");
find(s, "del");
find(s, "Nel");
find(s, "a...");

local l;
l = string.split( s, " " );
print( l, "\n" );
print( string.split( s ), "\n" );
print( string.join( l, ":" ), "\n" );
