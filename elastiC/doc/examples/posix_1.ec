package posix_1;

import basic;
import posix;
import hash;

local k;

local fd = posix.open( "/tmp/ectmpfile", #[ #O_CREAT, #O_WRONLY ], #[ #S_IRUSR, #S_IWUSR ] );
basic.printnl( "fd: ", fd );
local content = "Test 1\nTest 2\nTest 3\n";
posix.write(fd, content );
posix.write(1, content );
local stat = posix.fstat(fd);
for (k in hash.keys(stat))
	basic.printnl( "fstat[", k, "] = ", stat[k] );
posix.close(fd);
basic.printnl();
stat = posix.stat("/tmp/ectmpfile");
for (k in hash.keys(stat))
	basic.printnl( "stat[", k, "] = ", stat[k] );

basic.printnl( "S_IFMT : ", posix.S_IFMT );
basic.printnl( "S_IFREG: ", posix.S_IFREG );
basic.printnl( "S_IRWXU: ", posix.S_IRWXU );
basic.printnl( "WNOHANG: ", posix.WNOHANG );
basic.printnl( "O_EXCL: ",  posix.O_EXCL );

basic.printnl( "cwd: ", posix.getcwd() );

basic.printnl( "features: " );
for (k in hash.keys(posix.implements))
	basic.printnl( "  has[", k, "] = ", posix.implements[k] );

local rv = posix.fork();
if (rv == 0)
{
	basic.printnl("CHILD");
	posix.chdir("/tmp");
	posix.execv("/bin/ls", #[ "ls", "." ]);
} else
{
	basic.printnl("PARENT");
	posix.waitpid(rv);
}
