package client;

from basic import *;
import file;
import inet;

private function client(serveraddr, port)
{
	local sock, line;

	sock = inet.open( serveraddr, port );

	file.write( sock, "HELLO\n" );
	line = file.gets( sock );
	print( line );

	file.write( sock, "BYE\n" );
	line = file.gets( sock );
	print( line );

	file.close( sock );
}

client( "localhost", 6371 );
