/* ==========================================================================
 * elastic.c
 * ==========================================================================
 * elastiC language.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Mon May 25 19:15:36 MEST 1998
 *
 *   $Id: elastic.c,v 1.5 2002/06/07 10:17:48 panta Exp $
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "elastic.h"
#include "ast.h"
#include "bitstring.h"
#include "private.h"

#include "stackrecycle.h"

#define GC_FINAL_PASSES 2

extern int                     yyparse( void );
extern struct yy_buffer_state *yy_scan_string( const char *yy_str );
extern struct yy_buffer_state *yy_create_buffer( FILE *file, int size );
extern void                    yy_switch_to_buffer( struct yy_buffer_state *new_buffer );
extern void                    yy_delete_buffer( struct yy_buffer_state *b );

/* Forever-living objects */
EC_API EC_OBJ EcFalseObject = EC_NIL;
EC_API EC_OBJ EcTrueObject = EC_NIL;
EC_API EC_OBJ Ec_ERROR = EC_NIL;
EC_API EC_OBJ EcUndefinedObject = EC_NIL;
static EC_OBJ EcMemoryErrorObject = EC_NIL;

EC_API EC_OBJ EcObjectClass                = EC_NIL;
EC_API EC_OBJ EcExceptionClass             = EC_NIL;
EC_API EC_OBJ EcUnimplementedErrorClass    = EC_NIL;
EC_API EC_OBJ EcMathErrorClass             = EC_NIL;
EC_API EC_OBJ EcIOErrorClass               = EC_NIL;
EC_API EC_OBJ EcMemoryErrorClass           = EC_NIL;
EC_API EC_OBJ EcUnknownMethodErrorClass    = EC_NIL;
EC_API EC_OBJ EcUnknownOperationErrorClass = EC_NIL;
EC_API EC_OBJ EcIndexErrorClass            = EC_NIL;
EC_API EC_OBJ EcTypeErrorClass             = EC_NIL;
EC_API EC_OBJ EcTargetErrorClass           = EC_NIL;
EC_API EC_OBJ EcParameterCountErrorClass   = EC_NIL;
EC_API EC_OBJ EcCompileErrorClass          = EC_NIL;


EcPrivate _ec_private;

/* ========================================================================
 * P R I V A T E
 * ======================================================================== */

/*@unused@*/ static const EcUInt  _ec_v_number EC_UNUSED = EC_VERSION;
/*@unused@*/ static const char   *_ec_v_name   EC_UNUSED = EC_VERSION_STRING;

/* Symbol table stuff */

#define STRTABLESLOTS 8192

/* ========================================================================
 * P U B L I C
 * ======================================================================== */

/* Library Initialization/Cleanup */

