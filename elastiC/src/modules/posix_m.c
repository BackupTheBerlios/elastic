/* ==========================================================================
 * posix_m.c
 * ==========================================================================
 * POSIX module.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Wed Jun  5 21:48:10 CEST 2002
 *
 *   $Id: posix_m.c,v 1.5 2002/06/06 19:19:19 panta Exp $
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
#if HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif
#if HAVE_FCNTL_H
#include <fcntl.h>
#endif
#if HAVE_SIGNAL_H
#include <signal.h>
#endif
#if HAVE_DIRENT_H
#include <dirent.h>
#endif
#if HAVE_LIMITS_H
#include <limits.h>
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
static EcUInt s_WNOHANG = 0, s_WUNTRACED = 0;

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

static _ec_symbol2int sym2int_waitpid_options[] =
{
	{ 0, 0 },													/* WNOHANG   */
	{ 0, 0 },													/* WUNTRACED */
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

/* If PATH_MAX is not defined, look for MAXPATHLEN */
#if !defined (PATH_MAX)
#  if defined (HAVE_SYS_PARAM_H)
#    include <sys/param.h>
#    define maxpath_param_h
#  endif
#  if defined (MAXPATHLEN) && !defined (PATH_MAX)
#    define PATH_MAX MAXPATHLEN
#  endif /* MAXPATHLEN && !PATH_MAX */
#endif /* !PATH_MAX */

/* Default POSIX values */
#if !defined (PATH_MAX) && defined (_POSIX_PATH_MAX)
#  define PATH_MAX _POSIX_PATH_MAX
#endif

/* Default values */
#if !defined (PATH_MAX)
#  define PATH_MAX 4096
#endif

#if PATH_MAX < 4096
#  undef PATH_MAX
#  define PATH_MAX 4096
#endif

static EC_OBJ EcLibPosix_getcwd( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: char *getcwd(char *buf, size_t size) */

#if HAVE_GETCWD
	size_t  size;
	char   *buf;
	char   *rv;

	EC_CHECKNARGS_F("posix.getcwd", 0);

	size = PATH_MAX;
	do
	{
		buf = alloca( size+1 );
		if (! buf) return EcMemoryError();
		rv = getcwd( buf, size );
		buf[size] = '\0';
		if ((rv == NULL) && (errno == ERANGE))
		{
			if (size * 2 < size)
				return EcMemoryError();
			size *= 2;
		}
	} while ((rv == NULL) && (errno == ERANGE));

	if (rv == NULL)
		return posix2exception( errno, EC_NIL, "in posix.getcwd" );
	return EcMakeString( buf, 0 );
#else
	return EcUnimplementedError( "POSIX `getcwd' function not available" );
#endif /* HAVE_GETCWD */
}

static EC_OBJ EcLibPosix_mkdir( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int mkdir(const char *pathname, mode_t mode) */

#if HAVE_MKDIR
	char   *pathname;
	EC_OBJ  mode_o = EC_NIL;
	EcInt   mode_i = 0;
	mode_t  mode   = 0;
	int     rv;
	EC_OBJ  res;

	res = EcParseStackFunction( "posix.mkdir", TRUE, stack, "s|O",
								&pathname, &mode_o );
	if (EC_ERRORP(res))
		return res;

	if (EC_NNULLP(mode_o))
	{
		if (EC_INUMP(mode_o))
			mode_i = EC_INUM(mode_o);
		else
		{
			res = _ec_sequence2mask( "posix.mkdir", 2, sym2int_open_mode, mode_o, &mode_i );
			if (EC_ERRORP(res)) return res;
		}
	} else
		mode_i = 0;

	mode = (mode_t) mode_i;

	rv = mkdir( pathname, mode );
	if (rv < 0)
		return posix2exception( errno, EC_NIL, "in posix.mkdir" );
	else
		return EcTrueObject;
#else
	return EcUnimplementedError( "POSIX `mkdir' function not available" );
#endif /* HAVE_MKDIR */
}

static EC_OBJ EcLibPosix_rmdir( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int mkdir(const char *pathname) */

#if HAVE_RMDIR
	char   *pathname;
	int     rv;
	EC_OBJ  res;

	res = EcParseStackFunction( "posix.rmdir", TRUE, stack, "s",
								&pathname );
	if (EC_ERRORP(res))
		return res;

	rv = rmdir( pathname );
	if (rv < 0)
		return posix2exception( errno, EC_NIL, "in posix.rmdir" );
	else
		return EcTrueObject;
#else
	return EcUnimplementedError( "POSIX `rmdir' function not available" );
#endif /* HAVE_RMDIR */
}

