/* ==========================================================================
 * compile2c_base
 * ==========================================================================
 * elastiC C compiler base file.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Jacopo Pantaleoni         E-mail: jp@lightflowtech.com
 *
 *   Created: Sun May 25 08:05:50 MEST 2003
 *
 *   $Id: compile2c_base.h,v 1.1 2003/05/29 10:36:00 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1998-2002 Marco Pantaleoni. All rights reserved.
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

#ifndef __COMPILE2C_BASE_H
#define __COMPILE2C_BASE_H

#define EC_STACK_RECYCLE_OPS		1
#define EC_STACK_RECYCLE_INLINE_PUT 1
#include <elastic/basic.h>
#include <elastic/debug.h>
#include <elastic/private.h>
#include <elastic/elastic.h>
#include <elastic/bytecode.h>

#include <elastic/stackrecycle.h>

#define USE_MCACHE 0

#define USE_DIRECT_TRUTHTESTING 0

#define TRACE_EXECUTION 0
#define DO_BACKTRACE    0

#if DO_BACKTRACE
#define BACKTRACE(stack, compiled)       do { backtrace( (stack), (compiled) ); } while(0)
#else
#define BACKTRACE(stack, compiled)       do { } while(0)
#endif

static void backtrace( EC_OBJ stack, EC_OBJ compiled );

static EC_OBJ *findBP( EC_OBJ stack, EcInt up, EcInt pos );			/* Find in stack chain (BP relative)   */
static EC_OBJ *findBASE( EC_OBJ stack, EcInt up, EcInt pos );			/* Find in stack chain (BASE relative) */



#define iFETCH	(*codepc++)
#define iPC		(codepc - code)

#define CHECKGC \
do {                                                                                   \
	/* Check if GC is necessary */                                                     \
	if (LPRIVATE(heap).free_top - LPRIVATE(heap).free_base <= LPRIVATE(heap).gc_level) \
	{                                                                                  \
		LPRIVATE(rt.activeFrame) = stack;                                              \
		EcCheckForGC();                                                                \
	}                                                                                  \
} while (0)

EC_OBJ ec_binop_add( EC_OBJ v, EC_OBJ w );
EC_OBJ ec_binop_sub( EC_OBJ v, EC_OBJ w );
EC_OBJ ec_binop_mul( EC_OBJ v, EC_OBJ w );
EC_OBJ ec_binop_div( EC_OBJ v, EC_OBJ w );
EC_OBJ ec_binop_mod( EC_OBJ v, EC_OBJ w );
EC_OBJ ec_binop_pow( EC_OBJ v, EC_OBJ w );
EC_OBJ ec_binop_lshift( EC_OBJ v, EC_OBJ w );
EC_OBJ ec_binop_rshift( EC_OBJ v, EC_OBJ w );
EC_OBJ ec_binop_and( EC_OBJ v, EC_OBJ w );
EC_OBJ ec_binop_or( EC_OBJ v, EC_OBJ w );
EC_OBJ ec_binop_xor( EC_OBJ v, EC_OBJ w );
EC_OBJ ec_unaryop_neg( EC_OBJ v );
EC_OBJ ec_unaryop_pos( EC_OBJ v );
EC_OBJ ec_unaryop_abs( EC_OBJ v );
EC_OBJ ec_unaryop_invert( EC_OBJ v );
EC_OBJ ec_unaryop_int( EC_OBJ v );
EC_OBJ ec_unaryop_float( EC_OBJ v );
EC_OBJ ec_unaryop_true( EC_OBJ v );

#if USE_MCACHE
	/* Stupid out-of-line method cache */
#define MCACHE_SIZE	8
EC_OBJ mcache_class[MCACHE_SIZE];								/* class of receiver     */
EcInt  mcache_msig[MCACHE_SIZE];								/* method signature      */
EC_OBJ mcache_method[MCACHE_SIZE];								/* cached method         */
EC_OBJ mcache_rclass[MCACHE_SIZE];								/* cached receiver class */
EcInt  mcache_pos = 0;
EcInt  mcache_hits = 0, mcache_misses = 0;

#define MCACHE_STATS	\
do {                                                                        \
	if (mcache_misses != 0)                                                 \
	{                                                                       \
		double ratio = (mcache_misses != 0) ?                               \
			(double)mcache_hits / (double)mcache_misses * 100.0 : 0.0;      \
/*		ec_msg_printf( "MCache hits: %ld   misses: %ld  (ratio: %g%%)\n",*/   \
/*				       (long)mcache_hits, (long)mcache_misses,*/              \
/*				       ratio );*/                                             \
	}                                                                       \
} while (0)
#endif

