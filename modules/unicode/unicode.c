/* ==========================================================================
 * unicode.c
 * ==========================================================================
 * A 'unicode' elastiC module.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Emiliano                 E-mail: emile@iris-advies.nl
 *
 *   Minor modifications by:
 *            Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: 18 Oct 2001
 *
 *   $Id: unicode.c,v 1.1 2002/05/30 18:39:45 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 2001-2002 Emiliano <emile@iris-advies.nl>. All rights reserved.
 *    Copyright (C) Marco Pantaleoni. All rights reserved.
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

#include <elastic/basic.h>
#include <elastic/elastic.h>
#include <elastic/private.h>
#include <elastic/user.h>
#include <elastic/hashfunc.h>
#include <elastic/debug.h>
#include <unicode.h>

#include "config.h"
#include "unicode.h"

EC_API EcInt tc_ustring = -1;

#define EC_USTRINGP(obj) (EC_TYPEP(obj,tc_ustring))
#define EC_USTRDATA(obj) (EC_USER0(obj, Uchar*))
#define EC_USTRLEN(obj) (EC_USER1(obj, int))

static EC_OBJ ustring_copy( EC_OBJ obj, EcCopyType type );

static void   ustring_mark( EC_OBJ obj );
static void   ustring_free( EC_OBJ obj );
static EcInt  ustring_print( ec_string *str, EC_OBJ obj, EcBool detailed );

#if 0
static EC_OBJ ustring_getitem( EC_OBJ obj, EC_OBJ index );
static EC_OBJ ustring_setitem( EC_OBJ obj, EC_OBJ index, EC_OBJ value );
static EC_OBJ ustring_delitem( EC_OBJ obj, EC_OBJ index );
#endif
static EC_OBJ ustring_length( EC_OBJ obj );

static EcUInt ustring_hash( EC_OBJ obj, EcInt recur );
static EcBool ustring_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res );

#if 0
static EC_OBJ ustring_promote( EC_OBJ obj1, EC_OBJ obj2 );
#endif
static EC_OBJ ustring_add( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ ustring_radd( EC_OBJ obj1, EC_OBJ obj2 );
#if 0
static EC_OBJ ustring_mul( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ ustring_int( EC_OBJ obj );
static EC_OBJ ustring_float( EC_OBJ obj );
#endif
static EC_OBJ ustring_true( EC_OBJ obj );

/* C API */

EC_API EC_OBJ EcMakeUString( const Uchar *ustring, EcInt len , EcBool duplicate )
{
	EC_OBJ obj;
	EcInt l;

	obj = EcMakeUser( tc_ustring, NULL );
	if (EC_ERRORP(obj)) return obj;

	if (len > 0) l = len;
	else l = ustring ? uni_strlen( ustring ) : 0;

	EC_USTRLEN(obj) = 0;

	if (duplicate) {
		EC_USTRDATA(obj) = ec_malloc((l + 1) * sizeof(Uchar));
		if (! EC_USTRDATA(obj)) return Ec_ERROR;
		uni_strcpy(EC_USTRDATA(obj), ustring);
	} else {
		EC_USTRDATA(obj) = ustring;
	}

	EC_USTRLEN(obj) = l;

	return obj;
}

/* elastiC API */


/* Private */

static EC_OBJ ustring_copy( EC_OBJ obj, EcCopyType type )
{
	EC_ASSERT( EC_USTRINGP(obj) );

	return EcMakeUString( EC_USTRDATA(obj), EC_USTRLEN(obj) , EcTrue );
}

static void ustring_mark( EC_OBJ obj )
{
	EC_ASSERT( EC_USTRINGP(obj) );
}

static void ustring_free( EC_OBJ obj )
{
	EC_ASSERT( EC_USTRINGP(obj) );

	ec_free( EC_USTRDATA(obj) );
	EC_USTRDATA(obj) = NULL;
	EC_USTRLEN(obj) = 0;
}

static EcInt ustring_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	EcInt res = 0;
	EC_ASSERT( EC_USTRINGP(obj) );

	{
		ec_strcat( str, "<unicode string>", sizeof("<unicode string>") - 1 );
		res += sizeof("<unicode string>") - 1;
	}
	return res;
}

#if 0 /* unimplemented */
static EC_OBJ ustring_getitem( EC_OBJ obj, EC_OBJ index )
{
	if (! EC_INUMP(index))
	{
		/* TODO: Throw an exception */
		EC_ASSERT( FALSE );
		return Ec_ERROR;
	}

	if ((EC_INUM(index) < 0) || (EC_INUM(index) >= EC_USTRLEN(obj)))
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

	return EcMakeUChar( EC_USTRDATA(obj)[EC_INUM(index)] );
}

