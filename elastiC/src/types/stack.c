/* ==========================================================================
 * stack.c
 * ==========================================================================
 * Stack.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat May 23 11:02:52 MEST 1998
 *
 *   $Id: stack.c,v 1.1 2002/05/23 21:20:23 panta Exp $
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

#define EC_STACK_RECYCLE_OPS 1
#include "elastic.h"
#include "private.h"
#include "debug.h"

#include "hashfunc.h"
#include "bitstring.h"
#include "stackrecycle.h"

EC_API EcInt tc_stack = -1;

static EC_OBJ stack_copy( EC_OBJ obj, EcCopyType type );

static void   stack_mark( EC_OBJ obj );
static void   stack_free( EC_OBJ obj );
static EcInt  stack_print( ec_string *str, EC_OBJ obj, EcBool detailed );

static EC_OBJ stack_getitem( EC_OBJ obj, EC_OBJ index );
static EC_OBJ stack_setitem( EC_OBJ obj, EC_OBJ index, EC_OBJ value );
static EC_OBJ stack_length( EC_OBJ obj );

static EcUInt stack_hash( EC_OBJ obj, EcInt recur );
static EcBool stack_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res );

static EcBool stack_check( EC_OBJ obj );

/* C API */

EC_API EC_OBJ EcMakeStack( EC_OBJ callerstack, EcUInt dim )
{
	EC_OBJ obj;
	EC_OBJ *objp;
	EcInt i;

	/* fprintf(stderr, "EcMakeStack(0x%08lX, %ld)\n", (unsigned long)callerstack, (long)dim); */

	obj = EcMakeUser( tc_stack, NULL );
	if (EC_ERRORP(obj)) return obj;

	EC_STACK(obj) = ec_malloc( sizeof(EcStack) );
	if (! EC_STACK(obj)) return Ec_ERROR;

	EC_STACKNARGS_REAL(obj)    = 0;
	EC_STACKNARGS_LOGICAL(obj) = 0;

	EC_STACKPC(obj)        = 0;
	EC_STACKUP(obj)        = callerstack;
	EC_STACKLEXICAL(obj)   = EC_NIL;
	EC_STACKDIM(obj)       = dim;
	EC_STACKIMMUP(obj)     = EC_NIL;
	EC_STACKIMMCALLER(obj) = EC_NIL;
	EC_STACKIMMPC(obj)     = 0;
	EC_STACKCALLED(obj)    = EC_NIL;
#if EC_STACK_USES_STATIC
	if (dim <= EC_STACK_STATIC_SIZE)
		EC_STACKBASE(obj)  = EC_STACKSTATIC(obj);
	else {
		EC_STACKBASE(obj)  = ec_malloc( dim * sizeof(EC_OBJ) );
		/* memset( EC_STACKBASE(obj), 0x00, dim * sizeof(EC_OBJ) ); */
		objp = EC_STACKBASE(obj);
		for (i = 0; i < dim; i++)
			*objp++ = EcUndefinedObject;
	}
#else
	EC_STACKBASE(obj)      = ec_malloc( dim * sizeof(EC_OBJ) );
	/* memset( EC_STACKBASE(obj), 0x00, dim * sizeof(EC_OBJ) ); */
	objp = EC_STACKBASE(obj);
	for (i = 0; i < dim; i++)
		*objp++ = EcUndefinedObject;
#endif
	if (! EC_STACKBASE(obj))
		return Ec_ERROR;
	EC_STACKPOINTER(obj) = EC_STACKBASE(obj);
	EC_STACKBP(obj)      = EC_STACKPOINTER(obj);

#if EC_STACK_RECYCLE
	/* EC_STACKREF_SET(obj, 0); */
	EC_STACKREF_SET(obj, 1);

#if EC_STACK_RECYCLE_STATS
	PRIVATE(n_makestack)++;
#endif
#endif

	return obj;
}

