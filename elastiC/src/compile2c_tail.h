/* ==========================================================================
 * compile2c_tail
 * ==========================================================================
 * elastiC C compiler tail file.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Jacopo Pantaleoni         E-mail: jp@lightflowtech.com
 *
 *   Created: Sun May 25 08:05:50 MEST 2003
 *
 *   $Id: compile2c_tail.h,v 1.1 2003/05/29 10:36:00 panta Exp $
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

	if (EC_STACKPOINTER(stack) - EC_STACKBASE(stack) > 2)
	{
/*		LPRIVATE(rt.vm_level)--; */ /* We are not in the VM... #JP */
		return EC_STACKPOP(stack);
	}

/*	LPRIVATE(rt.vm_level)--;*/
	return EC_NIL;

/* WRONG! Needs rethinking... #JP */
on_error:
{
	EC_OBJ frame, comp, sf;
	EC_OBJ ret, obj;
	EcInt  i, nargs;
	EC_OBJ handlerchain;

	printf("\n*** ERROR ***\n");
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

/*			ec_fprintf( stderr, "# handlers: %ld\n", EC_ARRAYLEN(handlerchain) );*/
			for (i = 0; i < EC_ARRAYLEN(handlerchain); i++)
			{
				ASSERT(EC_ARRAYLEN(handlerchain) > i);
/*				handler = EC_ARRAYMEM(handlerchain)[i];*/
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
/* inlined return */EC_STACKIMMUP(frame)     = EC_STACKIMMUP(sf);		    /* same as corresponding try block */
					EC_STACKIMMCALLER(frame) = EC_STACKIMMCALLER(sf);	    /* same as corresponding try block */
					EC_STACKIMMPC(frame)     = EC_STACKIMMPC(sf);	        /* idem                            */
					EC_STACKCALLED(frame)    = obj;							/* called bytecode  */
/* normal return */	EC_STACKPC(frame) = EC_STACKPC(sf);			        	/* idem for the master caller PC   */
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

					ret = EcExecute( EC_NIL, EC_NIL, obj, frame );
					if (EC_ERRORP(ret)) goto on_error;
					EC_STACKPUSH( stack, ret );
					/* Does this suffice? Where should we go now? #JP */

/*					compiled = obj;
					stack    = frame;
					LPRIVATE(rt.compiled) = compiled;

					code   = EC_COMPILEDCODE(compiled);
					ncode  = EC_COMPILEDNCODE(compiled);
					lframe = EC_COMPILEDLFRAME(compiled);
					codepc  = code;
					codeend = code + ncode;

					BACKTRACE( stack, compiled );
#if TRACE_EXECUTION
					printf("\n");
					EcDumpCompiled( compiled, 0 );
					printf("\n");
					printf("LEXICALLY UPPER FRAME: 0x%08lX\n", (unsigned long)EC_STACKLEXICAL(stack));
#endif
					LPRIVATE(rt.exc_line_num) = -1;
					goto restart;*/
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
		/* Should continue from somewhere in the code... but where? #JP */
		return EC_NIL;
	} else
	{
/*		LPRIVATE(rt.vm_level)--;*/ /* We are not in the VM... #JP */
		return Ec_ERROR;
	}
}