EC_API EcBool EcInit( void )
{
#if MEM_STATS
	EcInt i;
#endif
	void _ec_initializeOT( void );

	if (! EcDLInit())
	{
		fprintf( stderr, "DL error: %s\n", EcDLError() );
		return FALSE;
	}

	PRIVATE(in_cleanup) = FALSE;

#if 0
	PRIVATE(argc) = 0;
	PRIVATE(argv) = NULL;
#endif

#ifdef EC_DEBUG
	PRIVATE(debug_enable) = TRUE;
#else
	PRIVATE(debug_enable) = FALSE;
#endif

	PRIVATE(sourceOrigin) = NULL;
	PRIVATE(fileSource)   = NULL;
	PRIVATE(fileOutput)   = NULL;
	PRIVATE(startLine)    = 0;
	PRIVATE(startColumn)  = 0;
	PRIVATE(endLine)      = 0;
	PRIVATE(endColumn)    = 0;

	PRIVATE(parse_result) = NULL;

	PRIVATE(symTable)     = ec_strtable_create( STRTABLESLOTS );
	if (! PRIVATE(symTable))
		return FALSE;
	PRIVATE(currentId)    = 0;
	PRIVATE(symName)      = NULL;
	PRIVATE(symNameSize)  = 0;

	PRIVATE(nodePool)     = NULL;

	PRIVATE(globalScope)  = NULL;
	PRIVATE(currentScope) = NULL;

	PRIVATE(globalFrame)     = EC_NIL;
	PRIVATE(rt).activeFrame  = EC_NIL;
	PRIVATE(rt).exc          = EC_NIL;
	PRIVATE(rt).vm_level     = -1;
	PRIVATE(rt).line_num     = 0;
	PRIVATE(rt).exc_line_num = -1;

#if MEM_STATS
	for (i = 0; i < EC_MAX_TYPES; i++)
	{
		_ec_mark_stats[i] = 0;
		_ec_make_stats[i] = 0;
	}
#endif

#if 0
	ASSERT( argc >= 0 );
	if (argc > 0)
	{
		EcInt i;

		/* Duplicate args */
		PRIVATE(argv) = (char **) ec_malloc( argc * sizeof(char *) );
		if (! argv)
			return FALSE;
		memset( PRIVATE(argv), 0x00, argc * sizeof(char *) );
		PRIVATE(argc) = argc;

		for (i = 0; i < argc; i++)
		{
			PRIVATE(argv)[i] = ec_stringdup( argv[i] );
			if (! PRIVATE(argv)[i])
				return FALSE;
		}
	}
#endif

	/* Register symbols */
	PRIVATE(selfID)              = EcInternSymbol( "self" );
	PRIVATE(at_classID)          = EcInternSymbol( "at_class" );
	PRIVATE(initID)              = EcInternSymbol( "init" );
	PRIVATE(newID)               = EcInternSymbol( "new" );
	PRIVATE(copyID)              = EcInternSymbol( "_copy" );
	PRIVATE(compareID)           = EcInternSymbol( "_compare" );
	PRIVATE(hashID)              = EcInternSymbol( "_hash" );
	PRIVATE(getitemID)           = EcInternSymbol( "_getitem" );
	PRIVATE(setitemID)           = EcInternSymbol( "_setitem" );
	PRIVATE(delitemID)           = EcInternSymbol( "_delitem" );
	PRIVATE(lengthID)            = EcInternSymbol( "_length" );
	PRIVATE(gcmarkID)            = EcInternSymbol( "_gcmark" );
	PRIVATE(gcfreeID)            = EcInternSymbol( "_gcfree" );
	PRIVATE(printID)             = EcInternSymbol( "_print" );
	PRIVATE(doesNotUnderstandID) = EcInternSymbol( "doesNotUnderstand" );
	PRIVATE(promoteID)           = EcInternSymbol( "_promote" );
	PRIVATE(addID)               = EcInternSymbol( "_add" );
	PRIVATE(raddID)              = EcInternSymbol( "_radd" );
	PRIVATE(subID)               = EcInternSymbol( "_sub" );
	PRIVATE(rsubID)              = EcInternSymbol( "_rsub" );
	PRIVATE(mulID)               = EcInternSymbol( "_mul" );
	PRIVATE(rmulID)              = EcInternSymbol( "_rmul" );
	PRIVATE(divID)               = EcInternSymbol( "_div" );
	PRIVATE(rdivID)              = EcInternSymbol( "_rdiv" );
	PRIVATE(modID)               = EcInternSymbol( "_mod" );
	PRIVATE(rmodID)              = EcInternSymbol( "_rmod" );
	PRIVATE(powID)               = EcInternSymbol( "_pow" );
	PRIVATE(rpowID)              = EcInternSymbol( "_rpow" );
	PRIVATE(lshiftID)            = EcInternSymbol( "_lshift" );
	PRIVATE(rlshiftID)           = EcInternSymbol( "_rlshift" );
	PRIVATE(rshiftID)            = EcInternSymbol( "_rshift" );
	PRIVATE(rrshiftID)           = EcInternSymbol( "_rrshift" );
	PRIVATE(andID)               = EcInternSymbol( "_and" );
	PRIVATE(randID)              = EcInternSymbol( "_rand" );
	PRIVATE(orID)                = EcInternSymbol( "_or" );
	PRIVATE(rorID)               = EcInternSymbol( "_ror" );
	PRIVATE(xorID)               = EcInternSymbol( "_xor" );
	PRIVATE(rxorID)              = EcInternSymbol( "_rxor" );
	PRIVATE(negID)               = EcInternSymbol( "_neg" );
	PRIVATE(posID)               = EcInternSymbol( "_pos" );
	PRIVATE(absID)               = EcInternSymbol( "_abs" );
	PRIVATE(invertID)            = EcInternSymbol( "_invert" );
	PRIVATE(intID)               = EcInternSymbol( "_int" );
	PRIVATE(floatID)             = EcInternSymbol( "_float" );
	PRIVATE(trueID)              = EcInternSymbol( "_true" );
	PRIVATE(cmpID)               = EcInternSymbol( "_cmp" );
	PRIVATE(rcmpID)              = EcInternSymbol( "_rcmp" );

	PRIVATE(lineID)              = EcInternSymbol( "line" );

	_ec_initializeOT();

	PRIVATE(global)       = NULL;
	PRIVATE(nglobals)     = 0;
	PRIVATE(ncoreglobals) = 0;

#if EC_STACK_RECYCLE
	{
		EcInt j;

		for (j = 0; j < EC_STACK_POOL_SIZE; j++)
			PRIVATE(stack_pool)[j] = EC_NIL;
		ec_bit_nclear(PRIVATE(stack_pool_bmap), 0, EC_STACK_POOL_SIZE-1);

#if EC_STACK_RECYCLE_STATS
		PRIVATE(n_makestack)            = 0;
		PRIVATE(n_recycle_put_attempts) = 0;
		PRIVATE(n_recycle_put)          = 0;
		PRIVATE(n_recycle_get_attempts) = 0;
		PRIVATE(n_recycle_get)          = 0;
		PRIVATE(n_pool_empty)           = 0;
		PRIVATE(n_pool_full)            = 0;
		PRIVATE(pool_max_fill)          = 0;
		PRIVATE(n_ref_inc)              = 0;
		PRIVATE(n_ref_dec)              = 0;
#endif
	}
#endif

	EcCheckForGC();

	PRIVATE(usertypes) = tc_userbase - 1;
	PRIVATE(typespec)  = NULL;

	PRIVATE(package)   = NULL;
	PRIVATE(npackages) = 0;

	PRIVATE(patchmap)  = NULL;

	PRIVATE(ncpointers) = 0;
	PRIVATE(cpointer)   = NULL;

	PRIVATE(builtin_name) = ec_hash_create( 0, 0, 0, &ec_hash_obj2string );

	/* Built-ins */

	if (EC_NULLP(EcFalseObject))
	{
		EcFalseObject = EcMakeBool( FALSE );
		EcTrueObject  = EcMakeBool( TRUE );

		Ec_ERROR = EcAllocObject();
		EC_OBJTYPE(Ec_ERROR) = tc_error;

		EcUndefinedObject = EcMakeUndefined();
	}

	/* Register built-in packages, types, primitives & classes */
	if (! _ec_register_builtin())
		return FALSE;

	/* Preallocate a MemoryError object */
	if (EC_NULLP(EcMemoryErrorObject))
	{
		EcMemoryError();										/* create one ...            */
		PRIVATE(rt).exc = EC_NIL;								/* ... but don't assert it ! */
	}

	/* Save global forever-living objects from GC */
	EcGCProtect( EcFalseObject );
	EcGCProtect( EcTrueObject );
	EcGCProtect( Ec_ERROR );
	EcGCProtect( EcUndefinedObject );
	EcGCProtect( EcMemoryErrorObject );

	PRIVATE(ncoreglobals) = PRIVATE(nglobals);

	_ec_packageio_init();

	return TRUE;
}

