/* ==========================================================================
 * compiled.c
 * ==========================================================================
 * Compiled bytecode.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat May 23 11:02:52 MEST 1998
 *
 *   $Id: compiled.c,v 1.2 2002/05/25 19:37:45 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1998-2001 Marco Pantaleoni. All rights reserved.
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

#define EC_STACK_RECYCLE_OPS 1
#include "basic.h"
#include "elastic.h"
#include "debug.h"
#include "private.h"
#include "compat.h"

#include "hashfunc.h"
#include "stackrecycle.h"


EC_API EcInt tc_compiled = -1;

static EC_OBJ compiled_copy( EC_OBJ obj, EcCopyType type );

static void   compiled_mark( EC_OBJ obj );
static void   compiled_free( EC_OBJ obj );
static EcInt  compiled_print( ec_string *str, EC_OBJ obj, EcBool detailed );

static EC_OBJ compiled_getitem( EC_OBJ obj, EC_OBJ index );
static EC_OBJ compiled_setitem( EC_OBJ obj, EC_OBJ index, EC_OBJ value );
static EC_OBJ compiled_length( EC_OBJ obj );

static EcUInt compiled_hash( EC_OBJ obj, EcInt recur );
static EcBool compiled_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res );

static EcBool compiled_check( EC_OBJ obj );

/* C API */

EC_API EC_OBJ EcMakeCompiled( EC_OBJ package, const char *name, EcInt nargs, EcInt nargs_def, EcBool varargs, EcBool ismethod )
{
	EC_OBJ obj;

	obj = EcMakeUser( tc_compiled, NULL );
	if (EC_ERRORP(obj)) return obj;

	EC_COMPILED(obj) = ec_malloc( sizeof(EcCompiled) );
	if (! EC_COMPILED(obj)) return Ec_ERROR;

	EC_COMPILEDCODE(obj)     = NULL;
	EC_COMPILEDNCODE(obj)    = 0;
	EC_COMPILEDNARG(obj)     = nargs;
	EC_COMPILEDNARG_DEF(obj) = nargs_def;
	EC_COMPILEDVARG(obj)     = varargs ? 1 : 0;
	EC_COMPILEDNLOC(obj)     = 0;
	EC_COMPILEDMAXTEMPS(obj) = 0;
	EC_COMPILEDLEXICAL(obj)  = EC_NIL;
	EC_COMPILEDLFRAME(obj)   = EcMakeArray( 0 );
	EC_COMPILEDHANDLER(obj)  = EcMakeArray( 0 );
	EC_COMPILEDPACKAGE(obj)  = package;

	if (name && index(name, '.'))
	{
		EC_COMPILEDNAME(obj) = EcMakeString( name, (EcInt) strlen( name ) );
	} else {
		ec_string ds;

		ec_string_init( &ds, NULL );
		ec_sprintf( &ds, "%w.%s", EC_PACKAGENAME(package), name ? name : "$ANONYMOUS$" );
/*		EC_COMPILEDNAME(obj)     = name ? EcMakeString( name, 0 ) : EC_NIL;*/
		EC_COMPILEDNAME(obj)     = EcMakeString( ec_strdata( &ds ), ec_strlen( &ds ) );
		ec_string_cleanup( &ds );
	}
	EC_COMPILEDISMETHOD(obj) = ismethod;

	EC_COMPILEDINFO(obj) = EcMakeHash();
	if (EC_NULLP(EC_COMPILEDINFO(obj)))
	{
		ec_free(EC_COMPILED(obj));
		return Ec_ERROR;
	}
	EcHashSet( EC_COMPILEDINFO(obj),							/* line number info */
			   EcMakeSymbolFromId( PRIVATE(lineID) ),
			   EcMakeArray( 4 ) );

	return obj;
}

#define BCQUANTUM 16

EC_API EcInt EcCompiledPush( EC_OBJ compiled, EcBytecode bc )
{
    EcInt size = 0, newSize;

/*	ASSERT( EC_COMPILEDP(compiled) );
	ASSERT( EC_COMPILED(compiled) );*/

    if (EC_COMPILEDNCODE(compiled) != 0) {
        size  = EC_COMPILEDNCODE(compiled) + BCQUANTUM;
        size /= BCQUANTUM;
        size *= BCQUANTUM;
	}
	
	newSize  = EC_COMPILEDNCODE(compiled) + 1 + BCQUANTUM;
	newSize /= BCQUANTUM;
	newSize *= BCQUANTUM;

	if ((newSize != size) || (! EC_COMPILEDCODE(compiled))) {
		EcBytecode *newCode;
		
		newCode = (EcBytecode *) ec_realloc( EC_COMPILEDCODE(compiled), newSize * sizeof(EcBytecode) );
		if (! newCode) return -1;
		
		EC_COMPILEDCODE(compiled) = newCode;
	}

	EC_COMPILEDCODE(compiled)[EC_COMPILEDNCODE(compiled)] = bc;
	return EC_COMPILEDNCODE(compiled)++;
}

