/* ==========================================================================
 * hash.c
 * ==========================================================================
 * elastiC hash table.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Wed Jun 23 20:12:45 MEST 1999
 *
 *   $Id: hash.c,v 1.1 2002/05/23 21:20:19 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1999-2001 Marco Pantaleoni. All rights reserved.
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
#include "debug.h"

#include "elastic.h"

EC_API EcInt tc_hash = -1;

static EC_OBJ hash_copy( EC_OBJ obj, EcCopyType type );
static void   hash_mark( EC_OBJ obj );
static void   hash_free( EC_OBJ obj );
static EcInt  hash_print( ec_string *str, EC_OBJ obj, EcBool detailed );
static EC_OBJ hash_getitem( EC_OBJ obj, EC_OBJ index );
static EC_OBJ hash_setitem( EC_OBJ obj, EC_OBJ index, EC_OBJ value );
static EC_OBJ hash_delitem( EC_OBJ obj, EC_OBJ index );
static EC_OBJ hash_length( EC_OBJ obj );
static EcUInt hash_hash( EC_OBJ obj, EcInt recur );
static EcBool hash_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res );

/* C API */

EC_API EC_OBJ EcMakeHash( void )
{
	EC_OBJ obj;
	ec_hash hash;

	hash = ec_hash_create( 0, 0, 0, &ec_hash_obj2obj );
	if (! hash) return Ec_ERROR;

	obj = EcMakeUser( tc_hash, hash );
	if (EC_ERRORP(obj)) return obj;

	return obj;
}

EC_API EcBool EcHashGet( EC_OBJ hash, EC_OBJ key, EC_OBJ *value )
{
	EcAny val;

	if (! ec_hash_get( EC_HASH(hash), (EcAny)key, &val ))
		return FALSE;

	*value = val;
	return TRUE;
}

EC_API EcBool EcHashSet( EC_OBJ hash, EC_OBJ key, EC_OBJ value )
{
	if (ec_hash_set( EC_HASH(hash), (EcAny)key, (EcAny)value ))
		return TRUE;
	return FALSE;
}

EC_API EcBool EcHashDel( EC_OBJ hash, EC_OBJ key )
{
	return ec_hash_del( EC_HASH(hash), (EcAny)key );
}

EC_API EcBool EcHashHasKey( EC_OBJ hash, EC_OBJ key )
{
	return ec_hash_has_key( EC_HASH(hash), (EcAny)key );
}

EC_API EC_OBJ *EcHashKeys( EC_OBJ hash, EcInt *size )
{
	ec_hash_iterator  iter;
	EcAny             key;
	EcInt             nentries, i;

	EC_OBJ           *res;

	nentries = ec_hash_size( EC_HASH(hash) );
	if (size) *size = nentries;

	iter = ec_hash_iterator_create( EC_HASH(hash) );
	if (! iter)
		return NULL;

	res = ec_malloc( sizeof(EC_OBJ) * nentries );
	if (! res)
	{
		ec_hash_iterator_destroy( iter );
		return NULL;
	}

	i = 0;
	while (ec_hash_iterator_next( iter, &key ))
		res[i++] = (EC_OBJ) key;

	ec_hash_iterator_destroy( iter );
	return res;
}

EC_API EC_OBJ *EcHashValues( EC_OBJ hash, EcInt *size )
{
	ec_hash_iterator  iter;
	EcAny             key, value;
	EcInt             nentries, i;

	EC_OBJ           *res;

	nentries = ec_hash_size( EC_HASH(hash) );
	if (size) *size = nentries;

	iter = ec_hash_iterator_create( EC_HASH(hash) );
	if (! iter)
		return NULL;

	res = ec_malloc( sizeof(EC_OBJ) * nentries );
	if (! res)
	{
		ec_hash_iterator_destroy( iter );
		return NULL;
	}

	i = 0;
	while (ec_hash_iterator_next( iter, &key ))
	{
		ec_hash_get( EC_HASH(hash), key, &value );
		res[i++] = (EC_OBJ) value;
	}

	ec_hash_iterator_destroy( iter );
	return res;
}

