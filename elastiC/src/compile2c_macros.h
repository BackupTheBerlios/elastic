/* ==========================================================================
 * compile2c_macros
 * ==========================================================================
 * elastiC C compiler macros file.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Jacopo Pantaleoni         E-mail: jp@lightflowtech.com
 *
 *   Created: Sun May 25 08:05:50 MEST 2003
 *
 *   $Id: compile2c_macros.h,v 1.1 2003/05/29 10:36:00 panta Exp $
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

#ifndef __COMPILE2C_MACROS_H
#define __COMPILE2C_MACROS_H

#define C_PRE_OP \
{\
}

#define C_POST_OP \
{\
}

#define C_PUSH_HERE_OP( op1 ) \
{\
	iFETCH;\
	EC_STACKPUSH( stack, EC_STACKBPGET( stack, op1 ) );\
}

#define C_POP_HERE_OP( op1 ) \
{\
	iFETCH;\
	EC_STACKBP(stack)[op1] = EC_STACKPOP( stack );\
}

#define C_PUSH_PARAM_OP( op1, op2 ) \
{\
	register EC_OBJ obj = EC_NIL;\
	EC_OBJ sf;\
\
	iFETCH;\
	iFETCH;\
	sf = stack;\
	ASSERT( EC_NNULLP(sf) );\
	ASSERT( op1 >= 0 );\
	{\
		EcInt op = op1;\
		while (EC_NNULLP(sf) && (op >= 0))\
		{\
			if (op == 0)\
			{\
				obj = EC_STACKGET( sf, op2 );\
				break;\
			}\
\
			sf = EC_STACKLEXICAL(sf);\
			op--;\
		}\
		ASSERT( op == 0 );\
	}\
	EC_STACKPUSH( stack, obj );\
}

#define C_POP_PARAM_OP( op1, op2 ) \
{\
	register EC_OBJ obj, *where;\
\
	iFETCH;\
	iFETCH;\
	where = findBASE( stack, op1, op2 );\
	obj = EC_STACKPOP( stack );\
	*where = obj;\
/*	EC_STACKBASE(stack)[op1] = obj;*/\
}

#define C_PUSH_OP( op1, op2 ) \
{\
	register EC_OBJ obj = EC_NIL;\
	EC_OBJ sf;\
\
	iFETCH;\
	iFETCH;\
	sf = stack;\
	ASSERT( EC_NNULLP(sf) );\
	ASSERT( op1 >= 0 );\
	{\
		EcInt op = op1;\
		while (EC_NNULLP(sf) && (op >= 0))\
		{\
			if (op == 0)\
			{\
				obj = EC_STACKBPGET( sf, op2 );\
				break;\
			}\
\
			sf = EC_STACKLEXICAL(sf);\
			op--;\
		}\
	}\
	EC_STACKPUSH( stack, obj );\
}

#define C_PUSH_LITERAL_OP( op1, op2 ) \
{\
	register EC_OBJ obj = EC_NIL;\
	EC_OBJ comp, lf, sf;\
\
	iFETCH;\
	iFETCH;\
	if (op1 == 0)\
	{\
		/* Special case: most frequent case */\
		comp = EC_STACKCALLED(stack);\
		lf   = EC_COMPILEDLFRAME(comp);\
		ASSERT(EC_ARRAYLEN(lf) > op2);\
		/* obj  = EcArrayGet( lf, op2 ); */\
		obj  = EC_ARRAYGET( lf, op2 );\
		EC_STACKPUSH( stack, obj );\
	}\
	else\
	{\
		EcInt op = op1;\
		sf   = stack;\
		comp = compiled;\
		ASSERT( comp == EC_STACKCALLED(sf) );\
		while (EC_NNULLP(sf) && (op >= 0))					/* go to the right frame */\
		{\
			if (op == 0)\
			{\
				comp = EC_STACKCALLED(sf);\
				lf   = EC_COMPILEDLFRAME(comp);\
				ASSERT(EC_ARRAYLEN(lf) > op2);\
				/* obj  = EcArrayGet( lf, op2 ); */\
				obj  = EC_ARRAYGET( lf, op2 );\
				break;\
			}\
\
			sf = EC_STACKLEXICAL(sf);\
			op--;\
		}\
/*		printf( "PushLiteral: " ); ec_fprintf( stdout, "%w\n", obj ); */\
		EC_STACKPUSH( stack, obj );\
	}\
}

#define C_PUSH_PACKAGE_OP( op1, op2 ) \
{\
	register EC_OBJ obj;\
\
	iFETCH;\
	iFETCH;\
	ASSERT( op1 < LPRIVATE(npackages) );\
	ASSERT(EC_ARRAYLEN(EC_PACKAGEFRAME(LPRIVATE(package)[op1].obj)) > op2);\
	/* obj = EcArrayGet( EC_PACKAGEFRAME(LPRIVATE(package)[op1].obj), op2 ); */\
	obj = EC_ARRAYGET( EC_PACKAGEFRAME(LPRIVATE(package)[op1].obj), op2 );\
	EC_STACKPUSH( stack, obj );\
}

