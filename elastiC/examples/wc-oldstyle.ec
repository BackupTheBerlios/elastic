package wc;

import basic;
import file;
import string;
import sys;

private fh, line = "";
private tot_lines = 0, tot_chars = 0;

fh = file.open( sys.args[0], "r" );
while (line = file.gets(fh))
{
    tot_lines++;
    tot_chars += string.length( line );
}
file.close( fh );

basic.print( "\t", tot_lines, "\t", tot_chars, "\n" );
