/* ==========================================================================
 * ast.c
 * ==========================================================================
 * Abstract Syntax Tree.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat May 23 20:34:10 MEST 1998
 *
 *   $Id: ast.c,v 1.6 2005/03/22 10:09:33 panta Exp $
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

#include "ast.h"
#include "compile.h"
#include "elastic.h"
#include "compat.h"
#include "private.h"

#include "debug.h"


#if defined(WITH_STDIO) && EC_AST_DEBUG
/* AST Printing */

static Scope  currentScope = NULL;
static EC_OBJ currentPackage;
static EC_OBJ currentBytecode;
static EC_OBJ currentLiteral;

static void printChar( char ch );
static void printString( const char *strng );

static void printSymbol( int lev, ASTNode node );
static void printQualifiedSymbol( int lev, ASTNode node );
static void printConstant( int lev, ASTNode node );
static void printVariable( int lev, ASTNode node );
static void printArrayCons( int lev, ASTNode node );
static void printHashCons( int lev, ASTNode node );
static void printUnary( int lev, ASTNode node );
static void printBinary( int lev, ASTNode node );
static void printConditional( int lev, ASTNode node );
static void printOr( int lev, ASTNode node );
static void printAnd( int lev, ASTNode node );
static void printAssign( int lev, ASTNode node );
static void printSimAssign( int lev, ASTNode node );
static void printArrayRef( int lev, ASTNode node );
static void printDecl( int lev, ASTNode node );
static void printDeclAtom( int lev, ASTNode node );
static void printStatement( int lev, ASTNode node );
static void printLabeledStmt( int lev, ASTNode node );
static void printExprStmt( int lev, ASTNode node );
static void printIf( int lev, ASTNode node );
static void printWhile( int lev, ASTNode node );
static void printDo( int lev, ASTNode node );
static void printFor( int lev, ASTNode node );
static void printForIn( int lev, ASTNode node );
static void printBreak( int lev, ASTNode node );
static void printContinue( int lev, ASTNode node );
static void printGoto( int lev, ASTNode node );
static void printTry( int lev, ASTNode node );
static void printCatch( int lev, ASTNode node );
static void printThrow( int lev, ASTNode node );
static void printImport( int lev, ASTNode node );
static void printParam( int lev, ASTNode node );
static void printParamList( int lev, ASTNode node );
static void printCall( int lev, ASTNode node );
static void printMethodCall( int lev, ASTNode node );
static void printStmtList( int lev, ASTNode node );
static void printFunction( int lev, ASTNode node );
static void printReturn( int lev, ASTNode node );
static void printClass( int lev, ASTNode node );
static void printMethod( int lev, ASTNode node );
static void printPackage( int lev, ASTNode node );
#endif /* end of defined(WITH_STDIO) && EC_AST_DEBUG */

/* ========================================================================
 * A P I
 * ======================================================================== */

ASTNode ASTNodeAdd( ASTNodeType type )
{
	ASTNode res;

	if (! PRIVATE(nodePool))
		PRIVATE(nodePool) = ec_allocpool( sizeof(struct ASTNodeStruct), -1 ); /* use platform alignment */

	if (! PRIVATE(nodePool)) return NULL;

	res = ec_palloc( PRIVATE(nodePool), sizeof(struct ASTNodeStruct) );
	ASSERT( res );
	if (! res) return NULL;

	memset( res, 0x00, sizeof(struct ASTNodeStruct) );
	res->tloc.first_line = 0;

	res->bytecode = EC_NIL;

	res->type = type;
	return res;
}

void ASTFree( void )
{
	ec_freepool( PRIVATE(nodePool) );
	PRIVATE(nodePool) = NULL;
}

ASTNode makeConstInt( EcInt number )
{
	ASTNode res;

	res = ASTNodeAdd( constExprType );
	res->vConstExpr.type = inumberConst;
	res->vConstExpr.vInt = number;

	return res;
}

ASTNode makeConstFloat( EcFloat number )
{
	ASTNode res;

	res = ASTNodeAdd( constExprType );
	res->vConstExpr.type = fnumberConst;
	res->vConstExpr.vFloat = number;

	return res;
}

ASTNode makeConstString( const char *string )
{
	ASTNode res;

	res = ASTNodeAdd( constExprType );
	res->vConstExpr.type = stringConst;
	res->vConstExpr.vString = ec_stringduppool( PRIVATE(nodePool), string );

	return res;
}

ASTNode makeConstChar( EcChar ch )
{
	ASTNode res;

	res = ASTNodeAdd( constExprType );
	res->vConstExpr.type  = charConst;
	res->vConstExpr.vChar = ch;

	return res;
}

ASTNode makeConstSym( EcUInt symid )
{
	ASTNode res;

	res = ASTNodeAdd( constExprType );
	res->vConstExpr.type  = symbolConst;
	res->vConstExpr.vSym = symid;

	return res;
}

ASTNode makeConstObject( EC_OBJ obj )
{
	ASTNode res;

	res = ASTNodeAdd( constExprType );
	res->vConstExpr.type = objectConst;
	res->vConstExpr.vObject = obj;

	return res;
}

