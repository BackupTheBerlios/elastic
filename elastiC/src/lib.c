/* ==========================================================================
 * lib.c
 * ==========================================================================
 * Library functions.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sun Jul 26 17:04:46 MEST 1998
 *
 *   $Id: lib.c,v 1.4 2002/06/01 16:17:22 panta Exp $
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

#include "elastic.h"
#include "private.h"
#include "compat.h"
#include "debug.h"

#include <math.h>
#include <time.h>


static EcUInt shallow_type = 0, deep_type = 0;

/* ========================================================================
 * basic.* primitives
 * ======================================================================== */

EC_API EC_OBJ EcLibStr( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ res;
	EcInt len;
	ec_string ds;

	res = EcParseStackFunction( "basic.str", TRUE, stack, "O", &obj );
	if (EC_ERRORP(res))
		return res;

	ec_string_init( &ds, NULL );
	len = EcPrintObject( &ds, obj, FALSE );
	res = EcMakeString( ec_strdata( &ds ), ec_strlen( &ds ) );
	ec_string_cleanup( &ds );

	return res;
}

EC_API EC_OBJ EcLibRepr( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ res;
	EcInt len;
	ec_string ds;

	res = EcParseStackFunction( "basic.repr", TRUE, stack, "O", &obj );
	if (EC_ERRORP(res))
		return res;

	ec_string_init( &ds, NULL );
	len = EcPrintObject( &ds, obj, TRUE );
	res = EcMakeString( ec_strdata( &ds ), ec_strlen( &ds ) );
	ec_string_cleanup( &ds );

	return res;
}

EC_OBJ ec_unaryop_int( EC_OBJ v );
EC_OBJ ec_unaryop_float( EC_OBJ v );

EC_API EC_OBJ EcLibInt( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ res;

	res = EcParseStackFunction( "basic.int", TRUE, stack, "O", &obj );
	if (EC_ERRORP(res))
		return res;

	return ec_unaryop_int( obj );
}

EC_API EC_OBJ EcLibFloat( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ res;

	res = EcParseStackFunction( "basic.int", TRUE, stack, "O", &obj );
	if (EC_ERRORP(res))
		return res;

	return ec_unaryop_float( obj );
}

EC_API EC_OBJ EcLibPrint( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ *obj;
	EC_OBJ res;
	EcInt  num = 0;
	EcInt  i;

	res = EcParseStackFunction( "basic.print", TRUE, stack, "|o", &num, &obj );
	if (EC_ERRORP(res))
		return res;

	for (i = 0; i < num; i++)
	{
		ec_fprintf( stdout, "%w", obj[i] );
	}

	return EC_NIL;
}

EC_API EC_OBJ EcLibSPrintf( EC_OBJ stack, EcAny userdata )
{
	char   *fmt;
	EcInt   fmtlen;
	EC_OBJ *obj;
	EC_OBJ  res;
	EcInt   num = 0;

	res = EcParseStackFunction( "basic.sprintf", TRUE, stack, "s#|o", &fmt, &fmtlen, &num, &obj );
	if (EC_ERRORP(res))
		return res;

	res = ec_printf_obj( fmt, obj, num, 1, EC_NIL );

	return res;
}

EC_API EC_OBJ EcLibPrintf( EC_OBJ stack, EcAny userdata )
{
	char   *fmt;
	EcInt   fmtlen;
	EC_OBJ *obj;
	EC_OBJ  res;
	EcInt   num = 0;

	res = EcParseStackFunction( "basic.printf", TRUE, stack, "s#|o", &fmt, &fmtlen, &num, &obj );
	if (EC_ERRORP(res))
		return res;

	res = ec_printf_obj( fmt, obj, num, 1, EC_NIL );
	ec_fprintf( stdout, "%w", res );

	return res;
}

EC_API EC_OBJ EcLibCopy( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EcUInt copytype;
	EC_OBJ res;

	copytype = shallow_type;
	res = EcParseStackFunction( "basic.copy", TRUE, stack, "O|k", &obj, &copytype );
	if (EC_ERRORP(res))
		return res;

	res = EcCopy( obj, copytype == shallow_type ? EcShallowCopyType : EcDeepCopyType );

	return res;
}

EC_API EC_OBJ EcLibShallowCopy( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ res;

	res = EcParseStackFunction( "basic.shallowcopy", TRUE, stack, "O", &obj );
	if (EC_ERRORP(res))
		return res;

	res = EcCopy( obj, EcShallowCopyType );

	return res;
}

EC_API EC_OBJ EcLibDeepCopy( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ res;

	res = EcParseStackFunction( "basic.deepcopy", TRUE, stack, "O", &obj );
	if (EC_ERRORP(res))
		return res;

	res = EcCopy( obj, EcDeepCopyType );

	return res;
}

EC_API EC_OBJ EcLibExit( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;

	obj = EC_STACKPOP(stack);
	if (EC_NULLP(obj))
		exit( EXIT_FAILURE );
	switch (EC_TYPE(obj))
	{
	case tc_inum:
	case tc_fnum:
		exit( EC_TOINUM(obj) );
		break;

	default:
		exit( EC_ASBOOLVAL(obj) ? EXIT_SUCCESS : EXIT_FAILURE );
		break;
	}

	exit( EXIT_FAILURE );
	return EC_NIL;
}

