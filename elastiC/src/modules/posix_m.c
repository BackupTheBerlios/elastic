/* ==========================================================================
 * posix_m.c
 * ==========================================================================
 * POSIX module.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Wed Jun  5 21:48:10 CEST 2002
 *
 *   $Id: posix_m.c,v 1.3 2002/06/06 16:10:57 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 2002 Marco Pantaleoni. All rights reserved.
 *
 *  The contents of this file are subject to the elastiC License version 1.0
 *  (the "elastiC License"); you may not use this file except in compliance
 *  with the elastiC License. You may obtain a copy of the elastiC License at
 *  http://www.elasticworld.org/LICENSE
 *
 *  IN NO EVENT SHALL THE AUTHOR OR DISTRIBUTORS BE LIABLE TO ANY PARTY
 *  FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 *  ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
 *  DERIVATIVES THEREOF, EVEN IF THE AUTHOR HAS BEEN ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *  THE AUTHOR AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE
 *  IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHOR AND DISTRIBUTORS HAVE
 *  NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
 *  MODIFICATIONS.
 *
 *  See the elastiC License for the specific language governing rights and
 *  limitations under the elastiC License.
 * ==========================================================================
 */

#include "basic.h"
#include "debug.h"

#include "elastic.h"
#include "private.h"
#include "compat.h"

#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#if HAVE_FCNTL_H
#include <fcntl.h>
#endif
#if HAVE_ERRNO_H
#include <errno.h>
#endif

#if HAVE_UNISTD_H
	/* this modules relies on <unistd.h> */

static EcUInt s_R_OK = 0, s_W_OK = 0, s_X_OK = 0, s_F_OK = 0;
static EcUInt s_O_RDONLY = 0, s_O_WRONLY = 0, s_O_RDWR = 0,
	s_O_CREAT = 0, s_O_EXCL = 0, s_O_NOCTTY = 0, s_O_TRUNC = 0,
	s_O_APPEND = 0, s_O_NONBLOCK = 0, s_O_NDELAY = 0,
	s_O_SYNC = 0, s_O_DSYNC = 0, s_O_RSYNC = 0,
	s_O_NOFOLLOW = 0, s_O_DIRECTORY = 0, s_O_LARGEFILE = 0;
static EcUInt s_S_IRWXU = 0,
	s_S_IRUSR = 0, s_S_IREAD = 0,
	s_S_IWUSR = 0, s_S_IWRITE = 0,
	s_S_IXUSR = 0, s_S_IEXEC = 0,
	s_S_IRWXG = 0,
	s_S_IRGRP = 0,
	s_S_IWGRP = 0,
	s_S_IXGRP = 0,
	s_S_IRWXO = 0,
	s_S_IROTH = 0,
	s_S_IWOTH = 0,
	s_S_IXOTH = 0;

static _ec_symbol2int sym2int_access_mode[] =
{
	{ 0, 0 },													/* R_OK */
	{ 0, 0 },													/* W_OK */
	{ 0, 0 },													/* X_OK */
	{ 0, 0 },													/* F_OK */
	{ 0, 0 }
};

static _ec_symbol2int sym2int_open_flags[] =
{
	{ 0, 0 },													/* O_RDONLY    */
	{ 0, 0 },													/* O_WRONLY    */
	{ 0, 0 },													/* O_RDWR      */
	{ 0, 0 },													/* O_CREAT     */
	{ 0, 0 },													/* O_EXCL      */
	{ 0, 0 },													/* O_NOCTTY    */
	{ 0, 0 },													/* O_TRUNC     */
	{ 0, 0 },													/* O_APPEND    */
	{ 0, 0 },													/* O_NONBLOCK  */
	{ 0, 0 },													/* O_NDELAY    */
	{ 0, 0 },													/* O_SYNC      */
	{ 0, 0 },													/* O_DSYNC     */
	{ 0, 0 },													/* O_RSYNC     */
	{ 0, 0 },													/* O_NOFOLLOW  */
	{ 0, 0 },													/* O_DIRECTORY */
	{ 0, 0 },													/* O_LARGEFILE */
	{ 0, 0 }
};