ASTNode makeSymbol( EcUInt symid )
{
	ASTNode res;

	res = ASTNodeAdd( symbolType );
	res->vSymbol.symbolid = symid;

	return res;
}

ASTNode makeQualifiedSymbol( ASTNode prefix, ASTNode symbol )
{
	ASTNode res;

	ASSERT( symbol->type == symbolType );

	if (prefix)
		res = prefix;
	else
	{
		res = ASTNodeAdd( qualifiedSymbolType );
		EcQualifiedInit( QSYM(res) );
	}

	EcQualifiedAdd( QSYM(res), SYMID(symbol) );

	return res;
}

ASTNode makeVariable( ASTNode node )
{
	ASSERT( node->type == qualifiedSymbolType );
	node->type = variableExprType;
	return node;
}

ASTNode makeArrayCons( ASTNode arglist )
{
	ASTNode res;

	res = ASTNodeAdd( arrayConstructionExprType );
	res->vArrayConsExpr.arglist = arglist;

	return res;
}

ASTNode makeHashCons( ASTNode arglist, EcBool oldstyle )
{
	ASTNode res;

	res = ASTNodeAdd( hashConstructionExprType );
	res->vHashConsExpr.oldstyle = oldstyle;
	res->vHashConsExpr.arglist = arglist;

	return res;
}

ASTNode makeUnary( UnaryOp op, ASTNode operand )
{
	ASTNode res;

	res = ASTNodeAdd( unaryExprType );
	res->vUnaryExpr.op      = op;
	res->vUnaryExpr.operand = operand;

	return res;
}

ASTNode makeBinary( BinaryOp op, ASTNode first, ASTNode second )
{
	ASTNode res;

	res = ASTNodeAdd( binaryExprType );
	res->vBinaryExpr.op     = op;
	res->vBinaryExpr.first  = first;
	res->vBinaryExpr.second = second;

	return res;
}

ASTNode makeConditional( ASTNode cond, ASTNode texpr, ASTNode fexpr )
{
	ASTNode res;

	res = ASTNodeAdd( conditionalExprType );
	res->vCondExpr.cond  = cond;
	res->vCondExpr.texpr = texpr;
	res->vCondExpr.fexpr = fexpr;

	return res;
}

ASTNode makeOr( ASTNode first, ASTNode second )
{
	ASTNode res;

	res = ASTNodeAdd( orExprType );
	res->vOrExpr.first = first;
	res->vOrExpr.second = second;

	return res;
}

ASTNode makeAnd( ASTNode first, ASTNode second )
{
	ASTNode res;

	res = ASTNodeAdd( andExprType );
	res->vAndExpr.first = first;
	res->vAndExpr.second = second;

	return res;
}

ASTNode makeAssign( ASTNode lhs, ASTNode rhs )
{
	ASTNode res;

	res = ASTNodeAdd( assignExprType );
	res->vAssignExpr.lhs = lhs;
	res->vAssignExpr.rhs = rhs;

	return res;
}

ASTNode makeSimultaneousAssign( ASTNode lhs, ASTNode rhs )
{
	ASTNode res;

	res = ASTNodeAdd( simAssignExprType );
	res->vSimAssignExpr.lhs = lhs;
	res->vSimAssignExpr.rhs = rhs;

	return res;
}

ASTNode makeArrayRef( ASTNode ref, ASTNode idx )
{
	ASTNode res;

	res = ASTNodeAdd( arrayRefExprType );
	res->vArrayRefExpr.ref = ref;
	res->vArrayRefExpr.idx = idx;

	return res;
}

ASTNode makeDecl( SymbolClass symclass, ASTNode decllist )
{
	ASTNode res;

	res = ASTNodeAdd( declNodeType );
	res->vDecl.symclass = symclass;
	res->vDecl.decllist = decllist;

	return res;
}

ASTNode makeDeclAtom( ASTNode sym, ASTNode init )
{
	ASTNode res;

	res = ASTNodeAdd( declAtomNodeType );
	res->vDeclAtom.sym  = sym;
	res->vDeclAtom.init = init;

	return res;
}

ASTNode makeStmt( ASTNode stmt )
{
	ASTNode res;

	res = ASTNodeAdd( statementType );
	res->vStmt.stmt = stmt;

	return res;
}

ASTNode makeLabeledStmt( EcUInt label, ASTNode stmt )
{
	ASTNode res;
	char *labelstring;

	labelstring = (char *)EcSymbolAt( label );
	labelstring[strlen(labelstring)-1] = '\0';

	res = ASTNodeAdd( labeledStmtType );
	res->vLabeledStmt.label = EcInternSymbol( labelstring );
	res->vLabeledStmt.stmt  = stmt;

	return res;
}

ASTNode makeExprStmt( ASTNode expr )
{
	ASTNode res;

	if (expr)
	{
		res = ASTNodeAdd( exprStmtType );
		res->vExpr.expr = expr;
	} else
		res = NULL;

	return res;
}

ASTNode makeIf( ASTNode cond, ASTNode thenClause, ASTNode elseClause )
{
	ASTNode res;

	res = ASTNodeAdd( ifStmtType );
	res->vIfStmt.cond = cond;
	res->vIfStmt.thenClause = thenClause;
	res->vIfStmt.elseClause = elseClause;

	return res;
}

