/* ==========================================================================
 * compile.c
 * ==========================================================================
 * Bytecode Compiler.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat May 23 18:12:33 MEST 1998
 *
 *   $Id: compile.c,v 1.2 2002/05/24 17:17:14 panta Exp $
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

#include "ast.h"
#include "compile.h"
#include "elastic.h"

#include "private.h"
#include "debug.h"

/*
#undef EC_VERBOSE_COMPILATION
#undef EC_DEBUG_COMPILATION
#define EC_VERBOSE_COMPILATION 1
#define EC_DEBUG_COMPILATION   1
*/

#define PRINTVINFO 0
#if EC_DEBUG_COMPILATION
#undef PRINTVINFO
#define PRINTVINFO 1
#endif

#define EC_DEBUG_DUMP_EMIT 0

#define EC_DEBUG_SCOPE 0

#if 0
static void print_labels( void );
#endif

/*
 * Compiler context structure
 *
 * :TODO: evaluate what needs to be moved from EcPrivate to
 * the context structure below (perhaps everything compiler-related).
 * It will be hard to do with scanner/parser things (how do we pass
 * these things arount ?).
 */

struct ec_compiler_ctxt_struct
{
	EC_OBJ compilationResult;									/* target of EcCompile*() (bytecode or package) */

	/* current scope, package, bytecode, literal */
	Scope  currentScope;
	EC_OBJ currentPackage;
	EC_OBJ currentBytecode;
	EC_OBJ currentLiteral;

	/* last l-value info */
	EcVarRefStruct lValue;										/* Last l-value info */
	EcBool         asLValue;									/* If TRUE, compile the subsequent variables as l-values, if applicable */

	/* Block return status */
	EcBool hasReturned;											/* TRUE if the compiled block performed a return */

	/* Discard expression result flag */
	EcBool discardResult;										/* default: TRUE (discard) */

	/* List of compiled packages to save */
	ec_list compiled_packages;


	/*
	 * 'labels': hash mapping bytecode to a list of labels.
	 * Each list is an ec_list with key label symid and data a labelinfo pointer.
	 */
	ec_hash labels;												/* bytecode -> labelinfo list       */


	/* package under compilation */
	EC_OBJ package_package;										/* package being compiled (or EC_NIL)                                   */
	EcBool package_saveHasReturned;								/* temporary for saving status of CCTXT(hasReturned) during compilation */
	Scope  package_packageScope;								/* package scope                                                        */


	/* compiler options */
	ec_compiler_options opts;
};

#define CCTXT(field)		((ctxt)->field)

/*
 * On the call frame, arguments and locals have an offset, since on the first
 * positions there is the caller object.
 */
#define FRAMEOFFSET					1
#define FUNCTIONFRAMEOFFSET			FRAMEOFFSET + 2
#define METHODFRAMEOFFS(pos)		((pos) + FRAMEOFFSET)
#define FUNCTIONFRAMEOFFS(pos)		((pos) + FUNCTIONFRAMEOFFSET)


/* ========================================================================
 * S T A T I C   P R O T O T Y P E S
 * ======================================================================== */

/* Scope */

static Scope       openScope( ec_compiler_ctxt ctxt, Scope upper, EC_OBJ target, FunctionType functype );
static void        closeScope( ec_compiler_ctxt ctxt, Scope scope );
static Scope       getMasterScope( ec_compiler_ctxt ctxt, Scope scope );

static Scope       buildScopeForPackage( ec_compiler_ctxt ctxt, EC_OBJ package );

#if EC_DEBUG_SCOPE
static void        dumpScope( ec_compiler_ctxt ctxt, Scope scope );
#endif

static STableEntry addSymbolItem( ec_compiler_ctxt ctxt, Scope scope,
								  EcUInt symid, EcInt position,
								  SymbolClass symclass, EcBool isconst );
static STableEntry addSymbol( ec_compiler_ctxt ctxt, Scope scope, EcUInt symid, SymbolClass symclass, EcBool isconst );
static EcBool      getSymbol( ec_compiler_ctxt ctxt, Scope scope,
							  EcQualifiedSymbol *qsym,			/* Symbol we want information for */
							  EcVarRef results );				/* Resulting information or NULL  */

