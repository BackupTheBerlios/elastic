/* ==========================================================================
 * string.c
 * ==========================================================================
 * String basic type.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat May 23 11:02:52 MEST 1998
 *
 *   $Id: string.c,v 1.2 2002/05/25 19:37:45 panta Exp $
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "elastic.h"
#include "private.h"
#include "compat.h"
#include "debug.h"

#include "hashfunc.h"


EC_API EcInt tc_string = -1;

static EC_OBJ string_copy( EC_OBJ obj, EcCopyType type );

static void   string_mark( EC_OBJ obj );
static void   string_free( EC_OBJ obj );
static EcInt  string_print( ec_string *str, EC_OBJ obj, EcBool detailed );

static EC_OBJ string_getitem( EC_OBJ obj, EC_OBJ index );
static EC_OBJ string_setitem( EC_OBJ obj, EC_OBJ index, EC_OBJ value );
static EC_OBJ string_delitem( EC_OBJ obj, EC_OBJ index );
static EC_OBJ string_length( EC_OBJ obj );

static EcUInt string_hash( EC_OBJ obj, EcInt recur );
static EcBool string_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res );

#if 0
static EC_OBJ string_promote( EC_OBJ obj1, EC_OBJ obj2 );
#endif
static EC_OBJ string_add( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ string_radd( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ string_mul( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ string_mod( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ string_int( EC_OBJ obj );
static EC_OBJ string_float( EC_OBJ obj );
static EC_OBJ string_true( EC_OBJ obj );

/* C API */

EC_API EC_OBJ EcMakeString( const char *string, EcInt len )
{
	EC_OBJ obj;
	EcInt l;

	obj = EcMakeUser( tc_string, NULL );
	if (EC_ERRORP(obj)) return obj;

	if (len > 0)
		l = len;
	else
		l = string ? strlen( string ) : 0;
	EC_STRLEN(obj) = 0;
	EC_STRDATA(obj) = ec_malloc( l + 1 );
	if (! EC_STRDATA(obj)) return Ec_ERROR;
	EC_STRLEN(obj) = l;

	memcpy( EC_STRDATA(obj), string ? string : "", l );
	EC_STRDATA(obj)[l] = '\0';
	return obj;
}

/* elastiC API */


/* Private */

static EC_OBJ string_copy( EC_OBJ obj, EcCopyType type )
{
	ASSERT( EC_STRINGP(obj) );

	return EcMakeString( EC_STRDATA(obj), EC_STRLEN(obj) );
}

static void string_mark( EC_OBJ obj )
{
	ASSERT( EC_STRINGP(obj) );
}

static void string_free( EC_OBJ obj )
{
	ASSERT( EC_STRINGP(obj) );

	ec_free( EC_STRDATA(obj) );
	EC_STRDATA(obj) = NULL;
	EC_STRLEN(obj) = 0;
}

static EcInt detailed_printstring( ec_string *dst, const char *src, EcInt srclen )
{
	const char *p = src;
	char  buf[10];
	EcInt i;
	EcInt res;

	res = 0;
	for (i = 0; i < srclen; i++, p++)
	{
		if (isprint( (int)*p ))
		{
			ec_strcatc( dst, *p );
			res++;
		}
		else
		{
			switch (*p)
			{
			case '\n':
				ec_strcat( dst, "\\n", 0 );
				res += 2;
				break;
			case '\t':
				ec_strcat( dst, "\\t", 0 );
				res += 2;
				break;
			case '\b':
				ec_strcat( dst, "\\b", 0 );
				res += 2;
				break;
			case '\r':
				ec_strcat( dst, "\\r", 0 );
				res += 2;
				break;
			case '\f':
				ec_strcat( dst, "\\f", 0 );
				res += 2;
				break;
			case '\v':
				ec_strcat( dst, "\\v", 0 );
				res += 2;
				break;
			case '\a':
				ec_strcat( dst, "\\a", 0 );
				res += 2;
				break;
			case '\?':
				ec_strcat( dst, "\\?", 0 );
				res += 2;
				break;
			case '\0':
				ec_strcat( dst, "\\0", 0 );
				res += 2;
				break;
			default:
				sprintf( buf, "\\x%02x", (int)*p );
				ec_strcat( dst, buf, 0 );
				res += strlen( buf );
			}
		}
	}

	return res;
}

