/* ==========================================================================
 * object.c
 * ==========================================================================
 * Instance.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat May 23 11:02:52 MEST 1998
 *
 *   $Id: object.c,v 1.2 2002/06/13 17:44:18 panta Exp $
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


EC_API EcInt tc_object = -1;

static EC_OBJ object_copy( EC_OBJ obj, EcCopyType type );

static void   object_mark( EC_OBJ obj );
static void   object_free( EC_OBJ obj );
static EcInt  object_print( ec_string *str, EC_OBJ obj, EcBool detailed );

static EC_OBJ object_getitem( EC_OBJ obj, EC_OBJ index );
static EC_OBJ object_setitem( EC_OBJ obj, EC_OBJ index, EC_OBJ value );
static EC_OBJ object_delitem( EC_OBJ obj, EC_OBJ index );
static EC_OBJ object_length( EC_OBJ obj );

static EcUInt object_hash( EC_OBJ obj, EcInt recur );
static EcBool object_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res );

static EC_OBJ object_promote( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_add( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_radd( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_sub( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_rsub( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_mul( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_rmul( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_div( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_rdiv( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_mod( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_rmod( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_pow( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_rpow( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_lshift( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_rlshift( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_rshift( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_rrshift( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_and( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_rand( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_or( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_ror( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_xor( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_rxor( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ object_neg( EC_OBJ obj );
static EC_OBJ object_pos( EC_OBJ obj );
static EC_OBJ object_invert( EC_OBJ obj );
static EC_OBJ object_int( EC_OBJ obj );
static EC_OBJ object_float( EC_OBJ obj );
static EC_OBJ object_true( EC_OBJ obj );

static EcBool object_check( EC_OBJ obj );

/* C API */

EC_API EC_OBJ EcMakeObject( EC_OBJ ofclass, void *user )
{
	EC_OBJ obj;
	EcInt nvars;

	obj = EcMakeUser( tc_object, NULL );
	if (EC_ERRORP(obj)) return obj;

	EC_OBJECT(obj) = ec_malloc( sizeof(EcObject) );
	if (! EC_OBJECT(obj)) return Ec_ERROR;

	EC_OBJECTCLASS(obj) = ofclass;
	nvars = EC_CLASSNIVARS(ofclass);

#if 0
	nvars = 0;
	inclass = ofclass;
	ec_msg_printf("of class %w\n", ofclass);
	while (EC_CLASSP(inclass)) {
		ec_msg_printf("  in class %w: ivars = %I\n", inclass, EC_CLASSNIVARS(inclass));
		nvars += EC_CLASSNIVARS(inclass);
		inclass = EC_CLASSSUPER(inclass);
	}
#endif

	if (nvars)
	{
		EcInt   i;
		EC_OBJ *objp;

		EC_OBJECTIVARS(obj) = ec_malloc( nvars * sizeof(EC_OBJ) );
		if (! EC_OBJECTIVARS(obj))
		{
			/* TODO XXX: how to handle this situation ? */
			/* EcAlert( EcFatal, "out of memory while building object" ); */
			return Ec_ERROR;
		}
		objp = EC_OBJECTIVARS(obj);
		for (i = 0; i < nvars; i++)
			*objp++ = EcUndefinedObject;
	}
	else
		EC_OBJECTIVARS(obj) = NULL;
	EC_OBJECTUSER(obj) = user;

	return obj;
}

/* elastiC API */


/* Private */

