/* ==========================================================================
 * posix_m.c
 * ==========================================================================
 * POSIX module.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Wed Jun  5 21:48:10 CEST 2002
 *
 *   $Id: posix_m.c,v 1.7 2002/06/07 16:09:56 panta Exp $
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

/*
 * MISSING:
 *
 *   utime
 *   opendir / readdir / closedir / rewinddir
 *
 *   fcntl
 *   fdopen
 *   fileno
 *
 *   [signal handling functions]
 *
 *   [process properties functions]
 *   [process groups and job control]
 *
 *   mknod
 *   truncate
 *   mount
 *
 *   system
 */

#if HAVE_UNISTD_H
	/* this modules relies on <unistd.h> */

static EcUInt s_R_OK = 0, s_W_OK = 0, s_X_OK = 0, s_F_OK = 0;
static EcUInt s_O_RDONLY = 0, s_O_WRONLY = 0, s_O_RDWR = 0,
	s_O_CREAT = 0, s_O_EXCL = 0, s_O_NOCTTY = 0, s_O_TRUNC = 0,
	s_O_APPEND = 0, s_O_NONBLOCK = 0, s_O_NDELAY = 0,
	s_O_SYNC = 0, s_O_DSYNC = 0, s_O_RSYNC = 0,
	s_O_NOFOLLOW = 0, s_O_DIRECTORY = 0, s_O_LARGEFILE = 0;
static EcUInt
	s_S_IFMT   = 0,
	s_S_IFSOCK = 0,
	s_S_IFLNK  = 0,
	s_S_IFREG  = 0,
	s_S_IFBLK  = 0,
	s_S_IFDIR  = 0,
	s_S_IFCHR  = 0,
	s_S_IFIFO  = 0,
	s_S_ISUID  = 0,
	s_S_ISGID  = 0,
	s_S_ISVTX  = 0,
	s_S_IRWXU = 0,
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
static EcUInt s_SEEK_SET = 0, s_SEEK_CUR = 0, s_SEEK_END = 0;
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

