/* ==========================================================================
 * elastic.h
 * ==========================================================================
 * elastiC language.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Mon May 25 19:15:36 MEST 1998
 *
 *   $Id: elastic.h,v 1.3 2002/06/06 16:10:57 panta Exp $
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

#ifndef __ELASTIC_H
#define __ELASTIC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include <elastic/conf.h>
#include <elastic/basic.h>
#include <elastic/os.h>
#include <elastic/object.h>
#include <elastic/user.h>
#include <elastic/memory.h>
#include <elastic/bytecode.h>
#include <elastic/oop.h>
#include <elastic/dstring.h>
#include <elastic/hashtable.h>
#include <elastic/list.h>
#include <elastic/hashdefs.h>
#include <elastic/file.h>
#include <elastic/hash.h>

#include <elastic/lib.h>

EC_BEGIN_DECLS

/* Globally available objects */

extern EC_API EC_OBJ EcFalseObject;
extern EC_API EC_OBJ EcTrueObject;
extern EC_API EC_OBJ Ec_ERROR;
extern EC_API EC_OBJ EcUndefinedObject;

extern EC_API EC_OBJ EcObjectClass;
extern EC_API EC_OBJ EcExceptionClass;
extern EC_API EC_OBJ EcUnimplementedErrorClass;
extern EC_API EC_OBJ EcMathErrorClass;
extern EC_API EC_OBJ EcIOErrorClass;
extern EC_API EC_OBJ EcMemoryErrorClass;
extern EC_API EC_OBJ EcUnknownMethodErrorClass;
extern EC_API EC_OBJ EcUnknownOperationErrorClass;
extern EC_API EC_OBJ EcIndexErrorClass;
extern EC_API EC_OBJ EcTypeErrorClass;
extern EC_API EC_OBJ EcTargetErrorClass;
extern EC_API EC_OBJ EcParameterCountErrorClass;
extern EC_API EC_OBJ EcCompileErrorClass;

/* Compiler context and options */

typedef struct ec_compiler_ctxt_struct *ec_compiler_ctxt;
typedef struct ec_compiler_options_struct
{
	EcBool whole_package;										/* TRUE if we're compiling a whole package (or whole packages)         */
	EC_OBJ in_package;											/* compile the code in the given package, if not specified differently */

	EcBool  save;												/* TRUE if compiled packages must be saved to disk */
	char   *outputfile;											/* output filename or NULL                         */
} ec_compiler_options;

/* Qualified Symbol */

typedef struct
{
	EcInt  n;
	EcUInt sym[EC_MAXSYMBOLNESTING];
} EcQualifiedSymbol;

typedef enum { EcFatal, EcError, EcWarning } EcErrorSeverity;

/* Parameter conversion through EcParseStack() */
typedef EC_OBJ (*EcParamConverter)( EcInt paramindex, EC_OBJ obj, void *into, EcDWord user1, EcDWord user2 );

/* Dynamically loaded code */
typedef void *ec_dlhandle;										/* module handle  */
typedef void *ec_dlptr;											/* symbol pointer */

/* ========================================================================
 * A P I
 * ======================================================================== */

/* Library Initialization/Cleanup */

EC_API EcBool      EcInit( void );
EC_API void        EcCleanup( void );
EC_API EcUInt      EcVersionNumber( void );
EC_API const char *EcVersionString( void );

EC_API EC_OBJ      EcSetArgs( int argc, char *argv[] );				/* Always use *after* package loading/saving ! */
#if 0
EcInt         EcArgc( void );
const char  **EcArgv( void );
EcInt        *EcArgcp( void );
const char ***EcArgvp( void );
const char   *EcArg( EcInt arg );
#endif

/* Error handling */

