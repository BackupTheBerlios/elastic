/* ==========================================================================
 * debug.c
 * ==========================================================================
 * Debug facilities.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Tue Jan 19 09:18:54 MET 1999
 *
 *   $Id: debug.c,v 1.4 2005/03/22 10:09:34 panta Exp $
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

#include <stdlib.h>
#include <errno.h>

#include "basic.h"
#include "elastic.h"
#include "private.h"
#include "debug.h"

/* Debugging */

#define BAILOUT	do { abort(); } while (0)

EC_API EcBool EcIsObject( void *pointer )
{
	struct heap_chunk *chunk;
	EC_OBJ             chunkStart, chunkEnd;
	EC_OBJ             obj;

	obj = pointer;

	if (EC_NULLP(obj))
		return TRUE;

	/* ------------------------------------------------------------------------
	 * Check pointer validity
	 * ------------------------------------------------------------------------ */
	chunk = PRIVATE(heap).first;
	while (chunk)
	{
		chunkStart = (EC_OBJ)(((char *)chunk) + sizeof(struct heap_chunk));
		chunkEnd   = chunkStart + chunk->size;					/* chunk->size is in number of objects */
		if ((obj >= chunkStart) && (obj < chunkEnd))
			return TRUE;

		chunk = chunk->next;
	}
	return FALSE;
}