EC_API EC_OBJ EcMakeStackForCompiled( EC_OBJ compiled, EcInt nargs )
{
	EC_OBJ stack;
	EcInt method_nargs, nargs_req, nargs_real, vargs;

	ASSERT( EC_COMPILEDP(compiled) );

	if (EC_COMPILEDISMETHOD(compiled))
		method_nargs = 2;
	else
		method_nargs = 0;

	nargs_req = EC_COMPILEDNARG(compiled) - method_nargs;

	vargs      = nargs - nargs_req;
	/* XXX TODO: Find a solution for ParameterCountError */
	if (vargs < 0)
		return EcParameterCountError( /* target   */ EC_NIL,
									  /* function */ compiled,
									  /* expected */ nargs_req,
									  /* no stack */ EC_NIL );
	/* XXX TODO: Check vargs */ ASSERT( vargs >= 0 );
	nargs_real = nargs_req + ((vargs > 0) ? 1 : 0);

	/* fprintf( stderr, "nargs_real: %ld  vargs: %ld\n", (long)nargs_real, (long)vargs ); */

	stack = EcMakeStack( EC_NIL,
						 1 + 2 + nargs_real + EC_COMPILEDNLOC(compiled) + EC_COMPILEDMAXTEMPS(compiled) );
	EC_STACKNARGS_REAL(stack)    = nargs_real;
	EC_STACKNARGS_LOGICAL(stack) = nargs;
	EC_STACKLEXICAL(stack) = EC_COMPILEDLEXICAL(compiled);
#if EC_STACK_RECYCLE
	if (EC_STACKP(EC_STACKLEXICAL(stack)))
		EC_STACKREF_INC(EC_STACKLEXICAL(stack));
#endif

	/* Set SP and BP */
	EC_STACKBP(stack)      = EC_STACKBASE(stack) + 1 + 2 + nargs;
	EC_STACKPOINTER(stack) = EC_STACKBP(stack) + EC_COMPILEDNLOC(compiled);

	return stack;
}

EC_API EC_OBJ EcMakeStackForMethod( EC_OBJ target, EcUInt methodid, EcInt nargs )
{
	EC_OBJ method;

	ASSERT( EC_CLASSP(target) || EC_OBJECTP(target) );

	if (EC_CLASSP(target))
		method = EcFindClassMethod( target, NULL, methodid );
	else
		method = EcFindMethod( target, NULL, methodid );

	ASSERT( EC_CMETHODP(method) || EC_COMPILEDP(method) || EC_NULLP(method) );

	return EcMakeStackForMethod2( target, method, nargs );
}

EC_API EC_OBJ EcMakeStackForMethod2( EC_OBJ target, EC_OBJ method, EcInt nargs )
{
	EC_OBJ stack;

	ASSERT( EC_CLASSP(target) || EC_OBJECTP(target) );
	ASSERT( EC_CMETHODP(method) || EC_COMPILEDP(method) || EC_NULLP(method) );

	if (EC_CMETHODP(method))
	{
		stack = EcMakeStack( EC_NIL, 1 + 2 + nargs );
		EC_STACKNARGS_REAL(stack)    = nargs;
		EC_STACKNARGS_LOGICAL(stack) = nargs;

		/* Set SP and BP */
		EC_STACKBP(stack)      = EC_STACKBASE(stack) + 1 + 2 + nargs;
		EC_STACKPOINTER(stack) = EC_STACKBP(stack);
	}
	else if (EC_COMPILEDP(method))
	{
		EcInt nargs_real, vargs;

		vargs      = nargs - (EC_COMPILEDNARG(method) - 2);
		/* XXX TODO: Find a solution for ParameterCountError */
		if (vargs < 0)
			return EcParameterCountError( /* target   */ target,
										  /* function */ method,
										  /* expected */ EC_COMPILEDNARG(method)-2,
										  /* no stack */ EC_NIL );
		/* XXX TODO: Check vargs */ ASSERT( vargs >= 0 );
		nargs_real = (EC_COMPILEDNARG(method) - 2) + ((vargs > 0) ? 1 : 0);

		stack = EcMakeStack( EC_NIL,
							 1 + 2 + nargs_real +
							 EC_COMPILEDNLOC(method) + EC_COMPILEDMAXTEMPS(method) );
		EC_STACKNARGS_REAL(stack)    = nargs_real;
		EC_STACKNARGS_LOGICAL(stack) = nargs;

		/* Unknown things. */
		EC_STACKSET( stack, 0, EC_NIL );						/* caller   */
		EC_STACKSET( stack, 1, EC_NIL );						/* self     */
		EC_STACKSET( stack, 2, EC_NIL );						/* at_class */

		/* Set SP and BP */
		EC_STACKBP(stack)      = EC_STACKBASE(stack) + 1 + 2 + nargs;
		EC_STACKPOINTER(stack) = EC_STACKBP(stack) + EC_COMPILEDNLOC(method);
	} else if (EC_NULLP(method))
	{
		/*
		 * No such method, BUT build the stack as it was requested, to allow
		 * processing of 'doesNotUnderstand' method and/or UnknownMethodError exception.
		 * Be conservative on number of arguments (varargs expanded on stack).
		 */
		stack = EcMakeStack( EC_NIL, 1 + 2 + nargs );
		EC_STACKNARGS_REAL(stack)    = nargs;
		EC_STACKNARGS_LOGICAL(stack) = nargs;

		/* Set SP and BP */
		EC_STACKBP(stack)      = EC_STACKBASE(stack) + 1 + 2 + nargs;
		EC_STACKPOINTER(stack) = EC_STACKBP(stack);
	} else
	{
		ec_fprintf( stderr, "method: %w\n", method );
		ASSERT( FALSE );
		stack = EC_NIL;
	}

	return stack;
}