static _ec_symbol2int sym2int_open_mode[] =
{
	{ 0, 0 },													/* S_IRWXU  */
	{ 0, 0 },													/* S_IRUSR  */
	{ 0, 0 },													/* S_IREAD  */
	{ 0, 0 },													/* S_IWUSR  */
	{ 0, 0 },													/* S_IWRITE */
	{ 0, 0 },													/* S_IXUSR  */
	{ 0, 0 },													/* S_IEXEC  */
	{ 0, 0 },													/* S_IRWXG  */
	{ 0, 0 },													/* S_IRGRP  */
	{ 0, 0 },													/* S_IWGRP  */
	{ 0, 0 },													/* S_IXGRP  */
	{ 0, 0 },													/* S_IRWXO  */
	{ 0, 0 },													/* S_IROTH  */
	{ 0, 0 },													/* S_IWOTH  */
	{ 0, 0 },													/* S_IXOTH  */
	{ 0, 0 }
};

static EcUInt posix2symbol( int posix_errnum )
{
	switch (posix_errnum)
	{
	case 0:
		return EcInternSymbol("no-error");
#include "errno_switch.c.incl"
	default :
		return EcInternSymbol("unknown");
	}
}

static EC_OBJ posix2exception( int posix_errnum, EC_OBJ ioObject, const char *msg )
{
	EcUInt    errSym;
	ec_string ds;
	EC_OBJ    exc;

	errSym = posix2symbol( posix_errnum );

#if HAVE_STRERROR
	if (posix_errnum != 0)
	{
		ec_string_init( &ds, NULL );
		ec_sprintf( &ds, "%s: %s", msg, strerror( posix_errnum ) );
		exc = EcIOError( ioObject, errSym, ec_strdata( &ds ) );
		ec_string_cleanup( &ds );
	} else
		exc = EcIOError( ioObject, errSym, msg );
#else
	exc = EcIOError( ioObject, errSym, msg );
#endif
	return exc;
}

/* ========================================================================
 * posix.* primitives
 * ======================================================================== */

	/* files and directories */

	/* file operations */

static EC_OBJ EcLibPosix_access( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int access(const char *pathname, int mode) */

#if HAVE_ACCESS
	char   *pathname;
	EC_OBJ  mode_o;
	EcInt   mode;
	int     rv;
	EC_OBJ  res;

	res = EcParseStackFunction( "posix.access", TRUE, stack, "sO",
								&pathname, &mode_o );
	if (EC_ERRORP(res))
		return res;

	res = _ec_sequence2mask( "posix.access", 2, sym2int_access_mode, mode_o, &mode );
	if (EC_ERRORP(res)) return res;

	rv = access( pathname, (int) mode );
	if (rv < 0)
		return posix2exception( errno, EC_NIL, "in posix.access" );
	else
		return EcTrueObject;
#else
	return EcUnimplementedError( "POSIX `access' function not available" );
#endif /* HAVE_ACCESS */
}

static EC_OBJ EcLibPosix_open( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int open(const char *pathname, int mode) */

#if HAVE_OPEN
	char   *pathname;
	EC_OBJ  flags_o, mode_o = EC_NIL;
	EcInt   flags, mode = 0;
	int     rv;
	EC_OBJ  res;

	res = EcParseStackFunction( "posix.open", TRUE, stack, "sO|O",
								&pathname, &flags_o, &mode_o );
	if (EC_ERRORP(res))
		return res;

	res = _ec_sequence2mask( "posix.open", 2, sym2int_open_flags, flags_o, &flags );
	if (EC_ERRORP(res)) return res;

	if (EC_NNULLP(mode_o))
	{
		res = _ec_sequence2mask( "posix.open", 3, sym2int_open_mode, mode_o, &mode );
		if (EC_ERRORP(res)) return res;
	} else
		mode = 0;

	if (flags & O_CREAT)
		rv = open( pathname, (int) flags, (mode_t) mode );
	else
		rv = open( pathname, (int) flags );
	if (rv < 0)
		return posix2exception( errno, EC_NIL, "in posix.open" );
	else
		return EcMakeInt( rv );
#else
	return EcUnimplementedError( "POSIX `open' function not available" );
#endif /* HAVE_OPEN */
}

