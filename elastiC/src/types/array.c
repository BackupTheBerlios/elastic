/* ==========================================================================
 * array.c
 * ==========================================================================
 * elastiC array.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat May 23 11:02:52 MEST 1998
 *
 *   $Id: array.c,v 1.1 2002/05/23 21:20:15 panta Exp $
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

#include "hashfunc.h"

EC_API EcInt tc_array = -1;

static EC_OBJ array_copy( EC_OBJ obj, EcCopyType type );
static void   array_mark( EC_OBJ obj );
static void   array_free( EC_OBJ obj );
static EcInt  array_print( ec_string *str, EC_OBJ obj, EcBool detailed );
static EC_OBJ array_getitem( EC_OBJ obj, EC_OBJ index );
static EC_OBJ array_setitem( EC_OBJ obj, EC_OBJ index, EC_OBJ value );
static EC_OBJ array_delitem( EC_OBJ obj, EC_OBJ index );
static EC_OBJ array_length( EC_OBJ obj );

static EcUInt array_hash( EC_OBJ obj, EcInt recur );
static EcBool array_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res );

static EC_OBJ array_add( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ array_true( EC_OBJ obj );

static EcBool array_check( EC_OBJ obj );

/* C API */

#define ARRAY_INITIAL_SIZE     16								/* this had better to be a power of 2 ! */
#define ARRAY_EXPANSION_FACTOR 2

EC_API EC_OBJ EcMakeArray( EcInt dim )
{
	EC_OBJ obj;
	EcInt  ndim;

	obj = EcMakeUser( tc_array, NULL );
	if (EC_ERRORP(obj)) return obj;

	ndim  = (dim + (EcInt)ARRAY_INITIAL_SIZE) / (EcInt)ARRAY_INITIAL_SIZE;
	ndim *= (EcInt)ARRAY_INITIAL_SIZE;

	if (ndim < dim)
		ndim = dim;

	EC_ARRAYMEM(obj)  = ec_malloc( ndim * sizeof(EC_OBJ) );
	EC_ARRAYLEN(obj)  = 0;
	EC_ARRAYREAL(obj) = EC_ARRAYMEM(obj) ? ARRAY_INITIAL_SIZE : 0;

	return obj;
}

EC_API EC_OBJ EcMakeArrayInit( EcInt nargs, ... )
{
	va_list ap;
	EC_OBJ obj;
	EcInt i;

	va_start( ap, nargs );

	obj = EcMakeArray( nargs );
	if (EC_ERRORP(obj)) return obj;
	for (i = 0; i < nargs; i++)
		EcArrayPush( obj, va_arg( ap, EC_OBJ ) );

	va_end( ap );
	return obj;
}

/* elastiC API */

/* Non-exponential resizing */
EC_API EcBool EcResizeArray( EC_OBJ array, EcInt newSize )
{
	EC_OBJ *newArray;
	EcInt newRealSize;
	EcInt oldUsed, oldReal, i;
	size_t dim;

	ASSERT( EC_ARRAYP(array) );
	ASSERT( newSize >= 0 );

	newRealSize  = (newSize + ARRAY_INITIAL_SIZE);
	newRealSize /= ARRAY_INITIAL_SIZE;
	newRealSize *= ARRAY_INITIAL_SIZE;

	ASSERT( newRealSize > 0 );

	if (newRealSize == EC_ARRAYREAL(array))
		return TRUE;

	dim = newRealSize * sizeof(EC_OBJ);
	if (dim <= (size_t)newRealSize)
		return FALSE;
	newArray = ec_realloc( EC_ARRAYMEM(array), dim );
	if (! newArray) return FALSE;

	oldUsed = EC_ARRAYLEN(array);
	oldReal = EC_ARRAYREAL(array);

	EC_ARRAYMEM(array)  = newArray;
	EC_ARRAYLEN(array)  = (oldUsed > newRealSize) ? newRealSize : oldUsed;
	EC_ARRAYREAL(array) = newRealSize;

	for (i = oldUsed; i < EC_ARRAYREAL(array); i++)
	{
	    EC_ARRAYMEM(array)[i] = EC_NIL;
	}

	return TRUE;
}

