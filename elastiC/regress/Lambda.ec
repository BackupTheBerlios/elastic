package Lambda;

import basic;
import array;

public class Lambda extends basic.Object
{
	local argc;
	local argv;
	local clos;

	method init(closure)
	{
		clos = closure;

		local arity = basic.arity( closure );
		argc = arity[0];
		argv = #[ ];
	}

	method apply(argument)
	{
		array.push(argv, argument);

		if (array.length(argv) == argc)
			return basic.apply(clos, argv);
	}
}

local f = [Lambda new function (i1, i2, i3) { return i1 + i2 + i3; }];
basic.print([[[f apply 1] apply 1] apply 1], "\n");

local f2 = [Lambda new function (i1, i2) { return i1 - i2; }];
basic.print([[f2 apply 12] apply 5], "\n");