static EC_OBJ EcLibPosix_creat( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int creat(const char *pathname, mode_t mode) */

#if HAVE_CREAT
	char   *pathname;
	EC_OBJ  mode_o = EC_NIL;
	EcInt   mode = 0;
	int     rv;
	EC_OBJ  res;

	res = EcParseStackFunction( "posix.creat", TRUE, stack, "sO",
								&pathname, &mode_o );
	if (EC_ERRORP(res))
		return res;

	res = _ec_sequence2mask( "posix.creat", 2, sym2int_open_mode, mode_o, &mode );
	if (EC_ERRORP(res)) return res;

	rv = creat( pathname, (mode_t) mode );
	if (rv < 0)
		return posix2exception( errno, EC_NIL, "in posix.creat" );
	else
		return EcMakeInt( rv );
#elif HAVE_OPEN
	rv = open( pathname, O_CREAT, (mode_t) mode );
	if (rv < 0)
		return posix2exception( errno, EC_NIL, "in posix.creat" );
	else
		return EcMakeInt( rv );
#else
	return EcUnimplementedError( "POSIX `creat' function not available" );
#endif
}

static EC_OBJ EcLibPosix_close( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int close(int fd) */

#if HAVE_CLOSE
	int     fd;
	int     rv;
	EC_OBJ  res;

	res = EcParseStackFunction( "posix.close", TRUE, stack, "i",
								&fd );
	if (EC_ERRORP(res))
		return res;

	rv = close( fd );
	if (rv < 0)
		return posix2exception( errno, EC_NIL, "in posix.close" );
	else
		return EcTrueObject;
#else
	return EcUnimplementedError( "POSIX `close' function not available" );
#endif /* HAVE_CLOSE */
}

static EC_OBJ EcLibPosix_read( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: ssize_t read(int fd, void *buf, size_t count) */

#if HAVE_READ
	int     fd, count;
	ssize_t rv;
	EC_OBJ  buf;
	EC_OBJ  res;

	res = EcParseStackFunction( "posix.read", TRUE, stack, "ii",
								&fd, &count );
	if (EC_ERRORP(res))
		return res;

	buf = EcMakeString( "", count + 1 );
	if (EC_ERRORP(buf)) return buf;
	rv = read( fd, EC_STRDATA(buf), count );
	if (rv < 0)
		return posix2exception( errno, EC_NIL, "in posix.read" );
	else
		return EcMakeInt( rv );
#else
	return EcUnimplementedError( "POSIX `read' function not available" );
#endif /* HAVE_READ */
}

static EC_OBJ EcLibPosix_write( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: ssize_t write(int fd, const void *buf, size_t count) */

#if HAVE_WRITE
	int     fd;
	ssize_t rv;
	EC_OBJ  buf;
	EC_OBJ  res;

	res = EcParseStackFunction( "posix.write", TRUE, stack, "iO!",
								&fd, tc_string, &buf );
	if (EC_ERRORP(res))
		return res;

	rv = write( fd, EC_STRDATA(buf), EC_STRLEN(buf) );
	if (rv < 0)
		return posix2exception( errno, EC_NIL, "in posix.write" );
	else
		return EcMakeInt( rv );
#else
	return EcUnimplementedError( "POSIX `write' function not available" );
#endif /* HAVE_WRITE */
}

static EC_OBJ EcLibPosix_dup( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int dup(int oldfd) */

#if HAVE_DUP
	int    oldfd;
	int    newfd;
	EC_OBJ res;

	res = EcParseStackFunction( "posix.dup", TRUE, stack, "i",
								&oldfd );
	if (EC_ERRORP(res))
		return res;

	newfd = dup( oldfd );
	if (newfd < 0)
		return posix2exception( errno, EC_NIL, "in posix.dup" );
	else
		return EcMakeInt( newfd );
#else
	return EcUnimplementedError( "POSIX `dup' function not available" );
#endif /* HAVE_DUP */
}