EC_API void EcCleanup( void )
{
	void _ec_releaseOT( void );
	EcInt i;

	PRIVATE(in_cleanup) = TRUE;

#if MEM_STATS
	fprintf( stderr, "\nTYPE                   ALLOCATED    MARKINGS\n" );
	fprintf( stderr, "============================================\n" );
	for (i = 0; i <= PRIVATE(usertypes); i++)
		fprintf( stderr, "%-20s    %8ld    %8ld\n", EcTypeName( i ), _ec_make_stats[i], _ec_mark_stats[i] );
#endif

	_ec_packageio_cleanup();

	if (PRIVATE(builtin_name))
		ec_hash_destroy( PRIVATE(builtin_name) );
	PRIVATE(builtin_name) = NULL;

	ec_free( PRIVATE(cpointer) );
	PRIVATE(ncpointers) = 0;
	PRIVATE(cpointer)   = NULL;

	EcGCUnprotectAll();

	PRIVATE(patchmap) = NULL;

	/* BREAK THE LINKS between toplevel and objects ! */
	PRIVATE(rt.activeFrame) = EC_NIL;
	PRIVATE(globalFrame)    = EC_NIL;
	PRIVATE(mainTarget)     = EC_NIL;
	for (i = 0; i < PRIVATE(nglobals); i++)
		PRIVATE(global[i]) = EC_NIL;
	for (i = 0; i < PRIVATE(npackages); i++)
		PRIVATE(package[i].obj) = EC_NIL;
#if EC_STACK_RECYCLE
	{
		EcInt j;

#if EC_STACK_RECYCLE_STATS
		fprintf( stderr, "\n== Stack statistics =================\n" );
		fprintf( stderr, "Calls to EcMakeStack()   : %ld\n", (long)PRIVATE(n_makestack) );
		fprintf( stderr, "# stores in recycle bin  : %ld\n", (long)PRIVATE(n_recycle_put) );
		fprintf( stderr, "# failed stores          : %ld\n", (long)(PRIVATE(n_recycle_put_attempts) - PRIVATE(n_recycle_put)) );
		fprintf( stderr, "# fetched from rec. bin  : %ld\n", (long)PRIVATE(n_recycle_get) );
		fprintf( stderr, "# failed fetches         : %ld\n", (long)(PRIVATE(n_recycle_get_attempts) - PRIVATE(n_recycle_get)) );
		fprintf( stderr, "# times the bin was empty: %ld\n", (long)PRIVATE(n_pool_empty) );
		fprintf( stderr, "# times the bin was full : %ld\n", (long)PRIVATE(n_pool_full) );
		/* fprintf( stderr, "bin max fill             : %ld\n", (long)PRIVATE(pool_max_fill) ); */
		fprintf( stderr, "# refcount increments    : %ld\n", (long)PRIVATE(n_ref_inc) );
		fprintf( stderr, "# refcount decrements    : %ld\n", (long)PRIVATE(n_ref_dec) );
		fprintf( stderr, "\n" );
#endif

		for (j = 0; j < EC_STACK_POOL_SIZE; j++)
			PRIVATE(stack_pool)[j] = EC_NIL;
		ec_bit_nclear(PRIVATE(stack_pool_bmap), 0, EC_STACK_POOL_SIZE-1);
	}
#endif
	for (i = 0; i < GC_FINAL_PASSES; i++) EcGC();

	/* Release all objects in a linear fashion */
	_ec_releaseOT();

	for (i = 0; i < PRIVATE(npackages); i++)
	{
		if (PRIVATE(package)[i].dlhandle)
		{
			if (PRIVATE(package)[i].dlcleanup_fcn)
				(*(PRIVATE(package)[i].dlcleanup_fcn))();
			EcDLClose( PRIVATE(package)[i].dlhandle );
		}
		ec_string_destroy( PRIVATE(package)[i].name );
		PRIVATE(package)[i].name          = NULL;
		PRIVATE(package)[i].dlhandle      = NULL;
		PRIVATE(package)[i].dlinit_fcn    = NULL;
		PRIVATE(package)[i].dlcleanup_fcn = NULL;
	}
	ec_free( PRIVATE(package) );
	PRIVATE(package)   = NULL;
	PRIVATE(npackages) = 0;

	for (i = tc_userbase; i <= PRIVATE(usertypes); i++)
	{
		ec_free( USERTYPE(i).name );
		USERTYPE(i).name = NULL;

		ec_free( USERTYPE(i).sequence_cb );
		ec_free( USERTYPE(i).numeric_cb );
	}
	ec_free( PRIVATE(typespec) );
	PRIVATE(typespec)  = NULL;
	PRIVATE(usertypes) = 0;

	EcTrueObject        = EC_NIL;
	EcFalseObject       = EC_NIL;
	Ec_ERROR            = EC_NIL;
	EcUndefinedObject   = EC_NIL;
	EcMemoryErrorObject = EC_NIL;

	PRIVATE(rt).exc         = EC_NIL;
	PRIVATE(rt).activeFrame = EC_NIL;
	PRIVATE(globalFrame)    = EC_NIL;
	PRIVATE(rt).vm_level    = -1;

	/* CloseScope( PRIVATE(currentScope) ); */
	PRIVATE(currentScope) = NULL;
	PRIVATE(globalScope)  = NULL;

	ec_freepool( PRIVATE(nodePool) );
	PRIVATE(nodePool) = NULL;

	if (PRIVATE(symName))
	{
		EcUInt i;

		for (i = 1; i <= PRIVATE(currentId); i++)
		{
			ec_free( PRIVATE(symName)[i] );
			PRIVATE(symName)[i] = NULL;
		}

		ec_freeblock( PRIVATE(symName) );
		PRIVATE(symName) = NULL;
		PRIVATE(symNameSize) = 0;
	}
	if (PRIVATE(symTable)) ec_strtable_destroy( PRIVATE(symTable) );
	PRIVATE(symTable) = NULL;
	PRIVATE(currentId) = 0;

	ec_free( PRIVATE(fileOutput) );
	ec_free( PRIVATE(fileSource) );
	ec_free( PRIVATE(sourceOrigin) );
	PRIVATE(sourceOrigin) = NULL;
	PRIVATE(fileSource)   = NULL;
	PRIVATE(fileOutput)   = NULL;
	PRIVATE(startLine)    = 0;
	PRIVATE(startColumn)  = 0;
	PRIVATE(endLine)      = 0;
	PRIVATE(endColumn)    = 0;

#if 0
	for (i = 0; i < PRIVATE(argc); i++)
		ec_free( PRIVATE(argv)[i] );
	ec_free( PRIVATE(argv) );
	PRIVATE(argc) = 0;
	PRIVATE(argv) = NULL;
#endif

	_ec_list_cleanup();
	_ec_hash_cleanup();
	_ec_object_cleanup();
	_ec_class_cleanup();
	_ec_package_cleanup();
	_ec_handler_cleanup();
	_ec_compiled_cleanup();
	_ec_stack_cleanup();
	_ec_string_cleanup();
	_ec_char_cleanup();
	_ec_array_cleanup();

	EcDLCleanup();

	PRIVATE(in_cleanup) = FALSE;
}

