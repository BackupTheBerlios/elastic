package trunky2;

import basic;

function trunky()
{
	local funky;
	static v = 0;

	v = v + 1;

	funky = function() {
		basic.print( v, '\n' );
	};

	return funky;
}

local func;

func = trunky();
basic.print( func, '\n' );
func();