#define C_POP_OP( op1, op2 ) \
{\
	register EC_OBJ obj, *where;\
\
	iFETCH;\
	iFETCH;\
	where = findBP( stack, op1, op2 );\
	obj = EC_STACKPOP( stack );\
	*where = obj;\
}

#define C_POP_LITERAL_OP( op1, op2 ) \
{\
	register EC_OBJ obj, *where;\
	EC_OBJ comp, lf, sf;\
	EcInt  op = op1;\
\
	iFETCH;\
	iFETCH;\
	sf   = stack;\
	comp = compiled;\
	ASSERT( comp == EC_STACKCALLED(sf) );\
	where = NULL;\
	while (EC_NNULLP(sf) && (op >= 0))					/* go to the right frame */\
	{\
		if (op == 0)\
		{\
			comp  = EC_STACKCALLED(sf);\
			lf    = EC_COMPILEDLFRAME(comp);\
			ASSERT(EC_ARRAYLEN(lf) > op2);\
/*			where = &(EC_ARRAYMEM(lf)[op2]);*/\
			where = EC_ARRAYGETR(lf, op2);\
			break;\
		}\
\
		sf = EC_STACKLEXICAL(sf);\
		op--;\
	}\
	ASSERT( where );\
	/* where = findLiteral( stack, op, op2 ); */\
	obj = EC_STACKPOP( stack );\
	*where = obj;\
}

#define C_POP_PACKAGE_OP( op1, op2 ) \
{\
	register EC_OBJ obj;\
\
	iFETCH;\
	iFETCH;\
	obj = EC_STACKPOP( stack );\
	ASSERT(EC_ARRAYLEN(EC_PACKAGEFRAME(LPRIVATE(package)[op1].obj)) > op2);\
	/* EcArraySet( EC_PACKAGEFRAME(LPRIVATE(package)[op1].obj), op2, obj ); */\
	EC_ARRAYSET( EC_PACKAGEFRAME(LPRIVATE(package)[op1].obj), op2, obj );\
}

#define C_PUSH_INSTANCE_OP( op1 ) \
{\
	register EC_OBJ obj;\
\
	iFETCH;\
	if (EC_OBJECTP(self))\
	{\
		obj = EC_OBJECTIVARS(self)[EC_CLASSIOFFSET(at_class) + op1];\
	} else\
	{\
		ASSERT( EC_CLASSP(self) );\
		ASSERT( EC_CLASSP(at_class) );\
		ASSERT(EC_ARRAYLEN(EC_CLASSLFRAME(at_class)) > op1);\
/*		obj = EC_ARRAYMEM(EC_CLASSLFRAME(at_class))[op1];*/\
		obj = EC_ARRAYGET(EC_CLASSLFRAME(at_class), op1);\
	}\
	EC_STACKPUSH( stack, obj );\
}

#define C_POP_INSTANCE_OP( op1 ) \
{\
	register EC_OBJ obj;\
\
	iFETCH;\
	obj = EC_STACKPOP( stack );\
	if (EC_OBJECTP(self))\
	{\
		EC_OBJECTIVARS(self)[EC_CLASSIOFFSET(at_class) + op1] = obj;\
	} else\
	{\
		ASSERT( EC_CLASSP(self) );\
		ASSERT( EC_CLASSP(at_class) );\
		ASSERT(EC_ARRAYLEN(EC_CLASSLFRAME(at_class)) > op1);\
/*		EC_ARRAYMEM(EC_CLASSLFRAME(at_class))[op1] = obj;*/\
		EC_ARRAYSET(EC_CLASSLFRAME(at_class), op1, obj);\
	}\
}

#define C_PUSH_CLASS_OP( op1 ) \
{\
	register EC_OBJ obj;\
\
	iFETCH;\
	obj = EcArrayGet( EC_CLASSLFRAME(at_class), op1 );\
	EC_STACKPUSH( stack, obj );\
}

#define C_POP_CLASS_OP( op1 ) \
{\
	register EC_OBJ obj, *where;\
\
	iFETCH;\
	obj = EC_STACKPOP( stack );\
	EcArraySet( EC_CLASSLFRAME(at_class), op1, obj );\
}

#if 0
#define C_DUP_OP \
{\
	register EC_OBJ obj;\
\
	obj = EC_STACKPOP( stack );\
	EC_STACKPUSH( stack, obj );\
	EC_STACKPUSH( stack, obj );\
}
#else
#define C_DUP_OP \
{\
	EC_STACKDUP( stack );\
}
#endif

#define C_DISCARD_OP \
{\
	/* This should sound like EC_STACKPOP(stack) */\
	--EC_STACKPOINTER(stack);\
}

#define C_PUSH_STACK_OP \
{\
	EC_STACKPUSH( stack, stack );\
}