EC_API EcUInt EcVersionNumber( void )
{
	return EC_VERSION;
}

EC_API const char *EcVersionString( void )
{
	return EC_VERSION_STRING;
}

	/* Always use *after* package loading/saving ! */
EC_API EC_OBJ EcSetArgs( int argc, char *argv[] )
{
	EcInt  i;
	EC_OBJ obj;

	obj = EcMakeArray( argc );
	if (EC_ERRORP(obj))
		return obj;
	for (i = 0; i < argc; i++)
		EcArrayPush( obj, EcMakeString( argv[i], 0 ) );

	EcSetQVariable( "sys.args", obj );

	return obj;
}

#if 0
EcInt EcArgc( void )
{
	return PRIVATE(argc);
}

const char **EcArgv( void )
{
	return (const char **) PRIVATE(argv);
}

EcInt *EcArgcp( void )
{
	return &PRIVATE(argc);
}

const char ***EcArgvp( void )
{
	return (const char ***) &PRIVATE(argv);
}

const char *EcArg( EcInt arg )
{
	if ((arg < 0) || (arg >= PRIVATE(argc)))
	{
        ec_string ds;

        ec_string_init( &ds, NULL );
        ec_sprintf( &ds, "Index %ld out of bounds for program arguments" );
        EcMakeExceptionWithArgs( EcIndexErrorClass, ec_strdata( &ds ), 2,
								 EcMakeInt( arg ), EC_NIL );
        ec_string_cleanup( &ds );

		return NULL;
	}

	return PRIVATE(argv)[arg];
}
#endif

/* Error handling */

EC_API void EcAlert( EcErrorSeverity severity, const char *fmt, ... )
{
	va_list ap;
	ec_string msgfmt;

	va_start( ap, fmt );

	ec_string_init( &msgfmt, NULL );

	switch (severity)
	{
	case EcFatal:
		ec_sprintf( &msgfmt, "FATAL: %s\n", fmt );
		ec_vfprintf( stderr, ec_strdata( &msgfmt), ap );
		va_end( ap );
		exit( EXIT_FAILURE );
		break;

	case EcError:
		ec_sprintf( &msgfmt, "ERROR: %s\n", fmt );
		ec_vfprintf( stderr, ec_strdata( &msgfmt), ap );
		va_end( ap );
		exit( EXIT_FAILURE );
		break;

	case EcWarning:
		ec_sprintf( &msgfmt, "WARNING: %s\n", fmt );
		ec_vfprintf( stderr, ec_strdata( &msgfmt), ap );
		break;
	}

	ec_string_cleanup( &msgfmt );

	va_end( ap );
}

EC_API EC_OBJ EcMakeException( EC_OBJ exc_class, const char *fmt, ... )
{
	va_list   ap;
	EC_OBJ    exc;
	EC_OBJ    stack;
	ec_string ds;

	va_start( ap, fmt );

	ec_string_init( &ds, NULL );
	ec_vsprintf( &ds, fmt, ap );

	stack = EcMakeStack( EC_NIL, /* dim */ 1 );
	EC_STACKNARGS_REAL(stack)    = 1;							/* set real num. of passed args    */
	EC_STACKNARGS_LOGICAL(stack) = 1;							/* set logical num. of passed args */

	EC_STACKLEXICAL(stack) = EC_NIL;
	EC_STACKPUSH( stack, EcMakeString( ec_strdata( &ds ), ec_strlen( &ds ) ) );
	exc = EcLibExceptionClass_NewWith( exc_class, EC_NIL, stack );
	PRIVATE(rt).exc = exc;
#if EC_STACK_RECYCLE
#if EC_STACK_RECYCLE_PARANOID_CHECKS
	ASSERT( EC_STACKREF(stack) == 1 );
#endif
	if (EC_STACKREF(stack) > 0)
		EC_STACKREF_DEC(stack);
#endif

	ec_string_cleanup( &ds );

	va_end( ap );
	return Ec_ERROR;
}

EC_API EC_OBJ EcMakeExceptionWithArgs( EC_OBJ exc_class, const char *msg, EcInt nargs, ... )
{
	va_list ap;
	EC_OBJ exc;
	EC_OBJ stack;
	EcInt i;

	va_start( ap, nargs );

	stack = EcMakeStackForMethod( exc_class, PRIVATE(newID), (nargs + 1) );
	if (EC_ERRORP(stack)) return stack;

	EC_STACKLEXICAL(stack) = EC_NIL;
	EC_STACKSET( stack, 3, EcMakeString( msg, 0 ) );
	for (i = 0; i < nargs; i++)
		EC_STACKSET( stack, i + 4, va_arg( ap, EC_OBJ ) );
	exc = EcSendMessage( exc_class, PRIVATE(newID), stack );
/*	exc = EcLibExceptionNewWith( exc_class, EC_NIL, stack, (nargs + 1) );*/
	PRIVATE(rt).exc = exc;
#if EC_STACK_RECYCLE
#if EC_STACK_RECYCLE_PARANOID_CHECKS
	ASSERT( EC_STACKREF(stack) == 1 );
#endif
	if (EC_STACKREF(stack) > 0)
		EC_STACKREF_DEC(stack);
#endif

	va_end( ap );
	return Ec_ERROR;
}

EC_API EC_OBJ EcUnimplementedError( const char *msg )
{
	return EcMakeExceptionWithArgs( EcUnimplementedErrorClass, msg, 0 );
}

EC_API EC_OBJ EcMathError( const char *msg )
{
	return EcMakeExceptionWithArgs( EcMathErrorClass, msg, 0 );
}

EC_API EC_OBJ EcIOError( EC_OBJ ioObject, EcUInt posixErrorSymbol, const char *msg )
{
	return EcMakeExceptionWithArgs( EcIOErrorClass, msg, 2, ioObject, EcMakeSymbolFromId( posixErrorSymbol ) );
}