/* Exponential resizing */
EC_API EcBool EcResizeArrayFor( EC_OBJ array, EcInt newSize, EcBool onlyExpand )
{
	EC_OBJ *newArray;
	EcInt newRealSize;
	EcInt oldUsed, oldReal, i;

	if (onlyExpand)
	{
		if (newSize < EC_ARRAYLEN(array))
			newSize = EC_ARRAYLEN(array);
	}

	newRealSize = ARRAY_INITIAL_SIZE;
	while (newRealSize < newSize)
		newRealSize *= ARRAY_EXPANSION_FACTOR;

	if (newRealSize == EC_ARRAYREAL(array))
		return TRUE;

	newArray = ec_realloc( EC_ARRAYMEM(array), newRealSize * sizeof(EC_OBJ) );
	if (! newArray) return FALSE;

	oldUsed = EC_ARRAYLEN(array);
	oldReal = EC_ARRAYREAL(array);

	EC_ARRAYMEM(array) = newArray;
	EC_ARRAYLEN(array) = (oldUsed > newRealSize) ? newRealSize : oldUsed;
	EC_ARRAYREAL(array) = newRealSize;

	for (i = oldUsed; i < EC_ARRAYREAL(array); i++)
	{
	    EC_ARRAYMEM(array)[i] = EC_NIL;
	}

	return TRUE;
}

EC_API EC_OBJ EcArrayGet( EC_OBJ array, EcInt i )
{
	ASSERT( EC_ARRAYP(array) );

	if ((i < 0) || (i >= EC_ARRAYLEN(array)))
		return EcIndexError( array, EcMakeInt( i ) );

	ASSERT((i >= 0) && (i < EC_ARRAYLEN(array)));

	return EC_ARRAYMEM(array)[i];
}

EC_API EC_OBJ *EcArrayGetR( EC_OBJ array, EcInt i )
{
	ASSERT( EC_ARRAYP(array) );

	ASSERT((i >= 0) && (i < EC_ARRAYLEN(array)));

	if ((i < 0) || (i >= EC_ARRAYLEN(array)))
	{
		EcIndexError( array, EcMakeInt( i ) );
		return NULL;
	}
	return (&(EC_ARRAYMEM(array)[i]));
}

EC_API EC_OBJ EcArraySet( EC_OBJ array, EcInt i, EC_OBJ val )
{
	ASSERT( EC_ARRAYP(array) );
	if (i >= EC_ARRAYREAL(array))
	{
		if (! EcResizeArrayFor( array, i + 1, TRUE ))
			return EcMemoryError();

		EC_ARRAYLEN(array) = i + 1;
	}

	ASSERT((i >= 0) && (i < EC_ARRAYREAL(array)));

	if (i >= EC_ARRAYLEN(array))
	{
		EC_ARRAYLEN(array) = i + 1;
	}

	if ((i < 0) || (i >= EC_ARRAYLEN(array)))
		return EcIndexError( array, EcMakeInt( i ) );

	ASSERT((i >= 0) && (i < EC_ARRAYLEN(array)));
	EC_ARRAYMEM(array)[i] = val;
	return array;
}

EC_API EC_OBJ EcArrayDel( EC_OBJ array, EcInt i )
{
	EcInt len, j;

	ASSERT( EC_ARRAYP(array) );

	len = EC_ARRAYLEN(array);
	if ((i < 0) || (i >= len))
		return EcIndexError( array, EcMakeInt( i ) );

	for (j = i+1; j < len; j++)
		EC_ARRAYMEM(array)[j-1] = EC_ARRAYMEM(array)[j];
	EcResizeArrayFor( array, len - 1, FALSE );
	EC_ARRAYLEN(array) = len - 1;

	return array;
}