/* elastiC API */

EC_API EC_OBJ EcLibHash_HasKey( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj, key;
	EC_OBJ res;

	res = EcParseStackFunction( "hash.has_key", TRUE, stack, "O!O", tc_hash, &obj, &key );
	if (EC_ERRORP(res))
		return res;

	return ec_hash_has_key( EC_HASH(obj), (EcAny) key ) ? EcTrueObject : EcFalseObject;
}

EC_API EC_OBJ EcLibHash_Keys( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ res;
	EC_OBJ keys;
	ec_hash_iterator iter;
	EcAny            key;

	res = EcParseStackFunction( "hash.keys", TRUE, stack, "O!", tc_hash, &obj );
	if (EC_ERRORP(res))
		return res;

	keys = EcMakeArray( 0 );
	if (EC_ERRORP(keys))
		return keys;

	iter = ec_hash_iterator_create( EC_HASH(obj) );
	if (!iter)
		return EcMemoryError();

	while (ec_hash_iterator_next( iter, &key ))
	{
		if (EcArrayPush( keys, (EC_OBJ) key ) < 0)
		{
			ec_hash_iterator_destroy( iter );
			return Ec_ERROR;
		}
	}

	ec_hash_iterator_destroy( iter );
	return keys;
}

EC_API EC_OBJ EcLibHash_Values( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ res;
	EC_OBJ values, value;
	ec_hash_iterator iter;
	EcAny            key;

	res = EcParseStackFunction( "hash.values", TRUE, stack, "O!", tc_hash, &obj );
	if (EC_ERRORP(res))
		return res;

	values = EcMakeArray( 0 );
	if (EC_ERRORP(values))
		return values;

	iter = ec_hash_iterator_create( EC_HASH(obj) );
	if (!iter)
		return EcMemoryError();

	while (ec_hash_iterator_next( iter, &key ))
	{
		ec_hash_get( EC_HASH(obj), (EcAny) key, (EcAny *) &value );
		if (EcArrayPush( values, value ) < 0)
		{
			ec_hash_iterator_destroy( iter );
			return Ec_ERROR;
		}
	}

	ec_hash_iterator_destroy( iter );
	return values;
}


/* Private */

static EC_OBJ hash_copy( EC_OBJ obj, EcCopyType type )
{
	EC_OBJ res;
	ec_hash_iterator iter;
	EcAny            key, value;

	ASSERT( EC_HASHP(obj) );

	res = EcMakeHash();
	if (EC_ERRORP(res)) return res;

	iter = ec_hash_iterator_create( EC_HASH(obj) );
	if (!iter)
		return EcMemoryError();

	if (type == EcShallowCopyType)
		while (ec_hash_iterator_next( iter, &key ))
		{
			ec_hash_get( EC_HASH(obj), key, &value );

			hash_setitem( res, (EC_OBJ) key, (EC_OBJ) value );
		}
	else
	{
		EC_OBJ copy;

		while (ec_hash_iterator_next( iter, &key ))
		{
			ec_hash_get( EC_HASH(obj), key, &value );

			copy = EcCopy( (EC_OBJ) value, EcDeepCopyType );
			if (EC_ERRORP(copy)) return copy;
			hash_setitem( res, (EC_OBJ) key, copy );
		}
	}

	ec_hash_iterator_destroy( iter );

	return res;
}

static void hash_mark( EC_OBJ obj )
{
	ec_hash_iterator iter;
	EcAny            key, value;

	ASSERT( EC_HASHP(obj) );

	iter = ec_hash_iterator_create( EC_HASH(obj) );
	if (!iter)
	{
		EcMemoryError();
		return;
	}

	while (ec_hash_iterator_next( iter, &key ))
	{
		ec_hash_get( EC_HASH(obj), key, &value );

		if (EC_NNULLP((EC_OBJ)key))   EcMarkObject( (EC_OBJ) key );
		if (EC_NNULLP((EC_OBJ)value)) EcMarkObject( (EC_OBJ) value );
	}

	ec_hash_iterator_destroy( iter );
}