EC_API EC_OBJ EcMemoryError( void )
{
	/* Use preallocated object if possible ... */
	if (EC_NNULLP(EcMemoryErrorObject))
	{
		PRIVATE(rt).exc = EcMemoryErrorObject;
		return Ec_ERROR;
	}

	/* ... otherwise allocate one */
	EcMakeExceptionWithArgs( EcMemoryErrorClass, "Out of memory", 0 );
	EcMemoryErrorObject = PRIVATE(rt).exc;
	return Ec_ERROR;
}

EC_API EC_OBJ EcTypeError( EC_OBJ target, EC_OBJ function,
						   EcInt index,
						   EcInt expected, EC_OBJ offending,
						   EcBool print_offending,
						   const char *reason )
{
	ec_string ds;												/* For error messages */

	ec_string_init( &ds, NULL );
	if (print_offending)
	{
		if (reason)
		{
			if (EC_NNULLP(function))
				ec_sprintf( &ds, "Wrong type for parameter %ld to %W: %s (got '%s': %W)",
							index,
							function,
							reason,
							EcObjectTypeName( offending ),
							offending );
			else
				ec_sprintf( &ds, "Wrong type for parameter %ld: %s (got '%s': %W)",
							index,
							reason,
							EcObjectTypeName( offending ),
							offending );
		}
		else if (expected > 0)
		{
			if (EC_NNULLP(function))
				ec_sprintf( &ds, "Wrong type for parameter %ld to %W: expected '%s' (got '%s': %W)",
							index,
							function,
							EcTypeName( expected ),
							EcObjectTypeName( offending ),
							offending );
			else
				ec_sprintf( &ds, "Wrong type for parameter %ld: expected '%s' (got '%s': %W)",
							index,
							EcTypeName( expected ),
							EcObjectTypeName( offending ),
							offending );
		}
		else
		{
			if (EC_NNULLP(function))
				ec_sprintf( &ds, "Wrong type for parameter %ld to %W (got '%s': %W)",
							index,
							function,
							EcObjectTypeName( offending ),
							offending );
			else
				ec_sprintf( &ds, "Wrong type for parameter %ld (got '%s': %W)",
							index,
							EcObjectTypeName( offending ),
							offending );
		}
	}
	else
	{
		if (reason)
		{
			if (EC_NNULLP(function))
				ec_sprintf( &ds, "Wrong type for parameter %ld to %W: %s (got '%s')",
							index,
							function,
							reason,
							EcObjectTypeName( offending ) );
			else
				ec_sprintf( &ds, "Wrong type for parameter %ld: %s (got '%s')",
							index,
							reason,
							EcObjectTypeName( offending ) );
		}
		else if (expected > 0)
		{
			if (EC_NNULLP(function))
				ec_sprintf( &ds, "Wrong type for parameter %ld to %W: expected '%s' (got '%s')",
							index,
							function,
							EcTypeName( expected ),
							EcObjectTypeName( offending ) );
			else
				ec_sprintf( &ds, "Wrong type for parameter %ld: expected '%s' (got '%s')",
							index,
							EcTypeName( expected ),
							EcObjectTypeName( offending ) );
		}
		else
		{
			if (EC_NNULLP(function))
				ec_sprintf( &ds, "Wrong type for parameter %ld to %W (got '%s')",
							index,
							function,
							EcObjectTypeName( offending ) );
			else
				ec_sprintf( &ds, "Wrong type for parameter %ld (got '%s')",
							index,
							EcObjectTypeName( offending ) );
		}
	}

	EcMakeExceptionWithArgs( EcTypeErrorClass,
							 ec_strdata( &ds ),
							 5,
							 target, function,
							 EcMakeInt( index ),
							 (expected > 0) ? EcMakeSymbol( EcTypeName( expected ) ) : EC_NIL,
							 offending );
	ec_string_cleanup( &ds );
	return Ec_ERROR;
}

EC_API EC_OBJ EcTargetError( EC_OBJ target, const char *message )
{
	EcMakeExceptionWithArgs( EcTargetErrorClass, message, 1,
							 target );
	return Ec_ERROR;
}

EC_API EC_OBJ EcUnknownOperationError( const char *opname, EcInt nargs, ... )
{
	va_list ap;
	ec_string ds;
	EC_OBJ operands;
	EcInt i;

	va_start( ap, nargs );

	operands = EcMakeArray( nargs );
	for (i = 0; i < nargs; i++)
		EcArrayPush( operands, va_arg( ap, EC_OBJ ) );

	ec_string_init( &ds, NULL );
	ec_sprintf( &ds, "Unable to evaluate operation '%s' for operands %W", opname, operands );

	EcMakeExceptionWithArgs( EcUnknownOperationErrorClass, ec_strdata( &ds ), 2,
							 operands, EcMakeString( opname, 0 ) );
	ec_string_cleanup( &ds );

	va_end( ap );
	return Ec_ERROR;
}

EC_API EC_OBJ EcIndexError( EC_OBJ object, EC_OBJ index )
{
	ec_string ds;

	ec_string_init( &ds, NULL );
	ec_sprintf( &ds, "Index %W out of bounds for object %W", index, object );
	EcMakeExceptionWithArgs( EcIndexErrorClass, ec_strdata( &ds ), 2,
							 index, object );
	ec_string_cleanup( &ds );
	return Ec_ERROR;
}

EC_API EC_OBJ EcParameterCountError( EC_OBJ target, EC_OBJ function, EcInt expected, EC_OBJ stack )
{
	EC_OBJ arg, parameters;
	ec_string ds;
	EcInt i;
	EcInt nargs;

	/* Copy original args */
	if (EC_STACKP(stack))
	{
		nargs = EC_STACKNARGS_REAL(stack);

		/*
		 * XXX TODO:
		 * Same problem described in EcUnknownMethod() in oop.c
		 */

		/* Build an array filled with the parameters on the stack */
		parameters = EcMakeArray( nargs );

		for (i = 0; i < nargs; i++)
		{
			arg = *(EC_STACKPOINTER(stack) - nargs + i);
			EcArraySet( parameters, i, arg );
		}

		ec_string_init( &ds, NULL );
		ec_sprintf( &ds, "Wrong # of parameters to %W (got %ld, expected %ld)", function, EC_STACKNARGS_LOGICAL(stack), expected );
		EcMakeExceptionWithArgs( EcParameterCountErrorClass, ec_strdata( &ds ), 4,
								 target, function, EcMakeInt( expected ), parameters );
		ec_string_cleanup( &ds );
	} else
	{
		/* We have no stack to refer to ... */

		ec_string_init( &ds, NULL );
		ec_sprintf( &ds, "Wrong # of parameters to %W (expected %ld)", function, expected );
		EcMakeExceptionWithArgs( EcParameterCountErrorClass, ec_strdata( &ds ), 4,
								 target, function, EcMakeInt( expected ), EC_NIL );
		ec_string_cleanup( &ds );
	}

	return Ec_ERROR;
}

