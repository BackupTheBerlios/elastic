package filetest;

import basic;
import file;

private fh;

fh = file.open( "/tmp/out.txt", "w" );
file.puts( fh, "Hello" );
file.puts( fh, "Row 2" );
file.write( fh, "This is a very long string we want on the file, but only in part.", 12 );
basic.print( file.tell( fh ), '\n' );
file.puts( file.stdout, "Say me something" );
basic.print( file.gets( file.stdin ), '\n' );
file.close( fh );