ASTNode makeWhile( ASTNode cond, ASTNode body )
{
	ASTNode res;

	res = ASTNodeAdd( whileStmtType );
	res->vWhileStmt.cond = cond;
	res->vWhileStmt.body = body;

	return res;
}

ASTNode makeDo( ASTNode cond, ASTNode body )
{
	ASTNode res;

	res = ASTNodeAdd( doStmtType );
	res->vDoStmt.cond = cond;
	res->vDoStmt.body = body;

	return res;
}

ASTNode makeFor( ASTNode init, ASTNode cond, ASTNode restart, ASTNode body )
{
	ASTNode res;

	res = ASTNodeAdd( forStmtType );
	res->vForStmt.init = init;
	res->vForStmt.cond = cond;
	res->vForStmt.restart = restart;
	res->vForStmt.body = body;

	return res;
}

ASTNode makeForIn( ASTNode var, ASTNode sequence, ASTNode body )
{
	ASTNode res;

	res = ASTNodeAdd( forInStmtType );
	res->vForInStmt.var      = var;
	res->vForInStmt.sequence = sequence;
	res->vForInStmt.body     = body;

	return res;
}

ASTNode makeBreak( ASTNode lev )
{
	ASTNode res;

	res = ASTNodeAdd( breakStmtType );
	res->vBreakStmt.lev = lev;

	return res;
}

ASTNode makeContinue( ASTNode lev )
{
	ASTNode res;

	res = ASTNodeAdd( continueStmtType );
	res->vContinueStmt.lev = lev;

	return res;
}

ASTNode makeGoto( ASTNode target )
{
	ASTNode res;

	res = ASTNodeAdd( gotoStmtType );
	res->vGotoStmt.target = target;

	return res;
}

ASTNode makeTry( ASTNode block, ASTNode catchlist )
{
	ASTNode res;

	res = ASTNodeAdd( tryStmtType );
	res->vTryStmt.block     = block;
	res->vTryStmt.catchlist = catchlist;

	return res;
}

ASTNode makeCatch( ASTNode type, ASTNode var, ASTNode handler )
{
	ASTNode res;

	res = ASTNodeAdd( catchStmtType );
	res->vCatchStmt.type    = type;
	res->vCatchStmt.var     = var;
	res->vCatchStmt.handler = handler;

	return res;
}

ASTNode makeThrow( ASTNode expr )
{
	ASTNode res;

	res = ASTNodeAdd( throwStmtType );
	res->vThrowStmt.expr = expr;

	return res;
}

ASTNode makeImport( EcBool detailed, ASTNode package, ASTNode symlist )
{
	ASTNode res;

	res = ASTNodeAdd( importStmtType );
	res->vImportStmt.detailed = detailed;
	res->vImportStmt.package  = package;
	res->vImportStmt.symlist  = symlist;

	return res;
}

ASTNode makeParam( ASTNode var, ASTNode init )
{
	ASTNode res;

	res = ASTNodeAdd( paramNodeType );
	res->vParam.var  = var;
	res->vParam.init = init;

	return res;
}

ASTNode makeParamList( EcBool varargs, ASTNode list )
{
	ASTNode res;

	res = ASTNodeAdd( paramListType );
	res->vParamList.varargs = varargs;
	res->vParamList.list    = list;

	return res;
}

ASTNode makeCall( ASTNode fnct, ASTNode arglist )
{
	ASTNode res;

	res = ASTNodeAdd( callNodeType );
	res->vCall.fnct = fnct;
	res->vCall.arglist = arglist;

	return res;
}

ASTNode makeMethodCall( EcBool super, ASTNode receiver, ASTNode method, ASTNode arglist )
{
	ASTNode res;

	res = ASTNodeAdd( methodCallNodeType );
	res->vMethodCall.super    = super;
	res->vMethodCall.receiver = receiver;
	res->vMethodCall.method   = method;
	res->vMethodCall.arglist  = arglist;

	return res;
}

ASTNode makeList( EcBool asArg, EcBool retVal, ASTNode prefix, ASTNode stmt )
{
	ASTNode res;
	ASTNode node;

	res = ASTNodeAdd( stmtListType );
	res->vStmtList.asArg   = asArg;
	res->vStmtList.retVal  = retVal;
	res->vStmtList.inlined = FALSE;								/* changed by code */
	res->vStmtList.stmt    = stmt;
	res->vStmtList.next    = NULL;

	/* Get to the last position in prefix list */
	node = prefix;
	while (node)
	{
		if (! node->vStmtList.next) break;

		node = node->vStmtList.next;
	}

	if (node)
	{
		ASSERT( ! node->vStmtList.next );
		node->vStmtList.next = res;
	}

	if (prefix)
		return prefix;

	return res;
}

ASTNode makePair( ASTNode left, ASTNode right )
{
	return makeList( TRUE, FALSE, makeList( TRUE, FALSE, NULL, left ), right );
}

ASTNode astPairLeft( ASTNode astPair )
{
	ASSERT( astPair );

	return astPair->vStmtList.stmt;
}

ASTNode astPairRight( ASTNode astPair )
{
	ASTNode next;

	ASSERT( astPair );
	next = astPair->vStmtList.next;
	ASSERT( next );

	return next->vStmtList.stmt;
}