EC_API EC_OBJ EcArrayGetNE( EC_OBJ array, EcInt i )
{
	ASSERT( EC_ARRAYP(array) );

	ASSERT((i >= 0) && (i < EC_ARRAYLEN(array)));
	return EC_ARRAYMEM(array)[i];
}

EC_API EC_OBJ *EcArrayGetRNE( EC_OBJ array, EcInt i )
{
	ASSERT( EC_ARRAYP(array) );

	ASSERT((i >= 0) && (i < EC_ARRAYLEN(array)));
	return (&(EC_ARRAYMEM(array)[i]));
}

EC_API EC_OBJ EcArraySetNE( EC_OBJ array, EcInt i, EC_OBJ val )
{
	ASSERT( EC_ARRAYP(array) );

	ASSERT((i >= 0) && (i < EC_ARRAYLEN(array)));
	return (EC_ARRAYMEM(array)[i] = val);
}

EC_API EcInt EcArrayPush( EC_OBJ array, EC_OBJ obj )
{
	EcInt pos;

	pos = EC_ARRAYLEN(array);
	if (EC_ERRORP(EcArraySet( array, pos, obj )))
		return -1;
	return pos;
}

EC_API EC_OBJ EcArrayPop( EC_OBJ array )
{
	EC_OBJ res;

	if (EC_ARRAYLEN(array) <= 0)
	{
        ec_string ds;

        ec_string_init( &ds, NULL );
        ec_sprintf( &ds, "Can't pop from empty array" );
        EcMakeExceptionWithArgs( EcIndexErrorClass, ec_strdata( &ds ), 2,
								 EcMakeInt( -1 ), array );
        ec_string_cleanup( &ds );
        return Ec_ERROR;
	}

/*	res = EC_ARRAYMEM(array)[EC_ARRAYLEN(array) - 1];*/
	res = EC_ARRAYGET(array, EC_ARRAYLEN(array) - 1);

	EcResizeArrayFor( array, EC_ARRAYLEN(array) - 1, FALSE );
	EC_ARRAYLEN(array)--;

	return res;
}

/* Private */

static EC_OBJ array_copy( EC_OBJ obj, EcCopyType type )
{
	EC_OBJ res, copy;
	EcInt  dim, i;

	ASSERT( EC_ARRAYP(obj) );

	dim = EC_ARRAYLEN(obj);
	res = EcMakeArray( dim );
	if (EC_ERRORP(res)) return res;

	/* ASSERT(EC_ARRAYREAL(res) >= dim); */

	for (i = 0; i < dim; i++)
	{
		if (type == EcShallowCopyType)
			copy = EC_ARRAYMEM(obj)[i];
		else
		{
			copy = EcCopy( EC_ARRAYMEM(obj)[i], EcDeepCopyType );
			if (EC_ERRORP(copy)) return copy;
		}
		/* EC_ARRAYMEM(res)[i] = copy; */
		EcArraySet(res, i, copy);
	}
	/* EC_ARRAYLEN(res) = dim; */

	return res;
}

static void array_mark( EC_OBJ obj )
{
	EcInt i;

	for (i = 0; i < EC_ARRAYLEN(obj); i++)
	{
		if (EC_NNULLP(EC_ARRAYMEM(obj)[i])) EcMarkObject( EC_ARRAYMEM(obj)[i] );
	}
}

static void array_free( EC_OBJ obj )
{
	ec_free( EC_ARRAYMEM(obj) );
	EC_ARRAYMEM(obj)  = NULL;
	EC_ARRAYLEN(obj)  = 0;
	EC_ARRAYREAL(obj) = 0;
}

#define MAXARRAYWIDTH 60

