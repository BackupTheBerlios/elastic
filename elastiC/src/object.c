/* ==========================================================================
 * object.c
 * ==========================================================================
 * EcObject handling.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat May 23 11:02:52 MEST 1998
 *
 *   $Id: object.c,v 1.3 2005/03/08 13:40:05 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1998-2002 Marco Pantaleoni. All rights reserved.
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
#include "object.h"
#include "debug.h"
#include "compat.h"

#include "strtable.h"
#include "memory.h"

#include "private.h"
#include "hashfunc.h"


#if MEM_STATS
EcInt _ec_make_stats[EC_MAX_TYPES];
#endif

#if MEM_STATS
#define LOG_MAKE(type)	do { _ec_make_stats[(type)]++; } while (0)
#else
#define LOG_MAKE(type)	do {} while (0)
#endif


/* ========================================================================
 * A P I
 * ======================================================================== */

EC_API EC_OBJ EcMakeUndefined( void )
{
	EC_OBJ obj;

	if (EC_UNDEFINEDP(EcUndefinedObject)) return EcUndefinedObject;

	obj = EcAllocObject();
	if (EC_ERRORP(obj)) return obj;

	EC_OBJTYPE(obj) = tc_undefined;
	return obj;
}

EC_API EC_OBJ EcMakeBool( EcBool value )
{
	EC_OBJ obj;

	LOG_MAKE(tc_bool);

	if (value)
	{
		if (EC_BOOLP(EcTrueObject)) return EcTrueObject;
	} else
	{
		if (EC_BOOLP(EcFalseObject)) return EcFalseObject;
	}

	obj = EcAllocObject();
	if (EC_ERRORP(obj)) return obj;

	EC_OBJTYPE(obj) = tc_bool;
	EC_BOOLVAL(obj) = (value ? 1 : 0);
	return obj;
}

#if ! EC_INLINED_TYPES
EC_API EC_OBJ EcMakeInt( EcInt value )
{
	EC_OBJ obj;

	LOG_MAKE(tc_inum);

	obj = EcAllocObject();
	if (EC_ERRORP(obj)) return obj;

	EC_TYPE(obj) = tc_inum;
	EC_INUM(obj) = value;
	return obj;
}
#endif

EC_API EC_OBJ EcMakeFloat( EcFloat value )
{
	EC_OBJ obj;

	LOG_MAKE(tc_fnum);

	obj = EcAllocObject();
	if (EC_ERRORP(obj)) return obj;

	EC_OBJTYPE(obj) = tc_fnum;
	EC_FNUM(obj) = value;
	return obj;
}

#if ! EC_INLINED_TYPES
EC_API EC_OBJ EcMakeSymbolFromId( EcUInt symid )
{
	EC_OBJ obj;

	LOG_MAKE(tc_symbol);

	obj = EcAllocObject();
	if (EC_ERRORP(obj)) return obj;

	EC_OBJTYPE(obj)   = tc_symbol;
	EC_SYMBOL(obj) = symid;

	return obj;
}

EC_API EC_OBJ EcMakeSymbol( const char *string )
{
	return EcMakeSymbolFromId( EcInternSymbol( string ) );
}
#endif

EC_API EC_OBJ EcMakePrimitive( EcCPrimitive impl, const char *name, EcAny userdata, EcBool userdata_is_object )
{
	EC_OBJ obj;

	LOG_MAKE(tc_primitive);

	obj = EcAllocObject();
	if (EC_ERRORP(obj)) return obj;

	EC_OBJTYPE(obj)           = tc_primitive;
	EC_PRIMITIVE(obj)         = impl;
	EC_PRIMITIVENAME(obj)     = name ? ec_stringdup( name ) : NULL;
	EC_PRIMITIVEUSERDATA(obj) = userdata;

	if (userdata_is_object)
		EC_SET_UDOBJ(obj);
	else
		EC_CLEAR_UDOBJ(obj);

	/* Register primitive pointer */
	ec_register_cpointer( (EcCPointer) impl );

	return obj;
}

