package testnetfile;

from basic import *;
import netfile;
import file;

private function test()
{
	local fh, line;

	fh = file.open( "tcp://localhost:http" );
	file.write( fh, "GET /index.html\n" );
	file.flush( fh );
	while (line = file.gets( fh ))
		print( line );
	file.close( fh );
}

test();