static EC_OBJ EcLibPosix_chdir( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int chdir(const char *path) */

#if HAVE_CHDIR
	char   *path;
	int     rv;
	EC_OBJ  res;

	res = EcParseStackFunction( "posix.chdir", TRUE, stack, "s",
								&path );
	if (EC_ERRORP(res))
		return res;

	rv = chdir( path );
	if (rv < 0)
		return posix2exception( errno, EC_NIL, "in posix.chdir" );
	else
		return EcTrueObject;
#else
	return EcUnimplementedError( "POSIX `chdir' function not available" );
#endif /* HAVE_CHDIR */
}

static EC_OBJ EcLibPosix_link( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int link(const char *oldpath, const char *newpath) */

#if HAVE_LINK
	char   *oldpath, *newpath;
	int     rv;
	EC_OBJ  res;

	res = EcParseStackFunction( "posix.link", TRUE, stack, "ss",
								&oldpath, &newpath );
	if (EC_ERRORP(res))
		return res;

	rv = link( oldpath, newpath );
	if (rv < 0)
		return posix2exception( errno, EC_NIL, "in posix.link" );
	else
		return EcTrueObject;
#else
	return EcUnimplementedError( "POSIX `link' function not available" );
#endif /* HAVE_LINK */
}

static EC_OBJ EcLibPosix_unlink( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int unlink(const char *pathname) */

#if HAVE_UNLINK
	char   *pathname;
	int     rv;
	EC_OBJ  res;

	res = EcParseStackFunction( "posix.unlink", TRUE, stack, "s",
								&pathname );
	if (EC_ERRORP(res))
		return res;

	rv = unlink( pathname );
	if (rv < 0)
		return posix2exception( errno, EC_NIL, "in posix.unlink" );
	else
		return EcTrueObject;
#else
	return EcUnimplementedError( "POSIX `unlink' function not available" );
#endif /* HAVE_UNLINK */
}

static EC_OBJ EcLibPosix_rename( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int rename(const char *oldpath, const char *newpath) */

#if HAVE_RENAME
	char   *oldpath, *newpath;
	int     rv;
	EC_OBJ  res;

	res = EcParseStackFunction( "posix.rename", TRUE, stack, "ss",
								&oldpath, &newpath );
	if (EC_ERRORP(res))
		return res;

	rv = rename( oldpath, newpath );
	if (rv < 0)
		return posix2exception( errno, EC_NIL, "in posix.rename" );
	else
		return EcTrueObject;
#else
	return EcUnimplementedError( "POSIX `rename' function not available" );
#endif /* HAVE_RENAME */
}

static EC_OBJ EcLibPosix_chmod( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int chmod(const char *path, mode_t mode) */

#if HAVE_CHMOD
	char   *path;
	EC_OBJ  mode_o = EC_NIL;
	EcInt   mode_i = 0;
	mode_t  mode   = 0;
	int     rv;
	EC_OBJ  res;

	res = EcParseStackFunction( "posix.chmod", TRUE, stack, "sO",
								&path, &mode_o );
	if (EC_ERRORP(res))
		return res;

	if (EC_NNULLP(mode_o))
	{
		if (EC_INUMP(mode_o))
			mode_i = EC_INUM(mode_o);
		else
		{
			res = _ec_sequence2mask( "posix.chmod", 2, sym2int_open_mode, mode_o, &mode_i );
			if (EC_ERRORP(res)) return res;
		}
	} else
		mode_i = 0;

	mode = (mode_t) mode_i;

	rv = chmod( path, mode );
	if (rv < 0)
		return posix2exception( errno, EC_NIL, "in posix.chmod" );
	else
		return EcTrueObject;
#else
	return EcUnimplementedError( "POSIX `chmod' function not available" );
#endif /* HAVE_CHMOD */
}

