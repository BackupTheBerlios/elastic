package os_ex;

import basic;
import os;
import hash;

local k;

local fd = os.open( "/tmp/ectmpfile", #[ #O_CREAT, #O_WRONLY ], #[ #S_IRUSR, #S_IWUSR ] );
basic.printnl( "fd: ", fd );
local content = "Test 1\nTest 2\nTest 3\n";
os.write(fd, content );
os.write(1, content );
local stat = os.fstat(fd);
for (k in hash.keys(stat))
	basic.printnl( "fstat[", k, "] = ", stat[k] );
os.close(fd);
basic.printnl();
stat = os.stat("/tmp/ectmpfile");
for (k in hash.keys(stat))
	basic.printnl( "stat[", k, "] = ", stat[k] );

basic.printnl( "S_IFMT : ", os.S_IFMT );
basic.printnl( "S_IFREG: ", os.S_IFREG );
basic.printnl( "S_IRWXU: ", os.S_IRWXU );
basic.printnl( "WNOHANG: ", os.WNOHANG );
basic.printnl( "O_EXCL: ",  os.O_EXCL );

basic.printnl( "cwd: ", os.getcwd() );

basic.printnl( "features: " );
for (k in hash.keys(os.implements))
	basic.printnl( "  has[", k, "] = ", os.implements[k] );

local rv = os.fork();
if (rv == 0)
{
	basic.printnl("CHILD");
	os.chdir("/tmp");
	os.execv("/bin/ls", #[ "ls", "." ]);
} else
{
	basic.printnl("PARENT");
	os.waitpid(rv);
}