EC_API void    EcAlert( EcErrorSeverity severity, const char *format, ... );
EC_API EC_OBJ  EcMakeException( EC_OBJ exc_class, const char *format, ... );
EC_API EC_OBJ  EcMakeExceptionWithArgs( EC_OBJ exc_class, const char *msg, EcInt nargs, ... );
EC_API EC_OBJ  EcUnimplementedError( const char *msg );
EC_API EC_OBJ  EcMathError( const char *msg );
EC_API EC_OBJ  EcIOError( EC_OBJ ioObject, EcUInt posixErrorSymbol, const char *msg );
EC_API EC_OBJ  EcMemoryError( void );
EC_API EC_OBJ  EcTypeError( EC_OBJ target, EC_OBJ function,
							EcInt index,
							EcInt expected,
							EC_OBJ offending,
							EcBool print_offending,
							const char *reason );
EC_API EC_OBJ  EcTargetError( EC_OBJ target, const char *message );
EC_API EC_OBJ  EcUnknownOperationError( const char *opname, EcInt nargs, ... );
EC_API EC_OBJ  EcIndexError( EC_OBJ object, EC_OBJ index );
EC_API EC_OBJ  EcParameterCountError( EC_OBJ target,
									  EC_OBJ function,
									  EcInt expected,
									  EC_OBJ stack );
EC_API EC_OBJ  EcParameterCountErrorFunction( const char *funcname,
											  EcInt expected,
											  EC_OBJ stack );
EC_API EC_OBJ  EcParameterCountErrorMethod( EC_OBJ target,
											const char *method,
											EcInt expected,
											EC_OBJ stack );
EC_API EC_OBJ  EcParameterCountErrorClassMethod( EC_OBJ objclass,
												 const char *method,
												 EcInt expected,
												 EC_OBJ stack );
EC_API EC_OBJ  EcCompileError( EcInt line, EcInt column, const char *format, ... );

/* (see also EcTypeError() */

/* Basic Object handling */

EC_API EC_OBJ EcAllocObject( void );
EC_API EC_OBJ EcMakeUndefined( void );
EC_API EC_OBJ EcMakeBool( EcBool value );
#if EC_INLINED_TYPES
#define EcMakeInt(value)			(EC_MAKE_INT((value)))
#else
EC_API EC_OBJ EcMakeInt( EcInt value );
#endif
EC_API EC_OBJ EcMakeFloat( EcFloat value );
EC_API EC_OBJ EcMakeChar( EcChar value );
EC_API EC_OBJ EcMakeString( const char *string, EcInt len );
#if EC_INLINED_TYPES
#define EcMakeSymbolFromId(symid)	(EC_MAKE_SYMBOL((symid)))
#define EcMakeSymbol(string)		(EcMakeSymbolFromId(EcInternSymbol(string)))
#else
EC_API EC_OBJ EcMakeSymbolFromId( EcUInt symid );
EC_API EC_OBJ EcMakeSymbol( const char *string );
#endif
EC_API EC_OBJ EcMakeArray( EcInt dim );
EC_API EC_OBJ EcMakeArrayInit( EcInt nargs, ... );
EC_API EC_OBJ EcMakeStack( EC_OBJ callerstack, EcUInt dim );
EC_API EC_OBJ EcMakePrimitive( EcCPrimitive impl, const char *name,
							   EcAny userdata, EcBool userdata_is_object );
EC_API EC_OBJ EcMakeCMethod( EcCMethod impl, const char *name );
EC_API EC_OBJ EcMakeCompiled( EC_OBJ package,
							  const char *name,
							  EcInt  req_nargs,					/* minimum # of params (counting also default ones)                  */
							  EcInt  def_nargs,					/* # of parameters with default values (must be included in above #) */
							  EcBool varargs,
							  EcBool ismethod );
EC_API EC_OBJ EcMakeHandler( EC_OBJ type, EC_OBJ code );
EC_API EC_OBJ EcMakePackage( const char *name );
EC_API EC_OBJ EcMakeClass( EC_OBJ package, EC_OBJ superclass, const char *name );
EC_API EC_OBJ EcMakeObject( EC_OBJ ofclass, void *user );
EC_API EC_OBJ EcMakeUser( EcInt type, void *data );

/* Construction & Copy construction */