#define C_GET_OP( op1 ) \
{\
	register EC_OBJ obj;\
\
	iFETCH;\
	obj = *(EC_STACKPOINTER(stack) - 1 - op1);\
	EC_STACKPUSH( stack, obj );\
}

#if USE_DIRECT_TRUTHTESTING

#define C_BRANCH_IF_TRUE_OP( op1 ) \
{\
	register EC_OBJ obj;\
\
	iFETCH;\
	obj = EC_STACKPOP( stack );\
	if (EC_ASBOOLVAL(obj))\
	{\
		codepc = code + op1;\
		goto label_##op1;\
	}\
}

#define C_BRANCH_IF_FALSE_OP( op1 ) \
{\
	register EC_OBJ obj;\
\
	iFETCH;\
	obj = EC_STACKPOP( stack );\
	if (! EC_ASBOOLVAL(obj))\
	{\
		codepc = code + op1;\
		goto label_##op1;\
	}\
}

#else

#define C_BRANCH_IF_TRUE_OP( op1 ) \
{\
	register EC_OBJ obj;\
\
	iFETCH;\
	obj = EC_STACKPOP( stack );\
	if (EC_BOOLP(obj))\
	{\
		if (EC_BOOLVAL(obj))\
		{\
			codepc = code + op1;\
			goto label_##op1;\
		}\
	} else if (EC_INUMP(obj))\
	{\
		if (EC_INUM(obj))\
		{\
			codepc = code + op1;\
			goto label_##op1;\
		}\
	} else if (ec_unaryop_true( obj ) == EcTrueObject)\
	{\
		codepc = code + op1;\
		goto label_##op1;\
	}\
}

#define C_BRANCH_IF_FALSE_OP( op1 ) \
{\
	register EC_OBJ obj;\
\
	iFETCH;\
	obj = EC_STACKPOP( stack );\
	if (EC_BOOLP(obj))\
	{\
		if (! EC_BOOLVAL(obj))\
			goto label_##op1;\
	} else if (EC_INUMP(obj))\
	{\
		if (! EC_INUM(obj))\
		{\
			codepc = code + op1;\
			goto label_##op1;\
		}\
	} else if (ec_unaryop_true( obj ) == EcFalseObject)\
	{\
		codepc = code + op1;\
		goto label_##op1;\
	}\
}

#endif

#define C_BRANCH_IF_DEFINED_OP( op1 ) \
{\
	register EC_OBJ obj;\
\
	iFETCH;\
	obj = EC_STACKPOP( stack );\
	if (! EC_UNDEFINEDP(obj))\
	{\
		codepc = code + op1;\
		goto label_##op1;\
	}\
}

#define C_JUMP_OP( op1 ) \
{\
	iFETCH;\
	codepc = code + op1;								/* jump to */\
	goto label_##op1;\
}

