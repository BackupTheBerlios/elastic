/* ==========================================================================
 * ast.h
 * ==========================================================================
 * Abstract Syntax Tree.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat May 23 20:34:10 MEST 1998
 *
 *   $Id: ast.h,v 1.4 2005/03/22 10:09:33 panta Exp $
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

#ifndef __AST_H
#define __AST_H

#include <elastic/elastic.h>
#include <elastic/compile.h>

EC_BEGIN_DECLS

typedef struct
{
	EcInt first_line, first_column;
	EcInt last_line,  last_column;
} ASTLocation;

typedef enum
{
	nullType,
	symbolType,
	qualifiedSymbolType,
	constExprType,
	variableExprType,
	arrayConstructionExprType,
	hashConstructionExprType,
	unaryExprType,
	binaryExprType,
	conditionalExprType,
	orExprType,
	andExprType,
	assignExprType,
	simAssignExprType,
	arrayRefExprType,
	declNodeType,
	declAtomNodeType,
	paramNodeType,
	statementType,
	labeledStmtType,
	exprStmtType,
	ifStmtType,
	whileStmtType,
	doStmtType,
	forStmtType,
	forInStmtType,
	breakStmtType,
	continueStmtType,
	gotoStmtType,
	tryStmtType,
	catchStmtType,
	throwStmtType,
	importStmtType,
	paramListType,
	callNodeType,
	methodCallNodeType,
	stmtListType,
	funcNodeType,
	returnNodeType,
	classNodeType,
	methodNodeType,
	packageNodeType
} ASTNodeType;

typedef struct ASTNodeStruct *ASTNode;

typedef enum ConstType
{ inumberConst, fnumberConst, charConst, symbolConst, stringConst, objectConst } ConstType;

typedef enum UnaryOp
{ NEG_OP, POS_OP, INVERT_OP, LNOT_OP, PREINC_OP , PREDEC_OP, POSTINC_OP, POSTDEC_OP } UnaryOp;

typedef enum BinaryOp
{ ADD_OP, SUB_OP, MUL_OP, DIV_OP, MOD_OP, POW_OP, LSHIFT_OP, RSHIFT_OP,
  AND_OP, OR_OP, XOR_OP, IN_OP, LT_OP, GT_OP, LE_OP, GE_OP, EQ_OP, NE_OP } BinaryOp;

typedef struct
{
	EcUInt symbolid;
} SymbolNode;

typedef struct
{
	EcQualifiedSymbol qsym;
} QualifiedSymbolNode;

typedef struct
{
	ConstType type;
	union
	{
		EcInt       inumber;
		EcFloat     fnumber;
		EcChar      ch;
		EcUInt      symid;
		const char *string;
		EC_OBJ      obj;										/* Constant object, such as EC_NIL, #t, ... */
	} v;
} ConstNode;

#define vInt    v.inumber
#define vFloat  v.fnumber
#define vChar   v.ch
#define vSym    v.symid
#define vString v.string
#define vObject v.obj

typedef struct
{
	ASTNode arglist;
} ArrayConstruction;

typedef struct
{
	ASTNode arglist;
	EcBool  oldstyle;
} HashConstruction;

typedef struct
{
	UnaryOp op;
	ASTNode operand;
} UnaryExpr;

typedef struct
{
	BinaryOp op;
	ASTNode  first, second;
} BinaryExpr;

typedef struct
{
	ASTNode cond;
	ASTNode texpr;
	ASTNode fexpr;
} ConditionalExpr;

typedef struct
{
	ASTNode first, second;
} OrExpr;

typedef struct
{
	ASTNode first, second;
} AndExpr;

typedef struct
{
	ASTNode lhs;										/* Left Hand Side  */
	ASTNode rhs;										/* Right Hand Side */
} AssignExpr;

typedef struct
{
	ASTNode lhs;										/* Left Hand Side (list)  */
	ASTNode rhs;										/* Right Hand Side (list) */
} SimAssignExpr;