static EC_OBJ EcLibPosix_chown( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int chown(const char *path, uid_t owner, gid_t group) */

#if HAVE_CHOWN
	char  *path;
	EcInt  owner_i, group_i;
	uid_t  owner;
	gid_t  group;
	int    rv;
	EC_OBJ res;

	res = EcParseStackFunction( "posix.chown", TRUE, stack, "sii",
								&path, &owner_i, &group_i );
	if (EC_ERRORP(res))
		return res;

	owner = (uid_t) owner_i;
	group = (gid_t) group_i;

	rv = chown( path, owner, group );
	if (rv < 0)
		return posix2exception( errno, EC_NIL, "in posix.chown" );
	else
		return EcTrueObject;
#else
	return EcUnimplementedError( "POSIX `chown' function not available" );
#endif /* HAVE_CHOWN */
}

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
	EC_OBJ  flags_o = EC_NIL, mode_o = EC_NIL;
	EcInt   flags_i = 0, mode_i = 0;
	int     flags;
	mode_t  mode;
	int     rv;
	EC_OBJ  res;

	flags_o = EcMakeArrayInit( 1, EcMakeSymbol("O_RDONLY") );
	if (EC_ERRORP(flags_o)) return flags_o;

	res = EcParseStackFunction( "posix.open", TRUE, stack, "s|OO",
								&pathname, &flags_o, &mode_o );
	if (EC_ERRORP(res))
		return res;

	res = _ec_sequence2mask( "posix.open", 2, sym2int_open_flags, flags_o, &flags_i );
	if (EC_ERRORP(res)) return res;

	if (EC_NNULLP(mode_o))
	{
		if (EC_INUMP(mode_o))
			mode_i = EC_INUM(mode_o);
		else
		{
			res = _ec_sequence2mask( "posix.open", 3, sym2int_open_mode, mode_o, &mode_i );
			if (EC_ERRORP(res)) return res;
		}
	} else
		mode_i = 0;

	flags = (int)    flags_i;
	mode  = (mode_t) mode_i;

	if (flags & O_CREAT)
		rv = open( pathname, flags, mode );
	else
		rv = open( pathname, flags );
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
	EcInt   mode_i = 0;
	mode_t  mode;
	int     rv;
	EC_OBJ  res;

	res = EcParseStackFunction( "posix.creat", TRUE, stack, "s|O",
								&pathname, &mode_o );
	if (EC_ERRORP(res))
		return res;

	if (EC_NNULLP(mode_o))
	{
		if (EC_INUMP(mode_o))
			mode_i = EC_INUM(mode_o);
		else
		{
			res = _ec_sequence2mask( "posix.creat", 2, sym2int_open_mode, mode_o, &mode_i );
			if (EC_ERRORP(res)) return res;
		}
	} else
		mode_i = 0;

	mode = (mode_t) mode_i;

	rv = creat( pathname, mode );
	if (rv < 0)
		return posix2exception( errno, EC_NIL, "in posix.creat" );
	else
		return EcMakeInt( rv );
#elif HAVE_OPEN
	rv = open( pathname, O_CREAT, mode );
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
	EcInt   fd_i;
	int     fd;
	int     rv;
	EC_OBJ  res;

	res = EcParseStackFunction( "posix.close", TRUE, stack, "i",
								&fd_i );
	if (EC_ERRORP(res))
		return res;

	fd = (int) fd_i;

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
	EcInt   fd_i, count;
	int     fd;
	ssize_t rv;
	EC_OBJ  buf;
	EC_OBJ  res;

	res = EcParseStackFunction( "posix.read", TRUE, stack, "ii",
								&fd_i, &count );
	if (EC_ERRORP(res))
		return res;

	fd = (int) fd_i;

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
	EcInt   fd_i;
	int     fd;
	ssize_t rv;
	EC_OBJ  buf;
	EC_OBJ  res;

	res = EcParseStackFunction( "posix.write", TRUE, stack, "iO!",
								&fd_i, tc_string, &buf );
	if (EC_ERRORP(res))
		return res;

	fd = (int) fd_i;

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
	EcInt  oldfd_i;
	int    oldfd;
	int    newfd;
	EC_OBJ res;

	res = EcParseStackFunction( "posix.dup", TRUE, stack, "i",
								&oldfd_i );
	if (EC_ERRORP(res))
		return res;

	oldfd = (int) oldfd_i;

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
	EcInt  oldfd_i, newfd_i;
	int    oldfd, newfd;
	int    newfd_r;
	EC_OBJ res;

	res = EcParseStackFunction( "posix.dup2", TRUE, stack, "ii",
								&oldfd_i, &newfd_i );
	if (EC_ERRORP(res))
		return res;

	oldfd = (int) oldfd_i;
	newfd = (int) newfd_i;

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
	if (EC_ERRORP(res))
		return res;

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
	if (EC_ERRORP(res))
		return res;

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

