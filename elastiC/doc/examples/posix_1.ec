package posix_1;

import basic;
import posix;
import hash;

local fd = posix.open( "/tmp/ectmpfile", #[ #O_CREAT, #O_WRONLY ], #[ #S_IRUSR, #S_IWUSR ] );
basic.printnl( "fd: ", fd );
local content = "Test 1\nTest 2\nTest 3\n";
posix.write(fd, content );
posix.write(1, content );
posix.close(fd);

basic.printnl( "cwd: ", posix.getcwd() );

basic.printnl( "features: " );
local k;
for (k in hash.keys(posix.has))
	basic.printnl( "  has[", k, "] = ", posix.has[k] );

local rv = posix.fork();
if (rv == 0)
{
	basic.printnl("CHILD");
	posix.chdir("/tmp");
	posix.execv("/bin/ls", #[ "ls", "." ]);
} else
{
	basic.printnl("PARENT");
}
