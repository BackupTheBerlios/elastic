/* ==========================================================================
 * utility.h
 * ==========================================================================
 * Utility functions & structures.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Tue Feb 24 10:03:50 MET 1998
 *
 *   $Id: utility.h,v 1.1 2002/05/23 21:17:42 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1997-2000 Marco Pantaleoni. All rights reserved.
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

#ifndef __UTILITY_H
#define __UTILITY_H

#include <elastic/basic.h>

EC_BEGIN_DECLS

typedef struct
{
	EcUInt *mem;
	EcUInt *last;
	EcUInt *head, *tail;
	EcUInt size;
} ec_arena, ec_stack, ec_queue;

EC_API EcBool ec_arena_create( ec_arena *arena, EcUInt initial_size );
EC_API void   ec_arena_destroy( ec_arena *arena );
EC_API EcUInt ec_arena_get( ec_arena *arena, EcUInt at );
EC_API void   ec_arena_set( ec_arena *arena, EcUInt at, EcUInt x );

EC_API EcBool ec_stack_empty( ec_stack *stack );
EC_API EcBool ec_stack_push(  ec_stack *stack, EcUInt x );
EC_API EcUInt ec_stack_pop(   ec_stack *stack );
EC_API EcBool ec_queue_empty(   ec_queue *queue );
EC_API EcBool ec_queue_enqueue( ec_queue *queue, EcUInt x );
EC_API EcUInt ec_queue_dequeue( ec_queue *queue );

EC_API const char *ec_string_sub( char *dst, const char *str, EcInt start, EcInt len );
EC_API const char *ec_string_tokenize( const char **sepp, EcInt *tokenlen, char delim );

EC_END_DECLS

#endif /* __UTILITY_H */