static EC_OBJ EcLibPosix_execvp( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int execvp(const char *file, char *const argv[]) */

#if HAVE_EXECVP
	const char *filename;
	EC_OBJ argv_o, argvl_o, el;
	EcInt  argvl, i;
	char **argv;
	pid_t  rv;
	EC_OBJ res;

	res = EcParseStackFunction( "posix.execvp", TRUE, stack, "sO",
								&filename, &argv_o );
	if (EC_ERRORP(res))
		return res;

	if (! EcIsSequence( argv_o ))
		return EC_TYPEERROR_F(/* function name    */ "posix.execvp",
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
			return EC_TYPEERROR_F(/* function name    */ "posix.execvp",
								  /* parameter index  */ 2,
								  /* expected         */ tc_none,
								  /* offending object */ argv_o,
								  /* reason           */ "expected a sequence of strings");
		argv[i] = ec_stringdup( EC_STRDATA(el) );
	}
	argv[i] = NULL;

	rv = execvp(filename, argv);
	/* we shouldn't return if everything worked */
	return posix2exception( errno, EC_NIL, "in posix.execvp" );
#else
	return EcUnimplementedError( "POSIX `execvp' function not available" );
#endif /* HAVE_EXECVP */
}

static EC_OBJ waitstatus2hash( int status )
{
	EC_OBJ status_h;
	status_h = EcMakeHash();
	if (EC_ERRORP(status_h)) return status_h;

	EcHashSet( status_h, EcMakeSymbol("ifexited"),   EcMakeBool(WIFEXITED(status)) );
	EcHashSet( status_h, EcMakeSymbol("exitstatus"), WIFEXITED(status) ? EcMakeInt(WEXITSTATUS(status)) : EC_NIL );
	EcHashSet( status_h, EcMakeSymbol("ifsignaled"), EcMakeBool(WIFSIGNALED(status)) );
	EcHashSet( status_h, EcMakeSymbol("termsig"),    WIFSIGNALED(status) ? EcMakeInt(WTERMSIG(status)) : EC_NIL );
	EcHashSet( status_h, EcMakeSymbol("ifstopped"),  EcMakeBool(WIFSTOPPED(status)) );
	EcHashSet( status_h, EcMakeSymbol("stopsig"),    WIFSTOPPED(status) ? EcMakeBool(WSTOPSIG(status)) : EC_NIL );

	return status_h;
}

static EC_OBJ EcLibPosix_wait( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: pid_t wait(int *status) */

#if HAVE_WAIT
	pid_t  rv;
	EC_OBJ status_h;
	int    status;

	EC_CHECKNARGS_F("posix.wait", 0);

	rv = wait( &status );
	if (rv <= 0)
		return posix2exception( errno, EC_NIL, "in posix.wait" );

	status_h = waitstatus2hash( status );
	if (EC_ERRORP(status_h)) return status_h;

	return EcMakeArrayInit( 2,
							EcMakeInt( rv ),
							status_h );
#else
	return EcUnimplementedError( "POSIX `wait' function not available" );
#endif /* HAVE_WAIT */
}

static EC_OBJ EcLibPosix_waitpid( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: pid_t waitpid(pid_t pid, int *status, int options) */

#if HAVE_WAITPID
	EcInt  pid_i;
	pid_t  pid;
	EC_OBJ options_o = EC_NIL;
	int    options = 0;
	pid_t  rv;
	EC_OBJ status_h;
	int    status;
	EC_OBJ res;

	res = EcParseStackFunction( "posix.waitpid", TRUE, stack, "i|O",
								&pid_i, &options_o );
	if (EC_ERRORP(res))
		return res;

	pid = (pid_t) pid_i;

	res = _ec_sequence2mask( "posix.waitpid", 2, sym2int_waitpid_options, options_o, &options );
	if (EC_ERRORP(res)) return res;

	rv = waitpid( pid, &status, options );
	if (rv <= 0)
		return posix2exception( errno, EC_NIL, "in posix.waitpid" );

	status_h = waitstatus2hash( status );
	if (EC_ERRORP(status_h)) return status_h;

	return EcMakeArrayInit( 2,
							EcMakeInt( rv ),
							status_h );
#else
	return EcUnimplementedError( "POSIX `waitpid' function not available" );
#endif /* HAVE_WAITPID */
}

