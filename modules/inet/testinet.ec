package testinet;

from basic import *;
import file;
import inet;

private function test()
{
	local sock, line;

	sock = inet.open( "localhost", "http" );
	file.write( sock, "GET /index.html\n" );
	while (line = file.gets( sock ))
		print( line );
	file.close( sock );
}

test();
