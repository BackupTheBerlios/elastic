/* ==========================================================================
 * handler.c
 * ==========================================================================
 * Exception Handler.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat May 23 11:02:52 MEST 1998
 *
 *   $Id: handler.c,v 1.1 2002/05/23 21:20:19 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1998-2000 Marco Pantaleoni. All rights reserved.
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

#include "elastic.h"
#include "private.h"
#include "debug.h"


EC_API EcInt tc_handler = -1;

static EC_OBJ handler_copy( EC_OBJ obj, EcCopyType type );

static void   handler_mark( EC_OBJ obj );
static void   handler_free( EC_OBJ obj );
static EcInt  handler_print( ec_string *str, EC_OBJ obj, EcBool detailed );

static EcUInt handler_hash( EC_OBJ obj, EcInt recur );
static EcBool handler_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res );

static EcBool handler_check( EC_OBJ obj );

/* C API */

EC_API EC_OBJ EcMakeHandler( EC_OBJ type, EC_OBJ code )
{
	EC_OBJ obj;

	obj = EcMakeUser( tc_handler, NULL );
	if (EC_ERRORP(obj)) return obj;

	EC_HANDLERTYPE(obj) = type;
	EC_HANDLERCODE(obj) = code;

	return obj;
}

/* elastiC API */


/* Private */

static EC_OBJ handler_copy( EC_OBJ obj, EcCopyType type )
{
	EC_OBJ res;

	ASSERT( EC_HANDLERP(obj) );

	res = EcMakeHandler( EC_HANDLERTYPE(obj), EC_HANDLERCODE(obj) );
	if (EC_ERRORP(res)) return res;

	if (type == EcDeepCopyType)
	{
		EC_OBJ copy;

		copy = EcCopy( EC_HANDLERTYPE(obj), EcDeepCopyType);
		if (EC_ERRORP(copy)) return copy;
		EC_HANDLERTYPE(obj) = copy;

		copy = EcCopy( EC_HANDLERCODE(obj), EcDeepCopyType);
		if (EC_ERRORP(copy)) return copy;
		EC_HANDLERCODE(obj) = copy;
	}

	return res;
}

static void handler_mark( EC_OBJ obj )
{
	if (EC_NNULLP(EC_HANDLERTYPE(obj))) EcMarkObject( EC_HANDLERTYPE(obj) );
	if (EC_NNULLP(EC_HANDLERCODE(obj))) EcMarkObject( EC_HANDLERCODE(obj) );
}

static void handler_free( EC_OBJ obj )
{
	ASSERT( EC_HANDLERP(obj) );

	EC_HANDLERTYPE(obj) = EC_NIL;
	EC_HANDLERCODE(obj) = EC_NIL;
}

static EcInt handler_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	EcInt res = 0;
	ASSERT( EC_HANDLERP(obj) );

	res = ec_sprintf( str, "<handler>" );
	return res;
}

static EcUInt handler_hash( EC_OBJ obj, EcInt recur )
{
	EcUInt res;

	ASSERT( EC_HANDLERP(obj) );

	res = 0;
	res += _ec_hash( EC_HANDLERTYPE(obj), recur );
	res += _ec_hash( EC_HANDLERCODE(obj), recur );
	return res;
}

static EcBool handler_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res )
{
	if (obj1 == obj2)
	{
		*res = 0;
		return TRUE;
	}

	if (! EC_HANDLERP(obj2))
		return FALSE;

	*res = 0;
	if (! EcObjectEqual( EC_HANDLERTYPE(obj1), EC_HANDLERTYPE(obj2) ))
		return FALSE;
	if (! EcObjectEqual( EC_HANDLERCODE(obj1), EC_HANDLERCODE(obj2) ))
		return FALSE;
	return TRUE;
}

static EcBool handler_check( EC_OBJ obj )
{
/*	ASSERTFIELDN( EC_HANDLERTYPE(obj), tc_class );
	ASSERTFIELDN( EC_HANDLERCODE(obj), tc_compiled );
*/
	return TRUE;
}

EcBool _ec_handler_init( void )
{
	EcTypespec handlerspec = {
		/* type    */		0,
		/* name    */		"handler",

		/* copy    */		handler_copy,

		/* mark    */		handler_mark,
		/* free    */		handler_free,
		/* print   */		handler_print,

		/* hash    */		handler_hash,
		/* compare */		handler_compare,

		/* check   */		handler_check,

		/* sequence */      NULL,
		/* numeric  */      NULL
	};

	tc_handler = EcAddType( &handlerspec );
	if (tc_handler == 0)
		return FALSE;

	return TRUE;
}

void _ec_handler_cleanup( void )
{
}