EC_API EC_OBJ EcBuildValue( const char *format, ... );
EC_API EC_OBJ EcCopy( EC_OBJ obj, EcCopyType type );
EC_API EC_OBJ EcShallowCopy( EC_OBJ obj );
EC_API EC_OBJ EcDeepCopy( EC_OBJ obj );

/* Basic types' operations */

EC_API EcBool  EcResizeArray( EC_OBJ array, EcInt newSize );
EC_API EcBool  EcResizeArrayFor( EC_OBJ array, EcInt newSize, EcBool onlyExpand );
EC_API EC_OBJ  EcArrayGet( EC_OBJ array, EcInt i );
EC_API EC_OBJ *EcArrayGetR( EC_OBJ array, EcInt i );
EC_API EC_OBJ  EcArraySet( EC_OBJ array, EcInt i, EC_OBJ val );
EC_API EC_OBJ  EcArrayDel( EC_OBJ array, EcInt i );
EC_API EC_OBJ  EcArrayGetNE( EC_OBJ array, EcInt i ); 				/* Version without exc. handling             */
EC_API EC_OBJ *EcArrayGetRNE( EC_OBJ array, EcInt i );				/* Version without exc. handling             */
EC_API EC_OBJ  EcArraySetNE( EC_OBJ array, EcInt i, EC_OBJ val );	/* Version without exc. handling & expansion */
EC_API EcInt   EcArrayPush( EC_OBJ array, EC_OBJ obj );
EC_API EC_OBJ  EcArrayPop(  EC_OBJ array );

EC_API EcBool  EcHashGet( EC_OBJ hash, EC_OBJ key, EC_OBJ *value );
EC_API EcBool  EcHashSet( EC_OBJ hash, EC_OBJ key, EC_OBJ value );
EC_API EcBool  EcHashDel( EC_OBJ hash, EC_OBJ key );
EC_API EcBool  EcHashHasKey( EC_OBJ hash, EC_OBJ key );
EC_API EC_OBJ *EcHashKeys( EC_OBJ hash, EcInt *size );
EC_API EC_OBJ *EcHashValues( EC_OBJ hash, EcInt *size );

EC_API void   EcStackPush( EC_OBJ stack, EC_OBJ obj );
EC_API EC_OBJ EcStackPop(  EC_OBJ stack );
EC_API void   EcStackSet(  EC_OBJ stack, EcInt i, EC_OBJ obj );
EC_API EC_OBJ EcStackGet(  EC_OBJ stack, EcInt i );
EC_API void   EcStackBPSet(  EC_OBJ stack, EcInt i, EC_OBJ obj );
EC_API EC_OBJ EcStackBPGet(  EC_OBJ stack, EcInt i );

EC_API EcInt EcCompiledPush( EC_OBJ compiled, EcBytecode bc );

EC_API const char *EcTypeName( EcInt type );
EC_API const char *EcObjectTypeName( EC_OBJ obj );

EC_API EcBool EcIsSequence( EC_OBJ obj );
EC_API EcInt  EcSequenceNormalizeIndex( EC_OBJ sequence, EcInt i );
EC_API EC_OBJ EcSequenceGetElement( EC_OBJ sequence, EcInt index );
EC_API EC_OBJ EcSequenceGetItem( EC_OBJ sequence, EC_OBJ index );
EC_API EC_OBJ EcSequenceSetElement( EC_OBJ sequence, EcInt index, EC_OBJ value );
EC_API EC_OBJ EcSequenceSetItem( EC_OBJ sequence, EC_OBJ index, EC_OBJ value );
EC_API EC_OBJ EcSequenceDelElement( EC_OBJ sequence, EcInt index );
EC_API EC_OBJ EcSequenceDelItem( EC_OBJ sequence, EC_OBJ index );
EC_API EC_OBJ EcSequenceLength( EC_OBJ sequence );
EC_API EC_OBJ EcSequenceIn( EC_OBJ sequence, EC_OBJ element );

EC_API EcBool EcCompiledSetLine( EC_OBJ compiled, EcUInt bytecode_idx, EcInt line_num );
EC_API EcInt  EcCompiledGetLine( EC_OBJ compiled, EcUInt bytecode_idx );