EcInt call_op(EcInt op1,
			  EcPrivate* lprivate,
			  EC_OBJ self, EC_OBJ at_class, EC_OBJ compiled, EC_OBJ stack,
			  EcBytecode *code,
			  EcBytecode *codepc)
{
	register EC_OBJ ret, obj, frame;
	EcInt  nargs;
	EcInt  i;

	obj = EC_STACKPOP( stack );							/* function to call */

	CHECKGC;

	if (EC_PRIMITIVEP(obj))
	{
		EC_STACKNARGS_REAL(stack) = op1;				/* set real num. of passed args    */
		EC_STACKNARGS_LOGICAL(stack) = op1;				/* set logical num. of passed args */

		ret = EC_PRIMITIVE(obj)( stack, EC_PRIMITIVEUSERDATA(obj) );
		if (EC_ERRORP(ret)) return 0;
		EC_STACKPUSH( stack, ret );
	} else
	{
		if (! EC_COMPILEDP(obj))
		{
			EcUnimplementedError( "non-callable object called" );
			return 0;
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
			return 0;
		}

		/* if op1 < nargs we'll have to set unspecified parameters to @undef */

		if ((! EC_COMPILEDVARG(obj)) && (op1 > nargs))
		{
			/* There are more arguments than necessary */
			EcParameterCountError( EC_NIL, obj, /* expected */ nargs, stack );
			/*EcMakeException( EcExceptionClass,
			  "too many arguments to function %w (got %ld, expected %ld)",
			  obj, op1, nargs );*/
			return 0;
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
		EC_STACKPUSH( frame, NULL /* compiled */ );			/* caller bytecode        */
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

		if (EC_COMPILEDCCALLABLE(obj)) /* Check for a C-callable */
			ret = EC_COMPILEDCCALLABLE(obj)( self, at_class, obj, frame );
		else
			ret = EcExecute( self, at_class, obj, frame );

		if (EC_ERRORP(ret)) return 0;
#if EC_STACK_RECYCLE
		/* RECYCLE_STACK_PUT(frame); */
		EC_STACKREF_DEC(frame);							/* this should force a recycle if needed */
#endif
		EC_STACKPUSH( stack, ret );
		LPRIVATE(rt.compiled) = compiled;

		/* Returning to C code ? */
/*		if (EC_NULLP(compiled))
		{
			LPRIVATE(rt.vm_level)--;
			return ret;
		}*/
	}
	return 1;
}

#define C_CALL_OP( op1 ) \
{\
	iFETCH;\
	if (! call_op( op1, lprivate,\
				   self, at_class, compiled, stack,\
				   code, codepc ))\
		goto on_error;\
}

EcInt call_method_op(EcInt op1, EcInt op2,
					 EcPrivate* lprivate,
					 EC_OBJ self, EC_OBJ at_class, EC_OBJ compiled, EC_OBJ stack,
					 EcBytecode *code,
					 EcBytecode *codepc)
{
	register EC_OBJ ret, obj, obj1, frame;
	EC_OBJ receiver, receiver_class;
	EcInt  nargs;
	EcInt  i;

	obj = EC_STACKPOP( stack );							/* receiver */

/*	ec_fprintf( stderr, "Calling method %k for object %w, with %ld args.\n", op1, obj, op2 );*/

/*	if (! (EC_OBJECTP(obj) || EC_CLASSP(obj)))
	{
		ec_fprintf(stderr, "[1] 0x%08lX %W\n", obj, obj);
		backtrace(stack, compiled);
	}*/
	ASSERT( EC_OBJECTP(obj) || EC_CLASSP(obj) );
	/*ec_fprintf(stderr, "[1b] %W\n", obj);*/

	CHECKGC;

/*	if (! (EC_OBJECTP(obj) || EC_CLASSP(obj)))
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
/*		ec_fprintf( stderr, "Found method %k at class %w\n", op1, receiver_class );*/
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
		EC_STACKPUSH( frame, NULL /* compiled */ );			/* caller bytecode        */
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
		if (EC_ERRORP(ret)) return 0;
		EC_STACKPUSH( stack, ret );
		return 1;
	}


	if (EC_CMETHODP(obj1))
	{
		EC_STACKNARGS_REAL(stack)    = op2;				/* set real num. of passed args    */
		EC_STACKNARGS_LOGICAL(stack) = op2;				/* set logical num. of passed args */

		/* Calling a method of self: put in 'self' and 'at_class' */
		ret = EC_CMETHOD(obj1)( receiver, receiver_class, stack );
		if (EC_ERRORP(ret)) return 0;
/*		ec_fprintf( stderr, "CMethod returns object %w\n", ret );*/
		EC_STACKPUSH( stack, ret );
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
			return 0;
		}

		/* if op2 < nargs we'll have to set unspecified parameters to @undef */

		if ((! EC_COMPILEDVARG(obj1) && (op2 > nargs)))
		{
			/* There are more arguments than necessary */
			EcParameterCountError( receiver, obj1, /* expected */ nargs, stack );
			/*EcMakeException( EcExceptionClass,
			  "too many arguments to method (got %ld, expected %ld)",
			  op2, nargs );*/
			return 0;
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
		EC_STACKPUSH( frame, NULL /* compiled */ );			/* caller bytecode        */
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

		if (EC_COMPILEDCCALLABLE(obj1)) /* Check for a C-callable #JP */
			ret = EC_COMPILEDCCALLABLE(obj1)( obj, receiver_class, obj1, frame );
		else
			ret = EcExecute( obj, receiver_class, obj1, frame );

		if (EC_ERRORP(ret)) return 0;
#if EC_STACK_RECYCLE
		/* RECYCLE_STACK_PUT(frame); */
		EC_STACKREF_DEC(frame);							/* this should force a recycle if needed */
#endif
		EC_STACKPUSH( stack, ret );
	}
	return 1;
}

#define C_CALL_METHOD_OP( op1, op2 ) \
{\
	iFETCH;\
	iFETCH;\
	if (! call_method_op( op1, op2, lprivate,\
						  self, at_class, compiled, stack,\
						  code, codepc ))\
		goto on_error;\
}

