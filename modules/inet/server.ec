package server;

from basic import *;
import file;
import inet;

private function service( sock, sockaddr )
{
	print( sprintf( "CONNECTION FROM: %r\n", sockaddr ) );

	local line;
	while (line = file.gets( sock ))
	{
		if (line == "HELLO\n")
			file.write( sock, "HELLO TO YOU\n" );
		else if (line == "BYE\n")
		{
			file.write( sock, "BYE\n" );
			return @false;
		}
		else if (line == "QUIT\n")
		{
			file.write( sock, "ADIEU\n" );
			return @true;
		}
	}
}

private function server(port)
{
	local sockfd, sock, sockaddr;
	local conn;
	local res;

	sockfd = inet.socket( port );
	inet.listen( sockfd );
	for (;;)
	{
		// Wait for a connection from a client
		conn = inet.accept( sockfd );
		sock     = conn[0];
		sockaddr = conn[1];
		res = service( sock, sockaddr );
		file.close( sock );

		if (res)
			return;
	}
}

server( 6371 );