EC_API EcBool EcCheck( EC_OBJ obj )
{
	struct heap_chunk *chunk;
	EC_OBJ             chunkStart, chunkEnd;

	if (EC_NULLP(obj))
		return TRUE;

#if EC_INLINED_TYPES
	if (EC_INLINED(obj)) return TRUE;
#endif

	/* ------------------------------------------------------------------------
	 * Check pointer validity
	 * ------------------------------------------------------------------------ */
	chunk = PRIVATE(heap).first;
	while (chunk)
	{
		chunkStart = (EC_OBJ)(((char *)chunk) + sizeof(struct heap_chunk));
		chunkEnd   = chunkStart + chunk->size;					/* chunk->size is in number of objects */
		if ((obj >= chunkStart) && (obj < chunkEnd))
			goto pointerOk;

		chunk = chunk->next;
	}
	/* BAD POINTER ! */
	EcAlert( EcWarning, "BAD OBJECT POINTER (OBJECT 0x%08lX)", (unsigned long)obj );
	BAILOUT;

pointerOk:
	/* ------------------------------------------------------------------------
	 * Check for deleted object
	 * ------------------------------------------------------------------------ */
	if (EC_TYPE(obj) == tc_none) return TRUE;
/*	if (EC_MARKVALUE(obj) == PRIVATE(mark))
		return TRUE;*/

	/* ------------------------------------------------------------------------
	 * Check type coherency
	 * ------------------------------------------------------------------------ */

#define ASSERTMEM(field)                                                       \
do {                                                                           \
	if (! (field)) {                                                           \
		PRINTFUNC;                                                             \
		EcAlert( EcWarning,                                                    \
				 "INVALID FIELD %s (NO MEMORY) FOR TYPE %s (OBJECT 0x%08lX)",  \
				 #field, EcObjectTypeName( obj ), (unsigned long)obj );        \
		goto onError;                                                          \
	}                                                                          \
} while (0)
#define ASSERTFIELD(field,type)                                                           \
do {                                                                                      \
	if (! EC_TYPEP((field), (type))) {                                                    \
		PRINTFUNC;                                                                        \
		EcAlert( EcWarning,                                                               \
				 "INVALID FIELD %s FOR TYPE %s (expected: %s, got: %w) (OBJECT 0x%08lX)", \
				 #field, EcObjectTypeName( obj ),                                         \
				 EcTypeName( type ), obj, (unsigned long)obj );                           \
		goto onError;                                                                     \
	}                                                                                     \
} while (0)
#define ASSERTFIELDN(field,type)                                                          \
do {                                                                                      \
	if (EC_NNULLP((field)) && (! EC_TYPEP((field), (type)))) {                            \
		PRINTFUNC;                                                                        \
		EcAlert( EcWarning,                                                               \
				 "INVALID FIELD %s FOR TYPE %s (expected: %s, got: %w) (OBJECT 0x%08lX)", \
				 #field, EcObjectTypeName( obj ),                                         \
				 EcTypeName( type ), obj, (unsigned long)obj );                           \
		goto onError;                                                                     \
	}                                                                                     \
} while (0)
#define ASSERTCOND(cond)                                                   \
do {                                                                       \
	if (! (cond)) {                                                        \
		PRINTFUNC;                                                         \
		EcAlert( EcWarning,                                                \
				 "CONDITION %s NOT VERIFIED FOR TYPE %s (OBJECT 0x%08lX)", \
				 #cond, EcObjectTypeName( obj ), (unsigned long)obj );     \
		goto onError;                                                      \
	}                                                                      \
} while (0)

	switch (EC_TYPE(obj))
	{
	case tc_none:
/*		EcAlert( EcWarning, "INVALID TYPE %s (OBJECT 0x%08lX)", EcObjectTypeName( obj ), (unsigned long)obj );
		goto onError;*/
		break;

	case tc_undefined:
		break;

	case tc_error:
		break;

	case tc_bool:
/*		if ((EC_BOOLVAL(obj) != FALSE) || (EC_BOOLVAL(obj) != TRUE))
		{
			EcAlert( EcWarning, "INVALID VALUE %ld FOR %s (OBJECT 0x%08lX)",
					 (long)EC_BOOLVAL(obj), EcObjectTypeName( obj ), (unsigned long)obj );
			goto onError;
		}*/
		break;

	case tc_inum:
	case tc_fnum:
		break;

	case tc_symbol:
		if ((EC_SYMBOL(obj) <= 0) || (EC_SYMBOL(obj) > PRIVATE(currentId)))
		{
			EcAlert( EcWarning, "INVALID SYMBOL VALUE %ld FOR %s (OBJECT 0x%08lX)",
					 (long)EC_SYMBOL(obj), EcObjectTypeName( obj ), (unsigned long)obj );
			goto onError;
		}
		break;

	case tc_primitive:
		ASSERTCOND( ec_pointer_registered( (EcCPointer) EC_PRIMITIVE(obj) ) );
		break;

	case tc_cmethod:
		ASSERTCOND( ec_pointer_registered( (EcCPointer) EC_CMETHOD(obj) ) );
		break;

	default:
		if (EC_STRINGP(obj))
		{
			if ((! EC_STRDATA(obj)) && (EC_STRLEN(obj) > 0))
			{
				EcAlert( EcWarning, "INVALID STRING: NO MEMORY (OBJECT 0x%08lX)",
						 (unsigned long)obj );
				goto onError;
			}

			break;
		}

		ASSERTCOND( EC_TYPE(obj) <= PRIVATE(usertypes) );

		if (EC_USERP(obj))
		{
			if (USERTYPE(EC_TYPE(obj)).check_fcn)
				USERTYPE(EC_TYPE(obj)).check_fcn( obj );
		}
		break;
	}

	return TRUE;

onError:
	EcDump( obj );
	BAILOUT;
}

EC_API EcBool EcCheckAll( void )
{
	struct heap_chunk *chunk;
	EC_OBJ             chunkStart, chunkEnd;
	EC_OBJ             obj;

	/* Walk the heap */
	chunk = PRIVATE(heap).first;
	while (chunk)
	{
		chunkStart = (EC_OBJ)(((char *)chunk) + sizeof(struct heap_chunk));
		chunkEnd   = chunkStart + chunk->size;					/* chunk->size is in number of objects */
		for (obj = chunkStart; obj < chunkEnd; obj++)
			if (! EcCheck( obj ))
				BAILOUT;

		chunk = chunk->next;
	}
	return TRUE;
}

EC_API void EcDump( EC_OBJ obj )
{
#if defined(WITH_STDIO)
	ec_stderr_printf( "OBJECT DUMP: (obj: 0x%08lX)\n%r\n\n", (unsigned long)obj, obj );
#endif /* end of defined(WITH_STDIO) */
}

EC_API void EcDumpCompiled( EC_OBJ compiled, EcInt at )
{
#if defined(WITH_STDIO)
	EcInt       i, j;
	EcBytecode  bc, op[2];
	const char *name;
	int         npar;
	char       *compname;
	const char *docstring = NULL;


	ASSERT( EC_COMPILEDP(compiled) );

	if (EC_STRINGP(EC_COMPILEDNAME(compiled)))
		compname = EC_STRDATA(EC_COMPILEDNAME(compiled));
	else
		compname = "$ANONYMOUS$";

	if (EC_STRINGP(EC_COMPILEDDOCSTRING(compiled)))
		docstring = EC_STRDATA(EC_COMPILEDDOCSTRING(compiled));
	else
		docstring = "";

	ec_stderr_printf( "== Compiled   %-20s  0x%08lX ========\n", compname, (unsigned long)compiled );
	ec_stdout_printf( "  \"%s\"\n", docstring );
	ec_stderr_printf( "Bytecode len.        : %ld\n", (long)EC_COMPILEDNCODE(compiled) );
	ec_stderr_printf( "# req. arguments     : %ld\n", (long)EC_COMPILEDNARG(compiled) );
	ec_stderr_printf( "  of which, defaulted: %ld\n", (long)EC_COMPILEDNARG_DEF(compiled) );
	ec_stderr_printf( "         varargs     : %s\n",  EC_COMPILEDVARG(compiled) ? "yes" : "no" );
	ec_stderr_printf( "# locals             : %ld\n", (long)EC_COMPILEDNLOC(compiled) );
	ec_stderr_printf( "# max. temps         : %ld\n", (long)EC_COMPILEDMAXTEMPS(compiled) );
	ec_stderr_printf( "\n" );
	ec_stderr_printf( "-- Literal Frame --(0x%08lX)------------------------\n", (unsigned long) EC_COMPILEDLFRAME(compiled) );
	for (i = 0; i < EC_ARRAYLEN(EC_COMPILEDLFRAME(compiled)); i++)
	{
		ec_stderr_printf( "%5ld  ", (long)i );
		ec_stderr_printf( "%r\n", EcArrayGet( EC_COMPILEDLFRAME(compiled), i ) );
	}
	ec_stderr_printf( "\n" );
	ec_stderr_printf( "-- Listing --------------------------------------------------------\n" );
	ec_stderr_printf( "   ADDR            BYTECODE           OP1           OP2       STACK\n" );
	ec_stderr_printf( "-------------------------------------------------------------------\n" );
	for (i = 0; i < EC_COMPILEDNCODE(compiled); i++)
	{
		bc = EC_COMPILEDCODE(compiled)[i];
		name = EcBytecodeName( bc );
		npar = EcBytecodeParams( bc );

		if ((at >= 0) && (at == i))
			ec_stderr_printf( " *%5ld  ", (long)i );
		else
			ec_stderr_printf( "  %5ld  ", (long)i );
		ec_stderr_printf( "%18s", name );
		for (j = i+1; j <= i+npar; j++)
		{
			op[j - (i+1)] = EC_COMPILEDCODE(compiled)[j];
			ec_stderr_printf( "  %12ld", (long)EC_COMPILEDCODE(compiled)[j] );
		}
		if (npar < 1)
			ec_stderr_printf( "  %12s", "" );
		if (npar < 2)
			ec_stderr_printf( "  %12s", "" );
		switch (bc)
		{
		case CallOP:
		case InlinedCallOP:
			ec_stderr_printf( "%12d", (int)(EcBytecodeStackgrow( bc ) - op[0]));
			break;
		case CallMethodOP:
			ec_stderr_printf( "%12d", (int)(EcBytecodeStackgrow( bc ) - op[1]));
			break;
		default:
			ec_stderr_printf( "%12d", (int)EcBytecodeStackgrow( bc ) );
			break;
		}
		i += npar;
		ec_stderr_printf( "\n" );
	}
#endif /* end of defined(WITH_STDIO) */
}
