/* ==========================================================================
 * os.c
 * ==========================================================================
 * Operating System services access layer - UNIX version.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat Jan 22 15:56:53 MET 2000
 *
 *   $Id: os.c,v 1.1 2002/05/23 21:20:24 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 2000 Marco Pantaleoni. All rights reserved.
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
#include "private.h"

#include <elastic/os.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <time.h>

/*
 * Damn !
 * CLOCKS_PER_SEC and CLK_TCK are different on some machines,
 * even if CLK_TCK is supposed to be superseded by CLOCKS_PER_SEC !
 * When using clock() you have to use CLOCKS_PER_SEC
 * When using times() you have to use CLK_TCK.
 * But both return clock_t !
 *
 * We'll have to fix this.
 */
#ifdef CLK_TCK
#define CLOCK_TICKS_PER_SECOND ((long) CLK_TCK)
#else
#define CLOCK_TICKS_PER_SECOND ((long) 100)
#endif


	/* file */
EC_API ec_fmode ec_file_getmode( FILE *fh )
{
	int mode;
	int fd;

	fd = fileno( fh );
	mode = fcntl( fd, F_GETFL, 0 );
	switch (mode)
	{
	case O_RDONLY:
		return ec_fmode_rdonly;
		break;
	case O_WRONLY:
		return ec_fmode_wronly;
		break;
	case O_RDWR:
		return ec_fmode_rdwr;
		break;
	default:
		return ec_fmode_unknown;
		break;
	}
}

EC_API EcBool ec_file_ready( FILE *fh )
{
	int            fd;
	fd_set         fds;
	struct timeval timeout;

	/* WARNING: I'm not so sure about this. This routine mixes I/O stream and low-level I/O.
	            I tried to keep it as safe as possible, with flushing, but you never know... */

	if (! fh)
		return FALSE;

	if (feof( fh ))
		return FALSE;

#ifdef _GNU_SOURCE
	/*fclean( fh );*/												/* we don't have it ! */
#endif /* _GNU_SOURCE */

	fflush( fh );
	fd = fileno( fh );
	FD_ZERO( &fds );
	FD_SET( fd, &fds );
	timeout.tv_sec  = 0;
	timeout.tv_usec = 200;                                                                          /* 200 us timeout */

	if (select( FD_SETSIZE, &fds, NULL, NULL, &timeout ) < 1)
		return FALSE;
	return FD_ISSET( fd, &fds ) ? TRUE : FALSE;
}

	/* time */
EC_API EcFloat ec_get_utime( void )
{
	struct timeval tv;

	if (gettimeofday( &tv, NULL ) != 0)
		return -1.0;			/* ???? */

	return (EcFloat)tv.tv_sec * 1000000.0 + (EcFloat)tv.tv_usec;
}

EC_API void ec_get_times( ec_times *tb )
{
    long secs;
    long rem_ticks;
	long usecs;

	struct tms tbuf;
	clock_t    up;

	up = times( &tbuf );

#define TICKS2SECS(ticks)		((long)(ticks) / CLOCK_TICKS_PER_SECOND)
#define TICKS2MICROS(ticks)		((long)ticks * (1000000L / CLOCK_TICKS_PER_SECOND))
#define SECS2TICKS(secs)		((long)(secs) * CLOCK_TICKS_PER_SECOND)

	secs      = TICKS2SECS(up);
	rem_ticks = (long)up - SECS2TICKS(secs);
	usecs     = TICKS2MICROS(rem_ticks);
	tb->up_s = (EcInt) secs;
	tb->up_u = (EcInt) usecs;

	secs      = TICKS2SECS(tbuf.tms_utime);
	rem_ticks = (long)tbuf.tms_utime - SECS2TICKS(secs);
	usecs     = TICKS2MICROS(rem_ticks);
	tb->user_s = secs;
	tb->user_u = usecs;

	secs      = TICKS2SECS(tbuf.tms_stime);
	rem_ticks = (long)tbuf.tms_stime - SECS2TICKS(secs);
	usecs     = TICKS2MICROS(rem_ticks);
	tb->system_s = secs;
	tb->system_u = usecs;
}

	/* environment */
EC_API char *ec_getenv( const char *name )
{
	return getenv( name );
}

EC_API EcBool ec_setenv( const char *name, const char *value, EcBool overwrite )
{
	return setenv( name, value, (int) overwrite ) == 0 ? TRUE : FALSE;
}

EC_API void ec_unsetenv( const char *name )
{
	unsetenv( name );
}
