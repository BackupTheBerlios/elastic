/* ==========================================================================
 * list.h
 * ==========================================================================
 * Simple list of keyed objects. Keys and data are of type EcAny.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat Dec  4 10:01:34 MET 1999
 *
 *   $Id: list.h,v 1.1 2002/05/23 21:17:23 panta Exp $
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

#ifndef __LIST_H
#define __LIST_H

#include <elastic/basic.h>

EC_BEGIN_DECLS

typedef struct ec_list_struct          *ec_list;
typedef struct ec_list_node_struct     *ec_list_node;
typedef struct ec_list_iterator_struct *ec_list_iterator;


EC_API ec_list          ec_list_create( void );
EC_API void             ec_list_destroy( ec_list list );
EC_API ec_list          ec_list_copy( ec_list list );

EC_API ec_list_node     ec_list_head( ec_list list );
EC_API ec_list_node     ec_list_tail( ec_list list );

EC_API ec_list_node     ec_list_append( ec_list list, EcAny key, EcAny data );
EC_API ec_list_node     ec_list_prepend( ec_list list, EcAny key, EcAny data );
EC_API ec_list          ec_list_concat( ec_list list1, ec_list list2 );
EC_API ec_list          ec_list_remove( ec_list list, ec_list_node node );

EC_API ec_list_node     ec_list_find( ec_list list, EcAny key );
EC_API EcBool           ec_list_finddata( ec_list list, EcAny key, EcAny *data );

EC_API EcAny            ec_list_key( ec_list_node node );
EC_API EcAny            ec_list_data( ec_list_node node );

EC_API ec_list_iterator ec_list_iterator_create( ec_list list );
EC_API void             ec_list_iterator_destroy( ec_list_iterator iterator );
EC_API void             ec_list_iterator_rewind( ec_list_iterator iterator );
EC_API ec_list_node     ec_list_iterator_next( ec_list_iterator iterator );

EC_END_DECLS

#endif /* __LIST_H */