/* Stack utility function */

	/* These are for building a stack suitable to a call from C code */
EC_API EC_OBJ EcMakeStackForCompiled( EC_OBJ compiled, EcInt nargs );
EC_API EC_OBJ EcMakeStackForMethod( EC_OBJ target, EcUInt methodid, EcInt nargs );
EC_API EC_OBJ EcMakeStackForMethod2( EC_OBJ target, EC_OBJ method, EcInt nargs );

	/* Parsing a stack into a series of arguments */
EC_API EC_OBJ EcParseStack( EC_OBJ target, EC_OBJ function, EcBool consume, EC_OBJ stack, const char *format, ... );
EC_API EC_OBJ EcParseStackV( EC_OBJ target, EC_OBJ function, EcBool consume, EC_OBJ stack, const char *format, va_list ap );
EC_API EC_OBJ EcParseStackFunction( const char *funcname, EcBool consume, EC_OBJ stack, const char *format, ... );
EC_API EC_OBJ EcParseStackMethod( EC_OBJ self, const char *methodname, EcBool consume, EC_OBJ stack, const char *format, ... );
EC_API EC_OBJ EcParseStackClassMethod( EC_OBJ objclass, const char *methodname, EcBool consume, EC_OBJ stack, const char *format, ... );

/* User types */

EC_API EcInt EcAddType( EcTypespec *newtype );

/* Numerical */

EC_API EcBool EcPromote( EC_OBJ *num1, EC_OBJ *num2 );

/* Calling */

EC_API EcBool EcIsCallable( EC_OBJ obj );
EC_API EC_OBJ EcCall( EC_OBJ callerstack, EC_OBJ self, EC_OBJ at_class, EC_OBJ obj, EcInt nargs, ... );
EC_API EC_OBJ EcCallVA( EC_OBJ callerstack, EC_OBJ self, EC_OBJ at_class, EC_OBJ obj, EcInt nargs, va_list args );
EC_API EC_OBJ EcCallArgs( EC_OBJ callerstack, EC_OBJ self, EC_OBJ at_class, EC_OBJ obj, EcInt nargs, EC_OBJ *args );

/* Object Oriented Programming */

	/* Adding classes */
EC_API EC_OBJ EcAddClass( const char *package, const char *name, EC_OBJ superclass );
EC_API EC_OBJ EcAddClassDef( EcClassDef *classdef );

	/* Adding methods & data members */
EC_API EcBool EcAddMethod( EC_OBJ classobj, const char *name, EC_OBJ impl );
EC_API EcBool EcAddClassMethod( EC_OBJ classobj, const char *name, EC_OBJ impl );
EC_API EcInt  EcAddInstanceVariable( EC_OBJ classobj, const char *name );
EC_API EcInt  EcAddClassVariable( EC_OBJ classobj, const char *name );

	/* Inspecting objects & classes */
EC_API EcBool EcDoesUnderstand( EC_OBJ obj, EcUInt methodid );
EC_API EC_OBJ EcFindMethod( EC_OBJ obj, EC_OBJ *at_class, EcUInt methodid );
EC_API EC_OBJ EcFindClassMethod( EC_OBJ classobj, EC_OBJ *at_class, EcUInt methodid );
EC_API EcBool EcIsOfClass( EC_OBJ obj, EC_OBJ classobj );

	/* Sending messages */
EC_API EC_OBJ EcSendMessage( EC_OBJ obj, EcUInt methodid, EC_OBJ stack );
EC_API EC_OBJ EcSendMessageVA( EC_OBJ obj, EcUInt methodid, EcInt nargs, ... );
EC_API EC_OBJ EcSendMessageArgs( EC_OBJ obj, EcUInt methodid, EcInt nargs, EC_OBJ *args );
EC_API EC_OBJ EcSendSuperMessage( EC_OBJ obj, EC_OBJ at_class,
								  EcUInt methodid, EC_OBJ stack ); /* at_class is *current* class */
