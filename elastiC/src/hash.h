/* ==========================================================================
 * hash.h
 * ==========================================================================
 * elastiC hash table.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Wed Jun 23 20:12:45 MEST 1999
 *
 *   $Id: hash.h,v 1.1 2002/05/23 21:17:06 panta Exp $
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

#ifndef __HASH_H
#define __HASH_H 1

#include <elastic/elastic.h>

EC_BEGIN_DECLS

extern EC_API EcInt tc_hash;


/* ========================================================================
 * M A C R O S
 * ======================================================================== */

#define EC_HASHP(obj)			(EC_TYPEP(obj,tc_hash))
#define EC_HASH(obj)			((ec_hash) EC_USER(obj))
#define EC_HASH_SET(obj,val)	do { EC_USER(obj) = (ec_hash)(val); } while (0)


/* ========================================================================
 * P R O T O T Y P E S
 * ======================================================================== */

/* C API */

EC_API EC_OBJ EcMakeHash( void );

/* elastiC API */

EC_API EC_OBJ EcLibHash_HasKey( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibHash_Keys( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibHash_Values( EC_OBJ stack, EcAny userdata );

EC_END_DECLS

#endif /* __HASH_H */
