/* ==========================================================================
 * interp.c
 * ==========================================================================
 * elastiC interpreter.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Thu Jul 16 18:28:50 MEST 1998
 *
 *   $Id: interp.c,v 1.2 2002/06/13 17:44:18 panta Exp $
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

#define EC_STACK_RECYCLE_OPS		1
#define EC_STACK_RECYCLE_INLINE_PUT 1
#include "basic.h"
#include "debug.h"
#include "private.h"
#include "elastic.h"
#include "bytecode.h"

#include "stackrecycle.h"

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

/* ========================================================================
 * P U B L I C
 * ======================================================================== */

inline EC_API EcBool EcIsSequence( EC_OBJ obj )
{
	return (EC_ARRAYP(obj) ||
			EC_HASHP(obj)  ||
			IS_USER_SEQUENCE(obj));
}

inline EC_API EcInt EcSequenceNormalizeIndex( EC_OBJ sequence, EcInt i )
{
	EcInt len = -1;

	ASSERT( i < 0 );

	/*
	 * For user sequences, we must ensure that the user wants index normalization
	 * (for hash tables, for example, we don't since negative integers are valid
	 * indexes. This can be done by checking for length_fcn.
	 */

	if (EC_ARRAYP(sequence))
	{
		len = EC_ARRAYLEN(sequence);
	} else if (IS_USER_SEQUENCE(sequence) && CB_SEQUENCE(sequence)->length_fcn)
	{
		EC_OBJ len_o;

		len_o = CB_SEQUENCE(sequence)->length_fcn( sequence );
		if (! EC_INUMP(len_o)) return i;

		len = EC_INUM(len_o);
	}

	if ((len >= 0) && (len + i >= 0))
		return len + i;

	return i;
}

inline EC_API EC_OBJ EcSequenceGetElement( EC_OBJ sequence, EcInt index )
{
	if (index < 0)
		index = EcSequenceNormalizeIndex( sequence, index );

	if (EC_ARRAYP(sequence))
	{
		return EcArrayGet( sequence, index );
	} else if (IS_USER_SEQUENCE(sequence))
	{
		return CB_SEQUENCE(sequence)->getitem_fcn( sequence, EcMakeInt( index ) );
	}

	return EcTargetError( sequence, "indexing a non sequence" );
}

inline EC_API EC_OBJ EcSequenceGetItem( EC_OBJ sequence, EC_OBJ index )
{
	if (EC_INUMP(index) && (EC_INUM(index) < 0))
#if EC_INLINED_TYPES
		index = EC_MAKE_INT( EcSequenceNormalizeIndex( sequence, EC_INUM(index) ) );
#else
		EC_INUM(index) = EcSequenceNormalizeIndex( sequence, EC_INUM(index) );
#endif

	if (EC_ARRAYP(sequence))
	{
		if (! EC_INUMP(index))
			return EcTypeError( EC_NIL, EC_NIL, 2, tc_inum, index, TRUE, "array getitem" );

		return EcArrayGet( sequence, EC_INUM(index) );
	} else if (IS_USER_SEQUENCE(sequence))
	{
		return CB_SEQUENCE(sequence)->getitem_fcn( sequence, index );
	}

	return EcTargetError( sequence, "indexing a non sequence" );
}

EC_API EC_OBJ EcSequenceSetElement( EC_OBJ sequence, EcInt index, EC_OBJ value )
{
	if (index < 0)
		index = EcSequenceNormalizeIndex( sequence, index );

	if (EC_ARRAYP(sequence))
	{
		return EcArraySet( sequence, index, value );
	} else if (IS_USER_SEQUENCE(sequence))
	{
		return CB_SEQUENCE(sequence)->setitem_fcn( sequence, EcMakeInt( index ), value );
	}

	return EcTargetError( sequence, "indexing a non sequence" );
}

inline EC_API EC_OBJ EcSequenceSetItem( EC_OBJ sequence, EC_OBJ index, EC_OBJ value )
{
	if (EC_INUMP(index) && (EC_INUM(index) < 0))
#if EC_INLINED_TYPES
		index = EC_MAKE_INT( EcSequenceNormalizeIndex( sequence, EC_INUM(index) ) );
#else
		EC_INUM(index) = EcSequenceNormalizeIndex( sequence, EC_INUM(index) );
#endif

	if (EC_ARRAYP(sequence))
	{
		if (! EC_INUMP(index))
			return EcTypeError( EC_NIL, EC_NIL, 2, tc_inum, index, TRUE, "array setitem" );

		return EcArraySet( sequence, EC_INUM(index), value );
	} else if (IS_USER_SEQUENCE(sequence))
	{
		return CB_SEQUENCE(sequence)->setitem_fcn( sequence, index, value );
	}

	return EcTargetError( sequence, "indexing a non sequence" );
}

inline EC_API EC_OBJ EcSequenceDelElement( EC_OBJ sequence, EcInt index )
{
	if (index < 0)
		index = EcSequenceNormalizeIndex( sequence, index );

	if (EC_ARRAYP(sequence))
		return EcArrayDel( sequence, index );
	else if (IS_USER_SEQUENCE(sequence))
		return CB_SEQUENCE(sequence)->delitem_fcn( sequence, EcMakeInt( index ) );

	return EcTargetError( sequence, "indexing a non sequence" );
}

inline EC_API EC_OBJ EcSequenceDelItem( EC_OBJ sequence, EC_OBJ index )
{
	if (EC_INUMP(index) && (EC_INUM(index) < 0))
#if EC_INLINED_TYPES
		index = EC_MAKE_INT( EcSequenceNormalizeIndex( sequence, EC_INUM(index) ) );
#else
		EC_INUM(index) = EcSequenceNormalizeIndex( sequence, EC_INUM(index) );
#endif

	if (EC_ARRAYP(sequence))
	{
		if (! EC_INUMP(index))
			return EcTypeError( EC_NIL, EC_NIL, 2, tc_inum, index, TRUE, "array delitem" );

		return EcArrayDel( sequence, EC_INUM(index) );
	} else if (IS_USER_SEQUENCE(sequence))
		return CB_SEQUENCE(sequence)->delitem_fcn( sequence, index );

	return EcTargetError( sequence, "indexing a non sequence" );
}

inline EC_API EC_OBJ EcSequenceLength( EC_OBJ sequence )
{
	if (EC_ARRAYP(sequence))
	{
		return EcMakeInt( EC_ARRAYLEN(sequence) );
	} else if (EC_NULLP(sequence))
	{
		return EcMakeInt( 0 );
	} else if (IS_USER_SEQUENCE(sequence))
	{
		if (! CB_SEQUENCE(sequence)->length_fcn)
			return EcTargetError( sequence, "asked length for a non ordered collection" );
		return CB_SEQUENCE(sequence)->length_fcn( sequence );
	}

	return EcTargetError( sequence, "asked length for a non sequence" );
}

inline EC_API EC_OBJ EcSequenceIn( EC_OBJ sequence, EC_OBJ element )
{
	EcInt  len = 0, i;
	EC_OBJ len_o;
	EcBool found;
	EcInt  cmp;
	EC_OBJ el;

	len_o = EcSequenceLength( sequence );
	if (EC_ERRORP(len_o)) return len_o;

	if (EC_INUMP(len_o))
		len = EC_INUM(len_o);

	found = FALSE;
	for (i = 0; i < len; i++)
	{
		el = EcSequenceGetElement( sequence, i );
		if (EC_ERRORP(el)) return el;
		if (EcObjectCompare( element, el, &cmp ))
		{
			if (cmp == 0)
			{
				found = TRUE;
				break;
			}
		}
	}

	return found ? EcTrueObject : EcFalseObject;
}

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
		ec_msg_printf( "MCache hits: %ld   misses: %ld  (ratio: %g%%)\n",   \
				       (long)mcache_hits, (long)mcache_misses,              \
				       ratio );                                             \
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

EC_API EC_OBJ EcMainExecute( EC_OBJ package )
{
	EC_OBJ code;
	EC_OBJ stack;
	EC_OBJ res;

#if USE_MCACHE
	/* Initialize method cache */
	EcInt i;
	for (i = 0; i < MCACHE_SIZE; i++)
	{
		mcache_class[i]  = EC_NIL;
		mcache_msig[i]   = -1;
		mcache_method[i] = EC_NIL;
		mcache_rclass[i] = EC_NIL;
	}
	mcache_pos = 0;
	mcache_hits   = 0;
	mcache_misses = 0;
#endif


	ASSERT( EC_PACKAGEP(package) );

	code = EC_PACKAGECODE(package);

	ASSERT( EC_COMPILEDP(code) );
	ASSERT( EC_COMPILEDNLOC(code) >= 0 );

	stack = EcMakeStack( EC_NIL,
						 1 + 2 +
						 0 +									/* ... no args ... */
						 EC_COMPILEDNLOC(code) +
						 EC_COMPILEDMAXTEMPS(code) );
#if EC_STACK_RECYCLE
	/* STACK_INC_REF(stack); done in EcMakeStack */
#endif
	EC_STACKNARGS_REAL(stack)    = 0;				/* set real num. of passed args    */
	EC_STACKNARGS_LOGICAL(stack) = 0;				/* set logical num. of passed args */

	EC_STACKLEXICAL(stack) = EC_NIL;

#if 0
	EcStackPush( stack, code );									/* caller bytecode  */
#else
	EcStackPush( stack, EC_NIL );								/* caller bytecode  */
#endif
	EcStackPush( stack, EC_NIL );								/* caller object    */
	EcStackPush( stack, EC_NIL );								/* caller at_class  */
	EC_STACKPC(stack) = 0;										/* caller PC        */

	EC_STACKIMMUP(stack)     = EC_NIL;							/* inline up        */
	EC_STACKIMMCALLER(stack) = EC_NIL;							/* inline caller    */
	EC_STACKIMMPC(stack)     = 0;								/* inline caller PC */

	EC_STACKCALLED(stack)    = code;							/* called bytecode  */

	/* Set SP and BP */
	EC_STACKBP(stack)      = EC_STACKBASE(stack) + 1 + 2 + 0;			/* ... no args ... */
	EC_STACKPOINTER(stack) = EC_STACKBP(stack) + EC_COMPILEDNLOC(code);

	PRIVATE(globalFrame) = stack;

#ifdef EC_DEBUG
#if defined(WITH_STDIO) && TRACE_EXECUTION
	_ec_dbg_dump_package_frame( EC_PACKAGEFRAME(package) );
#endif
#endif
	res = EcExecute( EC_NIL, EC_NIL, code, stack );

#if EC_STACK_RECYCLE
	/*
	 * Here the stack reference count can be > 1, because of SaveLexicalOP
	 * used to link stacks in top-level functions to package top-level stack
	 * (which happens to be this one).
	 */
	/* EC_STACKREF_SET(stack, 1); */									/* this stack shouldn't be reference by someone else, right ??? */
	EC_STACKREF_DEC(stack);										/* this stack shouldn't be reference by someone else, right ??? */
#endif

#if USE_MCACHE
	MCACHE_STATS;
#endif
	return res;
}