static EC_OBJ object_copy( EC_OBJ obj, EcCopyType type )
{
	EC_OBJ res, res2;
	EC_OBJ ofclass;

	ASSERT( EC_OBJECTP(obj) );
	ofclass = EC_OBJECTCLASS(obj);

	/* Is there a user-defined copy method ? If so, call it */
	if (EcDoesUnderstand( ofclass, PRIVATE(copyID) ))
	{
		res = EcMakeObject( ofclass, NULL );
		if (EC_ERRORP(res)) return res;

		res2 = EcSendMessageVA( res, PRIVATE(copyID), 1, obj );
		if (EC_ERRORP(res2)) return res2;
		return res;
	}

	res = EcMakeObject( ofclass, NULL );
	if (EC_ERRORP(res)) return res;

	if (type == EcShallowCopyType)
	{
		/* Shallow copy */

		EcInt i;
		for (i = 0; i < EC_CLASSNIVARS(ofclass); i++)
			EC_OBJECTIVARS(res)[i] = EC_OBJECTIVARS(obj)[i];
		EC_OBJECTUSER(res) = EC_OBJECTUSER(obj);
	} else
	{
		/* Deep copy */

		EC_OBJ copy;
		EcInt i;

		for (i = 0; i < EC_CLASSNIVARS(ofclass); i++)
		{
			copy = EcCopy( EC_OBJECTIVARS(obj)[i], EcDeepCopyType );
			if (EC_ERRORP(copy)) return copy;
			EC_OBJECTIVARS(res)[i] = copy;
		}

		/* This is hard to (deeply) copy ! */
		/* TODO: Work on this ! */
		EC_OBJECTUSER(res) = EC_OBJECTUSER(obj);
	}

	return res;
}

static void object_mark( EC_OBJ obj )
{
	EcInt  i;

	ASSERT( EC_OBJECTP(obj) );

	if (EC_NNULLP(EC_OBJECTCLASS(obj))) EcMarkObject( EC_OBJECTCLASS(obj) );
	for (i = 0; i < EC_CLASSNIVARS(EC_OBJECTCLASS(obj)); i++)
		if (EC_NNULLP(EC_OBJECTIVARS(obj)[i])) EcMarkObject( EC_OBJECTIVARS(obj)[i] );

	/* Is there a user-defined mark method ? If so, call it */
	/* TODO: call user-defined mark method */
	if (EcDoesUnderstand( obj, PRIVATE(gcmarkID) ))
	{
		EcSendMessage( obj, PRIVATE(gcmarkID), EC_NIL );
	}
}

static void object_free( EC_OBJ obj )
{
	ASSERT( EC_OBJECTP(obj) );

	/* Is there a user-defined free method ? If so, call it */
	/* TODO: call user-defined free method */
	if (EcDoesUnderstand( obj, PRIVATE(gcfreeID) ))
	{
		EcSendMessage( obj, PRIVATE(gcfreeID), EC_NIL );
	}

	EC_OBJECTCLASS(obj) = EC_NIL;
	ec_free( EC_OBJECTIVARS(obj) );
	EC_OBJECTIVARS(obj) = NULL;
	EC_OBJECTUSER(obj)  = NULL;
}

static EcInt object_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	EcInt res = 0;

	ASSERT( EC_OBJECTP(obj) );

	/* Is there a user-defined print method ? If so, call it */
	if (EcDoesUnderstand( obj, PRIVATE(printID) ))
	{
		EC_OBJ r;

		r = EcSendMessageVA( obj, PRIVATE(printID), 1,
							 detailed ? EcTrueObject : EcFalseObject );
		if (EC_ERRORP(r))
		{
			/* TODO: Throw an exception */
		}
		if (EC_STRINGP(r))
		{
			res  = ec_sprintf( str, "%s", EC_STRDATA(r) );
			return res;
		} else
		{
			/* TODO: Throw an exception */
		}
	}

	res  = ec_sprintf( str, "<instance of %w>",
					   EC_CLASSNAME(EC_OBJECTCLASS(obj)) );
	return res;
}

static EC_OBJ object_getitem( EC_OBJ obj, EC_OBJ index )
{
	return EcSendMessageVA( obj, PRIVATE(getitemID), 1, index );
}

static EC_OBJ object_setitem( EC_OBJ obj, EC_OBJ index, EC_OBJ value )
{
	return EcSendMessageVA( obj, PRIVATE(setitemID), 2, index, value );
}