EC_API void EcStackPush( EC_OBJ stack, EC_OBJ obj )
{
#if EC_STACKCHECK_ENABLED
	if ((EC_STACKPOINTER(stack) - EC_STACKBASE(stack)) >= EC_STACKDIM(stack))
	{
		printf("Burp\n");
	}
	ASSERT( (EC_STACKPOINTER(stack) - EC_STACKBASE(stack)) < EC_STACKDIM(stack) );
#endif

	ASSERT( ! EC_ERRORP(obj) );
	*EC_STACKPOINTER(stack)++ = obj;
}

EC_API EC_OBJ EcStackPop( EC_OBJ stack )
{
#if EC_STACKCHECK_ENABLED
/*	ASSERT( (EC_STACKPOINTER(stack) - 1 >= EC_STACKBP(stack)) );*/
	ASSERT( (EC_STACKPOINTER(stack) - 1 >= EC_STACKBASE(stack)) );
#endif

	return *(--EC_STACKPOINTER(stack));
}

EC_API void EcStackSet(  EC_OBJ stack, EcInt i, EC_OBJ obj )
{
#if EC_STACKCHECK_ENABLED
	ASSERT( (i >= 0) && (i < EC_STACKDIM(stack)) );
#endif

	EC_STACKBASE(stack)[i] = obj;
}

EC_API EC_OBJ EcStackGet( EC_OBJ stack, EcInt i )
{
#if EC_STACKCHECK_ENABLED
	ASSERT( (i >= 0) && (i < EC_STACKDIM(stack)) );
/*	ASSERT( i < (EC_STACKPOINTER(stack) - EC_STACKBASE(stack)) );*/
#endif

	return EC_STACKBASE(stack)[i];
}

EC_API void EcStackBPSet(  EC_OBJ stack, EcInt i, EC_OBJ obj )
{
#if EC_STACKCHECK_ENABLED
	ASSERT( (i >= 0) && (i < EC_STACKDIM(stack)) );
#endif

	EC_STACKBP(stack)[i] = obj;
}

EC_API EC_OBJ EcStackBPGet( EC_OBJ stack, EcInt i )
{
#if EC_STACKCHECK_ENABLED
	ASSERT( (i >= 0) && (i < EC_STACKDIM(stack)) );
	ASSERT( i < (EC_STACKPOINTER(stack) - EC_STACKBASE(stack)) );
#endif

	return EC_STACKBP(stack)[i];
}

/* Parsing a stack into a series of arguments */

EC_API EC_OBJ EcParseStack( EC_OBJ target, EC_OBJ function, EcBool consume, EC_OBJ stack, const char *format, ... )
{
	EC_OBJ res;
	va_list args;

	va_start( args, format );
	res = EcParseStackV( target, function, consume, stack, format, args );
	va_end( args );

	return res;
}

EC_API EC_OBJ EcParseStackFunction( const char *funcname, EcBool consume, EC_OBJ stack, const char *format, ... )
{
	EC_OBJ res;
	va_list args;

	va_start( args, format );
	res = EcParseStackV( EC_NIL,
						 funcname ? EcGetQVariable( funcname ) : EC_NIL,
						 consume, stack, format, args );
	va_end( args );

	return res;
}