EC_API EC_OBJ EcUnknownMethod( EC_OBJ obj, EC_OBJ at_class,
							   EcUInt methodid, EC_OBJ stack );
EC_API EC_OBJ EcDispatchObjectMessage( EC_OBJ obj, EC_OBJ at_class, EcUInt methodid, EC_OBJ stack );
EC_API EC_OBJ EcDispatchClassMessage( EC_OBJ obj, EcUInt methodid, EC_OBJ stack );

	/* Modifing state */
EC_API EC_OBJ EcGetInstanceVariableSym( EC_OBJ obj, EC_OBJ at_class, EcUInt varSymbol );
EC_API EC_OBJ EcGetInstanceVariable( EC_OBJ obj, EC_OBJ at_class, const char *name );
EC_API EC_OBJ EcSetInstanceVariableSym( EC_OBJ obj, EC_OBJ at_class, EcUInt varSymbol, EC_OBJ value );
EC_API EC_OBJ EcSetInstanceVariable( EC_OBJ obj, EC_OBJ at_class, const char *name, EC_OBJ value );
EC_API EC_OBJ EcGetClassVariableSym( EC_OBJ classobj, EcUInt varSymbol );
EC_API EC_OBJ EcGetClassVariable( EC_OBJ classobj, const char *name );
EC_API EC_OBJ EcSetClassVariableSym( EC_OBJ classobj, EcUInt varSymbol, EC_OBJ value );
EC_API EC_OBJ EcSetClassVariable( EC_OBJ classobj, const char *name, EC_OBJ value );


/* Garbage Collection */

EC_API EcBool EcCheckForGC( void );
EC_API void   EcGC( void );
EC_API void   EcMarkObject( EC_OBJ obj );
EC_API EcInt  EcGCProtect( EC_OBJ obj );
EC_API void   EcGCUnprotect( EC_OBJ obj );
EC_API void   EcGCUnprotectAll( void );

/* Symbol handling */

EC_API EcUInt      EcInternSymbol( const char *string );
EC_API EcUInt      EcSymbolId( const char *string );
EC_API const char *EcSymbolAt( EcUInt symbolid );
EC_API EcBool      EcSymbolExists( EcUInt symbolid );

/* Qualified names */

EC_API EcBool EcNameQualified( const ec_string *name );
EC_API char  *EcQualifyName( ec_string *dst, const ec_string *prefix, const ec_string *suffix );
EC_API char  *EcNamePrefix( ec_string *dst, const ec_string *name );
EC_API char  *EcNameSuffix( ec_string *dst, const ec_string *name );

/* Qualified symbol handling */

EC_API void               EcQualifiedInit( EcQualifiedSymbol *qsymbol );
EC_API EcQualifiedSymbol *EcQualifiedCopy( EcQualifiedSymbol *dst, EcQualifiedSymbol *src );
EC_API void               EcQualifiedAdd( EcQualifiedSymbol *qsymbol, EcUInt sym );
EC_API void               EcQualifiedCat( EcQualifiedSymbol *dst, EcQualifiedSymbol *qs1, EcQualifiedSymbol *qs2 );
EC_API EcBool             EcQualifiedEq( EcQualifiedSymbol *s1, EcQualifiedSymbol *s2 );
EC_API char              *EcQualifiedString( EcQualifiedSymbol *qsymbol );
EC_API void               EcQualifiedFromString( EcQualifiedSymbol *dst, const char *string );

/* Packages */

EC_API char  *EcPackagePathFromName( ec_string *path,
									 const ec_string *packagename, const char *suffix );
EC_API EC_OBJ EcPackageIntroduce( const char *name );	                                   /* name is qualified                               */
EC_API EcInt  EcPackageVariable( EC_OBJ package, const char *symbol,
								 EC_OBJ value, EcBool pub, EcBool isconst );               /* name unqualified or package EC_NIL              */
EC_API EcBool EcPackageImport( EC_OBJ inPackage,
							   const char *name, EC_OBJ array_of_syms,
							   EcBool execute );						                   /* name qualified, array_of_syms can be EC_NIL/#t  */