EC_API EC_OBJ EcMakeCMethod( EcCMethod impl, const char *name )
{
	EC_OBJ obj;

	LOG_MAKE(tc_cmethod);

	obj = EcAllocObject();
	if (EC_ERRORP(obj)) return obj;

	EC_OBJTYPE(obj)        = tc_cmethod;
	EC_CMETHOD(obj)     = impl;
	EC_CMETHODNAME(obj) = name ? ec_stringdup( name ) : NULL;

	/* Register method pointer */
	ec_register_cpointer( (EcCPointer) impl );

	return obj;
}

EC_API EC_OBJ EcMakeUser( EcInt type, void *data )
{
	EC_OBJ obj;

	LOG_MAKE(type);

	obj = EcAllocObject();
	if (EC_ERRORP(obj)) return obj;

	EC_OBJTYPE(obj) = (EcWord) type;
	EC_USER(obj) = data;

	return obj;
}


/* ------------------------------------------------------------------------
 * M I S C   F U N C T I O N S
 * ------------------------------------------------------------------------ */

/* Analogous to Python Py_BuildValue */

typedef union {
	char    *s;
	EcInt    i;
	EcBool   b;
	EcChar   c;
	EcFloat  f;
	EC_OBJ   o;
	void    *v;
} Any;

#define AS_STRING(any)		((any).s)
#define AS_INT(any)			((any).i)
#define AS_BOOL(any)		((any).b)
#define AS_CHAR(any)		((any).c)
#define AS_FLOAT(any)		((any).f)
#define AS_OBJ(any)			((any).o)
#define AS_VOIDP(any)		((any).v)

static EC_OBJ buildvalue( const char *format, va_list args );
static EC_OBJ buildvalue_sub( const char **formatp, int mode, Any **args );

EC_API EC_OBJ EcBuildValue( const char *format, ... )
{
	va_list args;
	EC_OBJ res;

	va_start( args, format );
	res = buildvalue( format, args );
	va_end( args );

	return res;
}

static EC_OBJ buildvalue( const char *format, va_list args )
{
	const char *fmt;
	Any *array;
	int l, i;

	l = strlen( format ) * 2;
	array = alloca( l * sizeof(Any) );

	i = 0;
	for (fmt = format; *fmt; fmt++)
	{
		switch (*fmt)
		{
		case ' ':
		case '\t':
		case ',':
		case ':':
			/* ignore */
			break;

		case 's':
		case 'z':
			if (fmt[1] == '#')
			{
				fmt++;
				AS_STRING( array[i++] ) = va_arg( args, char * );
				AS_INT( array[i++] )    = va_arg( args, EcInt );
			} else
			{
				AS_STRING( array[i++] ) = va_arg( args, char * );
			}
			break;

		case 'b':
			AS_BOOL( array[i++] ) = va_arg( args, EcBool );
			break;
		case 'i':
			AS_INT( array[i++] ) = va_arg( args, EcInt );
			break;
		case 'c':
#if EC_VARARG_UNPROMOTED
			/*
			 * Are there any platforms/compilers which (erroneously) want
			 * unpromoted types in variadic arguments ?
			 */
			AS_CHAR( array[i++] ) = va_arg( args, EcChar );
#else
			AS_CHAR( array[i++] ) = va_arg( args, EcInt );
#endif
			break;
		case 'f':
			AS_FLOAT( array[i++] ) = va_arg( args, double );	/* can't use float ! (only fully promoted types) */
			break;
		case 'k':
			AS_STRING( array[i++] ) = va_arg( args, char * );
			break;
		case 'O':
			AS_OBJ( array[i++] ) = va_arg( args, EC_OBJ );
			break;

		case '(':
		case '{':
		case ')':
		case '}':
			break;

		default:
			break;
		}
	}

	return buildvalue_sub( &format, 0, &array );
}

