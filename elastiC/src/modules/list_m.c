/* ==========================================================================
 * list_m.c
 * ==========================================================================
 * elastiC list module.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Mon Aug 16 15:04:02 MEST 1999
 *
 *   $Id: list_m.c,v 1.1 2002/05/23 21:18:39 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1999-2000 Marco Pantaleoni. All rights reserved.
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
#include "basic.h"
#include "debug.h"

static EC_OBJ EcLibList_Push( EC_OBJ stack, EcAny userdata );
static EC_OBJ EcLibList_Pop( EC_OBJ stack, EcAny userdata );
static EC_OBJ EcLibList_Cons( EC_OBJ stack, EcAny userdata );
static EC_OBJ EcLibList_Car( EC_OBJ stack, EcAny userdata );
static EC_OBJ EcLibList_Cdr( EC_OBJ stack, EcAny userdata );
static EC_OBJ EcLibList_List( EC_OBJ stack, EcAny userdata );

/* C API */


/* elastiC API */

static EC_OBJ EcLibList_Push( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ list, obj;
	EC_OBJ res;

	res = EcParseStackFunction( "list.push", TRUE, stack, "O!O", tc_array, &list, &obj );
	if (EC_ERRORP(res))
		return res;

	if (EcArrayPush( list, obj ) < 0)
		return Ec_ERROR;										/* pass along error from EcArrayPush */

	return list;
}

static EC_OBJ EcLibList_Pop( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ list, obj;
	EC_OBJ res;

	res = EcParseStackFunction( "list.pop", TRUE, stack, "O!", tc_array, &list );
	if (EC_ERRORP(res))
		return res;

	obj = EcArrayPop( list );
	return obj;
}

static EC_OBJ EcLibList_Cons( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj1, obj2;
	EC_OBJ res;

	res = EcParseStackFunction( "list.cons", TRUE, stack, "OO", &obj1, &obj2 );
	if (EC_ERRORP(res))
		return res;

	res = EcMakeArray( 2 );
	if (EC_ERRORP(res)) return res;
	EcArraySet( res, 0, obj1 );
	EcArraySet( res, 1, obj2 );

	return res;
}

static EC_OBJ EcLibList_Car( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ list, obj;
	EC_OBJ res;

	res = EcParseStackFunction( "list.car", TRUE, stack, "O!", tc_array, &list );
	if (EC_ERRORP(res))
		return res;

	obj = EcArrayGet( list, 0 );
	return obj;
}

static EC_OBJ EcLibList_Cdr( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ list, obj;
	EC_OBJ res;

	res = EcParseStackFunction( "list.cdr", TRUE, stack, "O!", tc_array, &list );
	if (EC_ERRORP(res))
		return res;

	obj = EcArrayGet( list, 1 );
	return obj;
}

static EC_OBJ EcLibList_List( EC_OBJ stack, EcAny userdata )
{
	EcInt  nargs, i;
	EC_OBJ list, car, cons;

	nargs = EC_STACKNARGS_REAL(stack);

	list = EC_NIL;
	car  = EC_NIL;
	for (i = 0; i < nargs; i++)
	{
		cons = EcMakeArray( 2 );
		if (EC_NULLP(cons) || EC_ERRORP(cons))
			return Ec_ERROR;	

		EcArraySet( cons, 0, EC_STACKPOP(stack) );
		EcArraySet( cons, 1, EC_NIL );

		if (EC_NULLP(car))
			list = cons;

		if (EC_NNULLP(car))
			EcArraySet( car, 1, cons );
		car = cons;
	}

	return list;
}

/* Private */

EcBool _ec_list_init( void )
{
	EC_OBJ pkg;

	pkg = EcPackageIntroduce( "list" );
	EcAddPrimitive( "list.push",	EcLibList_Push );
	EcAddPrimitive( "list.pop",		EcLibList_Pop );
	EcAddPrimitive( "list.cons",	EcLibList_Cons );
	EcAddPrimitive( "list.car",		EcLibList_Car );
	EcAddPrimitive( "list.cdr",		EcLibList_Cdr );
	EcAddPrimitive( "list.list",	EcLibList_List );

	return TRUE;
}

void _ec_list_cleanup( void )
{
}
