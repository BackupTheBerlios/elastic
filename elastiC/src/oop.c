/* ==========================================================================
 * oop.c
 * ==========================================================================
 * Object Oriented Programming support.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat Dec 12 22:01:08 MET 1998
 *
 *   $Id: oop.c,v 1.1 2002/05/23 21:17:28 panta Exp $
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

#define EC_STACK_RECYCLE_OPS 1
#include "basic.h"
#include "debug.h"

#include "elastic.h"

#include "private.h"

#include "stackrecycle.h"


static inline EcInt  search_method( EcInt nmethods, EcMethodEntry *mtable, EcUInt methodid );
static inline EcInt  search_variable( EcInt nvars, EcVariableEntry *vtable, EcUInt symid );
static        EcBool does_object_understand( EC_OBJ obj, EcUInt methodid );
static        EcBool does_class_understand( EC_OBJ classobj, EcUInt methodid );

/* EcObject Oriented Programming */

EC_API EC_OBJ EcAddClass( const char *package, const char *name, EC_OBJ superclass )
{
	EC_OBJ packageObj;
	EC_OBJ res;

	packageObj = EcPackageFromName( package );

	res = EcMakeClass( packageObj, superclass, name );
	if (EC_ERRORP(res)) return res;

	ASSERT( EC_CLASSP(res) );

	if (EcPackageVariable( packageObj,
						   name,
						   res, TRUE, TRUE ) < 0)
		return Ec_ERROR;

	return res;
}

EC_API EC_OBJ  EcAddClassDef( EcClassDef *classdef )
{
	EC_OBJ newclass;
	EcCMethodDef  *methodlist;
	EcVariableDef *varlist;

	newclass = EcAddClass( classdef->pkgname, classdef->name, classdef->superclass );
	if (EC_ERRORP(newclass))
		return newclass;

	ASSERT( EC_CLASSP(newclass) );

	/* class methods and variables */

	methodlist = classdef->classmethods;
	while (methodlist && methodlist->name)
	{
		ASSERT( methodlist );
		ASSERT( methodlist->name );
		EcAddClassMethod( newclass, methodlist->name, EcMakeCMethod( methodlist->impl, methodlist->name ) );

		methodlist++;
	}

	varlist = classdef->classvariables;
	while (varlist && varlist->name)
	{
		ASSERT( varlist );
		ASSERT( varlist->name );
		EcAddClassVariable( newclass, varlist->name );

		varlist++;
	}

	/* instance methods and variables */

	methodlist = classdef->methods;
	while (methodlist && methodlist->name)
	{
		ASSERT( methodlist );
		ASSERT( methodlist->name );
		EcAddMethod( newclass, methodlist->name, EcMakeCMethod( methodlist->impl, methodlist->name ) );

		methodlist++;
	}

	varlist = classdef->variables;
	while (varlist && varlist->name)
	{
		ASSERT( varlist );
		ASSERT( varlist->name );
		EcAddInstanceVariable( newclass, varlist->name );

		varlist++;
	}

	return newclass;
}

EC_API EcBool EcAddMethod( EC_OBJ classobj, const char *name, EC_OBJ impl )
{
	EcInt i, pos, cmp;
	EcUInt symid;

	EcMethodEntry *newmtable;
	EcInt          newsize;

	ASSERT( EC_CLASSP(classobj) );
	ASSERT( name );
	ASSERT( EC_NNULLP(impl) );

	/* Insert at the right place */
	symid = EcInternSymbol( name );
	for (i = 0, pos = 0; i < EC_CLASSNMETHODS(classobj); i++)
	{
		cmp = EC_CLASSMTABLE(classobj)[i].symid - symid;
		if (cmp < 0)
			pos++;
		else if (cmp > 0)
			break;
		else
		{
			/* Found ! */
			return FALSE;
		}
	}

	newsize   = EC_CLASSNMETHODS(classobj) + 1;
	newmtable = ec_realloc( EC_CLASSMTABLE(classobj), newsize * sizeof(EcMethodEntry) );
	if (! newmtable)
		return FALSE;
	EC_CLASSMTABLE(classobj)   = newmtable;
	EC_CLASSNMETHODS(classobj) = newsize;

	ASSERT( pos >= 0 );
	ASSERT( pos < EC_CLASSNMETHODS(classobj) );

	/* Shift greater elements */
	for (i = EC_CLASSNMETHODS(classobj)-1; i > pos; i--)
		memcpy( &(EC_CLASSMTABLE(classobj)[i]), &(EC_CLASSMTABLE(classobj)[i-1]), sizeof(EcMethodEntry) );

	ASSERT( EC_CMETHODP(impl) || EC_COMPILEDP(impl) );

	EC_CLASSMTABLE(classobj)[pos].symid = symid;
	EC_CLASSMTABLE(classobj)[pos].impl  = impl;

	return TRUE;
}