EC_API EC_OBJ EcLibArray( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ dim;
	EC_OBJ res;

	EC_CHECKNARGS_F( "basic.array", 1 );

	dim = EC_STACKPOP(stack);
	EC_CHECKTYPE_F( "basic.array", 1, dim, tc_inum );

	res = EcMakeArray( EC_INUM(dim) );
	if (EC_NULLP(res) || EC_ERRORP(res))
		return Ec_ERROR;

	if (! EcResizeArray( res, EC_INUM(dim) ))
		return EcMemoryError();

	return res;
}

EC_API EC_OBJ EcLibTypeOf( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ res;

	EC_CHECKNARGS_F( "basic.typeof", 1 );

	obj = EC_STACKPOP(stack);

	res = EcMakeSymbol( EcObjectTypeName( obj ) );
	return res;
}

EC_API EC_OBJ EcLibClassCode( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;

	EC_CHECKNARGS_F( "basic.classCode", 1 );

	obj = EC_STACKPOP(stack);
	if (! EC_CLASSP(obj))
	{
		*((char *)0) = 0;
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	return EC_CLASSCODE(obj);
}

EC_API EC_OBJ EcLibSend( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ  receiver;
	EcUInt  methodid;
	EC_OBJ *args;
	EcInt   argslen;
	EC_OBJ  res;

	res = EcParseStackFunction( "basic.send", TRUE, stack, "Ok|o", &receiver, &methodid, &argslen, &args );
	if (EC_ERRORP(res)) return res;

	return EcSendMessageArgs( receiver, methodid, argslen, args );
}

EC_API EC_OBJ EcLibSendWith( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ  receiver;
	EcUInt  methodid;
	EC_OBJ  argsarray;
	EC_OBJ *args;
	EcInt   argslen;
	EC_OBJ  res;

	res = EcParseStackFunction( "basic.sendwith", TRUE, stack, "OkO!", &receiver, &methodid, tc_array, &argsarray );
	if (EC_ERRORP(res)) return res;

	argslen = EC_ARRAYLEN(argsarray);
	args    = alloca( argslen * sizeof(EC_OBJ) );
	if (! args) return EcMemoryError();
	memcpy( args, EC_ARRAYMEM(argsarray), argslen * sizeof(EC_OBJ) );

	return EcSendMessageArgs( receiver, methodid, argslen, args );
}

#if 0
EC_API EC_OBJ EcLibCall( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ *args_base;
	EC_OBJ  callable;
	EC_OBJ *args;
	EcInt   nargs;

	EC_CHECKNARGS_MIN_F( "basic.call", 1 );

	nargs     = EC_STACKNARGS_REAL(stack);
	ASSERT( nargs <= EC_STACKDIM(stack) );
	args_base = EC_STACKPOINTER(stack) - nargs;
	ASSERT( args_base >= EC_STACKBASE(stack) );

	callable = *args_base++; nargs--;
	args     = args_base;

	EC_STACKPOINTER(stack) -= EC_STACKNARGS_REAL(stack);		/* consume arguments */

	if ((! EC_PRIMITIVEP(callable)) &&
		(! EC_COMPILEDP(callable)))
		return EC_TYPEERROR_F( "basic.call", 1, tc_none, callable, "expected a callable object (bytecode or primitive)" );

	return EcCallArgs( stack, EC_NIL, EC_NIL, callable, nargs, args );
}
#endif

#if EC_WITHOUT_COMPILER
EC_API EC_OBJ EcLibCompile( EC_OBJ stack, EcAny userdata )
{
	return EcCompileError( -1, -1, "compiler not available" );
}
#else /* not EC_WITHOUT_COMPILER */
EC_API EC_OBJ EcLibCompile( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ current_bytecode;
	EC_OBJ current_package = EC_NIL;

	ec_compiler_ctxt    ctxt;
	ec_compiler_options comp_opts;

	EC_OBJ source;
	EC_OBJ compiled;

	EC_CHECKNARGS_F( "basic.compile", 1 );

	source = EC_STACKPOP(stack);
	EC_CHECKTYPE_F( "basic.compile", 1, source, tc_string );

	current_bytecode = PRIVATE(rt.compiled);
	if (! EC_NULLP(current_bytecode))
		current_package = EC_COMPILEDPACKAGE(current_bytecode);

	comp_opts.whole_package = EC_PACKAGEP(current_package) ? FALSE : TRUE; /* it's a whole package         */
	comp_opts.in_package    = current_package;
	comp_opts.save          = FALSE;							/* don't save compiled packages */
	comp_opts.outputfile    = NULL;								/* output filename              */
	ctxt = EcCompilerContextCreate();
	compiled = EcCompileString( ctxt,
								EC_STRDATA(source),
								/* execute: */ FALSE, /* executeImported: */ TRUE,
								&comp_opts );
	EcCompilerContextDestroy( ctxt );

	return compiled;
}
#endif /* not EC_WITHOUT_COMPILER */

EC_API EC_OBJ EcLibApply( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ  callable, arguments;
	EC_OBJ  nargs_o, el;
	EC_OBJ *argument_v;
	EcInt   nargs, i;

	EC_CHECKNARGS_F( "basic.apply", 2 );

	arguments = EC_STACKPOP(stack);
	callable  = EC_STACKPOP(stack);

	if ((! EC_PRIMITIVEP(callable)) &&
		(! EC_COMPILEDP(callable)))
		return EC_TYPEERROR_F( "basic.apply", 1, tc_none, callable, "expected a callable object (bytecode or primitive)" );	/* :TODO: should become an EC_TYPCHECK_*_F macro */
	if (! EcIsSequence( arguments))
		return EC_TYPEERROR_F( "basic.apply", 2, tc_none, arguments, "expected a sequence object" );	/* :TODO: should become an EC_TYPCHECK_*_F macro */

	nargs_o = EcSequenceLength( arguments );
	if (EC_ERRORP(nargs_o)) return nargs_o;
	ASSERT( EC_INUMP(nargs_o) );
	nargs = EC_INUM(nargs_o);
	argument_v = alloca( nargs );
	if (! argument_v)
		return EcMemoryError();

	for (i = 0; i < nargs; i++)
	{
		el = EcSequenceGetElement( arguments, i );
		if (EC_ERRORP(el)) return el;
		argument_v[i] = el;
	}

	return EcCallArgs( /* caller's stack */ stack, EC_NIL, EC_NIL, callable, nargs, argument_v );
}

EC_API EC_OBJ EcLibArity( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ callable;
	EcInt  nargs_req;
	EcBool variadic;

	EcBool ismethod;

	EC_CHECKNARGS_F( "basic.arity", 1 );

	callable  = EC_STACKPOP(stack);

	if (EC_NULLP(callable))
		return EC_TYPEERROR_F( "basic.arity", 1, tc_none, callable, "expected a callable object (bytecode or primitive)" );	/* :TODO: should become an EC_TYPCHECK_*_F macro */

	switch (EC_TYPE(callable))
	{
	case tc_primitive:
		nargs_req = -1;											/* argument count unknown: throw an exception */
		variadic  = FALSE;
		return Ec_ERROR;										/* :TODO: throw the correct exception */
		break;

	case tc_cmethod:
		nargs_req = -1;											/* argument count unknown: throw an exception */
		variadic  = FALSE;
		return Ec_ERROR;										/* :TODO: throw the correct exception */
		break;

	default:
		if (EC_COMPILEDP(callable))
		{
			/* EcInt n_req; */

			ismethod = EC_COMPILEDISMETHOD(callable);
			nargs_req = EC_COMPILEDNARG(callable) - (ismethod ? 2 : 0);
			variadic  = EC_COMPILEDVARG(callable);
			break;
		}

		return EC_TYPEERROR_F( "basic.arity", 1, tc_none, callable, "expected a callable object (bytecode or primitive)" );	/* :TODO: should become an EC_TYPCHECK_*_F macro */
	}

	return EcMakeArrayInit( 2, EcMakeInt( nargs_req ), EcMakeBool( variadic ) );
}

EC_API EC_OBJ EcLibLength( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;

	EC_CHECKNARGS_F( "basic.length", 1 );

	obj = EC_STACKPOP(stack);

	return EcSequenceLength( obj );
}

EC_API EC_OBJ EcLibHash( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;

	EC_CHECKNARGS_F( "basic.hash", 1 );

	obj = EC_STACKPOP(stack);

	return EcMakeInt( EcHash( obj ) );
}

EC_API EC_OBJ EcLibTime( EC_OBJ stack, EcAny userdata )
{
	EcFloat res;

	EC_CHECKNARGS_F( "basic.time", 0 );

	res = ec_get_utime();
	if (res < 0.0)
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	return EcMakeFloat( res );
}

EC_API EC_OBJ EcLibTimes( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ   obj;
	ec_times tb;

	EC_CHECKNARGS_F( "basic.times", 0 );

	ec_get_times( &tb );
	obj = EcMakeArray( 3 );
	if (EC_ERRORP(obj))
		return obj;

	EcArraySet( obj, 0, EcMakeFloat( (EcFloat)tb.up_s     * 1000000.0 + (EcFloat)tb.up_u ) );
	EcArraySet( obj, 1, EcMakeFloat( (EcFloat)tb.user_s   * 1000000.0 + (EcFloat)tb.user_u ) );
	EcArraySet( obj, 2, EcMakeFloat( (EcFloat)tb.system_s * 1000000.0 + (EcFloat)tb.system_u ) );

	return obj;
}


/* ========================================================================
 * sys.* primitives
 * ======================================================================== */

EC_API EC_OBJ EcLibGetEnv( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ  res;
	char   *str;
	EcInt   length;
	char   *val;

	res = EcParseStackFunction( "sys.getenv", TRUE, stack, "s#", &str, &length );
	if (EC_ERRORP(res))
		return res;

	val = ec_getenv( str );

	if (! val) return EC_NIL;
	return EcMakeString( val, 0 );
}

EC_API EC_OBJ EcLibSetEnv( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ  res;
	char   *key, *val;
	EcInt   keylen, vallen;

	res = EcParseStackFunction( "sys.setenv", TRUE, stack, "s#s#", &key, &keylen, &val, &vallen );
	if (EC_ERRORP(res))
		return res;

	ec_setenv( key, val, TRUE );

	return EC_NIL;
}

/* ========================================================================
 * math.* primitives
 * ======================================================================== */

EC_API EC_OBJ EcLibMath_Sin( EC_OBJ stack, EcAny userdata )
{
#if 0
	EC_OBJ  res;
	EcFloat num;

	res = EcParseStackFunction( "math.sin", TRUE, stack, "f", &num );
	if (EC_ERRORP(res))
		return res;

	return EcMakeFloat( sin( num ) );
#endif
	EC_OBJ num;

	EC_CHECKNARGS_F("math.sin", 1);
	num = EC_STACKPOP(stack);
	EC_CHECKTYPE_F( "math.sin", 1, num, tc_fnum );
	return EcMakeFloat( sin( EC_FNUM(num) ) );
}

EC_API EC_OBJ EcLibMath_Cos( EC_OBJ stack, EcAny userdata )
{
#if 0
	EC_OBJ  res;
	EcFloat num;

	res = EcParseStackFunction( "math.cos", TRUE, stack, "f", &num );
	if (EC_ERRORP(res))
		return res;

	return EcMakeFloat( cos( num ) );
#endif
	EC_OBJ num;

	EC_CHECKNARGS_F("math.cos", 1);
	num = EC_STACKPOP(stack);
	EC_CHECKTYPE_F( "math.cos", 1, num, tc_fnum );
	return EcMakeFloat( cos( EC_FNUM(num) ) );
}


/* ========================================================================
 * Builtin classes methods
 * ======================================================================== */

EC_API EC_OBJ EcLibObjectClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ obj;

	/*
	 * This is a class method, so self is the class.
	 */
	obj = EcMakeObject( self, NULL );

	if (EcDoesUnderstand( obj, PRIVATE(initID) ))
	{
		EC_OBJ *args;
		EcInt nargs, i;
		EC_OBJ method;

		method = EcFindMethod( obj, NULL, PRIVATE(initID) );

		nargs = EC_STACKNARGS_REAL(stack);

		args = alloca( nargs * sizeof(EC_OBJ) );
		if (! args) return EcMemoryError();

		for (i = nargs-1; i >= 0; i--)
			args[i] = EC_STACKPOP(stack);

		EcCallArgs( stack, /* self */ obj, /* in_class */ self, method, nargs, args );
	}

	return obj;
}