ASTNode astListHead( ASTNode astList )
{
	return astList;
}

ASTNode astListTail( ASTNode astList )
{
	ASTNode node;

	node = astList;
	while (node)
	{
		if (! node->vStmtList.next) break;

		node = node->vStmtList.next;
	}

	return node;
}

ASTNode makeFunction( ASTNode funcName, ASTNode decl, ASTNode paramList, ASTNode body, const char *docstring )
{
	ASTNode res;

	res = ASTNodeAdd( funcNodeType );
	res->vFunction.funcName  = funcName;
	res->vFunction.decl      = decl;
	res->vFunction.paramList = paramList;
	res->vFunction.body      = body;
	res->vFunction.docstring = ec_stringduppool( PRIVATE(nodePool), docstring );

	res->vFunction.scope = NULL;

	return res;
}

ASTNode makeReturn( ASTNode expr )
{
	ASTNode res;

	res = ASTNodeAdd( returnNodeType );
	res->vReturn.expr = expr;

	return res;
}

ASTNode makeClass( ASTNode decl, ASTNode name, ASTNode base, ASTNode body )
{
	ASTNode res;

	res = ASTNodeAdd( classNodeType );
	res->vClassDef.name = name;
	res->vClassDef.decl = decl;
	res->vClassDef.base = base;
	res->vClassDef.body = body;

	res->vClassDef.scope = NULL;

	return res;
}

ASTNode makeMethod( EcBool classmethod, ASTNode name, ASTNode paramList, ASTNode body )
{
	ASTNode res;

	res = ASTNodeAdd( methodNodeType );
	res->vMethodDef.classmethod = classmethod;
	res->vMethodDef.name        = name;
	res->vMethodDef.paramList   = paramList;
	res->vMethodDef.body        = body;

	res->vMethodDef.scope = NULL;

	return res;
}

ASTNode makePackage( ASTNode name )
{
	ASTNode res;

	res = ASTNodeAdd( packageNodeType );
	res->vPackage.name   = name;

	res->vPackage.scope = NULL;

	return res;
}

#if defined(WITH_STDIO) && EC_AST_DEBUG
static void indent( int lev );

static void printChar( char ch )
{
	if (isprint( (int)ch ))
	{
		/* fputc( ch, stdout ); */
		ec_stderr_printf( "%c", ch );
	} else
	{
		switch (ch)
		{
		case '\n':
			ec_stderr_printf( "\\n" );
			break;
		case '\t':
			ec_stderr_printf( "\\t" );
			break;
		case '\b':
			ec_stderr_printf( "\\b" );
			break;
		case '\r':
			ec_stderr_printf( "\\r" );
			break;
		case '\f':
			ec_stderr_printf( "\\f" );
			break;
		case '\v':
			ec_stderr_printf( "\\v" );
			break;
		case '\a':
			ec_stderr_printf( "\\a" );
			break;
		case '\?':
			ec_stderr_printf( "\\?" );
			break;
		case '\0':
			ec_stderr_printf( "\\0" );
			break;
		default:
			ec_stderr_printf( "\\x%02x", (unsigned int)ch );
		}
	}
}

static void printString( const char *strng )
{
	const char *p = strng;

	while (p && *p)
	{
		printChar( *p );
		p++;
	}
}

void ASTPrint( int lev, ASTNode node )
{
	if (! node)
	{
		indent( lev ); ec_stderr_printf( "NULL" );
		return;
	}

	/* Simplify our lives ... */
	if (currentScope)
	{
		if (currentScope->type == PackageScope)
		{
			currentPackage  = currentScope->target;
			currentBytecode = EC_PACKAGECODE(currentScope->target);
		} else
		{
			currentBytecode = currentScope->target;
		}
		currentLiteral  = EC_COMPILEDLFRAME(currentBytecode);
	}

	switch ( node->type )
	{
	case nullType:
		indent( lev ); ec_stderr_printf( "<nullType>" );
		break;

	case symbolType:
		printSymbol( lev, node );
		break;

	case qualifiedSymbolType:
		printQualifiedSymbol( lev, node );
		break;

	case constExprType:
		printConstant( lev, node );
		break;

	case variableExprType:
		printVariable( lev, node );
		break;

	case arrayConstructionExprType:
		printArrayCons( lev, node );
		break;

	case hashConstructionExprType:
		printHashCons( lev, node );
		break;

	case unaryExprType:
		printUnary( lev, node );
		break;

	case binaryExprType:
		printBinary( lev, node );
		break;

	case conditionalExprType:
		printConditional( lev, node );
		break;

	case orExprType:
		printOr( lev, node );
		break;

	case andExprType:
		printAnd( lev, node );
		break;

	case assignExprType:
		printAssign( lev, node );
		break;

	case simAssignExprType:
		printSimAssign( lev, node );
		break;

	case arrayRefExprType:
		printArrayRef( lev, node );
		break;

	case declNodeType:
		printDecl( lev, node );
		break;

	case declAtomNodeType:
		printDeclAtom( lev, node );
		break;

	case statementType:
		printStatement( lev, node );
		break;

	case labeledStmtType:
		printLabeledStmt( lev, node );
		break;

	case exprStmtType:
		printExprStmt( lev, node );
		break;

	case ifStmtType:
		printIf( lev, node );
		break;

	case whileStmtType:
		printWhile( lev, node );
		break;

	case doStmtType:
		printDo( lev, node );
		break;

	case forStmtType:
		printFor( lev, node );
		break;

	case forInStmtType:
		printForIn( lev, node );
		break;

	case breakStmtType:
		printBreak( lev, node );
		break;

	case continueStmtType:
		printContinue( lev, node );
		break;

	case gotoStmtType:
		printGoto( lev, node );
		break;

	case tryStmtType:
		printTry( lev, node );
		break;

	case catchStmtType:
		printCatch( lev, node );
		break;

	case throwStmtType:
		printThrow( lev, node );
		break;

	case importStmtType:
		printImport( lev, node );
		break;

	case paramNodeType:
		printParam( lev, node );
		break;

	case paramListType:
		printParamList( lev, node );
		break;

	case callNodeType:
		printCall( lev, node );
		break;

	case methodCallNodeType:
		printMethodCall( lev, node );
		break;

	case stmtListType:
		printStmtList( lev, node );
		break;

	case funcNodeType:
		printFunction( lev, node );
		break;

	case returnNodeType:
		printReturn( lev, node );
		break;

	case classNodeType:
		printClass( lev, node );
		break;

	case methodNodeType:
		printMethod( lev, node );
		break;

	case packageNodeType:
		printPackage( lev, node );
		break;
	}
}

