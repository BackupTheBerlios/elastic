#!/usr/bin/perl -w

undef(@deps);
@deps = ();

%def_mods = (
	     'basic'      => 1,
	     'string'     => 1,
	     'sys'        => 1,
	     'hash'       => 1,
	     'array'      => 1,
	     'posix'      => 1,
	     'stream'     => 1,
	     'filestream' => 1,
	     'file'       => 1,
	     're'         => 1,
	     );

for ($i = 0; $i <= $#ARGV; $i++) {
    $file = $ARGV[$i];

    $deps = "";

    $basefile = $file;
    $basefile =~ s/\.ec//;

    open(IN, $file) || die "can't open file '$file': $!";
    while (<IN>) {
	chomp;

	$mod = "";
	if (/from \s+ ([a-zA-Z0-9_]+) \s+ import/x) {
	    $mod = $1;
	} elsif (/import \s+ ([a-zA-Z0-9_]+) \s* \;/x) {
	    $mod = $1;
	}

	if ($mod ne "") {
	    if (! defined($def_mods{$mod})) {
		$deps .= " " . "${mod}.ecc";
	    }
	}
    }
    close(IN);

    print "${basefile}.ecc: $file $deps\n";
}