EcInt call_super_method_op(EcInt op1, EcInt op2,
						   EcPrivate* lprivate,
						   EC_OBJ self, EC_OBJ at_class, EC_OBJ compiled, EC_OBJ stack,
						   EcBytecode *code,
						   EcBytecode *codepc)
{
	register EC_OBJ ret, obj, obj1, frame;
	EC_OBJ receiver, receiver_class;
	EcInt  nargs;
	EcInt  i;

	obj = self;											/* receiver */

/*	ec_fprintf( stderr, "Calling method %k for super of object %w, with %ld args.\n", op1, obj, op2 ); */

	ASSERT( EC_OBJECTP(obj) || EC_CLASSP(obj) );
	ASSERT( obj == self );

	CHECKGC;

	ASSERT( EC_OBJECTP(obj) || EC_CLASSP(obj) );

	if (EC_OBJECTP(obj))
	{
		receiver       = self;
		receiver_class = EC_CLASSSUPER(at_class);

/*		ec_fprintf( stderr, "Searching method %k\n", op1 );*/
		obj1 = EcFindMethod( receiver, &receiver_class, op1 );
/*		ec_fprintf( stderr, "Found method %k at class %w\n", op1, receiver_class );*/
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
		EC_STACKPUSH( frame, NULL /* compiled */ );			/* caller bytecode        */
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
		if (EC_ERRORP(ret)) return 0;
		EC_STACKPUSH( stack, ret );
		return 1;
	}

	if (EC_CMETHODP(obj1))
	{
		EC_STACKNARGS_REAL(stack)    = op2;				/* set real num. of passed args    */
		EC_STACKNARGS_LOGICAL(stack) = op2;				/* set logical num. of passed args */

		/* Calling a method of self: put in 'self' and 'at_class' */
		ret = EC_CMETHOD(obj1)( receiver, receiver_class, stack );
		if (EC_ERRORP(ret)) return 0;
/*		ec_fprintf( stderr, "CMethod returns object %w\n", ret );*/
		EC_STACKPUSH( stack, ret );
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
			return 0;
		}

		/* if op2 < nargs we'll have to set unspecified parameters to @undef */

		if ((! EC_COMPILEDVARG(obj1) && (op2 > nargs)))
		{
			/* There are more arguments than necessary */
			EcParameterCountError( receiver, obj1, /* expected */ nargs, stack );
			/*EcMakeException( EcExceptionClass,
			  "too many arguments to method (got %ld, expected %ld)",
			  op2, nargs );*/
			return 0;
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
		EC_STACKPUSH( frame, NULL /* compiled */ );			/* caller bytecode        */
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

		if (EC_COMPILEDCCALLABLE(obj1)) /* Check for a C-callable #JP */
			ret = EC_COMPILEDCCALLABLE(obj1)( receiver, receiver_class, obj1, frame );
		else
			ret = EcExecute( receiver, receiver_class, obj1, frame );

		if (EC_ERRORP(ret)) return 0;
#if EC_STACK_RECYCLE
		/* RECYCLE_STACK_PUT(frame); */
		EC_STACKREF_DEC(frame);							/* this should force a recycle if needed */
#endif
		EC_STACKPUSH( stack, ret );
	}
	return 1;
}

#define C_CALL_SUPER_METHOD_OP( op1, op2 ) \
{\
	iFETCH;\
	iFETCH;\
	if (! call_super_method_op( op1, op2, lprivate,\
								self, at_class, compiled, stack,\
								code, codepc ))\
		goto on_error;\
}

EcInt inlined_call_op(EcInt op1,
					  EcPrivate* lprivate,
					  EC_OBJ self, EC_OBJ at_class, EC_OBJ compiled, EC_OBJ stack,
					  EcBytecode *code,
					  EcBytecode *codepc)
{
	register EC_OBJ ret, obj, frame;
	EcInt  nargs;
	EcInt  i;

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
		return 0;
	}

	if ((! EC_COMPILEDVARG(obj)) && (op1 > nargs))
	{
		/* There are more arguments than necessary */
		EcParameterCountError( EC_NIL, obj, /* expected */ nargs, stack );
		/*EcMakeException( EcExceptionClass,
		  "too many arguments to function (got %ld, expected %ld)",
		  op1, nargs );*/
		return 0;
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
	EC_STACKPUSH( frame, NULL /* EC_STACKGET( stack, 0 ) */ );		/* (master) caller bytecode */
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

/*	DMP("InlinedCallOP", frame, obj);*/

	if (EC_COMPILEDCCALLABLE(obj)) /* Check for a C-callable #JP */
		ret = EC_COMPILEDCCALLABLE(obj)( self, at_class, obj, frame );
	else
		ret = EcExecute( self, at_class, obj, frame );

	/* TODO: explicit return from inlined calls needs special treatment...
	 * We should check if the function has returned explicitly and, if
	 * so, exit.
	 */

	if (EC_ERRORP(ret)) return 0;
#if EC_STACK_RECYCLE
	/* RECYCLE_STACK_PUT(frame); */
	EC_STACKREF_DEC(frame);							/* this should force a recycle if needed */
#endif
	EC_STACKPUSH( stack, ret );
	return 1;
}

#define C_INLINED_CALL_OP( op1 ) \
{\
	iFETCH;\
	if (! inlined_call_op( op1, lprivate,\
						   self, at_class, compiled, stack,\
						   code, codepc ))\
		goto on_error;\
}

#define C_RETURN_OP \
{\
	register EC_OBJ ret;\
\
	ret = EC_STACKPOP( stack );\
	ASSERT( ! EC_ERRORP(ret) );\
	if (EC_ERRORP(ret)) goto on_error;\
/*	LPRIVATE(rt.vm_level)--; */ /* We are not in the VM... #JP */\
	return ret;\
}