static void printSymbol( int lev, ASTNode node )
{
	indent( lev );
	ec_stderr_printf( "<symbol `%s' (%ld)>",
			EcSymbolAt( node->vSymbol.symbolid ),
			(long)node->vSymbol.symbolid );
}

static void printQualifiedSymbol( int lev, ASTNode node )
{
	char *string;

	indent( lev );
	string = EcQualifiedString( QSYM(node) );
	ec_stderr_printf( "<qsymbol `%s'>", string );
	ec_free( string );
}

static void printConstant( int lev, ASTNode node )
{
	indent( lev );
	switch (node->vConstExpr.type)
	{
	case inumberConst:
		ec_stderr_printf( "<const: %ld>", (long)node->vConstExpr.vInt );
		break;
	case fnumberConst:
		ec_stderr_printf( "<const: %g>", node->vConstExpr.vFloat );
		break;
	case charConst:
		if (isprint( (int) node->vConstExpr.vChar ))
			ec_stderr_printf( "<const: '%c'>", node->vConstExpr.vChar );
		else
		{
			ec_stderr_printf( "<const: '" );
			printChar( node->vConstExpr.vChar );
			ec_stderr_printf( "'>" );
		}
		break;
	case symbolConst:
		ec_stderr_printf( "<const: %s>", EcSymbolAt( node->vConstExpr.vSym ) );
		break;
	case stringConst:
		ec_stderr_printf( "<const: \"" );
		printString( node->vConstExpr.vString );
		ec_stderr_printf( "\">" );
		break;
	case objectConst:
		ec_stderr_printf( "<const: EC_OBJ 0x%08lX>", (unsigned long)node->vConstExpr.vObject );
		break;
	}
}

static void printVariable( int lev, ASTNode node )
{
	char *name;

	name = EcQualifiedString( QSYM(node) );

	indent( lev );
	ec_stderr_printf( "<var `%s'>", name );
	ec_free( name );
}

static void printArrayCons( int lev, ASTNode node )
{
	indent( lev );
	ec_stderr_printf( "#[ " );
	ASTPrint( 0, node->vArrayConsExpr.arglist );
	ec_stderr_printf( " ]" );
}

static void printHashCons( int lev, ASTNode node )
{
	indent( lev );
	if (node->vHashConsExpr.oldstyle)
	{
		ec_stderr_printf( "%%[ " );
		ASTPrint( 0, node->vHashConsExpr.arglist );
		ec_stderr_printf( " ]" );
	} else
	{
		ec_stderr_printf( "{ " );
		ASTPrint( 0, node->vHashConsExpr.arglist );
		ec_stderr_printf( " }" );
	}
}

static void printUnary( int lev, ASTNode node )
{
	indent( lev );
	switch (node->vUnaryExpr.op)
	{
	case NEG_OP:
		ec_stderr_printf( "- " );
		break;

	case POS_OP:
		ec_stderr_printf( "+ " );
		break;

	case INVERT_OP:
		ec_stderr_printf( "~ " );
		break;

	case LNOT_OP:
		ec_stderr_printf( "! " );
		break;

	case PREINC_OP:
		ec_stderr_printf( "++" );
		break;

	case PREDEC_OP:
		ec_stderr_printf( "--" );
		break;

	case POSTINC_OP:
	case POSTDEC_OP:
		/* NOTHING */
		break;
	}

	ASTPrint( 0, node->vUnaryExpr.operand );

	switch (node->vUnaryExpr.op)
	{
	case POSTINC_OP:
		ec_stderr_printf( "++" );
		break;

	case POSTDEC_OP:
		ec_stderr_printf( "--" );
		break;

	case NEG_OP:
	case POS_OP:
	case INVERT_OP:
	case LNOT_OP:
	case PREINC_OP:
	case PREDEC_OP:
		/* NOTHING */
		break;
	}
}

