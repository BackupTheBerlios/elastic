/* ==========================================================================
 * hashtable.h
 * ==========================================================================
 * Generic hash table.h
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Thu Jun 24 10:34:09 MEST 1999
 *
 *   $Id: hashtable.h,v 1.1 2002/05/23 21:17:09 panta Exp $
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

#ifndef __HASHTABLE_H
#define __HASHTABLE_H 1

#include <elastic/elastic.h>

EC_BEGIN_DECLS

typedef struct ec_hash_struct          *ec_hash;
typedef struct ec_hash_entry_struct    *ec_hash_entry;
typedef struct ec_hash_iterator_struct *ec_hash_iterator;

typedef EcUInt (*ec_lambda_hash)    ( EcAny key );
typedef EcAny  (*ec_lambda_copy)    ( EcAny obj );
typedef EcInt  (*ec_lambda_compare) ( EcAny obj1, EcAny obj2 );
typedef void   (*ec_lambda_destroy) ( EcAny obj );

typedef struct ec_hash_def
{
        ec_lambda_hash    key_hash;

        ec_lambda_copy    key_copy;
        ec_lambda_compare key_compare;
        ec_lambda_destroy key_destroy;

        ec_lambda_copy    value_copy;
        ec_lambda_compare value_compare;
        ec_lambda_destroy value_destroy;

        EcAny             invalid_key;
        EcAny             invalid_value;
} ec_hash_def;

struct ec_hash_entry_struct
{
	EcAny key;
	EcAny value;
};

#define EC_HASH_ENTRY_KEY(entry)		((entry)->key)
#define EC_HASH_ENTRY_VALUE(entry)		((entry)->value)


/* ========================================================================
 * P R O T O T Y P E S
 * ======================================================================== */

EC_API ec_hash ec_hash_create( EcUInt minSize, EcUInt loLoad, EcUInt hiLoad, ec_hash_def *def );
EC_API void    ec_hash_destroy( ec_hash hash );

EC_API EcBool  ec_hash_get_entry( ec_hash hash, EcAny key, ec_hash_entry *entry );
EC_API EcBool  ec_hash_get( ec_hash hash, EcAny key, EcAny *value );
EC_API EcBool  ec_hash_set( ec_hash hash, EcAny key, EcAny value );
EC_API EcBool  ec_hash_del( ec_hash hash, EcAny key );
EC_API EcBool  ec_hash_has_key( ec_hash hash, EcAny key );
EC_API EcInt   ec_hash_size( ec_hash hash );
/*
EC_API EcBool  ec_hash_valid_key( ec_hash hash, EcAny key );
EC_API EcBool  ec_hash_valid_value( ec_hash hash, EcAny value );
*/

EC_API ec_hash_iterator ec_hash_iterator_create( ec_hash hash );
EC_API void             ec_hash_iterator_destroy( ec_hash_iterator iterator );
EC_API void             ec_hash_iterator_first( ec_hash_iterator iterator );
EC_API EcBool           ec_hash_iterator_next( ec_hash_iterator iterator, EcAny *key );

EC_END_DECLS

#endif /* __HASHTABLE_H */