EC_API EC_OBJ EcParseStackMethod( EC_OBJ self, const char *methodname, EcBool consume, EC_OBJ stack, const char *format, ... )
{
	EC_OBJ res;
	va_list args;

	va_start( args, format );
	res = EcParseStackV( self,
						 EcFindMethod( self, NULL, EcInternSymbol( methodname ) ),
						 consume, stack, format, args );
	va_end( args );

	return res;
}

EC_API EC_OBJ EcParseStackClassMethod( EC_OBJ objclass, const char *methodname, EcBool consume, EC_OBJ stack, const char *format, ... )
{
	EC_OBJ res;
	va_list args;

	va_start( args, format );
	res = EcParseStackV( objclass,
						 EcFindClassMethod( objclass, NULL, EcInternSymbol( methodname ) ),
						 consume, stack, format, args );
	va_end( args );

	return res;
}

static EcInt count_required( const char *format )
{
	EcInt i, len, rem;

	len = strlen( format );
	for (i = 0; *format; format++, len--)
	{
		rem = len - 1;

		if (*format == '|')
			return i;

		switch (*format)
		{
		case 'z':
		case 's':
			i++;
			if (rem && (format[1] == '#'))						/* generic string */
				format++;
			break;

		case 'b':
		case 'i':
		case 'c':
		case 'f':
		case 'k':
			i++;
			break;

		case 'o':
			/* TODO: This should be optional !!!! */
			ASSERT( FALSE );
			if (rem && (format[1] == '#'))
				format++;
			break;

		case 'O':
			i++;
			if (rem && (format[1] == '!'))
				format++;
			else if (rem && (format[1] == '&'))
				format++;
			break;

		case 'C':
			i++;
			if (rem && (format[1] == '!'))
				format++;
			break;

		default:
			/* Bad parameter format specification */
			ASSERT( FALSE );
		}
	}

	return i;
}