static EC_OBJ object_delitem( EC_OBJ obj, EC_OBJ index )
{
	return EcSendMessageVA( obj, PRIVATE(delitemID), 1, index );
}

static EC_OBJ object_length( EC_OBJ obj )
{
	return EcSendMessageVA( obj, PRIVATE(lengthID), 0 );
}

static EcUInt object_hash( EC_OBJ obj, EcInt recur )
{
	EcUInt res;
	EcInt  i;
	EC_OBJ objclass;
	EC_OBJ r;

	ASSERT( EC_OBJECTP(obj) );

	res = 0;
	if (! EC_OBJECT(obj))
		return res;

	/* Is there a user-defined hash method ? If so, call it */
	if (EcDoesUnderstand( obj, PRIVATE(hashID) ))
	{
		r = EcSendMessageVA( obj, PRIVATE(hashID), 0 );
		if (EC_ERRORP(r))
		{
			/* TODO: Throw an exception */
		}
		if (EC_INUMP(r))
		{
			return (EC_INUM(r));
		} else
		{
			/* TODO: Throw an exception */
		}
	}

	res += _ec_hash( EC_OBJECTCLASS(obj), recur );
	objclass = EC_OBJECTCLASS(obj);
	for (i = 0; i < EC_CLASSNIVARS(objclass); i++)
		res += _ec_hash( EC_OBJECTIVARS(obj)[i], recur );
	return res;
}

static EcBool object_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res )
{
	EcInt  i;
	EC_OBJ objclass;
	EC_OBJ rs;

	if (obj1 == obj2)
	{
		*res = 0;
		return TRUE;
	}

	/* Is there a user-defined comparison method ? If so, call it */
	if (EcDoesUnderstand( obj1, PRIVATE(compareID) ))
	{
		rs = EcSendMessageVA( obj1, PRIVATE(compareID), 1, obj2 );
		if (EC_ERRORP(rs))
		{
			/* XXX TODO: Huh ? How can we return exception ? */
			return FALSE;
		}
		if (EC_BOOLP(rs))
		{
			if (EC_BOOLVAL(rs))
			{
				*res = 0;										/* identical objects */
				return TRUE;
			} else
				return FALSE;									/* objects not comparable */
		}
		if (EC_INUMP(rs))
		{
			*res = EC_INUM(rs);
			return TRUE;
		} else
		{
			/* TODO: Throw an exception */
			return FALSE;
		}
		return FALSE;
	}

	/* No comparison method. Do our best */

	if (! EC_OBJECTP(obj2))
		return FALSE;

	*res = 0;
	if (((EC_OBJECT(obj1) && (! EC_OBJECT(obj2))) ||
		 ((! EC_OBJECT(obj1)) && EC_OBJECT(obj2))))
		return FALSE;
	if (! EcObjectEqual( EC_OBJECTCLASS(obj1), EC_OBJECTCLASS(obj2) ))
		return FALSE;
	objclass = EC_OBJECTCLASS(obj1);
	for (i = 0; i < EC_CLASSNIVARS(objclass); i++)
		if (! EcObjectEqual( EC_OBJECTIVARS(obj1)[i], EC_OBJECTIVARS(obj2)[i] ))
			return FALSE;
	if (EC_OBJECTUSER(obj1) != EC_OBJECTUSER(obj2))
		return FALSE;
	return TRUE;
}

static EC_OBJ object_promote( EC_OBJ obj1, EC_OBJ obj2 )
{
	ASSERT( EC_OBJECTP(obj1) );

	/*
	 * XXXX TODO: Check this thing
	 *
	 * Here we break the rules ! We check for existance of
	 * promotion method, without directly calling it !!!!
	 * This imply not calling doesNotUnderstand for the method !!
	 */
	/* Is there a user-defined promote method ? If so, call it */
	if (EcDoesUnderstand( obj1, PRIVATE(promoteID) ))
	{
		return EcSendMessageVA( obj1, PRIVATE(promoteID), 1, obj2 );
	}

	/* say we don't have a promotion method */
	return EcFalseObject;
}