static void printBinary( int lev, ASTNode node )
{
	ASTPrint( lev, node->vBinaryExpr.first );

	switch (node->vBinaryExpr.op)
	{
	case ADD_OP:
		ec_stderr_printf( " + " );
		break;

	case SUB_OP:
		ec_stderr_printf( " - " );
		break;

	case MUL_OP:
		ec_stderr_printf( " * " );
		break;

	case DIV_OP:
		ec_stderr_printf( " / " );
		break;

	case MOD_OP:
		ec_stderr_printf( " %% " );
		break;

	case POW_OP:
		ec_stderr_printf( " ** " );
		break;

	case LSHIFT_OP:
		ec_stderr_printf( " << " );
		break;

	case RSHIFT_OP:
		ec_stderr_printf( " >> " );
		break;

	case AND_OP:
		ec_stderr_printf( " & " );
		break;

	case OR_OP:
		ec_stderr_printf( " | " );
		break;

	case XOR_OP:
		ec_stderr_printf( " ^ " );
		break;

	case IN_OP:
		ec_stderr_printf( " in " );
		break;

	case LT_OP:
		ec_stderr_printf( " < " );
		break;

	case GT_OP:
		ec_stderr_printf( " > " );
		break;

	case LE_OP:
		ec_stderr_printf( " <= " );
		break;

	case GE_OP:
		ec_stderr_printf( " >= " );
		break;

	case EQ_OP:
		ec_stderr_printf( " == " );
		break;

	case NE_OP:
		ec_stderr_printf( " != " );
		break;

	default:
		ASSERT( FALSE );
		break;
	}

	ASTPrint( 0, node->vBinaryExpr.second );
}

static void printConditional( int lev, ASTNode node )
{
	ASTPrint( lev, node->vCondExpr.cond );
	ec_stderr_printf( " ? " );
	ASTPrint( 0, node->vCondExpr.texpr );
	ec_stderr_printf( " : " );
	ASTPrint( 0, node->vCondExpr.fexpr );
}

static void printOr( int lev, ASTNode node )
{
	ASTPrint( lev, node->vOrExpr.first );
	ec_stderr_printf( " || " );
	ASTPrint( 0, node->vOrExpr.second );
}

static void printAnd( int lev, ASTNode node )
{
	ASTPrint( lev, node->vAndExpr.first );
	ec_stderr_printf( " && " );
	ASTPrint( 0, node->vAndExpr.second );
}

static void printAssign( int lev, ASTNode node )
{
	ASTPrint( lev, node->vAssignExpr.lhs );
	ec_stderr_printf( " = " );
	ASTPrint( 0, node->vAssignExpr.rhs );
}

static void printSimAssign( int lev, ASTNode node )
{
	ASTNode lhs_list, rhs_list;
	ASTNode lhs, rhs;
	EcBool  first;

	lhs_list = node->vAssignExpr.lhs;
	rhs_list = node->vAssignExpr.rhs;

	indent( lev );

	first = TRUE;
	while (lhs_list)
	{
		if (! first) ec_stderr_printf( ", " );
		first = FALSE;
		lhs = lhs_list->vStmtList.stmt;

		ASTPrint( 0, lhs );

		lhs_list = lhs_list->vStmtList.next;
	}

	ec_stderr_printf( " = " );

	first = TRUE;
	while (rhs_list)
	{
		if (! first) ec_stderr_printf( ", " );
		first = FALSE;
		rhs = rhs_list->vStmtList.stmt;

		ASTPrint( 0, rhs );

		rhs_list = rhs_list->vStmtList.next;
	}
}

static void printArrayRef( int lev, ASTNode node )
{
	ASTPrint( lev, node->vArrayRefExpr.ref );
	ec_stderr_printf( "[" );
	ASTPrint( 0, node->vArrayRefExpr.idx );
	ec_stderr_printf( "]" );
}

static void printDecl( int lev, ASTNode node )
{
	Scope targetScope;
	ASTNode decllist;

	indent( lev );
	ec_stderr_printf( "<" );
	switch (node->vDecl.symclass)
	{
	case SymbolParam:
		ec_stderr_printf( "param " );
		break;
	case SymbolLocal:
		ec_stderr_printf( "local " );
		break;
	case SymbolStatic:
		ec_stderr_printf( "static " );
		break;
	case SymbolPrivate:
		ec_stderr_printf( "private " );
		break;
	case SymbolPublic:
		ec_stderr_printf( "public " );
		break;
	}
	targetScope = currentScope;

	decllist = node->vDecl.decllist;
	while (decllist)
	{
		ASTPrint( 0, decllist->vStmtList.stmt );
		decllist = decllist->vStmtList.next;
		if (decllist) ec_stderr_printf( ", " );
	}
	ec_stderr_printf( ">" );
}

static void printDeclAtom( int lev, ASTNode node )
{
	indent( lev );
	ec_stderr_printf( "`%s' id:%ld",
			EcSymbolAt( SYMID(node->vDeclAtom.sym) ), (long)SYMID(node->vDeclAtom.sym) );
	if (node->vDeclAtom.init) {
		ec_stderr_printf( " = " );
		ASTPrint( 0, node->vDeclAtom.init );
	}
}