EC_API EC_OBJ EcParseStackV( EC_OBJ target, EC_OBJ function, EcBool consume, EC_OBJ stack, const char *format, va_list args )
{
	EcInt   i, len, rem;
	EC_OBJ  obj;
	EC_OBJ  res;
	EcInt   nargs;

	EC_OBJ     *parameterbase;									/* Start of params on stack */
	EcInt       original_nargs;
	const char *original_format;

	ec_string ds;												/* For error messages */

	EcBool optional = FALSE;

	/* Variables for conversion */
	char    **strp;
	EcInt    *strlenp;
	EcBool   *boolp;
	EcInt    *intp;
	EcChar   *charp;
	EcFloat  *floatp;
	EcUInt   *symp;
	EC_OBJ  **arrayp;
	EcInt    *arraylenp;
	EcInt     arraylen;
	EC_OBJ   *objp;
	EcInt     type;
	EC_OBJ    ofclass;
	EcParamConverter  converter;
	void             *anythingp;
	EcDWord           param1;
	EcDWord           param2;

	ASSERT( EC_STACKNARGS_LOGICAL(stack) == EC_STACKNARGS_REAL(stack) );
	nargs = EC_STACKNARGS_REAL(stack);

	original_nargs  = nargs;
	original_format = format;

	/* Parameters start here... */
	ASSERT( nargs <= EC_STACKDIM(stack) );
	parameterbase = EC_STACKPOINTER(stack) - nargs;
	ASSERT( parameterbase >= EC_STACKBASE(stack) );

	res = EcTrueObject;

	len = strlen( format );
	for (i = 0; *format; format++, len--)
	{
		rem = len - 1;

		if (*format == '|')
		{
			optional = TRUE;
			continue;
		}

		if ((! optional) && (nargs <= 0))
		{
			EcInt required_args;

			required_args = count_required( original_format );
			res = EcParameterCountError( target, function, required_args, stack );
			goto finally_error;
		}

		if (optional && (nargs <= 0)) continue;

		switch (*format)
		{
		case 'z':
			obj = *parameterbase++;
			nargs--; i++;
			if ((! EC_STRINGP(obj)) && (! EC_NULLP(obj)))
			{
				res = EcTypeError( target, function, i, tc_none, obj, TRUE, "expected 'string' or @nil" );
				goto finally_error;
			}
			goto string_continue;
			break;

		case 's':
			obj = *parameterbase++;
			nargs--; i++;
			if (! EC_STRINGP(obj))
			{
				res = EcTypeError( target, function, i, tc_string, obj, TRUE, NULL );
				goto finally_error;
			}
		string_continue:
			ASSERT( EC_NULLP(obj) || EC_STRINGP(obj) );
			if (rem && (format[1] == '#'))						/* generic string */
			{
				strp    = va_arg( args, char ** );
				strlenp = va_arg( args, EcInt * );
				if (! EC_NULLP(obj))
				{
					*strp    = EC_STRDATA(obj);
					*strlenp = EC_STRLEN(obj);
				} else
				{
					*strp    = NULL;
					*strlenp = 0;
				}

				format++;
			} else												/* null-terminated string */
			{
				strp = va_arg( args, char ** );
				if (! EC_NULLP(obj))
				{
					if (EC_STRLEN(obj) && (EC_STRLEN(obj) > strlen( EC_STRDATA(obj) )))
					{
						res = EcTypeError( target, function, i, tc_none, obj, TRUE, "string not null-terminated" );
						goto finally_error;
					}
					if (EC_STRLEN(obj))
						*strp = EC_STRDATA(obj);
					else
						*strp = "";
				} else
				{
					*strp = NULL;
				}
			}
			break;

		case 'b':
			obj = *parameterbase++;
			nargs--; i++;
			boolp = va_arg( args, EcBool * );
			*boolp = EC_ASBOOLVAL(obj);
			break;

		case 'i':
			obj = *parameterbase++;
			nargs--; i++;
			intp = va_arg( args, EcInt * );
			if (EC_INUMP(obj))
			{
				*intp = EC_INUM(obj);
			} else if (EC_FNUMP(obj))
			{
				*intp = (EcInt)EC_FNUM(obj);
			} else
			{
				res = EcTypeError( target, function, i, tc_inum, obj, TRUE, NULL );
				goto finally_error;
			}
			break;

		case 'c':
			obj = *parameterbase++;
			nargs--; i++;
			charp = va_arg( args, EcChar * );
			if (! EC_CHARP(obj))
			{
				res = EcTypeError( target, function, i, tc_char, obj, TRUE, NULL );
				goto finally_error;
			}
			*charp = EC_CHAR(obj);
			break;

		case 'f':
			obj = *parameterbase++;
			nargs--; i++;
			floatp = va_arg( args, EcFloat * );
			if (EC_FNUMP(obj))
			{
				*floatp = EC_FNUM(obj);
			} else if (EC_INUMP(obj))
			{
				*floatp = (EcFloat)EC_INUM(obj);
			} else
			{
				res = EcTypeError( target, function, i, tc_fnum, obj, TRUE, NULL );
				goto finally_error;
			}
			break;

		case 'k':
			obj = *parameterbase++;
			nargs--; i++;
			symp = va_arg( args, EcUInt * );
			if (! EC_SYMBOLP(obj))
			{
				res = EcTypeError( target, function, i, tc_symbol, obj, TRUE, NULL );
				goto finally_error;
			}
			*symp = EC_SYMBOL(obj);
			break;

		case 'o':
			ASSERT( optional );
			if (rem && (format[1] == '#'))
			{
				arraylen = va_arg( args, EcInt );
				arrayp   = va_arg( args, EC_OBJ ** );
				if (arraylen != nargs)
				{
					/* TODO: Throw an exception */
					ASSERT( FALSE );
					res = Ec_ERROR;
					goto finally_error;
				}
				*arrayp        = parameterbase;
				parameterbase += nargs;
				i             += nargs;
				nargs          = 0;

				format++;
			} else
			{
				arraylenp = va_arg( args, EcInt * );
				arrayp    = va_arg( args, EC_OBJ ** );
				*arraylenp = nargs;
				*arrayp    = parameterbase;
				parameterbase += nargs;
				i             += nargs;
				nargs          = 0;
			}
			break;

		case 'O':
			obj = *parameterbase++;
			nargs--; i++;
			if (rem && (format[1] == '!'))
			{
				type = va_arg( args, EcInt );
				objp = va_arg( args, EC_OBJ * );
				if ((EC_NULLP(obj) && (type != tc_none)) ||
					(EC_TYPE(obj) != type))
				{
					res = EcTypeError( target, function, i, type, obj, TRUE, NULL );
					goto finally_error;
				}
				*objp = obj;

				format++;
			} else if (rem && (format[1] == '&'))
			{
				converter = va_arg( args, EcParamConverter );
				anythingp = va_arg( args, void * );
				param1    = va_arg( args, EcDWord );
				param2    = va_arg( args, EcDWord );
				res = converter( i, obj, anythingp, param1, param2 );
				if (EC_ERRORP(res))
					goto finally_error;

				format++;
			} else
			{
				objp = va_arg( args, EC_OBJ * );
				*objp = obj;
			}
			break;

		case 'C':
			obj = *parameterbase++;
			nargs--; i++;
			if (rem && (format[1] == '!'))
			{
				ofclass = va_arg( args, EC_OBJ );
				objp    = va_arg( args, EC_OBJ * );
				ASSERT( EC_NULLP(ofclass) || EC_CLASSP(ofclass) );
				if ((! EC_OBJECTP(obj)) || (! EcIsOfClass( obj, ofclass )))
				{
					ec_string_init( &ds, NULL );
					ec_sprintf( &ds, "expected an object of class %w",
								EC_NNULLP(ofclass) ? EC_CLASSNAME(ofclass) : EC_NIL );
					res = EcTypeError( target, function, i, tc_object, obj, TRUE, ec_strdata( &ds ) );
					ec_string_cleanup( &ds );
					goto finally_error;
				}
				*objp = obj;

				format++;
			} else
			{
				objp = va_arg( args, EC_OBJ * );
				if (! EC_OBJECTP(obj))
				{
					res = EcTypeError( target, function, i, tc_object, obj, TRUE, NULL );
					goto finally_error;
				}
				*objp = obj;
			}
			break;

		case '.':												/* ignore this parameter ! */
			obj = *parameterbase++;
			nargs--; i++;
			break;

		default:
			/* Bad parameter format specification */
			ASSERT( FALSE );
			res = Ec_ERROR;
			goto finally_error;
		}
	}

	if (nargs > 0)
	{
		/* we left some args not parsed ! */
		res = EcParameterCountError( target, function, i, stack );
	}

finally_error:
	if (consume) EC_STACKPOINTER(stack) -= original_nargs;
	return res;
}