static EcInt string_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	EcInt res = 0;
	ASSERT( EC_STRINGP(obj) );

	if (detailed)
	{
		ec_strcat( str, "\"", 0 );
		res += detailed_printstring( str, EC_STRDATA(obj), EC_STRLEN(obj) );
		ec_strcat( str, "\"", 0 );
		res += 2;
	} else
	{
		ec_strcat( str, EC_STRDATA(obj), EC_STRLEN(obj) );
		res += EC_STRLEN(obj);
	}
	return res;
}

static EC_OBJ string_getitem( EC_OBJ obj, EC_OBJ index )
{
	if (! EC_INUMP(index))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	if ((EC_INUM(index) < 0) || (EC_INUM(index) >= EC_STRLEN(obj)))
	{
		ec_string ds;

		ec_string_init( &ds, NULL );
		ec_sprintf( &ds, "Index %ld out of bounds for object %w", EC_INUM(index), obj );

		EcMakeExceptionWithArgs( EcIndexErrorClass,
								 ec_strdata( &ds ), 2,
								 index, obj );
		ec_string_cleanup( &ds );
		return Ec_ERROR;
	}

	return EcMakeChar( EC_STRDATA(obj)[EC_INUM(index)] );
}

static EC_OBJ string_setitem( EC_OBJ obj, EC_OBJ index, EC_OBJ value )
{
	if (! EC_INUMP(index))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	if (! EC_CHARP(value))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	if (EC_INUM(index) >= EC_STRLEN(obj))
	{
		/* expand the string */

		EcInt newlen, i;
		char  *newdata;

		newlen  = EC_INUM(index) + 1;
		newdata = ec_realloc( EC_STRDATA(obj), newlen + 1 );
		if (! newdata)
			return EcMemoryError();

		for (i = EC_STRLEN(obj); i < newlen; i++)
			newdata[i] = '\0';
		newdata[newlen] = '\0';

		EC_STRDATA(obj) = newdata;
		EC_STRLEN(obj) = newlen;
	}

	if ((EC_INUM(index) < 0) || (EC_INUM(index) >= EC_STRLEN(obj)))
	{
		ec_string ds;

		ec_string_init( &ds, NULL );
		ec_sprintf( &ds, "Index %ld out of bounds for object %w", EC_INUM(index), obj );

		EcMakeExceptionWithArgs( EcIndexErrorClass,
								 ec_strdata( &ds ), 2,
								 index, obj );
		ec_string_cleanup( &ds );
		return Ec_ERROR;
	}

	EC_STRDATA(obj)[EC_INUM(index)] = EC_CHAR(value);
	return obj;
}

static EC_OBJ string_delitem( EC_OBJ obj, EC_OBJ index )
{
	EcInt idx;

	if (! EC_INUMP(index))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	if ((EC_INUM(index) < 0) || (EC_INUM(index) >= EC_STRLEN(obj)))
	{
		ec_string ds;

		ec_string_init( &ds, NULL );
		ec_sprintf( &ds, "Index %ld out of bounds for object %w", EC_INUM(index), obj );

		EcMakeExceptionWithArgs( EcIndexErrorClass,
								 ec_strdata( &ds ), 2,
								 index, obj );
		ec_string_cleanup( &ds );
		return Ec_ERROR;
	}

	idx = EC_INUM(index);
	if (idx < EC_STRLEN(obj) - 1)
	{
		memcpy( EC_STRDATA(obj) + idx, EC_STRDATA(obj) + idx + 1, EC_STRLEN(obj) - idx - 1 );
		EC_STRLEN(obj) -= 1;
	}
	else
	{
		EC_STRDATA(obj)[idx] = '\0';							/* unnecessary */
		EC_STRLEN(obj) -= 1;
	}

	return obj;
}

