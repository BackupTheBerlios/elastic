package wc;

import basic;
import stream;
import filestream;
import string;
import sys;

private line = "";
private tot_lines = 0;
private tot_chars = 0;

private fh = filestream.open( sys.args[0], "r" );
while (line = stream.gets(fh))
{
	//basic.print(line);
    tot_lines++;
    tot_chars += string.length( line );
}
stream.close( fh );

basic.print( "\t", tot_lines, "\t", tot_chars, "\n" );