/* elastiC API */


/* Private */

static EC_OBJ stack_copy( EC_OBJ obj, EcCopyType type )
{
	/*******************************************************************
	 * Stacks are *SPECIAL* !                                          *
	 *******************************************************************/
	return obj;
}

static void stack_mark( EC_OBJ obj )
{
	EC_OBJ *stk;

	ASSERT( EC_STACKP(obj) );

	ASSERT( EC_NULLP(EC_STACKUP(obj))      || EC_STACKP(EC_STACKUP(obj)) );
	ASSERT( EC_NULLP(EC_STACKLEXICAL(obj)) || EC_STACKP(EC_STACKLEXICAL(obj)) );
	if (EC_NNULLP(EC_STACKUP(obj)))      EcMarkObject( EC_STACKUP(obj) );
	if (EC_NNULLP(EC_STACKLEXICAL(obj))) EcMarkObject( EC_STACKLEXICAL(obj) );
	if (EC_NNULLP(EC_STACKIMMUP(obj)))     EcMarkObject( EC_STACKIMMUP(obj) );
	if (EC_NNULLP(EC_STACKIMMCALLER(obj))) EcMarkObject( EC_STACKIMMCALLER(obj) );
	if (EC_NNULLP(EC_STACKCALLED(obj)))    EcMarkObject( EC_STACKCALLED(obj) );
	for (stk = EC_STACKBASE(obj); stk < EC_STACKPOINTER(obj); stk++)
		if (EC_NNULLP(*stk)) EcMarkObject( *stk );
}