#if EC_STACK_RECYCLE
#if EC_STACK_RECYCLE_STATS
#define STKSTAT_INC_V(var)	do { PRIVATE(var)++; } while (0)
#define STKSTAT_DEC_V(var)	do { PRIVATE(var)--; } while (0)
#else
#define STKSTAT_INC_V(var)	do { } while (0)
#define STKSTAT_DEC_V(var)	do { } while (0)
#endif
#endif /* EC_STACK_RECYCLE */

#if 0
static void dmp( const char *msg, EC_OBJ stack, EC_OBJ compiled )
{
	ec_fprintf( stderr, "-> %-15s    STACK   : %r   0x%08lX       COMPILED: %r   0x%08lX\n",
				msg, stack, (unsigned long)stack, compiled, (unsigned long)compiled );
}

#define DMP(msg, sf,comp)		do { dmp( msg, sf, comp ); } while (0)
#endif

#if EC_STACK_RECYCLE

#if defined(_GNU_SOURCE) && (! defined(__STRICT_ANSI__))
#define GOT_MSG(stack, sp_slot) \
do {                                                                        \
	EC_VADEBMESSAGE(EC_STACK_RECYCLE_DEBUG_MSGS,							\
			        "Got stack in pool (0x%08lX, slot: %d)\n",				\
			        (unsigned long)stack, sp_slot);							\
} while (0)
#define NEW_MSG(stack) \
do {                                                                        \
	EC_VADEBMESSAGE(EC_STACK_RECYCLE_DEBUG_MSGS,							\
			        "New stack created (0x%08lX)\n",						\
			        (unsigned long)(stack));								\
} while (0)
#else
#define GOT_MSG(stack, sp_slot) do { } while (0)
#define NEW_MSG(stack) do { } while (0)
#endif

	/* Search a recycled stack */
#define RECYCLE_STACK_GET(stack_name, callerstack, stackdim)				\
do																			\
{																			\
	EC_OBJ _stack;															\
	EcBool _got = FALSE;													\
																			\
	int sp_slot;															\
																			\
	STKSTAT_INC_V(n_recycle_get_attempts);									\
																			\
	sp_slot = 0;															\
	do {																	\
		ec_bit_ffs_offset(LPRIVATE(stack_pool_bmap),						\
						  EC_STACK_POOL_SIZE,								\
						  sp_slot, &sp_slot);								\
		if (sp_slot < 0)													\
		{																	\
			EC_DEBMESSAGE(EC_STACK_RECYCLE_DEBUG_MSGS,						\
					        "NO STACK IN POOL\n");							\
			STKSTAT_INC_V(n_pool_empty);									\
			_got = FALSE;					/* no stack in pool */			\
			break;															\
		}																	\
		_stack = LPRIVATE(stack_pool)[sp_slot];								\
		if (EC_STACKP(_stack) && (EC_STACKDIM(_stack) >= (stackdim)))		\
		{																	\
			GOT_MSG(_stack, sp_slot);										\
																			\
			LPRIVATE(stack_pool)[sp_slot] = EC_NIL;							\
			ec_bit_clear(LPRIVATE(stack_pool_bmap), sp_slot);				\
			STKSTAT_INC_V(n_recycle_get);									\
																			\
			EC_STACKUP(_stack)      = (callerstack);						\
			EC_STACKPOINTER(_stack) = EC_STACKBASE(_stack);					\
			EC_STACKBP(_stack)      = EC_STACKPOINTER(_stack);				\
			_got = TRUE;					/* got a suitable stack */		\
			break;															\
		}																	\
		sp_slot++;															\
	} while (sp_slot < EC_STACK_POOL_SIZE);									\
																			\
	if (_got) {																\
		EC_STACKREF_SET(_stack, 1);											\
		(stack_name) = _stack;												\
	} else {																\
		(stack_name) = EcMakeStack( (callerstack), (stackdim) );			\
		NEW_MSG(stack_name);												\
	}																		\
	ASSERT(EC_STACKP(stack_name));											\
} while (0)

#endif /* EC_STACK_RECYCLE */