typedef struct
{
	ASTNode ref;										/* Referenced expression */
	ASTNode idx;										/* Index                 */
} ArrayRefExpr;

typedef struct
{
	SymbolClass symclass;
	ASTNode     decllist;
} DeclNode;

typedef struct
{
	ASTNode     sym;
	ASTNode     init;
} DeclAtomNode;

/*
 * We need to be able to distinguish statements.
 * For example to emit debug information, and also to distinguish
 * expressions from expression statements.
 */
typedef struct
{
	ASTNode stmt;
} Statement;

typedef struct
{
	EcUInt  label;
	ASTNode stmt;
} LabeledStatement;

typedef struct
{
	ASTNode expr;
} ExprStmt;

typedef struct
{
	ASTNode cond;
	ASTNode thenClause, elseClause;
} IfStmt;

typedef struct
{
	ASTNode cond;
	ASTNode body;
} WhileStmt;

typedef struct
{
	ASTNode cond;
	ASTNode body;
} DoStmt;

typedef struct
{
	ASTNode init, cond, restart;
	ASTNode body;
} ForStmt;

typedef struct
{
	ASTNode var, sequence;
	ASTNode body;
} ForInStmt;

typedef struct
{
	ASTNode lev;
} BreakStmt;

typedef struct
{
	ASTNode lev;
} ContinueStmt;

typedef struct
{
	ASTNode target;
} GotoStmt;

typedef struct
{
	ASTNode block;
	ASTNode catchlist;
} TryStmt;

typedef struct
{
	ASTNode type;
	ASTNode var;
	ASTNode handler;
} CatchStmt;

typedef struct
{
	ASTNode expr;
} ThrowStmt;

typedef struct
{
	EcBool  detailed;											/* if TRUE it is a detailed import of symbols */
	ASTNode package;
	ASTNode symlist;											/* NULL: if detailed then an 'import *'. */
																/* If !detailed, this has to be NULL.    */
} ImportStmt;

typedef struct
{
	ASTNode var;
	ASTNode init;
} ParamNode;

typedef struct
{
	EcBool  varargs;
	ASTNode list;
} ParamListStmt;												/* :TODO: change this name */

typedef struct
{
	ASTNode fnct;
	ASTNode arglist;
} CallNode;

typedef struct
{
	EcBool  super;												/* call to super (receiver NULL) */
	ASTNode receiver;
	ASTNode method;
	ASTNode arglist;
} MethodCallNode;

typedef struct
{
	EcBool    asArg;												/* If TRUE, push expression       */
	EcBool    retVal;												/* If TRUE, return a value        */
	EcBool    inlined;											/* If TRUE, an inlined code block */
	ASTNode stmt;
	ASTNode next;
} StmtList;

typedef struct
{
	ASTNode funcName;
	ASTNode decl;												/* eventual declaration */
	ASTNode paramList;
	ASTNode body;

	const char *docstring;

	/* Decorations */
	Scope   scope;
} FunctionNode;

typedef struct
{
	ASTNode expr;
} ReturnNode;

typedef struct
{
	ASTNode name;
	ASTNode decl;
	ASTNode base;
	ASTNode body;

	/* Decorations */
	Scope   scope;
} ClassNode;

typedef struct
{
	EcBool  classmethod;
	ASTNode name;
	ASTNode paramList;
	ASTNode body;

	/* Decorations */
	Scope   scope;
} MethodNode;

typedef struct
{
	ASTNode name;

	/* Decorations */
	Scope   scope;
} PackageNode;

struct ASTNodeStruct
{
	ASTNodeType type;

	ASTLocation tloc;

	/* Associated bytecode to this node */
	EC_OBJ bytecode;

	union
	{
		/* Symbol */
		SymbolNode			symbol;

		/* Qualified Symbol */
		QualifiedSymbolNode	qualifiedSymbol;

		/* Constant expression */
		ConstNode			constExpr;

		/* Variable expression */
		QualifiedSymbolNode	variableExpr;

