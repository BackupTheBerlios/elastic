/* ==========================================================================
 * stackrecycle.h
 * ==========================================================================
 * Stack recycling macros and functions.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Wed Oct 17 11:42:08 CEST 2001
 *
 *   $Id: stackrecycle.h,v 1.1 2002/05/23 21:17:43 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 2001-2002 Marco Pantaleoni. All rights reserved.
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

#ifndef __STACKRECYCLE_H
#define __STACKRECYCLE_H

#if EC_STACK_RECYCLE

#include <elastic/basic.h>
#include <elastic/debug.h>
#include <elastic/elastic.h>
#include <elastic/bitstring.h>
#include <elastic/private.h>

#ifdef EC_STACK_RECYCLE_OPS

static inline void _ec_stackref_dec(EC_OBJ obj);
#if ! EC_STACK_RECYCLE_INLINE_PUT
static void _ec_recycle_stack_put(EC_OBJ stack);
#endif

#if EC_STACK_RECYCLE_STATS
#define STKSTAT_INC_R()		do { PRIVATE(n_ref_inc)++; } while (0)
#define STKSTAT_DEC_R()		do { PRIVATE(n_ref_dec)++; } while (0)
#define STKSTAT_INC_V(var)	do { PRIVATE(var)++; } while (0)
#define STKSTAT_DEC_V(var)	do { PRIVATE(var)--; } while (0)
#else /* EC_STACK_RECYCLE_STATS */
#define STKSTAT_INC_R()		do { } while (0)
#define STKSTAT_DEC_R()		do { } while (0)
#define STKSTAT_INC_V(var)	do { } while (0)
#define STKSTAT_DEC_V(var)	do { } while (0)
#endif /* ! EC_STACK_RECYCLE_STATS */

#if defined(_GNU_SOURCE) && (! defined(__STRICT_ANSI__))
#define EC_STACKREF_SET(obj, v) \
do {																							\
	EC_ASSERT(EC_STACKP(obj));																	\
	EC_STACKREF(obj) = (v);																		\
	EC_ASSERT(EC_STACKREF(obj) >= 0);															\
	EC_VADEBMESSAGE(EC_STACK_RECYCLE_DEBUG_MSGS,												\
					"set ref(0x%08lX) = %ld\n", (unsigned long)obj, (long)EC_STACKREF(obj));	\
} while (0);

#define EC_STACKREF_INC(obj) \
do {														 									\
	EC_ASSERT(EC_STACKP(obj));																	\
	EC_ASSERT(EC_STACKREF(obj) >= 0);															\
	EC_STACKREF(obj)++;															   				\
	STKSTAT_INC_R();															   				\
	EC_VADEBMESSAGE(EC_STACK_RECYCLE_DEBUG_MSGS,									   			\
					"++ref(0x%08lX) = %ld\n", (unsigned long)obj, (long)EC_STACKREF(obj));		\
} while (0)
#else
#define EC_STACKREF_SET(obj, v) \
do {																							\
	EC_ASSERT(EC_STACKP(obj));																	\
	EC_STACKREF(obj) = (v);																		\
	EC_ASSERT(EC_STACKREF(obj) >= 0);															\
} while (0);

#define EC_STACKREF_INC(obj) \
do {														 									\
	EC_ASSERT(EC_STACKP(obj));																	\
	EC_ASSERT(EC_STACKREF(obj) >= 0);															\
	EC_STACKREF(obj)++;															   				\
	STKSTAT_INC_R();															   				\
} while (0)
#endif

#if defined(_GNU_SOURCE) && (! defined(__STRICT_ANSI__))
#define RECYCLE_MSG(stack, first_free_slot) \
do {                                                                        \
	EC_VADEBMESSAGE(EC_STACK_RECYCLE_DEBUG_MSGS,                            \
					"RECYCLING STACK 0x%08lX (ffs: %d)\n",                  \
					(unsigned long)(stack), (first_free_slot));             \
} while (0)
#else
#define RECYCLE_MSG(stack, first_free_slot) do { } while (0)
#endif

	/* Eventually recycle a stack */