#define C_INLINED_RETURN_OP \
{\
	register EC_OBJ ret;\
\
	ret = EC_STACKPOP( stack );\
	ASSERT( ! EC_ERRORP(ret) );\
	if (EC_ERRORP(ret)) goto on_error;\
/*	LPRIVATE(rt.vm_level)--; */ /* We are not in the VM... #JP */\
	return ret;\
}

#define C_THROW_OP \
{\
	register EC_OBJ obj;\
\
	CHECKGC;\
	obj = EC_STACKPOP( stack );\
	LPRIVATE(rt).exc = obj;\
	goto on_error;\
}

#define C_SEQUENCE_GET_OP \
{\
	register EC_OBJ ret, obj, obj2;\
\
	CHECKGC;\
	obj2 = EC_STACKPOP(stack);\
	obj  = EC_STACKPOP(stack);\
	ret = EcSequenceGetItem( obj, obj2 );\
	if (EC_ERRORP(ret)) goto on_error;\
	EC_STACKPUSH( stack, ret );\
}

#define C_SEQUENCE_SET_OP \
{\
	register EC_OBJ ret, obj, obj1, obj2;\
\
	CHECKGC;\
	obj2 = EC_STACKPOP(stack);\
	obj1 = EC_STACKPOP(stack);\
	obj  = EC_STACKPOP(stack);\
	ret = EcSequenceSetItem( obj, obj1, obj2 );\
	if (EC_ERRORP(ret)) goto on_error;\
	EC_STACKPUSH( stack, ret );\
}

#define C_ADD_OP \
{\
	register EC_OBJ ret, obj1, obj2;\
\
	CHECKGC;\
	obj2 = EC_STACKPOP( stack );\
	obj1 = EC_STACKPOP( stack );\
	if (EC_INUMP(obj1) && EC_INUMP(obj2))\
		ret = EcMakeInt( EC_INUM(obj1) + EC_INUM(obj2) );\
	else {\
		ret = ec_binop_add( obj1, obj2 );\
		if (EC_ERRORP(ret)) goto on_error;\
	}\
	EC_STACKPUSH( stack, ret );\
}

#define C_SUB_OP \
{\
	register EC_OBJ ret, obj1, obj2;\
\
	CHECKGC;\
	obj2 = EC_STACKPOP( stack );\
	obj1 = EC_STACKPOP( stack );\
	ret = ec_binop_sub( obj1, obj2 );\
	if (EC_ERRORP(ret)) goto on_error;\
	EC_STACKPUSH( stack, ret );\
}

#define C_MUL_OP \
{\
	register EC_OBJ ret, obj1, obj2;\
\
	CHECKGC;\
	obj2 = EC_STACKPOP( stack );\
	obj1 = EC_STACKPOP( stack );\
	ret = ec_binop_mul( obj1, obj2 );\
	if (EC_ERRORP(ret)) goto on_error;\
	EC_STACKPUSH( stack, ret );\
}

#define C_DIV_OP \
{\
	register EC_OBJ ret, obj1, obj2;\
\
	CHECKGC;\
	obj2 = EC_STACKPOP( stack );\
	obj1 = EC_STACKPOP( stack );\
	ret = ec_binop_div( obj1, obj2 );\
	if (EC_ERRORP(ret)) goto on_error;\
	EC_STACKPUSH( stack, ret );\
}

#define C_MOD_OP \
{\
	register EC_OBJ ret, obj1, obj2;\
\
	CHECKGC;\
	obj2 = EC_STACKPOP( stack );\
	obj1 = EC_STACKPOP( stack );\
	ret = ec_binop_mod( obj1, obj2 );\
	if (EC_ERRORP(ret)) goto on_error;\
	EC_STACKPUSH( stack, ret );\
}

#define C_POW_OP \
{\
	register EC_OBJ ret, obj1, obj2;\
\
	CHECKGC;\
	obj2 = EC_STACKPOP( stack );\
	obj1 = EC_STACKPOP( stack );\
	ret = ec_binop_pow( obj1, obj2 );\
	if (EC_ERRORP(ret)) goto on_error;\
	EC_STACKPUSH( stack, ret );\
}

#define C_LSHIFT_OP \
{\
	register EC_OBJ ret, obj1, obj2;\
\
	CHECKGC;\
	obj2 = EC_STACKPOP( stack );\
	obj1 = EC_STACKPOP( stack );\
	ret = ec_binop_lshift( obj1, obj2 );\
	if (EC_ERRORP(ret)) goto on_error;\
	EC_STACKPUSH( stack, ret );\
}

#define C_RSHIFT_OP \
{\
	register EC_OBJ ret, obj1, obj2;\
\
	CHECKGC;\
	obj2 = EC_STACKPOP( stack );\
	obj1 = EC_STACKPOP( stack );\
	ret = ec_binop_rshift( obj1, obj2 );\
	if (EC_ERRORP(ret)) goto on_error;\
	EC_STACKPUSH( stack, ret );\
}