EC_API EcBool EcAddClassMethod( EC_OBJ classobj, const char *name, EC_OBJ impl )
{
	EcInt i, pos, cmp;
	EcUInt symid;

	EcMethodEntry *newcmtable;
	EcInt          newsize;

	ASSERT( EC_CLASSP(classobj) );
	ASSERT( name );
	ASSERT( EC_NNULLP(impl) );

	/* Insert at the right place */
	symid = EcInternSymbol( name );
	for (i = 0, pos = 0; i < EC_CLASSNCMETHODS(classobj); i++)
	{
		cmp = EC_CLASSCMTABLE(classobj)[i].symid - symid;
		if (cmp < 0)
			pos++;
		else if (cmp > 0)
			break;
		else
		{
			/* Found ! */
			return FALSE;
		}
	}

	newsize   = EC_CLASSNCMETHODS(classobj) + 1;
	newcmtable = ec_realloc( EC_CLASSCMTABLE(classobj), newsize * sizeof(EcMethodEntry) );
	if (! newcmtable)
		return FALSE;
	EC_CLASSCMTABLE(classobj)   = newcmtable;
	EC_CLASSNCMETHODS(classobj) = newsize;

	ASSERT( pos >= 0 );
	ASSERT( pos < EC_CLASSNCMETHODS(classobj) );

	/* Shift greater elements */
	for (i = EC_CLASSNCMETHODS(classobj)-1; i > pos; i--)
		memcpy( &(EC_CLASSCMTABLE(classobj)[i]), &(EC_CLASSCMTABLE(classobj)[i-1]), sizeof(EcMethodEntry) );

	EC_CLASSCMTABLE(classobj)[pos].symid = symid;
	EC_CLASSCMTABLE(classobj)[pos].impl  = impl;

	return TRUE;
}

EC_API EcInt EcAddInstanceVariable( EC_OBJ classobj, const char *name )
{
	EcInt i;
	EcUInt symid;
	EcInt offset;
	EcInt specific;

	EcVariableEntry *newivtable;
	EcInt            newsize;

	ASSERT( EC_CLASSP(classobj) );
	ASSERT( name );

	symid = EcInternSymbol( name );
	ASSERT( symid > 0 );

	specific = EC_CLASSNIVARS(classobj) - EC_CLASSIOFFSET(classobj);
	ASSERT( specific >= 0 );
	/* Already present ? */
	for (i = 0; i < specific; i++)
	{
		if (EC_CLASSIVTABLE(classobj)[i].symid == symid)
			return -1;
	}

	newsize    = specific + 1;
	ASSERT( newsize >= 1 );
	newivtable = ec_realloc( EC_CLASSIVTABLE(classobj), newsize * sizeof(EcVariableEntry) );
	if (! newivtable)
		return -1;
	EC_CLASSIVTABLE(classobj) = newivtable;
	EC_CLASSNIVARS(classobj)  = EC_CLASSIOFFSET(classobj) + newsize;

	offset = newsize - 1;										/* offset is from ioffset */
	EC_CLASSIVTABLE(classobj)[newsize - 1].symid  = symid;
	EC_CLASSIVTABLE(classobj)[newsize - 1].offset = offset;

	return offset;
}

EC_API EcInt EcAddClassVariable( EC_OBJ classobj, const char *name )
{
	EcInt i;
	EcUInt symid;
	EcInt offset;

	EcVariableEntry *newcvtable;
	EcInt            newsize;

	ASSERT( EC_CLASSP(classobj) );
	ASSERT( name );

	symid = EcInternSymbol( name );

	/* Already present ? */
	for (i = 0; i < EC_CLASSNCVARS(classobj); i++)
	{
		if (EC_CLASSCVTABLE(classobj)[i].symid == symid)
			return -1;
	}

	newsize    = EC_CLASSNCVARS(classobj) + 1;
	newcvtable = ec_realloc( EC_CLASSCVTABLE(classobj), newsize * sizeof(EcVariableEntry) );
	if (! newcvtable)
		return -1;
	EC_CLASSCVTABLE(classobj) = newcvtable;
	EC_CLASSNCVARS(classobj)  = newsize;

	offset = EcArrayPush( EC_CLASSLFRAME(classobj), EC_NIL );
	EC_CLASSCVTABLE(classobj)[EC_CLASSNCVARS(classobj) - 1].symid  = symid;
	EC_CLASSCVTABLE(classobj)[EC_CLASSNCVARS(classobj) - 1].offset = offset;

	return offset;
}