static EC_OBJ string_setitem( EC_OBJ obj, EC_OBJ index, EC_OBJ value )
{
	if (! EC_INUMP(index))
	{
		/* TODO: Throw an exception */
		EC_ASSERT( FALSE );
		return Ec_ERROR;
	}

	if (! EC_CHARP(value))
	{
		/* TODO: Throw an exception */
		EC_ASSERT( FALSE );
		return Ec_ERROR;
	}

	if (EC_INUM(index) >= EC_USTRLEN(obj))
	{
		/* expand the string */

		EcInt newlen, i;
		char  *newdata;

		newlen  = EC_INUM(index) + 1;
		newdata = ec_realloc( EC_USTRDATA(obj), newlen + 1 );
		if (! newdata)
			return EcMemoryError();

		for (i = EC_USTRLEN(obj); i < newlen; i++)
			newdata[i] = '\0';
		newdata[newlen] = '\0';

		EC_USTRDATA(obj) = newdata;
		EC_USTRLEN(obj) = newlen;
	}

	if ((EC_INUM(index) < 0) || (EC_INUM(index) >= EC_USTRLEN(obj)))
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

	EC_USTRDATA(obj)[EC_INUM(index)] = EC_CHAR(value);
	return obj;
}

static EC_OBJ string_delitem( EC_OBJ obj, EC_OBJ index )
{
	EcInt idx;

	if (! EC_INUMP(index))
	{
		/* TODO: Throw an exception */
		EC_ASSERT( FALSE );
		return Ec_ERROR;
	}

	if ((EC_INUM(index) < 0) || (EC_INUM(index) >= EC_USTRLEN(obj)))
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
	if (idx < EC_USTRLEN(obj) - 1)
	{
		memcpy( EC_USTRDATA(obj) + idx, EC_USTRDATA(obj) + idx + 1, EC_USTRLEN(obj) - idx - 1 );
		EC_USTRLEN(obj) -= 1;
	}
	else
	{
		EC_USTRDATA(obj)[idx] = '\0';							/* unnecessary */
		EC_USTRLEN(obj) -= 1;
	}

	return obj;
}
#endif

static EC_OBJ ustring_length( EC_OBJ obj )
{
	return EcMakeInt( EC_USTRLEN(obj) );
}

static EcUInt ustring_hash( EC_OBJ obj, EcInt recur )
{
	EC_ASSERT( EC_USTRINGP(obj) );

	return ec_hash_pjw_string( (char *)EC_USTRDATA(obj) );
}

static EcBool ustring_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res )
{
	EcInt l1;
	EcInt l2;
	int t;

	if (! EC_USTRINGP(obj2))
		return FALSE;

	l1 = EC_USTRLEN(obj1);
	l2 = EC_USTRLEN(obj2);

	if (l1 < l2)
	{
		t = memcmp( EC_USTRDATA(obj1), EC_USTRDATA(obj2), l1 );
		if (t <= 0)
			*res = -1;
		else
			*res = 1;
	}
	else if (l1 > l2)
	{
		t = memcmp( EC_USTRDATA(obj1), EC_USTRDATA(obj2), l2 );
		if (t >= 0)
			*res = 1;
		else
			*res = -1;
	}
	else
		*res = uni_strcmp( EC_USTRDATA(obj1), EC_USTRDATA(obj2) );

	return TRUE;
}