static void hash_free( EC_OBJ obj )
{
	ASSERT( EC_HASHP(obj) );

	ec_hash_destroy( EC_HASH(obj) );
	EC_HASH_SET(obj, NULL);
}

static EcInt hash_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	ASSERT( EC_HASHP(obj) );

	if (detailed)
		return ec_sprintf( str, "<hash>" );
	else
		return ec_sprintf( str, "<hash>" );
}

static EC_OBJ hash_getitem( EC_OBJ obj, EC_OBJ index )
{
	EcAny value;

	ASSERT( EC_HASHP(obj) );

	if (! ec_hash_get( EC_HASH(obj), (EcAny)index, &value ))
	{
		ec_string ds;

		ec_string_init( &ds, NULL );
		ec_sprintf( &ds, "object %w has no key %w", obj, index );

		EcMakeExceptionWithArgs( EcIndexErrorClass,
								 ec_strdata( &ds ), 2,
								 index, obj );
		ec_string_cleanup( &ds );
		return Ec_ERROR;
	}

	return (EC_OBJ)value;
}

static EC_OBJ hash_setitem( EC_OBJ obj, EC_OBJ index, EC_OBJ value )
{
	ASSERT( EC_HASHP(obj) );

	if (! ec_hash_set( EC_HASH(obj), (EcAny)index, (EcAny)value ))
	{
		ec_string ds;

		ec_string_init( &ds, NULL );
		ec_sprintf( &ds, "can't set key %w for object %w", index, obj );

		EcMakeExceptionWithArgs( EcIndexErrorClass,
								 ec_strdata( &ds ), 2,
								 index, obj );
		ec_string_cleanup( &ds );
		return Ec_ERROR;
	}

	return obj;
}

static EC_OBJ hash_delitem( EC_OBJ obj, EC_OBJ index )
{
	ASSERT( EC_HASHP(obj) );

	ec_hash_del( EC_HASH(obj), (EcAny)index );
	return obj;
}

static EC_OBJ hash_length( EC_OBJ obj )
{
	ASSERT( EC_HASHP(obj) );

	return EcMakeInt( -1 );										/* not a real sequence */
}

static EcUInt hash_hash( EC_OBJ obj, EcInt recur )
{
	ASSERT( EC_HASHP(obj) );

	/* Equivalent object MUST have the same hash */
	return 1;													/* this is all we can do ... */
}

static EcBool hash_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res )
{
	if (obj1 == obj2)
	{
		*res = 0;
		return TRUE;
	}

	if (! EC_HASHP(obj2))
		return FALSE;

	return FALSE;
}

EcBool _ec_hash_init( void )
{
	EcSequenceCallbacks sequence_cb = {
		/* getitem */		hash_getitem,
		/* setitem */		hash_setitem,
		/* delitem */		hash_delitem,
		/* length  */		hash_length
	};

	EcTypespec hashspec = {
		/* type    */		0,
		/* name    */		"hash",

		/* copy    */		hash_copy,

		/* mark    */		hash_mark,
		/* free    */		hash_free,
		/* print   */		hash_print,

		/* hash    */		hash_hash,
		/* compare */		hash_compare,

		/* check   */		NULL,

		/* sequence */      &sequence_cb,
		/* numeric  */      NULL
	};
	EC_OBJ pkg;

	tc_hash = EcAddType( &hashspec );
	if (tc_hash == 0)
		return FALSE;

	pkg = EcPackageIntroduce( "hash" );
	EcAddPrimitive( "hash.has_key", EcLibHash_HasKey );
	EcAddPrimitive( "hash.keys",    EcLibHash_Keys );
	EcAddPrimitive( "hash.values",  EcLibHash_Values );

	return TRUE;
}

void _ec_hash_cleanup( void )
{
}