		/* Array construction expression */
		ArrayConstruction   arrayConsExpr;

		/* Hash construction expression */
		HashConstruction    hashConsExpr;

		/* Unary expression */
		UnaryExpr			unaryExpr;

		/* Binary expression */
		BinaryExpr			binaryExpr;

		/* Conditional expression */
		ConditionalExpr		condExpr;

		/* Logical expressions */
		OrExpr				orExpr;
		AndExpr				andExpr;

		/* Assignment */
		AssignExpr			assignExpr;

		/* (multiple) Simultaneous Assignment */
		SimAssignExpr		simAssignExpr;

		/* Array reference */
		ArrayRefExpr		arrayRefExpr;

		/* Statement */
		Statement			stmt;

		/* Labeled Statement */
		LabeledStatement	labeledStmt;

		/* Expression statement */
		ExprStmt			exprStmt;

		/* Declaration */
		DeclNode			declNode;

		/* Declaration Atom */
		DeclAtomNode		declAtomNode;

		/* Control structures */
		IfStmt				ifStmt;
		WhileStmt			whileStmt;
		DoStmt				doStmt;
		ForStmt				forStmt;
		ForInStmt			forInStmt;
		BreakStmt			breakStmt;
		ContinueStmt		continueStmt;
		GotoStmt			gotoStmt;
		TryStmt				tryStmt;
		CatchStmt			catchStmt;
		ThrowStmt			throwStmt;

		/* Import */
		ImportStmt			importStmt;

		/* Parameter */
		ParamNode			paramNode;

		/* Parameter list */
		ParamListStmt		paramList;

		/* Call */
		CallNode			callNode;

		/* Method Call */
		MethodCallNode		methodCallNode;

		/* Sequencing statements, expressions, declarations */
		StmtList			stmtList;

		/* Function definition */
		FunctionNode		funcNode;

		/* Returning */
		ReturnNode			retNode;

		/* Classes */
		ClassNode			classNode;

		/* Methods */
		MethodNode			methodNode;

		/* Package */
		PackageNode			package;
	} v;
};

#define vSymbol          v.symbol
#define vQualifiedSymbol v.qualifiedSymbol
#define vConstExpr       v.constExpr
#define vVariableExpr    v.variableExpr
#define vArrayConsExpr   v.arrayConsExpr
#define vHashConsExpr    v.hashConsExpr
#define vUnaryExpr       v.unaryExpr
#define vBinaryExpr      v.binaryExpr
#define vCondExpr        v.condExpr
#define vOrExpr          v.orExpr
#define vAndExpr         v.andExpr
#define vAssignExpr      v.assignExpr
#define vSimAssignExpr   v.simAssignExpr
#define vArrayRefExpr    v.arrayRefExpr
#define vDecl            v.declNode
#define vDeclAtom        v.declAtomNode
#define vStmt            v.stmt
#define vLabeledStmt     v.labeledStmt
#define vExpr            v.exprStmt
#define vIfStmt          v.ifStmt
#define vWhileStmt       v.whileStmt
#define vDoStmt          v.doStmt
#define vForStmt         v.forStmt
#define vForInStmt       v.forInStmt
#define vBreakStmt       v.breakStmt
#define vContinueStmt    v.continueStmt
#define vGotoStmt        v.gotoStmt
#define vTryStmt         v.tryStmt
#define vCatchStmt       v.catchStmt
#define vThrowStmt       v.throwStmt
#define vImportStmt		 v.importStmt
#define vParam       	 v.paramNode
#define vParamList       v.paramList
#define vCall            v.callNode
#define vMethodCall      v.methodCallNode
#define vStmtList        v.stmtList
#define vFunction		 v.funcNode
#define vReturn          v.retNode
#define vClassDef        v.classNode
#define vMethodDef       v.methodNode
#define vPackage         v.package


/* ========================================================================
 * A P I
 * ======================================================================== */