#if 0
static EC_OBJ string_promote( EC_OBJ obj1, EC_OBJ obj2 )
{
	ec_string str;
	EC_OBJ    repr2;
	EC_OBJ    res;

	EC_ASSERT( EC_USTRINGP(obj1 ) );

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

static EC_OBJ ustring_add( EC_OBJ obj1, EC_OBJ obj2 )
{
	/* ec_string str; */
	EC_OBJ    res;
	EcInt     len;

	/* add chars later */
	if (/* (! EC_CHARP(obj2)) && */(! EC_USTRINGP(obj2)))
		return EcTypeError( EC_NIL, EC_NIL, 2, tc_none, obj2, TRUE, "string add" );

	EC_ASSERT( EC_USTRINGP(obj1) );

	len = EC_USTRLEN(obj1) + EC_USTRLEN(obj2);
	res = EcMakeUString( EC_USTRDATA(obj1), len , EcTrue );
	u_strcat(EC_USTRDATA(res), EC_USTRDATA(obj2) );
	EC_USTRLEN(res) = len;

	return res;
}

static EC_OBJ ustring_radd( EC_OBJ obj1, EC_OBJ obj2 )
{
	/* ec_string str; */
	EC_OBJ    res;
	EcInt     len;

	if (! EC_USTRINGP(obj2))
		return EcTypeError( EC_NIL, EC_NIL, 2, tc_string, obj2, TRUE, "string radd" );

	EC_ASSERT( EC_USTRINGP(obj1) );
	EC_ASSERT( EC_USTRINGP(obj2) );

	len = EC_USTRLEN(obj1) + EC_USTRLEN(obj2);
	res = EcMakeUString( EC_USTRDATA(obj2), len , EcTrue );
	u_strcat(EC_USTRDATA(res), EC_USTRDATA(obj1) );
	EC_USTRLEN(res) = len;

	return res;
}

#if 0
static EC_OBJ ustring_mul( EC_OBJ obj1, EC_OBJ obj2 )
{
	ec_string str;
	EC_OBJ    res;
	EcInt     i;

	if (! EC_INUMP(obj2))
		return EcTypeError( EC_NIL, EC_NIL, 2, tc_inum, obj2, TRUE, "string mul" );

	EC_ASSERT( EC_USTRINGP(obj1) );

	ec_string_init( &str, NULL );
	for (i = 0; i < EC_INUM(obj2); i++)
		ec_strcat( &str, EC_USTRDATA(obj1), EC_USTRLEN(obj1) );
	res = EcMakeString( ec_strdata( &str ), ec_strlen( &str ) );
	ec_string_cleanup( &str );

	return res;
}

static EC_OBJ string_int( EC_OBJ obj )
{
	EcInt   val;
	char   *endptr;

	EC_ASSERT( EC_USTRINGP(obj) );

	val = strtol( EC_USTRDATA(obj), &endptr, 0 );
	if (((val == LONG_MIN) ||
		 (val == LONG_MAX)) &&
		(errno == ERANGE))
		return EcTypeError( EC_NIL, EC_NIL, 1, tc_none, obj, TRUE, "string int: undeflow or overflow" );
	if (endptr == EC_USTRDATA(obj))
		return EcTypeError( EC_NIL, EC_NIL, 1, tc_none, obj, TRUE, "string int: bad integer number" );

	return EcMakeInt( val );
}

static EC_OBJ string_float( EC_OBJ obj )
{
	EcFloat val;
	char   *endptr;

	EC_ASSERT( EC_USTRINGP(obj) );

	val = strtod( EC_USTRDATA(obj), &endptr );
	if (((val == HUGE_VAL) ||
		 (val == -HUGE_VAL)) &&
		(errno == ERANGE))
		return EcTypeError( EC_NIL, EC_NIL, 1, tc_none, obj, TRUE, "string float: undeflow or overflow" );
	if (endptr == EC_USTRDATA(obj))
		return EcTypeError( EC_NIL, EC_NIL, 1, tc_none, obj, TRUE, "string float: bad decimal number" );

	return EcMakeFloat( val );
}
#endif

static EC_OBJ ustring_true( EC_OBJ obj )
{
	return EcTrueObject;
}

EC_OBJ EcUnicode_UTF8ToUnicode( EC_OBJ stack, EcAny userdata );
EC_OBJ EcUnicode_UnicodeToUTF8( EC_OBJ stack, EcAny userdata );

EC_OBJ EcUnicode_UTF8ToUnicode( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ res;
	char *str;

	res = EcParseStackFunction("unicode.utf8_to_uc", TRUE, stack, "z", &str);
	if (EC_ERRORP(res)) return res;

	return EcMakeUString(u_utf8_to_uc(str), 0, EcFalse);
}

EC_OBJ EcUnicode_UnicodeToUTF8( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ res;
	EC_OBJ ustring;
	char *str;

	res = EcParseStackFunction("midgard.core.rows", TRUE, stack, "O!",
							   tc_ustring, &ustring);
	if (EC_ERRORP(res)) return res;

	str = u_uc_to_utf8(EC_USTRDATA(ustring));
	res = EcMakeString(str, 0);
	free(str);
	return res;
}

/* ------------------------------------------------------------------------
 * Initialization & cleanup
 * ------------------------------------------------------------------------ */

EC_OBJ ec_unicode_init( void )
{
	EC_OBJ pkg;

	EcSequenceCallbacks sequence_cb = {
		/* getitem */		NULL /* ustring_getitem */,
		/* setitem */		NULL /* ustring_setitem */,
		/* delitem */		NULL /* ustring_delitem */,
		/* length  */		ustring_length
	};

	EcNumericCallbacks numeric_cb = {
		/* promote */		NULL /* ustring_promote */,
		/* add     */		ustring_add,
		/* radd    */		ustring_radd,
		/* sub     */		NULL,
		/* rsub    */		NULL,
		/* mul     */		NULL /* ustring_mul */,
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
		/* int     */		NULL /* ustring_int */,
		/* float   */		NULL /* ustring_float */,
		/* true    */		ustring_true
	};

	EcTypespec ustringspec = {
		/* type    */		0,
		/* name    */		"ustring",

		/* copy    */		ustring_copy,

		/* mark    */		ustring_mark,
		/* free    */		ustring_free,
		/* print   */		ustring_print,

		/* hash    */		ustring_hash,
		/* compare */		ustring_compare,

		/* check   */		NULL,

		/* sequence */      &sequence_cb,
		/* numeric  */      &numeric_cb
	};

	tc_ustring = EcAddType( &ustringspec );
	EC_ASSERT (tc_ustring != 0);

	/* Register package */
	pkg = EcPackageIntroduce( "unicode" );

	EcAddPrimitive( "unicode.utf8_to_uc", EcUnicode_UTF8ToUnicode );
	EcAddPrimitive( "unicode.uc_to_utf8", EcUnicode_UnicodeToUTF8 );

	return pkg;
}

void ec_unicode_cleanup( void )
{
}