#if 0
static void dmp( const char *msg, EC_OBJ stack, EC_OBJ compiled )
{
	ec_msg_printf( "-> %-15s    STACK   : %r   0x%08lX       COMPILED: %r   0x%08lX\n",
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

EC_API EC_OBJ EcExecute( EC_OBJ self, EC_OBJ at_class, EC_OBJ compiled, EC_OBJ stack )
{
	register EcBytecode instr;
	register EcInt      op1, op2;
	register EC_OBJ   obj1;
	EC_OBJ   obj2;

	EC_OBJ ret = Ec_ERROR;										/* Return register */

	/* These are unnecessarily duplicated, but are kept here to speed up */
	EcBytecode *code, *codepc, *codeend;						/* Start and end of code   */
	EcInt       ncode;											/* Lenght of code          */
	EC_OBJ      lframe;											/* Literal Frame           */

	/* Necessary for single operations */
	EC_OBJ   sf, obj = EC_NIL, *where;
	EC_OBJ   frame;
	EcInt    i;
	EC_OBJ   comp;
	EC_OBJ   lf;
	EcInt    nargs;

	EC_OBJ   receiver, receiver_class;

	EC_OBJ   handlerchain;

	EcPrivate *lprivate;

	ASSERT( EC_COMPILEDP(compiled) );
	ASSERT( EC_STACKP(stack) );

	lprivate = &_ec_private;

	/* First-time initialization */
	code   = EC_COMPILEDCODE(compiled);
	ncode  = EC_COMPILEDNCODE(compiled);
	lframe = EC_COMPILEDLFRAME(compiled);
	codepc  = code;
	codeend = code + ncode;

	EC_STACKCALLED(stack) = compiled;

	ASSERT( EC_NULLP(self) || EC_OBJECTP(self) );
	if (EC_NULLP(at_class) && EC_NNULLP(self))
		at_class = EC_OBJECTCLASS(self);

	LPRIVATE(rt.vm_level)++;

/*	fprintf( stderr, "VM LEVEL: %ld\n", LPRIVATE(rt.vm_level) );*/

#if defined(WITH_STDIO) && TRACE_EXECUTION
	printf("\n");
	EcDumpCompiled( compiled, codepc - code );
	printf("\n");
	printf("LEXICALLY UPPER FRAME: 0x%08lX\n", (unsigned long)EC_STACKLEXICAL(stack));
#endif

	/* Set current compiled object & line number */
	LPRIVATE(rt.compiled)     = compiled;
	LPRIVATE(rt.line_num)     = 0;
	LPRIVATE(rt.exc_line_num) = -1;

	BACKTRACE( stack, compiled );

	/* Go ! */
restart:
	for (;;)
	{
/*		EC_CHECK(compiled);
		EC_CHECKALL();*/
		if (codepc >= codeend)
		{
			LPRIVATE(rt.vm_level)--;
			return EC_NIL;
		}

#if defined(WITH_STDIO) && TRACE_EXECUTION
		printf( "\n" );
		_ec_dbg_dump_stack( stack );
		_ec_dbg_print_instruction( compiled, codepc - code );
#endif

/*		ASSERT( iPC < ncode );*/

		/* Fetch */
		instr = iFETCH;

		/* Decode & Execute */
		switch (instr)
		{
			/* Stack bytecodes      */

		case PushHereOP:
#if 0
			op1 = (EcInt)iFETCH;								/* pos */
			obj = EC_STACKBPGET( stack, op1 );
			EC_STACKPUSH( stack, obj );
#endif
			EC_STACKPUSH( stack, EC_STACKBPGET( stack, (EcInt)iFETCH ) );
			continue;

		case PopHereOP:
#if 0
			op1 = (EcInt)iFETCH;								/* pos */
			obj = EC_STACKPOP( stack );
			EC_STACKBP(stack)[op1] = obj;
#endif
			EC_STACKBP(stack)[(EcInt)iFETCH] = EC_STACKPOP( stack );
			continue;

		case PushParamOP:
			op1 = (EcInt)iFETCH;								/* up  */
			op2 = (EcInt)iFETCH;								/* pos */
			sf = stack;
			ASSERT( EC_NNULLP(sf) );
			ASSERT( op1 >= 0 );
			while (EC_NNULLP(sf) && (op1 >= 0))
			{
				if (op1 == 0)
				{
					obj = EC_STACKGET( sf, op2 );
					break;
				}

				sf = EC_STACKLEXICAL(sf);
				op1--;
			}
			ASSERT( op1 == 0 );
			EC_STACKPUSH( stack, obj );
			continue;

		case PopParamOP:
			op1 = (EcInt)iFETCH;								/* up  */
			op2 = (EcInt)iFETCH;								/* pos */
			where = findBASE( stack, op1, op2 );
			obj = EC_STACKPOP( stack );
			*where = obj;
/*			EC_STACKBASE(stack)[op1] = obj;*/
			continue;

		case PushOP:
			op1 = (EcInt)iFETCH;								/* up  */
			op2 = (EcInt)iFETCH;								/* pos */
			sf = stack;
			ASSERT( EC_NNULLP(sf) );
			ASSERT( op1 >= 0 );
			while (EC_NNULLP(sf) && (op1 >= 0))
			{
				if (op1 == 0)
				{
					obj = EC_STACKBPGET( sf, op2 );
					break;
				}

				sf = EC_STACKLEXICAL(sf);
				op1--;
			}
			EC_STACKPUSH( stack, obj );
			continue;

		case PushLiteralOP:
			op1 = (EcInt)iFETCH;								/* up  */
			op2 = (EcInt)iFETCH;								/* pos */

			if (op1 == 0)
			{
				/* Special case: most frequent case */
				comp = EC_STACKCALLED(stack);
				lf   = EC_COMPILEDLFRAME(comp);
				ASSERT(EC_ARRAYLEN(lf) > op2);
				/* obj  = EcArrayGet( lf, op2 ); */
				obj  = EC_ARRAYGET( lf, op2 );
				EC_STACKPUSH( stack, obj );
				continue;
			}

			sf   = stack;
			comp = compiled;
			ASSERT( comp == EC_STACKCALLED(sf) );
			while (EC_NNULLP(sf) && (op1 >= 0))					/* go to the right frame */
			{
				if (op1 == 0)
				{
					comp = EC_STACKCALLED(sf);
					lf   = EC_COMPILEDLFRAME(comp);
					ASSERT(EC_ARRAYLEN(lf) > op2);
					/* obj  = EcArrayGet( lf, op2 ); */
					obj  = EC_ARRAYGET( lf, op2 );
					break;
				}

				sf = EC_STACKLEXICAL(sf);
				op1--;
			}
/*			printf( "PushLiteral: " ); ec_fprintf( stdout, "%w\n", obj ); */
			EC_STACKPUSH( stack, obj );
			continue;

		case PushPackageOP:
			op1 = (EcInt)iFETCH;								/* pkg */
			op2 = (EcInt)iFETCH;								/* pos */
			ASSERT( op1 < LPRIVATE(npackages) );
			ASSERT(EC_ARRAYLEN(EC_PACKAGEFRAME(LPRIVATE(package)[op1].obj)) > op2);
			/* obj = EcArrayGet( EC_PACKAGEFRAME(LPRIVATE(package)[op1].obj), op2 ); */
			obj = EC_ARRAYGET( EC_PACKAGEFRAME(LPRIVATE(package)[op1].obj), op2 );
			EC_STACKPUSH( stack, obj );
			continue;

		case PopOP:
			op1 = (EcInt)iFETCH;								/* up  */
			op2 = (EcInt)iFETCH;								/* pos */
			where = findBP( stack, op1, op2 );
			obj = EC_STACKPOP( stack );
			*where = obj;
			continue;

		case PopLiteralOP:
			op1 = (EcInt)iFETCH;								/* up  */
			op2 = (EcInt)iFETCH;								/* pos */
			sf   = stack;
			comp = compiled;
			ASSERT( comp == EC_STACKCALLED(sf) );
			where = NULL;
			while (EC_NNULLP(sf) && (op1 >= 0))					/* go to the right frame */
			{
				if (op1 == 0)
				{
					comp  = EC_STACKCALLED(sf);
					lf    = EC_COMPILEDLFRAME(comp);
					ASSERT(EC_ARRAYLEN(lf) > op2);
/*					where = &(EC_ARRAYMEM(lf)[op2]);*/
					where = EC_ARRAYGETR(lf, op2);
					break;
				}

				sf = EC_STACKLEXICAL(sf);
				op1--;
			}
			ASSERT( where );
			/* where = findLiteral( stack, op1, op2 ); */
			obj = EC_STACKPOP( stack );
			*where = obj;
			continue;

		case PopPackageOP:
			op1 = (EcInt)iFETCH;								/* pkg */
			op2 = (EcInt)iFETCH;								/* pos */
			obj = EC_STACKPOP( stack );
			ASSERT(EC_ARRAYLEN(EC_PACKAGEFRAME(LPRIVATE(package)[op1].obj)) > op2);
			/* EcArraySet( EC_PACKAGEFRAME(LPRIVATE(package)[op1].obj), op2, obj ); */
			EC_ARRAYSET( EC_PACKAGEFRAME(LPRIVATE(package)[op1].obj), op2, obj );
			continue;

		case PushInstanceOP:
			op1 = (EcInt)iFETCH;								/* pos */
			if (EC_OBJECTP(self))
			{
				obj = EC_OBJECTIVARS(self)[EC_CLASSIOFFSET(at_class) + op1];
			} else
			{
				ASSERT( EC_CLASSP(self) );
				ASSERT( EC_CLASSP(at_class) );
				ASSERT(EC_ARRAYLEN(EC_CLASSLFRAME(at_class)) > op1);
/*				obj = EC_ARRAYMEM(EC_CLASSLFRAME(at_class))[op1];*/
				obj = EC_ARRAYGET(EC_CLASSLFRAME(at_class), op1);
			}
			EC_STACKPUSH( stack, obj );
			continue;

		case PopInstanceOP:
			op1 = (EcInt)iFETCH;								/* pos */
			obj = EC_STACKPOP( stack );
			if (EC_OBJECTP(self))
			{
				EC_OBJECTIVARS(self)[EC_CLASSIOFFSET(at_class) + op1] = obj;
			} else
			{
				ASSERT( EC_CLASSP(self) );
				ASSERT( EC_CLASSP(at_class) );
				ASSERT(EC_ARRAYLEN(EC_CLASSLFRAME(at_class)) > op1);
/*				EC_ARRAYMEM(EC_CLASSLFRAME(at_class))[op1] = obj;*/
				EC_ARRAYSET(EC_CLASSLFRAME(at_class), op1, obj);
			}
			continue;

		case PushClassOP:
			op1 = (EcInt)iFETCH;								/* pos */
			obj = EcArrayGet( EC_CLASSLFRAME(at_class), op1 );
			EC_STACKPUSH( stack, obj );
			continue;

		case PopClassOP:
			op1 = (EcInt)iFETCH;								/* pos */
			obj = EC_STACKPOP( stack );
			EcArraySet( EC_CLASSLFRAME(at_class), op1, obj );
			continue;

		case DupOP:
#if 0
			obj = EC_STACKPOP( stack );
			EC_STACKPUSH( stack, obj );
			EC_STACKPUSH( stack, obj );
#endif
			EC_STACKDUP( stack );
			continue;

		case DiscardOP:
			/* This should sound like EC_STACKPOP(stack) */
			--EC_STACKPOINTER(stack);
			continue;

		case PushStackOP:
			EC_STACKPUSH( stack, stack );
			continue;

		case GetOP:
			op1 = (EcInt)iFETCH;								/* pos */
			obj = *(EC_STACKPOINTER(stack) - 1 - op1);
			EC_STACKPUSH( stack, obj );
			continue;

			/* Control bytecodes    */

		case BranchIfTrueOP:
			op1 = (EcInt)iFETCH;								/* to  */
			obj = EC_STACKPOP( stack );
#if USE_DIRECT_TRUTHTESTING
			if (EC_ASBOOLVAL(obj))
				codepc = code + op1;
#else
			if (EC_BOOLP(obj))
			{
				if (EC_BOOLVAL(obj))
					codepc = code + op1;
			} else if (EC_INUMP(obj))
			{
				if (EC_INUM(obj))
					codepc = code + op1;
			} else if (ec_unaryop_true( obj ) == EcTrueObject)
				codepc = code + op1;
#endif
			continue;

		case BranchIfFalseOP:
			op1 = (EcInt)iFETCH;								/* to  */
			obj = EC_STACKPOP( stack );
#if USE_DIRECT_TRUTHTESTING
			if (! EC_ASBOOLVAL(obj))
				codepc = code + op1;
#else
			if (EC_BOOLP(obj))
			{
				if (! EC_BOOLVAL(obj))
					codepc = code + op1;
			} else if (EC_INUMP(obj))
			{
				if (! EC_INUM(obj))
					codepc = code + op1;
			} else if (ec_unaryop_true( obj ) == EcFalseObject)
				codepc = code + op1;
#endif
			continue;

		case BranchIfDefinedOP:
			op1 = (EcInt)iFETCH;								/* to  */
			obj = EC_STACKPOP( stack );
			if (! EC_UNDEFINEDP(obj))
				codepc = code + op1;
			continue;

		case JumpOP:
			codepc = code + (EcInt)*codepc;						/* jump to */
			continue;

		case CallOP:
			op1 = (EcInt)iFETCH;								/* nargs (effective number of arguments) */
			obj = EC_STACKPOP( stack );							/* function to call */

			CHECKGC;

			if (EC_PRIMITIVEP(obj))
			{
				EC_STACKNARGS_REAL(stack) = op1;				/* set real num. of passed args    */
				EC_STACKNARGS_LOGICAL(stack) = op1;				/* set logical num. of passed args */

				ret = EC_PRIMITIVE(obj)( stack, EC_PRIMITIVEUSERDATA(obj) );
				if (EC_ERRORP(ret)) goto on_error;
				EC_STACKPUSH( stack, ret );
			} else
			{
				if (! EC_COMPILEDP(obj))
				{
					EcUnimplementedError( "non-callable object called" );
					goto on_error;
				}
				ASSERT( EC_COMPILEDP(obj) );

				nargs = EC_COMPILEDNARG(obj);					/* explicit arguments */

				/* Set on *CURRENT* stack the number of paramters, for error reporting */
				EC_STACKNARGS_REAL(stack)    = nargs + EC_COMPILEDVARG(obj);	/* set real num. of passed args    */
				EC_STACKNARGS_LOGICAL(stack) = op1;								/* set logical num. of passed args */

				if (op1 < (nargs - EC_COMPILEDNARG_DEF(obj)))
				{
					/* There are less arguments than necessary */
					EcParameterCountError( EC_NIL, obj, /* expected */ nargs - EC_COMPILEDNARG_DEF(obj), stack );
					/*EcMakeException( EcExceptionClass,
					  "too few arguments to function %w (got %ld, expected %ld)",
					  obj, op1, nargs - EC_COMPILEDNARG_DEF(obj) );*/
					goto on_error;
				}

				/* if op1 < nargs we'll have to set unspecified parameters to @undef */

				if ((! EC_COMPILEDVARG(obj)) && (op1 > nargs))
				{
					/* There are more arguments than necessary */
					EcParameterCountError( EC_NIL, obj, /* expected */ nargs, stack );
					/*EcMakeException( EcExceptionClass,
									 "too many arguments to function %w (got %ld, expected %ld)",
									 obj, op1, nargs );*/
					goto on_error;
				}

#if EC_STACK_RECYCLE
				RECYCLE_STACK_GET(frame,						/* where we want the result */
								  stack,						/* caller stack */
								  (1 + 2 +						/* dimension */
								   nargs + EC_COMPILEDVARG(obj) +
								   EC_COMPILEDNLOC(obj) + EC_COMPILEDMAXTEMPS(obj)));

				/* STACK_INC_REF(frame); */
#else
				/* Prepare a stack frame for the call */
				frame = EcMakeStack( stack,
									 1 + 2 +
									 nargs + EC_COMPILEDVARG(obj) +
									 EC_COMPILEDNLOC(obj) + EC_COMPILEDMAXTEMPS(obj) );
#endif
				ASSERT( EC_STACKP(frame) );
				EC_STACKNARGS_REAL(frame)    = nargs + EC_COMPILEDVARG(obj);	/* set real num. of passed args    */
				EC_STACKNARGS_LOGICAL(frame) = op1;								/* set logical num. of passed args */

				EC_STACKLEXICAL(frame) = EC_COMPILEDLEXICAL(obj);
#if EC_STACK_RECYCLE
				if (EC_STACKP(EC_STACKLEXICAL(frame)))
					EC_STACKREF_INC(EC_STACKLEXICAL(frame));
#endif
				EC_STACKPC(frame)        = iPC;						/* Program Counter */
				EC_STACKIMMUP(frame)     = EC_NIL;
				EC_STACKIMMCALLER(frame) = EC_NIL;
				EC_STACKIMMPC(frame)     = 0;
				EC_STACKCALLED(frame)    = obj;						/* called bytecode  */
				EC_STACKPUSH( frame, compiled );					/* caller bytecode        */
				EC_STACKPUSH( frame, self );						/* caller object          */
				EC_STACKPUSH( frame, at_class );					/* caller object at_class */
				/* +3 */

				/* Put varargs on callee's frame */
				if (EC_COMPILEDVARG(obj))
				{
					EC_OBJ vargs;

					/* BUG: vargs = EcMakeArray( op1 - EC_COMPILEDVARG(obj) ); */
					if (op1 > nargs)
					{
						vargs = EcMakeArray( op1 - nargs );
						EC_ARRAYLEN(vargs) = op1 - nargs;
					} else
					{
						vargs = EcMakeArray( 0 );
						EC_ARRAYLEN(vargs) = 0;
					}

					for (i = op1 - nargs - 1; i >= 0; i--)
					{
						ASSERT(EC_ARRAYLEN(vargs) > i);
						/* EcArraySet( vargs, i, EC_STACKPOP( stack ) ); */
						EC_ARRAYSET( vargs, i, EC_STACKPOP( stack ) );
					}

					EC_STACKSET( frame, nargs + 3, vargs );
				}

				/* Put required arguments on callee's frame */
				if (op1 >= nargs)
				{
					for (i = nargs - 1; i >= 0; i--)
						EC_STACKSET( frame, i + 3, EC_STACKPOP( stack ) );
				} else
				{
					EcInt uninit_low;

					ASSERT( (op1 < nargs) && (op1 >= (nargs - EC_COMPILEDNARG_DEF(obj))) );

					uninit_low = nargs - EC_COMPILEDNARG_DEF(obj);
					if (op1 > uninit_low)
						uninit_low = op1;

					for (i = nargs - 1; i >= uninit_low; i--)	/* set parameters with default values to @undef */
						EC_STACKSET( frame, i + 3, EcUndefinedObject );
					for (i = op1 - 1; i >= 0; i--)				/* ... then use arguments                       */
						EC_STACKSET( frame, i + 3, EC_STACKPOP( stack ) );
				}

				/*
				 * Stack base now must point to the beginning of local variables, and
				 * stack pointer must be just after them, at the beginning of temporary position
				 */

				/* Set SP and BP */
				EC_STACKBP(frame)      = EC_STACKBASE(frame) + 1 + 2 + nargs + EC_COMPILEDVARG(obj);
				EC_STACKPOINTER(frame) = EC_STACKBP(frame) + EC_COMPILEDNLOC(obj);

/*				DMP("CallOP", frame, obj);*/

				compiled = obj;
				stack    = frame;
				LPRIVATE(rt.compiled) = compiled;

				code   = EC_COMPILEDCODE(compiled);
				ncode  = EC_COMPILEDNCODE(compiled);
				lframe = EC_COMPILEDLFRAME(compiled);
				codepc  = code;
				codeend = code + ncode;

				BACKTRACE( stack, compiled );
#if defined(WITH_STDIO) && TRACE_EXECUTION
				printf("\n");
				EcDumpCompiled( compiled, codepc - code );
				printf("\n");
				printf("LEXICALLY UPPER FRAME: 0x%08lX\n", (unsigned long)EC_STACKLEXICAL(stack));
#endif
				/* fprintf(stderr, "Restarting cycle\n"); fflush(stderr); */
			}
			continue;

		case CallMethodOP:
			op1 = (EcInt)iFETCH;								/* method   */
			op2 = (EcInt)iFETCH;								/* nargs    */
			obj = EC_STACKPOP( stack );							/* receiver */

/*			ec_fprintf( stderr, "Calling method %k for object %w, with %ld args.\n", op1, obj, op2 );*/

/*			if (! (EC_OBJECTP(obj) || EC_CLASSP(obj)))
			{
				ec_fprintf(stderr, "[1] 0x%08lX %W\n", obj, obj);
				backtrace(stack, compiled);
			}*/
			ASSERT( EC_OBJECTP(obj) || EC_CLASSP(obj) );
			/*ec_fprintf(stderr, "[1b] %W\n", obj);*/

			CHECKGC;

/*			if (! (EC_OBJECTP(obj) || EC_CLASSP(obj)))
			{
				ec_fprintf(stderr, "[2] 0x%08lX %W\n", obj, obj);
				backtrace(stack, compiled);
			}*/
			ASSERT( EC_OBJECTP(obj) || EC_CLASSP(obj) );

			if (EC_OBJECTP(obj))
			{
				if (obj == self)
				{
					receiver       = self;
					/*receiver_class = at_class;*/
					receiver_class = EC_NIL;
				} else
				{
					receiver       = obj;
					/*receiver_class = EC_OBJECTCLASS(obj);*/
					receiver_class = EC_NIL;
				}

#if USE_MCACHE
				/* Check method cache */
				{
					EC_OBJ oclass = EC_OBJECTCLASS(obj);

					for (i = 0; i < MCACHE_SIZE; i++)
					{
						if ((mcache_class[i] == oclass) &&
							(mcache_msig[i]  == op1))
						{
							obj1           = mcache_method[i];
							receiver_class = mcache_rclass[i];
							mcache_hits++;
							goto method_found;
						}
					}
				}
#endif

				obj1 = EcFindMethod( receiver, &receiver_class, op1 );
#if USE_MCACHE
				mcache_class[mcache_pos]  = EC_OBJECTCLASS(obj);
				mcache_msig[mcache_pos]   = op1;
				mcache_method[mcache_pos] = obj1;
				mcache_rclass[mcache_pos] = receiver_class;
				mcache_pos++;
				mcache_misses++;
				if (mcache_pos >= MCACHE_SIZE) mcache_pos = 0;
#endif
/*				ec_fprintf( stderr, "Found method %k at class %w\n", op1, receiver_class );*/
			} else
			{
				receiver       = obj;
				receiver_class = EC_NIL;

				obj1 = EcFindClassMethod( receiver, &receiver_class, op1 );
			}

#if USE_MCACHE
		method_found:
#endif
			if (EC_NULLP(obj1))
			{
				/* Method not found */

				/* TODO: try to get a stack from stack pool */

				/* Prepare a stack frame for the call as if the method was found */
				frame = EcMakeStack( stack,
									 1 + 2 + op2 );
#if EC_STACK_RECYCLE
				/* STACK_INC_REF(frame); */
#endif
				EC_STACKNARGS_REAL(frame)    = op2;				/* set real num. of passed args    */
				EC_STACKNARGS_LOGICAL(frame) = op2;				/* set logical num. of passed args */

				EC_STACKLEXICAL(frame) = EC_NIL;				/* no method, no lexical... */
				EC_STACKPC(frame) = iPC;						/* Program Counter */
				EC_STACKIMMUP(frame)     = EC_NIL;
				EC_STACKIMMCALLER(frame) = EC_NIL;
				EC_STACKIMMPC(frame)     = 0;
				EC_STACKCALLED(frame)    = obj1;					/* called bytecode  */
				EC_STACKPUSH( frame, compiled );					/* caller bytecode        */
				EC_STACKPUSH( frame, receiver );					/* caller object          */
				EC_STACKPUSH( frame, receiver_class );				/* caller object at_class */
				/* +3 */

				/* Put (explicit) arguments on callee's frame */
				for (i = op2 - 1; i >= 0; i--)
					EC_STACKSET( frame, i + 3, EC_STACKPOP( stack ) );

				/*
				 * Stack base now must point to the beginning of local variables, and
				 * stack pointer must be just after them, at the beginning of temporary position
				 */

				/* Set SP and BP */
				EC_STACKBP(frame)      = EC_STACKBASE(frame) + 1 + 2 + op2;
				EC_STACKPOINTER(frame) = EC_STACKBP(frame) + 0;

				ret = EcUnknownMethod( receiver, receiver_class, op1, frame );
				if (EC_ERRORP(ret)) goto on_error;
				EC_STACKPUSH( stack, ret );
				continue;
			}


			if (EC_CMETHODP(obj1))
			{
				EC_STACKNARGS_REAL(stack)    = op2;				/* set real num. of passed args    */
				EC_STACKNARGS_LOGICAL(stack) = op2;				/* set logical num. of passed args */

				/* Calling a method of self: put in 'self' and 'at_class' */
				ret = EC_CMETHOD(obj1)( receiver, receiver_class, stack );
				if (EC_ERRORP(ret)) goto on_error;
/*				ec_fprintf( stderr, "CMethod returns object %w\n", ret );*/
				EC_STACKPUSH( stack, ret );
				continue;
			} else
			{
				ASSERT( EC_COMPILEDP(obj1) );
				ASSERT( EC_COMPILEDNLOC(obj1) >= 0 );

				nargs = EC_COMPILEDNARG(obj1) - 2;				/* explicit arguments */

				/* Set on *CURRENT* stack the number of paramters, for error reporting */
				EC_STACKNARGS_REAL(stack)    = nargs + EC_COMPILEDVARG(obj1);	/* set real num. of passed args    */
				EC_STACKNARGS_LOGICAL(stack) = op2;								/* set logical num. of passed args */

				if (op2 < (nargs - EC_COMPILEDNARG_DEF(obj1)))
				{
					/* There are less arguments than necessary */
					EcParameterCountError( receiver, obj1, /* expected */ nargs - EC_COMPILEDNARG_DEF(obj1), stack );
					/*EcMakeException( EcExceptionClass,
									 "too few arguments to method (got %ld, expected %ld)",
									 op2, nargs - EC_COMPILEDNARG_DEF(obj1) );*/
					goto on_error;
				}

				/* if op2 < nargs we'll have to set unspecified parameters to @undef */

				if ((! EC_COMPILEDVARG(obj1) && (op2 > nargs)))
				{
					/* There are more arguments than necessary */
					EcParameterCountError( receiver, obj1, /* expected */ nargs, stack );
					/*EcMakeException( EcExceptionClass,
									 "too many arguments to method (got %ld, expected %ld)",
									 op2, nargs );*/
					goto on_error;
				}

				/* Prepare a stack frame for the call */
#if EC_STACK_RECYCLE
				RECYCLE_STACK_GET(frame,						/* where we want the result */
								  stack,						/* caller stack */
								  (1 + 2 +						/* dimension */
								   nargs + EC_COMPILEDVARG(obj1) +
								   EC_COMPILEDNLOC(obj1) + EC_COMPILEDMAXTEMPS(obj1)));

				/* STACK_INC_REF(frame); */
#else
				frame = EcMakeStack( stack,
									 1 + 2 +
									 nargs + EC_COMPILEDVARG(obj1) +
									 EC_COMPILEDNLOC(obj1) + EC_COMPILEDMAXTEMPS(obj1) );
#endif
				EC_STACKNARGS_REAL(frame)    = nargs + EC_COMPILEDVARG(obj1);	/* set real num. of passed args    */
				EC_STACKNARGS_LOGICAL(frame) = op2;								/* set logical num. of passed args */

				EC_STACKLEXICAL(frame) = EC_COMPILEDLEXICAL(obj1);
#if EC_STACK_RECYCLE
				if (EC_STACKP(EC_STACKLEXICAL(frame)))
					EC_STACKREF_INC(EC_STACKLEXICAL(frame));
#endif
				EC_STACKPC(frame) = iPC;							/* Program Counter */
				EC_STACKIMMUP(frame)     = EC_NIL;
				EC_STACKIMMCALLER(frame) = EC_NIL;
				EC_STACKIMMPC(frame)     = 0;
				EC_STACKCALLED(frame)    = obj1;					/* called bytecode  */
				EC_STACKPUSH( frame, compiled );					/* caller bytecode        */
				EC_STACKPUSH( frame, receiver );					/* caller object          */
				EC_STACKPUSH( frame, receiver_class );				/* caller object at_class */
				/* +3 */

				/* Put varargs on callee's frame */
				if (EC_COMPILEDVARG(obj1))
				{
					EC_OBJ vargs;

					/* BUG: vargs = EcMakeArray( op2 - EC_COMPILEDVARG(obj1) ); */
					vargs = EcMakeArray( op2 - nargs );
					EC_ARRAYLEN(vargs) = op2 - nargs;

					for (i = op2 - nargs - 1; i >= 0; i--)
					{
						ASSERT(EC_ARRAYLEN(vargs) > i);
						/* EcArraySet( vargs, i, EC_STACKPOP( stack ) ); */
						EC_ARRAYSET( vargs, i, EC_STACKPOP( stack ) );
					}

					EC_STACKSET( frame, nargs + 3, vargs );
				}

				/* Put (explicit) required arguments on callee's frame */
				if (op2 >= nargs)
				{
					for (i = nargs - 1; i >= 0; i--)
						EC_STACKSET( frame, i + 3, EC_STACKPOP( stack ) );
				} else
				{
					EcInt uninit_low;

					ASSERT( (op2 < nargs) && (op2 >= (nargs - EC_COMPILEDNARG_DEF(obj1))) );

					uninit_low = nargs - EC_COMPILEDNARG_DEF(obj1);
					if (op2 > uninit_low)
						uninit_low = op2;

					for (i = nargs - 1; i >= uninit_low; i--)	/* set parameters with default values to @undef */
						EC_STACKSET( frame, i + 3, EcUndefinedObject );
					for (i = op2 - 1; i >= 0; i--)				/* ... then use arguments                       */
						EC_STACKSET( frame, i + 3, EC_STACKPOP( stack ) );
				}

				/*
				 * Stack base now must point to the beginning of local variables, and
				 * stack pointer must be just after them, at the beginning of temporary position
				 */

				/* Set SP and BP */
				EC_STACKBP(frame)      = EC_STACKBASE(frame) + 1 + 2 + nargs + EC_COMPILEDVARG(obj1);
				EC_STACKPOINTER(frame) = EC_STACKBP(frame) + EC_COMPILEDNLOC(obj1);

				compiled = obj1;
				stack    = frame;
				LPRIVATE(rt.compiled) = compiled;

				self     = obj;
				at_class = receiver_class;
				code   = EC_COMPILEDCODE(compiled);
				ncode  = EC_COMPILEDNCODE(compiled);
				lframe = EC_COMPILEDLFRAME(compiled);
				codepc  = code;
				codeend = code + ncode;

				BACKTRACE( stack, compiled );
#if defined(WITH_STDIO) && TRACE_EXECUTION
				printf("\n");
				EcDumpCompiled( compiled, codepc - code );
				printf("\n");
				printf("LEXICALLY UPPER FRAME: 0x%08lX\n", (unsigned long)EC_STACKLEXICAL(stack));
#endif
			}
			continue;

		case CallSuperMethodOP:
			op1 = (EcInt)iFETCH;								/* method   */
			op2 = (EcInt)iFETCH;								/* nargs    */
			obj = self;											/* receiver */

/*			ec_fprintf( stderr, "Calling method %k for super of object %w, with %ld args.\n", op1, obj, op2 ); */

			ASSERT( EC_OBJECTP(obj) || EC_CLASSP(obj) );
			ASSERT( obj == self );

			CHECKGC;

			ASSERT( EC_OBJECTP(obj) || EC_CLASSP(obj) );

			if (EC_OBJECTP(obj))
			{
				receiver       = self;
				receiver_class = EC_CLASSSUPER(at_class);

/*				ec_fprintf( stderr, "Searching method %k\n", op1 );*/
				obj1 = EcFindMethod( receiver, &receiver_class, op1 );
/*				ec_fprintf( stderr, "Found method %k at class %w\n", op1, receiver_class );*/
			} else
			{
				receiver       = EC_CLASSSUPER(obj);
				receiver_class = EC_NIL;

				obj1 = EcFindClassMethod( receiver, &receiver_class, op1 );
			}

			if (EC_NULLP(obj1))
			{
				/* Method not found */

				/* TODO: try to get the stack from the stack pool */

				/* Prepare a stack frame for the call as if the method was found */
				frame = EcMakeStack( stack,
									 1 + 2 + op2 );
#if EC_STACK_RECYCLE
				/* STACK_INC_REF(frame); */
#endif
				EC_STACKNARGS_REAL(frame)    = op2;				/* set real num. of passed args    */
				EC_STACKNARGS_LOGICAL(frame) = op2;				/* set logical num. of passed args */

				EC_STACKLEXICAL(frame) = EC_NIL;				/* no method, no lexical... */
				EC_STACKPC(frame) = iPC;						/* Program Counter */
				EC_STACKIMMUP(frame)     = EC_NIL;
				EC_STACKIMMCALLER(frame) = EC_NIL;
				EC_STACKIMMPC(frame)     = 0;
				EC_STACKCALLED(frame)    = obj1;					/* called bytecode  */
				EC_STACKPUSH( frame, compiled );					/* caller bytecode        */
				EC_STACKPUSH( frame, receiver );					/* caller object          */
				EC_STACKPUSH( frame, receiver_class );				/* caller object at_class */
				/* +3 */

				/* Put (explicit) arguments on callee's frame */
				for (i = op2 - 1; i >= 0; i--)
					EC_STACKSET( frame, i + 3, EC_STACKPOP( stack ) );

				/*
				 * Stack base now must point to the beginning of local variables, and
				 * stack pointer must be just after them, at the beginning of temporary position
				 */

				/* Set SP and BP */
				EC_STACKBP(frame)      = EC_STACKBASE(frame) + 1 + 2 + op2;
				EC_STACKPOINTER(frame) = EC_STACKBP(frame) + 0;

				ret = EcUnknownMethod( receiver, receiver_class, op1, frame );
				if (EC_ERRORP(ret)) goto on_error;
				EC_STACKPUSH( stack, ret );
				continue;
			}

			if (EC_CMETHODP(obj1))
			{
				EC_STACKNARGS_REAL(stack)    = op2;				/* set real num. of passed args    */
				EC_STACKNARGS_LOGICAL(stack) = op2;				/* set logical num. of passed args */

				/* Calling a method of self: put in 'self' and 'at_class' */
				ret = EC_CMETHOD(obj1)( receiver, receiver_class, stack );
				if (EC_ERRORP(ret)) goto on_error;
/*				ec_fprintf( stderr, "CMethod returns object %w\n", ret );*/
				EC_STACKPUSH( stack, ret );
				continue;
			} else
			{
				ASSERT( EC_COMPILEDP(obj1) );
				ASSERT( EC_COMPILEDNLOC(obj1) >= 0 );

				nargs = EC_COMPILEDNARG(obj1) - 2;				/* explicit arguemnts */

				/* Set on *CURRENT* stack the number of paramters, for error reporting */
				EC_STACKNARGS_REAL(stack)    = nargs + EC_COMPILEDVARG(obj1);	/* set real num. of passed args    */
				EC_STACKNARGS_LOGICAL(stack) = op2;								/* set logical num. of passed args */

				if (op2 < (nargs - EC_COMPILEDNARG_DEF(obj1)))
				{
					/* There are less arguments than necessary */
					EcParameterCountError( receiver, obj1, /* expected */ nargs - EC_COMPILEDNARG_DEF(obj1), stack );
					/*EcMakeException( EcExceptionClass,
									 "too few arguments to method (got %ld, expected %ld)",
									 op2, nargs - EC_COMPILEDNARG_DEF(obj1));*/
					goto on_error;
				}

				/* if op2 < nargs we'll have to set unspecified parameters to @undef */

				if ((! EC_COMPILEDVARG(obj1) && (op2 > nargs)))
				{
					/* There are more arguments than necessary */
					EcParameterCountError( receiver, obj1, /* expected */ nargs, stack );
					/*EcMakeException( EcExceptionClass,
									 "too many arguments to method (got %ld, expected %ld)",
									 op2, nargs );*/
					goto on_error;
				}

				/* Prepare a stack frame for the call */
#if EC_STACK_RECYCLE
				RECYCLE_STACK_GET(frame,						/* where we want the result */
								  stack,						/* caller stack */
								  (1 + 2 +						/* dimension */
								   nargs + EC_COMPILEDVARG(obj1) +
								   EC_COMPILEDNLOC(obj1) + EC_COMPILEDMAXTEMPS(obj1)));

				/* STACK_INC_REF(frame); */
#else
				frame = EcMakeStack( stack,
									 1 + 2 +
									 nargs + EC_COMPILEDVARG(obj1) +
									 EC_COMPILEDNLOC(obj1) + EC_COMPILEDMAXTEMPS(obj1) );
#endif
				EC_STACKNARGS_REAL(frame)    = nargs + EC_COMPILEDVARG(obj1);	/* set real num. of passed args    */
				EC_STACKNARGS_LOGICAL(frame) = op2;								/* set logical num. of passed args */

				EC_STACKLEXICAL(frame) = EC_COMPILEDLEXICAL(obj1);
#if EC_STACK_RECYCLE
				if (EC_STACKP(EC_STACKLEXICAL(frame)))
					EC_STACKREF_INC(EC_STACKLEXICAL(frame));
#endif
				EC_STACKPC(frame) = iPC;							/* Program Counter */
				EC_STACKIMMUP(frame)     = EC_NIL;
				EC_STACKIMMCALLER(frame) = EC_NIL;
				EC_STACKIMMPC(frame)     = 0;
				EC_STACKCALLED(frame)    = obj1;					/* called bytecode  */
				EC_STACKPUSH( frame, compiled );					/* caller bytecode        */
				EC_STACKPUSH( frame, receiver );					/* caller object          */
				EC_STACKPUSH( frame, receiver_class );				/* caller object at_class */
				/* +3 */

				/* Put varargs on callee's frame */
				if (EC_COMPILEDVARG(obj1))
				{
					EC_OBJ vargs;

					/* BUG: vargs = EcMakeArray( op2 - EC_COMPILEDVARG(obj1) ); */
					vargs = EcMakeArray( op2 - nargs );
					EC_ARRAYLEN(vargs) = op2 - nargs;

					for (i = op2 - nargs - 1; i >= 0; i--)
					{
						ASSERT(EC_ARRAYLEN(vargs) > i);
						/* EcArraySet( vargs, i, EC_STACKPOP( stack ) ); */
						EC_ARRAYSET( vargs, i, EC_STACKPOP( stack ) );
					}

					EC_STACKSET( frame, nargs + 3, vargs );
				}

				/* Put (explicit) required arguments on callee's frame */
				if (op2 >= nargs)
				{
					for (i = nargs - 1; i >= 0; i--)
						EC_STACKSET( frame, i + 3, EC_STACKPOP( stack ) );
				} else
				{
					EcInt uninit_low;

					ASSERT( (op2 < nargs) && (op2 >= (nargs - EC_COMPILEDNARG_DEF(obj1))) );

					uninit_low = nargs - EC_COMPILEDNARG_DEF(obj1);
					if (op2 > uninit_low)
						uninit_low = op2;

					for (i = nargs - 1; i >= uninit_low; i--)	/* set parameters with default values to @undef */
						EC_STACKSET( frame, i + 3, EcUndefinedObject );
					for (i = op2 - 1; i >= 0; i--)				/* ... then use arguments                       */
						EC_STACKSET( frame, i + 3, EC_STACKPOP( stack ) );
				}

				/*
				 * Stack base now must point to the beginning of local variables, and
				 * stack pointer must be just after them, at the beginning of temporary position
				 */

				/* Set SP and BP */
				EC_STACKBP(frame)      = EC_STACKBASE(frame) + 1 + 2 + nargs + EC_COMPILEDVARG(obj1);
				EC_STACKPOINTER(frame) = EC_STACKBP(frame) + EC_COMPILEDNLOC(obj1);

				compiled = obj1;
				stack    = frame;
				LPRIVATE(rt.compiled) = compiled;

				self     = receiver;
				at_class = receiver_class;
				code   = EC_COMPILEDCODE(compiled);
				ncode  = EC_COMPILEDNCODE(compiled);
				lframe = EC_COMPILEDLFRAME(compiled);
				codepc  = code;
				codeend = code + ncode;

				BACKTRACE( stack, compiled );
#if defined(WITH_STDIO) && TRACE_EXECUTION
				printf("\n");
				EcDumpCompiled( compiled, codepc - code );
				printf("\n");
				printf("LEXICALLY UPPER FRAME: 0x%08lX\n", (unsigned long)EC_STACKLEXICAL(stack));
#endif
			}
			continue;

		case InlinedCallOP:
			op1 = (EcInt)iFETCH;								/* nargs  */
			obj = EC_STACKPOP( stack );							/* function to call */

			CHECKGC;

			ASSERT( EC_COMPILEDP(obj) );

			nargs = EC_COMPILEDNARG(obj);						/* explicit arguments */

			/* Set on *CURRENT* stack the number of paramters, for error reporting */
			EC_STACKNARGS_REAL(stack)    = nargs + EC_COMPILEDVARG(obj);	/* set real num. of passed args    */
			EC_STACKNARGS_LOGICAL(stack) = op1;								/* set logical num. of passed args */

			if (op1 < nargs)
			{
				/* There are less arguments than necessary */
				EcParameterCountError( EC_NIL, obj, /* expected */ nargs, stack );
				/*EcMakeException( EcExceptionClass,
								 "too few arguments to function (got %ld, expected %ld)",
								 op1, nargs );*/
				goto on_error;
			}

			if ((! EC_COMPILEDVARG(obj)) && (op1 > nargs))
			{
				/* There are more arguments than necessary */
				EcParameterCountError( EC_NIL, obj, /* expected */ nargs, stack );
				/*EcMakeException( EcExceptionClass,
								 "too many arguments to function (got %ld, expected %ld)",
								 op1, nargs );*/
				goto on_error;
			}

#if EC_STACK_RECYCLE
			RECYCLE_STACK_GET(frame,							/* where we want the result */
							  stack,							/* caller stack */
							  (1 + 2 +							/* dimenstion */
							   nargs + EC_COMPILEDVARG(obj) +
							   EC_COMPILEDNLOC(obj) + EC_COMPILEDMAXTEMPS(obj)));

			/* STACK_INC_REF(frame); */
#else

			/* Prepare a stack frame for the call */
			frame = EcMakeStack( stack,
								 1 + 2 +
								 nargs + EC_COMPILEDVARG(obj) +
								 EC_COMPILEDNLOC(obj) + EC_COMPILEDMAXTEMPS(obj) );
#endif
			EC_STACKNARGS_REAL(frame)    = nargs + EC_COMPILEDVARG(obj);	/* set real num. of passed args    */
			EC_STACKNARGS_LOGICAL(frame) = op1;								/* set logical num. of passed args */

			EC_STACKLEXICAL(frame) = EC_COMPILEDLEXICAL(obj);
#if EC_STACK_RECYCLE
			if (EC_STACKP(EC_STACKLEXICAL(frame)))
				EC_STACKREF_INC(EC_STACKLEXICAL(frame));
#endif
			EC_STACKIMMUP(frame)   = stack;
#if EC_STACK_RECYCLE
			/* STACK_INC_REF(stack); */
#endif
			EC_STACKIMMCALLER(frame) = compiled;				/* immediate return target */
			EC_STACKIMMPC(frame)     = iPC;
			EC_STACKCALLED(frame)    = obj;						/* called bytecode  */
			/* Duplicate "official" (master) return target on the slave stack */
			EC_STACKUP(frame) = EC_STACKUP(stack);				/* Master Program Counter   */
			EC_STACKPC(frame) = EC_STACKPC(stack);				/* Master Program Counter   */
			EC_STACKPUSH( frame, EC_STACKGET( stack, 0 ) );		/* (master) caller bytecode */
			EC_STACKPUSH( frame, self );						/* caller object            */
			EC_STACKPUSH( frame, at_class );					/* caller object at_class   */
			/* if (EC_STACKP(EC_STACKUP(stack))) {
			     STACK_INC_REF(EC_STACKUP(stack));
			   } */
			/* +3 */

			/* Put varargs on callee's frame */
			if (EC_COMPILEDVARG(obj))
			{
				EC_OBJ vargs;

				/* BUG: vargs = EcMakeArray( op1 - EC_COMPILEDVARG(obj) ); */
				vargs = EcMakeArray( op1 - nargs );
				EC_ARRAYLEN(vargs) = op1 - nargs;

				for (i = op1 - nargs - 1; i >= 0; i--)
				{
					ASSERT(EC_ARRAYLEN(vargs) > i);
					/* EcArraySet( vargs, i, EC_STACKPOP( stack ) ); */
					EC_ARRAYSET( vargs, i, EC_STACKPOP( stack ) );
				}

				EC_STACKSET( frame, nargs + 3, vargs );
			}

			/* Put arguments on callee's frame */
			for (i = op1 - 1; i >= 0; i--)
				EC_STACKSET( frame, i + 3, EC_STACKPOP( stack ) );

			/*
			 * Stack base now must point to the beginning of local variables, and
			 * stack pointer must be just after them, at the beginning of temporary position
			 */

			/* Set SP and BP */
			EC_STACKBP(frame)      = EC_STACKBASE(frame) + 1 + 2 + nargs + EC_COMPILEDVARG(obj);
			EC_STACKPOINTER(frame) = EC_STACKBP(frame) + EC_COMPILEDNLOC(obj);

/*			DMP("InlinedCallOP", frame, obj);*/

			compiled = obj;
			stack    = frame;
			LPRIVATE(rt.compiled) = compiled;

			code   = EC_COMPILEDCODE(compiled);
			ncode  = EC_COMPILEDNCODE(compiled);
			lframe = EC_COMPILEDLFRAME(compiled);
			codepc  = code;
			codeend = code + ncode;

			BACKTRACE( stack, compiled );
#if defined(WITH_STDIO) && TRACE_EXECUTION
			printf("\n");
			EcDumpCompiled( compiled, codepc - code );
			printf("\n");
			printf("LEXICALLY UPPER FRAME: 0x%08lX\n", (unsigned long)EC_STACKLEXICAL(stack));
#endif
			continue;

		case ReturnOP:
			ret = EC_STACKPOP( stack );
			ASSERT( ! EC_ERRORP(ret) );
			if (EC_ERRORP(ret)) goto on_error;
			compiled = EC_STACKGET( stack, 0 );
			self     = EC_STACKGET( stack, 1 );
			at_class = EC_STACKGET( stack, 2 );
			LPRIVATE(rt.compiled) = compiled;

/*			DMP("ReturnOP", stack, compiled);*/

			/* Returning to C code ? */
			if (EC_NULLP(compiled))
			{
				LPRIVATE(rt.vm_level)--;
				return ret;
			}

			code   = EC_COMPILEDCODE(compiled);
			ncode  = EC_COMPILEDNCODE(compiled);
			lframe = EC_COMPILEDLFRAME(compiled);
			codepc  = code + EC_STACKPC( stack );
			codeend = code + ncode;

#if EC_STACK_RECYCLE
			{
				EC_OBJ stackup;

				stackup = EC_STACKUP(stack);
				/* RECYCLE_STACK_PUT(stack); */
				EC_STACKREF_DEC(stack);							/* this should force a recycle if needed */
				ASSERT( EC_STACKP(stackup) );

				stack    = stackup;
				self     = EC_STACKGET( stack, 1 );				/* Added. Close bug (2) 01-12-1999 */
				at_class = EC_STACKGET( stack, 2 );				/* Added. Close bug (2) 01-12-1999 */
				EC_STACKPUSH( stack, ret );
			}
#else
			stack    = EC_STACKUP(stack);
			self     = EC_STACKGET( stack, 1 );					/* Added. Close bug (2) 01-12-1999 */
			at_class = EC_STACKGET( stack, 2 );					/* Added. Close bug (2) 01-12-1999 */
			EC_STACKPUSH( stack, ret );
#endif

#if defined(WITH_STDIO) && TRACE_EXECUTION
			printf("\n");
			EcDumpCompiled( compiled, codepc - code );
			printf("\n");
			printf("LEXICALLY UPPER FRAME: 0x%08lX\n", (unsigned long)EC_STACKLEXICAL(stack));
#endif
			continue;

		case InlinedReturnOP:
			ret = EC_STACKPOP( stack );
			ASSERT( ! EC_ERRORP(ret) );
			if (EC_ERRORP(ret)) goto on_error;
			compiled = EC_STACKIMMCALLER(stack);
			self     = EC_STACKGET( stack, 1 );
			at_class = EC_STACKGET( stack, 2 );
			LPRIVATE(rt.compiled) = compiled;

/*			DMP("InlinedReturnOP", stack, compiled);*/

			code   = EC_COMPILEDCODE(compiled);
			ncode  = EC_COMPILEDNCODE(compiled);
			lframe = EC_COMPILEDLFRAME(compiled);
			codepc  = code + EC_STACKIMMPC( stack );
			codeend = code + ncode;

#if EC_STACK_RECYCLE
			{
				EC_OBJ stackimmup;

				stackimmup = EC_STACKIMMUP(stack);
				/* RECYCLE_STACK_PUT(stack); */
				EC_STACKREF_DEC(stack);							/* this should force a recycle if needed */
				ASSERT( EC_STACKP(stackimmup) );

				stack = stackimmup;
			}
#else
			stack = EC_STACKIMMUP(stack);
#endif
			EC_STACKPUSH( stack, ret );
#if defined(WITH_STDIO) && TRACE_EXECUTION
			printf("\n");
			EcDumpCompiled( compiled, codepc - code );
			printf("\n");
			printf("LEXICALLY UPPER FRAME: 0x%08lX\n", (unsigned long)EC_STACKLEXICAL(stack));
#endif
			continue;

		case ThrowOP:
			CHECKGC;
			obj = EC_STACKPOP( stack );
			LPRIVATE(rt).exc = obj;
			goto on_error;
			break;

			/* Referencing bytecodes */

		case SequenceGetOP:
			CHECKGC;
			obj2 = EC_STACKPOP(stack);
			obj  = EC_STACKPOP(stack);
			ret = EcSequenceGetItem( obj, obj2 );
			if (EC_ERRORP(ret)) goto on_error;
			EC_STACKPUSH( stack, ret );
			continue;

		case SequenceSetOP:
			CHECKGC;
			obj2 = EC_STACKPOP(stack);
			obj1 = EC_STACKPOP(stack);
			obj  = EC_STACKPOP(stack);
			ret = EcSequenceSetItem( obj, obj1, obj2 );
			if (EC_ERRORP(ret)) goto on_error;
			EC_STACKPUSH( stack, ret );
			continue;

			/* Arithmetic bytecodes */

		case AddOP:
			CHECKGC;
			obj2 = EC_STACKPOP( stack );
			obj1 = EC_STACKPOP( stack );
			if (EC_INUMP(obj1) && EC_INUMP(obj2))
				ret = EcMakeInt( EC_INUM(obj1) + EC_INUM(obj2) );
			else {
				ret = ec_binop_add( obj1, obj2 );
				if (EC_ERRORP(ret)) goto on_error;
			}
			EC_STACKPUSH( stack, ret );
			continue;

		case SubOP:
			CHECKGC;
			obj2 = EC_STACKPOP( stack );
			obj1 = EC_STACKPOP( stack );
			ret = ec_binop_sub( obj1, obj2 );
			if (EC_ERRORP(ret)) goto on_error;
			EC_STACKPUSH( stack, ret );
			continue;

		case MulOP:
			CHECKGC;
			obj2 = EC_STACKPOP( stack );
			obj1 = EC_STACKPOP( stack );
			ret = ec_binop_mul( obj1, obj2 );
			if (EC_ERRORP(ret)) goto on_error;
			EC_STACKPUSH( stack, ret );
			continue;

		case DivOP:
			CHECKGC;
			obj2 = EC_STACKPOP( stack );
			obj1 = EC_STACKPOP( stack );
			ret = ec_binop_div( obj1, obj2 );
			if (EC_ERRORP(ret)) goto on_error;
			EC_STACKPUSH( stack, ret );
			continue;

		case ModOP:
			CHECKGC;
			obj2 = EC_STACKPOP( stack );
			obj1 = EC_STACKPOP( stack );
			ret = ec_binop_mod( obj1, obj2 );
			if (EC_ERRORP(ret)) goto on_error;
			EC_STACKPUSH( stack, ret );
			continue;

		case PowOP:
			CHECKGC;
			obj2 = EC_STACKPOP( stack );
			obj1 = EC_STACKPOP( stack );
			ret = ec_binop_pow( obj1, obj2 );
			if (EC_ERRORP(ret)) goto on_error;
			EC_STACKPUSH( stack, ret );
			continue;

		case LShiftOP:
			CHECKGC;
			obj2 = EC_STACKPOP( stack );
			obj1 = EC_STACKPOP( stack );
			ret = ec_binop_lshift( obj1, obj2 );
			if (EC_ERRORP(ret)) goto on_error;
			EC_STACKPUSH( stack, ret );
			continue;

		case RShiftOP:
			CHECKGC;
			obj2 = EC_STACKPOP( stack );
			obj1 = EC_STACKPOP( stack );
			ret = ec_binop_rshift( obj1, obj2 );
			if (EC_ERRORP(ret)) goto on_error;
			EC_STACKPUSH( stack, ret );
			continue;

		case AndOP:
			CHECKGC;
			obj2 = EC_STACKPOP( stack );
			obj1 = EC_STACKPOP( stack );
			ret = ec_binop_and( obj1, obj2 );
			if (EC_ERRORP(ret)) goto on_error;
			continue;

		case OrOP:
			CHECKGC;
			obj2 = EC_STACKPOP( stack );
			obj1 = EC_STACKPOP( stack );
			ret = ec_binop_or( obj1, obj2 );
			if (EC_ERRORP(ret)) goto on_error;
			continue;

		case XorOP:
			CHECKGC;
			obj2 = EC_STACKPOP( stack );
			obj1 = EC_STACKPOP( stack );
			ret = ec_binop_xor( obj1, obj2 );
			if (EC_ERRORP(ret)) goto on_error;
			continue;

		case NegOP:
			CHECKGC;
			ret = ec_unaryop_neg( EC_STACKPOP( stack ) );
			if (EC_ERRORP(ret)) goto on_error;
			EC_STACKPUSH( stack, ret );
			continue;

		case PosOP:
			CHECKGC;
			ret = ec_unaryop_pos( EC_STACKPOP( stack ) );
			if (EC_ERRORP(ret)) goto on_error;
			EC_STACKPUSH( stack, ret );
			continue;

		case InvertOP:
			CHECKGC;
			ret = ec_unaryop_invert( EC_STACKPOP( stack ) );
			if (EC_ERRORP(ret)) goto on_error;
			EC_STACKPUSH( stack, ret );
			continue;

		case InOP:
			CHECKGC;
			obj2 = EC_STACKPOP( stack );
			obj1 = EC_STACKPOP( stack );
			ret = EcSequenceIn( obj2, obj1 );
			if (EC_ERRORP(ret)) goto on_error;
			EC_STACKPUSH( stack, ret );
			continue;

		case LtOP:
			CHECKGC;
			obj2 = EC_STACKPOP( stack );
			obj1 = EC_STACKPOP( stack );
			if (EC_INUMP(obj1) && EC_INUMP(obj2))
				ret = EC_INUM_LT(obj1, obj2) ? EcTrueObject : EcFalseObject;
			else {
				ret = EcLibLt( obj1, obj2 );
				if (EC_ERRORP(ret)) goto on_error;
			}
			EC_STACKPUSH( stack, ret );
			continue;

		case GtOP:
			CHECKGC;
			obj2 = EC_STACKPOP( stack );
			obj1 = EC_STACKPOP( stack );
			if (EC_INUMP(obj1) && EC_INUMP(obj2))
				ret = EC_INUM_GT(obj1, obj2) ? EcTrueObject : EcFalseObject;
			else {
				ret = EcLibGt( obj1, obj2 );
				if (EC_ERRORP(ret)) goto on_error;
			}
			EC_STACKPUSH( stack, ret );
			continue;

		case LeOP:
			CHECKGC;
			obj2 = EC_STACKPOP( stack );
			obj1 = EC_STACKPOP( stack );
			if (EC_INUMP(obj1) && EC_INUMP(obj2))
				ret = EC_INUM_LE(obj1, obj2) ? EcTrueObject : EcFalseObject;
			else {
				ret = EcLibLe( obj1, obj2 );
				if (EC_ERRORP(ret)) goto on_error;
			}
			EC_STACKPUSH( stack, ret );
			continue;

		case GeOP:
			CHECKGC;
			obj2 = EC_STACKPOP( stack );
			obj1 = EC_STACKPOP( stack );
			if (EC_INUMP(obj1) && EC_INUMP(obj2))
				ret = EC_INUM_GE(obj1, obj2) ? EcTrueObject : EcFalseObject;
			else {
				ret = EcLibGe( obj1, obj2 );
				if (EC_ERRORP(ret)) goto on_error;
			}
			EC_STACKPUSH( stack, ret );
			continue;

		case EqOP:
			CHECKGC;
			obj2 = EC_STACKPOP( stack );
			obj1 = EC_STACKPOP( stack );
			if (EC_INUMP(obj1) && EC_INUMP(obj2))
				ret = EC_INUM_EQ(obj1, obj2) ? EcTrueObject : EcFalseObject;
			else {
				ret = EcLibEq( obj1, obj2 );
				if (EC_ERRORP(ret)) goto on_error;
			}
			EC_STACKPUSH( stack, ret );
			continue;

		case NeOP:
			CHECKGC;
			obj2 = EC_STACKPOP( stack );
			obj1 = EC_STACKPOP( stack );
			if (EC_INUMP(obj1) && EC_INUMP(obj2))
				ret = EC_INUM_EQ(obj1, obj2) ? EcFalseObject : EcTrueObject;
			else {
				ret = EcLibNe( obj1, obj2 );
				if (EC_ERRORP(ret)) goto on_error;
			}
			EC_STACKPUSH( stack, ret );
			continue;

		case LNotOP:
			CHECKGC;
			obj = EC_STACKPOP( stack );
			if (EC_BOOLP(obj))
			{
				ret = EC_BOOLVAL(obj) ? EcFalseObject : EcTrueObject;
			} else if (EC_INUMP(obj))
			{
				ret = EC_INUM(obj) ? EcFalseObject : EcTrueObject;
			} else
			{
				obj1 = ec_unaryop_true( obj );
				if (EC_ERRORP(obj1)) goto on_error;
				ret = (obj1 == EcTrueObject) ? EcFalseObject : EcTrueObject;
			}
			/* if (EC_ERRORP(ret)) goto on_error; */
			EC_STACKPUSH( stack, ret );
			continue;

		case SaveLexicalOP:
			CHECKGC;
			obj = EC_STACKPOP( stack );
			ASSERT( EC_COMPILEDP(obj) );
			ASSERT( EC_STACKP(stack) );
			EC_COMPILEDLEXICAL(obj) = stack;
#if EC_STACK_RECYCLE
			EC_STACKREF_INC(stack);
#endif
			EC_STACKPUSH( stack, obj );
			continue;

		case CopyOP:
			CHECKGC;
			op1 = (EcInt)iFETCH;								/* copy type (EcShallowCopyType or EcDeepCopyType) */
			obj = EC_STACKPOP( stack );
			ret = EcCopy( obj, (EcCopyType) op1 );
			if (EC_ERRORP(ret)) goto on_error;
			EC_STACKPUSH( stack, ret );
			continue;

		case ArrayConsOP:
			CHECKGC;
			op1 = (EcInt)iFETCH;								/* nargs */
			EC_STACKNARGS_REAL(stack)    = op1;
			EC_STACKNARGS_LOGICAL(stack) = op1;
			ret = EcLibArrayCons( stack, NULL );
			if (EC_ERRORP(ret)) goto on_error;
			EC_STACKPUSH( stack, ret );
			continue;

		case HashConsOP:
			CHECKGC;
			op1 = (EcInt)iFETCH;								/* nargs */
			EC_STACKNARGS_REAL(stack)    = op1;
			EC_STACKNARGS_LOGICAL(stack) = op1;
			ret = EcLibHashCons( stack, NULL );
			if (EC_ERRORP(ret)) goto on_error;
			EC_STACKPUSH( stack, ret );
			continue;

		case IncOP:
			CHECKGC;
			obj = EC_STACKPOP( stack );
			if (EC_INUMP(obj))
				ret = EcMakeInt( EC_INUM(obj) + 1 );
			else {
				ret = EcLibInc( obj );
				if (EC_ERRORP(ret)) goto on_error;
			}
			EC_STACKPUSH( stack, ret );
			continue;

		case DecOP:
			CHECKGC;
			obj = EC_STACKPOP( stack );
			ret = EcLibDec( obj );
			if (EC_ERRORP(ret)) goto on_error;
			EC_STACKPUSH( stack, ret );
			continue;
		}


	on_error:
		/* ------------------------------------------------------------------------
		 * E X C E P T I O N   H A N D L I N G
		 * ------------------------------------------------------------------------ */
		/*
		 * The exception has already been created here, so we have to retrofit the
		 * proper line number.
		 */
		LPRIVATE(rt.line_num) = EcCompiledGetLine( compiled, iPC - 1 );
		if (LPRIVATE(rt.exc_line_num) < 0)
			LPRIVATE(rt.exc_line_num) = LPRIVATE(rt.line_num);

		if (EC_NNULLP(LPRIVATE(rt).exc) &&
			EC_OBJECTP(LPRIVATE(rt).exc) &&
			EcIsOfClass(LPRIVATE(rt).exc, EcExceptionClass))
			EcSetInstanceVariable(LPRIVATE(rt).exc, EcExceptionClass,
								  "line", EcMakeInt( LPRIVATE(rt.exc_line_num) ) );
		sf   = stack;
		comp = compiled;
		while (EC_NNULLP(sf) && EC_NNULLP(comp))
		{
			ASSERT( EC_COMPILEDP(comp) );
			/* ec_fprintf( stderr, "sf: 0x%08lX   comp: 0x%08lX %w\n", (unsigned long)sf, (unsigned long)comp, comp ); */
			handlerchain = EC_COMPILEDHANDLER(comp);
			if (EC_NNULLP(handlerchain))
			{
				EC_OBJ handler;
				EcInt  i;

/*				ec_fprintf( stderr, "# handlers: %ld\n", EC_ARRAYLEN(handlerchain) );*/
				for (i = 0; i < EC_ARRAYLEN(handlerchain); i++)
				{
					ASSERT(EC_ARRAYLEN(handlerchain) > i);
/*					handler = EC_ARRAYMEM(handlerchain)[i];*/
					handler = EC_ARRAYGET(handlerchain, i);

					/* ec_fprintf( stderr, "Exception object: %w\n", LPRIVATE(rt).exc ); */
					/* ec_fprintf( stderr, "Handler type    : %w\n", EC_HANDLERTYPE(handler) );*/
					if (EcIsOfClass( LPRIVATE(rt).exc, EC_HANDLERTYPE(handler) ))
					{
						/*
						 * Found a suitable handler. Call it.
						 *
						 * Remember that an handler is an inlined code block, so
						 * it will do an inlined return. We must give it the correct
						 * immediate return target (that is the same immediate target
						 * of the corresponding try block).
						 */

						obj = EC_HANDLERCODE(handler);

						ASSERT( EC_COMPILEDP(obj) );

						nargs = EC_COMPILEDNARG(obj);

						/* Here the number of args can be 0 (no exception passed) or 1 (the exception is passed) */
						ASSERT( (nargs == 0) || (nargs == 1) );
						ASSERT( EC_COMPILEDVARG(obj) == FALSE );

						/*
						 * Prepare a stack frame for the call
						 *
						 * Now sf is the frame of the try block.
						 */
						/* TODO: try to get the stack from the stack pool */
						/* The stack frame is a child of the handler containing block */
						/* TODO XXXX: check EC_STACKUP(sf) as up */
						frame = EcMakeStack( EC_STACKUP(sf),			/* same as corresponding try block */
											 1 + 2 +
											 nargs + EC_COMPILEDVARG(obj) +
											 EC_COMPILEDNLOC(obj) + EC_COMPILEDMAXTEMPS(obj) );
#if EC_STACK_RECYCLE
						/* STACK_INC_REF(frame); */
#endif
						EC_STACKNARGS_REAL(frame)    = nargs + EC_COMPILEDVARG(obj);	/* set real num. of passed args    */
						EC_STACKNARGS_LOGICAL(frame) = nargs;							/* set logical num. of passed args */

						EC_STACKLEXICAL(frame)   = EC_COMPILEDLEXICAL(obj); /* TODO XXXX: check !!! */
#if EC_STACK_RECYCLE
						if (EC_STACKP(EC_STACKLEXICAL(frame)))
							EC_STACKREF_INC(EC_STACKLEXICAL(frame));
#endif
/* inlined return */    EC_STACKIMMUP(frame)     = EC_STACKIMMUP(sf);		    /* same as corresponding try block */
						EC_STACKIMMCALLER(frame) = EC_STACKIMMCALLER(sf);	    /* same as corresponding try block */
						EC_STACKIMMPC(frame)     = EC_STACKIMMPC(sf);	        /* idem                            */
						EC_STACKCALLED(frame)    = obj;							/* called bytecode  */
/* normal return */		EC_STACKPC(frame) = EC_STACKPC(sf);			        	/* idem for the master caller PC   */
						EC_STACKPUSH( frame, EC_STACKGET( sf, 0 ) );			/* idem for the master caller      */
						EC_STACKPUSH( frame, EC_STACKGET( sf, 1 ) );			/* idem for caller object          */
						EC_STACKPUSH( frame, EC_STACKGET( sf, 2 ) );			/* idem for caller object at_class */
						/* +3 */

						/* Put arguments on callee's frame */
						ASSERT( (nargs == 0) || (nargs == 1) );
						ASSERT( EC_COMPILEDVARG(obj) == FALSE );
						if (nargs)
							EC_STACKSET( frame, 3, LPRIVATE(rt).exc );

						/*
						 * Stack base now must point to the beginning of local variables, and
						 * stack pointer must be just after them, at the beginning of temporary position
						 */

						/* Set SP and BP */
						EC_STACKBP(frame)      = EC_STACKBASE(frame) + 1 + 2 + nargs + EC_COMPILEDVARG(obj);
						EC_STACKPOINTER(frame) = EC_STACKBP(frame) + EC_COMPILEDNLOC(obj);

						compiled = obj;
						stack    = frame;
						LPRIVATE(rt.compiled) = compiled;

						code   = EC_COMPILEDCODE(compiled);
						ncode  = EC_COMPILEDNCODE(compiled);
						lframe = EC_COMPILEDLFRAME(compiled);
						codepc  = code;
						codeend = code + ncode;

						BACKTRACE( stack, compiled );
#if defined(WITH_STDIO) && TRACE_EXECUTION
						printf("\n");
						EcDumpCompiled( compiled, codepc - code );
						printf("\n");
						printf("LEXICALLY UPPER FRAME: 0x%08lX\n", (unsigned long)EC_STACKLEXICAL(stack));
#endif
						LPRIVATE(rt.exc_line_num) = -1;			/* reset exception origin */
						goto restart;
					}
				}
			}

			/* Try upper stack frame */
			if (EC_NNULLP(EC_STACKIMMUP(sf))) {					/* If there are other try/catch blocks in this function... */
				comp = EC_STACKIMMCALLER( sf );
				sf   = EC_STACKIMMUP(sf);
			} else {											/* otherwise... */
				comp = EC_STACKGET( sf, 0 );
				sf   = EC_STACKUP(sf);
			}
			/* ec_fprintf( stderr, "NOW sf: 0x%08lX   comp: 0x%08lX %w\n", (unsigned long)sf, (unsigned long)comp, comp ); */
		}

		if (LPRIVATE(rt.vm_level) == 0)
		{
			EC_OBJ in_bytecode, in_package = EC_NIL, in_source = EC_NIL;

			backtrace( stack, compiled );

			/* Uncaught exception ! */
			if (EC_OBJECTP(LPRIVATE(rt).exc) && EcIsOfClass( LPRIVATE(rt).exc, EcExceptionClass ))
			{
				EC_OBJ msg;

				in_bytecode = EcGetInstanceVariable( LPRIVATE(rt).exc, EcExceptionClass, "in" );
				if (EC_COMPILEDP(in_bytecode))
				{
					in_package = EC_COMPILEDPACKAGE(in_bytecode);
					if (EC_PACKAGESOURCE(in_package)) in_source = EC_PACKAGESOURCE(in_package);
				}

				msg = EcGetInstanceVariable( LPRIVATE(rt).exc, EcExceptionClass, "msg" );
				if (EC_NNULLP(in_package) || EC_NNULLP(in_source))
					EcAlert( EcFatal, "uncaught exception (package: %w, %w, line: %ld)\n\t %w: %w\n",
							 EC_PACKAGENAME(in_package), in_source, (long) LPRIVATE(rt.exc_line_num), LPRIVATE(rt).exc, msg );
				else
					EcAlert( EcFatal, "uncaught exception (line: %ld)\n\t %w: %w\n",
							 (long) LPRIVATE(rt.exc_line_num), LPRIVATE(rt).exc, msg );
				LPRIVATE(rt.exc_line_num) = -1;			/* reset exception origin */
			} else
			{
				in_bytecode = LPRIVATE(rt.compiled);
				if (EC_COMPILEDP(in_bytecode))
				{
					in_package = EC_COMPILEDPACKAGE(in_bytecode);
					if (EC_PACKAGESOURCE(in_package)) in_source = EC_PACKAGESOURCE(in_package);
				}

				if (EC_NNULLP(in_package) || EC_NNULLP(in_source))
					EcAlert( EcFatal, "uncaught exception (package: %w, %w, line: %ld)\n\t %w\n",
							 EC_PACKAGENAME(in_package), in_source, (long) LPRIVATE(rt.exc_line_num), LPRIVATE(rt).exc );
				else
					EcAlert( EcFatal, "uncaught exception (line: %ld) %w\n", (long) LPRIVATE(rt.exc_line_num), LPRIVATE(rt).exc );
				LPRIVATE(rt.exc_line_num) = -1;			/* reset exception origin */
			}
		} else
		{
			LPRIVATE(rt.vm_level)--;
			return Ec_ERROR;
		}
	}

	if (EC_STACKPOINTER(stack) - EC_STACKBASE(stack) > 2)
	{
		LPRIVATE(rt.vm_level)--;
		return EC_STACKPOP(stack);
	}

	LPRIVATE(rt.vm_level)--;
	return EC_NIL;
}


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

	ec_msg_printf( "\nBACKTRACE (stack depth %ld):\n", (long)stackdepth );

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

		if (line_num > 0)
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

		ec_msg_printf( ")\n" );

		depth--;
		if (EC_NNULLP(EC_STACKIMMCALLER(sf)))
			compiled = EC_STACKIMMCALLER(sf);
		else
			compiled = EC_STACKGET(sf, 0);
		calledsf = sf;
		sf       = EC_STACKUP(sf);
	}

	ec_msg_printf( "\n" );
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
