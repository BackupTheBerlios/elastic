/* ==========================================================================
 * stream_t.c
 * ==========================================================================
 * elastiC language stream type.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Wed Jun 12 10:52:30 CEST 2002
 *
 *   $Id: stream_t.c,v 1.1 2002/06/12 16:00:06 panta Exp $
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

#include <stdio.h>
#include <stdlib.h>

#include "basic.h"
#include "elastic.h"
#include "private.h"
#include "compat.h"
#include "debug.h"

EC_API EcInt tc_stream = -1;

static EC_OBJ stream_copy( EC_OBJ obj, EcCopyType type );
static void   stream_mark( EC_OBJ obj );
static void   stream_free( EC_OBJ obj );
static EcInt  stream_print( ec_string *str, EC_OBJ obj, EcBool detailed );
static EcUInt stream_hash( EC_OBJ obj, EcInt recur );

/* C API */

EC_API EC_OBJ EcMakeStream( ec_stream *stream )
{
	EC_OBJ res;

	res = EcMakeUser( tc_stream, stream );
	if (EC_ERRORP(res)) return res;
	return res;
}

EC_API EC_OBJ EcLibStreamClose( EC_OBJ obj )
{
	ec_stream *str;
	EcInt      rv;

	if (! EC_STREAMP(obj))
	{
		/* TODO: Throw an exception */
		return EcUnimplementedError( "unimplemented exception" );
	}

	str = EC_STREAM_STR(obj);
	if (! str)
	{
		/* TODO: Throw an exception */
		return EcUnimplementedError( "unimplemented exception" );
	}

	ec_stream_exception_clear( str );
	rv = ec_stream_close( str );
	if (EC_ERRORP(str->exc))
		return str->exc;

	return EcMakeInt( rv );
}

/* Private */

static EC_OBJ stream_copy( EC_OBJ obj, EcCopyType type )
{
	ASSERT( EC_STREAMP(obj) );

	return EcMakeStream( EC_STREAM_STR(obj) );
}

static void stream_mark( EC_OBJ obj )
{
	ASSERT( EC_STREAMP(obj) );
	ec_stream_mark( EC_STREAM_STR(obj) );
}

static void stream_free( EC_OBJ obj )
{
	ASSERT( EC_STREAMP(obj) );

	if (EC_STREAM_STR(obj))
	{
		EC_OBJ exc;

		ec_stream_destroy( EC_STREAM_STR(obj), &exc );
		/* :TODO: do something with the exception! */
	}
}

static EcInt stream_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	ASSERT( EC_STREAMP(obj) );

	return ec_stream_print( EC_STREAM_STR(obj), str, detailed );
}

static EcUInt stream_hash( EC_OBJ obj, EcInt recur )
{
	ASSERT( EC_STREAMP(obj) );

	return ec_stream_hash( EC_STREAM_STR(obj), recur );
}

EcBool _ec_stream_t_init( void )
{
	EcTypespec streamspec = {
		/* type    */		0,
		/* name    */		"stream",

		/* copy    */		stream_copy,

		/* mark    */		stream_mark,
		/* free    */		stream_free,
		/* print   */		stream_print,

		/* hash    */		stream_hash,
		/* compare */		NULL,

		/* check   */		NULL,

		/* sequence */      NULL,
		/* numeric  */      NULL
	};

	tc_stream = EcAddType( &streamspec );
	if (tc_stream == 0)
		return FALSE;

	return TRUE;
}

void _ec_stream_t_cleanup( void )
{
}