/* Code generation */
static void   compileRoot( ec_compiler_ctxt ctxt, ASTNode node );
static void   compile( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileConstant( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileVariable( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileArrayCons( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileHashCons( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileUnary( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileBinary( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileConditional( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileOr( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileAnd( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileAssign( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileSimAssign( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileArrayRef( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileDecl( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileStatement( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileLabeledStmt( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileExpr( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileIf( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileWhile( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileDo( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileFor( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileForIn( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileBreak( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileContinue( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileGoto( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileTry( ec_compiler_ctxt ctxt, ASTNode node );
static EC_OBJ compileCatch( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileThrow( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileImport( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileCall( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileMethodCall( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileStmtList( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileFunction( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileReturn( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileClass( ec_compiler_ctxt ctxt, ASTNode node );
static void   compileMethod( ec_compiler_ctxt ctxt, ASTNode node );
static void   compilePackage_pre( ec_compiler_ctxt ctxt, ASTNode node );
static void   compilePackage_post( ec_compiler_ctxt ctxt );
static void   compilePackage( ec_compiler_ctxt ctxt, ASTNode node );
static EC_OBJ compileInlineCodeBlock( ec_compiler_ctxt ctxt, ASTNode node );

/* Second pass compilation */
static void resolve( ec_compiler_ctxt ctxt );

/* Misc utility functions */
static ASTNode getServiceVariable( ec_compiler_ctxt ctxt, Scope scope );
static EcBool  compileNamedVariable( ec_compiler_ctxt ctxt, const char *varname );

/* Bytecode manipulation */
static EcInt emit0( ec_compiler_ctxt ctxt, EcBytecode bc );
static EcInt emit1( ec_compiler_ctxt ctxt, EcBytecode bc, EcInt op1 );
static EcInt emit2( ec_compiler_ctxt ctxt, EcBytecode bc, EcInt op1, EcInt op2 );
/*static EcInt emit3( ec_compiler_ctxt ctxt, EcBytecode bc, EcInt op1, EcInt op2, EcInt op3 );*/
static EcInt nextPosition( ec_compiler_ctxt ctxt );
static void  patch( ec_compiler_ctxt ctxt, EcInt where, EcInt with );

static EcInt emitLoad( ec_compiler_ctxt ctxt, Scope scope, EcVarRef vinfo );
static EcInt emitSave( ec_compiler_ctxt ctxt, Scope scope, EcVarRef vinfo );

static EcBool getSymbolHere( ec_compiler_ctxt ctxt, Scope scope, EcUInt symid, EcVarRef results );
static EcBool getQualifiedSymbol( ec_compiler_ctxt ctxt, Scope scope, EcQualifiedSymbol *qsym, EcVarRef results );
static EcBool getForeignSymbol( ec_compiler_ctxt ctxt, EcUInt symid, EcVarRef results );

static EC_OBJ getLiteral( ec_compiler_ctxt ctxt, Scope scope, EcInt up );
static EC_OBJ getPackageFrame( ec_compiler_ctxt ctxt, EcInt pkg );

static void initVInfo( ec_compiler_ctxt ctxt, EcVarRef ref );
static void saveVInfo( ec_compiler_ctxt ctxt, EcVarRef dst, EcVarRef src );
#if PRINTVINFO
static void printVInfo( ec_compiler_ctxt ctxt, EcVarRef ref );
#endif

/* Label references */
	/* Get a list of bytecode objects produces in the compilation */
static ec_list produced_bytecode( ec_compiler_ctxt ctxt );
	/* Define a label in current bytecode and current position. */
static EcBool  define_label( ec_compiler_ctxt ctxt, EcUInt labelid );
	/* Get label position */
static EcInt   label_position( ec_compiler_ctxt ctxt, EC_OBJ bytecode, EcUInt labelid );
	/* Add a label reference in current bytecode at specified position. */
static EcBool  reference_label( ec_compiler_ctxt ctxt, EcUInt labelid, EcInt position );
	/* Returns a list of referenced labels in specified bytecode. */
static ec_list referenced_labels( ec_compiler_ctxt ctxt, EC_OBJ bytecode );
	/* Returns label references in specified bytecode. */
static ec_list label_references( ec_compiler_ctxt ctxt, EcUInt labelid, EC_OBJ bytecode );
	/* Cleanup data structures used for label references */
static void    labels_cleanup( ec_compiler_ctxt ctxt );

#define STACKGROW(sc, n)	\
do {                                                 \
	Scope master = getMasterScope( ctxt, sc );       \
	(master)->stackdim += (n);                       \
	if ((master)->stackdim > (master)->maxstackdim)  \
		(master)->maxstackdim = (master)->stackdim;  \
} while (0)

#define STACKSHRINK(sc, n)	\
do {                                            \
	Scope master = getMasterScope( ctxt, sc );  \
	(master)->stackdim -= (n);                  \
} while (0)


/* 'break'/'continue' handling */

#define CYCLE_LEVEL(scope)		((scope)->cycle_nesting)
#define ABS_LEVEL(scope, lev)	(CYCLE_LEVEL(scope) - (lev))
#define PUSH_BREAK(scope, pos, lev)	\
do {                                                                 \
	ec_stack_push( (scope)->break_pos, (pos) );                      \
	ec_stack_push( (scope)->break_lev, ABS_LEVEL((scope), (lev)) );  \
} while (0)
#define PUSH_CONTINUE(scope, pos, lev)	\
do {                                                                    \
	ec_stack_push( (scope)->continue_pos, (pos) );                      \
	ec_stack_push( (scope)->continue_lev, ABS_LEVEL((scope), (lev)) );  \
} while (0)

/* Line number handling */
#define SET_LINE(node) \
do {                                                             \
	EcCompiledSetLine( CCTXT(currentBytecode),                   \
					   EC_COMPILEDNCODE(CCTXT(currentBytecode)), \
					   FLINE(node) );                            \
} while (0);


/* ========================================================================
 * A P I
 * ======================================================================== */

static void opts_init(ec_compiler_options *opts)
{
	ASSERT(opts);
	opts->whole_package = FALSE;
	opts->in_package    = EC_NIL;
	opts->save          = FALSE;
	opts->outputfile    = NULL;
}

static void opts_cleanup(ec_compiler_options *opts)
{
	ASSERT(opts);
	opts->whole_package = FALSE;
	opts->in_package    = EC_NIL;
	opts->save          = FALSE;
	if (opts->outputfile)
		ec_free( opts->outputfile );
	opts->outputfile = NULL;
}

static void opts_copy(ec_compiler_options *dst, ec_compiler_options *src)
{
	ASSERT(dst);

	if (src)
	{
		ASSERT(dst != src);

		dst->whole_package = src->whole_package;
		dst->in_package    = src->in_package;
		dst->save          = src->save;
		if (src->outputfile)
			dst->outputfile = ec_stringdup( src->outputfile );
		else
			dst->outputfile = NULL;
	} else
		opts_init( dst );
}

EC_API ec_compiler_ctxt EcCompilerContextCreate( void )
{
	ec_compiler_ctxt ctxt;

	ctxt = ec_malloc( sizeof(struct ec_compiler_ctxt_struct) );
	if (! ctxt) return NULL;

	ctxt->compilationResult = EC_NIL;

	ctxt->currentScope    = NULL;
	ctxt->currentPackage  = EC_NIL;
	ctxt->currentBytecode = EC_NIL;
	ctxt->currentLiteral  = EC_NIL;

	memset(&(ctxt->lValue), 0x00, sizeof(EcVarRefStruct));
	ctxt->asLValue = FALSE;										/* if TRUE, subsequent variables are to be compiled as l-values */

	ctxt->hasReturned = FALSE;									/* TRUE if compiled block performed a return */

	ctxt->discardResult = TRUE;									/* discard expression result flag            */

	ctxt->compiled_packages = NULL;								/* list of compiled packages                 */

	ctxt->labels = NULL;										/* hash mapping bytecode to list of labels   */

	ctxt->package_package = EC_NIL;								/* package being compiled (or EC_NIL)        */
	ctxt->package_saveHasReturned = FALSE;
	ctxt->package_packageScope = NULL;							/* package scope                             */


	/* compiler options */
	opts_init( &ctxt->opts );

	return ctxt;
}

EC_API void EcCompilerContextDestroy( ec_compiler_ctxt ctxt )
{
	opts_cleanup( &ctxt->opts );

	ctxt->package_package = EC_NIL;								/* package being compiled (or EC_NIL)        */
	ctxt->package_saveHasReturned = FALSE;
	ctxt->package_packageScope = NULL;							/* package scope                             */

	if (ctxt->labels) labels_cleanup( ctxt );
	ASSERT( ctxt->labels == NULL );

	if (ctxt->compiled_packages)
		ec_list_destroy( ctxt->compiled_packages );
	ctxt->compiled_packages = NULL;

	/* :TODO: XXX currentScope ??? */

	ec_free( ctxt );
}

extern struct yy_buffer_state *yy_create_buffer ( FILE *file, int size );
extern void yy_switch_to_buffer ( struct yy_buffer_state *new_buffer );
extern int yyparse( void );
extern void yy_delete_buffer ( struct yy_buffer_state *b );

EC_API EC_OBJ EcCompile( ec_compiler_ctxt ctxt,
						 const char *filename,
						 EcBool execute, EcBool executeImported,
						 ec_compiler_options *opts )
{
    FILE *fh = NULL;
    struct yy_buffer_state *bufState = NULL;

	/* memorize compiler options in context */
	opts_copy( &ctxt->opts, opts );

	/* ==================================================
	 * 1.  P A R S E
	 * ================================================== */

    fh = fopen( filename, "r" );
    if (! fh) goto error;

	PRIVATE(fileSource) = filename ? ec_stringdup( filename ) : NULL;
	PRIVATE(fileOutput) = (opts && opts->outputfile) ? ec_stringdup( opts->outputfile ) : NULL;

	PRIVATE(line) = 1; PRIVATE(column) = 0; PRIVATE(charnum) = 0;
	PRIVATE(parse_result) = NULL;

#ifndef YY_BUF_SIZE
#define YY_BUF_SIZE 16384
#endif

    bufState = yy_create_buffer( fh, YY_BUF_SIZE );
    if (! bufState) goto error;
    yy_switch_to_buffer( bufState );

    /* Parse, building the AST */
    if (yyparse() != 0)
        goto error;

    yy_delete_buffer( bufState );
    fclose( fh );

	/* ==================================================
	 * 2.  C O M P I L E
	 * ================================================== */

	PRIVATE(execute)         = execute;
	PRIVATE(executeImported) = executeImported;

    /* Compile the AST */
    compileRoot( ctxt, PRIVATE(parse_result) );

	PRIVATE(execute)         = FALSE;
	PRIVATE(executeImported) = FALSE;

    /* Print the AST */
#if EC_VERBOSE_COMPILATION
	printf( "\nAbstract Syntax Tree:\n" );
    ASTPrint( 0, PRIVATE(parse_result) );
	printf( "\n\n" );
#endif

    /* Close the global scope */
/*    closeScope( PRIVATE(globalScope) );*/
    PRIVATE(globalScope)  = NULL;
    PRIVATE(currentScope) = NULL;

    return PRIVATE(mainTarget);

error:
    if (bufState) yy_delete_buffer( bufState );
    if (fh)       fclose( fh );
    return Ec_ERROR;
}

EC_API EC_OBJ EcCompileString( ec_compiler_ctxt ctxt,
							   const char *sourcestring,
							   EcBool execute, EcBool executeImported,
							   ec_compiler_options *opts )
{
	extern struct yy_buffer_state *yy_scan_string( const char *yy_str );

    struct yy_buffer_state *bufState = NULL;

	/* memorize compiler options in context */
	opts_copy( &ctxt->opts, opts );

	/* ==================================================
	 * 1.  P A R S E
	 * ================================================== */

    if (! sourcestring) goto error;

	PRIVATE(fileSource) = NULL;
	PRIVATE(fileOutput) = (opts && opts->outputfile) ? ec_stringdup( opts->outputfile ) : NULL;

	PRIVATE(line) = 1; PRIVATE(column) = 0; PRIVATE(charnum) = 0;
	PRIVATE(parse_result) = NULL;

#ifndef YY_BUF_SIZE
#define YY_BUF_SIZE 16384
#endif

    bufState = yy_scan_string( sourcestring );
    if (! bufState) goto error;
    yy_switch_to_buffer( bufState );

    /* Parse, building the AST */
    if (yyparse() != 0)
        goto error;

    yy_delete_buffer( bufState );

	/* ==================================================
	 * 2.  C O M P I L E
	 * ================================================== */

	PRIVATE(execute)         = execute;
	PRIVATE(executeImported) = executeImported;

    /* Compile the AST */
	if ((! CCTXT(opts).whole_package) && EC_NNULLP(CCTXT(opts).in_package))
	{
		Scope   pkgScope;
		ASTNode anonFuncNode;

		pkgScope = buildScopeForPackage( ctxt, CCTXT(opts).in_package );
		CCTXT(currentScope)    = pkgScope;
		CCTXT(currentPackage)  = CCTXT(opts).in_package;
		CCTXT(package_package) = CCTXT(opts).in_package;

		anonFuncNode = makeFunction( NULL, NULL, NULL, PRIVATE(parse_result) );
		compileRoot( ctxt, anonFuncNode );

		closeScope( ctxt, pkgScope );
	}
	else
	{
		CCTXT(currentPackage) = CCTXT(opts).in_package;

		compileRoot( ctxt, PRIVATE(parse_result) );
	}

	PRIVATE(execute)         = FALSE;
	PRIVATE(executeImported) = FALSE;

    /* Print the AST */
#if EC_VERBOSE_COMPILATION
	printf( "\nAbstract Syntax Tree:\n" );
    ASTPrint( 0, PRIVATE(parse_result) );
	printf( "\n\n" );
#endif

    /* Close the global scope */
/*    closeScope( PRIVATE(globalScope) );*/
    PRIVATE(globalScope)  = NULL;
    PRIVATE(currentScope) = NULL;

    return CCTXT(compilationResult);

error:
    if (bufState) yy_delete_buffer( bufState );
    return Ec_ERROR;
}


/* ========================================================================
 * C O M P I L E R   -   Code Generators
 * ======================================================================== */

/*
 * compileRoot
 *
 * This function is the main entry point for the compilation process.
 * It is responsible of performing the various passes (currently 2).
 */
static void compileRoot( ec_compiler_ctxt ctxt, ASTNode node )
{
	ec_list_iterator iter;
	ec_list_node     pnode;
	EC_OBJ           package;

	ASSERT( node );

	/* reset compiler context */
	CCTXT(compilationResult) = EC_NIL;
	CCTXT(asLValue)      = FALSE;
	CCTXT(hasReturned)   = FALSE;
	CCTXT(discardResult) = TRUE;
	if (CCTXT(compiled_packages))
		ec_list_destroy( CCTXT(compiled_packages) );
	CCTXT(compiled_packages) = ec_list_create();
	ASSERT( CCTXT(compiled_packages) );
	if (! CCTXT(compiled_packages)) return;						/* :TODO: XXX argh ! */

	/*
	 * PASS 1
	 *
	 * Generate code
	 */
	compile( ctxt, node );

	if (CCTXT(opts).whole_package)
	{
		if (EC_NNULLP(CCTXT(package_package)))
			compilePackage_post( ctxt );
	}

	/*
	 * PASS 2
	 *
	 * Resolve jumps
	 */
	resolve( ctxt );

	/* Save generated packages if requested */
	if (CCTXT(opts).whole_package && CCTXT(opts).save)
	{
		iter = ec_list_iterator_create( CCTXT(compiled_packages) );
		ASSERT( iter );
		if (! iter) return;										/* :TODO: XXX argh ! */
		while ((pnode = ec_list_iterator_next( iter )))
		{
			package = (EC_OBJ) ec_list_data( pnode );

			ASSERT( EC_PACKAGEP(package) );
			EcPackageSave( package, PRIVATE(fileOutput) );
			EC_CHECK(package);
			EC_CHECKALL();
		}
		ec_list_iterator_destroy( iter );
	}

#if 0 /* cleaned-up by EcCompilerContextDestroy() */
	ec_list_destroy( CCTXT(compiled_packages) );
	CCTXT(compiled_packages) = NULL;
#endif
}

/* resolve jumps, using backpatching */
static void resolve( ec_compiler_ctxt ctxt )
{
	ec_list          compiled_bytecode, referenced, refs;
	ec_list_iterator bytecode_iter, referenced_iter, loc_iter;
	ec_list_node     bnode, rnode, lnode;
	EC_OBJ           bytecode;
	EcUInt           labelid;
	EcInt            pos, loc;

	compiled_bytecode = produced_bytecode( ctxt );
	if (! compiled_bytecode) return;

	bytecode_iter = ec_list_iterator_create( compiled_bytecode );
	while ((bnode = ec_list_iterator_next( bytecode_iter )))
	{
		bytecode = ec_list_data( bnode );
		ASSERT( EC_COMPILEDP(bytecode) );

		/*ec_fprintf( stderr, "RESOLVING ON BYTECODE: %w 0x%08lX\n", bytecode, (unsigned long)bytecode );*/

		referenced = referenced_labels( ctxt, bytecode );		/* labelinfo list */
		if (! referenced) continue;
		/*fprintf( stderr, "HAS REFERENCED LABELS\n" ); fflush( stderr );*/
		referenced_iter = ec_list_iterator_create( referenced );
		while ((rnode = ec_list_iterator_next( referenced_iter )))
		{
			labelid = (EcUInt)(EcPointerInteger) ec_list_key( rnode );
			pos     = label_position( ctxt, bytecode, labelid );
			if (pos == -1)
			{
				EcAlert( EcWarning, "unused label %k", labelid );
				continue;
			}
			ASSERT( EC_COMPILEDNCODE(bytecode) > pos );

			/*ec_fprintf( stderr, "RESOLVING LABEL %k (pos: %ld)\n", labelid, (long)pos );*/

			refs = label_references( ctxt, labelid, bytecode );	/* LI_REF(linfo) */
			if (! refs)
			{
				EcAlert( EcWarning, "unused label %k", labelid );
				continue;
			}
			loc_iter = ec_list_iterator_create( refs );
			while ((lnode = ec_list_iterator_next( loc_iter )))
			{
				loc = (EcUInt)(EcPointerInteger) ec_list_data( lnode );
				ASSERT( loc >= 0 );

				/*ec_fprintf( stderr, "    BACKPATCHING at loc: %ld  pos: %ld\n", (long)loc, (long)pos );*/

				/* backpatch */
				ASSERT( EC_COMPILEDNCODE(bytecode) > loc );
				EC_COMPILEDCODE(bytecode)[loc] = (EcBytecode) pos;
			}
			ec_list_iterator_destroy( loc_iter );
			ec_list_destroy( refs );
		}
		ec_list_iterator_destroy( referenced_iter );
		ec_list_destroy( referenced );
	}
	ec_list_iterator_destroy( bytecode_iter );

	ec_list_destroy( compiled_bytecode );
	labels_cleanup( ctxt );
}

/*
 * compile
 *
 * First pass compilation entry point
 */
static void compile( ec_compiler_ctxt ctxt, ASTNode node )
{
	if (! node) return;

	/* create a default package if we don't have one */
	if (EC_NULLP(CCTXT(package_package)) &&
		(node->type != packageNodeType) &&
		(node->type != stmtListType))
	{
		/*
		 * the source code didn't introduce a package, so we surely
		 * don't want to save the anonymous one introduced here
		 * if the user didn't specify explicitly an output filename.
		 */
		if (CCTXT(opts).outputfile == NULL)
			CCTXT(opts).save = FALSE;
		compilePackage( ctxt, makePackage( makeQualifiedSymbol( NULL, makeSymbol( EcInternSymbol( DEFAULT_PACKAGENAME ) ) ) ) );
		ASSERT( ! EC_NULLP(CCTXT(package_package)) );
	}

	switch( node->type )
	{
	case nullType:
		ASSERT( FALSE );
		break;

	case symbolType:
		ASSERT( FALSE );
		break;

	case qualifiedSymbolType:
		ASSERT( FALSE );
		break;

	case constExprType:
		compileConstant( ctxt, node );
		break;

	case variableExprType:
		compileVariable( ctxt, node );
		break;

	case arrayConstructionExprType:
		compileArrayCons( ctxt, node );
		break;

	case hashConstructionExprType:
		compileHashCons( ctxt, node );
		break;

	case unaryExprType:
		compileUnary( ctxt, node );
		break;

	case binaryExprType:
		compileBinary( ctxt, node );
		break;

	case conditionalExprType:
		compileConditional( ctxt, node );
		break;

	case orExprType:
		compileOr( ctxt, node );
		break;

	case andExprType:
		compileAnd( ctxt, node );
		break;

	case assignExprType:
		compileAssign( ctxt, node );
		break;

	case simAssignExprType:
		compileSimAssign( ctxt, node );
		break;

	case arrayRefExprType:
		compileArrayRef( ctxt, node );
		break;

	case declNodeType:
		compileDecl( ctxt, node );
		break;

	case declAtomNodeType:
		ASSERT( FALSE );
		break;

	case statementType:
		compileStatement( ctxt, node );
		break;

	case labeledStmtType:
		compileLabeledStmt( ctxt, node );
		break;

	case exprStmtType:
		compileExpr( ctxt, node );
		break;

	case ifStmtType:
		compileIf( ctxt, node );
		break;

	case whileStmtType:
		compileWhile( ctxt, node );
		break;

	case doStmtType:
		compileDo( ctxt, node );
		break;

	case forStmtType:
		compileFor( ctxt, node );
		break;

	case forInStmtType:
		compileForIn( ctxt, node );
		break;

	case breakStmtType:
		compileBreak( ctxt, node );
		break;

	case continueStmtType:
		compileContinue( ctxt, node );
		break;

	case gotoStmtType:
		compileGoto( ctxt, node );
		break;

	case tryStmtType:
		compileTry( ctxt, node );
		break;

	case catchStmtType:
		ASSERT( FALSE );
		break;

	case throwStmtType:
		compileThrow( ctxt, node );
		break;

	case importStmtType:
		compileImport( ctxt, node );
		break;

	case paramNodeType:
		ASSERT( FALSE );
		break;

	case paramListType:
		ASSERT( FALSE );
		break;

	case callNodeType:
		compileCall( ctxt, node );
		break;

	case methodCallNodeType:
		compileMethodCall( ctxt, node );
		break;

	case stmtListType:
		compileStmtList( ctxt, node );
		break;

	case funcNodeType:
		compileFunction( ctxt, node );
		break;

	case returnNodeType:
		compileReturn( ctxt, node );
		break;

	case packageNodeType:
		compilePackage( ctxt, node );
		break;

	case classNodeType:
		compileClass( ctxt, node );
		break;

	case methodNodeType:
		compileMethod( ctxt, node );
		break;
	}
}

static void compileConstant( ec_compiler_ctxt ctxt, ASTNode node )
{
	EcVarRefStruct constinfo;
	EcInt pos;
	EC_OBJ obj = EC_NIL;

	/* Make constant object */
	switch (node->vConstExpr.type)
	{
	case inumberConst:
		obj = EcMakeInt( node->vConstExpr.vInt );
		break;
	case fnumberConst:
		obj = EcMakeFloat( node->vConstExpr.vFloat );
		break;
	case charConst:
		obj = EcMakeChar( node->vConstExpr.vChar );
		break;
	case symbolConst:
		obj = EcMakeSymbol( EcSymbolAt( node->vConstExpr.vSym ) );
		break;
	case stringConst:
		obj = EcMakeString( node->vConstExpr.vString, 0 );
		break;
	case objectConst:
		obj = node->vConstExpr.vObject;
		break;
	}

	ASSERT( ! EC_ERRORP(obj) );

	/* Push constant object in literal frame */
	pos = EcArrayPush( CCTXT(currentLiteral), obj );

	/* Emit a push of the constant */
	initVInfo( ctxt, &constinfo );
	V_CLASS(&constinfo) = SymbolStatic;
	V_UP(&constinfo)    = 0;
	V_POS(&constinfo)   = pos;
	emitLoad( ctxt, CCTXT(currentScope), &constinfo );
	if (node->vConstExpr.type == stringConst)
	{
		/*
		 * string constants are special in that they require a copy
		 * construction, since they are modifiable (think of a loop
		 * like this:
		 *
		 * for (i = 0; i < 5; i++) {
		 *   a = "Hello World";
		 *   a[i] = '*';
		 * }
		 *
		 * The result would be that the constant string "Hello world" would
		 * be modified in place.
		 */
		emit1( ctxt, CopyOP, (EcInt) EcShallowCopyType );
	}
}

static void compileVariable( ec_compiler_ctxt ctxt, ASTNode node )
{
	EcVarRefStruct vinfo;

	/* Get the location of the variable */
	if (! getSymbol( ctxt, CCTXT(currentScope), QSYM(node), &vinfo ))	/* Search in compile-time env.  */
	{
		char *name;

		name = EcQualifiedString( QSYM(node) );
		EcCompileError( FLINE(node), FCOL(node), "variable not defined: `%s'", name );
		EcAlert( EcError,
				 "(LINE: %ld, COLUMN: %ld) variable not defined: `%s'",
				 FLINE(node), FCOL(node), name );
		ec_free( name );
	}

	ASSERT( V_POS(&vinfo) != -1 );

	if (! CCTXT(asLValue))										/* R-value */
	{
#if 0
		/* TODO: Check for initialized variables is wrong: it doesn't take into account different possible paths
		 *       of execution
		 */
		if (! V_INITIALIZED(&vinfo))
		{
			char *name;

			name = EcQualifiedString( QSYM(node) );
			EcCompileError( FLINE(node), FCOL(node), "reference to uninitialized variable `%s'", name );
			EcAlert( EcError,
					 "(LINE: %ld, COLUMN: %ld) reference to uninitialized variable `%s'",
					 FLINE(node), FCOL(node), name );
			ec_free( name );
		}
#endif

		/* Emit a push of the variable value */
		emitLoad( ctxt, CCTXT(currentScope), &vinfo );
	} else														/* L-value */
		saveVInfo( ctxt, &CCTXT(lValue), &vinfo );
}

static void compileArrayCons( ec_compiler_ctxt ctxt, ASTNode node )
{
	ASTNode argList;
	EcInt nargs;

	/* Count number of arguments
	 *
	 * Note that we must count the number of arguments, since
	 * the callee, unknown until run-time, must know the number
	 * of objects to fetch from the stack frame.
	 */
	nargs = 0;
	argList = node->vArrayConsExpr.arglist;
	while (argList) {
		nargs++;
		argList = argList->vStmtList.next;
	}

	/* Compile arguments */
	argList = node->vArrayConsExpr.arglist;
	while (argList) {
		ASSERT( argList->vStmtList.stmt );
		compile( ctxt, argList->vStmtList.stmt );

		argList = argList->vStmtList.next;
	}

	emit1( ctxt, ArrayConsOP, nargs );
}

static void compileHashCons( ec_compiler_ctxt ctxt, ASTNode node )
{
	ASTNode argList;
	EcInt nargs;

	/* Count number of arguments
	 *
	 * Note that we must count the number of arguments, since
	 * the callee, unknown until run-time, must know the number
	 * of objects to fetch from the stack frame.
	 */
	nargs = 0;
	argList = node->vHashConsExpr.arglist;
	while (argList) {
		nargs++;
		argList = argList->vStmtList.next;
	}

	/* Compile arguments */
	argList = node->vHashConsExpr.arglist;
	while (argList) {
		ASSERT( argList->vStmtList.stmt );
		compile( ctxt, argList->vStmtList.stmt );

		argList = argList->vStmtList.next;
	}

	emit1( ctxt, HashConsOP, nargs );
}

static void compileUnary( ec_compiler_ctxt ctxt, ASTNode node )
{
	EcVarRefStruct vinfo;

	switch (node->vUnaryExpr.op)
	{
	case PREINC_OP:
	case PREDEC_OP:
		ASSERT( CCTXT(asLValue) == FALSE );

		/* As r-value now */
		compile( ctxt, node->vUnaryExpr.operand );

		switch (node->vUnaryExpr.op)
		{
		case PREINC_OP:
			emit0( ctxt, IncOP );
			break;

		case PREDEC_OP:
			emit0( ctxt, DecOP );
			break;

		default:
			/* NOTHING */
			ASSERT( FALSE );
			break;
		}

		/* As l-value this time */
		CCTXT(asLValue) = TRUE;
		compile( ctxt, node->vUnaryExpr.operand );
		CCTXT(asLValue) = FALSE;

		/* Now we have lhs position in lValue */
		saveVInfo( ctxt, &vinfo, &CCTXT(lValue) );

		if (! V_INDEXED(&vinfo))
			emit0( ctxt, DupOP );

		if (V_CONST(&vinfo) && V_INITIALIZED(&vinfo))
		{
			EcCompileError( FLINE(node), FCOL(node), "left side of assignment is constant" );
			EcAlert( EcError,
					 "(LINE: %ld, COLUMN: %ld) left side of assignment is constant",
					 FLINE(node), FCOL(node) );
		}

		if (V_INDEXED(&vinfo))
		{
			/* Get the value to store */
			emit1( ctxt, GetOP, 2 );
		}

		/* Save the computed value in variable location */
		emitSave( ctxt, CCTXT(currentScope), &vinfo );

		if (V_INDEXED(&vinfo))
		{
			/* Discard new value */
			emit0( ctxt, DiscardOP );
		}
		break;

	case POSTINC_OP:
	case POSTDEC_OP:
		ASSERT( CCTXT(asLValue) == FALSE );

		/* As r-value now */
		compile( ctxt, node->vUnaryExpr.operand );

		/* As l-value this time */
		CCTXT(asLValue) = TRUE;
		compile( ctxt, node->vUnaryExpr.operand );
		CCTXT(asLValue) = FALSE;

		/* Now we have lhs position in lValue */
		saveVInfo( ctxt, &vinfo, &CCTXT(lValue) );

		if (V_CONST(&vinfo) && V_INITIALIZED(&vinfo))
		{
			EcCompileError( FLINE(node), FCOL(node), "left side of assignment is constant" );
			EcAlert( EcError,
					 "(LINE: %ld, COLUMN: %ld) left side of assignment is constant",
					 FLINE(node), FCOL(node) );
		}

		/* Postfix operation: duplicate before the operation */
		if (! V_INDEXED(&vinfo))
			emit0( ctxt, DupOP );
		else
			emit1( ctxt, GetOP, 2 );

		switch (node->vUnaryExpr.op)
		{
		case POSTINC_OP:
			emit0( ctxt, IncOP );
			break;

		case POSTDEC_OP:
			emit0( ctxt, DecOP );
			break;

		default:
			/* NOTHING */
			ASSERT( FALSE );
			break;
		}

		/* Save the computed value in variable location */
		emitSave( ctxt, CCTXT(currentScope), &vinfo );

		/*
		 * Since SequenceSetOP push on the stack the value, a DiscardOP is necessary in case
		 * of an array based assignment to eliminate the new value from stack.
		 */
		if (V_INDEXED(&vinfo))
		{
			/* Discard new value */
			emit0( ctxt, DiscardOP );
		}
		break;

	case NEG_OP:
	case POS_OP:
	case INVERT_OP:
	case LNOT_OP:
		compile( ctxt, node->vUnaryExpr.operand );

		switch (node->vUnaryExpr.op)
		{
		case NEG_OP:
			emit0( ctxt, NegOP );
			break;

		case POS_OP:
			emit0( ctxt, PosOP );
			break;

		case INVERT_OP:
			emit0( ctxt, InvertOP );
			break;

		case LNOT_OP:
			emit0( ctxt, LNotOP );
			break;

		default:
			/* NOTHING */
			break;
		}
		break;
	}
}

static void compileBinary( ec_compiler_ctxt ctxt, ASTNode node )
{
	compile( ctxt, node->vBinaryExpr.first );
	compile( ctxt, node->vBinaryExpr.second );

	switch (node->vBinaryExpr.op)
	{
	case ADD_OP:
		emit0( ctxt, AddOP );
		break;

	case SUB_OP:
		emit0( ctxt, SubOP );
		break;

	case MUL_OP:
		emit0( ctxt, MulOP );
		break;

	case DIV_OP:
		emit0( ctxt, DivOP );
		break;

	case MOD_OP:
		emit0( ctxt, ModOP );
		break;

	case POW_OP:
		emit0( ctxt, PowOP );
		break;

	case LSHIFT_OP:
		emit0( ctxt, LShiftOP );
		break;

	case RSHIFT_OP:
		emit0( ctxt, RShiftOP );
		break;

	case AND_OP:
		emit0( ctxt, AndOP );
		break;

	case OR_OP:
		emit0( ctxt, OrOP );
		break;

	case XOR_OP:
		emit0( ctxt, XorOP );
		break;

	case IN_OP:
		emit0( ctxt, InOP );
		break;

	case LT_OP:
		emit0( ctxt, LtOP );
		break;

	case GT_OP:
		emit0( ctxt, GtOP );
		break;

	case LE_OP:
		emit0( ctxt, LeOP );
		break;

	case GE_OP:
		emit0( ctxt, GeOP );
		break;

	case EQ_OP:
		emit0( ctxt, EqOP );
		break;

	case NE_OP:
		emit0( ctxt, NeOP );
		break;
	}
}

static void compileConditional( ec_compiler_ctxt ctxt, ASTNode node )
{
	EcInt truePatch, jumpPatch, truePos, finish;

	compile( ctxt, node->vCondExpr.cond );

	/* Branch if TRUE (address to patch lately) */
	truePatch = emit1( ctxt, BranchIfTrueOP, 0x0000 );
	truePatch++;                                         /* First operand of instruction */

	compile( ctxt, node->vCondExpr.fexpr );
	jumpPatch = emit1( ctxt, JumpOP, 0x0000 );
	jumpPatch++;

	truePos = nextPosition( ctxt );
	compile( ctxt, node->vCondExpr.texpr );

	finish = nextPosition( ctxt );

	/* Patch true branch */
	patch( ctxt, truePatch, truePos );

	/* Patch jump */
	patch( ctxt, jumpPatch, finish );
}

static void compileOr( ec_compiler_ctxt ctxt, ASTNode node )
{
	EcInt pos, finish;

	/* SHORT CIRCUIT EVALUATION */

	/* Evaluate the first expression */
	compile( ctxt, node->vOrExpr.first );

	/* Duplicate value on the stack */
	emit0( ctxt, DupOP );

	/* Branch if TRUE (address to patch lately) */
	pos = emit1( ctxt, BranchIfTrueOP, 0x0000 );
	pos++;                                         /* First operand of instruction */

	/* Discard the old value on the stack */
	emit0( ctxt, DiscardOP );

	/* Evaluate the second expression */
	compile( ctxt, node->vOrExpr.second );

	/* Patch old branch */
	finish = nextPosition( ctxt );
	patch( ctxt, pos, finish );
}

static void compileAnd( ec_compiler_ctxt ctxt, ASTNode node )
{
	EcInt pos, finish;

	/* SHORT CIRCUIT EVALUATION */

	/* Evaluate the first expression */
	compile( ctxt, node->vAndExpr.first );

	/* Duplicate value on the stack */
	emit0( ctxt, DupOP );

	/* Branch if FALSE (address to patch lately) */
	pos = emit1( ctxt, BranchIfFalseOP, 0x0000 );
	pos++;                                         /* First operand of instruction */

	/* Discard the old value on the stack */
	emit0( ctxt, DiscardOP );

	/* Evaluate the second expression */
	compile( ctxt, node->vAndExpr.second );

	/* Patch old branch */
	finish = nextPosition( ctxt );
	patch( ctxt, pos, finish );
}

static void compileAssign( ec_compiler_ctxt ctxt, ASTNode node )
{
	EcVarRefStruct vinfo;

	/* Compile the left hand side as an l-value */
	CCTXT(asLValue) = TRUE;
	compile( ctxt, node->vAssignExpr.lhs );
	CCTXT(asLValue) = FALSE;

	/* Now we have lhs position in lValue */
	saveVInfo( ctxt, &vinfo, &CCTXT(lValue) );

	if (V_CONST(&vinfo) && V_INITIALIZED(&vinfo))
	{
		EcCompileError( FLINE(node), FCOL(node), "left side of assignment is constant" );
		EcAlert( EcError,
				 "(LINE: %ld, COLUMN: %ld) left side of assignment is constant",
				 FLINE(node), FCOL(node) );
	}

	/* Compile the right hand side (rhs) */
	compile( ctxt, node->vAssignExpr.rhs );

	/*
	 * Since SequenceSetOP push on the stack the value, a DupOP is not necessary in case
	 * of an array based assignment.
	 */
	if (! V_INDEXED(&vinfo))
	{
		/* Duplicate the value */
		emit0( ctxt, DupOP );
	}

	/* Save the computed value in variable location */
	emitSave( ctxt, CCTXT(currentScope), &vinfo );
}

static void compileSimAssign( ec_compiler_ctxt ctxt, ASTNode node )
{
	ASTNode lhs_list, rhs_list;
	ASTNode lhs, rhs;
	EcInt   n_lvalues, n_rvalues;
	EcInt   i;

	EcVarRefStruct *vinfo;

	if (node->vSimAssignExpr.rhs &&
		(node->vSimAssignExpr.rhs->type == stmtListType))
	{
		/*
		 * [lvalue1, ..., lvalueN] = [expr1, ... exprN]
		 */

		lhs_list = node->vSimAssignExpr.lhs;
		rhs_list = node->vSimAssignExpr.rhs;

		n_lvalues = 0;
		n_rvalues = 0;

		while (lhs_list)
		{
			n_lvalues++;
			lhs_list = lhs_list->vStmtList.next;
		}

		while (rhs_list)
		{
			n_rvalues++;
			rhs_list = rhs_list->vStmtList.next;
		}

		/* fprintf(stderr, "n_lvalues: %ld\n", (long)n_lvalues); */
		if (n_lvalues != n_rvalues)
		{
			EcCompileError( FLINE(node), FCOL(node), "in multiple assignment there is a different number of elements on left and right" );
			EcAlert( EcError, "(LINE: %ld, COLUMN: %ld) in multiple assignment there is a different number of elements on left and right",
					 FLINE(node), FCOL(node) );
		}

		vinfo = ec_malloc( sizeof(EcVarRefStruct) * n_lvalues );
		ASSERT( vinfo );
		if (! vinfo)
		{
			/* TODO */
			EcCompileError( FLINE(node), FCOL(node), "out of memory" );
			EcAlert( EcError, "(LINE: %ld, COLUMN: %ld) out of memory", FLINE(node), FCOL(node) );
		}

		/* Compile the left hand sides as l-values */
		lhs_list = node->vSimAssignExpr.lhs;
		i = 0;
		while (lhs_list)
		{
			lhs = lhs_list->vStmtList.stmt;

			CCTXT(asLValue) = TRUE;
			/* fprintf(stderr, "Compiling LHS %ld\n", (long)i);
			   ASTPrint( 0, lhs );
			   printf("\n");*/
			compile( ctxt, lhs );
			CCTXT(asLValue) = FALSE;

			/* Now we have lhs position in lValue */
			saveVInfo( ctxt, &vinfo[i], &CCTXT(lValue) );

			if (V_CONST(&vinfo[i]) && V_INITIALIZED(&vinfo[i]))
			{
				EcCompileError( FLINE(node), FCOL(node), "left side member %ld in assignment is constant", (long)i );
				EcAlert( EcError,
						 "(LINE: %ld, COLUMN: %ld) left side member %ld in assignment is constant",
						 FLINE(node), FCOL(node), (long)i );
			}

			i++;
			lhs_list = lhs_list->vStmtList.next;
		}
		/* fprintf(stderr, "Compiling LHSs done\n"); */

		/*
		 * The first rhs is the whole expression value
		 */

		/* Compile the right hand sides (rhs) */
		rhs_list = node->vSimAssignExpr.rhs;
		i = 0;
		while (rhs_list)
		{
			rhs = rhs_list->vStmtList.stmt;

			/* fprintf(stderr, "Compiling RHS %ld\n", (long)i);
			   ASTPrint( 0, rhs );
			   printf("\n"); */
			compile( ctxt, rhs );

			i++;
			rhs_list = rhs_list->vStmtList.next;
		}
		/* fprintf(stderr, "Compiling RHSs done\n"); */

		/* Save the right hand sides (rhs) in reverse order */
		for (i = n_lvalues - 1; i >= 0; i--)
		{
			/* fprintf(stderr, "Saving RHS %ld\n", (long)i); */

			/*
			 * The first rhs is the whole expression value.
			 */
			if (! V_INDEXED(&vinfo[i]) && (i == 0))
			{
				/* Duplicate the value */
				emit0( ctxt, DupOP );
			}

			/* Save the computed value in variable location */
			emitSave( ctxt, CCTXT(currentScope), &vinfo[i] );

			/*
			 * Since SequenceSetOP push on the stack the value, we'll have to discard it
			 * if this is not the first expression.
			 */
			if (V_INDEXED(&vinfo[i]) && (i != 0))
			{
				/* Discard the value */
				emit0( ctxt, DiscardOP );
			}
		}
		/* fprintf(stderr, "Saving done\n"); */
	} else
	{
		/*
		 * [lvalue1, ..., lvalueN] = expression
		 */
		lhs_list = node->vSimAssignExpr.lhs;
		rhs      = node->vSimAssignExpr.rhs;

		n_lvalues = 0;

		while (lhs_list)
		{
			n_lvalues++;
			lhs_list = lhs_list->vStmtList.next;
		}

		/* fprintf(stderr, "n_lvalues: %ld\n", (long)n_lvalues); */

		vinfo = ec_malloc( sizeof(EcVarRefStruct) * n_lvalues );
		ASSERT( vinfo );
		if (! vinfo)
		{
			/* TODO */
			EcCompileError( FLINE(node), FCOL(node), "out of memory" );
			EcAlert( EcError, "(LINE: %ld, COLUMN: %ld) out of memory", FLINE(node), FCOL(node) );
		}

		/* Compile the left hand sides as l-values */
		lhs_list = node->vSimAssignExpr.lhs;
		i = 0;
		while (lhs_list)
		{
			lhs = lhs_list->vStmtList.stmt;

			CCTXT(asLValue) = TRUE;
			/* fprintf(stderr, "Compiling LHS %ld\n", (long)i);
			   ASTPrint( 0, lhs );
			   printf("\n");*/
			compile( ctxt, lhs );
			CCTXT(asLValue) = FALSE;

			/* Now we have lhs position in lValue */
			saveVInfo( ctxt, &vinfo[i], &CCTXT(lValue) );

			if (V_CONST(&vinfo[i]) && V_INITIALIZED(&vinfo[i]))
			{
				EcCompileError( FLINE(node), FCOL(node), "left side member %ld in assignment is constant", (long)i );
				EcAlert( EcError,
						 "(LINE: %ld, COLUMN: %ld) left side member %ld in assignment is constant",
						 FLINE(node), FCOL(node), (long)i );
			}

			i++;
			lhs_list = lhs_list->vStmtList.next;
		}
		/* fprintf(stderr, "Compiling LHSs done\n"); */

		/*
		 * The rhs is the whole expression value
		 */

		/* Compile the right hand side (rhs) */
		/* fprintf(stderr, "Compiling RHS\n");
		   ASTPrint( 0, rhs );
		   printf("\n"); */
		compile( ctxt, rhs );

		/* fprintf(stderr, "Compiling RHS done\n"); */

		/* Cycle through right hand side elements (rhs) in reverse order, saving them */
		for (i = n_lvalues - 1; i >= 0; i--)
		{
			EcVarRefStruct constinfo;
			EcInt idx_pos;
			EC_OBJ idx_obj = EC_NIL;

			/* fprintf(stderr, "Saving RHS %ld\n", (long)i); */

			/* 1. Compile indexing operation */

			/* 1.1. Compile referenced array */
			emit0( ctxt, DupOP );

			/* 1.2. Compile index */
			idx_obj = EcMakeInt( i );
			/* Push constant object in literal frame */
			idx_pos = EcArrayPush( CCTXT(currentLiteral), idx_obj );
			/* Emit a push of the constant */
			initVInfo( ctxt, &constinfo );
			V_CLASS(&constinfo) = SymbolStatic;
			V_UP(&constinfo)    = 0;
			V_POS(&constinfo)   = idx_pos;
			emitLoad( ctxt, CCTXT(currentScope), &constinfo );

			/* 1.2. Compile array reference */
			emit0( ctxt, SequenceGetOP );

			/* Save the computed value in variable location */
			emitSave( ctxt, CCTXT(currentScope), &vinfo[i] );
		}
		/* fprintf(stderr, "Saving done\n"); */
	}
}

static void compileArrayRef( ec_compiler_ctxt ctxt, ASTNode node )
{
	if (! CCTXT(asLValue))										/* R-value */
	{
		/* Compile the referenced expression */
		compile( ctxt, node->vArrayRefExpr.ref );

		/* Compile the index expression */
		compile( ctxt, node->vArrayRefExpr.idx );

		/* Do the de-referencing */
		emit0( ctxt, SequenceGetOP );

		V_INDEXED(&CCTXT(lValue)) = FALSE;
	} else														/* L-value */
	{
		/*
		 * Array based L-value.
		 * We must save a value in a slot of an array based object.
		 * So we must compile the array object and index as a normal rhs,
		 * then, later, emit a SequenceSetOP.
		 * Say to the world we are doing an array based save.
		 */

		CCTXT(asLValue) = FALSE;

		/* Compile the referenced expression */
		compile( ctxt, node->vArrayRefExpr.ref );

		/* Compile the index expression */
		compile( ctxt, node->vArrayRefExpr.idx );

		CCTXT(asLValue) = TRUE;
		V_INDEXED(&CCTXT(lValue)) = TRUE;
	}
}

static void compileDecl( ec_compiler_ctxt ctxt, ASTNode node )
{
	Scope targetScope, master;
	ASTNode decllist, decl, init;
	EcUInt symid;

	targetScope = CCTXT(currentScope);

	master = getMasterScope( ctxt, CCTXT(currentScope) );

	switch (master->type)
	{
	case PackageScope:
		break;

	case FunctionScope:
		if (node->vDecl.symclass == SymbolPrivate)
		{
			EcCompileError( FLINE(node), FCOL(node), "can't declare private variables in function" );
			EcAlert( EcError,
					 "(LINE: %ld, COLUMN: %ld) can't declare private variables in function",
					 FLINE(node), FCOL(node) );
		}
		if (node->vDecl.symclass == SymbolPublic)
		{
			EcCompileError( FLINE(node), FCOL(node), "can't declare public variables in function" );
			EcAlert( EcError,
					 "(LINE: %ld, COLUMN: %ld) can't declare public variables in function",
					 FLINE(node), FCOL(node) );
		}
		break;

	case BlockScope:
		ASSERT( FALSE );
		break;

	case ClassScope:
		if (node->vDecl.symclass == SymbolPrivate)
		{
			EcCompileError( FLINE(node), FCOL(node), "can't declare private variables in class" );
			EcAlert( EcError, "(LINE: %ld, COLUMN: %ld) can't declare private variables in class", FLINE(node), FCOL(node) );
		}
		if (node->vDecl.symclass == SymbolPublic)
		{
			EcCompileError( FLINE(node), FCOL(node), "can't declare public variables in class" );
			EcAlert( EcError, "(LINE: %ld, COLUMN: %ld) can't declare public variables in class", FLINE(node), FCOL(node) );
		}
		break;
	}

	decllist = node->vDecl.decllist;
	while (decllist)
	{
		decl  = decllist->vStmtList.stmt;
		symid = SYMID(decl->vDeclAtom.sym);
		init  = decl->vDeclAtom.init;

		/* Add symbol */

#if EC_DEBUG_COMPILATION
		fprintf( stderr, "compileDecl: `%s' (%ld)\n", EcSymbolAt( symid ), symid );
#endif
		if (! addSymbol( ctxt, targetScope, symid, node->vDecl.symclass, FALSE ))
		{
			EcCompileError( FLINE(node), FCOL(node), "can't add symbol `%k'", symid );
			EcAlert( EcError, "(LINE: %ld, COLUMN: %ld) can't add symbol `%k'", FLINE(node), FCOL(node), symid );
		}

		/* Initialize, in case... */
		if (init)
		{
			ASTNode varnode, assignnode;

			if ((master->type == FunctionScope) && (node->vDecl.symclass == SymbolStatic))
			{
				/*
				 * initialization for static variables in function bodies must
				 * be done only once.
				 */
				EcBool oldDiscardMode;
				EcInt definedPatch, definedPos;

				/* push (possibly uninitialized) variable on the stack */
				varnode = makeVariable( makeQualifiedSymbol( NULL, makeSymbol( symid ) ) );
				oldDiscardMode = CCTXT(discardResult);
				CCTXT(discardResult) = 0;
				compile( ctxt, makeExprStmt( varnode ) );
				CCTXT(discardResult) = oldDiscardMode;

				/* check if we have to initialize */
				definedPatch = emit1( ctxt, BranchIfDefinedOP, 0x0000 );
				definedPatch++;

				/* assign initialization value */
				assignnode = makeExprStmt( makeAssign( varnode, init ) );
				compile( ctxt, assignnode );

				/* Backpatch defined branch */
				definedPos = nextPosition( ctxt );
				patch( ctxt, definedPatch, definedPos );
			} else
			{
				varnode = makeVariable( makeQualifiedSymbol( NULL, makeSymbol( symid ) ) );
				assignnode = makeExprStmt( makeAssign( varnode, init ) );

				compile( ctxt, assignnode );
			}
		}

		decllist = decllist->vStmtList.next;
	}
}

static void compileStatement( ec_compiler_ctxt ctxt, ASTNode node )
{
	ASTNode stmt;
	EcBool  isblock;
	Scope   blockscope;

	ASSERT( FLINE(node) > 0 );

	SET_LINE(node);

	isblock    = FALSE;
	blockscope = NULL;

	stmt = node->vStmt.stmt;
	if (stmt)
	{
		if (stmt->type == stmtListType)
		{
			isblock    = TRUE;
			blockscope = openScope( ctxt, CCTXT(currentScope), EC_NIL,
									CCTXT(currentScope) ? CCTXT(currentScope)->functype : NormalFunction );

			ASSERT( CCTXT(currentScope) == blockscope );
		}

		compile( ctxt, node->vStmt.stmt );

		if (isblock)
		{
			closeScope( ctxt, blockscope );
		}
	}
}

static void compileLabeledStmt( ec_compiler_ctxt ctxt, ASTNode node )
{
	if (! define_label( ctxt, node->vLabeledStmt.label ))
	{
		EcAlert( EcError,
				 "(LINE: %ld, COLUMN: %ld) label `%k' already defined",
				 FLINE(node), FCOL(node), node->vLabeledStmt.label );
	}

	compileStatement( ctxt, node->vLabeledStmt.stmt );
}

static void compileExpr( ec_compiler_ctxt ctxt, ASTNode node )
{
	if (node->vExpr.expr)
	{
		/* Evaluate expression */
		compile( ctxt, node->vExpr.expr );

		if (CCTXT(discardResult))
		{
			/* Discard result */
			emit0( ctxt, DiscardOP );
		}
	}
}

static void compileIf( ec_compiler_ctxt ctxt, ASTNode node )
{
	EcInt elsePatch, jumpPatch, elsePos, finish;

	compile( ctxt, node->vIfStmt.cond );

	/* Branch if FALSE (address to patch lately) */
	elsePatch = emit1( ctxt, BranchIfFalseOP, 0x0000 );
	elsePatch++;                                         /* First operand of instruction */

	compile( ctxt, node->vIfStmt.thenClause );
	jumpPatch = emit1( ctxt, JumpOP, 0x0000 );
	jumpPatch++;

	elsePos = nextPosition( ctxt );
	if (node->vIfStmt.elseClause)
		compile( ctxt, node->vIfStmt.elseClause );

	finish = nextPosition( ctxt );

	/* Patch else branch */
	patch( ctxt, elsePatch, elsePos );

	/* Patch jump */
	patch( ctxt, jumpPatch, finish );
}

static void backpatch_break( ec_compiler_ctxt ctxt, Scope scope, EcInt finish )
{
	ec_stack *stk_pos, *stk_lev;
	EcUInt level;
	EcUInt *pos, *pos_s, *pos_e;
	EcUInt *lev;

#if EC_DEBUG_COMPILATION
	fprintf( stderr, "BACKPATCH BREAK   finish: %ld\n", finish );
#endif

	stk_pos = scope->break_pos;
	stk_lev = scope->break_lev;

	/* Are there some 'break' statements exiting at this level ? */
	level = CYCLE_LEVEL(scope);
	pos_s = stk_pos->head + 1;
	pos_e = stk_pos->tail;
	lev   = stk_lev->head + 1;
	for (pos = pos_s; pos <= pos_e; pos++, lev++)
	{
		if (*lev == level)										/* Patch this */
		{
			ASSERT( *lev != (EcUInt) -1);
#if EC_DEBUG_COMPILATION
			fprintf( stderr, "    PATCHING %ld  to %ld\n", *pos, finish );
#endif
			patch( ctxt, *pos, finish );

			/* Delete */
			*pos = (EcUInt) -1;
			*lev = (EcUInt) -1;
		}
	}
}

static void backpatch_continue( ec_compiler_ctxt ctxt, Scope scope, EcInt finish )
{
	ec_stack *stk_pos, *stk_lev;
	EcUInt level;
	EcUInt *pos, *pos_s, *pos_e;
	EcUInt *lev;

#if EC_DEBUG_COMPILATION
	fprintf( stderr, "BACKPATCH CONTINUE   finish: %ld\n", finish );
#endif

	stk_pos = scope->continue_pos;
	stk_lev = scope->continue_lev;

	/* Are there some 'continue' statements continuing at this level ? */
	level = CYCLE_LEVEL(scope);
	pos_s = stk_pos->head + 1;
	pos_e = stk_pos->tail;
	lev   = stk_lev->head + 1;
	for (pos = pos_s; pos <= pos_e; pos++, lev++)
	{
		if (*lev == level)										/* Patch this */
		{
			ASSERT( *lev != (EcUInt) -1);
#if EC_DEBUG_COMPILATION
			fprintf( stderr, "    PATCHING %ld  to %ld\n", *pos, finish );
#endif
			patch( ctxt, *pos, finish );

			/* Delete */
			*pos = (EcUInt) -1;
			*lev = (EcUInt) -1;
		}
	}
}

static void compileWhile( ec_compiler_ctxt ctxt, ASTNode node )
{
	EcInt exitPatch, startPos, finish;

	startPos = nextPosition( ctxt );
	compile( ctxt, node->vWhileStmt.cond );

	/* Branch if FALSE (address to patch lately) */
	exitPatch = emit1( ctxt, BranchIfFalseOP, 0x0000 );
	exitPatch++;												/* First operand of instruction */

	CYCLE_LEVEL(CCTXT(currentScope))++;
	compile( ctxt, node->vWhileStmt.body );
	CYCLE_LEVEL(CCTXT(currentScope))--;
	emit1( ctxt, JumpOP, startPos );

	/* Patch exit branch */
	finish = nextPosition( ctxt );
	patch( ctxt, exitPatch, finish );

	/* Backpatch 'continue' locations */
	backpatch_continue( ctxt, CCTXT(currentScope), startPos );

	/* Backpatch 'break' locations */
	backpatch_break( ctxt, CCTXT(currentScope), finish );
}

static void compileDo( ec_compiler_ctxt ctxt, ASTNode node )
{
	EcInt startPos, restartPos, finish;

	startPos = nextPosition( ctxt );

	CYCLE_LEVEL(CCTXT(currentScope))++;
	compile( ctxt, node->vDoStmt.body );
	CYCLE_LEVEL(CCTXT(currentScope))--;

	restartPos = nextPosition( ctxt );
	compile( ctxt, node->vDoStmt.cond );
	emit1( ctxt, BranchIfTrueOP, startPos );

	finish = nextPosition( ctxt );

	/* Backpatch 'continue' locations */
	backpatch_continue( ctxt, CCTXT(currentScope), restartPos );

	/* Backpatch 'break' locations */
	backpatch_break( ctxt, CCTXT(currentScope), finish );
}

static void compileFor( ec_compiler_ctxt ctxt, ASTNode node )
{
	Scope blockscope;
	EcInt exitPatch = -1, startPos = -1, restartPos = -1, finish = -1;

	blockscope = openScope( ctxt, CCTXT(currentScope), EC_NIL,
							CCTXT(currentScope) ? CCTXT(currentScope)->functype : NormalFunction );
	ASSERT( CCTXT(currentScope) == blockscope );

	if (node->vForStmt.init)
	{
		compile( ctxt, node->vForStmt.init );
/*		emit0( DiscardOP );	*/									/* an expression statement, discard the result */
	}

	startPos = nextPosition( ctxt );
	if (node->vForStmt.cond)
	{
		compile( ctxt, node->vForStmt.cond );

		/* Branch if FALSE (address to patch lately) */
		exitPatch = emit1( ctxt, BranchIfFalseOP, 0x0000 );
		exitPatch++;												/* First operand of instruction */
	} else
		exitPatch = -1;

	if (node->vForStmt.body)
	{
		CYCLE_LEVEL(CCTXT(currentScope))++;
		compile( ctxt, node->vForStmt.body );
		CYCLE_LEVEL(CCTXT(currentScope))--;
	}
	restartPos = nextPosition( ctxt );
	if (node->vForStmt.restart)
	{
		compile( ctxt, node->vForStmt.restart );
		/* Discard the value calculated from the restart */
		emit0( ctxt, DiscardOP );
	}
	emit1( ctxt, JumpOP, startPos );

	/* Patch exit branch */
	finish = nextPosition( ctxt );
	if (exitPatch >= 0) patch( ctxt, exitPatch, finish );

	/* Backpatch 'continue' locations */
	backpatch_continue( ctxt, CCTXT(currentScope), restartPos );

	/* Backpatch 'break' locations */
	backpatch_break( ctxt, CCTXT(currentScope), finish );

	closeScope( ctxt, blockscope );
}

static void compileForIn( ec_compiler_ctxt ctxt, ASTNode node )
{
	EcInt exitPatch, startPos, restartPos, finish;
	ASTNode idxvar;
	ASTNode init, initexpr;

	/* We need an hidden variable to keep the index */
	idxvar = getServiceVariable( ctxt, CCTXT(currentScope) );

	/* Intialization */
	initexpr = makeConstInt( 0 );
	init     = makeExprStmt( makeAssign( idxvar, initexpr ) );
	compile( ctxt, init );

	/* Condition */
	startPos = nextPosition( ctxt );
	compile( ctxt, idxvar );
	compile( ctxt, node->vForInStmt.sequence );
	compileNamedVariable( ctxt, "basic.length" );						/* Get sequence length */
	emit1( ctxt, CallOP, 1 );
	emit0( ctxt, LtOP );

	/* Branch if FALSE (address to patch lately) */
	exitPatch = emit1( ctxt, BranchIfFalseOP, 0x0000 );
	exitPatch++;												/* First operand of instruction */

	/* Compile setting of variable */
	compile( ctxt, makeExprStmt( makeAssign( node->vForInStmt.var, makeArrayRef( node->vForInStmt.sequence, idxvar ) ) ) );

	if (node->vForInStmt.body)
	{
		CYCLE_LEVEL(CCTXT(currentScope))++;
		compile( ctxt, node->vForInStmt.body );
		CYCLE_LEVEL(CCTXT(currentScope))--;
	}

	/* Restart */
	restartPos = nextPosition( ctxt );
	compile( ctxt, makeUnary( PREINC_OP, idxvar ) );
	/* Discard the value calculated from the restart */
	emit0( ctxt, DiscardOP );
	emit1( ctxt, JumpOP, startPos );

	/* Patch exit branch */
	finish = nextPosition( ctxt );
	patch( ctxt, exitPatch, finish );

	/* Backpatch 'continue' locations */
	backpatch_continue( ctxt, CCTXT(currentScope), restartPos );

	/* Backpatch 'break' locations */
	backpatch_break( ctxt, CCTXT(currentScope), finish );
}

static void compileTry( ec_compiler_ctxt ctxt, ASTNode node )
{
	ASTNode funcNode;
	EcInt pos;
	EC_OBJ func;
	EC_OBJ handler, handlers;
	EcVarRefStruct constinfo;

	ASTNode handlerList;

	/*
	 * Compile the inline code block
	 * (like an anonymous function, but with different calling/return convention)
	 */
	funcNode = makeFunction( NULL, NULL, NULL, node->vTryStmt.block );
	func = compileInlineCodeBlock( ctxt, funcNode );

	handlers = EC_COMPILEDHANDLER(func);
	ASSERT( EC_ARRAYP(handlers) );

	/* Compile the handler chain */
	handlerList = node->vTryStmt.catchlist;
	while (handlerList) {
		ASSERT( handlerList->vStmtList.stmt );
		handler = compileCatch( ctxt, handlerList->vStmtList.stmt );

		ASSERT( EC_HANDLERP(handler) );

		/* Add handler to code block handler chain */
#if EC_DEBUG_COMPILATION
		ec_fprintf( stderr, "Compiled CATCH (type: %w): %w\n", EC_HANDLERTYPE(handler), handler );
		EcDumpCompiled( EC_HANDLERCODE(handler), 0 );
		ec_fprintf( stderr, "*********************************\n\n" );
#endif
		EcArrayPush( handlers, handler );

		handlerList = handlerList->vStmtList.next;
	}

	/* Push constant object (compiled code) in literal frame */
	pos = EcArrayPush( CCTXT(currentLiteral), func );

	/* Emit a push of the constant */
	initVInfo( ctxt, &constinfo );
	V_CLASS(&constinfo) = SymbolStatic;
	V_UP(&constinfo)    = 0;
	V_POS(&constinfo)   = pos;
	emitLoad( ctxt, CCTXT(currentScope), &constinfo );

	/* Compile a call to the try block */
	emit1( ctxt, InlinedCallOP, 0 );

	/* Discard the result */
	emit0( ctxt, DiscardOP );
}

static void compileBreak( ec_compiler_ctxt ctxt, ASTNode node )
{
	EcInt  pos;
	EcUInt lev;

	pos = emit1( ctxt, JumpOP, 0x0000 );
	pos++;

	if (node->vContinueStmt.lev)
		lev = node->vContinueStmt.lev->vConstExpr.vInt;
	else
		lev = 1;
#if EC_DEBUG_COMPILATION
	fprintf( stderr, "BREAK at %ld (pos: %ld, lev %ld)\n", pos-1, pos, lev );
#endif
	PUSH_BREAK( CCTXT(currentScope), pos, lev );
}

static void compileContinue( ec_compiler_ctxt ctxt, ASTNode node )
{
	EcInt  pos;
	EcUInt lev;

	pos = emit1( ctxt, JumpOP, 0x0000 );
	pos++;

	if (node->vContinueStmt.lev)
		lev = node->vContinueStmt.lev->vConstExpr.vInt;
	else
		lev = 1;
#if EC_DEBUG_COMPILATION
	fprintf( stderr, "CONTINUE at %ld (pos: %ld, lev %ld)\n", pos-1, pos, lev );
#endif
	PUSH_CONTINUE( CCTXT(currentScope), pos, lev );
}

static void compileGoto( ec_compiler_ctxt ctxt, ASTNode node )
{
	EcInt  pos;

	pos = emit1( ctxt, JumpOP, 0x0000 );
	pos++;

	if (! reference_label( ctxt, SYMID(node->vGotoStmt.target), pos ))
	{
		EcAlert( EcError,
				 "(LINE: %ld, COLUMN: %ld) internal error",
				 FLINE(node), FCOL(node) );
	}
}

static EC_OBJ compileCatch( ec_compiler_ctxt ctxt, ASTNode node )
{
	ASTNode funcNode;
	EC_OBJ typeobj;
	EC_OBJ code;
	EC_OBJ handler;

	EcVarRefStruct typeinfo;

	/*
	 * Obtain the type of object to catch.
	 */
	ASSERT( (node->vCatchStmt.type->type == qualifiedSymbolType) );
	if (! getSymbol( ctxt, CCTXT(currentScope), QSYM(node->vCatchStmt.type), &typeinfo ))
	{
		char *typename;

		typename  = EcQualifiedString( QSYM(node->vCatchStmt.type) );
		EcCompileError( FLINE(node), FCOL(node), "undefined variable `%s'", typename );
		EcAlert( EcError,
				 "(LINE: %ld, COLUMN: %ld) undefined variable `%s'",
				 FLINE(node), FCOL(node), typename );
		ec_free( typename );
	}
	if ((V_CLASS(&typeinfo) != SymbolPrivate) && (V_CLASS(&typeinfo) != SymbolPublic))
	{
		char *typename;

		typename  = EcQualifiedString( QSYM(node->vCatchStmt.type) );
		EcCompileError( FLINE(node), FCOL(node), "type of `%s' is not known at compile-time in handler", typename );
		EcAlert( EcError,
				 "(LINE: %ld, COLUMN: %ld) type of `%s' is not known at compile-time in handler",
				 FLINE(node), FCOL(node), typename );
		ec_free( typename );
	}

	/* Get the object to catch in the proper package and location */
	typeobj = EcArrayGet( getPackageFrame( ctxt, V_PKG(&typeinfo) ), V_POS(&typeinfo) );

	if (! EC_CLASSP(typeobj))
	{
		char *typename;

		typename  = EcQualifiedString( QSYM(node->vCatchStmt.type) );
		EcCompileError( FLINE(node), FCOL(node), "type of `%s' is not a class in handler", typename );
		EcAlert( EcError,
				 "(LINE: %ld, COLUMN: %ld) type of `%s' is not a class in handler",
				 FLINE(node), FCOL(node), typename );
		ec_free( typename );
	}

	ASSERT( EC_CLASSP(typeobj) );

	/* Compile the code block as an anonymous function */
	if (node->vCatchStmt.var)
		funcNode = makeFunction( NULL, NULL,
								 makeParamList( FALSE, makeList( TRUE, FALSE, NULL, makeParam(node->vCatchStmt.var, NULL) ) ),
								 node->vCatchStmt.handler );
	else
		funcNode = makeFunction( NULL, NULL, NULL,
								 node->vCatchStmt.handler );
	code = compileInlineCodeBlock( ctxt, funcNode );

	ASSERT( EC_CLASSP(typeobj) );
	ASSERT( EC_COMPILEDP(code) );
	handler = EcMakeHandler( typeobj, code );

	ASSERT( EC_HANDLERP(handler) );
	return handler;
}

static void compileThrow( ec_compiler_ctxt ctxt, ASTNode node )
{
	/* Compile expression to throw */
	compile( ctxt, node->vThrowStmt.expr );

	emit0( ctxt, ThrowOP );
}

static void compileImport( ec_compiler_ctxt ctxt, ASTNode node )
{
	ASTNode  pkg, symlist;
	char    *name;
	EC_OBJ   symarray;
	EcBool   importOk;

	ASSERT( node->type == importStmtType );

	if (node->vImportStmt.detailed == FALSE)
	{
		/* PACKAGE IMPORT: no symbol list */

		pkg  = node->vImportStmt.package;
		ASSERT( pkg->type == qualifiedSymbolType );

		name = EcQualifiedString( QSYM(pkg) );
		importOk = EcPackageImport( CCTXT(currentPackage), name, EC_NIL, PRIVATE(executeImported) );
		if (! importOk)
		{
			EcCompileError( FLINE(pkg), FCOL(pkg), "can't import package `%s'", name );
			EcAlert( EcError,
					 "(LINE: %ld, COLUMN: %ld) can't import package `%s'",
					 FLINE(pkg), FCOL(pkg), name );
		}
		ec_free( name );
	} else
	{
		/* DETAILED IMPORT: symbol list or '*', some symbols not qualified */

		pkg     = node->vImportStmt.package;
		symlist = node->vImportStmt.symlist;
		ASSERT( pkg->type == qualifiedSymbolType );
		ASSERT( (! symlist) || (symlist->type == stmtListType) );

		if (symlist)											/* Import a list of symbols */
		{
			/* build array of symbols */
			symarray = EcMakeArray( 0 );
			while (symlist) {
				ASSERT( symlist->vStmtList.stmt->type == symbolType );
				EcArrayPush( symarray,
							 EcMakeSymbol( EcSymbolAt( SYMID(symlist->vStmtList.stmt) ) ) );

				symlist = symlist->vStmtList.next;
			}
		} else
			symarray = EcTrueObject;							/* Import all symbols       */

		name = EcQualifiedString( QSYM(pkg) );
		importOk = EcPackageImport( CCTXT(currentPackage), name, symarray, PRIVATE(executeImported) );
		if (! importOk)
		{
			EcCompileError( FLINE(pkg), FCOL(pkg), "can't import package `%s'", name );
			EcAlert( EcError,
					 "(LINE: %ld, COLUMN: %ld) can't import package `%s'",
					 FLINE(pkg), FCOL(pkg), name );
		}
		ec_free( name );
	}
}

static void compileCall( ec_compiler_ctxt ctxt, ASTNode node )
{
	ASTNode argList;
	EcInt nargs;

	/* Count number of arguments
	 *
	 * Note that we must count the number of arguments, since
	 * the callee, unknown until run-time, must know the number
	 * of objects to fetch from the stack frame.
	 */
	nargs = 0;
	argList = node->vCall.arglist;
	while (argList) {
		nargs++;
		argList = argList->vStmtList.next;
	}
/*	fprintf( stderr, "Number of args in call: %ld\n", nargs );*/

	/* Compile arguments */
	argList = node->vCall.arglist;
	while (argList) {
		ASSERT( argList->vStmtList.stmt );
		compile( ctxt, argList->vStmtList.stmt );

		argList = argList->vStmtList.next;
	}

	/* Compile function to call */
	compile( ctxt, node->vCall.fnct );

	emit1( ctxt, CallOP, nargs );
}

static void compileMethodCall( ec_compiler_ctxt ctxt, ASTNode node )
{
	ASTNode argList;
	EcInt nargs;

	/* Count number of arguments
	 *
	 * Note that we must count the number of arguments, since
	 * the callee, unknown until run-time, must know the number
	 * of objects to fetch from the stack frame.
	 */
	nargs = 0;
	argList = node->vMethodCall.arglist;
	while (argList) {
		nargs++;
		argList = argList->vStmtList.next;
	}
/*	fprintf( stderr, "Number of args in call: %ld\n", nargs );*/

	/*
	 * Compile arguments
	 *
	 * NOTE: Here we does NOT have to, and CAN'T, pass self and at_class
	 *       These will be known at run-time, and automatically put in at
	 *       that time.
	 */
	argList = node->vMethodCall.arglist;
	while (argList) {
		ASSERT( argList->vStmtList.stmt );
		compile( ctxt, argList->vStmtList.stmt );

		argList = argList->vStmtList.next;
	}

	/* Compile receiver */
	if (! node->vMethodCall.super)
	{
		compile( ctxt, node->vMethodCall.receiver );						/* this is duplicated, but this way is more easy... */

		emit2( ctxt, CallMethodOP, SYMID(node->vMethodCall.method), nargs );
	} else
		emit2( ctxt, CallSuperMethodOP, SYMID(node->vMethodCall.method), nargs );
}

static void compileStmtList( ec_compiler_ctxt ctxt, ASTNode node )
{
	ASTNode start = NULL;

	if (! node)													/* exit but check if we have to add a ReturnOP */
		goto check_for_return;

	start = node;
	while (node)
	{
		compile( ctxt, node->vStmtList.stmt );

		node = node->vStmtList.next;
	}

	/*
	 * XXX TODO: this will *never* be called for node == NULL, since compile(NULL) returns
	 *           immediately. However every function compiling function/method bodies should
	 *           (and now they do) have code to add return if needed.
	 *           (We should check behavior for inlined code blocks and packages...)
	 */
 check_for_return:
	/*
	 * XXX TODO XXX
	 * Ugh ! If node is NULL, we cannot check start->vStmtList.retval and/or
	 * start->vStmtList.inlined !
	 * We have to solve this.
	 */
	if ((start && (start->vStmtList.retVal && (! CCTXT(hasReturned)) && (! start->vStmtList.inlined))) || /* we have code                   */
		((! start) && CCTXT(currentScope) && (CCTXT(currentScope)->functype == MethodFunction) &&
		 (CCTXT(currentScope)->upper && CCTXT(currentScope)->upper->type == ClassScope))) /* we have not, but it's a method */
	{
		/* Always return a value (except for inlined code blocks) */

		ASTNode retNode;
		ASTNode exprNode;

		if (CCTXT(currentScope)->upper && CCTXT(currentScope)->upper->type == ClassScope)
		{
			/* Return self */
			exprNode = makeVariable( makeQualifiedSymbol( NULL, makeSymbol( PRIVATE(selfID) ) ) );
			retNode  = makeReturn( exprNode );

			CCTXT(hasReturned) = TRUE;
		} else
		{
			/* Return EC_NIL */
			exprNode = makeConstObject( EC_NIL );
			retNode  = makeReturn( exprNode );

			CCTXT(hasReturned) = TRUE;
		}

		compile( ctxt, retNode );
	}
}

static void compileFunction( ec_compiler_ctxt ctxt, ASTNode node )
{
	Scope   funcScope;
	EC_OBJ  func;
	char   *name = NULL;
	EcInt   nargs   = 0;										/* # of parameters */
	EcInt   nparams_def = 0;									/* # of default values for parameters */
	EcBool  varargs = FALSE;
	ASTNode plist, paramList, param, paramVar, paramInit;
	EcInt   pos;
	EcBool  anonymous = FALSE;
	EcVarRefStruct vinfo;
	EcBool  saveHasReturned;

	if (! node->vFunction.funcName)
		anonymous = TRUE;

	/* Count number of arguments */
	plist = node->vFunction.paramList;
	if (plist)
	{
		varargs     = plist->vParamList.varargs;
		nargs       = 0;
		nparams_def = 0;
		paramList = plist->vParamList.list;
		while (paramList) {
			param = paramList->vStmtList.stmt;
			paramVar  = param->vParam.var;
			paramInit = param->vParam.init;
			if (paramInit)
				nparams_def++;
			else if ((! paramInit) && (nparams_def > 0))
			{
				EcCompileError( FLINE(paramVar), FCOL(paramVar), "paramater without a default value following defaulted ones" );
				EcAlert( EcError,
						 "(LINE: %ld, COLUMN: %ld) paramater without a default value following defaulted ones",
						 FLINE(paramVar), FCOL(paramVar) );
			}

			nargs++;
			paramList = paramList->vStmtList.next;
		}
		if (varargs)
			nargs--;
	} else
	{
		varargs     = FALSE;
		nargs       = 0;
		nparams_def = 0;
	}
/*	fprintf( stderr, "Number of args: %ld\n", nargs );*/

	/* Compile function variable name */
	if (! anonymous)
	{
		ASTNode decl;
		EcVarRefStruct fnameinfo;

		/* Already declared ? */
		decl = node->vFunction.decl;
		if (! getSymbol( ctxt, CCTXT(currentScope), QSYM(node->vFunction.funcName),
						 &fnameinfo ))
		{
			/* No, so declare it ! */

#if 0
			if ((CCTXT(currentScope)->type == PackageScope) && (decl->vDecl.symclass == SymbolLocal))
				decl->vDecl.symclass = SymbolPrivate;
#endif
			compileDecl( ctxt, node->vFunction.decl );
		} else
		{
			/* Yes. Check coerency ! */
#if 0
			if ((CCTXT(currentScope)->type == PackageScope) && (decl->vDecl.symclass == SymbolLocal))
				decl->vDecl.symclass = SymbolPrivate;
#endif
			if (decl->vDecl.symclass != V_CLASS(&fnameinfo))
			{
				name = EcQualifiedString( QSYM(node->vFunction.funcName) );
				EcCompileError( FLINE(decl), FCOL(decl), "variable `%s' redeclared with a different type", name );
				EcAlert( EcError,
						 "(LINE: %ld, COLUMN: %ld) variable `%s' redeclared with a different type",
						 FLINE(decl), FCOL(decl), name );
				ec_free( name );
			}

			if (V_CONST(&fnameinfo) && V_INITIALIZED(&fnameinfo))
			{
				name = EcQualifiedString( QSYM(node->vFunction.funcName) );
				EcCompileError( FLINE(decl), FCOL(decl), "variable `%s' is constant", name );
				EcAlert( EcError,
						 "(LINE: %ld, COLUMN: %ld) variable `%s' is constant",
						 FLINE(decl), FCOL(decl), name );
				ec_free( name );
			}
		}

		CCTXT(asLValue) = TRUE;
		compile( ctxt, node->vFunction.funcName );
		CCTXT(asLValue) = FALSE;

		/* Now we have lhs position in lValue */
#if EC_DEBUG_COMPILATION
		printVInfo( &CCTXT(lValue) );
#endif
		saveVInfo( ctxt, &vinfo, &CCTXT(lValue) );
	}

#if 0
	/* Save lexical environment */
	emit0( ctxt, PushStackOP );
#endif

	/* Create a compiled object for the function */
	if (! anonymous)
		name = EcQualifiedString( QSYM(node->vFunction.funcName) );
	else
		name = NULL;
	func = EcMakeCompiled( CCTXT(currentPackage), name, nargs, nparams_def, varargs, FALSE );
	ec_free( name );

	if (EC_NULLP(CCTXT(compilationResult)))
		CCTXT(compilationResult) = func;

	/* Introduce a new scope */
	funcScope = openScope( ctxt, CCTXT(currentScope), func, NormalFunction );

	ASSERT( CCTXT(currentScope) == funcScope );

	/* Decorate AST node */
	node->vFunction.scope = funcScope;

	/* Add function parameters to the function scope */
	if (plist)
	{
		EcUInt symid;

		paramList = plist->vParamList.list;
		while (paramList) {
			ASSERT( paramList->vStmtList.stmt );
			param = paramList->vStmtList.stmt;
			paramVar  = param->vParam.var;
			paramInit = param->vParam.init;

			if (QSLEN(QSYM(paramVar)) != 1)
			{
				EcCompileError( FLINE(paramVar), FCOL(paramVar), "parameter name can't be qualified in declaration" );
				EcAlert( EcError,
						 "(LINE: %ld, COLUMN: %ld) parameter name can't be qualified in declaration",
						 FLINE(paramVar), FCOL(paramVar) );
			}

			symid = QSCOMP(QSYM(paramVar), 0);
			if (! addSymbol( ctxt, CCTXT(currentScope), symid, SymbolParam, FALSE ))
			{
				EcCompileError( FLINE(paramVar), FCOL(paramVar), "can't add symbol `%k'", symid );
				EcAlert( EcError,
						 "(LINE: %ld, COLUMN: %ld) can't add symbol `%k'",
						 FLINE(paramVar), FCOL(paramVar), symid );
			}

			if (paramInit != NULL)
			{
				ASTNode initialize_ast;

				initialize_ast = makeIf( makeBinary( EQ_OP, paramVar, makeConstObject( EcUndefinedObject ) ),
										 makeExprStmt( makeAssign( paramVar, paramInit ) ),
										 NULL );
				compile( ctxt, initialize_ast );
			}

			paramList = paramList->vStmtList.next;
		}
	}

	/* Compile function body */
	saveHasReturned = CCTXT(hasReturned);
	CCTXT(hasReturned) = FALSE;
	compile( ctxt, node->vFunction.body );
	if (! CCTXT(hasReturned))
	{
		ASTNode retNode;
		ASTNode exprNode;

		/* Return EC_NIL */
		exprNode = makeConstObject( EC_NIL );
		retNode  = makeReturn( exprNode );
		CCTXT(hasReturned) = TRUE;

		compile( ctxt, retNode );
	}
	ASSERT( CCTXT(hasReturned) );
	CCTXT(hasReturned) = saveHasReturned;

	ASSERT( EC_COMPILEDMAXTEMPS(CCTXT(currentBytecode)) <= funcScope->maxstackdim );
	EC_COMPILEDMAXTEMPS(CCTXT(currentBytecode)) = funcScope->maxstackdim;
	ASSERT( EC_COMPILEDMAXTEMPS(CCTXT(currentBytecode)) >= 0 );

	/* Close function scope */
	closeScope( ctxt, funcScope );

	if ((! anonymous) && (V_CLASS(&vinfo) != SymbolLocal) && (V_CLASS(&vinfo) != SymbolParam))
	{
		switch (V_CLASS(&vinfo))
		{
		case SymbolParam:
			ASSERT( FALSE );
			break;
		case SymbolLocal:
			ASSERT( FALSE );
			break;
		case SymbolStatic:
			/* Save the compiled function in literal frame.                   */
			/* Note that the literal might be different from the current one. */
			ASSERT( ! anonymous );
			EcArraySet( getLiteral( ctxt, CCTXT(currentScope), V_UP(&vinfo) ), V_POS(&vinfo), func );
			if (V_ENTRY(&vinfo))
				V_ENTRY(&vinfo)->initialized = TRUE;
			V_INITIALIZED(&vinfo) = TRUE;

			/* Emit a push of the constant */
			emitLoad( ctxt, CCTXT(currentScope), &vinfo );
			/* Save in compiled object the lexical environment */
			emit0( ctxt, SaveLexicalOP );
			emit0( ctxt, DiscardOP );									/* discard the object */
			break;
		case SymbolPrivate:
		case SymbolPublic:
			ASSERT( ! anonymous );
			EcArraySet( getPackageFrame( ctxt, V_PKG(&vinfo) ), V_POS(&vinfo), func );
			if (V_ENTRY(&vinfo))
				V_ENTRY(&vinfo)->initialized = TRUE;
			V_INITIALIZED(&vinfo) = TRUE;

			/* Emit a push of the constant */
			emitLoad( ctxt, CCTXT(currentScope), &vinfo );
			/* Save in compiled object the lexical environment */
			emit0( ctxt, SaveLexicalOP );
			emit0( ctxt, DiscardOP );									/* discard the object */
			break;
		}
	} else
	{
		EcVarRefStruct funcvar;

		ASSERT( anonymous || (V_CLASS(&vinfo) == SymbolLocal) || (V_CLASS(&vinfo) == SymbolParam) );

		initVInfo( ctxt, &funcvar );

		/* Push constant object (compiled function) in literal frame */
		pos = EcArrayPush( CCTXT(currentLiteral), func );

		V_CLASS(&funcvar) = SymbolStatic;
		V_UP(&funcvar)    = 0;
		V_POS(&funcvar)   = pos;

		/* Emit a push of the constant */
		emitLoad( ctxt, CCTXT(currentScope), &funcvar );

		/*
		 * Bytecode objects are special, since closure creation
		 * requires that creating different functions from the same
		 * "lexical" source, gives effectively different functions
		 * and not simply a copy of the same one.
		 * This to prevent lexical binding to change the meaning for
		 * the same function on each creation.
		 *
		 * So stop chatting and do a ...
		 */
		emit1( ctxt, CopyOP, EcShallowCopyType );						/* Make a shallow copy of the code object */

		/* Save in compiled object the lexical environment */
		/* (this also leaves a copy on the stack, so we'll save that copy) */
		emit0( ctxt, SaveLexicalOP );

#if EC_DEBUG_COMPILATION
		if (anonymous) fprintf( stderr, "ANONYMOUS at pos: %ld\n", pos );
#endif
		if (! anonymous)
		{
			ASSERT( (V_CLASS(&vinfo) == SymbolLocal) || (V_CLASS(&vinfo) == SymbolParam) );
			/* ... we have lhs position in vinfo */

			/* Save the computed value in variable location */
			emitSave( ctxt, CCTXT(currentScope), &vinfo );
		}
	}

#if (EC_DEBUG_COMPILATION || EC_VERBOSE_COMPILATION)
	EcDumpCompiled( func, 0 );
#endif
}

static void compileClass( ec_compiler_ctxt ctxt, ASTNode node )
{
	Scope  classScope;
	EC_OBJ classobj, baseobj;

	EcQualifiedSymbol qsname;

	EcVarRefStruct baseinfo;

	ASTNode decl;
	EcVarRefStruct classinfo;

	EcVarRefStruct vinfo;

	EcQualifiedInit( &qsname );
	EcQualifiedAdd( &qsname, SYMID(node->vClassDef.name) );

	/*
	 * Obtain the base class object, indispensable for class creation.
	 */
	ASSERT( (node->vClassDef.base->type == qualifiedSymbolType) );
	if (! getSymbol( ctxt, CCTXT(currentScope), QSYM(node->vClassDef.base),
					 &baseinfo ))
	{
		char *classname, *basename;

		classname = EcQualifiedString( &qsname );
		basename  = EcQualifiedString( QSYM(node->vClassDef.base) );
		EcCompileError( FLINE(node), FCOL(node), "undefined base class `%s' in class `%s'", basename, classname );
		EcAlert( EcError,
				 "(LINE: %ld, COLUMN: %ld) undefined base class `%s' in class `%s'",
				 FLINE(node), FCOL(node), basename, classname );
		ec_free( basename );
		ec_free( classname );
	}
	if ((V_CLASS(&baseinfo) != SymbolPrivate) && (V_CLASS(&baseinfo) != SymbolPublic))
	{
		char *classname, *basename;

#if EC_DEBUG_COMPILATION
		printVInfo( &baseinfo );
#endif
		classname = EcQualifiedString( &qsname );
		basename  = EcQualifiedString( QSYM(node->vClassDef.base) );
		EcCompileError( FLINE(node), FCOL(node), "base class `%s' is not a class in class `%s'", basename, classname );
		EcAlert( EcError,
				 "(LINE: %ld, COLUMN: %ld) base class `%s' is not a class in class `%s'",
				 FLINE(node), FCOL(node), basename, classname );
		ec_free( basename );
		ec_free( classname );
	}

	/* Get the base object in the proper package and location */
	baseobj = EcArrayGet( getPackageFrame( ctxt, V_PKG(&baseinfo) ), V_POS(&baseinfo) );

	if (! EC_CLASSP(baseobj))
	{
		char *classname, *basename;

		classname = EcQualifiedString( &qsname );
		basename  = EcQualifiedString( QSYM(node->vClassDef.base) );
		EcCompileError( FLINE(node), FCOL(node), "base class `%s' is not a class in class `%s'", basename, classname );
		EcAlert( EcError,
				 "(LINE: %ld, COLUMN: %ld) base class `%s' is not a class in class `%s'",
				 FLINE(node), FCOL(node), basename, classname );
		ec_free( basename );
		ec_free( classname );
	}

	ASSERT( EC_CLASSP(baseobj) );

	classobj = EcMakeClass( CCTXT(currentPackage), baseobj, EcSymbolAt( SYMID(node->vClassDef.name) ) );
	ASSERT( EC_CLASSP(classobj) );


	/* Compile class variable name */

	/* Already declared ? */
	decl = node->vClassDef.decl;
	if (! getSymbol( ctxt, CCTXT(currentScope), &qsname, &classinfo ))
	{
		/* No, so declare it ! */

#if EC_DEBUG_COMPILATION
		printf( "Declaring: " );
		ASTPrint( 0, decl );
		printf( "\n" );
#endif
		compileDecl( ctxt, decl );
	} else
	{
		/* Yes. Check coerency ! */
		if (decl->vDecl.symclass != V_CLASS(&classinfo))
		{
			char *name;

			name = EcQualifiedString( &qsname );
			EcCompileError( FLINE(decl), FCOL(decl), "class `%s' redeclared with a different type", name );
			EcAlert( EcError,
					 "(LINE: %ld, COLUMN: %ld) class `%s' redeclared with a different type",
					 FLINE(decl), FCOL(decl), name );
			ec_free( name );
		}

		if (V_CONST(&classinfo) && V_INITIALIZED(&classinfo))
		{
			char *name;

			name = EcQualifiedString( &qsname );
			EcCompileError( FLINE(decl), FCOL(decl), "variable `%s' is constant", name );
			EcAlert( EcError,
					 "(LINE: %ld, COLUMN: %ld) variable `%s' is constant",
					 FLINE(decl), FCOL(decl), name );
			ec_free( name );
		}
	}

	CCTXT(asLValue) = TRUE;
	compile( ctxt, makeVariable( makeQualifiedSymbol( NULL, node->vClassDef.name ) ) );
	CCTXT(asLValue) = FALSE;

	/* Now we have lhs position in lValue */
#if EC_DEBUG_COMPILATION
	printVInfo( &CCTXT(lValue) );
#endif
	saveVInfo( ctxt, &vinfo, &CCTXT(lValue) );


	/* Introduce a new scope */
	ASSERT( EC_CLASSP(classobj) );
	classScope = openScope( ctxt, CCTXT(currentScope), classobj, NormalFunction );

	ASSERT( classScope );
	ASSERT( CCTXT(currentScope) == classScope );
	ASSERT( CCTXT(currentScope)->type == ClassScope );

	/* Decorate AST node */
	node->vClassDef.scope = classScope;

	/* Compile class body */
	compile( ctxt, node->vClassDef.body );

	emit0( ctxt, ReturnOP );

	ASSERT( EC_COMPILEDMAXTEMPS(CCTXT(currentBytecode)) <= classScope->maxstackdim );
	EC_COMPILEDMAXTEMPS(CCTXT(currentBytecode)) = classScope->maxstackdim;
	ASSERT( EC_COMPILEDMAXTEMPS(CCTXT(currentBytecode)) >= 0 );

	/* Close class scope */
#if EC_DEBUG_SCOPE
	dumpScope( classScope );
#endif
	closeScope( ctxt, classScope );

	/* Save the class in package */
	EcArraySet( getPackageFrame( ctxt, V_PKG(&vinfo) ), V_POS(&vinfo), classobj );
	if (V_ENTRY(&vinfo))
		V_ENTRY(&vinfo)->initialized = TRUE;
	V_INITIALIZED(&vinfo) = TRUE;

	/* Compile the setting of lexical environment for class code */

	/* Emit a push of the class */
	emitLoad( ctxt, CCTXT(currentScope), &vinfo );

	/* Get the class code */
	compileNamedVariable( ctxt, "basic.classCode" );
	emit1( ctxt, CallOP, 1 );

	/* Save in compiled object the lexical environment */
	emit0( ctxt, SaveLexicalOP );
	emit0( ctxt, DiscardOP );									/* discard the object */

	/* Compile the execution of class body */

	/* Emit a push of the class */
	emitLoad( ctxt, CCTXT(currentScope), &vinfo );

	/* Get the class code */
	compileNamedVariable( ctxt, "basic.classCode" );
	emit1( ctxt, CallOP, 1 );

	/* Compile a call to class code */
	emit1( ctxt, CallOP, 0 );

	/* Discard the result */
	emit0( ctxt, DiscardOP );

#if EC_DEBUG_COMPILATION
	EcDumpCompiled( EC_CLASSCODE(classobj), 0 );
#if 1
	_ec_dbg_dump_literal( EC_CLASSLFRAME(classobj) );
#endif
#endif
}

static void compileMethod( ec_compiler_ctxt ctxt, ASTNode node )
{
	Scope   methodScope, lexicalScope;
	EC_OBJ  method;
	EcInt   nargs       = 0;									/* # of parameters                    */
	EcInt   nparams_def = 0;									/* # of default values for parameters */
	EcBool  varargs = FALSE;
	ASTNode plist, paramList, param, paramVar, paramInit;
	EcBool  saveHasReturned;
	EcInt   pos;
	char   *fullname, *classname, *name;

	EcVarRefStruct methodinfo;

	/* Count number of arguments */
	plist = node->vMethodDef.paramList;
	if (plist)
	{
		varargs     = plist->vParamList.varargs;
		nargs       = 0;
		nparams_def = 0;
		paramList = plist->vParamList.list;
		while (paramList) {
			param = paramList->vStmtList.stmt;
			paramVar  = param->vParam.var;
			paramInit = param->vParam.init;
			if (paramInit)
				nparams_def++;
			else if ((! paramInit) && (nparams_def > 0))
			{
				EcCompileError( FLINE(paramVar), FCOL(paramVar), "paramater without a default value following defaulted ones" );
				EcAlert( EcError,
						 "(LINE: %ld, COLUMN: %ld) paramater without a default value following defaulted ones",
						 FLINE(paramVar), FCOL(paramVar) );
			}

			nargs++;
			paramList = paramList->vStmtList.next;
		}
		if (varargs)
			nargs--;
	} else
	{
		varargs     = FALSE;
		nargs       = 0;
		nparams_def = 0;
	}
#if EC_DEBUG_COMPILATION
	fprintf( stderr, "Method number of args: %ld (+ 2 implicit)\n", nargs );
#endif

	ASSERT( CCTXT(currentBytecode) );							/* This is bytecode of the entity containing the class */

#if 0
	/* Save lexical environment */
	emit0( ctxt, PushStackOP );
#endif

	/* Create a compiled object for the method */
	classname = EC_STRDATA(EC_CLASSSHORTNAME(CCTXT(currentScope)->target));
	name      = (char *)EcSymbolAt( SYMID(node->vMethodDef.name) );
	fullname = alloca( EC_STRLEN(EC_CLASSSHORTNAME(CCTXT(currentScope)->target)) + 2 + strlen( name ) + 1 );
	ASSERT( fullname );
	strcpy( fullname, classname );
	strcat( fullname, "::" );
	strcat( fullname, name );
	method = EcMakeCompiled( CCTXT(currentPackage),
							 fullname,
							 2 + nargs,
							 nparams_def,
							 varargs, TRUE );		/* space for self, at_class & args */

	if (EC_NULLP(CCTXT(compilationResult)))
		CCTXT(compilationResult) = method;

	/* Introduce a new scope */
	methodScope = openScope( ctxt, CCTXT(currentScope), method, node->vMethodDef.classmethod ? ClassMethodFunction : MethodFunction );

	ASSERT( CCTXT(currentScope) == methodScope );
	ASSERT( CCTXT(currentScope)->upper->type == ClassScope );

	/* Decorate AST node */
	node->vMethodDef.scope = methodScope;

	/* Add method parameters to the method scope */
	if (! addSymbol( ctxt, CCTXT(currentScope), PRIVATE(selfID), SymbolParam, TRUE ))
	{
		EcCompileError( FLINE(node), FCOL(node), "can't add variable self" );
		EcAlert( EcError,
				 "(LINE: %ld, COLUMN: %ld) can't add variable self",
				 FLINE(node), FCOL(node) );
	}
	if (! addSymbol( ctxt, CCTXT(currentScope), PRIVATE(at_classID), SymbolParam, TRUE ))
	{
		EcCompileError( FLINE(node), FCOL(node), "can't add variable at_class" );
		EcAlert( EcError,
				 "(LINE: %ld, COLUMN: %ld) can't add variable at_class",
				 FLINE(node), FCOL(node) );
	}
	if (plist)
	{
		paramList = plist->vParamList.list;
		while (paramList) {
			ASSERT( paramList->vStmtList.stmt );
			param     = paramList->vStmtList.stmt;
			if (param->type == paramNodeType)
			{
				paramVar  = param->vParam.var;
				paramInit = param->vParam.init;
			} else {
				paramVar = param;
				paramInit = NULL;
			}

			if (QSLEN(QSYM(paramVar)) != 1)
			{
				EcCompileError( FLINE(paramVar), FCOL(paramVar), "parameter name can't be qualified in declaration" );
				EcAlert( EcError,
						 "(LINE: %ld, COLUMN: %ld) parameter name can't be qualified in declaration",
						 FLINE(paramVar), FCOL(paramVar) );
			}

			if (! addSymbol( ctxt, CCTXT(currentScope), QSCOMP(QSYM(paramVar), 0), SymbolParam, FALSE ))
			{
				EcCompileError( FLINE(paramVar), FCOL(paramVar), "can't add symbol" );
				EcAlert( EcError,
						 "(LINE: %ld, COLUMN: %ld) can't add symbol",
						 FLINE(paramVar), FCOL(paramVar) );
			}

			if (paramInit != NULL)
			{
				ASTNode initialize_ast;

				initialize_ast = makeIf( makeBinary( EQ_OP, paramVar, makeConstObject( EcUndefinedObject ) ),
										 makeExprStmt( makeAssign( paramVar, paramInit ) ),
										 NULL );
				compile( ctxt, initialize_ast );
#if 0
				EcCompileError( FLINE(paramInit), FCOL(paramInit), "default values for parameters are not supported yet" );
				EcAlert( EcError,
						 "(LINE: %ld, COLUMN: %ld) default values for parameters are not supported yet",
						 FLINE(paramInit), FCOL(paramInit) );
#endif
			}

			paramList = paramList->vStmtList.next;
		}
	}

	/* Compile method body */
	saveHasReturned = CCTXT(hasReturned);
	CCTXT(hasReturned) = FALSE;
	compile( ctxt, node->vMethodDef.body );
	if (! CCTXT(hasReturned))
	{
		ASTNode retNode  = NULL;
		ASTNode exprNode;

		if (CCTXT(currentScope)->upper && CCTXT(currentScope)->upper->type == ClassScope)
		{
			/* Return self */
			exprNode = makeVariable( makeQualifiedSymbol( NULL, makeSymbol( PRIVATE(selfID) ) ) );
			retNode  = makeReturn( exprNode );
			CCTXT(hasReturned) = TRUE;
		}

		compile( ctxt, retNode );
	}
	ASSERT( CCTXT(hasReturned) );
	CCTXT(hasReturned) = saveHasReturned;

	ASSERT( EC_COMPILEDMAXTEMPS(CCTXT(currentBytecode)) <= methodScope->maxstackdim );
	EC_COMPILEDMAXTEMPS(CCTXT(currentBytecode)) = methodScope->maxstackdim;
	ASSERT( EC_COMPILEDMAXTEMPS(CCTXT(currentBytecode)) >= 0 );

	/* Close method scope */
	closeScope( ctxt, methodScope );

	ASSERT( CCTXT(currentScope) );
/*	lexicalScope = CCTXT(currentScope)->upper;
	ASSERT( lexicalScope && (lexicalScope->type == PackageScope) ); */
	lexicalScope = CCTXT(currentScope);
	ASSERT( lexicalScope && (lexicalScope->type == ClassScope) );

	/* Push constant object (compiled method) in literal frame */
	pos = EcArrayPush( lexicalScope->lframe, method );

	/* Emit a push of the constant */
	initVInfo( ctxt, &methodinfo );
	V_CLASS(&methodinfo) = SymbolStatic;
	V_UP(&methodinfo)    = 0;
	V_POS(&methodinfo)   = pos;
/*	ASSERT( CCTXT(currentScope) && CCTXT(currentScope)->upper && ((CCTXT(currentScope)->upper)->type == PackageScope) );*/
	emitLoad( ctxt, lexicalScope, &methodinfo );

	/* Save in compiled object the lexical environment */
	emit0( ctxt, SaveLexicalOP );
	emit0( ctxt, DiscardOP );									/* discard the object */

	if (node->vMethodDef.classmethod)
	{
		if (! EcAddClassMethod( CCTXT(currentScope)->target, EcSymbolAt( SYMID(node->vMethodDef.name) ), method ))
		{
			EcCompileError( FLINE(node), FCOL(node), "can't add class method `%s'", EcSymbolAt( SYMID(node->vMethodDef.name) ) );
			EcAlert( EcError,
					 "(LINE: %ld, COLUMN: %ld) can't add class method `%s'",
					 FLINE(node), FCOL(node), EcSymbolAt( SYMID(node->vMethodDef.name) ) );
		}
	} else
	{
		if (! EcAddMethod( CCTXT(currentScope)->target, EcSymbolAt( SYMID(node->vMethodDef.name) ), method ))
		{
			EcCompileError( FLINE(node), FCOL(node), "can't add method `%s'", EcSymbolAt( SYMID(node->vMethodDef.name) ) );
			EcAlert( EcError,
					 "(LINE: %ld, COLUMN: %ld) can't add method `%s'",
					 FLINE(node), FCOL(node), EcSymbolAt( SYMID(node->vMethodDef.name) ) );
		}
	}

#if (EC_DEBUG_COMPILATION || EC_VERBOSE_COMPILATION)
	EcDumpCompiled( method, 0 );
#endif
}

static void compileReturn( ec_compiler_ctxt ctxt, ASTNode node )
{
	/* Compile expression to return */
	compile( ctxt, node->vReturn.expr );

	emit0( ctxt, ReturnOP );

	/*
	 * WRONG !!!!
	 * we can't indiscriminately set CCTXT(hasReturned) !
	 * we can only if it is the very last statement in the body,
	 * otherwise it could be conditionalized by an 'if' for example.
	 *
	 * CCTXT(hasReturned) is now set in compileStmtList (check_for_return)
	 *

	CCTXT(hasReturned) = TRUE;

	*/
}

/*
 * RATIONALE for compilePackage()
 *
 *    compilePackage() has been split in two halves: compilePackage_pre()
 *    and compilePackage_post(). The split took place around the original
 *    body compile() call.
 *    This because 'package' has become a statement, so that whenever we
 *    compile it, we have to flush the previous active package compilation
 *    context, then begin with a fresh one.
 */

static void compilePackage_pre( ec_compiler_ctxt ctxt, ASTNode node )
{
	/*
	 * This function performs the initial half of the tasks needed to
	 * compile a package.
	 */

	char *packagename;

	/*
	 * Compilation of a package has some similarities with that
	 * of a function.
	 */

	EcInt       nexport;
	STableEntry entry;

	if (node->vPackage.name)
		packagename = EcQualifiedString( QSYM(node->vPackage.name) );
	else
		packagename = ec_stringdup( DEFAULT_PACKAGENAME );
	CCTXT(package_package) = EcPackageIntroduce( packagename );
	ec_free( packagename );
/*	package = EcPackageIntroduce( EcSymbolAt( SYMID(node->vPackage.name) ) );*/

	if ((EC_NULLP(CCTXT(compilationResult))) &&
		(! ((! CCTXT(opts).whole_package) && EC_NNULLP(CCTXT(opts).in_package))))
		CCTXT(compilationResult) = CCTXT(package_package);

	if (EC_NULLP(PRIVATE(mainTarget)))
		PRIVATE(mainTarget) = CCTXT(package_package);

	/* Introduce a new scope */
	CCTXT(package_packageScope) = openScope( ctxt, CCTXT(currentScope),
											 CCTXT(package_package), NormalFunction );

	ASSERT( CCTXT(package_packageScope) );
	ASSERT( CCTXT(currentScope) == CCTXT(package_packageScope) );

	/* Decorate AST node */
	node->vPackage.scope = CCTXT(package_packageScope);

	/* Compile the export list for the package */
	nexport = 0;
	entry = CCTXT(package_packageScope)->symtab;
	while (entry)
	{
		if (entry->symclass == SymbolPublic)
			nexport++;

		entry = entry->next;
	}

	if (nexport)
	{
		entry = CCTXT(package_packageScope)->symtab;
		nexport = 0;
		while (entry)
		{
			if (entry->symclass == SymbolPublic)
				EcPackageVariable( CCTXT(package_package),
								   EcSymbolAt( entry->symid ), EC_NIL, TRUE, entry->isconst );

			entry = entry->next;
		}
	}

	/* Compile package body */
	CCTXT(package_saveHasReturned) = CCTXT(hasReturned);
	CCTXT(hasReturned) = FALSE;

	/* ... here we'll start compiling package statements ... */
}

static void compilePackage_post( ec_compiler_ctxt ctxt )
{
	/*
	 * This function performs the initial half of the tasks needed to
	 * compile a package.
	 */

	/* If we have a pending package, finish compiling it */

	ASSERT( CCTXT(currentPackage) == CCTXT(package_package) );
	if (EC_NULLP(CCTXT(package_package)))						/* We are not in the middle of a package... */
		return;

	CCTXT(hasReturned) = CCTXT(package_saveHasReturned);

	ASSERT( EC_COMPILEDMAXTEMPS(CCTXT(currentBytecode)) <= CCTXT(package_packageScope)->maxstackdim );
	EC_COMPILEDMAXTEMPS(CCTXT(currentBytecode)) = CCTXT(package_packageScope)->maxstackdim;
	ASSERT( EC_COMPILEDMAXTEMPS(CCTXT(currentBytecode)) >= 0 );

	/* Add package to list of compiled packages (needed for example to save it later eventually) */
	EC_PACKAGESOURCE(CCTXT(package_package)) = EcMakeString( PRIVATE(fileSource), 0 );
	ec_list_append( CCTXT(compiled_packages), (EcAny) 0, (EcAny) CCTXT(package_package) );
#if 0
	EcPackageSave( CCTXT(package_package), PRIVATE(fileOutput) );
	EC_CHECK(CCTXT(package_package));
	EC_CHECKALL();
#endif

	/* Close package scope */
	closeScope( ctxt, CCTXT(package_packageScope) );

#if (EC_DEBUG_COMPILATION || EC_VERBOSE_COMPILATION)
	printf( "\nPACKAGE:\n" );
	EcDumpCompiled( EC_PACKAGECODE(CCTXT(package_package)), 0 );
#if 1
	ec_dbg_dump_package_frame( EC_PACKAGEFRAME(CCTXT(package_package)) );
#endif
#endif

	if (PRIVATE(execute))
		EcMainExecute( CCTXT(package_package) );
}

static void compilePackage( ec_compiler_ctxt ctxt, ASTNode node )
{
	/* close with the active package we're compiling, if any... */
	if (EC_NULLP(CCTXT(package_package)))
		compilePackage_post( ctxt );

	compilePackage_pre( ctxt, node );
}

/*
 * Compile an inlined Code Block
 *
 * An inlined Code Block is much like an anonymous function,
 * but with the remarkable difference that there are two types
 * of return:
 *
 *  1. Implicit return   return just after the calling point
 *  2. Explicit return   same as a return from the caller of the block
 *
 * Implicit return happens "falling off the edge" of the block, and
 * is caused by a special return instruction (InlinedReturnOP).
 * Explicit return is caused by the common return instruction.
 *
 * (Inlined code blocks are useful at least for try/catch blocks).
 */
static EC_OBJ compileInlineCodeBlock( ec_compiler_ctxt ctxt, ASTNode node )
{
	Scope   funcScope;
	EC_OBJ  func;
	EcInt   nargs   = 0;
	EcBool  varargs = FALSE;
	ASTNode plist, paramList, param, paramVar, paramInit;
	EcBool  saveHasReturned;
	EcInt   pos;

	EcVarRefStruct funcinfo;

	/*
	 * Well, we don't have parameters in inlined code blocks
	 * to tell the truth...
	 */

	/* Count number of arguments */
	plist = node->vFunction.paramList;
	if (plist)
	{
		varargs = plist->vParamList.varargs;
		nargs = 0;
		paramList = plist->vParamList.list;
		while (paramList) {
			nargs++;
			paramList = paramList->vStmtList.next;
		}
		if (varargs)
			nargs--;
	} else
	{
		varargs = FALSE;
		nargs   = 0;
	}
#if EC_DEBUG_COMPILATION
/*	fprintf( stderr, "Number of args: %ld\n", nargs );*/
#endif

#if 0
	/* Save lexical environment */
	emit0( ctxt, PushStackOP );
#endif

	/* Create a compiled object for the function */
	func = EcMakeCompiled( CCTXT(currentPackage), NULL, nargs, 0, varargs, FALSE );

	/* Introduce a new scope */
	funcScope = openScope( ctxt, CCTXT(currentScope), func, NormalFunction );

	ASSERT( CCTXT(currentScope) == funcScope );

	/* Decorate AST node */
	node->vFunction.scope = funcScope;

	/* Add function parameters to the function scope */
	if (plist)
	{
		paramList = plist->vParamList.list;
		while (paramList) {
			ASSERT( paramList->vStmtList.stmt );
			param    = paramList->vStmtList.stmt;
			paramVar  = param->vParam.var;
			paramInit = param->vParam.init;

			if (paramInit != NULL)
			{
				EcCompileError( FLINE(paramInit), FCOL(paramInit),
								"default values for parameters are not supported in inlined code blocks" );
				EcAlert( EcError,
						 "(LINE: %ld, COLUMN: %ld) default values for parameters are not supported in inlined code blocks",
						 FLINE(paramInit), FCOL(paramInit) );
			}

			if (QSLEN(QSYM(paramVar)) != 1)
			{
				EcCompileError( FLINE(paramVar), FCOL(paramVar), "parameter name can't be qualified in declaration" );
				EcAlert( EcError,
						 "(LINE: %ld, COLUMN: %ld) parameter name can't be qualified in declaration",
						 FLINE(paramVar), FCOL(paramVar) );
			}

			if (! addSymbol( ctxt, CCTXT(currentScope), QSCOMP(QSYM(paramVar), 0), SymbolParam, FALSE ))
			{
				EcCompileError( FLINE(paramVar), FCOL(paramVar), "can't add symbol" );
				EcAlert( EcError,
						 "(LINE: %ld, COLUMN: %ld) can't add symbol",
						 FLINE(paramVar), FCOL(paramVar) );
			}

			paramList = paramList->vStmtList.next;
		}
	}

	/* Compile function body */
	saveHasReturned = CCTXT(hasReturned);
	CCTXT(hasReturned) = FALSE;
	if (node->vFunction.body)
		node->vFunction.body->vStmtList.inlined = TRUE;
	compile( ctxt, node->vFunction.body );
	emit0( ctxt, InlinedReturnOP );
	CCTXT(hasReturned) = saveHasReturned;

	ASSERT( EC_COMPILEDMAXTEMPS(CCTXT(currentBytecode)) <= funcScope->maxstackdim );
	EC_COMPILEDMAXTEMPS(CCTXT(currentBytecode)) = funcScope->maxstackdim;
	ASSERT( EC_COMPILEDMAXTEMPS(CCTXT(currentBytecode)) >= 0 );

	/* Close function scope */
	closeScope( ctxt, funcScope );

	/* Push constant object (compiled function) in literal frame */
	pos = EcArrayPush( CCTXT(currentLiteral), func );

	/* Emit a push of the constant */
	initVInfo( ctxt, &funcinfo );
	V_CLASS(&funcinfo) = SymbolStatic;
	V_UP(&funcinfo)    = 0;
	V_POS(&funcinfo)   = pos;
	emitLoad( ctxt, CCTXT(currentScope), &funcinfo );

	/* Save in compiled object the lexical environment */
	emit0( ctxt, SaveLexicalOP );
	emit0( ctxt, DiscardOP );									/* discard the object */

#if EC_DEBUG_COMPILATION
	EcDumpCompiled( func, 0 );
#endif
	return func;
}

static ASTNode getServiceVariable( ec_compiler_ctxt ctxt, Scope scope )
{
	Scope master;

	char varname[20];
	EcUInt nextv;
	EcUInt symid;
	ASTNode varnode;

	nextv = scope->next_srv_var++;
	sprintf( varname, "$var_%ld", (long)nextv );
	symid = EcInternSymbol( varname );

	varnode = makeVariable( makeQualifiedSymbol( NULL, makeSymbol( symid ) ) );

	master = getMasterScope( ctxt, scope );

	switch (master->type)
	{
	case PackageScope:
	case FunctionScope:
		if (! addSymbol( ctxt, scope, symid, SymbolLocal, FALSE ))
		{
			EcCompileError( -1, -1, "can't add symbol" );
			EcAlert( EcError,
					 "(LINE: --, COLUMN: --) can't add symbol" );
		}
		break;

	case BlockScope:
		ASSERT( FALSE );
		break;

	case ClassScope:
		if (! addSymbol( ctxt, scope, symid, SymbolStatic, FALSE ))
		{
			EcCompileError( -1, -1, "can't add symbol" );
			EcAlert( EcError,
					 "(LINE: --, COLUMN: --) can't add symbol" );
		}
		break;

	default:
		ASSERT( FALSE );
		break;
	}

	return varnode;
}

static EcBool compileNamedVariable( ec_compiler_ctxt ctxt, const char *varname )
{
	EcQualifiedSymbol qsym;
	ASTNode varnode, qsymnode;
	EcInt i;

	EcQualifiedInit( &qsym );
	EcQualifiedFromString( &qsym, varname );

	qsymnode = NULL;
	for (i = 0; i < QSLEN(&qsym); i++)
		qsymnode = makeQualifiedSymbol( qsymnode, makeSymbol( QSCOMP(&qsym, i) ) );

	varnode = makeVariable( qsymnode );

	compileVariable( ctxt, varnode );

	return TRUE;
}


/* ========================================================================
 * C O D E   E M I T T E R S
 * ======================================================================== */

static void stackgrow(ec_compiler_ctxt ctxt, EcInt n)
{
	if (CCTXT(currentScope))
	{
		if (n > 0)
			STACKGROW(CCTXT(currentScope), n);
		else
			STACKSHRINK(CCTXT(currentScope), -n);
	}
}

static EcInt emit0( ec_compiler_ctxt ctxt, EcBytecode bc )
{
    EcInt pos;

	ASSERT( EC_COMPILEDP(CCTXT(currentBytecode)) );
	ASSERT( EcBytecodeParams( bc ) == 0 );

	stackgrow( ctxt, EcBytecodeStackgrow( bc ) );

    pos = EcCompiledPush( CCTXT(currentBytecode), bc );
#if EC_DEBUG_DUMP_EMIT
	fprintf(stderr, "%5ld %18s\n", (long)pos, EcBytecodeName( bc ));
#endif
    return pos;
}

static EcInt emit1( ec_compiler_ctxt ctxt, EcBytecode bc, EcInt op1 )
{
    EcInt pos;

	ASSERT( EC_COMPILEDP(CCTXT(currentBytecode)) );
	ASSERT( EcBytecodeParams( bc ) == 1 );

	stackgrow( ctxt, EcBytecodeStackgrow( bc ) );

    pos = EcCompiledPush( CCTXT(currentBytecode), bc );
    EcCompiledPush( CCTXT(currentBytecode), op1 );
#if EC_DEBUG_DUMP_EMIT
	fprintf(stderr, "%5ld %18s %12ld\n", (long)pos, EcBytecodeName( bc ), (long)op1);
#endif

    return pos;
}

static EcInt emit2( ec_compiler_ctxt ctxt, EcBytecode bc, EcInt op1, EcInt op2 )
{
    EcInt pos;

	ASSERT( EC_COMPILEDP(CCTXT(currentBytecode)) );
	ASSERT( EcBytecodeParams( bc ) == 2 );

	stackgrow( ctxt, EcBytecodeStackgrow( bc ) );

    pos = EcCompiledPush( CCTXT(currentBytecode), bc );
    EcCompiledPush( CCTXT(currentBytecode), op1 );
    EcCompiledPush( CCTXT(currentBytecode), op2 );
#if EC_DEBUG_DUMP_EMIT
	fprintf(stderr, "%5ld %18s %12ld %12ld\n", (long)pos, EcBytecodeName( bc ), (long)op1, (long)op2);
#endif

    return pos;
}

static EcInt nextPosition( ec_compiler_ctxt ctxt )
{
    return EC_COMPILEDNCODE(CCTXT(currentBytecode));
}

static void patch( ec_compiler_ctxt ctxt, EcInt where, EcInt with )
{
    ASSERT( where < EC_COMPILEDNCODE(CCTXT(currentBytecode)) );

    EC_COMPILEDCODE(CCTXT(currentBytecode))[where] = with;
}

/*
 * V_SCOPE could be null at times, because not only variables
 * can be used with emitLoad, but also constants. So we
 * must be able to obtain a suitable V_SCOPE
 */
static Scope find_scope( ec_compiler_ctxt ctxt, Scope scope, EcVarRef vinfo )
{
	EcInt i;

	if (scope->type == PackageScope)
	{
		V_SCOPE(vinfo) = scope;
	} else
	{
		for (i = 0; (i < V_UP(vinfo)) && scope; i++)
			scope = scope->upper;
		V_SCOPE(vinfo) = scope;
	}
	return scope;
}

static EcInt emitLoad( ec_compiler_ctxt ctxt, Scope scope, EcVarRef vinfo )
{
	Scope        starting, s;
	FunctionType ftype = NormalFunction;
	EcInt        res = 0;

	/* Save original starting scope */
	starting = scope;

	if (scope->type != PackageScope)
	{
		if (! V_SCOPE(vinfo))
			V_SCOPE(vinfo) = find_scope( ctxt, starting, vinfo );
		scope = V_SCOPE(vinfo);

		/*
		 * Find the class of the bytecode in which the symbol is
		 * referenced (function, method, or else).
		 */
		s = starting;
		do
		{
			if (s->functype != NormalFunction)
			{
				ftype = s->functype;
				break;
			}
			if (s == scope) break;

			s = s->upper;
		} while (s != scope);
	}

	switch (V_CLASS(vinfo))
	{
	case SymbolParam:
		if (scope->upper && scope->upper->type == ClassScope)
			res = emit2( ctxt, PushParamOP, V_UP(vinfo), METHODFRAMEOFFS( V_POS(vinfo) ) );
		else
			res = emit2( ctxt, PushParamOP, V_UP(vinfo), FUNCTIONFRAMEOFFS( V_POS(vinfo) ) );
		break;

	case SymbolLocal:											/* where stands for 'up' */
		if (scope->type == ClassScope)
		{
			if (ftype != MethodFunction)
			{
				if (V_ENTRY(vinfo))
				{
					EcCompileError( -1, -1, "referencing instance variable '%s' from outside methods", EcSymbolAt( V_ENTRY(vinfo)->symid ) );
					EcAlert( EcError,
							 "(LINE: --, COLUMN: --) Referencing instance variable '%s' from outside methods",
							 EcSymbolAt( V_ENTRY(vinfo)->symid ) );
				}
				else
				{
					EcCompileError( -1, -1, "referencing an instance variable from outside methods" );
					EcAlert( EcError,
							 "(LINE: --, COLUMN: --) Referencing an instance variable from outside methods" );
				}
			}

			ASSERT( V_UP(vinfo) >= 1 );
			res = emit1( ctxt, PushInstanceOP, V_POS(vinfo) );
		} else
		{
			if (V_UP(vinfo) == 0)
				res = emit1( ctxt, PushHereOP, V_POS(vinfo) );
			else
				res = emit2( ctxt, PushOP, V_UP(vinfo), V_POS(vinfo) );
		}
		break;

	case SymbolStatic:											/* where stands for 'up' */
		if (scope->type == ClassScope)
		{
/*			if (where == 0)
				res = emit2( ctxt, PushLiteralOP, where, pos );
			else
				res = emit1( ctxt, PushClassOP, pos );*/
			switch (ftype)
			{
			case NormalFunction:
				ASSERT( V_UP(vinfo) >= 0 );
				res = emit2( ctxt, PushLiteralOP, V_UP(vinfo), V_POS(vinfo) );
				break;
			case ClassMethodFunction:
				ASSERT( V_UP(vinfo) > 0 );
				res = emit1( ctxt, PushInstanceOP, V_POS(vinfo) );
				break;
			case MethodFunction:
				ASSERT( V_UP(vinfo) > 0 );
				res = emit1( ctxt, PushClassOP, V_POS(vinfo) );
				break;
			}
		} else
		{
			res = emit2( ctxt, PushLiteralOP, V_UP(vinfo), V_POS(vinfo) );
		}
		break;

	case SymbolPrivate:
		res = emit2( ctxt, PushPackageOP, V_PKG(vinfo), V_POS(vinfo) );
		break;

	case SymbolPublic:
		res = emit2( ctxt, PushPackageOP, V_PKG(vinfo), V_POS(vinfo) );
		break;
	}

	return res;
}

static EcInt emitSave( ec_compiler_ctxt ctxt, Scope scope, EcVarRef vinfo )
{
	Scope        starting, s;
	FunctionType ftype = NormalFunction;
	EcInt res = 0;

	/* Save original starting scope */
	starting = scope;

	if (V_INDEXED(vinfo))
	{
		/*
		 * We are saving into an array object, so at this stage
		 * we have on the run-time stack the following situation:
		 *     array object
		 *     index
		 *     value to store
		 */

		res = emit0( ctxt, SequenceSetOP );
		return res;
	}

	if (scope->type != PackageScope)
	{
		if (! V_SCOPE(vinfo))
			V_SCOPE(vinfo) = find_scope( ctxt, starting, vinfo );
		scope = V_SCOPE(vinfo);

		/*
		 * Find the class of the bytecode in which the symbol is
		 * referenced (function, method, or else).
		 */
		s = starting;
		do
		{
			if (s->functype != NormalFunction)
			{
				ftype = s->functype;
				break;
			}
			if (s == scope) break;

			s = s->upper;
		} while (s != scope);
	}

	switch (V_CLASS(vinfo))
	{
	case SymbolParam:
		if (scope->upper && scope->upper->type == ClassScope)
			res = emit2( ctxt, PopParamOP, V_UP(vinfo), METHODFRAMEOFFS( V_POS(vinfo) ) );
		else
			res = emit2( ctxt, PopParamOP, V_UP(vinfo), FUNCTIONFRAMEOFFS( V_POS(vinfo) ) );
		break;

	case SymbolLocal:
		if (scope->type == ClassScope)
		{
			if (ftype != MethodFunction)
			{
				if (V_ENTRY(vinfo))
				{
					EcCompileError( -1, -1, "modifying instance variable '%s' from outside methods", EcSymbolAt( V_ENTRY(vinfo)->symid ) );
					EcAlert( EcError,
							 "(LINE: --, COLUMN: --) Modifying instance variable '%s' from outside methods",
							 EcSymbolAt( V_ENTRY(vinfo)->symid ) );
				}
				else
				{
					EcCompileError( -1, -1, "modifying an instance variable from outside methods" );
					EcAlert( EcError,
							 "(LINE: --, COLUMN: --) Modifying an instance variable from outside methods" );
				}
			}

			ASSERT( V_UP(vinfo) >= 1 );
			res = emit1( ctxt, PopInstanceOP, V_POS(vinfo) );
		} else
		{
			if (V_UP(vinfo) == 0)
				res = emit1( ctxt, PopHereOP, V_POS(vinfo) );
			else
				res = emit2( ctxt, PopOP, V_UP(vinfo), V_POS(vinfo) );
		}
		break;

	case SymbolStatic:
		if (scope->type == ClassScope)
		{
/*			if (where == 0)
				res = emit2( ctxt, PopLiteralOP, where, pos );
			else
				res = emit1( ctxt, PopClassOP, pos );*/
			switch (ftype)
			{
			case NormalFunction:
				ASSERT( V_UP(vinfo) >= 0 );
				res = emit2( ctxt, PopLiteralOP, V_UP(vinfo), V_POS(vinfo) );
				break;
			case ClassMethodFunction:
				ASSERT( V_UP(vinfo) > 0 );
				res = emit1( ctxt, PopInstanceOP, V_POS(vinfo) );
				break;
			case MethodFunction:
				ASSERT( V_UP(vinfo) > 0 );
				res = emit1( ctxt, PopClassOP, V_POS(vinfo) );
				break;
			}
		} else
		{
			res = emit2( ctxt, PopLiteralOP, V_UP(vinfo), V_POS(vinfo) );
		}
		break;

	case SymbolPrivate:
		res = emit2( ctxt, PopPackageOP, V_PKG(vinfo), V_POS(vinfo) );
		break;

	case SymbolPublic:
		res = emit2( ctxt, PopPackageOP, V_PKG(vinfo), V_POS(vinfo) );
		break;
	}

	if (V_ENTRY(vinfo))
		V_ENTRY(vinfo)->initialized = TRUE;
	V_INITIALIZED(vinfo) = TRUE;

	return res;
}


/* ========================================================================
 * S C O P E   -   Compile-time environment handling
 * ======================================================================== */

static Scope openScope( ec_compiler_ctxt ctxt, Scope upper, EC_OBJ target, FunctionType functype )
{
	Scope res;

	ASSERT( EC_NULLP(target) || EC_COMPILEDP(target) || EC_PACKAGEP(target) || EC_CLASSP(target) );

	res = (Scope) ec_palloc( PRIVATE(nodePool), sizeof(struct ScopeStruct) );
	if (! res)
	{
		ASSERT( FALSE );
		return NULL;
	}

	if (EC_NULLP(target))
	{
		res->type = BlockScope;
	} else
	{
		if (EC_PACKAGEP(target))
		{
			res->type    = PackageScope;
			res->package = target;
		} else if (EC_COMPILEDP(target))
		{
			res->type = FunctionScope;
		} else if (EC_CLASSP(target))
		{
			res->type = ClassScope;
		}
	}
	res->upper       = upper;
	res->target      = target;
	res->stackdim    = 0;
	res->maxstackdim = 0;
	res->symtab      = NULL;

	/* Set this as default compile environment */
	CCTXT(currentScope) = res;
	if (EC_NULLP(target))
	{
		res->package  = upper ? upper->package : EC_NIL;
		res->bytecode = upper ? upper->bytecode : EC_NIL;
		res->lframe   = upper ? upper->lframe : EC_NIL;
		res->functype = upper ? upper->functype : NormalFunction;
		functype = res->functype;
	} else
	{
		if (EC_PACKAGEP(target))
		{
			res->package  = target;
			res->bytecode = EC_PACKAGECODE(target);
			res->lframe   = EC_COMPILEDLFRAME(res->bytecode);
		} else if (EC_COMPILEDP(target))
		{
			res->package  = upper ? upper->package : EC_NIL;
			res->bytecode = target;
			res->lframe   = EC_COMPILEDLFRAME(res->bytecode);
		} else if (EC_CLASSP(target))
		{
			res->package  = upper ? upper->package : EC_NIL;
			/*
			 * Once upon a time, when lexical scope weren't working, bytecode was set to EC_NIL.
			 * Nowadays, bytecode is necessary to modify at run-time the compiled method !
			 *
			 * Other time is passed from the previous note, and now classes must have their own
			 * bytecode to allow initializations of class variables.
			 */
/*		    res->bytecode = EC_NIL; */
			ASSERT( upper && EC_COMPILEDP(upper->bytecode) );
/*		    res->bytecode = upper ? upper->bytecode : EC_NIL;*/
			res->bytecode = EC_CLASSCODE(target);
			res->lframe   = EC_CLASSLFRAME(target);
			ASSERT( EC_CLASSLFRAME(target) == EC_COMPILEDLFRAME(res->bytecode) );
		}
	}

	res->functype = functype;

	CCTXT(currentPackage)  = res->package;
	CCTXT(currentBytecode) = res->bytecode;
	CCTXT(currentLiteral)  = res->lframe;

	ASSERT( EC_NNULLP(CCTXT(currentPackage)) );
	ASSERT( EC_NNULLP(CCTXT(currentLiteral)) );

	PRIVATE(currentScope) = CCTXT(currentScope);


	switch (res->type)
	{
	case PackageScope:
	case FunctionScope:
		res->break_pos = ec_malloc( sizeof(ec_arena) );
		ASSERT( res->break_pos );
		res->break_lev = ec_malloc( sizeof(ec_arena) );
		ASSERT( res->break_lev );
		res->cycle_nesting = 0;
		res->continue_pos = ec_malloc( sizeof(ec_arena) );
		ASSERT( res->continue_pos );
		res->continue_lev = ec_malloc( sizeof(ec_arena) );
		ASSERT( res->continue_lev );
		res->cycle_nesting = 0;

		ec_arena_create( res->break_pos, 16 );
		ec_arena_create( res->break_lev, 16 );
		ec_arena_create( res->continue_pos, 16 );
		ec_arena_create( res->continue_lev, 16 );
		break;

	case BlockScope:
		/* Inherit */
		if (upper)
		{
			res->break_pos     = upper->break_pos;
			res->break_lev     = upper->break_lev;
			res->continue_pos  = upper->continue_pos;
			res->continue_lev  = upper->continue_lev;
			res->cycle_nesting = upper->cycle_nesting;
		}
		break;

	case ClassScope:
		/* Inherit */
		if (upper)
		{
			res->break_pos     = upper->break_pos;
			res->break_lev     = upper->break_lev;
			res->continue_pos  = upper->continue_pos;
			res->continue_lev  = upper->continue_lev;
			res->cycle_nesting = upper->cycle_nesting;
		}
		break;
	}

	return res;
}

static void closeScope( ec_compiler_ctxt ctxt, Scope scope )
{
	Scope upper;

	ASSERT( scope );

	upper = scope->upper;

	/* Restore previous compile environment */
	CCTXT(currentScope)    = upper;
	CCTXT(currentPackage)  = upper ? upper->package  : EC_NIL;
	CCTXT(currentBytecode) = upper ? upper->bytecode : EC_NIL;
	CCTXT(currentLiteral)  = upper ? upper->lframe   : EC_NIL;
	PRIVATE(currentScope) = CCTXT(currentScope);

	switch (scope->type)
	{
	case PackageScope:
	case FunctionScope:
		ec_arena_destroy( scope->break_pos );
		ec_arena_destroy( scope->break_lev );
		ec_arena_destroy( scope->continue_pos );
		ec_arena_destroy( scope->continue_lev );

		ec_free( scope->break_pos );
		ec_free( scope->break_lev );
		ec_free( scope->continue_pos );
		ec_free( scope->continue_lev );

		scope->break_pos     = NULL;
		scope->break_lev     = NULL;
		scope->continue_pos  = NULL;
		scope->continue_lev  = NULL;
		scope->cycle_nesting = 0;
		break;

	case BlockScope:
		/* Inherited, nothing to do */
		break;

	case ClassScope:
		/* Inherited, nothing to do */
		break;
	}

#if 0
	/* TODO XXXX TODO: handle the release of this thing ! (note that we need it after compilation !) */
	STableEntry entry, next;

	/* Free symbol table */
	entry = scope->symtab;
	while (entry)
	{
		next = entry->next;

		ec_free( entry );
		entry = next;
	}
	scope->symtab = NULL;

	ec_free( scope ); /* ?????? e ec_palloc ? */
#endif
}

static Scope getMasterScope( ec_compiler_ctxt ctxt, Scope scope )
{
	Scope res;

	res = scope;
	while (res && (res->type == BlockScope))
	{
		res = res->upper;
	}
	return res;
}

static Scope buildScopeForPackage( ec_compiler_ctxt ctxt, EC_OBJ package )
{
	/*
	 * Build a Scope for a package, filling it with public symbols only.
	 * Used to build environments for EcCompileString().
	 */

	Scope scope;

	EcInt i;
	struct ec_public_sym *psym;
	EcUInt symid;
	EcInt  pos;

	ASSERT( EC_PACKAGEP(package) );

	scope = openScope( ctxt, CCTXT(currentScope),
					   package, NormalFunction );

	for (i = 0; i < EC_PACKAGENEXPORT(package); i++)
	{
		psym = &EC_PACKAGEEXPORT(package)[i];

		symid = psym->sym;										/* Symbol ID for the exported name     */
		pos   = psym->pos;										/* Position in the literal frame array */

		addSymbolItem( ctxt, scope, symid, pos, SymbolPublic, psym->isconst );
	}

	return scope;
}


static EcInt getMaxPosition( ec_compiler_ctxt ctxt, Scope master, Scope scope, SymbolClass symclass )
{
	Scope       sc, outer;
	STableEntry entry;
	EcInt       pos;

	ASSERT( master );
	ASSERT( scope );

	pos = -1;

	outer = master->upper;

	sc = scope;
	while (sc && (sc != outer))
	{
		entry = sc->symtab;
		while (entry)
		{
			if (entry->symclass == symclass)
			{
				if (entry->position > pos)
					pos = entry->position;
			}

			entry = entry->next;
		}

		sc = sc->upper;
	}

	return pos;
}

static STableEntry addSymbolItem( ec_compiler_ctxt ctxt, Scope scope,
								  EcUInt symid, EcInt position,
								  SymbolClass symclass, EcBool isconst )
{
	STableEntry entry;

	/* Add to head */
	entry = ec_palloc( PRIVATE(nodePool), sizeof(struct STableEntryStruct) );
	if (! entry) return NULL;

	entry->symid       = symid;
	entry->symclass    = symclass;
	entry->isconst     = isconst;
	entry->initialized = (symclass == SymbolParam) ? TRUE : FALSE;
	entry->position    = position;								/* Stack position at run-time */
	entry->next        = scope->symtab;
	scope->symtab      = entry;

	return entry;
}

static STableEntry addSymbol( ec_compiler_ctxt ctxt, Scope scope, EcUInt symid, SymbolClass symclass, EcBool isconst )
{
	STableEntry entry;
	EcInt pos;
	Scope master;

	ASSERT( scope );

	master = getMasterScope( ctxt, scope );
	ASSERT( master );

	/* Check if already present */
	entry = scope->symtab;
	while (entry)
	{
		/* Check to see if symbol is already present */
		if (entry->symid == symid)
		{
			/* Duplicate symbol !!! */
			EcCompileError( -1, -1, "duplicate symbol `%k'", symid );
			EcAlert( EcError,
					 "(LINE: --, COLUMN: --) duplicate symbol `%k'", symid );
		}
		entry = entry->next;
	}

	/* Get position */
	pos = getMaxPosition( ctxt, master, scope, symclass ) + 1;

	/* Add to head */
	entry = ec_palloc( PRIVATE(nodePool), sizeof(struct STableEntryStruct) );
	if (! entry) return NULL;
	entry->symid       = symid;
	entry->symclass    = symclass;
	entry->isconst     = isconst;
	entry->initialized = (symclass == SymbolParam) ? TRUE : FALSE;
	entry->position    = pos;										/* Stack position at run-time */
	entry->next        = scope->symtab;
	scope->symtab      = entry;

	ASSERT( master );

	switch (symclass)
	{
	case SymbolParam:
		ASSERT( master == scope );
		ASSERT( master->type == FunctionScope );
		entry->position = pos;
		break;

	case SymbolLocal:
		switch (master->type)
		{
		case PackageScope:
		case FunctionScope:
			/* Update number of local variables in current bytecode */
/*			STACKGROW( scope, 1 );*/
			EC_COMPILEDNLOC(CCTXT(currentBytecode))++;
/*			ASSERT( EC_COMPILEDMAXTEMPS(CCTXT(currentBytecode)) >= 0 );*/
			entry->position = pos;
			break;
		case BlockScope:
			ASSERT( FALSE );
			break;
		case ClassScope:
			/* Add an instance variable, getting its position */
			entry->position = EcAddInstanceVariable( scope->target, EcSymbolAt( symid ) );
			ASSERT( entry->position == pos );
			break;
		}
		break;

	case SymbolStatic:
		ASSERT( (master->type == FunctionScope) || (master->type == ClassScope) );
		switch (master->type)
		{
		case PackageScope:
			ASSERT( FALSE );
			break;

		case FunctionScope:
			/* Push a dummy object in the literal frame to get its position */
			/*entry->position = EcArrayPush( CCTXT(currentLiteral), EC_NIL );*/
			entry->position = EcArrayPush( CCTXT(currentLiteral), EcMakeUndefined() );
			break;

		case BlockScope:
			ASSERT( FALSE );
			break;

		case ClassScope:
			/* Add a class variable, getting its position */
			entry->position = EcAddClassVariable( scope->target, EcSymbolAt( symid ) );
			break;
		}
		break;

	case SymbolPrivate:
		ASSERT( master->type == PackageScope );
		/* A package private variable */
		entry->position = EcPackageVariable( scope->target, EcSymbolAt( symid ), EC_NIL, FALSE, isconst );
		break;

	case SymbolPublic:
		ASSERT( master->type == PackageScope );
		/* A package public variable */
		entry->position = EcPackageVariable( scope->target, EcSymbolAt( symid ), EC_NIL, TRUE, isconst );
		break;
	}

	return entry;
}

static EcBool getSymbol( ec_compiler_ctxt ctxt, Scope scope, EcQualifiedSymbol *qsym, EcVarRef results )
{
	if (results)
	{
		initVInfo( ctxt, results );
		V_ENTRY(results)       = NULL;
		V_INITIALIZED(results) = FALSE;
	}

	if (QSLEN(qsym) == 1)
	{
		/* Symbol is unqualified. Search it in local package first. */
		if (getSymbolHere( ctxt, scope, QSCOMP(qsym, 0), results ))
			return TRUE;

		/* Then in imported packages in which we have unqualified symbols */
		if (getForeignSymbol( ctxt, QSCOMP(qsym, 0), results ))
			return TRUE;
		return FALSE;
	}
	else
	{
		/* Symbol qualified. */
		if (results)
			V_CLASS(results) = SymbolPublic;
		return getQualifiedSymbol( ctxt, scope, qsym, results );
	}
}

/* Search the symbol in the local package */
static EcBool getSymbolHere( ec_compiler_ctxt ctxt, Scope scope, EcUInt symid, EcVarRef results )
{
	STableEntry entry = NULL;
	Scope declscope   = NULL;
	EcInt u,													/* up                        */
		  p = -1;												/* ordinal position in scope */
	EcBool found = FALSE;

	ASSERT( scope );

#if EC_DEBUG_COMPILATION
	fprintf( stderr, "getSymbolHere: id:%ld  name:`%s'\n", symid, EcSymbolAt( symid ) );
	fprintf( stderr, "        scope: 0x%08lX\n", (unsigned long)scope );
#endif

	u = 0;
	declscope = scope;
	while (declscope)
	{
		p     = -1;
		found = FALSE;
		entry = declscope->symtab;
		while (entry)
		{
			p++;
#if EC_DEBUG_COMPILATION
/*			fprintf( stderr, "symid: %ld  name:`%s'\n", entry->symid, EcSymbolAt( entry->symid ) );*/
#endif
			if (entry->symid == symid)
			{
				found = TRUE;
				break;
			}

			entry = entry->next;
		}
		if (found) break;

		if (getMasterScope( ctxt, declscope ) == declscope)
			u = u + 1;											/* count only master scopes */
		declscope = declscope->upper;
	}

	if (! found)
	{
		if (results)
			initVInfo( ctxt, results );

		return FALSE;
	}

	ASSERT( declscope );

	/* Ok. Found. */
	ASSERT( entry );

	if (results)
	{
		V_SCOPE(results) = declscope;
		V_ENTRY(results) = entry;
		V_CLASS(results) = entry->symclass;
		if ((entry->symclass != SymbolPrivate) && (entry->symclass != SymbolPublic))
			V_UP(results) = u;
		else
			V_PKG(results) = _ec_package_position_o( CCTXT(currentPackage) );
		V_POS(results)         = entry->position;
		V_CONST(results)       = entry->isconst;
		V_INITIALIZED(results) = entry->initialized;
	}

	return TRUE;
}

/* Search the unqualified symbol */
static EcBool getForeignSymbol( ec_compiler_ctxt ctxt, EcUInt symid, EcVarRef results )
{
	EC_OBJ pkgObj;
	EC_OBJ pair, symlist;
	EcBool found, imported;
	EcInt  pair_first, pair_end;
	EcInt  i, j;


#if EC_DEBUG_COMPILATION
	fprintf( stderr, "getForeignSymbol:   id:%ld  name:`%s'\n", symid, EcSymbolAt( symid ) );
#endif

	if (results)
	{
		initVInfo( ctxt, results );
		V_CLASS(results) = SymbolPublic;
	}

	i = 0;
	while (i < PRIVATE(npackages)) {
		pkgObj = PRIVATE(package)[i].obj;

		/* Are we importing this package ? */
		/*
		 * NOTE: We could be importing this package multiple times, each time with
		 *       different options. So we must not rely only on the first importing
		 *       rule (that could be too restrictive), but must check also the
		 *       others.
		 */
		pair_first = 0;
		pair_end   = EC_ARRAYLEN(EC_PACKAGEIMPORT(CCTXT(currentPackage)));

		while (pair_first < pair_end)
		{
			imported = FALSE;
			pair     = EC_NIL;
			for (j = pair_first; j < pair_end; j++) {
				pair_first = j + 1;								/* If we must restart, start from here */
				pair = EcArrayGet( EC_PACKAGEIMPORT(CCTXT(currentPackage)), j );
				if (EcArrayGet( pair, 0 ) == pkgObj)
				{
					imported = TRUE;
					break;
				}
			}

			if (! imported) goto restart;

			/* Ok, the package is imported. But the symbol is present in this package ? */
			found = FALSE;
			for (j = 0; j < EC_PACKAGENEXPORT(pkgObj); j++)
			{
				if (EC_PACKAGEEXPORT(pkgObj)[j].sym == symid)
				{
					if (results)
					{
						V_POS(results)   = j;
						V_CONST(results) = EC_PACKAGEEXPORT(pkgObj)[j].isconst;
						V_INITIALIZED(results) = TRUE;
					}
					found = TRUE;
					break;
				}
			}

			if (! found) goto restart;

			/* The package is imported, and the symbol is exported. */
			/* But are we authorized to use this symbol ?           */
			ASSERT( EC_ARRAYP(pair) );
			symlist = EcArrayGet( pair, 1 );
			if (! (EC_BOOLP(symlist) || EC_ARRAYP(symlist)))	/* NO !  */
			{
				if (results)
				{
					V_PKG(results)         = -1;
					V_POS(results)         = -1;
					V_CONST(results)       = FALSE;
					V_INITIALIZED(results) = FALSE;
				}
			}
			if (EC_BOOLP(symlist) && EC_BOOLVAL(symlist))		/* YES ! */
			{
				if (results)
				{
					ASSERT( V_POS(results) >= 0 );
					V_PKG(results)         = _ec_package_position_o( pkgObj );
					V_CONST(results)       = EC_PACKAGEEXPORT(pkgObj)[V_POS(results)].isconst;
					V_INITIALIZED(results) = TRUE;
				}
				return TRUE;
			}

			/* It depends ! */
			if (EC_ARRAYP(symlist))
			{
				ASSERT( EC_ARRAYP(symlist) );
				for (j = 0; j < EC_ARRAYLEN(symlist); j++)
				{
					if (EC_SYMBOL(EcArrayGet( symlist, j )) == symid) /* YES ! */
					{
						if (results)
						{
							ASSERT( V_POS(results) >= 0 );
							V_PKG(results)   = _ec_package_position_o( pkgObj );
							V_CONST(results) = EC_PACKAGEEXPORT(pkgObj)[V_POS(results)].isconst;
							V_INITIALIZED(results) = TRUE;
						}
						return TRUE;
					}
				}
			}

			/* Retry, if possible */
		}

	restart:
		i++;
	}

	return FALSE;
}

/* Search the qualified symbol */
static EcBool getQualifiedSymbol( ec_compiler_ctxt ctxt, Scope scope, EcQualifiedSymbol *qsym, EcVarRef results )
{
	ec_string  fullsymbol;
	ec_string  pkgname;
	char      *qsymbol = NULL;

	EC_OBJ pkgObj;
	EC_OBJ pair;
	EcBool imported;
	EcUInt symid;
	EcInt i;

	EcBool res = FALSE;


	initVInfo( ctxt, results );

	qsymbol = EcQualifiedString( qsym );

	ec_string_init( &fullsymbol, qsymbol );
	ec_string_init( &pkgname, NULL );

	EcNamePrefix( &pkgname, &fullsymbol );

#if EC_DEBUG_COMPILATION
	fprintf( stderr, "getQualifiedSymbol:   fullsymbol:`%s'  pkgname: '%s'\n", ec_strdata( &fullsymbol ), ec_strdata( &pkgname ) );
#endif

	pkgObj = _ec_package_loaded( ec_strdata( &pkgname ) );
	if (EC_NULLP(pkgObj))
	{
		res = FALSE;
		goto finish;
	}

	V_PKG(results) = _ec_package_position( ec_strdata( &pkgname ) );
	if (V_PKG(results) == -1)
	{
		res = FALSE;
		goto finish;
	}

	if (pkgObj == CCTXT(currentPackage))
	{
		res = getSymbolHere( ctxt, scope, QSCOMP(qsym, QSLEN(qsym)-1), results );
		goto finish;
	}

	/* Ok, the package is there. But is it imported by our current package ? */
	imported = FALSE;
	ASSERT( EC_ARRAYP(EC_PACKAGEIMPORT(CCTXT(currentPackage))) );
	for (i = 0; i < EC_ARRAYLEN(EC_PACKAGEIMPORT(CCTXT(currentPackage))); i++) {
		pair = EcArrayGet( EC_PACKAGEIMPORT(CCTXT(currentPackage)), i );
		if (EcArrayGet( pair, 0 ) == pkgObj)
		{
			imported = TRUE;
			break;
		}
	}
	if (! imported)
	{
		res = FALSE;
		goto finish;
	}

	symid = QSCOMP(qsym, QSLEN(qsym)-1);
	for (i = 0; i < EC_PACKAGENEXPORT(pkgObj); i++)
	{
		if (EC_PACKAGEEXPORT(pkgObj)[i].sym == symid)
		{
			V_CLASS(results)       = SymbolPublic;
			V_POS(results)         = EC_PACKAGEEXPORT(pkgObj)[i].pos; /* BUG: was 'i' */
			V_CONST(results)       = EC_PACKAGEEXPORT(pkgObj)[i].isconst;
			V_INITIALIZED(results) = TRUE;
			res = TRUE;
			goto finish;
		}
	}

	res = FALSE;

finish:
	ec_free( qsymbol );
	ec_string_cleanup( &fullsymbol );
	ec_string_cleanup( &pkgname );
	return res;
}

static EC_OBJ getLiteral( ec_compiler_ctxt ctxt, Scope scope, EcInt up )
{
	Scope master;
	EcInt i;

	for (i = 0; (i < up) && scope; i++)
	{
		scope = scope->upper;
	}

	if (! scope)
		return EC_NIL;

	master = getMasterScope( ctxt, scope );

	switch (master->type)
	{
	case PackageScope:
		return EC_COMPILEDLFRAME(EC_PACKAGECODE(scope->target));
		break;

	case FunctionScope:
		return EC_COMPILEDLFRAME(scope->target);
		break;

	case BlockScope:
		ASSERT( FALSE );
		break;

	case ClassScope:
		return EC_CLASSLFRAME(scope->target);
		break;
	}

	ASSERT( FALSE );
	return EC_NIL;
}

static EC_OBJ getPackageFrame( ec_compiler_ctxt ctxt, EcInt pkg )
{
	return EC_PACKAGEFRAME(PRIVATE(package)[pkg].obj);
}


static void initVInfo( ec_compiler_ctxt ctxt, EcVarRef ref )
{
	memset( ref, 0x00, sizeof(EcVarRefStruct) );
	V_CLASS(ref)       = SymbolLocal;
	V_PKG(ref)         = -1;
	V_UP(ref)          = -1;
	V_POS(ref)         = -1;
	V_CONST(ref)       = FALSE;
	V_INITIALIZED(ref) = FALSE;
	V_INDEXED(ref)     = FALSE;
	V_SCOPE(ref)       = NULL;
	V_ENTRY(ref)       = NULL;
}

static void saveVInfo( ec_compiler_ctxt ctxt, EcVarRef dst, EcVarRef src )
{
	memcpy( dst, src, sizeof(EcVarRefStruct) );
}

#if EC_DEBUG_SCOPE
static void dumpScope( ec_compiler_ctxt ctxt, Scope scope )
{
	const char *stype;
	const char *ftype;
	const char *sclass;
	STableEntry stentry;

	switch (scope->type)
	{
	case PackageScope:
		stype = "Package";
		break;
	case FunctionScope:
		stype = "Function";
		break;
	case BlockScope:
		stype = "Block";
		break;
	case ClassScope:
		stype = "Class";
		break;
	}

	switch (scope->functype)
	{
	case NormalFunction:
		ftype = "Normal";
		break;
	case MethodFunction:
		ftype = "Method";
		break;
	case ClassMethodFunction:
		ftype = "Class Method";
		break;
	}

	ec_fprintf( stderr, "\nScope:\n" );
	ec_fprintf( stderr, "\tType         : %s\n", stype );
	ec_fprintf( stderr, "\tPackage      : %w\n", scope->package );
	ec_fprintf( stderr, "\tTarget       : %r\n", scope->target );
	ec_fprintf( stderr, "\tBytecode     : %r\n", scope->bytecode );
	ec_fprintf( stderr, "\tLFrame       : %r\n", scope->lframe );
	ec_fprintf( stderr, "\tFunction Type: %s\n", ftype );

	ec_fprintf( stderr, "\n" );
	ec_fprintf( stderr, "\tStack dim    : %I\n", scope->stackdim );
	ec_fprintf( stderr, "\tStack max dim: %I\n", scope->maxstackdim );

	ec_fprintf( stderr, "\tSymbol table :\n" );
	for (stentry = scope->symtab; stentry; stentry = stentry->next)
	{
		switch (stentry->symclass)
		{
		case SymbolParam:
			sclass = "parameter";
			break;
		case SymbolLocal:
			sclass = "local";
			break;
		case SymbolStatic:
			sclass = "static";
			break;
		case SymbolPrivate:
			sclass = "private";
			break;
		case SymbolPublic:
			sclass = "public";
			break;
		}
		ec_fprintf( stderr, "\t  %s %k (const: %b, initialized: %b, pos: %I)\n",
					sclass, stentry->symid,
					stentry->isconst, stentry->initialized, stentry->position );
	}

	ec_fprintf( stderr, "\n" );
	ec_fprintf( stderr, "\tNext serv v. : %I\n", (EcInt)scope->next_srv_var );
}
#endif

#if PRINTVINFO
static const char *classname( SymbolClass symclass )
{
	switch (symclass)
	{
	case SymbolParam:   return "Parameter";
	case SymbolLocal:   return "Local";
	case SymbolStatic:  return "Static";
	case SymbolPrivate: return "Private";
	case SymbolPublic:  return "Public";
	}

	return "UNKNOWN";
}

static void printVInfo( EcVarRef ref )
{
	fprintf( stderr, "VARIABLE  : '%s'\n", V_ENTRY(ref) ? EcSymbolAt( V_ENTRY(ref)->symid ) : "--" );
	fprintf( stderr, "     Class: %s\n",   classname( V_CLASS(ref) ) );
	fprintf( stderr, "       pkg: %ld\n",  (long)V_PKG(ref) );
	fprintf( stderr, "        up: %ld\n",  (long)V_UP(ref) );
	fprintf( stderr, "       pos: %ld\n",  (long)V_POS(ref) );
	fprintf( stderr, "     const: %s\n",   V_CONST(ref) ? "YES" : "NO" );
	fprintf( stderr, "      init: %s\n",   V_INITIALIZED(ref) ? "YES" : "NO" );
	fprintf( stderr, "   indexed: %s\n",   V_INDEXED(ref) ? "YES" : "NO" );
}
#endif

/*
 * For each bytecode object we keep a list of 'labelinfo' structures,
 * one for each label.
 */
typedef struct labelinfo_struct
{
	EcUInt  labelid;											/* Symbol id for the label          */
	EcInt   pos;												/* Bytecode position of label       */
	ec_list ref;												/* List of references to this label */
} labelinfo;

#define LI_ID(linfo)		((linfo)->labelid)
#define LI_POS(linfo)		((linfo)->pos)
#define LI_REF(linfo)		((linfo)->ref)

/*
 * Definitions for ec_compiler_ctxt `labels' hash mapping bytecode to a list of labels.
 * Each list is an ec_list with key label symid and data a labelinfo pointer.
 */

#define INVALID_BYTECODE ((EC_OBJ)0xFFFFFFFF)
#define INVALID_LIST     ((ec_list)0xFFFFFFFF)

static EcUInt hash_pointer( EcAny objp ) { return (EcUInt)(EcPointerInteger) objp; }

static ec_hash_def hash_objp2list =
{
        (ec_lambda_hash)    hash_pointer,

        (ec_lambda_copy)    NULL,
        (ec_lambda_compare) NULL,								/* pointer comparison      */
        (ec_lambda_destroy) NULL,								/* don't destroy objects ! */

        (ec_lambda_copy)    NULL,
        (ec_lambda_compare) NULL,
        (ec_lambda_destroy) NULL, /*ec_list_destroy,*/

        (EcAny)             INVALID_BYTECODE,
        (EcAny)             INVALID_LIST
};

#if 0
static void print_linfo_list( ec_list linfo_list );
static void print_linfo( labelinfo *linfo );
#endif

static labelinfo *labelinfo_create( ec_compiler_ctxt ctxt, EcUInt labelid );
static void       labelinfo_destroy( ec_compiler_ctxt ctxt, labelinfo *linfo );
static labelinfo *labelinfo_get( ec_compiler_ctxt ctxt, EC_OBJ bytecode, EcUInt labelid, EcBool create );

static ec_list produced_bytecode( ec_compiler_ctxt ctxt )
{
	ec_hash_iterator hiter;
	EcAny            any;
	ec_list          bytecode_list;
	EC_OBJ           bytecode;

	if (! CCTXT(labels)) return NULL;

	bytecode_list = ec_list_create();
	ASSERT( bytecode_list );
	if (! bytecode_list) return NULL;							/* :TODO: XXX argh ! */

	hiter = ec_hash_iterator_create( CCTXT(labels) );
	ASSERT( hiter );
	while (ec_hash_iterator_next( hiter, &any ))
	{
		bytecode = (EC_OBJ) any;
		ec_list_append( bytecode_list, (EcAny) NULL, (EcAny) bytecode );
	}
	ec_hash_iterator_destroy( hiter );

	return bytecode_list;
}

/* Define a label in current bytecode and current position. */
static EcBool define_label( ec_compiler_ctxt ctxt, EcUInt labelid )
{
	labelinfo *linfo;

	/*ec_fprintf( stderr, "define_label \"%k\" (bc: %w)\n", labelid, CCTXT(currentBytecode) );*/
	/*print_labels();*/

	/* check if labelinfo exists, eventually creating it */
	linfo = labelinfo_get( ctxt, CCTXT(currentBytecode), labelid, TRUE );
	ASSERT( linfo );
	if (! linfo) return FALSE;									/* :TODO: XXX argh ! */

	/* set current position */
	LI_POS(linfo) = EC_COMPILEDNCODE(CCTXT(currentBytecode));

	/*ec_fprintf( stderr, "define_label \"%k\" (pos: %ld, bc: %w) --> OK\n\n", labelid, (long)LI_POS(linfo), CCTXT(currentBytecode) );*/
	/*print_labels();*/

	return TRUE;
}

static EcInt label_position( ec_compiler_ctxt ctxt, EC_OBJ bytecode, EcUInt labelid )
{
	labelinfo *linfo;

	linfo = labelinfo_get( ctxt, bytecode, labelid, FALSE );
	if (! linfo) return -1;

	return LI_POS(linfo);
}

/* Add a label reference in current bytecode at specified position. */
static EcBool reference_label( ec_compiler_ctxt ctxt, EcUInt labelid, EcInt position )
{
	labelinfo *linfo;
	EcInt res;
	EcAny any;

	/*ec_fprintf( stderr, "reference_label \"%k\" (pos: %ld, bc: %w)\n", labelid, (long)position, CCTXT(currentBytecode) );*/

	/* get a labelinfo, eventually creating it */
	linfo = labelinfo_get( ctxt, CCTXT(currentBytecode), labelid, TRUE );
	if (! linfo) return FALSE;

	res = ec_hash_get( CCTXT(labels), (EcAny) CCTXT(currentBytecode), &any );
	ASSERT( res );
	ASSERT( (ec_list)any != INVALID_LIST );
	/*ec_fprintf( stderr, "  list: 0x%08lX\n", (unsigned long) any );*/

	/* add a reference */
	if (! LI_REF(linfo))
	{
		LI_REF(linfo) = ec_list_create();
		if (! LI_REF(linfo))
			return FALSE;
	}
	if (ec_list_append( LI_REF(linfo), (EcAny)(EcPointerInteger) 0, (EcAny)(EcPointerInteger) position ))
		return TRUE;
	return FALSE;
}

/* Returns a list of referenced labels in specified bytecode. */
static ec_list referenced_labels( ec_compiler_ctxt ctxt, EC_OBJ bytecode )
{
	EcAny   any;
	ec_list linfo_list;

	if (! CCTXT(labels))
		return NULL;

	if (! ec_hash_get( CCTXT(labels), bytecode, &any ))
		return NULL;
	linfo_list = (ec_list) any;

	/* a list of labelinfo is a list of labels when observing only keys ! */
	return ec_list_copy( linfo_list );
}

/* Returns label references in specified bytecode. */
static ec_list label_references( ec_compiler_ctxt ctxt, EcUInt labelid, EC_OBJ bytecode )
{
	labelinfo *linfo;

	linfo = labelinfo_get( ctxt, bytecode, labelid, FALSE );
	if (! linfo) return NULL;

	return ec_list_copy( LI_REF(linfo) );
}

static labelinfo *labelinfo_create( ec_compiler_ctxt ctxt, EcUInt labelid )
{
	labelinfo *linfo;

	linfo = (labelinfo *) ec_malloc( sizeof(struct labelinfo_struct) );
	if (! linfo) return NULL;

	LI_ID(linfo)  = labelid;
	LI_POS(linfo) = -1;
	LI_REF(linfo) = NULL;
	return linfo;
}

static void labelinfo_destroy( ec_compiler_ctxt ctxt, labelinfo *linfo )
{
	if (LI_REF(linfo))
		ec_list_destroy( LI_REF(linfo) );
	LI_REF(linfo) = NULL;
	ec_free( linfo );
}

static labelinfo *labelinfo_get( ec_compiler_ctxt ctxt, EC_OBJ bytecode, EcUInt labelid, EcBool create )
{
	EcAny      any;
	ec_list    linfo_list;
	labelinfo *linfo;
	EcBool res;

	/*ec_fprintf( stderr, "\tlabelinfo_get  \"%k\" bc: %w  create: %d\n", labelid, bytecode, (int)create );*/

	if ((! CCTXT(labels)) && create)
	{
		/* Create hash table */
		CCTXT(labels) = ec_hash_create( 0, 0, 0, &hash_objp2list );
		ASSERT( CCTXT(labels) );
		if (! CCTXT(labels)) return NULL;								/* :TODO: XXX argh ! */
	}

	if (! CCTXT(labels))
		return NULL;

	res = ec_hash_get( CCTXT(labels), (EcAny) bytecode, &any );
	if (res)
		linfo_list = (ec_list) any;
	if (! res)
	{
		if (! create) return NULL;

		linfo_list = ec_list_create();
		ASSERT( linfo_list );
		if (! linfo_list) return NULL;							/* :TODO: XXX argh ! */
		ec_hash_set( CCTXT(labels), (EcAny) bytecode, (EcAny) linfo_list );
		res = ec_hash_get( CCTXT(labels), (EcAny) bytecode, &any );
		ASSERT( res );
		ASSERT( linfo_list == (ec_list)any );
	}

	res = ec_list_finddata( linfo_list, (EcAny)(EcPointerInteger) labelid, &any );
	if (res)
	{
		linfo = (labelinfo *) any;
		return linfo;
	}
	if (! create) return NULL;

	ASSERT( create );
	linfo = labelinfo_create( ctxt, labelid );
	ASSERT( linfo );
	if (! linfo) return NULL;									/* :TODO: XXX argh ! */
	ec_list_prepend( linfo_list, (EcAny)(EcPointerInteger) labelid, (EcAny)(EcPointerInteger) linfo );
	return linfo;
}

static void labels_cleanup( ec_compiler_ctxt ctxt )
{
	/* cleanup labels hash */
	ec_hash_iterator hiter;
	EC_OBJ           bytecode;
	ec_list          linfo_list;
	ec_list_iterator linfo_iter;
	ec_list_node     linfo_node;
	EcAny            any;

	if (! CCTXT(labels)) return;

	ASSERT( CCTXT(labels) );
	hiter = ec_hash_iterator_create( CCTXT(labels) );
	ASSERT( hiter );
	while (ec_hash_iterator_next( hiter, &any ))
	{
		bytecode = (EC_OBJ) any;
		if (! ec_hash_get( CCTXT(labels), (EcAny) bytecode, &any ))
			continue;
		linfo_list = (ec_list) any;

		/* cleanup labelinfo list */
		linfo_iter = ec_list_iterator_create( linfo_list );
		while ((linfo_node = ec_list_iterator_next(linfo_iter)))
			labelinfo_destroy( ctxt, (labelinfo *)ec_list_data( linfo_node ) );
		ec_list_iterator_destroy( linfo_iter );

		/* destroy list */
		ec_list_destroy( linfo_list );
	}
	ec_hash_iterator_destroy( hiter );

	/*
	 * destroy hash table
	 *
	 * (NOOO: this will automatically destroy lists)
	 */
	ec_hash_destroy( CCTXT(labels) );
	CCTXT(labels) = NULL;
}

#if 0
static void print_labels( ec_compiler_ctxt ctxt )
{
	ec_fprintf( stderr, "labels: %%{\n" );
	if (CCTXT(labels))
	{
		ec_hash_iterator hiter;
		EC_OBJ           bytecode;
		EcAny            any;
		ec_list          linfo_list;

		hiter = ec_hash_iterator_create( CCTXT(labels) );
		ASSERT( hiter );
		while (ec_hash_iterator_next( hiter, &any ))
		{
			bytecode = (EC_OBJ) any;
			ec_fprintf( stderr, "\tbc: %w\n", bytecode );
			ec_fprintf(stderr, "\tlabels: 0x%08lX  bytecode: 0x%08lX\n", (unsigned long)CCTXT(labels), (unsigned long)bytecode);
			if (! ec_hash_get( CCTXT(labels), (EcAny) bytecode, &any ))
				continue;
			linfo_list = (ec_list) any;
			print_linfo_list( linfo_list );
		}
	}
	ec_fprintf( stderr, "}%%\n\n" );
}

static void print_linfo_list( ec_list linfo_list )
{
	ec_list_iterator linfo_iter;
	ec_list_node     linfo_node;

	ec_fprintf( stderr, "\tlinfo_list: {\n" );
	if (linfo_list)
	{
		linfo_iter = ec_list_iterator_create( linfo_list );
		while ((linfo_node = ec_list_iterator_next(linfo_iter)))
			print_linfo( ec_list_data(linfo_node) );
		ec_list_iterator_destroy( linfo_iter );
	}
	ec_fprintf( stderr, "\t}\n" );
}

static void print_linfo( labelinfo *linfo )
{
	ec_list_iterator iter;
	ec_list_node     node;
	EcBool first = TRUE;

	ec_fprintf( stderr, "\t\t(linfo: \"%k\", %ld, (", LI_ID(linfo), (unsigned long)LI_POS(linfo) );
	if (LI_REF(linfo))
	{
		iter = ec_list_iterator_create( LI_REF(linfo) );
		while ((node = ec_list_iterator_next(iter)))
		{
			if (! first) ec_fprintf( stderr, ", " );
			ec_fprintf( stderr, "%ld", ec_list_data(node) );
			first = FALSE;
		}
		ec_list_iterator_destroy( iter );
	}
	ec_fprintf( stderr, ")\n" );
}
#endif