EC_API EcBool EcDoesUnderstand( EC_OBJ obj, EcUInt methodid )
{
	if (EC_OBJECTP(obj))
		return does_object_understand( obj, methodid );
	else if (EC_CLASSP(obj))
		return does_class_understand( obj, methodid );
	else
		return FALSE;
}

EC_API EC_OBJ EcUnknownMethod( EC_OBJ obj, EC_OBJ at_class, EcUInt methodid, EC_OBJ stack )
{
	EC_OBJ    res;
	ec_string ds;
	EcInt     nargs_real, nargs_logical;

	if (EcDoesUnderstand( obj, PRIVATE(doesNotUnderstandID) ))
	{
		EC_OBJ newstack, oldargs, arg;
		EcInt  newnargs;
		EcInt  i;

		if (EC_NNULLP(stack))
		{
			nargs_real    = EC_STACKNARGS_REAL(stack);
			nargs_logical = EC_STACKNARGS_LOGICAL(stack);
		} else
			nargs_real = nargs_logical = 0;

		/*
		 * We must build a new stack frame, put in it the arguments to the unexisting method and
		 * the name of the method.
		 */
		newnargs = 2;
		newstack = EcMakeStackForMethod( obj, PRIVATE(doesNotUnderstandID), newnargs );
		if (EC_ERRORP(newstack)) return stack;
		EC_STACKLEXICAL(newstack) = EC_NIL;

		oldargs = EcMakeArray( nargs_real );

		/*
		 * XXXX TODO:
		 * In the stack we receive there could be present varargs encoded in an array (last real arg.).
		 * Do we have to unroll this last argument in the stack we build or not ???
		 * I think that we have to, in order to permit a correct 'apply'.
		 *
		 * To do so we have to check EC_STACKNARGS_REAL and EC_STACKNARGS_LOGICAL
		 */

		/* Copy original args */
		if (EC_NNULLP(stack))
		{
			for (i = 0; i < nargs_real; i++)
			{
				arg = *(EC_STACKPOINTER(stack) - nargs_real + i);
				EcArraySet( oldargs, i, arg );
			}
		}

		/* Set arguments: the method & original args */
		EC_STACKSET( newstack, 3, EcMakeSymbolFromId( methodid ) );
		EC_STACKSET( newstack, 4, oldargs );

		res = EcDispatchObjectMessage( obj, EcTrueObject, PRIVATE(doesNotUnderstandID), newstack );
#if EC_STACK_RECYCLE
#if EC_STACK_RECYCLE_PARANOID_CHECKS
		ASSERT( EC_STACKREF(newstack) == 1 );
#endif
		if (EC_STACKREF(newstack) > 0)
			EC_STACKREF_DEC(newstack);
#endif
		if (! EC_ERRORP(res))
			return res;
	}

	/* Method not found and doesNotUnderstand not defined */
	ec_string_init( &ds, NULL );
	ec_sprintf( &ds, "Method %k not found for object %w", methodid, obj );
	EcMakeExceptionWithArgs( EcUnknownMethodErrorClass,
							 ec_strdata( &ds ), 2,
							 EcMakeSymbolFromId( methodid ), obj );
	ec_string_cleanup( &ds );
	return Ec_ERROR;
}

EC_API EC_OBJ EcSendMessage( EC_OBJ obj, EcUInt methodid, EC_OBJ stack )
{
	if (EC_OBJECTP(obj))
		return EcDispatchObjectMessage( obj, EcTrueObject, methodid, stack );
	else if (EC_CLASSP(obj))
		return EcDispatchClassMessage( obj, methodid, stack );

	/* TODO: Should raise an exception */
	ASSERT( FALSE );
	return Ec_ERROR;
}