/* elastiC API */

EC_API EcBool EcCompiledSetLine( EC_OBJ compiled, EcUInt bytecode_idx, EcInt line_num )
{
	EC_OBJ info, line_array, line_tp;

	info = EC_COMPILEDINFO(compiled);
	if (EC_NULLP(info)) return FALSE;

	ASSERT( EC_HASHP(info) );

	if (! EcHashGet( info, EcMakeSymbolFromId( PRIVATE(lineID) ), &line_array ))
		return FALSE;
	line_tp = EcMakeArrayInit( 2,
							   EcMakeInt( bytecode_idx ),
							   EcMakeInt( line_num ) );
	if (EC_ERRORP(line_tp)) return FALSE;
	if (EcArrayPush( line_array, line_tp ) < 0)
		return FALSE;
	return TRUE;
}

EC_API EcInt EcCompiledGetLine( EC_OBJ compiled, EcUInt bytecode_idx )
{
	EC_OBJ info, line_array, line_tp;
	EcInt len, i;
	EcInt bc_idx, l_num;
	EcInt line_num;

	info = EC_COMPILEDINFO(compiled);
	if (EC_NULLP(info)) return -1;

	ASSERT( EC_HASHP(info) );

	if (! EcHashGet( info, EcMakeSymbolFromId( PRIVATE(lineID) ), &line_array ))
		return -1;
	len = EC_ARRAYLEN(line_array);

	line_num = -1;
	for (i = 0; i < len; i++)
	{
		line_tp = EcArrayGet( line_array, i );

		bc_idx = EC_INUM(EcArrayGet( line_tp, 0 ));
		l_num  = EC_INUM(EcArrayGet( line_tp, 1 ));

		if ((EcUInt)bc_idx <= bytecode_idx)
			line_num = l_num;
		else
			break;
	}
	return line_num;
}

/* Private */

static EC_OBJ compiled_copy( EC_OBJ obj, EcCopyType type )
{
	EC_OBJ res, copy;
	EcBytecode *code;
	EcInt       size;
	EcInt i;

	ASSERT( EC_COMPILEDP(obj) );

	ASSERT( EC_NULLP(EC_COMPILEDNAME(obj)) || EC_STRINGP(EC_COMPILEDNAME(obj)) );
	res = EcMakeCompiled( EC_COMPILEDPACKAGE(obj),
						  EC_NNULLP(EC_COMPILEDNAME(obj)) ? EC_STRDATA(EC_COMPILEDNAME(obj)) : NULL,		/* EcMakeCompiled will strip leading package & '.' */
						  EC_COMPILEDNARG(obj),
						  EC_COMPILEDNARG_DEF(obj),
						  EC_COMPILEDVARG(obj),
						  EC_COMPILEDISMETHOD(obj) );
	if (EC_ERRORP(res)) return res;

	size  = EC_COMPILEDNCODE(obj) + BCQUANTUM;
	size /= BCQUANTUM;
	size *= BCQUANTUM;
	code = (EcBytecode *) ec_realloc( EC_COMPILEDCODE(res), size * sizeof(EcBytecode) );
	if (! code)
		return EcMemoryError();
	EC_COMPILEDCODE(res) = code;
	for (i = 0; i < EC_COMPILEDNCODE(obj); i++)
		code[i] = EC_COMPILEDCODE(obj)[i];
	EC_COMPILEDNCODE(res)    = EC_COMPILEDNCODE(obj);
	EC_COMPILEDNLOC(res)     = EC_COMPILEDNLOC(obj);
	EC_COMPILEDMAXTEMPS(res) = EC_COMPILEDMAXTEMPS(obj);

	if (type == EcShallowCopyType) 
		copy = EC_COMPILEDLEXICAL(obj);
	else
	{
		copy = EcCopy( EC_COMPILEDLEXICAL(obj), EcDeepCopyType );
		if (EC_ERRORP(copy)) return copy;
	}
	EC_COMPILEDLEXICAL(res) = copy;
#if EC_STACK_RECYCLE
	if (EC_STACKP(EC_COMPILEDLEXICAL(res)))
		EC_STACKREF_INC(EC_COMPILEDLEXICAL(res));
#endif

	if (type == EcShallowCopyType) 
		copy = EC_COMPILEDLFRAME(obj);
	else
	{
		copy = EcCopy( EC_COMPILEDLFRAME(obj), EcDeepCopyType );
		if (EC_ERRORP(copy)) return copy;
	}
	EC_COMPILEDLFRAME(res) = copy;

	if (type == EcShallowCopyType)
		copy = EC_COMPILEDHANDLER(obj);
	else
	{
		copy = EcCopy( EC_COMPILEDHANDLER(obj), EcDeepCopyType );
		if (EC_ERRORP(copy)) return copy;
	}
	EC_COMPILEDHANDLER(res) = copy;

	if (type == EcShallowCopyType) 
		copy = EC_COMPILEDINFO(obj);
	else
	{
		copy = EcCopy( EC_COMPILEDINFO(obj), EcDeepCopyType );
		if (EC_ERRORP(copy)) return copy;
	}
	EC_COMPILEDINFO(res) = copy;

	return res;
}