static void printStatement( int lev, ASTNode node )
{
	ASTPrint( lev, node->vStmt.stmt );
}

static void printLabeledStmt( int lev, ASTNode node )
{
	indent( lev );
	ec_stderr_printf( "%s:\n", EcSymbolAt( node->vLabeledStmt.label ) );
	ASTPrint( lev, node->vLabeledStmt.stmt );
}

static void printExprStmt( int lev, ASTNode node )
{
	ASTPrint( lev, node->vExpr.expr );
}

static void printIf( int lev, ASTNode node )
{
	indent( lev );
	ec_stderr_printf( "if " );
	ASTPrint( 0, node->vIfStmt.cond );
	ec_stderr_printf( "\n" );
	ASTPrint( lev+1, node->vIfStmt.thenClause );
	ec_stderr_printf( "\n" );
	if (node->vIfStmt.elseClause)
	{
		ASTPrint( lev+1, node->vIfStmt.elseClause );
		ec_stderr_printf( "\n" );
	}
}

static void printWhile( int lev, ASTNode node )
{
	indent( lev );
	ec_stderr_printf( "while " );
	ASTPrint( 0, node->vWhileStmt.cond );
	ec_stderr_printf( "\n" );
	ASTPrint( lev+1, node->vWhileStmt.body );
}

static void printDo( int lev, ASTNode node )
{
	indent( lev );
	ec_stderr_printf( "do\n" );
	ASTPrint( lev+1, node->vDoStmt.body );
	indent( lev );
	ec_stderr_printf( "while " );
	ASTPrint( 0, node->vDoStmt.cond );
}

static void printFor( int lev, ASTNode node )
{
	indent( lev );
	ec_stderr_printf( "for (" );
	ASTPrint( 0, node->vForStmt.init );
	ec_stderr_printf( ", " );
	ASTPrint( 0, node->vForStmt.cond );
	ec_stderr_printf( ", " );
	ASTPrint( 0, node->vForStmt.restart );
	ec_stderr_printf( ")\n" );
	ASTPrint( lev+1, node->vForStmt.body );
}

static void printForIn( int lev, ASTNode node )
{
	indent( lev );
	ec_stderr_printf( "for (" );
	ASTPrint( 0, node->vForInStmt.var );
	ec_stderr_printf( " in " );
	ASTPrint( 0, node->vForInStmt.sequence );
	ec_stderr_printf( ")\n" );
	ASTPrint( lev+1, node->vForInStmt.body );
}

static void printBreak( int lev, ASTNode node )
{
	indent( lev );
	if (node->vBreakStmt.lev)
	{
		ec_stderr_printf( "break " );
		ASTPrint( 0, node->vBreakStmt.lev );
	} else
	{
		ec_stderr_printf( "break" );
	}
	ec_stderr_printf( "\n" );
}

static void printContinue( int lev, ASTNode node )
{
	indent( lev );
	if (node->vContinueStmt.lev)
	{
		ec_stderr_printf( "continue " );
		ASTPrint( 0, node->vContinueStmt.lev );
	} else
	{
		ec_stderr_printf( "continue" );
	}
	ec_stderr_printf( "\n" );
}

static void printGoto( int lev, ASTNode node )
{
	indent( lev );
	ec_stderr_printf( "goto " );
	ASTPrint( 0, node->vGotoStmt.target );
	ec_stderr_printf( "\n" );
}

static void printTry( int lev, ASTNode node )
{
	indent( lev );
	ec_stderr_printf( "try\n" );
	ASTPrint( lev, node->vTryStmt.block );
	ec_stderr_printf( "\n" );
	ASTPrint( lev, node->vTryStmt.catchlist );
}

static void printCatch( int lev, ASTNode node )
{
	indent( lev );
	ec_stderr_printf( "catch " );
	ASTPrint( 0, node->vCatchStmt.type );
	if (node->vCatchStmt.var)
		ASTPrint( 0, node->vCatchStmt.var );
	ec_stderr_printf( "\n" );
	ASTPrint( lev, node->vCatchStmt.handler );
}

static void printThrow( int lev, ASTNode node )
{
	indent( lev );
	ec_stderr_printf( "throw " );
	ASTPrint( 0, node->vThrowStmt.expr );
	ec_stderr_printf( "\n" );
}

static void printImport( int lev, ASTNode node )
{
	indent( lev );
	if (node->vImportStmt.detailed)
	{
		ec_stderr_printf( "from " );
		ASTPrint( 0, node->vImportStmt.package );
		ec_stderr_printf( " import " );
		if (! node->vImportStmt.symlist)
			ec_stderr_printf( "*" );
		else
			ASTPrint( 0, node->vImportStmt.symlist );
	} else
	{
		ec_stderr_printf( "import " );
		ASTPrint( 0, node->vImportStmt.package );
	}
	ec_stderr_printf( "\n" );
}

static void printParam( int lev, ASTNode node )
{
	ASTNode var, init;

	var  = node->vParam.var;
	init = node->vParam.init;

	ASTPrint( 0, var );
	if (init)
	{
		ec_stderr_printf( " = " );
		ASTPrint( 0, init );
	}
}

