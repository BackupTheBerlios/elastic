package curious;

import basic;

function trunky()
{
	local funky;
	local v;

	v = 5;

	funky = function() {
		basic.print( v, '\n' );
	};

	// we return the function stored in the variable funky
	return funky;
}

local func;
func = trunky();

// func is the function returned by trunky() ...
// ... call it !
func();