static EC_OBJ EcLibPosix_dup2( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int dup2(int oldfd, int newfd) */

#if HAVE_DUP2
	int    oldfd, newfd;
	int    newfd_r;
	EC_OBJ res;

	res = EcParseStackFunction( "posix.dup2", TRUE, stack, "ii",
								&oldfd, &newfd );
	if (EC_ERRORP(res))
		return res;

	newfd_r = dup2( oldfd, newfd );
	if (newfd_r < 0)
		return posix2exception( errno, EC_NIL, "in posix.dup2" );
	else
		return EcMakeInt( newfd_r );
#else
	return EcUnimplementedError( "POSIX `dup2' function not available" );
#endif /* HAVE_DUP2 */
}

static EC_OBJ EcLibPosix_pipe( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int pipe(int filedes[2]) */

#if HAVE_PIPE
	int    filedes[2];
	int    rv;

	EC_CHECKNARGS_F("posix.pipe", 0);

	rv = pipe( filedes );
	if (rv < 0)
		return posix2exception( errno, EC_NIL, "in posix.pipe" );
	else
		return EcMakeArrayInit( 2, EcMakeInt( filedes[0] ), EcMakeInt( filedes[1] ) );
#else
	return EcUnimplementedError( "POSIX `pipe' function not available" );
#endif /* HAVE_PIPE */
}

	/* process properties */

	/* process management */

static EC_OBJ EcLibPosix_fork( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int fork(void) */

#if HAVE_FORK
	pid_t  rv;

	EC_CHECKNARGS_F("posix.fork", 0);

	rv = fork();
	if (rv < 0)
		return posix2exception( errno, EC_NIL, "in posix.fork" );
	else
		return EcMakeInt( rv );
#else
	return EcUnimplementedError( "POSIX `fork' function not available" );
#endif /* HAVE_FORK */
}

static EC_OBJ EcLibPosix_execv( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int execv(const char *path, char *const argv[]) */

#if HAVE_EXECV
	const char *pathname;
	EC_OBJ argv_o, argvl_o, el;
	EcInt  argvl, i;
	char **argv;
	pid_t  rv;
	EC_OBJ res;

	res = EcParseStackFunction( "posix.execv", TRUE, stack, "sO",
								&pathname, &argv_o );

	if (! EcIsSequence( argv_o ))
		return EC_TYPEERROR_F(/* function name    */ "posix.execv",
							  /* parameter index  */ 2,
							  /* expected         */ tc_none,
							  /* offending object */ argv_o,
							  /* reason           */ "expected a sequence of strings");
	argvl_o = EcSequenceLength( argv_o );
	if (EC_ERRORP(argvl_o)) return argvl_o;
	ASSERT( EC_INUMP(argvl_o) );
	argvl = EC_INUM(argvl_o);
	argv = alloca( sizeof(const char *) * (argvl + 1) );
	if (! argv) return EcMemoryError();
	for (i = 0; i < argvl; i++)
	{
		el = EcSequenceGetElement( argv_o, i );
		if (EC_ERRORP(el)) return el;
		if (! EC_STRINGP(el))
			return EC_TYPEERROR_F(/* function name    */ "posix.execv",
								  /* parameter index  */ 2,
								  /* expected         */ tc_none,
								  /* offending object */ argv_o,
								  /* reason           */ "expected a sequence of strings");
		argv[i] = ec_stringdup( EC_STRDATA(el) );
	}
	argv[i] = NULL;

	rv = execv(pathname, argv);
	/* we shouldn't return if everything worked */
	return posix2exception( errno, EC_NIL, "in posix.execv" );
#else
	return EcUnimplementedError( "POSIX `execv' function not available" );
#endif /* HAVE_EXECV */
}