EC_API EC_OBJ EcLibObjectClass_Name( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_CHECKNARGS_CM( self, "name", 0 );

	return EC_CLASSSHORTNAME(self);
}

EC_API EC_OBJ EcLibObjectClass_FullName( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_CHECKNARGS_CM( self, "fullname", 0 );

	return EC_CLASSNAME(self);
}

EC_API EC_OBJ EcLibObject_IsA( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_CHECKNARGS_M( self, "isA", 0 );

	return EC_OBJECTCLASS(self);
}

EC_API EC_OBJ EcLibObject_IsAP( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_CHECKNARGS_M( self, "isA", 1 );

	return EcIsOfClass( self, EC_STACKPOP( stack ) ) ? EcTrueObject : EcFalseObject;
}

EC_API EC_OBJ EcLibObject_DoesUnderstand( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;
	EcUInt methodid;

	res = EcParseStackMethod( self, "doesUnderstand:", TRUE, stack, "k", &methodid );
	if (EC_ERRORP(res))
		return res;

	return EcDoesUnderstand( self, methodid ) ? EcTrueObject : EcFalseObject;
}

EC_API EC_OBJ EcLibExceptionClass_NewWith( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ obj;
	EC_OBJ arg;
#if 0
	ec_string ds;
#endif

	EC_CHECKNARGS_CM( self, "new:", 1 );

	if ((PRIVATE(rt.line_num) > 0) &&							/* set only if we have a line number and we */
		(PRIVATE(rt.exc_line_num) < 0))							/* don't have the original exception source */
		PRIVATE(rt.exc_line_num) = PRIVATE(rt.line_num);

	/*
	 * This is a class method, so self is the class
	 */
	arg = EC_STACKPOP( stack );
	obj = EcLibObjectClass_New( self, in_class, stack );

	EcSetInstanceVariable( obj, EcExceptionClass,
						   "source",
						   (EC_NNULLP(PRIVATE(rt.compiled)) ?
							EC_PACKAGESOURCE(EC_COMPILEDPACKAGE(PRIVATE(rt.compiled))) : EC_NIL) );
	EcSetInstanceVariable( obj, EcExceptionClass,
						   "in", PRIVATE(rt.compiled) );
	EcSetInstanceVariable( obj, EcExceptionClass,
						   "line", EcMakeInt( PRIVATE(rt.exc_line_num) ) );

#if 0
	ec_string_init( &ds, NULL );
	if (EC_NNULLP(PRIVATE(rt.compiled)))
		ec_sprintf( &ds, "(%w: %ld  IN: \"%w\")\n\t%w",
					EC_PACKAGESOURCE(EC_COMPILEDPACKAGE(PRIVATE(rt.compiled))),
					(long) PRIVATE(rt.exc_line_num),
					EC_COMPILEDNAME(PRIVATE(rt.compiled)),
					arg );
	else
		ec_sprintf( &ds, "(--: %ld  IN: --)\n\t%w",
					(long) PRIVATE(rt.exc_line_num),
					arg );
	EcSendMessageVA( obj, EcInternSymbol( "setMessage" ),
					 1, EcMakeString( ec_strdata( &ds ), ec_strlen( &ds ) ) );
	ec_string_cleanup( &ds );
#endif
	EcSendMessageVA( obj, EcInternSymbol( "setMessage" ),
					 1, arg );
/*	EcSetInstanceVariable( obj, EcExceptionClass,
						   "msg",
						   arg );*/
	return obj;
}