EC_API EcBool EcPackageSave( EC_OBJ package, const char *pathname );	                   /* pathname can be NULL                            */
EC_API EC_OBJ EcPackageLoad( const char *name,
							 const char *pathname,
							 EcBool execute,
							 EcBool executeImported );					                   /* name is qualified, pathname can be NULL         */
EC_API const char *EcPackageShortName( ec_string *dst, EC_OBJ package );
EC_API EC_OBJ EcPackageFromName( const char *name );	                                   /* name is qualified                               */

EC_API EC_OBJ EcGetVariable( const char *packagename, const char *symbol );                /* return a variable's content given package name and variable name */
EC_API EC_OBJ EcSetVariable( const char *packagename, const char *symbol, EC_OBJ value );  /* set a variable's content given package name and variable name    */
EC_API EC_OBJ EcGetQVariable( const char *qualifiedsymbol );			     			   /* return a variable's content given full variable name             */
EC_API EC_OBJ EcSetQVariable( const char *qualifiedsymbol, EC_OBJ value );			       /* set a variable's content given full variable name                */

/* Primitive procedures */

EC_API EcBool EcAddPrimitive( const char *name, EcCPrimitive prim ); 						/* name is qualified */
EC_API EcBool EcAddPrimitiveUserData( const char *name, EcCPrimitive prim,					/* name is qualified */
									  EcAny userdata, EcBool userdata_is_object );

/* Builtin registration & retrieval */

EC_API void        EcRegisterBuiltin( const char *name, EC_OBJ builtin );
EC_API const char *EcBuiltinName( EC_OBJ builtin );

/* Compilation & Execution */

EC_API ec_compiler_ctxt EcCompilerContextCreate( void );
EC_API void             EcCompilerContextDestroy( ec_compiler_ctxt ctxt );
EC_API EC_OBJ           EcCompile( ec_compiler_ctxt ctxt, const char *filename,
								   EcBool execute, EcBool executeImported,
								   ec_compiler_options *opts );
EC_API EC_OBJ EcCompileString( ec_compiler_ctxt ctxt,
							   const char *sourcestring,
							   EcBool execute, EcBool executeImported,
							   ec_compiler_options *opts );
EC_API EC_OBJ EcMainExecute( EC_OBJ package );
EC_API EC_OBJ EcExecute( EC_OBJ self, EC_OBJ at_class, EC_OBJ compiled, EC_OBJ stack );

/* Printing */

	/* low-level printing */
EC_API EcInt EcPrintObject( ec_string *str, EC_OBJ obj, EcBool detailed );

	/* basic printing functions */
EC_API EcInt  ec_sprintf(    ec_string *ds, const char *format, ... );
EC_API EcInt  ec_vsprintf(   ec_string *ds, const char *format, va_list ap );
EC_API EcInt  ec_fprintf(    FILE *fh, const char *format, ... );
EC_API EcInt  ec_vfprintf(   FILE *fh, const char *format, va_list ap );
EC_API EcInt  ec_asprintf(   char **res, const char *format, ... );
EC_API EcInt  ec_vasprintf(  char **res, const char *format, va_list ap );
EC_API EC_OBJ ec_oprintf(    const char *format, ... );
EC_API EC_OBJ ec_voprintf(   const char *format, va_list ap );
EC_API EC_OBJ ec_printf_obj( const char *format, EC_OBJ *args, EcInt nargs, EcInt parstart, EC_OBJ named_sequence );

/* Object fundamental operations */

EC_API EcBool EcObjectEqual( EC_OBJ obj1, EC_OBJ obj2 );
EC_API EcUInt EcHash( EC_OBJ obj );
EC_API EcBool EcObjectCompare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res );

/* C dynamic loading support interface */

EC_API EcBool      EcDLInit( void );
EC_API void        EcDLCleanup( void );
EC_API ec_dlhandle EcDLOpen( const char *filename );
EC_API void        EcDLClose( ec_dlhandle handle );
EC_API ec_dlptr    EcDLSymbol( ec_dlhandle handle, const char *symname );
EC_API const char *EcDLError( void );