EC_API EC_OBJ EcSendMessageVA( EC_OBJ obj, EcUInt methodid, EcInt nargs, ... )
{
	va_list args;
	EC_OBJ stack;
	EC_OBJ res;
	EcInt i;

	va_start( args, nargs );

	stack = EcMakeStackForMethod( obj, methodid, nargs );
	if (EC_ERRORP(stack)) return stack;
	for (i = 0; i < nargs; i++)
		EC_STACKSET( stack, i + 3, va_arg( args, EC_OBJ ) );
	EC_STACKSET( stack, 1, obj );								/* 04/08/99 self     */
	EC_STACKSET( stack, 2, EC_OBJECTCLASS(obj) );				/* 04/08/99 at_class */
	res = EcSendMessage( obj, methodid, stack );
#if EC_STACK_RECYCLE
#if EC_STACK_RECYCLE_PARANOID_CHECKS
	ASSERT( EC_STACKREF(stack) == 1 );
#endif
	if (EC_STACKREF(stack) > 0)
		EC_STACKREF_DEC(stack);
#endif

	va_end( args );

	return res;
}

EC_API EC_OBJ EcSendMessageArgs( EC_OBJ obj, EcUInt methodid, EcInt nargs, EC_OBJ *args )
{
	EC_OBJ stack;
	EC_OBJ res;
	EcInt i;

	stack = EcMakeStackForMethod( obj, methodid, nargs );
	if (EC_ERRORP(stack)) return stack;
	for (i = 0; i < nargs; i++)
		EC_STACKSET( stack, i + 3, args[i] );
	EC_STACKSET( stack, 1, obj );								/* 04/08/99 self     */
	EC_STACKSET( stack, 2, EC_OBJECTCLASS(obj) );				/* 04/08/99 at_class */
	res = EcSendMessage( obj, methodid, stack );
#if EC_STACK_RECYCLE
#if EC_STACK_RECYCLE_PARANOID_CHECKS
	ASSERT( EC_STACKREF(stack) == 1 );
#endif
	if (EC_STACKREF(stack) > 0)
		EC_STACKREF_DEC(stack);
#endif

	return res;
}

EC_API EC_OBJ EcSendSuperMessage( EC_OBJ obj, EC_OBJ at_class, EcUInt methodid, EC_OBJ stack )
{
	if (EC_OBJECTP(obj))
	{
		if (EC_NULLP(at_class))
			at_class = EC_OBJECTCLASS(obj);
		return EcDispatchObjectMessage( obj, EC_CLASSSUPER(at_class), methodid, stack );
	}
	else if (EC_CLASSP(obj))
	{
		ASSERT( EC_NULLP(at_class) );
		return EcDispatchClassMessage( EC_CLASSSUPER(obj), methodid, stack );
	}

	/* TODO: Should raise an exception */
	ASSERT( FALSE );
	return Ec_ERROR;
}

EC_API EC_OBJ EcFindMethod( EC_OBJ obj, EC_OBJ *at_class, EcUInt methodid )
{
	EC_OBJ classobj;
	EcInt at;

	ASSERT( EC_OBJECTP(obj) );

	/*ec_fprintf( stderr, "EcFindMethod  methodid: %ld %k\n", (long)methodid, methodid );*/
	if (at_class && EC_NNULLP(*at_class))
		classobj = *at_class;
	else
		classobj = EC_OBJECTCLASS(obj);
	while (EC_NNULLP(classobj))
	{
		at = search_method( EC_CLASSNMETHODS(classobj), EC_CLASSMTABLE(classobj), methodid ); 
		if (at >= 0)
		{
			if (at_class) *at_class = classobj;
			return EC_CLASSMTABLE(classobj)[at].impl;
		}

		classobj = EC_CLASSSUPER(classobj);
	}

	if (at_class) *at_class = EC_NIL;
	return EC_NIL;
}

EC_API EC_OBJ EcFindClassMethod( EC_OBJ classobj, EC_OBJ *at_class, EcUInt methodid )
{
	EcInt at;

	ASSERT( EC_CLASSP(classobj) );

	while (EC_NNULLP(classobj))
	{
		at = search_method( EC_CLASSNCMETHODS(classobj), EC_CLASSCMTABLE(classobj), methodid ); 
		if (at >= 0)
		{
			if (at_class) *at_class = classobj;
			return EC_CLASSCMTABLE(classobj)[at].impl;
		}

		classobj = EC_CLASSSUPER(classobj);
	}

	return EC_NIL;
}