#define ACC_RESULT(r)								\
do {												\
	if (mode == 0) {								\
		res = (r);									\
		mode = 3;									\
	} else if (mode == 1) {							\
		if (EC_NULLP(res))							\
			res = EcMakeArray(1);					\
		ASSERT( EC_ARRAYP(res) );					\
		EcArrayPush( res, (r) );					\
	} else if (mode == 2) {							\
		if (EC_NULLP(res))							\
			res = EcMakeHash();						\
		ASSERT( EC_HASHP(res) );					\
		if (EC_NULLP(tmp1))							\
			tmp1 = (r);								\
		else {										\
			EcSequenceSetItem( res, tmp1, (r) );	\
			tmp1 = EC_NIL;							\
		}											\
	} else if (mode == 3) {							\
		EC_OBJ tmp;									\
		tmp = EcMakeArray(2);						\
		EcArrayPush( tmp, res );					\
		EcArrayPush( tmp, (r) );					\
		res = tmp;									\
		mode = 1;									\
	}												\
} while (0)

static EC_OBJ buildvalue_sub( const char **formatp, int mode, Any **args )
{
	EC_OBJ res;
	const char **fmtp;

	/* Variables for conversion */
	char    *strv;
	EcInt    strlenv;
	EcBool   boolv;
	EcInt    intv;
	EcChar   charv;
	EcFloat  floatv;
	EC_OBJ   objv;

	EC_OBJ   ttt, tmp1;

	res  = EC_NIL;
	tmp1 = EC_NIL;

	fmtp = formatp;

	if ((mode == 1) || (mode == 2))
		(*fmtp)++;

	/*ec_stderr_printf( "buildvalue_sub  format: \"%s\"  fmt: \"%s\"  mode: %d\n", *formatp, *fmtp, mode );*/

	for (; **fmtp; (*fmtp)++)
	{
		switch (**fmtp)
		{
		case ' ':
		case '\t':
		case ',':
		case ':':
			/* ignore */
			break;

		case 's':
		case 'z':
			if ((*fmtp)[1] == '#')
			{
				(*fmtp)++;
				strv    = AS_STRING( **args ); (*args)++;
				strlenv = AS_INT( **args ); (*args)++;
			} else
			{
				strv    = AS_STRING( **args ); (*args)++;
				strlenv = -1;
			}
			if (strlenv < 0)
				ACC_RESULT( EcMakeString( strv, 0 ) );
			else
				ACC_RESULT( EcMakeString( strv, strlenv ) );
			break;

		case 'b':
			boolv = AS_BOOL( **args ); (*args)++;
			ACC_RESULT( boolv ? EcTrueObject : EcFalseObject );
			break;

		case 'i':
			intv = AS_INT( **args ); (*args)++;
			ACC_RESULT( EcMakeInt( intv ) );
			break;

		case 'c':
			charv = AS_CHAR( **args ); (*args)++;
			ACC_RESULT( EcMakeChar( charv ) );
			break;

		case 'f':
			floatv = AS_FLOAT( **args ); (*args)++;
			ACC_RESULT( EcMakeFloat( floatv ) );
			break;

		case 'k':
			strv = AS_STRING( **args ); (*args)++;
			ACC_RESULT( EcMakeSymbol( strv ) );
			break;

		case 'O':
			objv = AS_OBJ( **args ); (*args)++;
			ACC_RESULT( objv );
			break;

		case '(':
			ttt = buildvalue_sub( fmtp, 1, args );
			/*ec_stderr_printf( "SUB (: %W\n", ttt );
			  ec_stderr_printf( "[1]RES : %W\n", res );*/
			ACC_RESULT( ttt );
			/*ec_stderr_printf( "[2]RES : %W   (fmt: \"%s\")\n", res, *fmtp );*/
			break;

		case '{':
			ttt = buildvalue_sub( fmtp, 2, args );
			ACC_RESULT( ttt );
			break;

		case ')':
			goto finish;
			break;

		case '}':
			goto finish;
			break;

		default:
			/* ignore */
			break;
		}
	}

 finish:
	/*ec_stderr_printf( "[3]RES : %W\n", res );*/
	return res;
}

