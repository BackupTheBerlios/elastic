package filestreamtest;

import basic;
import stream;
import filestream;

private fh;

fh = filestream.open( "/tmp/fsout.txt", "w" );
stream.puts( fh, "Hello" );
stream.puts( fh, "Row 2" );
stream.write( fh, "This is a very long string we want on the file, but only in part.", 12 );
basic.print( stream.tell( fh ), '\n' );
stream.puts( stream.stdout(), "Say me something" );
basic.print( stream.gets( stream.stdin() ), '\n' );
stream.close( fh );