static EC_OBJ object_add( EC_OBJ obj1, EC_OBJ obj2 )
{
	EC_OBJ res;
	ASSERT( EC_OBJECTP(obj1) );

	res = EcSendMessageVA( obj1, PRIVATE(addID), 1, obj2 );
	return res;
}

static EC_OBJ object_radd( EC_OBJ obj1, EC_OBJ obj2 )
{
	ASSERT( EC_OBJECTP(obj1) );

	return EcSendMessageVA( obj1, PRIVATE(raddID), 1, obj2 );
}

static EC_OBJ object_sub( EC_OBJ obj1, EC_OBJ obj2 )
{
	return EcSendMessageVA( obj1, PRIVATE(subID), 1, obj2 );
}

static EC_OBJ object_rsub( EC_OBJ obj1, EC_OBJ obj2 )
{
	return EcSendMessageVA( obj1, PRIVATE(rsubID), 1, obj2 );
}

static EC_OBJ object_mul( EC_OBJ obj1, EC_OBJ obj2 )
{
	return EcSendMessageVA( obj1, PRIVATE(mulID), 1, obj2 );
}

static EC_OBJ object_rmul( EC_OBJ obj1, EC_OBJ obj2 )
{
	return EcSendMessageVA( obj1, PRIVATE(rmulID), 1, obj2 );
}

static EC_OBJ object_div( EC_OBJ obj1, EC_OBJ obj2 )
{
	return EcSendMessageVA( obj1, PRIVATE(divID), 1, obj2 );
}

static EC_OBJ object_rdiv( EC_OBJ obj1, EC_OBJ obj2 )
{
	return EcSendMessageVA( obj1, PRIVATE(rdivID), 1, obj2 );
}

static EC_OBJ object_mod( EC_OBJ obj1, EC_OBJ obj2 )
{
	return EcSendMessageVA( obj1, PRIVATE(modID), 1, obj2 );
}

static EC_OBJ object_rmod( EC_OBJ obj1, EC_OBJ obj2 )
{
	return EcSendMessageVA( obj1, PRIVATE(rmodID), 1, obj2 );
}

static EC_OBJ object_pow( EC_OBJ obj1, EC_OBJ obj2 )
{
	return EcSendMessageVA( obj1, PRIVATE(powID), 1, obj2 );
}

static EC_OBJ object_rpow( EC_OBJ obj1, EC_OBJ obj2 )
{
	return EcSendMessageVA( obj1, PRIVATE(rpowID), 1, obj2 );
}

static EC_OBJ object_lshift( EC_OBJ obj1, EC_OBJ obj2 )
{
	return EcSendMessageVA( obj1, PRIVATE(lshiftID), 1, obj2 );
}

static EC_OBJ object_rlshift( EC_OBJ obj1, EC_OBJ obj2 )
{
	return EcSendMessageVA( obj1, PRIVATE(rlshiftID), 1, obj2 );
}

static EC_OBJ object_rshift( EC_OBJ obj1, EC_OBJ obj2 )
{
	return EcSendMessageVA( obj1, PRIVATE(rshiftID), 1, obj2 );
}

static EC_OBJ object_rrshift( EC_OBJ obj1, EC_OBJ obj2 )
{
	return EcSendMessageVA( obj1, PRIVATE(rrshiftID), 1, obj2 );
}

static EC_OBJ object_and( EC_OBJ obj1, EC_OBJ obj2 )
{
	return EcSendMessageVA( obj1, PRIVATE(andID), 1, obj2 );
}

static EC_OBJ object_rand( EC_OBJ obj1, EC_OBJ obj2 )
{
	return EcSendMessageVA( obj1, PRIVATE(randID), 1, obj2 );
}

static EC_OBJ object_or( EC_OBJ obj1, EC_OBJ obj2 )
{
	return EcSendMessageVA( obj1, PRIVATE(orID), 1, obj2 );
}