EC_API EC_OBJ EcCopy( EC_OBJ obj, EcCopyType type )
{
	if (EC_NULLP(obj))
		return EC_NIL;

	switch (EC_TYPE(obj))
	{
		/* These are (should be) constants and immutable ! */
	case tc_none:
	case tc_undefined:
	case tc_error:
	case tc_bool:
	case tc_inum:
	case tc_fnum:
	case tc_symbol:
	case tc_primitive:
	case tc_cmethod:
		return obj;
		break;
	default:
		if (EC_USERP(obj))
			return USERTYPE(EC_TYPE(obj)).copy_fcn( obj, type );
		ASSERT( FALSE );
		return EcUndefinedObject;
		break;
	}
}

EC_API EC_OBJ EcShallowCopy( EC_OBJ obj )
{
	return EcCopy( obj, EcShallowCopyType );
}

EC_API EC_OBJ EcDeepCopy( EC_OBJ obj )
{
	return EcCopy( obj, EcDeepCopyType );
}

EC_API const char *EcTypeName( EcInt type )
{
	switch (type)
	{
	case tc_none:
		return "NONE";
		break;
	case tc_undefined:
		return "UNDEFINED";
		break;
	case tc_error:
		return "ERROR";
		break;
	case tc_bool:
		return "bool";
		break;
	case tc_inum:
		return "int";
		break;
	case tc_fnum:
		return "float";
		break;
	case tc_symbol:
		return "symbol";
		break;
	case tc_primitive:
		return "primitive";
		break;
	case tc_cmethod:
		return "c-method";
		break;
	default:
		if ((type >= tc_userbase) && (type <= PRIVATE(usertypes)))
			return USERTYPE(type).name ? USERTYPE(type).name : "usertype";
		return "UNKNOWN";
		break;
	}
}

EC_API const char *EcObjectTypeName( EC_OBJ obj )
{
	if (EC_NULLP(obj))
		return "EC_NIL";

	if (EC_USERP(obj))
	{
		if (USERTYPE(EC_TYPE(obj)).name)
			return USERTYPE(EC_TYPE(obj)).name;
		else
			return EcTypeName( EC_TYPE(obj) );
	} else
		return EcTypeName( EC_TYPE(obj) );
}


/* Object fundamental operations */

EC_API EcBool EcObjectEqual( EC_OBJ obj1, EC_OBJ obj2 )
{
	if (obj1 == obj2)
		return TRUE;

	if (EC_NULLP(obj1) || EC_NULLP(obj2))
		return FALSE;

	if (EC_TYPE(obj1) != EC_TYPE(obj2))
		return FALSE;

	switch (EC_TYPE(obj1))
	{
	case tc_none:
		return TRUE;
		break;

	case tc_undefined:
		return FALSE;											/* undefined objects are always different */
		break;

	case tc_error:
		return TRUE;
		break;

	case tc_bool:
		return EC_BOOLVAL(obj1) == EC_BOOLVAL(obj2);
		break;

	case tc_inum:
		return EC_INUM(obj1) == EC_INUM(obj2);
		break;

	case tc_fnum:
		return EC_FNUM(obj1) == EC_FNUM(obj2);
		break;

	case tc_symbol:
		return EC_SYMBOL(obj1) == EC_SYMBOL(obj2);
		break;

	case tc_primitive:
		if (EC_UDOBJP(obj1) != EC_UDOBJP(obj2))
			return FALSE;
		if (EC_UDOBJP(obj1))
			return ((EC_PRIMITIVE(obj1) == EC_PRIMITIVE(obj2)) &&
					EcObjectEqual(EC_PRIMITIVEUSERDATA(obj1), EC_PRIMITIVEUSERDATA(obj2)));
		else
			return ((EC_PRIMITIVE(obj1) == EC_PRIMITIVE(obj2)) &&
					(EC_PRIMITIVEUSERDATA(obj1) == EC_PRIMITIVEUSERDATA(obj2)));
		break;

	case tc_cmethod:
		return EC_CMETHOD(obj1) == EC_CMETHOD(obj2);
		break;

	default:
		if (EC_USERP(obj1))
		{
			/*
			 * TODO: We really should use an equality test not a comparison !!!
			 *       We'll need to add such a test function in user types.
			 */

			EcInt res;

			if (USERTYPE(EC_TYPE(obj1)).compare_fcn)
			{
				if (USERTYPE(EC_TYPE(obj1)).compare_fcn( obj1, obj2, &res ))
					return (res == 0);

				/* Objects not comparable ! */
				return FALSE;
			}
			/* TODO: Throw an exception */
			ASSERT( FALSE );
			return FALSE;
		}
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return FALSE;
		break;
	}

	ASSERT( FALSE );
	return FALSE;
}

