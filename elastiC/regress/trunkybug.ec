package trunkybug;

import basic;

function trunky()
{
	static v = 3;

	v = v + 1;

	return function() {
		basic.print( v, '\n' );
		v = v + 1;
	};
}

local func;

func = trunky();
func();
func();
func();
func();