/* C standard functions equivalents */

EC_API double ec_strtod(const char *nptr, char **endptr);

/* Debugging */

EC_API EcBool EcCheck( EC_OBJ obj );
EC_API EcBool EcCheckAll( void );
EC_API void   EcDump( EC_OBJ obj );
EC_API void   EcDumpCompiled( EC_OBJ compiled, EcInt at );
EC_API EcBool EcIsObject( void *pointer );

/* Macros */

#define EC_MAKEVERSION(major, minor, micro)		((((major) & 0xFF) << 16) | (((minor) & 0xFF) << 8) | ((micro) & 0xFF))
#define EC_MAJOR(ver)							(((ver) >> 16) & 0xFF)
#define EC_MINOR(ver)							(((ver) >> 8)  & 0xFF)
#define EC_MICRO(ver)							((ver)         & 0xFF)

#define EC_CHECKNARGS_F(fname, expected)	              \
do {                                                      \
	if (EC_STACKNARGS_LOGICAL((stack)) != (expected))     \
		return EcParameterCountErrorFunction( (fname),    \
									          (expected), \
                                              (stack) );  \
} while (0)
#define EC_CHECKNARGS_MIN_F(fname, expected_min)              \
do {                                                          \
	if (EC_STACKNARGS_LOGICAL((stack)) < (expected_min))      \
		return EcParameterCountErrorFunction( (fname),        \
									          (expected_min), \
                                              (stack) );      \
} while (0)
#define EC_CHECKNARGS_OPT_F(fname, expected_min, expected_max) \
do {                                                           \
	if (EC_STACKNARGS_LOGICAL((stack)) < (expected_min))       \
		return EcParameterCountErrorFunction( (fname),         \
									          (expected_min),  \
                                              (stack) );       \
	else if (EC_STACKNARGS_LOGICAL((stack)) > (expected_max))  \
		return EcParameterCountErrorFunction( (fname),         \
									          (expected_max),  \
                                              (stack) );       \
} while (0)
#define EC_CHECKNARGS_M(self, mname, expected)	                   \
do {                                                               \
	if (EC_STACKNARGS_LOGICAL((stack)) != (expected))              \
		return EcParameterCountErrorMethod( (self),                \
									        (mname),               \
									        (expected), (stack) ); \
} while (0)
#define EC_CHECKNARGS_CM(objclass, mname, expected)	                    \
do {                                                                    \
	if (EC_STACKNARGS_LOGICAL((stack)) != (expected))                   \
		return EcParameterCountErrorClassMethod( (objclass),            \
									             (mname),               \
									             (expected), (stack) ); \
} while (0)

#define EC_TYPEERROR_F(fname, index, expected, off, reason)				EcTypeError( EC_NIL, EcGetQVariable( (fname) ), (index), (expected), (off), TRUE, (reason) )
#define EC_TYPEERROR_M(self, mname, index, expected, off, reason)		EcTypeError( (self), EcFindMethod( self, NULL, EcInternSymbol( (mname) ) ), (index), (expected), (off), TRUE, (reason) )
#define EC_TYPEERROR_CM(objclass, mname, index, expected, off, reason)	EcTypeError( (self), EcFindClassMethod( objclass, NULL, EcInternSymbol( (mname) ) ), (index), (expected), (off), TRUE, (reason) )

#define EC_CHECKTYPE_F(fname, index, var, expected)	                        \
do {                                                                        \
	if (EC_NTYPEP((var),(expected)))                                        \
		return EC_TYPEERROR_F( (fname), (index), (expected), (var), NULL ); \
} while (0)
#define EC_CHECKTYPE_M(self, mname, index, var, expected)	                        \
do {                                                                                \
	if (EC_NTYPEP((var),(expected)))                                                \
		return EC_TYPEERROR_M( (self), (mname), (index), (expected), (var), NULL ); \
} while (0)
#define EC_CHECKTYPE_CM(objclass, mname, index, var, expected)	                        \
do {                                                                                    \
	if (EC_NTYPEP((var),(expected)))                                                    \
		return EC_TYPEERROR_M( (objclass), (mname), (index), (expected), (var), NULL ); \
} while (0)


