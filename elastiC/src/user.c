/* ==========================================================================
 * user.c
 * ==========================================================================
 * User defined types handling.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat Dec 12 11:39:17 MET 1998
 *
 *   $Id: user.c,v 1.2 2002/05/25 19:37:44 panta Exp $
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

#include <stdlib.h>

#include "basic.h"
#include "debug.h"

#include "elastic.h"
#include "compat.h"

#include "private.h"


static EC_OBJ default_copy( EC_OBJ obj, EcCopyType type );

static void   default_mark( EC_OBJ obj );
static void   default_free( EC_OBJ obj );
static EcInt  default_print( ec_string *str, EC_OBJ obj, EcBool detailed );

static EcUInt default_hash( EC_OBJ obj, EcInt recursion_level );


EC_API EcInt EcAddType( EcTypespec *newtype )
{
	EcInt       newtypecode;
	EcTypespec *newspec;

	ASSERT( newtype );

	newtype->type = 0;

	newtypecode = PRIVATE(usertypes) + 1;

	newspec = ec_realloc( PRIVATE(typespec), (newtypecode + 1) * sizeof(struct EcTypespec) );
	if (! newspec)
		return 0;

	newtype->type = newtypecode;
	PRIVATE(typespec) = newspec;

	memcpy( &USERTYPE(newtypecode), newtype, sizeof(struct EcTypespec) );
	USERTYPE(newtypecode).name = NULL;
	if (newtype->name)
	{
		USERTYPE(newtypecode).name = ec_stringdup( newtype->name );
		if (! USERTYPE(newtypecode).name)
		{
			memset( &USERTYPE(newtypecode), 0x00, sizeof(struct EcTypespec) );
			newtype->type = 0;
			return 0;
		}
	}
	if (newtype->sequence_cb)
	{
		USERTYPE(newtypecode).sequence_cb = ec_malloc( sizeof(EcSequenceCallbacks) );
		if (! USERTYPE(newtypecode).sequence_cb)
		{
			memset( &USERTYPE(newtypecode), 0x00, sizeof(struct EcTypespec) );
			newtype->type = 0;
			return 0;
		}
		memcpy( USERTYPE(newtypecode).sequence_cb, newtype->sequence_cb, sizeof(EcSequenceCallbacks) );
	}
	if (newtype->numeric_cb)
	{
		USERTYPE(newtypecode).numeric_cb = ec_malloc( sizeof(EcNumericCallbacks) );
		if (! USERTYPE(newtypecode).numeric_cb)
		{
			if (USERTYPE(newtypecode).sequence_cb) ec_free( USERTYPE(newtypecode).sequence_cb );
			memset( &USERTYPE(newtypecode), 0x00, sizeof(struct EcTypespec) );
			newtype->type = 0;
			return 0;
		}
		memcpy( USERTYPE(newtypecode).numeric_cb, newtype->numeric_cb, sizeof(EcNumericCallbacks) );
	}

	if (! USERTYPE(newtypecode).copy_fcn)    USERTYPE(newtypecode).copy_fcn    = default_copy;
	if (! USERTYPE(newtypecode).mark_fcn)    USERTYPE(newtypecode).mark_fcn    = default_mark;
	if (! USERTYPE(newtypecode).free_fcn)    USERTYPE(newtypecode).free_fcn    = default_free;
	if (! USERTYPE(newtypecode).print_fcn)   USERTYPE(newtypecode).print_fcn   = default_print;
	if (! USERTYPE(newtypecode).hash_fcn)    USERTYPE(newtypecode).hash_fcn    = default_hash;

	PRIVATE(usertypes)++;

	return newtypecode;
}

static EC_OBJ default_copy( EC_OBJ obj, EcCopyType type )
{
	/* If a sequence, try to copy construct using what we now */
	/*
	 * TODO: to do this we *need* generic construction.
	 *       Also generic construction should support 0 parameters,
	 *       to allow later building (no initial parameters to constructor)
	 */

	/* We don't know how to copy, by default */
	return EC_NIL;
}

static void default_mark( EC_OBJ obj )
{
}

static void default_free( EC_OBJ obj )
{
}

static EcInt default_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	return ec_sprintf( str, "<%s>", EcObjectTypeName( obj ) );
}

#define DEFAULT_HASH_VALUE	13

static EcUInt default_hash( EC_OBJ obj, EcInt recursion_level )
{
	/* Equivalent objects MUST have equivalent hash values ! */
	return DEFAULT_HASH_VALUE;
}