static EcInt array_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	EcInt res = 0;

	EcInt  i;
	EC_OBJ el;
	EcInt  rep;

	res = ec_sprintf( str, "[" );
	for (i = 0; i < EC_ARRAYLEN(obj); i++)
	{
		if (i != 0) res += ec_sprintf( str, ", " );

/*		el = EC_ARRAYMEM(obj)[i];*/
		el = EC_ARRAYGET(obj, i);

		rep = 1;
		if (i + 1 < EC_ARRAYLEN(obj))
		{
			for (; ((i + rep) < EC_ARRAYLEN(obj)) &&
/*					 (el == EC_ARRAYMEM(obj)[i + rep]); rep++);*/
					 (el == EC_ARRAYGET(obj, i + rep)); rep++);
		}

		res += EcPrintObject( str, el, TRUE );
		if (rep >= 3)
		{
			res += ec_sprintf( str, " %ld times ", (long) rep );
			i += rep - 1;
		}

		if (res > MAXARRAYWIDTH)
		{
			res += ec_sprintf( str, " ... " );
			break;
		}
	}
	res += ec_sprintf( str, "]" );
	return res;
}

static EC_OBJ array_getitem( EC_OBJ obj, EC_OBJ index )
{
	EcInt i;

	ASSERT( EC_ARRAYP(obj) );

	if (! EC_INUMP(index))
		return EcTypeError( EC_NIL, EC_NIL, 2, tc_inum, index, TRUE, "array getitem" );

	i = EC_INUM(index);

	if ((i < 0) || (i >= EC_ARRAYLEN(obj)))
		return EcIndexError( obj, EcMakeInt( i ) );

	ASSERT((i >= 0) && (i < EC_ARRAYLEN(obj)));

	return EC_ARRAYMEM(obj)[i];
}

static EC_OBJ array_setitem( EC_OBJ obj, EC_OBJ index, EC_OBJ value )
{
	EcInt i;

	ASSERT( EC_ARRAYP(obj) );

	if (! EC_INUMP(index))
		return EcTypeError( EC_NIL, EC_NIL, 2, tc_inum, index, TRUE, "array setitem" );

	i = EC_INUM(index);

	if (i >= EC_ARRAYREAL(obj))
	{
		if (! EcResizeArrayFor( obj, i + 1, TRUE ))
			return EcMemoryError();

		EC_ARRAYLEN(obj) = i + 1;
	}

	ASSERT((i >= 0) && (i < EC_ARRAYREAL(obj)));

	if (i >= EC_ARRAYLEN(obj))
	{
		EC_ARRAYLEN(obj) = i + 1;
	}

	if ((i < 0) || (i >= EC_ARRAYLEN(obj)))
		return EcIndexError( obj, EcMakeInt( i ) );

	ASSERT((i >= 0) && (i < EC_ARRAYLEN(obj)));
	EC_ARRAYMEM(obj)[i] = value;
	return obj;
}

static EC_OBJ array_delitem( EC_OBJ obj, EC_OBJ index )
{
	EcInt len, i, j;

	ASSERT( EC_ARRAYP(obj) );

	if (! EC_INUMP(index))
		return EcTypeError( EC_NIL, EC_NIL, 2, tc_inum, index, TRUE, "array delitem" );

	i = EC_INUM(index);

	len = EC_ARRAYLEN(obj);
	if ((i < 0) || (i >= len))
		return EcIndexError( obj, EcMakeInt( i ) );

	for (j = i+1; j < len; j++)
		EC_ARRAYMEM(obj)[j-1] = EC_ARRAYMEM(obj)[j];
	EcResizeArrayFor( obj, len - 1, FALSE );
	EC_ARRAYLEN(obj) = len - 1;

	return obj;
}

static EC_OBJ array_length( EC_OBJ obj )
{
	return EcMakeInt( EC_ARRAYLEN(obj) );
}

static EcUInt array_hash( EC_OBJ obj, EcInt recur )
{
	EcInt res, i;

	res = 0;
	for (i = 0; i < EC_ARRAYLEN(obj); i++)
		res += _ec_hash( EC_ARRAYGET(obj, i), recur );
	return res;
}