static _ec_symbol2int sym2int_mode_t[] =
{
	{ 0, 0 },													/* S_IFMT   */
	{ 0, 0 },													/* S_IFSOCK */
    { 0, 0 },													/* S_IFLNK  */
    { 0, 0 },													/* S_IFREG  */
    { 0, 0 },													/* S_IFBLK  */
    { 0, 0 },													/* S_IFDIR  */
    { 0, 0 },													/* S_IFCHR  */
    { 0, 0 },													/* S_IFIFO  */
    { 0, 0 },													/* S_ISUID  */
    { 0, 0 },													/* S_ISGID  */
    { 0, 0 },													/* S_ISVTX  */
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

static _ec_symbol2int sym2int_seek_whence[] =
{
	{ 0, 0 },													/* SEEK_SET */
	{ 0, 0 },													/* SEEK_CUR */
	{ 0, 0 },													/* SEEK_END */
	{ 0, 0 }
};

static _ec_symbol2int sym2int_waitpid_options[] =
{
	{ 0, 0 },													/* WNOHANG   */
	{ 0, 0 },													/* WUNTRACED */
	{ 0, 0 }
};

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
		return _ec_errno2exception( errno, EC_NIL, "in posix.getcwd" );
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
			res = _ec_sequence2mask( "posix.mkdir", 2, sym2int_open_mode, TRUE, TRUE, mode_o, &mode_i );
			if (EC_ERRORP(res)) return res;
		}
	} else
		mode_i = 0777;											/* rwxrwxrwx */

	mode = (mode_t) mode_i;

	rv = mkdir( pathname, mode );
	if (rv < 0)
		return _ec_errno2exception( errno, EC_NIL, "in posix.mkdir" );
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
		return _ec_errno2exception( errno, EC_NIL, "in posix.rmdir" );
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
		return _ec_errno2exception( errno, EC_NIL, "in posix.chdir" );
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
		return _ec_errno2exception( errno, EC_NIL, "in posix.link" );
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
		return _ec_errno2exception( errno, EC_NIL, "in posix.unlink" );
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
		return _ec_errno2exception( errno, EC_NIL, "in posix.rename" );
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

	if (EC_INUMP(mode_o))
		mode_i = EC_INUM(mode_o);
	else
	{
		res = _ec_sequence2mask( "posix.chmod", 2, sym2int_open_mode, FALSE, TRUE, mode_o, &mode_i );
		if (EC_ERRORP(res)) return res;
	}

	mode = (mode_t) mode_i;

	rv = chmod( path, mode );
	if (rv < 0)
		return _ec_errno2exception( errno, EC_NIL, "in posix.chmod" );
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
		return _ec_errno2exception( errno, EC_NIL, "in posix.chown" );
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

	res = _ec_sequence2mask( "posix.access", 2, sym2int_access_mode, TRUE, TRUE, mode_o, &mode );
	if (EC_ERRORP(res)) return res;

	rv = access( pathname, (int) mode );
	if (rv < 0)
		return _ec_errno2exception( errno, EC_NIL, "in posix.access" );
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

	flags_o = EcMakeSymbol("O_RDONLY");
	if (EC_ERRORP(flags_o)) return flags_o;

	res = EcParseStackFunction( "posix.open", TRUE, stack, "s|OO",
								&pathname, &flags_o, &mode_o );
	if (EC_ERRORP(res))
		return res;

	res = _ec_sequence2mask( "posix.open", 2, sym2int_open_flags, FALSE, TRUE, flags_o, &flags_i );
	if (EC_ERRORP(res)) return res;

	if (EC_NNULLP(mode_o))
	{
		if (EC_INUMP(mode_o))
			mode_i = EC_INUM(mode_o);
		else
		{
			res = _ec_sequence2mask( "posix.open", 3, sym2int_open_mode, TRUE, TRUE, mode_o, &mode_i );
			if (EC_ERRORP(res)) return res;
		}
	} else
		mode_i = 0777;											/* rwxrwxrwx (autom. masked out by current umask) */

	flags = (int)    flags_i;
	mode  = (mode_t) mode_i;

	if (flags & O_CREAT)
		rv = open( pathname, flags, mode );
	else
		rv = open( pathname, flags );
	if (rv < 0)
		return _ec_errno2exception( errno, EC_NIL, "in posix.open" );
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
			res = _ec_sequence2mask( "posix.creat", 2, sym2int_open_mode, TRUE, TRUE, mode_o, &mode_i );
			if (EC_ERRORP(res)) return res;
		}
	} else
		mode_i = 0777;											/* rwxrwxrwx (autom. masked out by current umask) */

	mode = (mode_t) mode_i;

	rv = creat( pathname, mode );
	if (rv < 0)
		return _ec_errno2exception( errno, EC_NIL, "in posix.creat" );
	else
		return EcMakeInt( rv );
#elif HAVE_OPEN
	rv = open( pathname, O_CREAT, mode );
	if (rv < 0)
		return _ec_errno2exception( errno, EC_NIL, "in posix.creat" );
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
		return _ec_errno2exception( errno, EC_NIL, "in posix.close" );
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
		return _ec_errno2exception( errno, EC_NIL, "in posix.read" );
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
		return _ec_errno2exception( errno, EC_NIL, "in posix.write" );
	else
		return EcMakeInt( rv );
#else
	return EcUnimplementedError( "POSIX `write' function not available" );
#endif /* HAVE_WRITE */
}

