#!/usr/bin/perl -w     ; -*-perl-*-

while (<>) {
    chomp;

    if (/^\#define\s+(E[A-Z]+)\s+([0-9]+)/) {
	print "#ifdef $1\n";
	print <<"ENDOFHEREDOC";
	EcPackageVariable( pkg, "$1",
					   EcMakeInt( $1 ),
					   TRUE, TRUE );

	EcHashSet( value_h, EcMakeSymbol("$1"), EcMakeInt( $1 ) );
	EcHashSet( symbol_h, EcMakeInt( $1 ), EcMakeSymbol("$1") );
ENDOFHEREDOC

	print "#endif /* $1 */\n";
    }
}