static void compiled_mark( EC_OBJ obj )
{
	ASSERT( EC_COMPILEDP(obj) );

	if (EC_NNULLP(EC_COMPILEDLEXICAL(obj))) EcMarkObject( EC_COMPILEDLEXICAL(obj) );
	if (EC_NNULLP(EC_COMPILEDLFRAME(obj)))  EcMarkObject( EC_COMPILEDLFRAME(obj) );
	if (EC_NNULLP(EC_COMPILEDHANDLER(obj))) EcMarkObject( EC_COMPILEDHANDLER(obj) );
	if (EC_NNULLP(EC_COMPILEDPACKAGE(obj))) EcMarkObject( EC_COMPILEDPACKAGE(obj) );
	if (EC_NNULLP(EC_COMPILEDNAME(obj)))    EcMarkObject( EC_COMPILEDNAME(obj) );
	if (EC_NNULLP(EC_COMPILEDINFO(obj)))    EcMarkObject( EC_COMPILEDINFO(obj) );
}

static void compiled_free( EC_OBJ obj )
{
	ASSERT( EC_COMPILEDP(obj) );

	ec_free( EC_COMPILEDCODE(obj) );
	EC_COMPILEDCODE(obj)     = NULL;
	EC_COMPILEDNCODE(obj)    = 0;
	EC_COMPILEDNARG(obj)     = 0;
	EC_COMPILEDNARG_DEF(obj) = 0;
	EC_COMPILEDVARG(obj)     = FALSE;
	EC_COMPILEDNLOC(obj)     = 0;
	EC_COMPILEDMAXTEMPS(obj) = 0;
#if EC_STACK_RECYCLE
	if (EC_STACKP(EC_COMPILEDLEXICAL(obj)))
		EC_STACKREF_DEC(EC_COMPILEDLEXICAL(obj));
#endif
	EC_COMPILEDLEXICAL(obj)  = EC_NIL;
	EC_COMPILEDLFRAME(obj)   = EC_NIL;
	EC_COMPILEDHANDLER(obj)  = EC_NIL;
	EC_COMPILEDPACKAGE(obj)  = EC_NIL;
	EC_COMPILEDNAME(obj)     = EC_NIL;
	EC_COMPILEDISMETHOD(obj) = FALSE;
	EC_COMPILEDINFO(obj)     = EC_NIL;

	ec_free( EC_COMPILED(obj) );
	EC_COMPILED(obj) = NULL;
}