EC_API EcBool EcObjectCompare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res )
{
	/*
	 * PASS I
	 *
	 * Test for identity or absolute diversity
	 */

	if (obj1 == obj2)
	{
		*res = 0;												/* identity */
		return TRUE;
	}

	if (EC_NULLP(obj1) || EC_NULLP(obj2))
		return FALSE;

	/*
	 * PASS II
	 *
	 * Test if objects are of equal types
	 */

	if (EC_TYPE(obj1) == EC_TYPE(obj2))
	{
		switch (EC_TYPE(obj1))
		{
		case tc_none:
			ASSERT( FALSE );
			return TRUE;
			break;

		case tc_undefined:
			/* TODO: Referring to an undefined variable: launch an exception */
			ASSERT( FALSE );
			return FALSE;
			break;

		case tc_error:
			ASSERT( FALSE );
			return TRUE;
			break;

		case tc_bool:
			*res = (EcInt) (EC_BOOLVAL(obj1) - EC_BOOLVAL(obj2));
			return TRUE;
			break;

		case tc_inum:
			*res = EC_INUM(obj1) - EC_INUM(obj2);
			return TRUE;
			break;

		case tc_fnum:
		{
			EcFloat d;

			d = EC_FNUM(obj1) - EC_FNUM(obj2);
			if (d < 0.0)
				*res = -1;
			else if (d > 0.0)
				*res = 1;
			else
				*res = 0;
			return TRUE;
		}
		break;

		case tc_symbol:
			*res = (EcInt) (EC_SYMBOL(obj1) - EC_SYMBOL(obj2));
			return TRUE;
			break;

		default:
			if (EC_USERP(obj1))
			{
				if (USERTYPE(EC_TYPE(obj1)).compare_fcn)
				{
					if (USERTYPE(EC_TYPE(obj1)).compare_fcn( obj1, obj2, res ))
						return TRUE;

					/* Objects not comparable ! */
					return FALSE;
				}
				return FALSE;
			}
			return FALSE;
			break;
		}
	}

	/*
	 * PASS III
	 *
	 * Test for numerical types
	 */

	switch (EC_TYPE(obj1))
	{
	case tc_inum:
		switch (EC_TYPE(obj2))
		{
		case tc_inum:
			*res = EC_INUM(obj1) - EC_INUM(obj2);
			return TRUE;
			break;

		case tc_fnum:
		{
			EcFloat d;

			d = (EcFloat)EC_INUM(obj1) - EC_FNUM(obj2);
			if (d < 0.0)
				*res = -1;
			else if (d > 0.0)
				*res = 1;
			else
				*res = 0;
			return TRUE;
		}
		break;
		}

	case tc_fnum:
		switch (EC_TYPE(obj2))
		{
		case tc_inum:
		{
			EcFloat d;

			d = EC_FNUM(obj1) - (EcFloat)EC_INUM(obj2);
			if (d < 0.0)
				*res = -1;
			else if (d > 0.0)
				*res = 1;
			else
				*res = 0;
			return TRUE;
		}
		break;

		case tc_fnum:
		{
			EcFloat d;

			d = EC_FNUM(obj1) - EC_FNUM(obj2);
			if (d < 0.0)
				*res = -1;
			else if (d > 0.0)
				*res = 1;
			else
				*res = 0;
			return TRUE;
		}
		break;
		}
	}

	/*
	 * PASS IV
	 *
	 * Test for user types
	 */
	if (EC_USERP(obj1))
	{
		if (USERTYPE(EC_TYPE(obj1)).compare_fcn)
		{
			if (USERTYPE(EC_TYPE(obj1)).compare_fcn( obj1, obj2, res ))
				return TRUE;
		}
	}
	if (EC_USERP(obj2))
	{
		if (USERTYPE(EC_TYPE(obj2)).compare_fcn)
		{
			if (USERTYPE(EC_TYPE(obj2)).compare_fcn( obj2, obj1, res ))
				return TRUE;
		}
	}

	/*
	 * PASS V
	 *
	 * Give up ! :(
	 * Objects are really uncomparable.
	 */
	return FALSE;
}