static void printParamList( int lev, ASTNode node )
{
	EcBool  varargs;
	ASTNode list;
	EcBool  first = TRUE;

	varargs = node->vParamList.varargs;
	list    = node->vParamList.list;

	indent( lev );
	ec_stderr_printf( "(" );
	while (list)
	{
		if (! first) ec_stderr_printf( ", " );
		first = FALSE;
		ASTPrint( 0, list->vStmtList.stmt );

		list = list->vStmtList.next;
		if (varargs && !list) ec_stderr_printf( "..." );
	}
	ec_stderr_printf( ")" );
}

static void printCall( int lev, ASTNode node )
{
	indent( lev );
	ASTPrint( 0, node->vCall.fnct );
	ASTPrint( 0, node->vCall.arglist );
}

static void printMethodCall( int lev, ASTNode node )
{
	indent( lev );
	ec_stderr_printf( "[" );
	if (! node->vMethodCall.super)
		ASTPrint( 0, node->vMethodCall.receiver );
	else
		ec_stderr_printf( "super" );
	ec_stderr_printf( " " );
	ASTPrint( 0, node->vMethodCall.method );
	ASTPrint( 0, node->vMethodCall.arglist );
	ec_stderr_printf( "]" );
}

static void printStmtList( int lev, ASTNode node )
{
	if (! node->vStmtList.asArg)
	{
		indent( lev );
		ec_stderr_printf( "{" );
		while (node)
		{
			ec_stderr_printf( "\n" );
			ASTPrint( lev+1, node->vStmtList.stmt );

			node = node->vStmtList.next;
		}
		ec_stderr_printf( "\n" );
		indent( lev );
		ec_stderr_printf( "}" );
	} else
	{
		EcBool first = TRUE;

		indent( lev );
		ec_stderr_printf( "(" );
		while (node)
		{
			if (! first) ec_stderr_printf( ", " );
			first = FALSE;
			ASTPrint( 0, node->vStmtList.stmt );

			node = node->vStmtList.next;
		}
		ec_stderr_printf( ")" );
	}
}

static void printFunction( int lev, ASTNode node )
{
	indent( lev );
	ec_stderr_printf( "function " );
	ASTPrint( 0, node->vFunction.funcName );	
	if (node->vFunction.decl)
	{
		ec_stderr_printf( " " );
		ASTPrint( 0, node->vFunction.decl );
	}
	currentScope = node->vFunction.scope;
	ec_stderr_printf( " " );
	ASTPrint( 0, node->vFunction.paramList );
	ec_stderr_printf( "\n" );
	if (node->vFunction.docstring)
	{
		indent( lev );
		ec_stderr_printf( "\"%s\"\n", node->vFunction.docstring );
	}
	ASTPrint( lev, node->vFunction.body );
	if (currentScope)
		currentScope = currentScope->upper;
}

static void printReturn( int lev, ASTNode node )
{
	indent( lev );
	ec_stderr_printf( "return " );
	ASTPrint( 0, node->vReturn.expr );
	ec_stderr_printf( "\n" );
}

static void printClass( int lev, ASTNode node )
{
	indent( lev );
	ec_stderr_printf( "class " );
	ASTPrint( 0, node->vClassDef.name );	
	if (node->vClassDef.decl)
	{
		ec_stderr_printf( " " );
		ASTPrint( 0, node->vClassDef.decl );
	}
	ec_stderr_printf( " extends " );
	ASTPrint( 0, node->vClassDef.base );	
	currentScope = node->vClassDef.scope;
	ec_stderr_printf( "\n" );
	ASTPrint( lev, node->vClassDef.body );
	ec_stderr_printf( "\n" );
	if (currentScope)
		currentScope = currentScope->upper;
}

static void printMethod( int lev, ASTNode node )
{
	indent( lev );
	if (node->vMethodDef.classmethod)
		ec_stderr_printf( "class method " );
	else
		ec_stderr_printf( "method " );
	ASTPrint( 0, node->vMethodDef.name );	
	currentScope = node->vMethodDef.scope;
	ec_stderr_printf( " " );
	ASTPrint( 0, node->vMethodDef.paramList );
	ec_stderr_printf( "\n" );
	ASTPrint( lev, node->vMethodDef.body );
	if (currentScope)
		currentScope = currentScope->upper;
}

static void printPackage( int lev, ASTNode node )
{
	indent( lev );
	ec_stderr_printf( "Package: " );
	if (node->vPackage.name)
		ASTPrint( 0, node->vPackage.name );
	else
		ec_stderr_printf( DEFAULT_PACKAGENAME );
	ec_stderr_printf( "\n" );
#if 0
	currentScope = node->vPackage.scope;
	ASTPrint( lev, node->vPackage.body );
	if (currentScope)
		currentScope = currentScope->upper;
#endif
	currentScope = node->vPackage.scope;
}

static void indent( int lev )
{
	int i;

	if (lev < 0) lev = 0;
	for(i = 0; i < lev; i++)
		ec_stderr_printf( "   " );
}
#endif /* end of defined(WITH_STDIO) && EC_AST_DEBUG */
