/* ==========================================================================
 * hashdefs.c
 * ==========================================================================
 * Various hash definition structures.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat Jul 17 17:33:49 MEST 1999
 *
 *   $Id: hashdefs.c,v 1.2 2002/05/25 19:37:44 panta Exp $
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

#include "basic.h"
#include "elastic.h"
#include "compat.h"

static EcUInt hash_int( EcAny key );

static EcInt  object_compare( EcAny obj1, EcAny obj2 );

static EcAny  string_copy( EcAny obj );
static EcInt  string_compare( EcAny obj1, EcAny obj2 );
static void   string_destroy( EcAny obj );

/* EcInt -> EcInt */

EC_API ec_hash_def ec_hash_int2int =
{
	(ec_lambda_hash)    hash_int,

	(ec_lambda_copy)    NULL,
	(ec_lambda_compare) NULL,
	(ec_lambda_destroy) NULL,

	(ec_lambda_copy)    NULL,
	(ec_lambda_compare) NULL,
	(ec_lambda_destroy) NULL,

	(EcAny)             0xFFFFFFFF,
	(EcAny)             0xFFFFFFFF
};

/* EC_OBJ -> EC_OBJ */

EC_API ec_hash_def ec_hash_obj2obj =
{
	(ec_lambda_hash)    EcHash,

	(ec_lambda_copy)    NULL,
	(ec_lambda_compare) object_compare,
	(ec_lambda_destroy) NULL,

	(ec_lambda_copy)    NULL,
	(ec_lambda_compare) object_compare,
	(ec_lambda_destroy) NULL,

	(EcAny)             0xFFFFFFFF,
	(EcAny)             0xFFFFFFFF
};

/* EC_OBJ -> string */

EC_API ec_hash_def ec_hash_obj2string =
{
	(ec_lambda_hash)    EcHash,

	(ec_lambda_copy)    NULL,
	(ec_lambda_compare) object_compare,
	(ec_lambda_destroy) NULL,

	(ec_lambda_copy)    string_copy,
	(ec_lambda_compare) string_compare,
	(ec_lambda_destroy) string_destroy,

	(EcAny)             0xFFFFFFFF,
	(EcAny)             0xFFFFFFFF
};


static EcUInt hash_int( EcAny key )
{
	return (EcUInt)((EcPointerInteger) key);
}

static EcInt object_compare( EcAny obj1, EcAny obj2 )
{
	EcInt res;

	if (obj1 == obj2)
		return 0;
	if (EcObjectCompare( (EC_OBJ)obj1, (EC_OBJ)obj2, &res ))
		return res;
	return 1;
}

static EcAny string_copy( EcAny obj )
{
	return (EcAny)ec_stringdup( (const char *)obj );
}

static EcInt string_compare( EcAny obj1, EcAny obj2 )
{
	if (obj1 == obj2)
		return 0;
	if (! obj1) return 1;
	if (! obj2) return 1;
	return (EcInt)strcmp( (const char *)obj1, (const char *)obj2 );
}

static void string_destroy( EcAny obj )
{
	ec_free( (void *)obj );
}