static EC_OBJ EcLibPosix__exit( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: void _exit(int status) */

#if HAVE__EXIT
	EcInt  status_i;
	int    status;
	EC_OBJ res;

	res = EcParseStackFunction( "posix._exit", TRUE, stack, "i",
								&status_i );
	if (EC_ERRORP(res))
		return res;

	status = (int) status_i;

	_exit( status );
	/* _exit never returns ! */
	return posix2exception( errno, EC_NIL, "in posix._exit" );
#else
	return EcUnimplementedError( "POSIX `_exit' function not available" );
#endif /* HAVE__EXIT */
}

static EC_OBJ EcLibPosix_kill( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int kill(pid_t pid, int sig) */

#if HAVE_KILL
	EcInt  pid_i, sig_i;
	pid_t  pid;
	int    sig;
	int    rv;
	EC_OBJ res;

	res = EcParseStackFunction( "posix.kill", TRUE, stack, "ii",
								&pid_i, &sig_i );
	if (EC_ERRORP(res))
		return res;

	pid = (pid_t) pid_i;
	sig = (int)   sig_i;

	rv = kill( pid, sig );
	if (rv < 0)
		return posix2exception( errno, EC_NIL, "in posix.kill" );
	else
		return EcTrueObject;
#else
	return EcUnimplementedError( "POSIX `kill' function not available" );
#endif /* HAVE_KILL */
}

static EC_OBJ EcLibPosix_sleep( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int sleep(unsigned int seconds) */

#if HAVE_SLEEP
	EcInt        seconds_i;
	unsigned int seconds, rv;
	EC_OBJ       res;

	res = EcParseStackFunction( "posix.sleep", TRUE, stack, "i",
								&seconds_i );
	if (EC_ERRORP(res))
		return res;

	seconds = (unsigned int) seconds_i;

	rv = sleep( seconds );
	return EcMakeInt( rv );
#else
	return EcUnimplementedError( "POSIX `sleep' function not available" );
#endif /* HAVE_SLEEP */
}

static EC_OBJ EcLibPosix_pause( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int pause(void) */

#if HAVE_PAUSE
	int rv;

	EC_CHECKNARGS_F("posix.pause", 0);

	rv = pause();
	return posix2exception( errno, EC_NIL, "in posix.fork" );
#else
	return EcUnimplementedError( "POSIX `pause' function not available" );
#endif /* HAVE_PAUSE */
}

static EC_OBJ EcLibPosix_alarm( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: unsigned int alarm(unsigned int seconds) */

#if HAVE_ALARM
	EcInt        seconds_i;
	unsigned int seconds, rv;
	EC_OBJ       res;

	res = EcParseStackFunction( "posix.alarm", TRUE, stack, "i",
								&seconds_i );
	if (EC_ERRORP(res))
		return res;

	seconds = (unsigned int) seconds_i;

	rv = alarm( seconds );
	return EcMakeInt( rv );
#else
	return EcUnimplementedError( "POSIX `alarm' function not available" );
#endif /* HAVE_ALARM */
}

static EC_OBJ EcLibPosix_setuid( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int setuid(uid_t uid) */

#if HAVE_SETUID
	EcInt  uid_i;
	uid_t  uid;
	int    rv;
	EC_OBJ res;

	res = EcParseStackFunction( "posix.setuid", TRUE, stack, "i",
								&uid_i );
	if (EC_ERRORP(res))
		return res;

	uid = (uid_t) uid_i;

	rv = setuid( uid );
	if (rv < 0)
		return posix2exception( errno, EC_NIL, "in posix.setuid" );
	else
		return EcTrueObject;
#else
	return EcUnimplementedError( "POSIX `setuid' function not available" );
#endif /* HAVE_SETUID */
}