EC_API EC_OBJ EcDispatchObjectMessage( EC_OBJ obj, EC_OBJ at_class, EcUInt methodid, EC_OBJ stack )
{
	EC_OBJ classobj, callable;
	EcInt at;

	ASSERT( EC_OBJECTP(obj) );

/*	ec_fprintf( stderr,
				"EcDispatchObjectMessage   receiver: %w.%w  method: %k  nargs: %ld\n",
				at_class, obj, methodid, EC_NNULLP(stack) ? (long)EC_STACKNARGS_LOGICAL(stack) : 0 );*/

	if (EC_BOOLP(at_class))
		classobj = EC_OBJECTCLASS(obj);
	else
		classobj = at_class;

	while (EC_NNULLP(classobj))
	{
		at = search_method( EC_CLASSNMETHODS(classobj), EC_CLASSMTABLE(classobj), methodid ); 
/*		ec_fprintf( stderr,
					"   in class %w  at: %ld\n",
					classobj, (long)at );*/
		if (at >= 0)
		{
			callable = EC_CLASSMTABLE(classobj)[at].impl;
			switch (EC_TYPE(callable))
			{
			case tc_primitive:
				return EC_PRIMITIVE(callable)( stack, EC_PRIMITIVEUSERDATA(callable) );
				break;
			case tc_cmethod:
				return EC_CMETHOD(callable)( obj, classobj, stack );
				break;
			default:
				if (EC_COMPILEDP(callable))
					return EcExecute( obj, classobj, callable, stack );
				ASSERT( FALSE );
			}
		}

		classobj = EC_CLASSSUPER(classobj);
	}

	/* No such a method. Invoke doesNotUnderstand if available */
	return EcUnknownMethod( obj, at_class, methodid, stack );

	/* TODO: Should raise an exception */
	ASSERT( FALSE );
	return Ec_ERROR;
}

EC_API EC_OBJ EcDispatchClassMessage( EC_OBJ obj, EcUInt methodid, EC_OBJ stack )
{
	EcInt at;
	EC_OBJ callable;

	ASSERT( EC_CLASSP(obj) );

	while (EC_NNULLP(obj))
	{
		at = search_method( EC_CLASSNCMETHODS(obj), EC_CLASSCMTABLE(obj), methodid ); 
		if (at >= 0)
		{
			callable = EC_CLASSCMTABLE(obj)[at].impl;
			switch (EC_TYPE(callable))
			{
			case tc_primitive:
				return EC_PRIMITIVE(callable)( stack, EC_PRIMITIVEUSERDATA(callable) );
				break;
			case tc_cmethod:
				return EC_CMETHOD(callable)( obj, EC_NIL, stack );
				break;
			default:
				if (EC_COMPILEDP(callable))
					return EcExecute( EC_NIL, obj, callable, stack );
				ASSERT( FALSE );
			}
		}

		obj = EC_CLASSSUPER(obj);
	}

	/* No such a method. Invoke doesNotUnderstand if available */
	return EcUnknownMethod( obj, EC_NIL, methodid, stack );

	/* TODO: Should raise an exception */
	ASSERT( FALSE );
	return Ec_ERROR;
}

EC_API EC_OBJ EcGetInstanceVariable( EC_OBJ obj, EC_OBJ at_class, const char *name )
{
	EcInt offs;

	if (EC_NULLP(at_class))
		at_class = EC_OBJECTCLASS(obj);

	offs = search_variable( EC_CLASSNIVARS(at_class), EC_CLASSIVTABLE(at_class), EcInternSymbol( name ) );
/*	ec_fprintf( stderr, "GetIV  obj: %w  at_class: %w  name: `%s'\n", obj, at_class, name );
	ec_fprintf( stderr, "      offs: %d  nivars: %d  ioffs: %d\n", offs, EC_CLASSNIVARS(at_class), EC_CLASSIOFFSET(at_class) );*/
	if (offs < 0)
		return Ec_ERROR;
/*	fprintf( stderr, "GetIV: `%s'  offs: %d   coffs: %d\n", name, offs, EC_CLASSIOFFSET(at_class) );*/
	return EC_OBJECTIVARS(obj)[EC_CLASSIOFFSET(at_class) + offs];
}

EC_API EC_OBJ EcSetInstanceVariable( EC_OBJ obj, EC_OBJ at_class, const char *name, EC_OBJ value )
{
	EcInt offs;

	if (EC_NULLP(at_class))
		at_class = EC_OBJECTCLASS(obj);

	offs = search_variable( EC_CLASSNIVARS(at_class), EC_CLASSIVTABLE(at_class), EcInternSymbol( name ) );
/*	ec_fprintf( stderr, "SetIV  obj: %w  at_class: %w  name: `%s'\n", obj, at_class, name );
	ec_fprintf( stderr, "      offs: %d  nivars: %d  ioffs: %d\n", offs, EC_CLASSNIVARS(at_class), EC_CLASSIOFFSET(at_class) );*/
	if (offs < 0)
		return Ec_ERROR;
/*	fprintf( stderr, "SetIV: `%s'  offs: %d   coffs: %d\n", name, offs, EC_CLASSIOFFSET(at_class) );*/
	return (EC_OBJECTIVARS(obj)[EC_CLASSIOFFSET(at_class) + offs] = value);
}