static EcInt compiled_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	EcInt res = 0;
	char *compname;

	ASSERT( EC_COMPILEDP(obj) );

	if (EC_STRINGP(EC_COMPILEDNAME(obj)))
		compname = EC_STRDATA(EC_COMPILEDNAME(obj));
	else
		compname = "$ANONYMOUS$";
	if (detailed)
	{
		if (EC_COMPILEDNARG_DEF(obj))
			res = ec_sprintf( str, "<bytecode `%s' req.args:%I (def: %I) nlocals:%ld maxtemps:%ld>",
							  compname,
							  EC_COMPILEDNARG(obj),
							  EC_COMPILEDNARG_DEF(obj),
							  (long)EC_COMPILEDNLOC(obj),
							  (long)EC_COMPILEDMAXTEMPS(obj) );
		else
			res = ec_sprintf( str, "<bytecode `%s' req.args:%I nlocals:%ld maxtemps:%ld>",
							  compname,
							  EC_COMPILEDNARG(obj),
							  (long)EC_COMPILEDNLOC(obj),
							  (long)EC_COMPILEDMAXTEMPS(obj) );
	}
	else
	{
		if (EC_COMPILEDNARG_DEF(obj))
			res = ec_sprintf( str, "<bytecode `%s' req.args:%I (def: %I)>",
							  compname,
							  EC_COMPILEDNARG(obj),
							  EC_COMPILEDNARG_DEF(obj) );
		else
			res = ec_sprintf( str, "<bytecode `%s' req.args:%I>",
							  compname,
							  EC_COMPILEDNARG(obj) );
	}

	return res;
}

static EC_OBJ compiled_getitem( EC_OBJ obj, EC_OBJ index )
{
	if (! EC_INUMP(index))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	if ((EC_INUM(index) < 0) || (EC_INUM(index) >= EC_COMPILEDNCODE(obj)))
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

	return EcMakeInt( EC_COMPILEDCODE(obj)[EC_INUM(index)] );
}

static EC_OBJ compiled_setitem( EC_OBJ obj, EC_OBJ index, EC_OBJ value )
{
	if (! EC_INUMP(index))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	if (! EC_INUMP(value))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	if ((EC_INUM(index) < 0) || (EC_INUM(index) >= (EC_COMPILEDNCODE(obj) + 1)))
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

	if (EC_INUM(index) == EC_COMPILEDNCODE(obj))
		EcCompiledPush( obj, (EcBytecode)EC_INUM(value) );
	else
		EC_COMPILEDCODE(obj)[EC_INUM(index)] = (EcBytecode)EC_INUM(value);
	return obj;
}

static EC_OBJ compiled_length( EC_OBJ obj )
{
	return EcMakeInt( EC_COMPILEDNCODE(obj) );
}

static EcUInt compiled_hash( EC_OBJ obj, EcInt recur )
{
	EcUInt res;

	ASSERT( EC_COMPILEDP(obj) );

	res = 0;
	if (! EC_COMPILED(obj))
		return res;
	res += ec_hash_uint( EC_COMPILEDNCODE(obj) );
	res += ec_hash_uint( EC_COMPILEDNARG(obj) );
	res += ec_hash_uint( EC_COMPILEDNARG_DEF(obj) );
	res += ec_hash_uint( EC_COMPILEDVARG(obj) );
	res += ec_hash_uint( EC_COMPILEDNLOC(obj) );
	res += ec_hash_uint( EC_COMPILEDMAXTEMPS(obj) );
	if (EC_COMPILED(obj))
		res += ec_hash_pjw_memory( (const char *)EC_COMPILEDCODE(obj), EC_COMPILEDNCODE(obj) * sizeof(EcBytecode) );
	res += _ec_hash( EC_COMPILEDLEXICAL(obj), recur );
	res += _ec_hash( EC_COMPILEDLFRAME(obj), recur );
	res += _ec_hash( EC_COMPILEDHANDLER(obj), recur );
	res += _ec_hash( EC_COMPILEDPACKAGE(obj), recur );
	res += _ec_hash( EC_COMPILEDNAME(obj), recur );
	res += ec_hash_uint( EC_COMPILEDISMETHOD(obj) );
	/* res += _ec_hash( EC_COMPILEDINFO(obj), recur ); */				/* TODO XXX is it right ??? */
	return res;
}