EC_API EC_OBJ EcParameterCountErrorFunction( const char *funcname, EcInt expected, EC_OBJ stack )
{
	return EcParameterCountError( EC_NIL,
								  funcname ? EcGetQVariable( funcname ) : EC_NIL,
								  expected, stack );
}

EC_API EC_OBJ EcParameterCountErrorMethod( EC_OBJ target, const char *method, EcInt expected, EC_OBJ stack )
{
	return EcParameterCountError( target, EcFindMethod( target, NULL, EcInternSymbol( method ) ), expected, stack );
}

EC_API EC_OBJ EcParameterCountErrorClassMethod( EC_OBJ objclass, const char *method, EcInt expected, EC_OBJ stack )
{
	return EcParameterCountError( objclass, EcFindClassMethod( objclass, NULL, EcInternSymbol( method ) ), expected, stack );
}

EC_API EC_OBJ EcCompileError( EcInt line, EcInt column, const char *format, ... )
{
	va_list ap;
	ec_string ds, ds2;

	va_start( ap, format );

	ec_string_init( &ds, NULL );
	ec_vsprintf( &ds, format, ap );

	ec_string_init( &ds2, NULL );
	if ((line >= 0) && (column >= 0))
		ec_sprintf( &ds2, "(LINE: %ld, COLUMN: %ld) %s", line, column, ec_strdata( &ds ) );
	else
		ec_sprintf( &ds2, "(LINE: --, COLUMN: --) %s", ec_strdata( &ds ) );
	EcMakeExceptionWithArgs( EcCompileErrorClass, ec_strdata( &ds2 ), 2,
							 line, column );
	ec_string_cleanup( &ds2 );
	ec_string_cleanup( &ds );

	va_end( ap );
	return Ec_ERROR;
}


/* Primitive procedures */

EC_API EcBool EcAddPrimitive( const char *name, EcCPrimitive prim )
{
	return EcAddPrimitiveUserData( name, prim, NULL, FALSE );
}

EC_API EcBool EcAddPrimitiveUserData( const char *name, EcCPrimitive prim, EcAny userdata, EcBool userdata_is_object )
{
	EC_OBJ obj;

	obj = EcMakePrimitive( prim, name, userdata, userdata_is_object );

	/* Add to the proper package (the package must exist) */
	EcPackageVariable( EC_NIL, name, obj, TRUE, FALSE );

	EcRegisterBuiltin( name, obj );

	return TRUE;
}

/* Builtin registration & retrieval */

EC_API void EcRegisterBuiltin( const char *name, EC_OBJ builtin )
{
	ec_hash_set( PRIVATE(builtin_name), builtin, (EcAny)name );
}

EC_API const char *EcBuiltinName( EC_OBJ builtin )
{
	EcAny value;

	if (! ec_hash_get( PRIVATE(builtin_name), builtin, &value ))
		return NULL;

	return (const char *)value;
}

/* Calling */

EC_API EcBool EcIsCallable( EC_OBJ obj )
{
	if (EC_NULLP(obj)) return FALSE;

	switch (EC_TYPE(obj))
	{
	case tc_primitive:
	case tc_cmethod:
		return TRUE;
		break;

	default:
		if (EC_COMPILEDP(obj))
			return TRUE;

		return FALSE;
		break;
	}

	return FALSE;
}

EC_API EC_OBJ EcCall( EC_OBJ callerstack, EC_OBJ self, EC_OBJ at_class, EC_OBJ obj, EcInt nargs, ... )
{
	va_list ap;
	EC_OBJ  res;

	va_start( ap, nargs );
	res = EcCallVA( callerstack, self, at_class, obj, nargs, ap );
	va_end( ap );
	return res;
}

EC_API EC_OBJ EcCallVA( EC_OBJ callerstack, EC_OBJ self, EC_OBJ at_class, EC_OBJ obj, EcInt nargs, va_list ap )
{
	EcInt  i;
	EC_OBJ *args;

	args = alloca( nargs * sizeof(EC_OBJ) );
	if (! args) return EcMemoryError();

	for (i = 0; i < nargs; i++)
		args[i] = va_arg( ap, EC_OBJ );

	return EcCallArgs( callerstack, self, at_class, obj, nargs, args );
}