static EC_OBJ EcLibPosix_execve( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int execve(const char *path, char *const argv[], char *const envp[]) */

#if HAVE_EXECVE
	const char *pathname;
	EC_OBJ argv_o, argvl_o, el;
	EC_OBJ envp_o, envpl_o;
	EcInt  argvl, envpl, i;
	char **argv, **envp;
	pid_t  rv;
	EC_OBJ res;

	res = EcParseStackFunction( "posix.execve", TRUE, stack, "sOO",
								&pathname, &argv_o, &envp_o );

	if (! EcIsSequence( argv_o ))
		return EC_TYPEERROR_F(/* function name    */ "posix.execv",
							  /* parameter index  */ 2,
							  /* expected         */ tc_none,
							  /* offending object */ argv_o,
							  /* reason           */ "expected a sequence of strings");
	if (! EcIsSequence( envp_o ))
		return EC_TYPEERROR_F(/* function name    */ "posix.execv",
							  /* parameter index  */ 3,
							  /* expected         */ tc_none,
							  /* offending object */ envp_o,
							  /* reason           */ "expected a sequence of strings");

	argvl_o = EcSequenceLength( argv_o );
	if (EC_ERRORP(argvl_o)) return argvl_o;
	ASSERT( EC_INUMP(argvl_o) );
	argvl = EC_INUM(argvl_o);
	argv = alloca( sizeof(const char *) * (argvl + 1) );
	if (! argv) return EcMemoryError();
	for (i = 0; i < argvl; i++)
	{
		el = EcSequenceGetElement( argv_o, i );
		if (EC_ERRORP(el)) return el;
		if (! EC_STRINGP(el))
			return EC_TYPEERROR_F(/* function name    */ "posix.execve",
								  /* parameter index  */ 2,
								  /* expected         */ tc_none,
								  /* offending object */ argv_o,
								  /* reason           */ "expected a sequence of strings");
		argv[i] = ec_stringdup( EC_STRDATA(el) );
	}
	argv[i] = NULL;

	envpl_o = EcSequenceLength( envp_o );
	if (EC_ERRORP(envpl_o)) return envpl_o;
	ASSERT( EC_INUMP(envpl_o) );
	envpl = EC_INUM(envpl_o);
	envp = alloca( sizeof(const char *) * (envpl + 1) );
	if (! envp) return EcMemoryError();
	for (i = 0; i < envpl; i++)
	{
		el = EcSequenceGetElement( envp_o, i );
		if (EC_ERRORP(el)) return el;
		if (! EC_STRINGP(el))
			return EC_TYPEERROR_F(/* function name    */ "posix.execve",
								  /* parameter index  */ 3,
								  /* expected         */ tc_none,
								  /* offending object */ envp_o,
								  /* reason           */ "expected a sequence of strings");
		envp[i] = ec_stringdup( EC_STRDATA(el) );
	}
	envp[i] = NULL;

	rv = execve(pathname, argv, envp);
	/* we shouldn't return if everything worked */
	return posix2exception( errno, EC_NIL, "in posix.execve" );
#else
	return EcUnimplementedError( "POSIX `execve' function not available" );
#endif /* HAVE_EXECVE */
}

#endif /* HAVE_UNISTD_H */

/* Private */

