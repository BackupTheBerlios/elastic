/* ==========================================================================
 * hashfunc.h
 * ==========================================================================
 * Hashing routines.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Tue May  6 14:22:25 MET DST 1997
 *
 *   $Id: hashfunc.h,v 1.2 2002/05/31 13:03:31 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1997-2002 Marco Pantaleoni. All rights reserved.
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

#ifndef __HASHFUNC_H
#define __HASHFUNC_H

#include <elastic/basic.h>

EC_BEGIN_DECLS

EC_API EcUInt ec_hash_pjw_string( const char *key );
EC_API EcUInt ec_hash_pjw_memory( const char *key, EcUInt count );
EC_API EcUInt ec_hash_pjw_memory_r(const char *key, EcUInt count);
EC_API EcUInt ec_hash_djb_string( const char *key );
EC_API EcUInt ec_hash_djb_memory( const char *key, EcUInt count );
EC_API EcUInt ec_hash_djb_memory_r(const char *key, EcUInt count);
EC_API EcUInt ec_hash_sdbm_string( const char *key );
EC_API EcUInt ec_hash_sdbm_memory( const char *key, EcUInt count );
EC_API EcUInt ec_hash_sdbm_memory_r(const char *key, EcUInt count);
EC_API EcUInt ec_hash_two_string( const char *key );
EC_API EcUInt ec_hash_two_memory( const char *key, EcUInt count );
EC_API EcUInt ec_hash_two_memory_r( const char *key, EcUInt count );

#define ec_hash_uint(key)		((key))
#define ec_hash_two_uint(key)	(~(key))

EC_API EcUInt ec_hash_float( EcFloat key );
EC_API EcUInt ec_hash_two_float( EcFloat key );

EC_END_DECLS

#endif /* __HASHFUNC_H */
