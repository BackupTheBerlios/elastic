package wc;

import basic;
import stream;
import filestream;
import string;
import sys;

line = "";
tot_lines = 0;
tot_chars = 0;

fh = filestream.open( sys.args[0], "r" );
while (line = stream.gets(fh))
{
	//basic.print(line);
    tot_lines++;
    tot_chars += string.length( line );
}
stream.close( fh );

basic.print( "\t", tot_lines, "\t", tot_chars, "\n" );