EC_API EcUInt EcHash( EC_OBJ obj )
{
	/*
	 * HASHING RULES
	 *
	 * One is our commandment:
	 *
	 *   "Equal objects must have equal hash values"
	 *
	 */

	return _ec_hash( obj, 0 );
}

EcUInt _ec_hash( EC_OBJ obj, EcInt recur )
{
	/*
	 * HASHING RULES
	 *
	 * One is our commandment:
	 *
	 *   "Equal objects must have equal hash values"
	 *
	 */

	if (recur >= MAX_HASH_RECUR) return 11;
	recur++;

	if (EC_NULLP(obj))
		return 0;

	switch (EC_TYPE(obj))
	{
	case tc_none:
		return 0;
		break;

	case tc_undefined:
		return 0;
		break;

	case tc_error:
		return 1;
		break;

	case tc_bool:
		return 2 + EC_BOOLVAL(obj);
		break;

	case tc_inum:
		return ec_hash_uint( (EcUInt)EC_INUM(obj) );
		break;

	case tc_fnum:
		return ec_hash_float( EC_FNUM(obj) );
		break;

	case tc_symbol:
		return ec_hash_uint( (EcUInt)EC_SYMBOL(obj) );
		break;

	case tc_primitive:
		if (EC_UDOBJP(obj))
		{
			if (recur)
				return ec_hash_uint( (EcUInt)(EcPointerInteger)EC_PRIMITIVE(obj) +
									 _ec_hash( (EC_OBJ)EC_PRIMITIVEUSERDATA(obj), recur ) ); 		/* ???? Correct ???? */
			else
				return ec_hash_uint( (EcUInt)(EcPointerInteger) EC_PRIMITIVE(obj) ); 				/* ???? Correct ???? */
		}
		else
			return ec_hash_uint( (EcUInt)(EcPointerInteger)EC_PRIMITIVE(obj) +
								 (EcUInt)(EcPointerInteger)EC_PRIMITIVEUSERDATA(obj) );				/* ???? Correct ???? */
		break;

	case tc_cmethod:
		return ec_hash_uint( (EcUInt)(EcPointerInteger) EC_CMETHOD(obj) );							/* ???? Correct ???? */
		break;

	default:
		if (EC_USERP(obj))
		{
			/* if (USERTYPE(EC_TYPE(obj)).hash_fcn) */
			return (USERTYPE(EC_TYPE(obj)).hash_fcn( obj, recur ));
			/* return 13; */
		}

		ASSERT( FALSE );
		return 0;
		break;
	}

	ASSERT( FALSE );
	return 0;
}