#define C_AND_OP \
{\
	register EC_OBJ ret, obj1, obj2;\
\
	CHECKGC;\
	obj2 = EC_STACKPOP( stack );\
	obj1 = EC_STACKPOP( stack );\
	ret = ec_binop_and( obj1, obj2 );\
	if (EC_ERRORP(ret)) goto on_error;\
	EC_STACKPUSH( stack, ret );\
}

#define C_OR_OP \
{\
	register EC_OBJ ret, obj1, obj2;\
\
	CHECKGC;\
	obj2 = EC_STACKPOP( stack );\
	obj1 = EC_STACKPOP( stack );\
	ret = ec_binop_or( obj1, obj2 );\
	if (EC_ERRORP(ret)) goto on_error;\
	EC_STACKPUSH( stack, ret );\
}

#define C_XOR_OP \
{\
	register EC_OBJ ret, obj1, obj2;\
\
	CHECKGC;\
	obj2 = EC_STACKPOP( stack );\
	obj1 = EC_STACKPOP( stack );\
	ret = ec_binop_xor( obj1, obj2 );\
	if (EC_ERRORP(ret)) goto on_error;\
	EC_STACKPUSH( stack, ret );\
}

#define C_NEG_OP \
{\
	register EC_OBJ ret;\
\
	CHECKGC;\
	ret = ec_unaryop_neg( EC_STACKPOP( stack ) );\
	if (EC_ERRORP(ret)) goto on_error;\
	EC_STACKPUSH( stack, ret );\
}

#define C_POS_OP \
{\
	register EC_OBJ ret;\
\
	CHECKGC;\
	ret = ec_unaryop_pos( EC_STACKPOP( stack ) );\
	if (EC_ERRORP(ret)) goto on_error;\
	EC_STACKPUSH( stack, ret );\
}

#define C_INVERT_OP \
{\
	register EC_OBJ ret;\
\
	CHECKGC;\
	ret = ec_unaryop_invert( EC_STACKPOP( stack ) );\
	if (EC_ERRORP(ret)) goto on_error;\
	EC_STACKPUSH( stack, ret );\
}

#define C_IN_OP \
{\
	register EC_OBJ ret, obj1, obj2;\
\
	CHECKGC;\
	obj2 = EC_STACKPOP( stack );\
	obj1 = EC_STACKPOP( stack );\
	ret = EcSequenceIn( obj2, obj1 );\
	if (EC_ERRORP(ret)) goto on_error;\
	EC_STACKPUSH( stack, ret );\
}

#define C_LT_OP \
{\
	register EC_OBJ ret, obj1, obj2;\
\
	CHECKGC;\
	obj2 = EC_STACKPOP( stack );\
	obj1 = EC_STACKPOP( stack );\
	if (EC_INUMP(obj1) && EC_INUMP(obj2))\
		ret = EC_INUM_LT(obj1, obj2) ? EcTrueObject : EcFalseObject;\
	else {\
		ret = EcLibLt( obj1, obj2 );\
		if (EC_ERRORP(ret)) goto on_error;\
	}\
	EC_STACKPUSH( stack, ret );\
}

#define C_GT_OP \
{\
	register EC_OBJ ret, obj1, obj2;\
\
	CHECKGC;\
	obj2 = EC_STACKPOP( stack );\
	obj1 = EC_STACKPOP( stack );\
	if (EC_INUMP(obj1) && EC_INUMP(obj2))\
		ret = EC_INUM_GT(obj1, obj2) ? EcTrueObject : EcFalseObject;\
	else {\
		ret = EcLibGt( obj1, obj2 );\
		if (EC_ERRORP(ret)) goto on_error;\
	}\
	EC_STACKPUSH( stack, ret );\
}

#define C_LE_OP \
{\
	register EC_OBJ ret, obj1, obj2;\
\
	CHECKGC;\
	obj2 = EC_STACKPOP( stack );\
	obj1 = EC_STACKPOP( stack );\
	if (EC_INUMP(obj1) && EC_INUMP(obj2))\
		ret = EC_INUM_LE(obj1, obj2) ? EcTrueObject : EcFalseObject;\
	else {\
		ret = EcLibLe( obj1, obj2 );\
		if (EC_ERRORP(ret)) goto on_error;\
	}\
	EC_STACKPUSH( stack, ret );\
}

#define C_GE_OP \
{\
	register EC_OBJ ret, obj1, obj2;\
\
	CHECKGC;\
	obj2 = EC_STACKPOP( stack );\
	obj1 = EC_STACKPOP( stack );\
	if (EC_INUMP(obj1) && EC_INUMP(obj2))\
		ret = EC_INUM_GE(obj1, obj2) ? EcTrueObject : EcFalseObject;\
	else {\
		ret = EcLibGe( obj1, obj2 );\
		if (EC_ERRORP(ret)) goto on_error;\
	}\
	EC_STACKPUSH( stack, ret );\
}