#if EC_STACKCHECK_ENABLED
#define EC_STACKPUSH(stack, obj)		do { EcStackPush( stack, obj ); } while (0)
#define EC_STACKPOP(stack)				(EcStackPop( stack ))
#define EC_STACKDUP(stack)				do { EcStackPush( stack, *(EC_STACKPOINTER(stack) - 1) ); } while (0)
#define EC_STACKSET(stack, i, obj)		do { EcStackSet( stack, i, obj ); } while (0)
#define EC_STACKGET(stack, i)			(EcStackGet( stack, i ))
#define EC_STACKBPSET(stack, i, obj)	do { EcStackBPSet( stack, i, obj ); } while (0)
#define EC_STACKBPGET(stack, i)			(EcStackBPGet( stack, i ))
#else
#define EC_STACKPUSH(stack, obj)		do { *EC_STACKPOINTER(stack)++ = (obj); } while (0)
#define EC_STACKPOP(stack)				*(--EC_STACKPOINTER(stack))
#define EC_STACKDUP(stack)				do { EC_STACKPUSH(stack, *(EC_STACKPOINTER(stack) - 1)); } while (0)
#define EC_STACKSET(stack, i, obj)		do { EC_STACKBASE(stack)[(i)] = (obj); } while (0)
#define EC_STACKGET(stack, i)			(EC_STACKBASE(stack)[(i)])
#define EC_STACKBPSET(stack, i, obj)	do { EC_STACKBP(stack)[(i)] = (obj); } while (0)
#define EC_STACKBPGET(stack, i)			(EC_STACKBP(stack)[(i)])
#endif /* EC_STACKCHECK_ENABLED */

#if EC_ARRAYCHECK_ENABLED
#define EC_ARRAYGET(array, i)			(EcArrayGetNE( array, i ))
#define EC_ARRAYGETR(array, i)			(EcArrayGetRNE( array, i ))
#define EC_ARRAYSET(array, i, obj)		do { EcArraySetNE( array, i, obj ); } while (0)
#else
#define EC_ARRAYGET(array, i)			(EC_ARRAYMEM(array)[i])
#define EC_ARRAYGETR(array, i)			(&(EC_ARRAYMEM(array)[i]))
#define EC_ARRAYSET(array, i, obj)		do { EC_ARRAYMEM(array)[i] = (obj); } while (0)
#endif /* EC_ARRAYCHECK_ENABLED */

#if EC_DEBUG || EC_DEBUG_MINIMUM
void _ec_dbg_dump_stack( EC_OBJ stack );
void _ec_dbg_dump_literal( EC_OBJ lframe );
void _ec_dbg_dump_package_frame( EC_OBJ pframe );
void _ec_dbg_print_instruction( EC_OBJ compiled, EcUInt PC );
#endif

/* Define EC_VA_COPY() to do the right thing for copying va_list variables.
 * EC_VA_COPY may have already been defined in config.h (as va_copy or __va_copy).
 */
#if !defined (EC_VA_COPY)
#  if defined (__GNUC__) && defined (__PPC__) && (defined (_CALL_SYSV) || defined (_WIN32))
#    define EC_VA_COPY(ap1, ap2)   (*(ap1) = *(ap2))
#  elif defined (EC_VA_COPY_AS_ARRAY)
#    define EC_VA_COPY(ap1, ap2)   ec_memmove ((ap1), (ap2), sizeof (va_list))
#  else /* va_list is a pointer */
#    define EC_VA_COPY(ap1, ap2)   ((ap1) = (ap2))
#  endif /* va_list is a pointer */
#endif /* !EC_VA_COPY */


void ec_protect_obj( EC_OBJ obj );
void ec_protect_objp( EC_OBJ *objp );

EC_END_DECLS

#endif /* __ELASTIC_H */