EC_API EC_OBJ EcCallArgs( EC_OBJ callerstack, EC_OBJ self, EC_OBJ at_class, EC_OBJ obj, EcInt nargs, EC_OBJ *args )
{
	EC_OBJ stack;
	EcInt  i, offs;
	EcBool ismethod;

	EcInt nargs_real, vargs, nargs_req;

	/* TODO: Throw an exception */
	if (EC_NULLP(obj)) return Ec_ERROR;

	ismethod = FALSE;

	/* Make stack */

	switch (EC_TYPE(obj))
	{
	case tc_primitive:
		nargs_real = nargs;
		nargs_req  = nargs;
		vargs      = 0;

		stack = callerstack;
		break;

	case tc_cmethod:
		nargs_real = nargs;
		nargs_req  = nargs;
		vargs      = 0;

		/* TODO: try to use a stack from the pool ???? */
		stack = EcMakeStackForMethod2( self, obj, nargs );
		if (EC_ERRORP(stack)) return stack;
#if 0
		EC_STACKSET( stack, 0, EC_NIL );						/* caller   */
		EC_STACKSET( stack, 1, self );							/* self     */
		EC_STACKSET( stack, 2, at_class );						/* at_class */
#endif
		break;

	default:
		if (EC_COMPILEDP(obj))
		{
			EcInt n_req;

			ismethod = EC_COMPILEDISMETHOD(obj);
			n_req    = EC_COMPILEDNARG(obj) - (ismethod ? 2 : 0);

			vargs      = nargs - n_req;
			nargs_req  = n_req;
			nargs_real = nargs_req + ((vargs > 0) ? 1 : 0);

			/* TODO: try to use a stack from the pool ???? */
			stack = EcMakeStackForCompiled( obj, nargs );
			if (EC_ERRORP(stack)) return stack;
			break;
		}

		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	/* Put args */

	/* Added. Closes bug (1) 01-12-1999 */
	EcStackSet( stack, 0, EC_NIL );								/* caller   */
	EcStackSet( stack, 1, self );								/* receiver */
	EcStackSet( stack, 2, at_class );							/* at_class */

	/* Put args in reverse order */
	/* stack = callerstack;     XXXX ????????????? */
	/* offs = EC_STACKPOINTER(stack) - EC_STACKBASE(stack); */ /* ???? */
	offs = 3;

	/* printf( "offs: %ld\n", offs ); */

	/*
	 * :TODO: XXX Check for correct number of arguments !
	 * :TODO: XXX Handle parameters with default values
	 */

	/* Put required args */
	for (i = 0; i < nargs_req; i++)
	{
		EcStackSet( stack, i + offs, *args++ );
	}

	/* Put varargs */
	if (vargs)
	{
		EC_OBJ vargs_obj;

		vargs_obj = EcMakeArray( vargs );

		for (i = 0; i < vargs; i++)
			EcArraySet( vargs_obj, i, *args++ );

		EC_STACKSET( stack, nargs_req + offs, vargs_obj );
	}

	/* Call */
	switch (EC_TYPE(obj))
	{
	case tc_primitive:
		return EC_PRIMITIVE(obj)( stack, EC_PRIMITIVEUSERDATA(obj) );
		break;

	case tc_cmethod:
	{
		EC_OBJ res;
		res = EC_CMETHOD(obj)( self, at_class, stack );
#if EC_STACK_RECYCLE
#if EC_STACK_RECYCLE_PARANOID_CHECKS
		ASSERT( EC_STACKREF(stack) == 1 );
#endif
		if (EC_STACKREF(stack) > 0)
			EC_STACKREF_DEC(stack);
#endif
		return res;
	}
	break;

	default:
		if (EC_COMPILEDP(obj))
		{
			return EcExecute( self, at_class, obj, stack );
			break;
		}

		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}
}


/* Compilation & Execution */

/* C pointers */
EcInt ec_register_cpointer( EcCPointer pntr )
{
	EcInt res;
	EcCPointer *newcpointer;
	EcInt at, newsize;

	res = ec_cpointer2index( pntr );
	if (res >= 0)
		return res;

	/* new pointer */
	at = PRIVATE(ncpointers);
	newsize = PRIVATE(ncpointers) + 1;
	newcpointer = ec_realloc( PRIVATE(cpointer), newsize * sizeof(EcCPointer) );
	if (! newcpointer)
		return -1;
	PRIVATE(cpointer)   = newcpointer;
	PRIVATE(ncpointers) = newsize;

	PRIVATE(cpointer)[at] = pntr;
	return at;
}

EcInt ec_cpointer2index( EcCPointer pntr )
{
	EcInt i;

	for (i = 0; i < PRIVATE(ncpointers); i++)
		if (PRIVATE(cpointer)[i] == pntr)
			return i;

	return -1;
}

EcCPointer ec_index2cpointer( EcInt index )
{
	ASSERT( index >= 0 );
	ASSERT( index < PRIVATE(ncpointers) );

	return PRIVATE(cpointer)[index];
}

EcBool ec_pointer_registered( EcCPointer pntr )
{
	if (ec_cpointer2index( pntr ) >= 0)
		return TRUE;

	return FALSE;
}

/* C standard functions equivalents */

double ec_strtod(const char *nptr, char **endptr)
{
#if (! HAVE_STRTOD) || (! EC_FORCE_LIBC)
	extern double _ec_strtod(const char *s00, char **se);

	return _ec_strtod(nptr, endptr);
#else
	return strtod(nptr, endptr);
#endif
}

/* symbols to mask */

EC_OBJ _ec_symbol2mask( const char *func_name, EcInt param_index, _ec_symbol2int *map, EcBool nullok, EC_OBJ sym, EcInt *options )
{
	ec_string ds;
	EC_OBJ    exc;
	EcInt     i;

	ASSERT( options );

	*options = 0;

	if (EC_NULLP(sym))
	{
		if (nullok)
			return EC_NIL;
		return EC_TYPEERROR_F(/* function name    */ func_name,
							  /* parameter index  */ param_index,
							  /* expected         */ tc_symbol,
							  /* offending object */ sym,
							  /* reason           */ "expected a symbol");
	}

#define TYPE_ERROR_sym2m	\
EC_TYPEERROR_F(/* function name    */ func_name,                                     \
			   /* parameter index  */ param_index,                                   \
			   /* expected         */ nullok ? tc_none : tc_symbol,                  \
			   /* offending object */ sym,                                           \
			   /* reason           */ nullok ? "expected a symbol or @nil" : "expected a symbol")

	if (! EC_SYMBOLP(sym))
		return TYPE_ERROR_sym2m;

	i = 0;
	while (map[i].symbolid > 0)
	{
		if (map[i].symbolid == EC_SYMBOL(sym))
		{
			*options |= map[i].value;
			return EC_NIL;
		}
		i++;
	}

	ec_string_init( &ds, NULL );
	ec_sprintf( &ds, "symbol %w not recognized as a valid option", sym );
	exc = EC_TYPEERROR_F(/* function name    */ func_name,
						 /* parameter index  */ param_index,
						 /* expected         */ nullok ? tc_none : tc_symbol,
						 /* offending object */ sym,
						 /* reason           */ ec_strdata( &ds ) );
	ec_string_cleanup( &ds );
	return exc;
}

EC_OBJ _ec_sequence2mask( const char *func_name, EcInt param_index, _ec_symbol2int *map,
						  EcBool nullok, EcBool singleok, EC_OBJ seq, EcInt *options )
{
	EC_OBJ el, exc;
	EC_OBJ seql_o;
	EcInt  seql, i, j;
	EcBool found;

	ASSERT( options );

	*options = 0;

	if (EC_NULLP(seq))
	{
		if (nullok)
			return EC_NIL;
		return EC_TYPEERROR_F(/* function name    */ func_name,
							  /* parameter index  */ param_index,
							  /* expected         */ tc_none,
							  /* offending object */ seq,
							  /* reason           */ "expected a sequence of option symbols");
	}

#define TYPE_ERROR_seq2m	\
EC_TYPEERROR_F(/* function name    */ func_name,                                     \
			   /* parameter index  */ param_index,                                   \
			   /* expected         */ tc_none,                                       \
			   /* offending object */ seq,                                           \
			   /* reason           */ nullok ? "expected a sequence of option symbols or @nil" : "expected a sequence of option symbols")

	if (singleok && EC_SYMBOLP(seq))
		return _ec_symbol2mask( func_name, param_index, map, nullok, seq, options );

	if (! EcIsSequence( seq ))
		return TYPE_ERROR_seq2m;

	seql_o = EcSequenceLength( seq );
	if (EC_ERRORP(seql_o)) return seql_o;
	ASSERT( EC_INUMP(seql_o) );
	seql = EC_INUM(seql_o);

	for (i = 0; i < seql; i++)
	{
		el = EcSequenceGetElement( seq, i );
		if (EC_ERRORP(el)) return el;

		if (EC_SYMBOLP(el))
		{
			j = 0; found = FALSE;
			while (map[j].symbolid > 0)
			{
				if (map[j].symbolid == EC_SYMBOL(el))
				{
					*options |= map[j].value;
					found = TRUE;
					break;
				}
				j++;
			}

			if (! found)
			{
				ec_string ds;
				ec_string_init( &ds, NULL );
				ec_sprintf( &ds, "symbol %w not recognized in option sequence", el );
				exc = EC_TYPEERROR_F(/* function name    */ func_name,
									 /* parameter index  */ param_index,
									 /* expected         */ tc_none,
									 /* offending object */ seq,
									 /* reason           */ ec_strdata( &ds ) );
				ec_string_cleanup( &ds );
				return exc;
			}

		} else
			return TYPE_ERROR_seq2m;
	}

	/* ok */
	return EC_NIL;
}

EC_OBJ _ec_mask2sequence( const char *func_name, _ec_symbol2int *map,
						  unsigned long mask )
{
	EC_OBJ res;
	EcInt  i;

	res = EcMakeArray( 1 );
	if (EC_ERRORP(res)) return res;

	i = 0;
	while (map[i].symbolid > 0)
	{
		if (map[i].value & mask)
			EcArrayPush( res, EcMakeSymbolFromId( map[i].symbolid ) );
		i++;
	}

	return res;
}

/* Debugging */

#if EC_DEBUG || EC_DEBUG_MINIMUM

void _ec_dbg_dump_stack( EC_OBJ stack )
{
	EcInt i;

/*	fprintf( stderr, "SP - SB: %d, SD: %ld\n", (EC_STACKPOINTER(stack) - EC_STACKBASE(stack)), EC_STACKDIM(stack) );*/
	fflush( stdout );
	ASSERT( (EC_STACKPOINTER(stack) - EC_STACKBASE(stack)) <= EC_STACKDIM(stack) );
	ASSERT( (EC_STACKBP(stack) - EC_STACKBASE(stack))      <= EC_STACKDIM(stack) );

	printf( "/--STACK  dim:%3ld --(0x%08lX)----------\\\n", (long)EC_STACKDIM(stack), (unsigned long)stack );
	/*for (i = 0; i < EC_STACKDIM(stack); i++)*/
	for (i = 0; i < (EC_STACKPOINTER(stack) - EC_STACKBASE(stack)); i++)
	{
		if ((EC_STACKBP(stack) - EC_STACKBASE(stack)) == i)
			printf( " BP %3ld  ", (long)i );
		else
			printf( " %6ld  ", (long)i );
		ec_fprintf( stdout, "%r\n", EC_STACKGET(stack, i) );
	}
	printf( "\\-----------------------------------------/\n" );
	fflush( stdout );
}

void _ec_dbg_dump_literal( EC_OBJ lframe )
{
	EcInt i;

	ASSERT( EC_ARRAYP(lframe) );

	fflush( stdout );
	printf( "/--LITERAL FRAME--------------------------\\\n" );
	for (i = 0; i < EC_ARRAYLEN(lframe); i++)
	{
		printf( " %-3ld  ", (long)i );
		ec_fprintf( stdout, "%r\n", EcArrayGet( lframe, i ) );
	}
	printf( "\\-----------------------------------------/\n" );
	fflush( stdout );
}

void _ec_dbg_dump_package_frame( EC_OBJ pframe )
{
	EcInt i;

	ASSERT( EC_ARRAYP(pframe) );

	fflush( stdout );
	printf( "/--PACKAGE FRAME--------------------------\\\n" );
	for (i = 0; i < EC_ARRAYLEN(pframe); i++)
	{
		printf( " %-3ld  ", (long)i );
		ec_fprintf( stdout, "%r\n", EcArrayGet( pframe, i ) );
	}
	printf( "\\-----------------------------------------/\n" );
	fflush( stdout );
}

void _ec_dbg_print_instruction( EC_OBJ compiled, EcUInt PC )
{
	EcBytecode *code;											/* Start of code   */
	EcBytecode  instr;
	const char *name;
	EcInt       npar;
	EcUInt      j;

	ASSERT( EC_COMPILEDP(compiled) );

	code = EC_COMPILEDCODE(compiled);
	instr = code[PC];

	name = EcBytecodeName( instr );
	npar = EcBytecodeParams( instr );

	printf( "%5ld  ", (long)PC );
	printf( "%18s",   name );
	for (j = PC+1; j <= PC+npar; j++)
	{
		printf( "  %12ld", (long)code[j] );
	}
	printf( "\n" );
}

#endif /* EC_DEBUG || EC_DEBUG_MINIMUM */
