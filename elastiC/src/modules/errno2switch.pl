#!/usr/bin/perl -w     ; -*-perl-*-

while (<>) {
    chomp;

    if (/^\#define\s+(E[A-Z]+)\s+([0-9]+)/) {
	$casestr = sprintf("%-24s", "case $1:");
	print "#ifdef $1\n";
	print "\t$casestr return EcInternSymbol( \"$1\" );\n";
	print "#endif /* $1 */\n";
    }
}