EC_API EC_OBJ EcGetClassVariable( EC_OBJ classobj, const char *name )
{
	EcInt offs;

	offs = search_variable( EC_CLASSNCVARS(classobj), EC_CLASSCVTABLE(classobj), EcInternSymbol( name ) );
	if (offs < 0)
		return Ec_ERROR;
	return EcArrayGet( EC_CLASSLFRAME(classobj), offs );
}

EC_API EC_OBJ EcSetClassVariable( EC_OBJ classobj, const char *name, EC_OBJ value )
{
	EcInt offs;

	offs = search_variable( EC_CLASSNCVARS(classobj), EC_CLASSCVTABLE(classobj), EcInternSymbol( name ) );
	if (offs < 0)
		return Ec_ERROR;
	EcArraySet( EC_CLASSLFRAME(classobj), offs, value );
	return value;
}

EC_API EcBool EcIsOfClass( EC_OBJ obj, EC_OBJ classobj )
{
	EC_OBJ objclass;

	ASSERT( EC_CLASSP(classobj) );

	if (! EC_OBJECTP(obj)) return FALSE;

/*	ec_fprintf( stderr, "obj  : %w\n", obj );
	ec_fprintf( stderr, "class: %w\n", classobj );*/

	objclass = EC_OBJECTCLASS(obj);
	while (EC_NNULLP(objclass))
	{
/*		ec_fprintf( stderr, "objclass: %w\n", objclass );*/
		if (objclass == classobj)
			return TRUE;

		objclass = EC_CLASSSUPER(objclass);
	}

	return FALSE;
}

static inline EcInt search_method( EcInt nmethods, EcMethodEntry *mtable, EcUInt methodid )
{
	EcInt i, j, k, cmp;

	/*ec_fprintf( stderr, "search_method  methodid: %ld %k\n", (long)methodid, methodid );*/

	/* Binary Search */
	i = 0;
	j = nmethods - 1;
	while (i <= j)
	{
		k = (i + j) / 2;
		cmp = mtable[k].symid - methodid;
		/*fprintf( stderr,
				 "i: %ld   j: %ld   k: %ld   sym[k]: %ld   method: %ld   cmp: %ld\n",
				 (long)i, (long)j, (long)k, (long)mtable[k].symid, (long)methodid, (long)cmp );*/
		if (cmp < 0)
			i = k + 1;
		else if (cmp > 0)
			j = k - 1;
		else
		{
			/*fprintf( stderr, "FOUND   k: %ld\n", (long)k );*/
			/* Found ! */
			ASSERT( k >= 0 );
			ASSERT( k < nmethods );
			ASSERT( EC_CMETHODP(mtable[k].impl) || EC_COMPILEDP(mtable[k].impl) );
			return k;
		}

		if ((j < 0) || (i >= nmethods))
			break;
	}

	return -1;
}

static inline EcInt search_variable( EcInt nvars, EcVariableEntry *vtable, EcUInt symid )
{
	EcInt i;

	for (i = 0; i < nvars; i++)
	{
		if (vtable[i].symid == symid)
			return vtable[i].offset;
	}

	return -1;
}

static EcBool does_object_understand( EC_OBJ obj, EcUInt methodid )
{
	EC_OBJ classobj;

	ASSERT( EC_OBJECTP(obj) );

	classobj = EC_OBJECTCLASS(obj);

	while (EC_NNULLP(classobj))
	{
		if (search_method( EC_CLASSNMETHODS(classobj), EC_CLASSMTABLE(classobj), methodid ) >= 0)
			return TRUE;

		classobj = EC_CLASSSUPER(classobj);
	}

	return FALSE;
}

static EcBool does_class_understand( EC_OBJ classobj, EcUInt methodid )
{
	ASSERT( EC_CLASSP(classobj) );

	while (EC_NNULLP(classobj))
	{
		if (search_method( EC_CLASSNCMETHODS(classobj), EC_CLASSCMTABLE(classobj), methodid ) >= 0)
			return TRUE;

		classobj = EC_CLASSSUPER(classobj);
	}

	return FALSE;
}
