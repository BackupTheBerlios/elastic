package testenv;

import basic;
import sys;
import string;

public class EnvArray extends basic.Object
{
	method _getitem( idx ) {
		return sys.getenv( idx );
	}

	method _setitem( idx, val ) {
		sys.setenv( idx, val );
	}
}

public env = [EnvArray new];
private testenv = "ECTEST";

basic.print( "PATH: '", env["PATH"], "'\n" );
basic.print( "ECTEST    : '", env[testenv], "'\n" );
env[testenv] = "Hello";
basic.print( "Now ECTEST: '", env[testenv], "'\n" );