#define SYMID(symnode)	((symnode)->vSymbol.symbolid)
#define QSYM(qsymnode)	(&((qsymnode)->vQualifiedSymbol.qsym))

#define FLINE(node)		((node)->tloc.first_line)
#define FCOL(node)		((node)->tloc.first_column)
#define LLINE(node)		((node)->tloc.last_line)
#define LCOL(node)		((node)->tloc.last_column)

ASTNode ASTNodeAdd( ASTNodeType type );
void    ASTFree( void );

ASTNode makeConstInt( EcInt number ), makeConstFloat( EcFloat number ),
		makeConstChar( EcChar ch ), makeConstSym( EcUInt symid ),
		makeConstString( const char *string ), makeConstObject( EC_OBJ obj ),
	    makeSymbol( EcUInt symid ), makeQualifiedSymbol( ASTNode prefix, ASTNode symbol ),
		makeVariable( ASTNode node ),
		makeArrayCons( ASTNode arglist ),
		makeHashCons( ASTNode arglist, EcBool oldstyle ),
	    makeUnary( UnaryOp op, ASTNode operand ),
	    makeBinary( BinaryOp op, ASTNode first, ASTNode second ),
		makeConditional( ASTNode cond, ASTNode texpr, ASTNode fexpr ),
	    makeOr( ASTNode first, ASTNode second ), makeAnd( ASTNode first, ASTNode second ),
	    makeAssign( ASTNode lhs, ASTNode rhs ),
		makeSimultaneousAssign( ASTNode lhs, ASTNode rhs ),
		makeArrayRef( ASTNode ref, ASTNode idx ),
	    makeDecl( SymbolClass symclass, ASTNode symlist ),
		makeDeclAtom( ASTNode sym, ASTNode init ),
	    makeStmt( ASTNode stmt ),
	    makeLabeledStmt( EcUInt label, ASTNode stmt ),
	    makeExprStmt( ASTNode stmt ),
	    makeIf( ASTNode cond, ASTNode thenClause, ASTNode elseClause ),
	    makeWhile( ASTNode cond, ASTNode body ),
	    makeDo( ASTNode cond, ASTNode body ),
	    makeFor( ASTNode init, ASTNode cond, ASTNode restart, ASTNode body ),
	    makeForIn( ASTNode var, ASTNode sequence, ASTNode body ),
		makeBreak( ASTNode lev ),
		makeContinue( ASTNode lev ),
		makeGoto( ASTNode target ),
		makeTry( ASTNode block, ASTNode catchlist ),
		makeCatch( ASTNode type, ASTNode var, ASTNode handler ),
		makeThrow( ASTNode expr ),
		makeImport( EcBool detailed, ASTNode package, ASTNode symlist ),
		makeParam( ASTNode var, ASTNode init ),
		makeParamList( EcBool varargs, ASTNode list ),
	    makeCall( ASTNode fnct, ASTNode arglist ),
	    makeMethodCall( EcBool super, ASTNode receiver, ASTNode method, ASTNode arglist ),
	    makeList( EcBool asArg, EcBool ret, ASTNode prefix, ASTNode stmt ),
		makeFunction( ASTNode funcName, ASTNode decl, ASTNode paramList, ASTNode body, const char *docstring ),
	    makeReturn( ASTNode expr ),
		makeClass( ASTNode decl, ASTNode name, ASTNode base, ASTNode body ),
		makeMethod( EcBool classmethod, ASTNode name, ASTNode paramList, ASTNode body ),
		makePackage( ASTNode name );

ASTNode makePair( ASTNode left, ASTNode right );
ASTNode astPairLeft( ASTNode astPair );
ASTNode astPairRight( ASTNode astPair );

ASTNode astListHead( ASTNode astList );
ASTNode astListTail( ASTNode astList );

#if defined(WITH_STDIO) && EC_AST_DEBUG
void    ASTPrint( int lev, ASTNode node );
#endif

EC_END_DECLS

#endif /* __AST_H */
