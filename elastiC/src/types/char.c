/* ==========================================================================
 * char.c
 * ==========================================================================
 * Char basic type.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat May 23 11:02:52 MEST 1998
 *
 *   $Id: char.c,v 1.1 2002/05/23 21:20:16 panta Exp $
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
#include <string.h>

#include "elastic.h"
#include "private.h"
#include "debug.h"

#include "hashfunc.h"


EC_API EcInt tc_char = -1;

static EC_OBJ char_copy( EC_OBJ obj, EcCopyType type );

static EcInt  char_print( ec_string *str, EC_OBJ obj, EcBool detailed );

static EcUInt char_hash( EC_OBJ obj, EcInt recur );
static EcBool char_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res );

#if 0
static EC_OBJ char_promote( EC_OBJ obj1, EC_OBJ obj2 );
#endif
static EC_OBJ char_add( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ char_radd( EC_OBJ obj1, EC_OBJ obj2 );
static EC_OBJ char_int( EC_OBJ obj );
static EC_OBJ char_float( EC_OBJ obj );
static EC_OBJ char_true( EC_OBJ obj );

/* C API */

EC_API EC_OBJ EcMakeChar( EcChar value )
{
	EC_OBJ res;

	res = EcMakeUser( tc_char, NULL );
	EC_CHAR(res) = value;

	return res;
}

/* elastiC API */


/* Private */

static EC_OBJ char_copy( EC_OBJ obj, EcCopyType type )
{
	ASSERT( EC_CHARP(obj) );

	return EcMakeChar( (EcChar) EC_CHAR(obj) );
}

static EcInt detailed_printchar( ec_string *dst, EcChar ch )
{
	EcInt res = 0;
	char  buf[10];

	if (isprint( (int)ch ))
	{
		ec_strcatc( dst, (char) ch );
		res = 1;
	} else
	{
		switch (ch)
		{
		case '\n':
			ec_strcat( dst, "\\n", 0 );
			res = 2;
			break;
		case '\t':
			ec_strcat( dst, "\\t", 0 );
			res = 2;
			break;
		case '\b':
			ec_strcat( dst, "\\b", 0 );
			res = 2;
			break;
		case '\r':
			ec_strcat( dst, "\\r", 0 );
			res = 2;
			break;
		case '\f':
			ec_strcat( dst, "\\f", 0 );
			res = 2;
			break;
		case '\v':
			ec_strcat( dst, "\\v", 0 );
			res = 2;
			break;
		case '\a':
			ec_strcat( dst, "\\a", 0 );
			res = 2;
			break;
		case '\?':
			ec_strcat( dst, "\\?", 0 );
			res = 2;
			break;
		case '\0':
			ec_strcat( dst, "\\0", 0 );
			res = 2;
			break;
		default:
			sprintf( buf, "\\x%02x", (int)ch );
			ec_strcat( dst, buf, 0 );
			res += strlen( buf );
		}
	}

	return res;
}

static EcInt char_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	EcInt res = 0;

	ASSERT( EC_CHARP(obj) );

	if (detailed)
	{
		ec_strcat( str, "\'", 0 );
		res = detailed_printchar( str, (EcChar) EC_CHAR(obj) );
		ec_strcat( str, "\'", 0 );
		res += 2;
	} else
	{
		if (isprint( (int)EC_CHAR(obj) ) || (EC_CHAR(obj) == '\n'))
			res = ec_sprintf( str, "%c", EC_CHAR(obj) );
		else
			res = ec_sprintf( str, "\\x%02x", (int)EC_CHAR(obj) );
	}
	return res;
}

static EcUInt char_hash( EC_OBJ obj, EcInt recur )
{
	ASSERT( EC_CHARP(obj) );

	return ec_hash_uint( (EcUInt)EC_CHAR(obj) );
}

static EcBool char_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res )
{
	if (! EC_CHARP(obj2))
		return FALSE;

	*res = (EcInt) (EC_CHAR(obj1) - EC_CHAR(obj2));
	return TRUE;
}

#if 0
static EC_OBJ char_promote( EC_OBJ obj1, EC_OBJ obj2 )
{
}
#endif

static EC_OBJ char_add( EC_OBJ obj1, EC_OBJ obj2 )
{
	if (EC_INUMP(obj2))
		return EcMakeChar( (EcChar) ((EcInt)EC_CHAR(obj1) + EC_INUM(obj2)) );
	else if (EC_STRINGP(obj2))
	{
		ec_string str;
		EC_OBJ    res;

		ec_string_init( &str, NULL );
		ec_strcatc( &str, (char) EC_CHAR(obj1) );
		ec_strcat( &str, EC_STRDATA(obj2), EC_STRLEN(obj2) );
		res = EcMakeString( ec_strdata( &str ), ec_strlen( &str ) );
		ec_string_cleanup( &str );

		return res;
	}

	return EcTypeError( EC_NIL, EC_NIL, 2, tc_inum, obj2, TRUE, "char add" );
}

static EC_OBJ char_radd( EC_OBJ obj1, EC_OBJ obj2 )
{
	if (EC_INUMP(obj2))
		return EcMakeChar( (EcChar) ((EcInt)EC_CHAR(obj1) + EC_INUM(obj2)) );

	return EcTypeError( EC_NIL, EC_NIL, 2, tc_inum, obj2, TRUE, "char radd" );
}

static EC_OBJ char_int( EC_OBJ obj )
{
	return EcMakeInt( (EcInt) EC_CHAR(obj) );
}

static EC_OBJ char_float( EC_OBJ obj )
{
	return EcMakeFloat( (EcFloat) EC_CHAR(obj) );
}

static EC_OBJ char_true( EC_OBJ obj )
{
	return EC_CHAR(obj) ? EcTrueObject : EcFalseObject;
}

EcBool _ec_char_init( void )
{
	EcNumericCallbacks numeric_cb =
	{
		/* promote */		NULL /* char_promote */,
		/* add     */		char_add,
		/* radd    */		char_radd,
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
		/* int     */		char_int,
		/* float   */		char_float,
		/* true    */		char_true
	};

	EcTypespec charspec = {
		/* type     */		0,
		/* name     */		"char",

		/* copy     */		char_copy,

		/* mark     */		NULL,
		/* free     */		NULL,
		/* print    */		char_print,

		/* hash     */		char_hash,
		/* compare  */		char_compare,

		/* check    */		NULL,

		/* sequence */      NULL,
		/* numeric  */      &numeric_cb
	};

	tc_char = EcAddType( &charspec );
	if (tc_char == 0)
		return FALSE;

	return TRUE;
}

void _ec_char_cleanup( void )
{
}