static EC_OBJ stat2hash( const char *funcname, struct stat *buf )
{
	EC_OBJ stat_h;
	EC_OBJ ftype, mode_seq;

	stat_h = EcMakeHash();
	if (EC_ERRORP(stat_h)) return stat_h;

	EcHashSet( stat_h, EcMakeSymbol("dev"),     EcMakeInt(buf->st_dev) );
	EcHashSet( stat_h, EcMakeSymbol("ino"),     EcMakeInt(buf->st_ino) );
	EcHashSet( stat_h, EcMakeSymbol("mode"),    EcMakeInt(buf->st_mode) );

	ftype = EC_NIL;
	if (S_ISLNK(buf->st_mode))
		ftype = EcMakeSymbolFromId(s_S_IFLNK);
	else if (S_ISREG(buf->st_mode))
		ftype = EcMakeSymbolFromId(s_S_IFREG);
	else if (S_ISDIR(buf->st_mode))
		ftype = EcMakeSymbolFromId(s_S_IFDIR);
	else if (S_ISCHR(buf->st_mode))
		ftype = EcMakeSymbolFromId(s_S_IFCHR);
	else if (S_ISBLK(buf->st_mode))
		ftype = EcMakeSymbolFromId(s_S_IFBLK);
	else if (S_ISFIFO(buf->st_mode))
		ftype = EcMakeSymbolFromId(s_S_IFIFO);
	else if (S_ISSOCK(buf->st_mode))
		ftype = EcMakeSymbolFromId(s_S_IFSOCK);
	EcHashSet( stat_h, EcMakeSymbol("filetype"), ftype );

	mode_seq = _ec_mask2sequence( funcname, sym2int_mode_t, buf->st_mode & 07777 );
	if (EC_ERRORP(mode_seq)) return mode_seq;
	EcHashSet( stat_h, EcMakeSymbol("fileperm"), mode_seq );

	EcHashSet( stat_h, EcMakeSymbol("nlink"),   EcMakeInt(buf->st_nlink) );
	EcHashSet( stat_h, EcMakeSymbol("uid"),     EcMakeInt(buf->st_uid) );
	EcHashSet( stat_h, EcMakeSymbol("gid"),     EcMakeInt(buf->st_gid) );
	EcHashSet( stat_h, EcMakeSymbol("rdev"),    EcMakeInt(buf->st_rdev) );
	EcHashSet( stat_h, EcMakeSymbol("size"),    EcMakeInt(buf->st_size) );
	EcHashSet( stat_h, EcMakeSymbol("blksize"), EcMakeInt(buf->st_blksize) ); /* :TODO: not portable: need #ifdef */
	EcHashSet( stat_h, EcMakeSymbol("blocks"),  EcMakeInt(buf->st_blocks) );  /* :TODO: not portable: need #ifdef */
	EcHashSet( stat_h, EcMakeSymbol("atime"),   EcMakeInt(buf->st_atime) ); /* :TODO: convert? */
	EcHashSet( stat_h, EcMakeSymbol("mtime"),   EcMakeInt(buf->st_ctime) ); /* :TODO: convert? */
	EcHashSet( stat_h, EcMakeSymbol("ctime"),   EcMakeInt(buf->st_mtime) ); /* :TODO: convert? */

	return stat_h;
}

static EC_OBJ EcLibPosix_stat( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int stat(const char *file_name, struct stat *buf) */

#if HAVE_STAT
	const char *file_name;
	struct stat buf;
	int         rv;
	EC_OBJ      res;

	res = EcParseStackFunction( "posix.stat", TRUE, stack, "s",
								&file_name );
	if (EC_ERRORP(res))
		return res;

	memset( &buf, 0x00, sizeof(struct stat) );
	rv = stat( file_name, &buf );
	if (rv < 0)
		return _ec_errno2exception( errno, EC_NIL, "in posix.stat" );

	return stat2hash( "posix.stat", &buf );
#else
	return EcUnimplementedError( "POSIX `stat' function not available" );
#endif /* HAVE_STAT */
}

static EC_OBJ EcLibPosix_fstat( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int fstat(int filedes, struct stat *buf) */

#if HAVE_FSTAT
	EcInt       filedes_i;
	int         filedes;
	struct stat buf;
	int         rv;
	EC_OBJ      res;

	res = EcParseStackFunction( "posix.fstat", TRUE, stack, "i",
								&filedes_i );
	if (EC_ERRORP(res))
		return res;

	filedes = (int) filedes_i;

	memset( &buf, 0x00, sizeof(struct stat) );
	rv = fstat( filedes, &buf );
	if (rv < 0)
		return _ec_errno2exception( errno, EC_NIL, "in posix.fstat" );

	return stat2hash( "posix.fstat", &buf );
#else
	return EcUnimplementedError( "POSIX `fstat' function not available" );
#endif /* HAVE_FSTAT */
}

static EC_OBJ EcLibPosix_lstat( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int lstat(const char *file_name, struct stat *buf) */

#if HAVE_LSTAT
	const char *file_name;
	struct stat buf;
	int         rv;
	EC_OBJ      res;

	res = EcParseStackFunction( "posix.lstat", TRUE, stack, "s",
								&file_name );
	if (EC_ERRORP(res))
		return res;

	memset( &buf, 0x00, sizeof(struct stat) );
	rv = lstat( file_name, &buf );
	if (rv < 0)
		return _ec_errno2exception( errno, EC_NIL, "in posix.lstat" );

	return stat2hash( "posix.lstat", &buf );
#else
	return EcUnimplementedError( "POSIX `lstat' function not available" );
#endif /* HAVE_LSTAT */
}

