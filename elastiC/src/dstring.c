/* ==========================================================================
 * dstring.c
 * ==========================================================================
 * Dynamic Strings.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sun May  3 10:26:04 MEST 1998
 *
 *   $Id: dstring.c,v 1.2 2002/05/25 19:37:44 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1997-2002 Marco Pantaleoni. All rights reserved.
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

#include "compat.h"
#include "dstring.h"
#include "debug.h"


#define QUANTUM 64

#define DSDATA(ds)		((ds)->strng)
#define DSLEN(ds)		((ds)->length)
#define DSAVAIL(ds)		((ds)->avail)
#define DSSTATIC(ds)	((ds)->static_space)

/* ===================================================================================================
 * I M P L E M E N T A T I O N
 * =================================================================================================== */

EC_API ec_string *ec_string_init( ec_string *ds, const char *init )
{
	ASSERT( ds );

	DSDATA(ds)  = DSSTATIC(ds);
	DSLEN(ds)   = 0;
	DSAVAIL(ds) = EC_DSTRING_STATIC_SIZE;
#if EC_PARANOID_CODE
	memset( DSSTATIC(ds), 0, EC_DSTRING_STATIC_SIZE );
#endif

	if (init)
	{
		if (! ec_strcat( ds, init, 0 ))
		{
			ec_string_cleanup( ds );
			return NULL;
		}
	}
	return ds;
}

EC_API ec_string *ec_string_prepare( ec_string *ds, EcInt size )
{
	EcInt  newlen;
	char  *newloc;

	ASSERT( ds );

	if (size < EC_DSTRING_STATIC_SIZE)
		return ec_string_init( ds, NULL );

	DSLEN(ds) = 0;

	newlen  = size + 1 + QUANTUM;
	newlen /= QUANTUM;
	newlen *= QUANTUM;

	newloc = ec_malloc( newlen );
	if (! newloc)
	{
		DSAVAIL(ds) = -1;
		return NULL;
	}
	DSDATA(ds)  = newloc;
	DSAVAIL(ds) = newlen;
#if EC_PARANOID_CODE
	memset( DSDATA(ds), 0, newlen );
#endif

	return ds;
}

EC_API ec_string *ec_string_cleanup( ec_string *ds )
{
	ASSERT( ds );

	if (DSDATA(ds) != DSSTATIC(ds))
		ec_free( DSDATA(ds) );
	DSDATA(ds)  = DSSTATIC(ds);
	DSLEN(ds)   = 0;
	DSAVAIL(ds) = EC_DSTRING_STATIC_SIZE;
#if EC_PARANOID_CODE
	memset( DSSTATIC(ds), 0, EC_DSTRING_STATIC_SIZE );
#endif

	return ds;
}

EC_API ec_string *ec_string_create( const char *init )
{
	ec_string *ds;

	ds = (ec_string *) ec_malloc( sizeof(ec_string) );
	if (! ds) return NULL;

	return ec_string_init( ds, init );
}

EC_API void ec_string_destroy( ec_string *ds )
{
	ASSERT( ds );

	ec_string_cleanup( ds );
	ec_free( ds );
}

EC_API char *ec_strcat( ec_string *ds, const char *string, EcInt length )
{
	EcInt len, newlen;
	char *newloc;

	ASSERT( ds );

	if (! string) return DSDATA(ds);
	len = (length <= 0) ? strlen( string ) : length;

	if (len+1 > DSAVAIL(ds))
	{
		newlen  = DSLEN(ds) + len + 1 + QUANTUM;
		newlen /= QUANTUM;
		newlen *= QUANTUM;

		if (DSDATA(ds) == DSSTATIC(ds))
		{
			newloc = (char *) ec_malloc( newlen );
			if (! newloc)
			{
				DSAVAIL(ds) = -1;
				return NULL;
			}
			DSDATA(ds) = newloc;
			memcpy( DSDATA(ds), DSSTATIC(ds), EC_DSTRING_STATIC_SIZE );
		} else
		{
			newloc = (char *) ec_realloc( DSDATA(ds), newlen );
			if (! newloc)
			{
				DSAVAIL(ds) = -1;
				return NULL;
			}
			DSDATA(ds) = newloc;
		}

		DSAVAIL(ds) = newlen - DSLEN(ds);
	}

	ASSERT(len+1 <= DSAVAIL(ds));

	memcpy( DSDATA(ds) + DSLEN(ds), string, len );
	DSLEN(ds) += len;
	*(DSDATA(ds) + DSLEN(ds)) = '\0';							/* Just in case strncpy copied exactly len chars */
	DSAVAIL(ds) -= len;

	return DSDATA(ds);
}

EC_API char *ec_strcatc( ec_string *ds, char c )
{
	char buf[2];

	buf[0] = c;
	buf[1] = '\0';

	return ec_strcat( ds, buf, 1 );
}

EC_API char *ec_strcatd( ec_string *ds, const ec_string *string )
{
	return ec_strcat( ds, ec_strdata( string ), ec_strlen( string ) );
}
