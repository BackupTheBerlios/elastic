package test;

import basic;
import file;
import string;

from db import *;

// Read in the passwd file and create a database

private fh, ln;
private fields, login, uid, gid, name, home;

fh = file.open("/etc/passwd", "r");
ln = file.gets(fh);
while (ln && (ln != ""))
{
	fields = string.split(string.trim(ln), ":");
	login  = fields[0];
	uid    = basic.int(fields[2]);
	gid    = basic.int(fields[3]);
	name   = fields[4];
	home   = fields[5];
	add_user( #[login, uid, gid, name, home] );
	ln = file.gets(fh);
}
file.close(fh);

// Sort the users
sort_users();

// Find a few users

basic.print( "Looking for root\n" );
basic.print( lookup_user("root"), "\n" );
basic.print( "Looking for marco\n" );
basic.print( lookup_user("marco"), "\n" );

// print out all users
basic.print( "All users\n" );
private i;
for (i = 0; i < num_users(); i++)
	basic.print( get_user(i), "\n" );

