/* ==========================================================================
 * os.c
 * ==========================================================================
 * Operating System services access layer - WIN32 version.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat Jan 22 15:56:53 MET 2000
 *
 *   $Id: os.c,v 1.2 2002/05/25 19:37:46 panta Exp $
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
#include "compat.h"

#include <elastic/os.h>

#include <stdio.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/timeb.h>

	/* file */
EC_API ec_fmode ec_file_getmode( FILE *fh )
{
	/* This is all we can do... (I suppose) */
	return ec_fmode_rdonly;
}

EC_API EcBool ec_file_ready( FILE *fh )
{
	/* This is all we can do... (I suppose) */

	if (! fh)
		return FALSE;

	if (feof( fh ))
		return FALSE;

	return TRUE;
}

	/* time */
EC_API EcFloat ec_get_utime( void )
{
	struct _timeb tb;

	_ftime( &tb );

	return (EcFloat)tb.millitm * 1000.0 + (EcFloat)tb.time;
}

EC_API void ec_get_times( ec_times *tb )
{
	/* TODO */

	tb->up_s = 0;
	tb->up_u = 0;

	tb->user_s = 0;
	tb->user_u = 0;

	tb->system_s = 0;
	tb->system_u = 0;
}

	/* environment */
EC_API char *ec_getenv( const char *name )
{
	return getenv( name );
}

EC_API EcBool ec_setenv( const char *name, const char *value, EcBool overwrite )
{
	char *envstring;
	size_t l;

	l = strlen( name ) + strlen( value ) + 1;
	envstring = alloca( l + 1 );
	if (! envstring)
		return FALSE;

	strcpy( envstring, name );
	strcat( envstring, "=" );
	strcat( envstring, value );
	_putenv( envstring );
	return TRUE;
}

EC_API void ec_unsetenv( const char *name )
{
	/* nothing */
}