EC_API EC_OBJ EcLibException_GetSource( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getSource", 0 );

	ASSERT( in_class == EcExceptionClass );
	res = EcGetInstanceVariable( self,
								 EcExceptionClass,
								 "source" );
	return res;
}

EC_API EC_OBJ EcLibException_GetIn( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getIn", 0 );

	ASSERT( in_class == EcExceptionClass );
	res = EcGetInstanceVariable( self,
								 EcExceptionClass,
								 "in" );
	return res;
}

EC_API EC_OBJ EcLibException_GetLine( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getLine", 0 );

	ASSERT( in_class == EcExceptionClass );
	res = EcGetInstanceVariable( self,
								 EcExceptionClass,
								 "line" );
	return res;
}

EC_API EC_OBJ EcLibException_SetMessage( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "setMessage", 1 );

	ASSERT( in_class == EcExceptionClass );
	res = EcSetInstanceVariable( self,
								 EcExceptionClass,
								 "msg", EC_STACKPOP( stack ) );
	return self;
}

EC_API EC_OBJ EcLibException_GetMessage( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getMessage", 0 );

	ASSERT( in_class == EcExceptionClass );
	res = EcGetInstanceVariable( self,
								 EcExceptionClass,
								 "msg" );
	return res;
}

EC_API EC_OBJ EcLibException_GetFullMessage( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ source_o, in_o, line_o, msg_o;

	EC_CHECKNARGS_M( self, "getFullMessage", 0 );

	ASSERT( in_class == EcExceptionClass );
	source_o = EcGetInstanceVariable( self,
									  EcExceptionClass,
									  "source" );
	in_o = EcGetInstanceVariable( self,
								  EcExceptionClass,
								  "in" );
	line_o = EcGetInstanceVariable( self,
									EcExceptionClass,
									"line" );
	msg_o = EcGetInstanceVariable( self,
								   EcExceptionClass,
								   "msg" );
	return ec_oprintf( "(%w: %w  IN: %w)\n\t%w",
					   source_o, line_o, in_o, msg_o );
}