static EC_OBJ EcLibPosix_setgid( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int setgid(gid_t gid) */

#if HAVE_SETGID
	EcInt  gid_i;
	gid_t  gid;
	int    rv;
	EC_OBJ res;

	res = EcParseStackFunction( "posix.setgid", TRUE, stack, "i",
								&gid_i );
	if (EC_ERRORP(res))
		return res;

	gid = (gid_t) gid_i;

	rv = setgid( gid );
	if (rv < 0)
		return posix2exception( errno, EC_NIL, "in posix.setgid" );
	else
		return EcTrueObject;
#else
	return EcUnimplementedError( "POSIX `setgid' function not available" );
#endif /* HAVE_SETGID */
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
	EcAddPrimitive( "posix.getcwd",     EcLibPosix_getcwd );
	EcAddPrimitive( "posix.mkdir",      EcLibPosix_mkdir );
	EcAddPrimitive( "posix.rmdir",      EcLibPosix_rmdir );
	EcAddPrimitive( "posix.chdir",      EcLibPosix_chdir );
	EcAddPrimitive( "posix.link",       EcLibPosix_link );
	EcAddPrimitive( "posix.unlink",     EcLibPosix_unlink );
	EcAddPrimitive( "posix.rename",     EcLibPosix_rename );
	EcAddPrimitive( "posix.chmod",      EcLibPosix_chmod );
	EcAddPrimitive( "posix.chown",      EcLibPosix_chown );

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
	EcAddPrimitive( "posix.execvp",     EcLibPosix_execvp );
	EcAddPrimitive( "posix.wait",       EcLibPosix_wait );
	EcAddPrimitive( "posix.waitpid",    EcLibPosix_waitpid );
	EcAddPrimitive( "posix._exit",      EcLibPosix__exit );
	EcAddPrimitive( "posix.kill",       EcLibPosix_kill );
	EcAddPrimitive( "posix.sleep",      EcLibPosix_sleep );
	EcAddPrimitive( "posix.pause",      EcLibPosix_pause );
	EcAddPrimitive( "posix.alarm",      EcLibPosix_alarm );
	EcAddPrimitive( "posix.setuid",     EcLibPosix_setuid );
	EcAddPrimitive( "posix.setgid",     EcLibPosix_setgid );

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

	s_WNOHANG   = EcInternSymbol( "WNOHANG" );
	s_WUNTRACED = EcInternSymbol( "WUNTRACED" );

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

	i = 0;
	sym2int_waitpid_options[i].symbolid = s_WNOHANG;
#ifdef WNOHANG
	sym2int_waitpid_options[i++].value  = WNOHANG;
#else
	sym2int_waitpid_options[i++].value  = 0;
#endif
	sym2int_waitpid_options[i].symbolid = s_WUNTRACED;
#ifdef WUNTRACED
	sym2int_waitpid_options[i++].value  = WUNTRACED;
#else
	sym2int_waitpid_options[i++].value  = 0;
#endif
	ASSERT( i == 2 );

#endif /* HAVE_UNISTD_H */

	/* Variables */

	feature = EcMakeHash();
	if (EC_ERRORP(feature)) return feature;

	EcHashSet( feature, EcMakeSymbol("getcwd"),
#if HAVE_UNISTD_H && HAVE_GETCWD
			   EcTrueObject );
#else
			   EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("mkdir"),
#if HAVE_UNISTD_H && HAVE_MKDIR
			   EcTrueObject );
#else
			   EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("rmdir"),
#if HAVE_UNISTD_H && HAVE_RMDIR
			   EcTrueObject );
#else
			   EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("chdir"),
#if HAVE_UNISTD_H && HAVE_CHDIR
			   EcTrueObject );
#else
			   EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("link"),
#if HAVE_UNISTD_H && HAVE_LINK
			   EcTrueObject );
#else
			   EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("unlink"),
#if HAVE_UNISTD_H && HAVE_UNLINK
			   EcTrueObject );
#else
			   EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("rename"),
#if HAVE_UNISTD_H && HAVE_RENAME
			   EcTrueObject );
#else
			   EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("chmod"),
#if HAVE_UNISTD_H && HAVE_CHMOD
			   EcTrueObject );
#else
			   EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("chown"),
#if HAVE_UNISTD_H && HAVE_CHOWN
			   EcTrueObject );
#else
			   EcFalseObject );
#endif
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
	EcHashSet( feature, EcMakeSymbol("execvp"),
#if HAVE_UNISTD_H && HAVE_EXECVP
		       EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("wait"),
#if HAVE_UNISTD_H && HAVE_WAIT
		       EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("waitpid"),
#if HAVE_UNISTD_H && HAVE_WAITPID
		       EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("_exit"),
#if HAVE_UNISTD_H && HAVE__EXIT
		       EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("kill"),
#if HAVE_UNISTD_H && HAVE_KILL
		       EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("sleep"),
#if HAVE_UNISTD_H && HAVE_SLEEP
		       EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("pause"),
#if HAVE_UNISTD_H && HAVE_PAUSE
		       EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("alarm"),
#if HAVE_UNISTD_H && HAVE_ALARM
		       EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("setuid"),
#if HAVE_UNISTD_H && HAVE_SETUID
		       EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("setgid"),
#if HAVE_UNISTD_H && HAVE_SETGID
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
