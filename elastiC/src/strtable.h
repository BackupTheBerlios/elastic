/* ==========================================================================
 * strtable.h
 * ==========================================================================
 * String table.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat May 23 17:02:16 MEST 1998
 *
 *   $Id: strtable.h,v 1.1 2002/05/23 21:17:40 panta Exp $
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

#ifndef __STRTABLE_H
#define __STRTABLE_H

#include <elastic/basic.h>

EC_BEGIN_DECLS

typedef struct _strtable ec_strtable;

/* ========================================================================
 * A P I
 * ======================================================================== */

EC_API ec_strtable *ec_strtable_create( EcInt nslots );
EC_API void        ec_strtable_destroy( ec_strtable *table );

EC_API EcBool ec_strtable_set( ec_strtable *table, const char *key, EcUInt  value );
EC_API EcBool ec_strtable_get( ec_strtable *table, const char *key, EcUInt *value );

EC_END_DECLS

#endif /* __STRTABLE_H */