static void stack_free( EC_OBJ obj )
{
	ASSERT( EC_STACKP(obj) );

#if EC_STACK_RECYCLE
	ASSERT(EC_STACKP(obj));
#if EC_STACK_RECYCLE_DEBUG_MSGS
	if ((! PRIVATE(in_cleanup)) && (EC_STACKREF(obj) > 0))
	{
		ec_fprintf(stderr, "stack_free ref: %ld (0x%08lX)\n",
				   (long)EC_STACKREF(obj), (unsigned long)obj);
	}
#endif

	/*
	 * Ideally here the reference count should be 0
	 *
	 * If it is greater than 0, it means that our refcounting
	 * system is not optimal: we don't recycle optimally all the stacks
	 * we could.
	 *
	 * If it is smaller than 0, there is something _SERIOUSLY_ wrong !
	 * We could end up recycling stacks that are still referenced by
	 * other objects !!!
	 */

#if EC_STACK_RECYCLE_VERY_PARANOID_CHECKS
	EC_WARN_IF(EC_STACK_RECYCLE_PARANOID_CHECKS && (! PRIVATE(in_cleanup)),
			   EC_STACKREF(obj) > 0);
#endif

	ASSERT( EC_STACKREF(obj) >= 0 );

	if (EC_STACKREF(obj) <= 0)
	{
		/* Recycle this stack */

		int first_free_slot;

		ASSERT( EC_STACKREF(obj) == 0 );

#if EC_STACK_RECYCLE_STATS
		PRIVATE(n_recycle_put_attempts)++;
#endif

		ec_bit_ffc(PRIVATE(stack_pool_bmap),
				   EC_STACK_POOL_SIZE,
				   &first_free_slot);
		if (first_free_slot >= 0)
		{
			PRIVATE(stack_pool)[first_free_slot] = obj;
			ec_bit_set(PRIVATE(stack_pool_bmap), first_free_slot);

#if EC_STACK_RECYCLE_STATS
			PRIVATE(n_recycle_put)++;
#endif

			/* if (EC_STACKP(EC_STACKUP(stack))) {
			   STACK_DEC_REF(EC_STACKUP(stack));
			   } */

/*			if (EC_STACKP(EC_STACKIMMUP(stack))) {
			    STACK_DEC_REF(EC_STACKIMMUP(stack));
			}
*/

			EC_STACKUP(obj)        = EC_NIL;
#if EC_STACK_RECYCLE
			if (EC_STACKP(EC_STACKLEXICAL(obj)))
			    EC_STACKREF_DEC(EC_STACKLEXICAL(obj));
#endif
			EC_STACKLEXICAL(obj)   = EC_NIL;
			EC_STACKPOINTER(obj)   = EC_STACKBASE(obj);
			EC_STACKBP(obj)        = EC_STACKPOINTER(obj);
			EC_STACKPC(obj)        = 0;
			EC_STACKIMMUP(obj)     = EC_NIL;
			EC_STACKIMMCALLER(obj) = EC_NIL;
			EC_STACKIMMPC(obj)     = 0;
			EC_STACKCALLED(obj)    = EC_NIL;

			return;
		} else
		{
#if EC_STACK_RECYCLE_STATS
			PRIVATE(n_pool_full)++;
#endif
		}
	}
#endif


	EC_STACKUP(obj)        = EC_NIL;
#if EC_STACK_RECYCLE
	if (EC_STACKP(EC_STACKLEXICAL(obj))) {
		EC_STACKREF_DEC(EC_STACKLEXICAL(obj));
	}
#endif
	EC_STACKLEXICAL(obj)   = EC_NIL;
#if EC_STACK_USES_STATIC
	if (EC_STACKBASE(obj) != EC_STACKSTATIC(obj))
		ec_free( EC_STACKBASE(obj) );
#else
	ec_free( EC_STACKBASE(obj) );
#endif
	EC_STACKBASE(obj)      = NULL;
	EC_STACKPOINTER(obj)   = NULL;
	EC_STACKBP(obj)        = NULL;
	EC_STACKDIM(obj)       = 0;
	EC_STACKPC(obj)        = 0;
	EC_STACKIMMUP(obj)     = EC_NIL;
	EC_STACKIMMCALLER(obj) = EC_NIL;
	EC_STACKIMMPC(obj)     = 0;
	EC_STACKCALLED(obj)    = EC_NIL;
	ec_free( EC_STACK(obj) );
	EC_STACK(obj) = NULL;
}

static EcInt stack_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	EcInt res = 0;
	ASSERT( EC_STACKP(obj) );

	if (detailed)
		res = ec_sprintf( str, "<stack>" );
	else
		res = ec_sprintf( str, "<stack>" );
	return res;
}

static EC_OBJ stack_getitem( EC_OBJ obj, EC_OBJ index )
{
	if (! EC_INUMP(index))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	if ((EC_INUM(index) < 0) || (EC_INUM(index) >= EC_STACKDIM(obj)))
	{
		ec_string ds;

		ec_string_init( &ds, NULL );
		ec_sprintf( &ds, "Index %ld out of bounds for object %w", EC_INUM(index), obj );

		EcMakeExceptionWithArgs( EcIndexErrorClass,
								 ec_strdata( &ds ), 2,
								 index, obj );
		ec_string_cleanup( &ds );
		return Ec_ERROR;
	}

	return EcStackGet( obj, EC_INUM(index) );
}

