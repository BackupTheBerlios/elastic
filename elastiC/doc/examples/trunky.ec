package trunky;

import basic;

function trunky()
{
	local funky;
	local v;

	v = 5;

	funky = function() {
		basic.print( v, '\n' );
	};

	funky();
}

trunky();
