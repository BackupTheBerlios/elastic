package os;

import posix;

public getcwd  = posix.getcwd;
public mkdir   = posix.mkdir;
public rmdir   = posix.rmdir;
public chdir   = posix.chdir;

public open    = posix.open;
public write   = posix.write;
public close   = posix.close;
public fstat   = posix.fstat;
public stat    = posix.stat;

public fork    = posix.fork;
public execv   = posix.execv;
public waitpid = posix.waitpid;

public S_IFMT  = posix.S_IFMT;
public S_IFREG = posix.S_IFREG;
public S_IRWXU = posix.S_IRWXU;
public WNOHANG = posix.WNOHANG;
public O_EXCL  = posix.O_EXCL;

public implements = posix.implements;