EC_API EC_OBJ EcLibUnimplementedErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ obj;
	EC_OBJ msg;

	EC_CHECKNARGS_CM( self, "new", 1 );

	msg = EC_STACKPOP( stack );

	/*
	 * This is a class method, so self is the class
	 */
	EC_STACKNARGS_REAL(stack) = EC_STACKNARGS_LOGICAL(stack) = 1;
	EC_STACKPUSH( stack, msg );
	obj = EcLibExceptionClass_NewWith( self, in_class, stack );

	return obj;
}

EC_API EC_OBJ EcLibMathErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ obj;
	EC_OBJ msg;

	EC_CHECKNARGS_CM( self, "new", 1 );

	msg = EC_STACKPOP( stack );

	/*
	 * This is a class method, so self is the class
	 */
	EC_STACKNARGS_REAL(stack) = EC_STACKNARGS_LOGICAL(stack) = 1;
	EC_STACKPUSH( stack, msg );
	obj = EcLibExceptionClass_NewWith( self, in_class, stack );

	return obj;
}

EC_API EC_OBJ EcLibIOErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ obj;
	EC_OBJ msg, ioObject;

	EC_CHECKNARGS_CM( self, "new", 2 );

	ioObject = EC_STACKPOP( stack );
	msg      = EC_STACKPOP( stack );

	/*
	 * This is a class method, so self is the class
	 */
	EC_STACKNARGS_REAL(stack) = EC_STACKNARGS_LOGICAL(stack) = 1;
	EC_STACKPUSH( stack, msg );
	obj = EcLibExceptionClass_NewWith( self, in_class, stack );

	EcSetInstanceVariable( obj, EcIOErrorClass,
						   "ioObject",
						   ioObject );
	return obj;
}

EC_API EC_OBJ EcLibIOError_GetIOObject( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getIOObject", 0 );

	ASSERT( in_class == EcIOErrorClass );
	res = EcGetInstanceVariable( self,
								 EcIOErrorClass,
								 "ioObject" );
	return res;
}

