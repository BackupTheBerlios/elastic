package testre;

import string;
import re;
from basic import print, sprintf;

private function match(subject, pattern)
{
	local reg;
	local match;

	print( "SUBJECT: ", subject, "\n" );
	print( "PATTERN: ", pattern, "\n" );
	reg   = re.compile( pattern );
	match = re.match( reg, subject );
	print( "MATCH: ", match, "\n" );

	if (match) {
		local i, range;
		for (i = 0; i < re.number(match); i++) {
			range = re.range(match, i);
			print( sprintf("GROUP %2d: %-45r [%d,%d]\n", i, re.group(match, i), range[0], range[1] ) );
		}
	}
	print( "\n" );
}

private function matchdirect(subject, pattern)
{
	local match;

	print( "SUBJECT: ", subject, "\n" );
	print( "PATTERN: ", pattern, "\n" );
	match = re.match( pattern, subject );
	print( "MATCH: ", match, "\n" );
	if (match) {
		local i, range;
		for (i = 0; i < re.number(match); i++) {
			range = re.range(match, i);
			print( sprintf("GROUP %2d: %-45r [%d,%d]\n", i, re.group(match, i), range[0], range[1] ) );
		}
	}
	print( "\n" );
}

match( "...ABCabcDEFdef...", "(bcD)" );
match( "...ABCabcDEFdef...", "bcD" );
match( "...ABCabcDEFdef...", "bCD" );

match( "Nel mezzo del cammin di nostra vita mi ritrovai per una selva oscura", "(del|di).*(del|di).*per" );


matchdirect( "...ABCabcDEFdef...", "(bcD)" );
matchdirect( "...ABCabcDEFdef...", "bcD" );
matchdirect( "...ABCabcDEFdef...", "bCD" );

matchdirect( "Nel mezzo del cammin di nostra vita mi ritrovai per una selva oscura", "(del|di).*(del|di).*per" );
