package errno_ex;

import basic;
import hash;
import errno;

local k;

for (k in hash.keys(errno.value))
	basic.printnl( "errno[", k, "] = ", errno.value[k] );

for (k in hash.keys(errno.symbol))
	basic.printnl( "symbol[", k, "] = ", errno.symbol[k] );