static EC_OBJ EcLibPosix_lseek( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: off_t lseek(int fildes, off_t offset, int whence) */

#if HAVE_LSEEK
	EcInt  fildes_i, offset_i;
	EC_OBJ whence_o = EC_NIL;
	int    fildes;
	off_t  offset;
	int    whence;
	off_t  rv;
	EC_OBJ res;

	res = EcParseStackFunction( "posix.lseek", TRUE, stack, "ii|O",
								&fildes_i, &offset_i, &whence_o );
	if (EC_ERRORP(res))
		return res;

	if (EC_NNULLP(whence_o))
	{
		res = _ec_symbol2mask( "posix.lseek", 2, sym2int_seek_whence, TRUE, whence_o, &whence );
		if (EC_ERRORP(res)) return res;
	} else
		whence = SEEK_SET;

	fildes = (int)   fildes_i;
	offset = (off_t) offset_i;

	rv = lseek( fildes, offset, whence );
	if (rv == (off_t)-1)
		return _ec_errno2exception( errno, EC_NIL, "in posix.lseek" );
	else
		return EcMakeInt( rv );
#else
	return EcUnimplementedError( "POSIX `lseek' function not available" );
#endif /* HAVE_LSEEK */
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
		return _ec_errno2exception( errno, EC_NIL, "in posix.dup" );
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
		return _ec_errno2exception( errno, EC_NIL, "in posix.dup2" );
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
		return _ec_errno2exception( errno, EC_NIL, "in posix.pipe" );
	else
		return EcMakeArrayInit( 2, EcMakeInt( filedes[0] ), EcMakeInt( filedes[1] ) );
#else
	return EcUnimplementedError( "POSIX `pipe' function not available" );
#endif /* HAVE_PIPE */
}