EC_API EC_OBJ EcLibMemoryErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ obj;
	EC_OBJ msg;

	EC_CHECKNARGS_CM( self, "new", 1 );

	msg = EC_STACKPOP( stack );

	/*
	 * This is a class method, so self is the class
	 */
	EC_STACKNARGS_REAL(stack) = EC_STACKNARGS_LOGICAL(stack) = 1;
	EC_STACKPUSH( stack, msg );
	obj = EcLibExceptionClass_NewWith( self, in_class, stack );

	return obj;
}

EC_API EC_OBJ EcLibUnknownMethodErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ obj;
	EC_OBJ msg, methodName, target;

	EC_CHECKNARGS_CM( self, "new", 3 );

	target = EC_STACKPOP( stack );
	methodName = EC_STACKPOP( stack );
	msg = EC_STACKPOP( stack );

	/*
	 * This is a class method, so self is the class
	 */
	EC_STACKNARGS_REAL(stack) = EC_STACKNARGS_LOGICAL(stack) = 1;
	EC_STACKPUSH( stack, msg );
	obj = EcLibExceptionClass_NewWith( self, in_class, stack );

	EcSetInstanceVariable( obj, EcUnknownMethodErrorClass,
						   "target",
						   target );
	EcSetInstanceVariable( obj, EcUnknownMethodErrorClass,
						   "methodName",
						   methodName );
	return obj;
}

EC_API EC_OBJ EcLibUnknownMethodError_GetMethod( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getMethod", 0 );

	ASSERT( in_class == EcUnknownMethodErrorClass );
	res = EcGetInstanceVariable( self,
								 EcUnknownMethodErrorClass,
								 "methodName" );
	return res;
}

EC_API EC_OBJ EcLibUnknownMethodError_GetTarget( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getTarget", 0 );

	ASSERT( in_class == EcUnknownMethodErrorClass );
	res = EcGetInstanceVariable( self,
								 EcUnknownMethodErrorClass,
								 "target" );
	return res;
}

EC_API EC_OBJ EcLibUnknownOperationErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ obj;
	EC_OBJ msg, operands, operation;

	EC_CHECKNARGS_CM( self, "new", 3 );

	operation = EC_STACKPOP( stack );
	operands  = EC_STACKPOP( stack );
	msg       = EC_STACKPOP( stack );

	/*
	 * This is a class method, so self is the class
	 */
	EC_STACKNARGS_REAL(stack) = EC_STACKNARGS_LOGICAL(stack) = 1;
	EC_STACKPUSH( stack, msg );
	obj = EcLibExceptionClass_NewWith( self, in_class, stack );

	EcSetInstanceVariable( obj, EcUnknownOperationErrorClass,
						   "operation",
						   operation );
	EcSetInstanceVariable( obj, EcUnknownOperationErrorClass,
						   "operands",
						   operands );
	return obj;
}

EC_API EC_OBJ EcLibUnknownOperationError_GetOperands( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getOperands", 0 );

	ASSERT( in_class == EcUnknownOperationErrorClass );
	res = EcGetInstanceVariable( self,
								 EcUnknownOperationErrorClass,
								 "operands" );
	return res;
}

EC_API EC_OBJ EcLibUnknownOperationError_GetOperation( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getOperation", 0 );

	ASSERT( in_class == EcUnknownOperationErrorClass );
	res = EcGetInstanceVariable( self,
								 EcUnknownOperationErrorClass,
								 "operation" );
	return res;
}

EC_API EC_OBJ EcLibIndexErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ obj;
	EC_OBJ msg, index, target;

	EC_CHECKNARGS_CM( self, "new", 3 );

	target = EC_STACKPOP( stack );
	index  = EC_STACKPOP( stack );
	msg    = EC_STACKPOP( stack );

	/*
	 * This is a class method, so self is the class
	 */
	EC_STACKNARGS_REAL(stack) = EC_STACKNARGS_LOGICAL(stack) = 1;
	EC_STACKPUSH( stack, msg );
	obj = EcLibExceptionClass_NewWith( self, in_class, stack );

	EcSetInstanceVariable( obj, EcIndexErrorClass,
						   "target",
						   target );
	EcSetInstanceVariable( obj, EcIndexErrorClass,
						   "index",
						   index );
	return obj;
}

EC_API EC_OBJ EcLibIndexError_GetIndex( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getIndex", 0 );

	ASSERT( in_class == EcIndexErrorClass );
	res = EcGetInstanceVariable( self,
								 EcIndexErrorClass,
								 "index" );
	return res;
}

EC_API EC_OBJ EcLibIndexError_GetTarget( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getTarget", 0 );

	ASSERT( in_class == EcIndexErrorClass );
	res = EcGetInstanceVariable( self,
								 EcIndexErrorClass,
								 "target" );
	return res;
}