static EC_OBJ object_ror( EC_OBJ obj1, EC_OBJ obj2 )
{
	return EcSendMessageVA( obj1, PRIVATE(rorID), 1, obj2 );
}

static EC_OBJ object_xor( EC_OBJ obj1, EC_OBJ obj2 )
{
	return EcSendMessageVA( obj1, PRIVATE(xorID), 1, obj2 );
}

static EC_OBJ object_rxor( EC_OBJ obj1, EC_OBJ obj2 )
{
	return EcSendMessageVA( obj1, PRIVATE(rxorID), 1, obj2 );
}

static EC_OBJ object_neg( EC_OBJ obj )
{
	return EcSendMessageVA( obj, PRIVATE(negID), 1, obj );
}

static EC_OBJ object_pos( EC_OBJ obj )
{
	return EcSendMessageVA( obj, PRIVATE(posID), 1, obj );
}

static EC_OBJ object_abs( EC_OBJ obj )
{
	return EcSendMessageVA( obj, PRIVATE(absID), 1, obj );
}

static EC_OBJ object_invert( EC_OBJ obj )
{
	return EcSendMessageVA( obj, PRIVATE(invertID), 1, obj );
}

static EC_OBJ object_int( EC_OBJ obj )
{
	return EcSendMessageVA( obj, PRIVATE(intID), 1, obj );
}

static EC_OBJ object_float( EC_OBJ obj )
{
	return EcSendMessageVA( obj, PRIVATE(floatID), 1, obj );
}

static EC_OBJ object_true( EC_OBJ obj )
{
	if (EcDoesUnderstand( obj, PRIVATE(trueID) ))
		return EcSendMessageVA( obj, PRIVATE(trueID), 1, obj );
	else
		return EcTrueObject;
}


static EcBool object_check( EC_OBJ obj )
{
/*
	ASSERTMEM( EC_OBJECT(obj) );
	ASSERTFIELD( EC_OBJECTCLASS(obj), tc_class );
*/
	return TRUE;
}

EcBool _ec_object_init( void )
{
	EcSequenceCallbacks sequence_cb = {
		/* getitem */		object_getitem,
		/* setitem */		object_setitem,
		/* delitem */		object_delitem,
		/* length  */		object_length
	};

	EcNumericCallbacks numeric_cb = {
		/* promote */		object_promote,
		/* add     */		object_add,
		/* radd    */		object_radd,
		/* sub     */		object_sub,
		/* rsub    */		object_rsub,
		/* mul     */		object_mul,
		/* rmul    */		object_rmul,
		/* div     */		object_div,
		/* rdiv    */		object_rdiv,
		/* mod     */		object_mod,
		/* rmod    */		object_rmod,
		/* pow     */		object_pow,
		/* rpow    */		object_rpow,
		/* lshift  */		object_lshift,
		/* rlshift */		object_rlshift,
		/* rshift  */		object_rshift,
		/* rrshift */		object_rrshift,
		/* and     */		object_and,
		/* rand    */		object_rand,
		/* or      */		object_or,
		/* ror     */		object_ror,
		/* xor     */		object_xor,
		/* rxor    */		object_rxor,
		/* neg     */		object_neg,
		/* pos     */		object_pos,
		/* abs     */		object_abs,
		/* invert  */		object_invert,
		/* int     */		object_int,
		/* float   */		object_float,
		/* true    */		object_true
	};

	EcTypespec objectspec = {
		/* type    */		0,
		/* name    */		"instance",

		/* copy    */		object_copy,

		/* mark    */		object_mark,
		/* free    */		object_free,
		/* print   */		object_print,

		/* hash    */		object_hash,
		/* compare */		object_compare,


		/* check   */		object_check,

		/* sequence */      &sequence_cb,
		/* numeric  */      &numeric_cb
	};

	tc_object = EcAddType( &objectspec );
	if (tc_object == 0)
		return FALSE;

	return TRUE;
}

void _ec_object_cleanup( void )
{
}