static EC_OBJ string_length( EC_OBJ obj )
{
	return EcMakeInt( EC_STRLEN(obj) );
}

static EcUInt string_hash( EC_OBJ obj, EcInt recur )
{
	ASSERT( EC_STRINGP(obj) );

	return ec_hash_pjw_string( EC_STRDATA(obj) );
}

static EcBool string_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res )
{
	EcInt l1;
	EcInt l2;
	int t;

	if (! EC_STRINGP(obj2))
		return FALSE;

	l1 = EC_STRLEN(obj1);
	l2 = EC_STRLEN(obj2);

	if (l1 < l2)
	{
		t = memcmp( EC_STRDATA(obj1), EC_STRDATA(obj2), l1 );
		if (t <= 0)
			*res = -1;
		else
			*res = 1;
	}
	else if (l1 > l2)
	{
		t = memcmp( EC_STRDATA(obj1), EC_STRDATA(obj2), l2 );
		if (t >= 0)
			*res = 1;
		else
			*res = -1;
	}
	else
		*res = memcmp( EC_STRDATA(obj1), EC_STRDATA(obj2), l1 );

	return TRUE;
}

#if 0
static EC_OBJ string_promote( EC_OBJ obj1, EC_OBJ obj2 )
{
	ec_string str;
	EC_OBJ    repr2;
	EC_OBJ    res;

	ASSERT( EC_STRINGP(obj1 ) );

	ec_string_init( &str, NULL );
	ec_sprintf( &str, "%w", obj2 );
	repr2 = EcMakeString( ec_strdata( &str ), ec_strlen( &str ) );
	ec_string_cleanup( &str );

	res = EcMakeArray( 2 );
	if (EC_ERRORP(res)) return res;

	EcArrayPush( res, obj1 );
	EcArrayPush( res, repr2 );
	return res;
}
#endif

static EC_OBJ string_add( EC_OBJ obj1, EC_OBJ obj2 )
{
	ec_string str;
	EC_OBJ    res;

	if ((! EC_CHARP(obj2)) && (! EC_STRINGP(obj2)))
		return EcTypeError( EC_NIL, EC_NIL, 2, tc_none, obj2, TRUE, "string add" );

	ASSERT( EC_STRINGP(obj1) );

	ec_string_init( &str, NULL );
	ec_strcat( &str, EC_STRDATA(obj1), EC_STRLEN(obj1) );
	if (EC_STRINGP(obj2))
		ec_strcat( &str, EC_STRDATA(obj2), EC_STRLEN(obj2) );
	else if (EC_CHARP(obj2))
		ec_strcatc( &str, EC_CHAR(obj2) );
	res = EcMakeString( ec_strdata( &str ), ec_strlen( &str ) );
	ec_string_cleanup( &str );

	return res;
}

static EC_OBJ string_radd( EC_OBJ obj1, EC_OBJ obj2 )
{
	ec_string str;
	EC_OBJ    res;

	if (! EC_STRINGP(obj2))
		return EcTypeError( EC_NIL, EC_NIL, 2, tc_string, obj2, TRUE, "string radd" );

	ASSERT( EC_STRINGP(obj1) );
	ASSERT( EC_STRINGP(obj2) );

	ec_string_init( &str, NULL );
	ec_strcat( &str, EC_STRDATA(obj2), EC_STRLEN(obj2) );
	ec_strcat( &str, EC_STRDATA(obj1), EC_STRLEN(obj1) );
	res = EcMakeString( ec_strdata( &str ), ec_strlen( &str ) );
	ec_string_cleanup( &str );

	return res;
}

static EC_OBJ string_mul( EC_OBJ obj1, EC_OBJ obj2 )
{
	ec_string str;
	EC_OBJ    res;
	EcInt     i;

	if (! EC_INUMP(obj2))
		return EcTypeError( EC_NIL, EC_NIL, 2, tc_inum, obj2, TRUE, "string mul" );

	ASSERT( EC_STRINGP(obj1) );

	ec_string_init( &str, NULL );
	for (i = 0; i < EC_INUM(obj2); i++)
		ec_strcat( &str, EC_STRDATA(obj1), EC_STRLEN(obj1) );
	res = EcMakeString( ec_strdata( &str ), ec_strlen( &str ) );
	ec_string_cleanup( &str );

	return res;
}