EC_API EC_OBJ EcLibTypeErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ obj;
	EC_OBJ msg, target, function, index, expected, offensive;

	EC_CHECKNARGS_CM( self, "new", 6 );

	offensive = EC_STACKPOP( stack );
	expected  = EC_STACKPOP( stack );
	index     = EC_STACKPOP( stack );
	function  = EC_STACKPOP( stack );
	target    = EC_STACKPOP( stack );
	msg       = EC_STACKPOP( stack );

	/*
	 * This is a class method, so self is the class
	 */
	EC_STACKNARGS_REAL(stack) = EC_STACKNARGS_LOGICAL(stack) = 1;
	EC_STACKPUSH( stack, msg );
	obj = EcLibExceptionClass_NewWith( self, in_class, stack );

	EcSetInstanceVariable( obj, EcTypeErrorClass,
						   "target",
						   target );
	EcSetInstanceVariable( obj, EcTypeErrorClass,
						   "function",
						   function );
	EcSetInstanceVariable( obj, EcTypeErrorClass,
						   "index",
						   index );
	EcSetInstanceVariable( obj, EcTypeErrorClass,
						   "expected",
						   index );
	EcSetInstanceVariable( obj, EcTypeErrorClass,
						   "offensive",
						   offensive );
	return obj;
}

EC_API EC_OBJ EcLibTypeError_GetTarget( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getTarget", 0 );

	ASSERT( in_class == EcTypeErrorClass );
	res = EcGetInstanceVariable( self,
								 EcTypeErrorClass,
								 "target" );
	return res;
}

EC_API EC_OBJ EcLibTypeError_GetFunction( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getFunction", 0 );

	ASSERT( in_class == EcTypeErrorClass );
	res = EcGetInstanceVariable( self,
								 EcTypeErrorClass,
								 "function" );
	return res;
}

EC_API EC_OBJ EcLibTypeError_GetIndex( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getIndex", 0 );

	ASSERT( in_class == EcTypeErrorClass );
	res = EcGetInstanceVariable( self,
								 EcTypeErrorClass,
								 "index" );
	return res;
}

EC_API EC_OBJ EcLibTypeError_GetExpected( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getExpected", 0 );

	ASSERT( in_class == EcTypeErrorClass );
	res = EcGetInstanceVariable( self,
								 EcTypeErrorClass,
								 "expected" );
	return res;
}

EC_API EC_OBJ EcLibTypeError_GetOffensive( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getOffensive", 0 );

	ASSERT( in_class == EcTypeErrorClass );
	res = EcGetInstanceVariable( self,
								 EcTypeErrorClass,
								 "offensive" );
	return res;
}

EC_API EC_OBJ EcLibTargetErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ obj;
	EC_OBJ msg, target;

	EC_CHECKNARGS_CM( self, "new", 2 );

	target = EC_STACKPOP( stack );
	msg    = EC_STACKPOP( stack );

	/*
	 * This is a class method, so self is the class
	 */
	EC_STACKNARGS_REAL(stack) = EC_STACKNARGS_LOGICAL(stack) = 1;
	EC_STACKPUSH( stack, msg );
	obj = EcLibExceptionClass_NewWith( self, in_class, stack );

	EcSetInstanceVariable( obj, EcTargetErrorClass,
						   "target",
						   target );
	return obj;
}

EC_API EC_OBJ EcLibTargetError_GetTarget( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getTarget", 0 );

	ASSERT( in_class == EcTargetErrorClass );
	res = EcGetInstanceVariable( self,
								 EcTargetErrorClass,
								 "target" );
	return res;
}

EC_API EC_OBJ EcLibParameterCountErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ obj;
	EC_OBJ msg, target, function, expected, arguments;

	EC_CHECKNARGS_CM( self, "new", 5 );

	arguments = EC_STACKPOP( stack );
	expected  = EC_STACKPOP( stack );
	function  = EC_STACKPOP( stack );
	target    = EC_STACKPOP( stack );
	msg       = EC_STACKPOP( stack );

	/*
	 * This is a class method, so self is the class
	 */
	EC_STACKNARGS_REAL(stack) = EC_STACKNARGS_LOGICAL(stack) = 1;
	EC_STACKPUSH( stack, msg );
	obj = EcLibExceptionClass_NewWith( self, in_class, stack );

	EcSetInstanceVariable( obj, EcParameterCountErrorClass,
						   "target",
						   target );
	EcSetInstanceVariable( obj, EcParameterCountErrorClass,
						   "function",
						   function );
	EcSetInstanceVariable( obj, EcParameterCountErrorClass,
						   "expected",
						   expected );
	EcSetInstanceVariable( obj, EcParameterCountErrorClass,
						   "arguments",
						   arguments );
	return obj;
}

EC_API EC_OBJ EcLibParameterCountError_GetTarget( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getTarget", 0 );

	ASSERT( in_class == EcParameterCountErrorClass );
	res = EcGetInstanceVariable( self,
								 EcParameterCountErrorClass,
								 "target" );
	return res;
}

EC_API EC_OBJ EcLibParameterCountError_GetFunction( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getFunction", 0 );

	ASSERT( in_class == EcParameterCountErrorClass );
	res = EcGetInstanceVariable( self,
								 EcParameterCountErrorClass,
								 "function" );
	return res;
}

EC_API EC_OBJ EcLibParameterCountError_GetExpected( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getExpected", 0 );

	ASSERT( in_class == EcParameterCountErrorClass );
	res = EcGetInstanceVariable( self,
								 EcParameterCountErrorClass,
								 "expected" );
	return res;
}