#if ECMODULE_POSIX_STATIC
EC_OBJ _ec_modposix_init( void )
#else
EC_API EC_OBJ ec_posix_init( void )
#endif
{
	EC_OBJ pkg;
	EC_OBJ feature;
	int i;

	pkg = EcPackageIntroduce( "posix" );
	if (EC_ERRORP(pkg))
		return pkg;

#if HAVE_UNISTD_H
	EcAddPrimitive( "posix.access",     EcLibPosix_access );
	EcAddPrimitive( "posix.open",       EcLibPosix_open );
	EcAddPrimitive( "posix.creat",      EcLibPosix_creat );
	EcAddPrimitive( "posix.close",      EcLibPosix_close );
	EcAddPrimitive( "posix.read",       EcLibPosix_read );
	EcAddPrimitive( "posix.write",      EcLibPosix_write );
	EcAddPrimitive( "posix.dup",        EcLibPosix_dup );
	EcAddPrimitive( "posix.dup2",       EcLibPosix_dup2 );
	EcAddPrimitive( "posix.pipe",       EcLibPosix_pipe );
	EcAddPrimitive( "posix.fork",       EcLibPosix_fork );
	EcAddPrimitive( "posix.execv",      EcLibPosix_execv );
	EcAddPrimitive( "posix.execve",     EcLibPosix_execve );

	/* Symbols */

	s_R_OK = EcInternSymbol( "R_OK" );
	s_W_OK = EcInternSymbol( "W_OK" );
	s_X_OK = EcInternSymbol( "X_OK" );
	s_F_OK = EcInternSymbol( "F_OK" );

	s_O_RDONLY    = EcInternSymbol( "O_RDONLY" );
	s_O_WRONLY    = EcInternSymbol( "O_WRONLY" );
	s_O_RDWR      = EcInternSymbol( "O_RDWR" );
	s_O_CREAT     = EcInternSymbol( "O_CREAT" );
	s_O_EXCL      = EcInternSymbol( "O_EXCL" );
	s_O_NOCTTY    = EcInternSymbol( "O_NOCTTY" );
	s_O_TRUNC     = EcInternSymbol( "O_TRUNC" );
	s_O_APPEND    = EcInternSymbol( "O_APPEND" );
	s_O_NONBLOCK  = EcInternSymbol( "O_NONBLOCK" );
	s_O_NDELAY    = EcInternSymbol( "O_NDELAY" );
	s_O_SYNC      = EcInternSymbol( "O_SYNC" );
	s_O_DSYNC     = EcInternSymbol( "O_DSYNC" );
	s_O_RSYNC     = EcInternSymbol( "O_RSYNC" );
	s_O_NOFOLLOW  = EcInternSymbol( "O_NOFOLLOW" );
	s_O_DIRECTORY = EcInternSymbol( "O_DIRECTORY" );
	s_O_LARGEFILE = EcInternSymbol( "O_LARGEFILE" );

	s_S_IRWXU   = EcInternSymbol( "S_IRWXU" );
	s_S_IRUSR   = EcInternSymbol( "S_IRUSR" );
	s_S_IREAD   = EcInternSymbol( "S_IREAD" );
	s_S_IWUSR   = EcInternSymbol( "S_IWUSR" );
	s_S_IWRITE  = EcInternSymbol( "S_IWRITE" );
	s_S_IXUSR   = EcInternSymbol( "S_IXUSR" );
	s_S_IEXEC   = EcInternSymbol( "S_IEXEC" );
	s_S_IRWXG   = EcInternSymbol( "S_IRWXG" );
	s_S_IRGRP   = EcInternSymbol( "S_IRGRP" );
	s_S_IWGRP   = EcInternSymbol( "S_IWGRP" );
	s_S_IXGRP   = EcInternSymbol( "S_IXGRP" );
	s_S_IRWXO   = EcInternSymbol( "S_IRWXO" );
	s_S_IROTH   = EcInternSymbol( "S_IROTH" );
	s_S_IWOTH   = EcInternSymbol( "S_IWOTH" );
	s_S_IXOTH   = EcInternSymbol( "S_IXOTH" );

	i = 0;
	sym2int_access_mode[i].symbolid = s_R_OK;
#ifdef R_OK
	sym2int_access_mode[i++].value    = R_OK;
#else
	sym2int_access_mode[i++].value    = 0;
#endif
	sym2int_access_mode[i].symbolid = s_W_OK;
#ifdef W_OK
	sym2int_access_mode[i++].value    = W_OK;
#else
	sym2int_access_mode[i++].value    = 0;
#endif
	sym2int_access_mode[i].symbolid = s_X_OK;
#ifdef X_OK
	sym2int_access_mode[i++].value    = X_OK;
#else
	sym2int_access_mode[i++].value    = 0;
#endif
	sym2int_access_mode[i].symbolid = s_F_OK;
#ifdef F_OK
	sym2int_access_mode[i++].value    = F_OK;
#else
	sym2int_access_mode[i++].value    = 0;
#endif
	ASSERT( i == 4 );

	i = 0;
	sym2int_open_flags[i].symbolid   = s_O_RDONLY;
#ifdef O_CREAT
	sym2int_open_flags[i++].value    = O_RDONLY;
#else
	sym2int_open_flags[i++].value    = 0;
#endif
	sym2int_open_flags[i].symbolid   = s_O_WRONLY;
#ifdef O_CREAT
	sym2int_open_flags[i++].value    = O_WRONLY;
#else
	sym2int_open_flags[i++].value    = 0;
#endif
	sym2int_open_flags[i].symbolid   = s_O_RDWR;
#ifdef O_CREAT
	sym2int_open_flags[i++].value    = O_RDWR;
#else
	sym2int_open_flags[i++].value    = 0;
#endif
	sym2int_open_flags[i].symbolid   = s_O_CREAT;
#ifdef O_CREAT
	sym2int_open_flags[i++].value    = O_CREAT;
#else
	sym2int_open_flags[i++].value    = 0;
#endif
	sym2int_open_flags[i].symbolid   = s_O_EXCL;
#ifdef O_EXCL
	sym2int_open_flags[i++].value    = O_EXCL;
#else
	sym2int_open_flags[i++].value    = 0;
#endif
	sym2int_open_flags[i].symbolid   = s_O_NOCTTY;
#ifdef O_NOCTTY
	sym2int_open_flags[i++].value    = O_NOCTTY;
#else
	sym2int_open_flags[i++].value    = 0;
#endif
	sym2int_open_flags[i].symbolid   = s_O_TRUNC;
#ifdef O_TRUNC
	sym2int_open_flags[i++].value    = O_TRUNC;
#else
	sym2int_open_flags[i++].value    = 0;
#endif
	sym2int_open_flags[i].symbolid   = s_O_APPEND;
#ifdef O_APPEND
	sym2int_open_flags[i++].value    = O_APPEND;
#else
	sym2int_open_flags[i++].value    = 0;
#endif
	sym2int_open_flags[i].symbolid   = s_O_NONBLOCK;
#ifdef O_NONBLOCK
	sym2int_open_flags[i++].value    = O_NONBLOCK;
#else
	sym2int_open_flags[i++].value    = 0;
#endif
	sym2int_open_flags[i].symbolid   = s_O_NDELAY;
#ifdef O_NDELAY
	sym2int_open_flags[i++].value    = O_NDELAY;
#else
	sym2int_open_flags[i++].value    = 0;
#endif
	sym2int_open_flags[i].symbolid   = s_O_SYNC;
#ifdef O_SYNC
	sym2int_open_flags[i++].value    = O_SYNC;
#else
	sym2int_open_flags[i++].value    = 0;
#endif
	sym2int_open_flags[i].symbolid   = s_O_DSYNC;
#ifdef O_DSYNC
	sym2int_open_flags[i++].value    = O_DSYNC;
#else
	sym2int_open_flags[i++].value    = 0;
#endif
	sym2int_open_flags[i].symbolid   = s_O_RSYNC;
#ifdef O_RSYNC
	sym2int_open_flags[i++].value    = O_RSYNC;
#else
	sym2int_open_flags[i++].value    = 0;
#endif
	sym2int_open_flags[i].symbolid   = s_O_NOFOLLOW;
#ifdef O_NOFOLLOW
	sym2int_open_flags[i++].value    = O_NOFOLLOW;
#else
	sym2int_open_flags[i++].value    = 0;
#endif
	sym2int_open_flags[i].symbolid   = s_O_DIRECTORY;
#ifdef O_DIRECTORY
	sym2int_open_flags[i++].value    = O_DIRECTORY;
#else
	sym2int_open_flags[i++].value    = 0;
#endif
	sym2int_open_flags[i].symbolid   = s_O_LARGEFILE;
#ifdef O_LARGEFILE
	sym2int_open_flags[i++].value    = O_LARGEFILE;
#else
	sym2int_open_flags[i++].value    = 0;
#endif
	ASSERT( i == 16 );

	i = 0;
	sym2int_open_mode[i].symbolid   = s_S_IRWXU;
#ifdef S_IRWXU
	sym2int_open_mode[i++].value    = S_IRWXU;
#else
	sym2int_open_mode[i++].value    = 0;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IRUSR;
#ifdef S_IRUSR
	sym2int_open_mode[i++].value    = S_IRUSR;
#else
	sym2int_open_mode[i++].value    = 0;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IREAD;
#ifdef S_IREAD
	sym2int_open_mode[i++].value    = S_IREAD;
#else
	sym2int_open_mode[i++].value    = 0;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IWUSR;
#ifdef S_IWUSR
	sym2int_open_mode[i++].value    = S_IWUSR;
#else
	sym2int_open_mode[i++].value    = 0;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IWRITE;
#ifdef S_IWRITE
	sym2int_open_mode[i++].value    = S_IWRITE;
#else
	sym2int_open_mode[i++].value    = 0;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IXUSR;
#ifdef S_IXUSR
	sym2int_open_mode[i++].value    = S_IXUSR;
#else
	sym2int_open_mode[i++].value    = 0;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IEXEC;
#ifdef S_IEXEC
	sym2int_open_mode[i++].value    = S_IEXEC;
#else
	sym2int_open_mode[i++].value    = 0;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IRWXG;
#ifdef S_IRWXG
	sym2int_open_mode[i++].value    = S_IRWXG;
#else
	sym2int_open_mode[i++].value    = 0;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IRGRP;
#ifdef S_IRGRP
	sym2int_open_mode[i++].value    = S_IRGRP;
#else
	sym2int_open_mode[i++].value    = 0;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IWGRP;
#ifdef S_IWGRP
	sym2int_open_mode[i++].value    = S_IWGRP;
#else
	sym2int_open_mode[i++].value    = 0;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IXGRP;
#ifdef S_IXGRP
	sym2int_open_mode[i++].value    = S_IXGRP;
#else
	sym2int_open_mode[i++].value    = 0;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IRWXO;
#ifdef S_IRWXO
	sym2int_open_mode[i++].value    = S_IRWXO;
#else
	sym2int_open_mode[i++].value    = 0;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IROTH;
#ifdef S_IROTH
	sym2int_open_mode[i++].value    = S_IROTH;
#else
	sym2int_open_mode[i++].value    = 0;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IWOTH;
#ifdef S_IWOTH
	sym2int_open_mode[i++].value    = S_IWOTH;
#else
	sym2int_open_mode[i++].value    = 0;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IXOTH;
#ifdef S_IXOTH
	sym2int_open_mode[i++].value    = S_IXOTH;
#else
	sym2int_open_mode[i++].value    = 0;
#endif
	ASSERT( i == 15 );

#endif /* HAVE_UNISTD_H */

	/* Variables */

	feature = EcMakeHash();
	if (EC_ERRORP(feature)) return feature;

	EcHashSet( feature, EcMakeSymbol("access"),
#if HAVE_UNISTD_H && HAVE_ACCESS
			   EcTrueObject );
#else
			   EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("open"),
#if HAVE_UNISTD_H && HAVE_OPEN
			   EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("creat"),
#if HAVE_UNISTD_H && (HAVE_CREAT || HAVE_OPEN)
		       EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("close"),
#if HAVE_UNISTD_H && HAVE_CLOSE
		       EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("read"),
#if HAVE_UNISTD_H && HAVE_READ
		       EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("write"),
#if HAVE_UNISTD_H && HAVE_WRITE
		       EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("dup"),
#if HAVE_UNISTD_H && HAVE_DUP
		       EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("dup2"),
#if HAVE_UNISTD_H && HAVE_DUP2
		       EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("pipe"),
#if HAVE_UNISTD_H && HAVE_PIPE
		       EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("fork"),
#if HAVE_UNISTD_H && HAVE_FORK
		       EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("execv"),
#if HAVE_UNISTD_H && HAVE_EXECV
		       EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("execve"),
#if HAVE_UNISTD_H && HAVE_EXECVE
		       EcTrueObject );
#else
		       EcFalseObject );
#endif

	EcPackageVariable( pkg, "has",
					   feature,
					   TRUE, TRUE );

	return pkg;
}

#if ECMODULE_POSIX_STATIC
void _ec_modposix_cleanup( void )
#else
EC_API void ec_posix_cleanup( void )
#endif
{
}