static EcBool compiled_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res )
{
	if (obj1 == obj2)
	{
		*res = 0;
		return TRUE;
	}

	if (! EC_COMPILEDP(obj2))
		return FALSE;

	*res = 0;
	if (((EC_COMPILED(obj1) && (! EC_COMPILED(obj2))) ||
		 ((! EC_COMPILED(obj1)) && EC_COMPILED(obj2))))
		return FALSE;
	if (! EC_COMPILED(obj1))
		return TRUE;
	if (((EC_COMPILEDCODE(obj1) && (! EC_COMPILEDCODE(obj2))) ||
		 ((! EC_COMPILEDCODE(obj1)) && EC_COMPILEDCODE(obj2))))
		return FALSE;
	if (EC_COMPILEDNCODE(obj1) != EC_COMPILEDNCODE(obj2))
		return FALSE;
	if (EC_COMPILEDNARG(obj1) != EC_COMPILEDNARG(obj2))
		return FALSE;
	if (EC_COMPILEDNARG_DEF(obj1) != EC_COMPILEDNARG_DEF(obj2))
		return FALSE;
	if (EC_COMPILEDVARG(obj1) != EC_COMPILEDVARG(obj2))
		return FALSE;
	if (EC_COMPILEDNLOC(obj1) != EC_COMPILEDNLOC(obj2))
		return FALSE;
	if (EC_COMPILEDMAXTEMPS(obj1) != EC_COMPILEDMAXTEMPS(obj2))
		return FALSE;
	if (memcmp( EC_COMPILEDCODE(obj1), EC_COMPILEDCODE(obj2), EC_COMPILEDNCODE(obj1) * sizeof(EcBytecode) ) != 0)
		return FALSE;
	if (! EcObjectEqual( EC_COMPILEDLEXICAL(obj1), EC_COMPILEDLEXICAL(obj2) ))
		return FALSE;
	if (! EcObjectEqual( EC_COMPILEDLFRAME(obj1), EC_COMPILEDLFRAME(obj2) ))
		return FALSE;
	if (! EcObjectEqual( EC_COMPILEDHANDLER(obj1), EC_COMPILEDHANDLER(obj2) ))
		return FALSE;
	if (! EcObjectEqual( EC_COMPILEDPACKAGE(obj1), EC_COMPILEDPACKAGE(obj2) ))
		return FALSE;
	if (! EcObjectEqual( EC_COMPILEDNAME(obj1), EC_COMPILEDNAME(obj2) ))
		return FALSE;
	if (EC_COMPILEDISMETHOD(obj1) != EC_COMPILEDISMETHOD(obj2))
		return FALSE;
#if 0
	if (! EcObjectEqual( EC_COMPILEDINFO(obj1), EC_COMPILEDINFO(obj2) )) /* TODO XXX is it right ??? */
		return FALSE;
#endif
	return TRUE;

	return FALSE;
}

static EcBool compiled_check( EC_OBJ obj )
{
/*
	ASSERTMEM( EC_COMPILED(obj) );
	ASSERTCOND( (EC_COMPILEDNCODE(obj) == 0) || EC_COMPILEDCODE(obj) );
	ASSERTCOND( EC_COMPILEDNCODE(obj) >= 0 );
	ASSERTCOND( EC_COMPILEDNARG(obj) >= 0 );
	ASSERTCOND( EC_COMPILEDNARG_DEF(obj) >= 0 );
	ASSERTCOND( EC_COMPILEDVARG(obj) >= 0 );
	ASSERTCOND( EC_COMPILEDNLOC(obj) >= 0 );
	ASSERTCOND( EC_COMPILEDMAXTEMPS(obj) >= 0 );
	ASSERTFIELDN( EC_COMPILEDLEXICAL(obj), tc_stack );
	ASSERTFIELD( EC_COMPILEDLFRAME(obj), tc_array );
	ASSERTFIELDN( EC_COMPILEDHANDLER(obj), tc_array );
	ASSERTFIELDN( EC_COMPILEDPACKAGE(obj), tc_package );
	ASSERTFIELDN( EC_COMPILEDNAME(obj), tc_string );
	ASSERTFIELDN( EC_COMPILEDINFO(obj), tc_hash );
*/
	return TRUE;
}

EcBool _ec_compiled_init( void )
{
	EcSequenceCallbacks sequence_cb = {
		/* getitem */		compiled_getitem,
		/* setitem */		compiled_setitem,
		/* delitem */		NULL,
		/* length  */		compiled_length
	};

	EcTypespec compiledspec = {
		/* type    */		0,
		/* name    */		"bytecode",

		/* copy    */		compiled_copy,

		/* mark    */		compiled_mark,
		/* free    */		compiled_free,
		/* print   */		compiled_print,

		/* hash    */		compiled_hash,
		/* compare */		compiled_compare,

		/* check   */		compiled_check,

		/* sequence */      &sequence_cb,
		/* numeric  */      NULL
	};

	tc_compiled = EcAddType( &compiledspec );
	if (tc_compiled == 0)
		return FALSE;

	return TRUE;
}

void _ec_compiled_cleanup( void )
{
}