static void backtrace( EC_OBJ stack, EC_OBJ compiled )
{
	EC_OBJ sf, calledsf;
	EcInt stackdepth, depth;
	EcInt i, l;
	char *compname;

	EcInt      line_num;

	stackdepth = 0;
	sf = stack;
	while (EC_NNULLP(sf))
	{
		stackdepth++;
		sf = EC_STACKUP(sf);
	}

	/*	ec_msg_printf( "\nBACKTRACE (stack depth %ld):\n", (long)stackdepth );*/

	depth = stackdepth;

	/* Last line number executed */
	/* line_num = EcCompiledGetLine( compiled, EC_STACKPC(stack) ); */

	sf = stack;
	calledsf = EC_NIL;
	while (EC_NNULLP(sf))
	{
		if (EC_NNULLP(compiled))
		{
			ASSERT( EC_COMPILEDP(compiled) );
			compname = EC_NNULLP(EC_COMPILEDNAME(compiled)) ? EC_STRDATA(EC_COMPILEDNAME(compiled)) : "$NONE$";
			if (! compname)
				compname = "$ANONYMOUS$";
		} else {
			compname = "$NONE$";
		}

		/* determine line number if possible */
		if (EC_NULLP(calledsf))
		{
			line_num = PRIVATE(rt.line_num);
		}
		else
			line_num = 0;
		if (EC_NNULLP(calledsf))
			line_num = EcCompiledGetLine( compiled, EC_STACKPC(calledsf) );

/*		if (line_num > 0)
			ec_msg_printf( "    %2d (l: %3d) %s(", depth, line_num, compname );
		else
			ec_msg_printf( "    %2d %8s %s(", depth, " ", compname );

		l = EC_STACKBP(sf) - EC_STACKBASE(sf);
		for (i = 3; i < l; i++)
		{
			ec_msg_printf( "%W", EC_STACKGET(sf, i) );
			if (i + 1 < l)
				ec_msg_printf( ", " );
		}

		ec_msg_printf( ")\n" );*/

		depth--;
		if (EC_NNULLP(EC_STACKIMMCALLER(sf)))
			compiled = EC_STACKIMMCALLER(sf);
		else
			compiled = EC_STACKGET(sf, 0);
		calledsf = sf;
		sf       = EC_STACKUP(sf);
	}

/*	ec_msg_printf( "\n" );*/
}


static EC_OBJ *findBP( EC_OBJ stack, EcInt up, EcInt pos )
{
	EC_OBJ sf;

	ASSERT( EC_STACKP(stack) );
	ASSERT( (up >= 0) || (up == -1) );

	sf = stack;
	if (up >= 0)
	{
		while (EC_NNULLP(sf) && (up >= 0))
		{
			if (up == 0)
				return &(EC_STACKBP(sf)[pos]);

			sf = EC_STACKLEXICAL(sf);
			up--;
		}
	} else
	{
		while (EC_NNULLP(sf))
		{
			if (EC_NULLP(EC_STACKLEXICAL(sf)))
				return &(EC_STACKBP(sf)[pos]);

			sf = EC_STACKLEXICAL(sf);
		}
	}

	return NULL;
}

static EC_OBJ *findBASE( EC_OBJ stack, EcInt up, EcInt pos )
{
	EC_OBJ sf;

	ASSERT( EC_STACKP(stack) );
	ASSERT( (up >= 0) || (up == -1) );

	sf = stack;
	if (up >= 0)
	{
		while (EC_NNULLP(sf) && (up >= 0))
		{
			if (up == 0)
				return &(EC_STACKBASE(sf)[pos]);

			sf = EC_STACKLEXICAL(sf);
			up--;
		}
	} else
	{
		while (EC_NNULLP(sf))
		{
			if (EC_NULLP(EC_STACKLEXICAL(sf)))
				return &(EC_STACKBASE(sf)[pos]);

			sf = EC_STACKLEXICAL(sf);
		}
	}

	return NULL;
}

/*
WRONG !!! BUGGY !!! WRONG !!!

static EC_OBJ *findLiteral( EC_OBJ stack, EcInt up, EcInt pos )
{
	EC_OBJ sf;

	ASSERT( EC_STACKP(stack) );
	ASSERT( (up >= 0) );

	sf = stack;
	while (EC_NNULLP(sf) && (up >= 0))
	{
		if (up == 0)
		{
			ASSERT( EC_ARRAYP(EC_STACKGET( sf, 0 )) );
			return &(EC_ARRAYMEM(EC_STACKGET( sf, 0 ))[pos]);
		}

		sf = EC_STACKLEXICAL(sf);
		up--;
	}

	return NULL;
}*/

#endif