static EC_OBJ string_mod( EC_OBJ obj1, EC_OBJ obj2 )
{
	ASSERT( EC_STRINGP(obj1) );

	if (EC_ARRAYP(obj2))
		return ec_printf_obj( EC_STRDATA(obj1), EC_ARRAYMEM(obj2), EC_ARRAYLEN(obj2), 0, EC_NIL );
	else if (! EcIsSequence(obj2))
		return ec_printf_obj( EC_STRDATA(obj1), &obj2, 1, 0, EC_NIL );
	else
	{
		/* :TODO: XXX Handle non-array sequences ! */

		EC_ASSERT( (! EC_ARRAYP(obj2)) && EcIsSequence(obj2) );
		return EcTypeError( EC_NIL, EC_NIL, 2, tc_none, obj2, TRUE,
							"string mod: non-array sequences not handled yet" );
	}
}

static EC_OBJ string_int( EC_OBJ obj )
{
	long  val;
	char *endptr;

	ASSERT( EC_STRINGP(obj) );

	val = strtol( EC_STRDATA(obj), &endptr, 0 );
	if (((val == LONG_MIN) ||
		 (val == LONG_MAX) ||
		 (val < EC_ECINT_MIN) ||
		 (val > EC_ECINT_MAX)) &&
		(errno == ERANGE))
		return EcTypeError( EC_NIL, EC_NIL, 1, tc_none, obj, TRUE, "string int: undeflow or overflow" );
	if (endptr == EC_STRDATA(obj))
		return EcTypeError( EC_NIL, EC_NIL, 1, tc_none, obj, TRUE, "string int: bad integer number" );

	return EcMakeInt( val );
}

static EC_OBJ string_float( EC_OBJ obj )
{
	EcFloat val;
	char   *endptr;

	ASSERT( EC_STRINGP(obj) );

	val = ec_strtod( EC_STRDATA(obj), &endptr );
	if (((val == HUGE_VAL) ||
		 (val == -HUGE_VAL)) &&
		(errno == ERANGE))
		return EcTypeError( EC_NIL, EC_NIL, 1, tc_none, obj, TRUE, "string float: undeflow or overflow" );
	if (endptr == EC_STRDATA(obj))
		return EcTypeError( EC_NIL, EC_NIL, 1, tc_none, obj, TRUE, "string float: bad decimal number" );

	return EcMakeFloat( val );
}

static EC_OBJ string_true( EC_OBJ obj )
{
	return EcTrueObject;
}

EcBool _ec_string_init( void )
{
	EcSequenceCallbacks sequence_cb = {
		/* getitem */		string_getitem,
		/* setitem */		string_setitem,
		/* delitem */		string_delitem,
		/* length  */		string_length
	};

	EcNumericCallbacks numeric_cb = {
		/* promote */		NULL /* string_promote */,
		/* add     */		string_add,
		/* radd    */		string_radd,
		/* sub     */		NULL,
		/* rsub    */		NULL,
		/* mul     */		string_mul,
		/* rmul    */		NULL,
		/* div     */		NULL,
		/* rdiv    */		NULL,
		/* mod     */		string_mod,
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
		/* int     */		string_int,
		/* float   */		string_float,
		/* true    */		string_true
	};

	EcTypespec stringspec = {
		/* type    */		0,
		/* name    */		"string",

		/* copy    */		string_copy,

		/* mark    */		string_mark,
		/* free    */		string_free,
		/* print   */		string_print,

		/* hash    */		string_hash,
		/* compare */		string_compare,

		/* check   */		NULL,

		/* sequence */      &sequence_cb,
		/* numeric  */      &numeric_cb
	};

	tc_string = EcAddType( &stringspec );
	if (tc_string == 0)
		return FALSE;

	return TRUE;
}

void _ec_string_cleanup( void )
{
}