static EcBool array_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res )
{
	EcInt i;

	if (obj1 == obj2)
	{
		*res = 0;
		return TRUE;
	}

	if (! EC_ARRAYP(obj2))
		return FALSE;

	*res = 0;

	if (EC_ARRAYLEN(obj1) != EC_ARRAYLEN(obj2))
		return FALSE;
	for (i = 0; i < EC_ARRAYLEN(obj1); i++)
	{
		if (! EcObjectEqual( EC_ARRAYGET(obj1, i), EC_ARRAYGET(obj2, i) ))
			return FALSE;
	}
	return TRUE;
}

static EC_OBJ array_add( EC_OBJ obj1, EC_OBJ obj2 )
{
	EcInt i;
	EC_OBJ res;

	if (! EC_ARRAYP(obj2))
		return EcTypeError( EC_NIL, EC_NIL, 2, tc_array, obj2, TRUE, "array add" );

	res = EcMakeArray( EC_ARRAYLEN(obj1) + EC_ARRAYLEN(obj2) );
	if (EC_ERRORP(res)) return res;

	for (i = 0; i < EC_ARRAYLEN(obj1); i++)
		EcArrayPush( res, EC_ARRAYGET(obj1, i) );
	for (i = 0; i < EC_ARRAYLEN(obj2); i++)
		EcArrayPush( res, EC_ARRAYGET(obj2, i) );
	return res;
}

static EC_OBJ array_true( EC_OBJ obj )
{
	return EcTrueObject;
}

static EcBool array_check( EC_OBJ obj )
{
	if (EC_ARRAYLEN(obj) < 0)
	{
		EcAlert( EcWarning, "INVALID ARRAY: NEGATIVE LENGTH (OBJECT 0x%08lX)",
				 (unsigned long)obj );
		return FALSE;
	}
	if (EC_ARRAYLEN(obj) > EC_ARRAYREAL(obj))
	{
		EcAlert( EcWarning, "INVALID ARRAY: LENGTH > REAL LENGTH (OBJECT 0x%08lX)",
				 (unsigned long)obj );
		return FALSE;
	}
	if ((! EC_ARRAYMEM(obj)) && (EC_ARRAYLEN(obj) > 0))
	{
		EcAlert( EcWarning, "INVALID ARRAY: NO MEMORY (OBJECT 0x%08lX)",
				 (unsigned long)obj );
		return FALSE;
	}
	return TRUE;
}

EcBool _ec_array_init( void )
{
	EcSequenceCallbacks sequence_cb = {
		/* getitem */		array_getitem,
		/* setitem */		array_setitem,
		/* delitem */		array_delitem,
		/* length  */		array_length
	};

	EcNumericCallbacks numeric_cb = {
		/* promote */		NULL,
		/* add     */		array_add,
		/* radd    */		NULL,
		/* sub     */		NULL,
		/* rsub    */		NULL,
		/* mul     */		NULL,
		/* rmul    */		NULL,
		/* div     */		NULL,
		/* rdiv    */		NULL,
		/* mod     */		NULL,
		/* rmod    */		NULL,
		/* pow     */		NULL,
		/* rpow    */		NULL,
		/* lshift  */		NULL,
		/* rlshift */		NULL,
		/* rshift  */		NULL,
		/* rrshift */		NULL,
		/* and     */		NULL,
		/* rand    */		NULL,
		/* or      */		NULL,
		/* ror     */		NULL,
		/* xor     */		NULL,
		/* rxor    */		NULL,
		/* neg     */		NULL,
		/* pos     */		NULL,
		/* abs     */		NULL,
		/* invert  */		NULL,
		/* int     */		NULL,
		/* float   */		NULL,
		/* true    */		array_true
	};

	EcTypespec arrayspec = {
		/* type     */		0,
		/* name     */		"array",

		/* copy     */		array_copy,

		/* mark     */		array_mark,
		/* free     */		array_free,
		/* print    */		array_print,

		/* hash     */		array_hash,
		/* compare  */		array_compare,

		/* check    */		array_check,

		/* sequence */      &sequence_cb,
		/* numeric  */      &numeric_cb
	};

	tc_array = EcAddType( &arrayspec );
	if (tc_array == 0)
		return FALSE;

	return TRUE;
}

void _ec_array_cleanup( void )
{
}