#define EC_RECYCLE_STACK_PUT(stack)											\
do																			\
{																			\
	EC_ASSERT(EC_STACKP(stack));	   										\
	EC_WARN_IF(TRUE, EC_STACKREF(stack) > 0);								\
																			\
	/* if (EC_STACKREF(stack) > 0)											\
	   STACK_DEC_REF(stack); */												\
																			\
	if (EC_STACKREF(stack) == 0)											\
	{																		\
		/* Recycle this stack */											\
																			\
		int first_free_slot;												\
																			\
		EC_ASSERT( EC_STACKREF(stack) == 0 );								\
																			\
		STKSTAT_INC_V(n_recycle_put_attempts);								\
																			\
		ec_bit_ffc(PRIVATE(stack_pool_bmap),								\
				   EC_STACK_POOL_SIZE,										\
				   &first_free_slot);										\
		if (first_free_slot >= 0)											\
		{																	\
			RECYCLE_MSG(stack, first_free_slot);							\
			PRIVATE(stack_pool)[first_free_slot] = stack;					\
			ec_bit_set(PRIVATE(stack_pool_bmap), first_free_slot);			\
																			\
			STKSTAT_INC_V(n_recycle_put);									\
																			\
			/* if (EC_STACKP(EC_STACKUP(stack))) {							\
			   STACK_DEC_REF(EC_STACKUP(stack));							\
			   } */															\
/*																			\
			if (EC_STACKP(EC_STACKIMMUP(stack))) {							\
			    STACK_DEC_REF(EC_STACKIMMUP(stack));						\
			}																\
*/																			\
			EC_STACKUP(stack)        = EC_NIL;								\
			if (EC_STACKP(EC_STACKLEXICAL(stack)))							\
			    _ec_stackref_dec(EC_STACKLEXICAL(stack));					\
			EC_STACKLEXICAL(stack)   = EC_NIL;								\
			EC_STACKPOINTER(stack)   = EC_STACKBASE(stack);					\
			EC_STACKBP(stack)        = EC_STACKPOINTER(stack);				\
			EC_STACKPC(stack)        = 0;									\
			EC_STACKIMMUP(stack)     = EC_NIL;								\
			EC_STACKIMMCALLER(stack) = EC_NIL;								\
			EC_STACKIMMPC(stack)     = 0;									\
			EC_STACKCALLED(stack)    = EC_NIL;								\
		} else																\
		{																	\
			STKSTAT_INC_V(n_pool_full);										\
		}																	\
	}																		\
} while (0)

#if EC_STACK_RECYCLE_INLINE_PUT
#define _M_RECYCLE_STACK_PUT	EC_RECYCLE_STACK_PUT
#else
#define _M_RECYCLE_STACK_PUT	_ec_recycle_stack_put
#endif

#if defined(_GNU_SOURCE) && (! defined(__STRICT_ANSI__))
#define EC_STACKREF_DEC(obj) \
do {														 									\
	EC_ASSERT(EC_STACKP(obj));																	\
	EC_ASSERT(EC_STACKREF(obj) > 0);															\
	EC_STACKREF(obj)--;															   				\
	STKSTAT_DEC_R();															   				\
	EC_VADEBMESSAGE(EC_STACK_RECYCLE_DEBUG_MSGS,												\
					"--ref(0x%08lX) = %ld\n", (unsigned long)obj, (long)EC_STACKREF(obj));		\
	if (EC_STACKREF(obj) <= 0)													   				\
		_M_RECYCLE_STACK_PUT(obj);													   			\
} while (0)
#else
#define EC_STACKREF_DEC(obj) \
do {														 									\
	EC_ASSERT(EC_STACKP(obj));																	\
	EC_ASSERT(EC_STACKREF(obj) > 0);															\
	EC_STACKREF(obj)--;															   				\
	STKSTAT_DEC_R();															   				\
	if (EC_STACKREF(obj) <= 0)													   				\
		_M_RECYCLE_STACK_PUT(obj);													   			\
} while (0)
#endif

static inline void _ec_stackref_dec(EC_OBJ obj)
{
	EC_STACKREF_DEC(obj);
}

#if ! EC_STACK_RECYCLE_INLINE_PUT
	/* Eventually recycle a stack */
static void _ec_recycle_stack_put(EC_OBJ stack)
{
	EC_RECYCLE_STACK_PUT(stack);
}
#endif

#endif /* EC_STACK_RECYCLE_OPS */

#endif /* EC_STACK_RECYCLE */

#endif /* __STACKRECYCLE_H */