#define C_EQ_OP \
{\
	register EC_OBJ ret, obj1, obj2;\
\
	CHECKGC;\
	obj2 = EC_STACKPOP( stack );\
	obj1 = EC_STACKPOP( stack );\
	if (EC_INUMP(obj1) && EC_INUMP(obj2))\
		ret = EC_INUM_EQ(obj1, obj2) ? EcTrueObject : EcFalseObject;\
	else {\
		ret = EcLibEq( obj1, obj2 );\
		if (EC_ERRORP(ret)) goto on_error;\
	}\
	EC_STACKPUSH( stack, ret );\
}

#define C_NE_OP \
{\
	register EC_OBJ ret, obj1, obj2;\
\
	CHECKGC;\
	obj2 = EC_STACKPOP( stack );\
	obj1 = EC_STACKPOP( stack );\
	if (EC_INUMP(obj1) && EC_INUMP(obj2))\
		ret = EC_INUM_EQ(obj1, obj2) ? EcFalseObject : EcTrueObject;\
	else {\
		ret = EcLibNe( obj1, obj2 );\
		if (EC_ERRORP(ret)) goto on_error;\
	}\
	EC_STACKPUSH( stack, ret );\
}

#define C_LNOT_OP \
{\
	register EC_OBJ ret, obj, obj1;\
\
	CHECKGC;\
	obj = EC_STACKPOP( stack );\
	if (EC_BOOLP(obj))\
	{\
		ret = EC_BOOLVAL(obj) ? EcFalseObject : EcTrueObject;\
	} else if (EC_INUMP(obj))\
	{\
		ret = EC_INUM(obj) ? EcFalseObject : EcTrueObject;\
	} else\
	{\
		obj1 = ec_unaryop_true( obj );\
		if (EC_ERRORP(obj1)) goto on_error;\
		ret = (obj1 == EcTrueObject) ? EcFalseObject : EcTrueObject;\
	}\
	/* if (EC_ERRORP(ret)) goto on_error; */\
	EC_STACKPUSH( stack, ret );\
}

#if EC_STACK_RECYCLE

#define C_SAVE_LEXICAL_OP \
{\
	register EC_OBJ obj;\
\
	CHECKGC;\
	obj = EC_STACKPOP( stack );\
	ASSERT( EC_COMPILEDP(obj) );\
	ASSERT( EC_STACKP(stack) );\
	EC_COMPILEDLEXICAL(obj) = stack;\
	EC_STACKREF_INC(stack);\
	EC_STACKPUSH( stack, obj );\
}

#else

#define C_SAVE_LEXICAL_OP \
{\
	register EC_OBJ obj;\
\
	CHECKGC;\
	obj = EC_STACKPOP( stack );\
	ASSERT( EC_COMPILEDP(obj) );\
	ASSERT( EC_STACKP(stack) );\
	EC_COMPILEDLEXICAL(obj) = stack;\
	EC_STACKPUSH( stack, obj );\
}

#endif

#define C_COPY_OP( op1 ) \
{\
	register EC_OBJ ret, obj;\
\
	iFETCH;\
	CHECKGC;\
	obj = EC_STACKPOP( stack );\
	ret = EcCopy( obj, (EcCopyType) op1 );\
	if (EC_ERRORP(ret)) goto on_error;\
	EC_STACKPUSH( stack, ret );\
}

#define C_ARRAY_CONS_OP( op1 ) \
{\
	register EC_OBJ ret;\
\
	iFETCH;\
	CHECKGC;\
	EC_STACKNARGS_REAL(stack)    = op1;\
	EC_STACKNARGS_LOGICAL(stack) = op1;\
	ret = EcLibArrayCons( stack, NULL );\
	if (EC_ERRORP(ret)) goto on_error;\
	EC_STACKPUSH( stack, ret );\
}

#define C_HASH_CONS_OP( op1 ) \
{\
	register EC_OBJ ret;\
\
	iFETCH;\
	CHECKGC;\
	EC_STACKNARGS_REAL(stack)    = op1;\
	EC_STACKNARGS_LOGICAL(stack) = op1;\
	ret = EcLibHashCons( stack, NULL );\
	if (EC_ERRORP(ret)) goto on_error;\
	EC_STACKPUSH( stack, ret );\
}

#define C_INC_OP \
{\
	register EC_OBJ ret, obj;\
\
	CHECKGC;\
	obj = EC_STACKPOP( stack );\
	if (EC_INUMP(obj))\
		ret = EcMakeInt( EC_INUM(obj) + 1 );\
	else {\
		ret = EcLibInc( obj );\
		if (EC_ERRORP(ret)) goto on_error;\
	}\
	EC_STACKPUSH( stack, ret );\
}

#define C_DEC_OP \
{\
	register EC_OBJ ret, obj;\
\
	CHECKGC;\
	obj = EC_STACKPOP( stack );\
	ret = EcLibDec( obj );\
	if (EC_ERRORP(ret)) goto on_error;\
	EC_STACKPUSH( stack, ret );\
}

#endif