static EC_OBJ EcLibPosix_mkfifo( EC_OBJ stack, EcAny userdata )
{
	/* POSIX function: int mkfifo(const char *pathname, mode_t mode) */

#if HAVE_MKFIFO
	char   *pathname;
	EC_OBJ  mode_o = EC_NIL;
	EcInt   mode_i = 0;
	mode_t  mode   = 0;
	int     rv;
	EC_OBJ  res;

	res = EcParseStackFunction( "posix.mkfifo", TRUE, stack, "s|O",
								&pathname, &mode_o );
	if (EC_ERRORP(res))
		return res;

	if (EC_NNULLP(mode_o))
	{
		if (EC_INUMP(mode_o))
			mode_i = EC_INUM(mode_o);
		else
		{
			res = _ec_sequence2mask( "posix.mkfifo", 2, sym2int_open_mode, TRUE, TRUE, mode_o, &mode_i );
			if (EC_ERRORP(res)) return res;
		}
	} else
		mode_i = 0666;											/* rw-rw-rw */

	mode = (mode_t) mode_i;

	rv = mkfifo( pathname, mode );
	if (rv < 0)
		return _ec_errno2exception( errno, EC_NIL, "in posix.mkfifo" );
	else
		return EcTrueObject;
#else
	return EcUnimplementedError( "POSIX `mkfifo' function not available" );
#endif /* HAVE_MKFIFO */
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
		return _ec_errno2exception( errno, EC_NIL, "in posix.fork" );
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
	return _ec_errno2exception( errno, EC_NIL, "in posix.execv" );
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
	return _ec_errno2exception( errno, EC_NIL, "in posix.execve" );
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
	return _ec_errno2exception( errno, EC_NIL, "in posix.execvp" );
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
		return _ec_errno2exception( errno, EC_NIL, "in posix.wait" );

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

	res = _ec_sequence2mask( "posix.waitpid", 2, sym2int_waitpid_options, TRUE, TRUE, options_o, &options );
	if (EC_ERRORP(res)) return res;

	rv = waitpid( pid, &status, options );
	if (rv <= 0)
		return _ec_errno2exception( errno, EC_NIL, "in posix.waitpid" );

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
	return _ec_errno2exception( errno, EC_NIL, "in posix._exit" );
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
		return _ec_errno2exception( errno, EC_NIL, "in posix.kill" );
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
	return _ec_errno2exception( errno, EC_NIL, "in posix.fork" );
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
		return _ec_errno2exception( errno, EC_NIL, "in posix.setuid" );
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
		return _ec_errno2exception( errno, EC_NIL, "in posix.setgid" );
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
	EcAddPrimitive( "posix.stat",       EcLibPosix_stat );
	EcAddPrimitive( "posix.fstat",      EcLibPosix_fstat );
	EcAddPrimitive( "posix.lstat",      EcLibPosix_lstat );
	EcAddPrimitive( "posix.lseek",      EcLibPosix_lseek );
	EcAddPrimitive( "posix.dup",        EcLibPosix_dup );
	EcAddPrimitive( "posix.dup2",       EcLibPosix_dup2 );
	EcAddPrimitive( "posix.pipe",       EcLibPosix_pipe );
	EcAddPrimitive( "posix.mkfifo",     EcLibPosix_mkfifo );
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

	s_S_IFMT    = EcInternSymbol( "S_IFMT" );
	s_S_IFSOCK  = EcInternSymbol( "S_IFSOCK" );
	s_S_IFLNK   = EcInternSymbol( "S_IFLNK" );
	s_S_IFREG   = EcInternSymbol( "S_IFREG" );
	s_S_IFBLK   = EcInternSymbol( "S_IFBLK" );
	s_S_IFDIR   = EcInternSymbol( "S_IFDIR" );
	s_S_IFCHR   = EcInternSymbol( "S_IFCHR" );
	s_S_IFIFO   = EcInternSymbol( "S_IFIFO" );
	s_S_ISUID   = EcInternSymbol( "S_ISUID" );
	s_S_ISGID   = EcInternSymbol( "S_ISGID" );
	s_S_ISVTX   = EcInternSymbol( "S_ISVTX" );
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
	while (sym2int_access_mode[i].symbolid > 0)
	{
		EcPackageVariable( pkg, EcSymbolAt( sym2int_access_mode[i].symbolid ),
						   EcMakeInt( sym2int_access_mode[i].value ),
						   TRUE, TRUE );
		i++;
	}

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
	while (sym2int_open_flags[i].symbolid > 0)
	{
		EcPackageVariable( pkg, EcSymbolAt( sym2int_open_flags[i].symbolid ),
						   EcMakeInt( sym2int_open_flags[i].value ),
						   TRUE, TRUE );
		i++;
	}

	i = 0;
	sym2int_open_mode[i].symbolid   = s_S_IRWXU;
#ifdef S_IRWXU
	sym2int_open_mode[i++].value    = S_IRWXU;
#else
	sym2int_open_mode[i++].value    = 00700;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IRUSR;
#ifdef S_IRUSR
	sym2int_open_mode[i++].value    = S_IRUSR;
#else
	sym2int_open_mode[i++].value    = 00400;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IREAD;
#ifdef S_IREAD
	sym2int_open_mode[i++].value    = S_IREAD;
#else
	sym2int_open_mode[i++].value    = 00400;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IWUSR;
#ifdef S_IWUSR
	sym2int_open_mode[i++].value    = S_IWUSR;
#else
	sym2int_open_mode[i++].value    = 00200;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IWRITE;
#ifdef S_IWRITE
	sym2int_open_mode[i++].value    = S_IWRITE;
#else
	sym2int_open_mode[i++].value    = 00200;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IXUSR;
#ifdef S_IXUSR
	sym2int_open_mode[i++].value    = S_IXUSR;
#else
	sym2int_open_mode[i++].value    = 00100;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IEXEC;
#ifdef S_IEXEC
	sym2int_open_mode[i++].value    = S_IEXEC;
#else
	sym2int_open_mode[i++].value    = 00100;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IRWXG;
#ifdef S_IRWXG
	sym2int_open_mode[i++].value    = S_IRWXG;
#else
	sym2int_open_mode[i++].value    = 00070;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IRGRP;
#ifdef S_IRGRP
	sym2int_open_mode[i++].value    = S_IRGRP;
#else
	sym2int_open_mode[i++].value    = 00040;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IWGRP;
#ifdef S_IWGRP
	sym2int_open_mode[i++].value    = S_IWGRP;
#else
	sym2int_open_mode[i++].value    = 00020;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IXGRP;
#ifdef S_IXGRP
	sym2int_open_mode[i++].value    = S_IXGRP;
#else
	sym2int_open_mode[i++].value    = 00010;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IRWXO;
#ifdef S_IRWXO
	sym2int_open_mode[i++].value    = S_IRWXO;
#else
	sym2int_open_mode[i++].value    = 00007;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IROTH;
#ifdef S_IROTH
	sym2int_open_mode[i++].value    = S_IROTH;
#else
	sym2int_open_mode[i++].value    = 00004;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IWOTH;
#ifdef S_IWOTH
	sym2int_open_mode[i++].value    = S_IWOTH;
#else
	sym2int_open_mode[i++].value    = 00002;
#endif
	sym2int_open_mode[i].symbolid   = s_S_IXOTH;
#ifdef S_IXOTH
	sym2int_open_mode[i++].value    = S_IXOTH;
#else
	sym2int_open_mode[i++].value    = 00001;
#endif
	ASSERT( i == 15 );

	i = 0;
	sym2int_mode_t[i].symbolid   = s_S_IFMT;
#ifdef S_IFMT
	sym2int_mode_t[i++].value    = S_IFMT;
#else
	sym2int_mode_t[i++].value    = 0170000;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IFSOCK;
#ifdef S_IFSOCK
	sym2int_mode_t[i++].value    = S_IFSOCK;
#else
	sym2int_mode_t[i++].value    = 0140000;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IFLNK;
#ifdef S_IFLNK
	sym2int_mode_t[i++].value    = S_IFLNK;
#else
	sym2int_mode_t[i++].value    = 0120000;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IFREG;
#ifdef S_IFREG
	sym2int_mode_t[i++].value    = S_IFREG;
#else
	sym2int_mode_t[i++].value    = 0100000;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IFBLK;
#ifdef S_IFBLK
	sym2int_mode_t[i++].value    = S_IFBLK;
#else
	sym2int_mode_t[i++].value    = 0060000;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IFDIR;
#ifdef S_IFDIR
	sym2int_mode_t[i++].value    = S_IFDIR;
#else
	sym2int_mode_t[i++].value    = 0040000;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IFCHR;
#ifdef S_IFCHR
	sym2int_mode_t[i++].value    = S_IFCHR;
#else
	sym2int_mode_t[i++].value    = 0020000;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IFIFO;
#ifdef S_IFIFO
	sym2int_mode_t[i++].value    = S_IFIFO;
#else
	sym2int_mode_t[i++].value    = 0010000;
#endif
	sym2int_mode_t[i].symbolid   = s_S_ISUID;
#ifdef S_ISUID
	sym2int_mode_t[i++].value    = S_ISUID;
#else
	sym2int_mode_t[i++].value    = 0004000;
#endif
	sym2int_mode_t[i].symbolid   = s_S_ISGID;
#ifdef S_ISGID
	sym2int_mode_t[i++].value    = S_ISGID;
#else
	sym2int_mode_t[i++].value    = 0002000;
#endif
	sym2int_mode_t[i].symbolid   = s_S_ISVTX;
#ifdef S_ISVTX
	sym2int_mode_t[i++].value    = S_ISVTX;
#else
	sym2int_mode_t[i++].value    = 0001000;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IRWXU;
#ifdef S_IRWXU
	sym2int_mode_t[i++].value    = S_IRWXU;
#else
	sym2int_mode_t[i++].value    = 00700;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IRUSR;
#ifdef S_IRUSR
	sym2int_mode_t[i++].value    = S_IRUSR;
#else
	sym2int_mode_t[i++].value    = 00400;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IREAD;
#ifdef S_IREAD
	sym2int_mode_t[i++].value    = S_IREAD;
#else
	sym2int_mode_t[i++].value    = 00400;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IWUSR;
#ifdef S_IWUSR
	sym2int_mode_t[i++].value    = S_IWUSR;
#else
	sym2int_mode_t[i++].value    = 00200;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IWRITE;
#ifdef S_IWRITE
	sym2int_mode_t[i++].value    = S_IWRITE;
#else
	sym2int_mode_t[i++].value    = 00200;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IXUSR;
#ifdef S_IXUSR
	sym2int_mode_t[i++].value    = S_IXUSR;
#else
	sym2int_mode_t[i++].value    = 00100;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IEXEC;
#ifdef S_IEXEC
	sym2int_mode_t[i++].value    = S_IEXEC;
#else
	sym2int_mode_t[i++].value    = 00100;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IRWXG;
#ifdef S_IRWXG
	sym2int_mode_t[i++].value    = S_IRWXG;
#else
	sym2int_mode_t[i++].value    = 00070;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IRGRP;
#ifdef S_IRGRP
	sym2int_mode_t[i++].value    = S_IRGRP;
#else
	sym2int_mode_t[i++].value    = 00040;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IWGRP;
#ifdef S_IWGRP
	sym2int_mode_t[i++].value    = S_IWGRP;
#else
	sym2int_mode_t[i++].value    = 00020;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IXGRP;
#ifdef S_IXGRP
	sym2int_mode_t[i++].value    = S_IXGRP;
#else
	sym2int_mode_t[i++].value    = 00010;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IRWXO;
#ifdef S_IRWXO
	sym2int_mode_t[i++].value    = S_IRWXO;
#else
	sym2int_mode_t[i++].value    = 00007;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IROTH;
#ifdef S_IROTH
	sym2int_mode_t[i++].value    = S_IROTH;
#else
	sym2int_mode_t[i++].value    = 00004;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IWOTH;
#ifdef S_IWOTH
	sym2int_mode_t[i++].value    = S_IWOTH;
#else
	sym2int_mode_t[i++].value    = 00002;
#endif
	sym2int_mode_t[i].symbolid   = s_S_IXOTH;
#ifdef S_IXOTH
	sym2int_mode_t[i++].value    = S_IXOTH;
#else
	sym2int_mode_t[i++].value    = 00001;
#endif
	ASSERT( i == 26 );
	i = 0;
	while (sym2int_mode_t[i].symbolid > 0)
	{
		EcPackageVariable( pkg, EcSymbolAt( sym2int_mode_t[i].symbolid ),
						   EcMakeInt( sym2int_mode_t[i].value ),
						   TRUE, TRUE );
		i++;
	}

	i = 0;
	sym2int_seek_whence[i].symbolid = s_SEEK_SET;
#ifdef SEEK_SET
	sym2int_seek_whence[i++].value  = SEEK_SET;
#else
	sym2int_seek_whence[i++].value  = 0;
#endif
	sym2int_seek_whence[i].symbolid = s_SEEK_CUR;
#ifdef SEEK_CUR
	sym2int_seek_whence[i++].value  = SEEK_CUR;
#else
	sym2int_seek_whence[i++].value  = 1;
#endif
	sym2int_seek_whence[i].symbolid = s_SEEK_END;
#ifdef SEEK_END
	sym2int_seek_whence[i++].value  = SEEK_END;
#else
	sym2int_seek_whence[i++].value  = 2;
#endif
	ASSERT( i == 3 );
	i = 0;
	while (sym2int_seek_whence[i].symbolid > 0)
	{
		EcPackageVariable( pkg, EcSymbolAt( sym2int_seek_whence[i].symbolid ),
						   EcMakeInt( sym2int_seek_whence[i].value ),
						   TRUE, TRUE );
		i++;
	}

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
	i = 0;
	while (sym2int_waitpid_options[i].symbolid > 0)
	{
		EcPackageVariable( pkg, EcSymbolAt( sym2int_waitpid_options[i].symbolid ),
						   EcMakeInt( sym2int_waitpid_options[i].value ),
						   TRUE, TRUE );
		i++;
	}

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
	EcHashSet( feature, EcMakeSymbol("stat"),
#if HAVE_UNISTD_H && HAVE_STAT
		       EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("fstat"),
#if HAVE_UNISTD_H && HAVE_FSTAT
		       EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("lstat"),
#if HAVE_UNISTD_H && HAVE_LSTAT
		       EcTrueObject );
#else
		       EcFalseObject );
#endif
	EcHashSet( feature, EcMakeSymbol("lseek"),
#if HAVE_UNISTD_H && HAVE_LSEEK
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
	EcHashSet( feature, EcMakeSymbol("mkfifo"),
#if HAVE_UNISTD_H && HAVE_MKFIFO
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

	EcPackageVariable( pkg, "implements",
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