static EC_OBJ stack_setitem( EC_OBJ obj, EC_OBJ index, EC_OBJ value )
{
	if (! EC_INUMP(index))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	if ((EC_INUM(index) < 0) || (EC_INUM(index) >= EC_STACKDIM(obj)))
	{
		ec_string ds;

		ec_string_init( &ds, NULL );
		ec_sprintf( &ds, "Index %ld out of bounds for object %w", EC_INUM(index), obj );

		EcMakeExceptionWithArgs( EcIndexErrorClass,
								 ec_strdata( &ds ), 2,
								 index, obj );
		ec_string_cleanup( &ds );
		return Ec_ERROR;
	}

	EcStackSet( obj, EC_INUM(index), value );
	return obj;
}

static EC_OBJ stack_length( EC_OBJ obj )
{
	return EcMakeInt( EC_STACKDIM(obj) );
}

static EcUInt stack_hash( EC_OBJ obj, EcInt recur )
{
	EcUInt res;

	ASSERT( EC_STACKP(obj) );

	res = 0;
	if (! EC_STACK(obj))
		return res;
	res += _ec_hash( EC_STACKUP(obj), recur );
	res += _ec_hash( EC_STACKLEXICAL(obj), recur );
	res += ec_hash_uint( (EcUInt)EC_STACKDIM(obj) );
	res += ec_hash_uint( (EcUInt)EC_STACKPC(obj) );
	res += _ec_hash( EC_STACKIMMUP(obj), recur );
	res += _ec_hash( EC_STACKIMMCALLER(obj), recur );
	res += ec_hash_uint( (EcUInt)EC_STACKIMMPC(obj) );
	res += _ec_hash( EC_STACKCALLED(obj), recur );
	if (! EC_STACKBASE(obj))
		return res;
	if (EC_STACKBASE(obj))
	{
		EcInt i;

		for (i = 0; i < EC_STACKDIM(obj); i++)
			res += _ec_hash( EC_STACKBASE(obj)[i], recur );
	}
	return res;
}

static EcBool stack_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res )
{
	if (obj1 == obj2)
	{
		*res = 0;
		return TRUE;
	}

	if (! EC_STACKP(obj2))
		return FALSE;

	return FALSE;
}

static EcBool stack_check( EC_OBJ obj )
{
/*	ASSERTMEM( EC_STACK(obj) );
	ASSERTCOND( EC_STACKPOINTER(obj) >= EC_STACKBASE(obj) );
	ASSERTCOND( EC_STACKBP(obj) >= EC_STACKBASE(obj) );
	ASSERTCOND( EC_STACKDIM(obj) >= 0 );
	ASSERTFIELDN( EC_STACKUP(obj), tc_stack );
	ASSERTFIELDN( EC_STACKLEXICAL(obj), tc_stack );
	ASSERTCOND( EC_STACKPC(obj) >= 0 );
	ASSERTFIELDN( EC_STACKIMMUP(obj), tc_stack );
	ASSERTFIELDN( EC_STACKIMMCALLER(obj), tc_compiled );
	ASSERTCOND( EC_STACKIMMPC(obj) >= 0 );
	ASSERTFIELDN( EC_STACKCALLED(obj), tc_compiled );
*/
	return TRUE;
}

EcBool _ec_stack_init( void )
{
	EcSequenceCallbacks sequence_cb = {
		/* getitem */		stack_getitem,
		/* setitem */		stack_setitem,
		/* delitem */		NULL,
		/* length  */		stack_length
	};

	EcTypespec stackspec = {
		/* type    */		0,
		/* name    */		"stack",

		/* copy    */		stack_copy,

		/* mark    */		stack_mark,
		/* free    */		stack_free,
		/* print   */		stack_print,

		/* hash    */		stack_hash,
		/* compare */		stack_compare,

		/* check   */		stack_check,

		/* sequence */      &sequence_cb,
		/* numeric  */      NULL
	};

	tc_stack = EcAddType( &stackspec );
	if (tc_stack == 0)
		return FALSE;

	return TRUE;
}

void _ec_stack_cleanup( void )
{
}