EC_OBJ EcLibParameterCountError_GetArguments( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getArguments", 0 );

	ASSERT( in_class == EcParameterCountErrorClass );
	res = EcGetInstanceVariable( self,
								 EcParameterCountErrorClass,
								 "arguments" );
	return res;
}

EC_API EC_OBJ EcLibCompileErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ obj;
	EC_OBJ msg, line, column;

	EC_CHECKNARGS_CM( self, "new", 3 );

	column = EC_STACKPOP( stack );
	line   = EC_STACKPOP( stack );
	msg    = EC_STACKPOP( stack );

	/*
	 * This is a class method, so self is the class
	 */
	EC_STACKNARGS_REAL(stack) = EC_STACKNARGS_LOGICAL(stack) = 1;
	EC_STACKPUSH( stack, msg );
	obj = EcLibExceptionClass_NewWith( self, in_class, stack );

	EcSetInstanceVariable( obj, EcCompileErrorClass,
						   "line",
						   line );
	EcSetInstanceVariable( obj, EcCompileErrorClass,
						   "column",
						   column );
	return obj;
}

EC_API EC_OBJ EcLibCompileError_GetLine( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getLine", 0 );

	ASSERT( in_class == EcCompileErrorClass );
	res = EcGetInstanceVariable( self,
								 EcCompileErrorClass,
								 "line" );
	return res;
}

EC_API EC_OBJ EcLibCompileError_GetColumn( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getColumn", 0 );

	ASSERT( in_class == EcCompileErrorClass );
	res = EcGetInstanceVariable( self,
								 EcCompileErrorClass,
								 "column" );
	return res;
}

EC_API EC_OBJ EcLibArrayCons( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ   res;
	EC_OBJ   obj;
	EcInt i;
	EcInt nargs;

	nargs = EC_STACKNARGS_REAL(stack);

	res = EcMakeArray( nargs );
	if (EC_NULLP(res) || EC_ERRORP(res))
		return Ec_ERROR;

	for (i = nargs - 1; i >= 0; i--)
	{
		obj = EC_STACKPOP(stack);
		EcArraySet( res, i, obj );
	}

	return res;
}

EC_API EC_OBJ EcLibHashCons( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ   res;
	EC_OBJ   key, value;
	EcInt i;

	res = EcMakeHash();
	if (EC_NULLP(res) || EC_ERRORP(res))
		return Ec_ERROR;

	for (i = EC_STACKNARGS_REAL(stack) - 1; i >= 0; i -= 2)
	{
		ASSERT( i >= 1 );

		value = EC_STACKPOP(stack);
		key   = EC_STACKPOP(stack);
		EcSequenceSetItem( res, key, value );
	}

	return res;
}

EcBool _ec_lib_init( void )
{
	/* basic.* primitives */

	EcAddPrimitive( "basic.str",         EcLibStr );
	EcAddPrimitive( "basic.repr",        EcLibRepr );
	EcAddPrimitive( "basic.int",         EcLibInt );
	EcAddPrimitive( "basic.float",       EcLibFloat );
	EcAddPrimitive( "basic.print",       EcLibPrint );
	EcAddPrimitive( "basic.sprintf",     EcLibSPrintf );
	EcAddPrimitive( "basic.printf",      EcLibPrintf );
	EcAddPrimitive( "basic.copy",        EcLibCopy );
	EcAddPrimitive( "basic.shallowcopy", EcLibShallowCopy );
	EcAddPrimitive( "basic.deepcopy",    EcLibDeepCopy );
	EcAddPrimitive( "basic.exit",        EcLibExit );
	EcAddPrimitive( "basic.array",       EcLibArray );
	EcAddPrimitive( "basic.typeof",      EcLibTypeOf );
	EcAddPrimitive( "basic.classCode",   EcLibClassCode );
	EcAddPrimitive( "basic.send",        EcLibSend );
	EcAddPrimitive( "basic.sendwith",    EcLibSendWith );
#if 0
	EcAddPrimitive( "basic.call",        EcLibCall );
#endif
	EcAddPrimitive( "basic.compile",     EcLibCompile );
	EcAddPrimitive( "basic.apply",       EcLibApply );
	EcAddPrimitive( "basic.arity",       EcLibArity );
	EcAddPrimitive( "basic.length",      EcLibLength );
	EcAddPrimitive( "basic.hash",        EcLibHash );
	EcAddPrimitive( "basic.time",        EcLibTime );
	EcAddPrimitive( "basic.times",       EcLibTimes );

	/* sys.* primitives */

	EcAddPrimitive( "sys.getenv",      EcLibGetEnv );
	EcAddPrimitive( "sys.setenv",      EcLibSetEnv );

	/* sys.* variables */

	EcPackageVariable( EC_NIL, "sys.args", EC_NIL, TRUE, FALSE );

	/* math.* primitives */

	EcAddPrimitive( "math.sin",       EcLibMath_Sin );
	EcAddPrimitive( "math.cos",       EcLibMath_Cos );


	/* misc setup */
	shallow_type = EcInternSymbol( "shallow" );
	deep_type    = EcInternSymbol( "deep" );

	return TRUE;
}

void _ec_lib_cleanup( void )
{
	shallow_type = 0;
	deep_type    = 0;
}
