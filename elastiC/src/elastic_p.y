%{
/* ==========================================================================
 * elastic.y
 * ==========================================================================
 * Parser for elastiC language.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: 1997
 *
 *   $Id: elastic_p.y,v 1.2 2002/05/25 19:37:44 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1997-2001 Marco Pantaleoni. All rights reserved.
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "elastic.h"
#include "ast.h"
#include "private.h"
#include "compat.h"


int yyparse( void );
int yylex( void );
int yyerror( char *s );

static void saveLoc( ASTLocation *dst, void *src );
static EcUInt symconcat( EcUInt sym1, EcUInt sym2 );
static ASTNode listconcat( ASTNode l1, ASTNode l2 );

#define SAVE(dst,loc)		do { saveLoc( &(dst->tloc), &(loc) ); } while (0)

#define MAKEATOMLIST(el)	(makeList( FALSE, FALSE, NULL, (el) ))
#define MAKEDECLATOM(sym,init)	(makeDeclAtom( (sym), (init) ))
#define DECLSYM(t, sym)		(makeDecl( (t), MAKEATOMLIST(MAKEDECLATOM((sym), NULL)) ))
#define DECLVAR(t, var)		(DECLSYM( (t), makeSymbol( QSCOMP(QSYM(var), QSLEN(QSYM(var)) - 1) )))

#define YYERROR_VERBOSE 1
%}

%union {
	EcInt		icnst;          /* For returning numerical constants	     */
	EcFloat		fcnst;          /* For returning numerical constants	     */
	EcChar          ccnst;          /* For returning char constants	             */
	char           *strcnst;        /* For returning string constants or symbols */
	EC_OBJ          objcnst;        /* For returning object constants	     */
	ASTNode         ast;            /* Abstract Syntax Tree                      */
	EcUInt          sym;            /* Symbol                                    */
	struct {
		EcUInt  sym;
		ASTNode list;
	}               kwl;		/* Keyword Argument / Parameter List         */
}

/* Simple terminal tokens */
%token '!'
%token '(' ')' '<' '>'
%token T_ARRAYCONS "#["
%token T_HASHCONS  "%["
%token T_LE "<="
%token T_GE ">="
%token T_POW "**"
%token T_LSHIFT "<<"
%token T_RSHIFT ">>"
%token T_EQ T_NE T_LAND T_LOR
%token T_POWASSIGN T_MULASSIGN T_DIVASSIGN T_MODASSIGN T_ADDASSIGN T_SUBASSIGN
%token T_LSHIFTASSIGN T_RSHIFTASSIGN T_ANDASSIGN T_XORASSIGN T_ORASSIGN
%token T_INC T_DEC
%token T_IF T_ELSE
%token T_WHILE T_DO
%token T_FOR
%token T_IN
%token T_BREAK T_CONTINUE T_GOTO
%token T_TRY T_CATCH T_THROW
%token T_FUNCTION
%token T_RETURN
%token T_CLASS T_EXTENDS T_METHOD T_SUPER
%token T_LOCAL T_STATIC T_PRIVATE T_PUBLIC
%token T_PACKAGE T_IMPORT T_FROM
%token T_QUALIFIER
%token T_ELLIPSIS

/* Complex terminal tokens */
%token <icnst>   T_INUMBER              /* Integer number			     */
%token <fcnst>   T_FNUMBER              /* Double precision number		     */
%token <ccnst>   T_CHAR                 /* Char					     */
%token <strcnst> T_SYMBOL T_STRING      /* Symbol or string			     */
%token <objcnst> T_OBJECT               /* Object				     */
%token <sym>     T_KEYWORD              /* Keyword			             */
%token <sym>     T_CONSTSYM             /* Constant symbol		             */

/* Non terminal symbols */
%type <ast>	primary.expression postfix.expression lvalue.expression
%type <ast>	argument.list method.call array.construction hash.construction
%type <ast>	unary.expression exponentiation.expression multiplicative.expression additive.expression
%type <ast>	shift.expression relational.expression
%type <ast>	equality.expression and.expression exclusive.or.expression inclusive.or.expression
%type <ast>	in.expression logical.and.expression logical.or.expression
%type <ast>	conditional.expression assignment.expression lvalue.list sim.assignment.expression expression
%type <ast>	statement labeled.statement compound.statement statement.list
%type <ast>	expression.statement selection.statement iteration.statement jump.statement
%type <ast>	try.statement catch.list catch.statement import.statement
%type <ast>	parameter basic.parameter.list parameter.list function.definition
%type <ast>	declaration decl.atom decl.list initialization variable
%type <ast>	class.definition class.body class.statement.list class.statement method.definition
%type <ast>	package.declaration package.statement package.statement.list translation.unit
%type <ast>	symbol symbol.list qualified.symbol
%type <kwl>     keyword.argument.list keyword.parameter.list

%expect 4
%start translation.unit


/* GRAMMAR */

%%

/* Expressions and statements */

primary.expression
	: T_INUMBER                                                { $$ = makeConstInt( $1 );   SAVE($$, @1);                 }
	| T_FNUMBER                                                { $$ = makeConstFloat( $1 ); SAVE($$, @1);                 }
	| T_CHAR                                                   { $$ = makeConstChar( $1 );  SAVE($$, @1);                 }
	| T_CONSTSYM                                               { $$ = makeConstSym( $1 );   SAVE($$, @1);                 }
	| T_STRING                                                 { $$ = makeConstString( $1 ); ec_free( $1 ); SAVE($$, @1); }
	| T_OBJECT                                                 { $$ = makeConstObject( $1 ); SAVE($$, @1);                }
	| '(' expression ')'                                       { $$ = $2;                                                 }
;


postfix.expression
	: primary.expression                                       { $$ = $1;                                           }
	| lvalue.expression                                        { $$ = $1;                                           }
	| T_FUNCTION '(' parameter.list ')' compound.statement     { $$ = makeFunction( NULL, NULL, $3, $5 );
								     if ($5) $5->vStmtList.retVal = TRUE; SAVE($$, @1); }
	| postfix.expression '(' ')'                               { $$ = makeCall( $1, NULL ); SAVE($$, @1);           }
	| postfix.expression '(' argument.list ')'                 { $$ = makeCall( $1,   $3 ); SAVE($$, @1);           }
	| method.call                                              { $$ = $1;                                           }
	| array.construction                                       { $$ = $1;                                           }
	| hash.construction                                        { $$ = $1;                                           }
;

lvalue.expression
	: variable                                                 { $$ = $1;                                   }
	| postfix.expression '[' expression ']'                    { $$ = makeArrayRef( $1, $3 ); SAVE($$, @1); }
;

method.call
	: '[' postfix.expression symbol ']'                        { $$ = makeMethodCall( FALSE, $2, $3, NULL ); SAVE($$, @1);    }
	| '[' postfix.expression symbol argument.list ']'          { $$ = makeMethodCall( FALSE, $2, $3, $4 );   SAVE($$, @1);    }
	| '[' postfix.expression keyword.argument.list ']'         { $$ = makeMethodCall( FALSE, $2,
                                                                                          makeSymbol( $3.sym ), $3.list ); SAVE($$, @1);  }
	| '[' T_SUPER symbol ']'                        { $$ = makeMethodCall( TRUE, NULL, $3, NULL ); SAVE($$, @1);    }
	| '[' T_SUPER symbol argument.list ']'          { $$ = makeMethodCall( TRUE, NULL, $3, $4 );   SAVE($$, @1);    }
	| '[' T_SUPER keyword.argument.list ']'         { $$ = makeMethodCall( TRUE, NULL,
                                                                               makeSymbol( $3.sym ), $3.list ); SAVE($$, @1);  }
;

array.construction
	: T_ARRAYCONS ']'                                          { $$ = makeArrayCons( NULL ); SAVE($$, @1); }
	| T_ARRAYCONS argument.list ']'                            { $$ = makeArrayCons( $2 );   SAVE($$, @1); }
	| T_ARRAYCONS argument.list ',' ']'                        { $$ = makeArrayCons( $2 );   SAVE($$, @1); }
;

hash.construction
	: T_HASHCONS ']'                                          { $$ = makeHashCons( NULL ); SAVE($$, @1); }
	| T_HASHCONS argument.list ']'                            { $$ = makeHashCons( $2 );   SAVE($$, @1); }
	| T_HASHCONS argument.list ',' ']'                        { $$ = makeHashCons( $2 );   SAVE($$, @1); }
;

keyword.argument.list
	: T_KEYWORD assignment.expression                          { $$.sym = $1;
                                                                     $$.list = makeList( TRUE, FALSE, NULL, $2 ); SAVE($$.list, @1);    }
	| keyword.argument.list T_KEYWORD assignment.expression    { $$.sym = symconcat( $1.sym, $2 );
                                                                     $$.list = makeList( TRUE, FALSE, $1.list, $3 ); SAVE($$.list, @1); }
	| keyword.argument.list ',' argument.list                  { $$.sym = $1.sym;
                                                                     $$.list = listconcat( $1.list, $3 ); SAVE($$.list, @1); }
;

keyword.parameter.list
	: T_KEYWORD parameter                                      { $$.sym = $1;
                                                                     $$.list = makeList( TRUE, FALSE, NULL, $2 ); SAVE($$.list, @1);    }
	| keyword.parameter.list T_KEYWORD parameter               { $$.sym = symconcat( $1.sym, $2 );
                                                                     $$.list = makeList( TRUE, FALSE, $1.list, $3 ); SAVE($$.list, @1); }
	| keyword.parameter.list ',' basic.parameter.list          { $$.sym = $1.sym;
                                                                     $$.list = listconcat( $1.list, $3 ); SAVE($$.list, @1); }
;

argument.list
	: assignment.expression                                    { $$ = makeList( TRUE, FALSE, NULL, $1 ); SAVE($$, @1); }
	| argument.list ',' assignment.expression                  { $$ = makeList( TRUE, FALSE, $1, $3 );   SAVE($$, @1); }
;

unary.expression
	: postfix.expression                                       { $$ = $1;                                        }
	| '+' unary.expression                                     { $$ = makeUnary( POS_OP, $2 );     SAVE($$, @1); }
	| '-' unary.expression                                     { $$ = makeUnary( NEG_OP, $2 );     SAVE($$, @1); }
	| '~' unary.expression                                     { $$ = makeUnary( INVERT_OP, $2 );  SAVE($$, @1); }
	| '!' unary.expression                                     { $$ = makeUnary( LNOT_OP, $2 );    SAVE($$, @1); }
	| T_INC lvalue.expression                                  { $$ = makeUnary( PREINC_OP, $2 );  SAVE($$, @1); }
	| T_DEC lvalue.expression                                  { $$ = makeUnary( PREDEC_OP, $2 );  SAVE($$, @1); }
	| lvalue.expression T_INC                                  { $$ = makeUnary( POSTINC_OP, $1 ); SAVE($$, @2); }
	| lvalue.expression T_DEC                                  { $$ = makeUnary( POSTDEC_OP, $1 ); SAVE($$, @2); }
;

exponentiation.expression
	: unary.expression                                         { $$ = $1;                                         }
	| exponentiation.expression T_POW unary.expression         { $$ = makeBinary( POW_OP, $1, $3 ); SAVE($$, @2); }
;

multiplicative.expression
	: exponentiation.expression                                { $$ = $1;                                         }
	| multiplicative.expression '*' exponentiation.expression  { $$ = makeBinary( MUL_OP, $1, $3 ); SAVE($$, @2); }
	| multiplicative.expression '/' exponentiation.expression  { $$ = makeBinary( DIV_OP, $1, $3 ); SAVE($$, @2); }
	| multiplicative.expression '%' exponentiation.expression  { $$ = makeBinary( MOD_OP, $1, $3 ); SAVE($$, @2); }
;

additive.expression
	: multiplicative.expression
	| additive.expression '+' multiplicative.expression        { $$ = makeBinary( ADD_OP, $1, $3 ); SAVE($$, @2); }
	| additive.expression '-' multiplicative.expression        { $$ = makeBinary( SUB_OP, $1, $3 ); SAVE($$, @2); }
;

shift.expression
	: additive.expression
	| shift.expression T_LSHIFT additive.expression            { $$ = makeBinary( LSHIFT_OP, $1, $3 ); SAVE($$, @2); }
	| shift.expression T_RSHIFT additive.expression            { $$ = makeBinary( RSHIFT_OP, $1, $3 ); SAVE($$, @2); }
;

relational.expression
	: shift.expression                                         { $$ = $1;                                        }
	| relational.expression '<' shift.expression               { $$ = makeBinary( LT_OP, $1, $3 ); SAVE($$, @2); }
	| relational.expression '>' shift.expression               { $$ = makeBinary( GT_OP, $1, $3 ); SAVE($$, @2); }
	| relational.expression T_LE shift.expression              { $$ = makeBinary( LE_OP, $1, $3 ); SAVE($$, @2); }
	| relational.expression T_GE shift.expression              { $$ = makeBinary( GE_OP, $1, $3 ); SAVE($$, @2); }
;

equality.expression
	: relational.expression                                    { $$ = $1;                          SAVE($$, @1); }
	| equality.expression T_EQ relational.expression           { $$ = makeBinary( EQ_OP, $1, $3 ); SAVE($$, @2); }
	| equality.expression T_NE relational.expression           { $$ = makeBinary( NE_OP, $1, $3 ); SAVE($$, @2); }
;

and.expression
	: equality.expression                                      { $$ = $1;                                         }
	| and.expression '&' equality.expression                   { $$ = makeBinary( AND_OP, $1, $3 ); SAVE($$, @2); }
;

exclusive.or.expression
	: and.expression                                           { $$ = $1;                                         }
	| exclusive.or.expression '^' and.expression               { $$ = makeBinary( XOR_OP, $1, $3 ); SAVE($$, @2); }
;

inclusive.or.expression
	: exclusive.or.expression                                  { $$ = $1;                                        }
	| inclusive.or.expression '|' exclusive.or.expression      { $$ = makeBinary( OR_OP, $1, $3 ); SAVE($$, @2); }
;

in.expression
	: inclusive.or.expression                                  { $$ = $1;                                        }
	| in.expression T_IN inclusive.or.expression               { $$ = makeBinary( IN_OP, $1, $3 ); SAVE($$, @2); }
;

logical.and.expression
	: in.expression                                            { $$ = $1;                              }
	| logical.and.expression T_LAND equality.expression        { $$ = makeAnd( $1, $3 ); SAVE($$, @2); }
;

logical.or.expression
	: logical.and.expression                                   { $$ = $1;                             }
	| logical.or.expression T_LOR logical.and.expression       { $$ = makeOr( $1, $3 ); SAVE($$, @2); }
;

conditional.expression
	: logical.or.expression                                    { $$ = $1;                            }
	| logical.or.expression '?' expression ':' conditional.expression
                                                                   { $$ = makeConditional( $1, $3, $5 ); SAVE($$, @2); }
;

assignment.expression
	: conditional.expression                                   { $$ = $1;                                 }
	| lvalue.expression '=' assignment.expression              { $$ = makeAssign( $1, $3 ); SAVE($$, @2); }
	| lvalue.expression T_POWASSIGN assignment.expression      { $$ = makeAssign( $1, makeBinary( POW_OP, $1, $3 ) ); SAVE($$, @2);    }
	| lvalue.expression T_MULASSIGN assignment.expression      { $$ = makeAssign( $1, makeBinary( MUL_OP, $1, $3 ) ); SAVE($$, @2);    }
	| lvalue.expression T_DIVASSIGN assignment.expression      { $$ = makeAssign( $1, makeBinary( DIV_OP, $1, $3 ) ); SAVE($$, @2);    }
	| lvalue.expression T_MODASSIGN assignment.expression      { $$ = makeAssign( $1, makeBinary( MOD_OP, $1, $3 ) ); SAVE($$, @2);    }
	| lvalue.expression T_ADDASSIGN assignment.expression      { $$ = makeAssign( $1, makeBinary( ADD_OP, $1, $3 ) ); SAVE($$, @2);    }
	| lvalue.expression T_SUBASSIGN assignment.expression      { $$ = makeAssign( $1, makeBinary( SUB_OP, $1, $3 ) ); SAVE($$, @2);    }
	| lvalue.expression T_LSHIFTASSIGN assignment.expression   { $$ = makeAssign( $1, makeBinary( LSHIFT_OP, $1, $3 ) ); SAVE($$, @2); }
	| lvalue.expression T_RSHIFTASSIGN assignment.expression   { $$ = makeAssign( $1, makeBinary( RSHIFT_OP, $1, $3 ) ); SAVE($$, @2); }
	| lvalue.expression T_ANDASSIGN assignment.expression      { $$ = makeAssign( $1, makeBinary( AND_OP, $1, $3 ) ); SAVE($$, @2);    }
	| lvalue.expression T_XORASSIGN assignment.expression      { $$ = makeAssign( $1, makeBinary( XOR_OP, $1, $3 ) ); SAVE($$, @2);    }
	| lvalue.expression T_ORASSIGN assignment.expression       { $$ = makeAssign( $1, makeBinary( OR_OP, $1, $3 ) ); SAVE($$, @2);     }
;

lvalue.list
	: lvalue.expression                                        { $$ = makeList( FALSE, FALSE, NULL, $1 ); SAVE($$, @1); }
	| lvalue.list ',' lvalue.expression                        { $$ = makeList( FALSE, FALSE, $1, $3 );   SAVE($$, @1); }
;

sim.assignment.expression
	: '[' lvalue.list ']' '=' '[' argument.list ']'            { $$ = makeSimultaneousAssign( $2, $6 ); SAVE($$, @4); }
	| '[' lvalue.list ']' '=' assignment.expression            { $$ = makeSimultaneousAssign( $2, $5 ); SAVE($$, @4); }
;

expression
	: assignment.expression                                    { $$ = $1;                           }
	| sim.assignment.expression                                { $$ = $1;                           }
;

statement
	: labeled.statement                                        { $$ = makeStmt( $1 ); SAVE($$, @1); }
	| compound.statement                                       { $$ = makeStmt( $1 ); SAVE($$, @1); }
	| expression.statement                                     { $$ = makeStmt( makeExprStmt( $1 ) ); SAVE($$, @1); }
	| selection.statement                                      { $$ = makeStmt( $1 ); SAVE($$, @1); }
	| iteration.statement                                      { $$ = makeStmt( $1 ); SAVE($$, @1); }
	| try.statement                                            { $$ = makeStmt( $1 ); SAVE($$, @1); }
	| jump.statement                                           { $$ = makeStmt( $1 ); SAVE($$, @1); }
	| import.statement                                         { $$ = makeStmt( $1 ); SAVE($$, @1); }
	| function.definition                                      { $$ = makeStmt( $1 ); SAVE($$, @1); }
	| declaration                                              { $$ = makeStmt( $1 ); SAVE($$, @1); }
;

labeled.statement
	: T_KEYWORD statement                                      { $$ = makeLabeledStmt( $1, $2 ); SAVE($$, @1); }
;

compound.statement
	: '{' '}'                                                  { $$ = NULL; }
	| '{' statement.list '}'                                   { $$ = $2;   }
;

statement.list
	: statement                                                { $$ = makeList( FALSE, FALSE, NULL, $1 ); SAVE($$, @1); }
	| statement.list statement                                 { $$ = makeList( FALSE, FALSE,   $1, $2 ); SAVE($$, @1); }
;

expression.statement
	: ';'                                                      { $$ = NULL; }
	| expression ';'                                           { $$ = $1;   }
;

selection.statement
	: T_IF '(' expression ')' statement                        { $$ = makeIf( $3, $5, NULL ); SAVE($$, @1); }
	| T_IF '(' expression ')' statement T_ELSE statement       { $$ = makeIf( $3, $5,   $7 ); SAVE($$, @1); }
;

iteration.statement
	: T_WHILE '(' expression ')' statement                                           { $$ = makeWhile( $3, $5 );         SAVE($$, @1); }
	| T_DO statement T_WHILE '(' expression ')' ';'                                  { $$ = makeDo( $5, $2 );            SAVE($$, @1); }
	| T_FOR '(' statement expression.statement ')' statement                         { $$ = makeFor( $3, $4, NULL, $6 ); SAVE($$, @1); }
	| T_FOR '(' statement expression.statement expression ')' statement              { $$ = makeFor( $3, $4,   $5, $7 ); SAVE($$, @1); }
	| T_FOR '(' variable T_IN expression ')' statement                               { $$ = makeForIn( $3, $5, $7 );     SAVE($$, @1); }
;

jump.statement
	: T_RETURN ';'                                             { $$ = makeReturn( NULL );                 SAVE($$, @1); }
	| T_RETURN expression ';'                                  { $$ = makeReturn( $2 );                   SAVE($$, @1); }
	| T_BREAK ';'                                              { $$ = makeBreak( NULL );                  SAVE($$, @1); }
	| T_BREAK T_INUMBER ';'                                    { $$ = makeBreak( makeConstInt( $2 ) );    SAVE($$, @1); }
	| T_CONTINUE ';'                                           { $$ = makeContinue( NULL );               SAVE($$, @1); }
	| T_CONTINUE T_INUMBER ';'                                 { $$ = makeContinue( makeConstInt( $2 ) ); SAVE($$, @1); }
	| T_GOTO symbol ';'                                        { $$ = makeGoto( $2 );                     SAVE($$, @1); }
	| T_THROW expression ';'                                   { $$ = makeThrow( $2 );                    SAVE($$, @1); }
;

import.statement
	: T_IMPORT qualified.symbol ';'                            { $$ = makeImport( FALSE, $2, NULL);  SAVE($$, @1); }
	| T_FROM   qualified.symbol T_IMPORT symbol.list ';'       { $$ = makeImport( TRUE,  $2, $4 );   SAVE($$, @1); }
	| T_FROM   qualified.symbol T_IMPORT '*' ';'               { $$ = makeImport( TRUE,  $2, NULL ); SAVE($$, @1); }
;

try.statement
	: T_TRY compound.statement catch.list                      { $$ = makeTry( $2, $3 ); SAVE($$, @1); }
;

catch.list
	: catch.statement                                          { $$ = makeList( FALSE, FALSE, NULL, $1 ); SAVE($$, @1); }
	| catch.list catch.statement                               { $$ = makeList( FALSE, FALSE,   $1, $2 ); SAVE($$, @1); }
;

catch.statement
	:  T_CATCH '(' qualified.symbol ')' compound.statement           { $$ = makeCatch( $3, NULL, $5 );  SAVE($$, @1); }
	|  T_CATCH '(' qualified.symbol variable ')' compound.statement  { $$ = makeCatch( $3,   $4, $6 );  SAVE($$, @1); }
;

/* Parameter list */

parameter
	: variable initialization				   { $$ = makeParam( $1, $2 ); SAVE($$, @1);               }
;

basic.parameter.list
	: parameter                                                { $$ = makeList( TRUE, FALSE, NULL, $1 ); SAVE($$, @1); }
	| basic.parameter.list ',' parameter                       { $$ = makeList( TRUE, FALSE,   $1, $3 ); SAVE($$, @1); }
;

parameter.list
	: /* EMPTY */                                              { $$ = makeParamList( FALSE, NULL ); SAVE($$, @1);       }
	| basic.parameter.list                                     { $$ = makeParamList( FALSE, $1 ); SAVE($$, @1);         }
	| basic.parameter.list T_ELLIPSIS                          { $$ = makeParamList( TRUE,  $1 ); SAVE($$, @1);         }
;

/* Functions */

function.definition
	: T_FUNCTION variable '(' parameter.list ')' compound.statement
		{ $$ = makeFunction( $2, DECLVAR( SymbolLocal,   $2 ), $4, $6 ); if ($6) $6->vStmtList.retVal = TRUE; SAVE($$, @1); }
	| T_STATIC T_FUNCTION variable '(' parameter.list ')' compound.statement
		{ $$ = makeFunction( $3, DECLVAR( SymbolStatic,  $3 ), $5, $7 ); if ($7) $7->vStmtList.retVal = TRUE; SAVE($$, @1); }
	| T_PRIVATE T_FUNCTION variable '(' parameter.list ')' compound.statement
		{ $$ = makeFunction( $3, DECLVAR( SymbolPrivate, $3 ), $5, $7 ); if ($7) $7->vStmtList.retVal = TRUE; SAVE($$, @1); }
	| T_PUBLIC T_FUNCTION variable '(' parameter.list ')' compound.statement
		{ $$ = makeFunction( $3, DECLVAR( SymbolPublic,  $3 ), $5, $7 ); if ($7) $7->vStmtList.retVal = TRUE; SAVE($$, @1); }
;

/* Classes */

class.definition
	: T_PRIVATE T_CLASS symbol T_EXTENDS qualified.symbol class.body
		{ $$ = makeClass( DECLSYM( SymbolPrivate, $3 ), $3, $5, $6 ); SAVE($$, @1); }
	| T_PUBLIC T_CLASS symbol T_EXTENDS qualified.symbol class.body
		{ $$ = makeClass( DECLSYM( SymbolPublic, $3 ), $3, $5, $6 );  SAVE($$, @1); }
;

class.body
	: '{' '}'                                                  { $$ = NULL; }
	| '{' class.statement.list '}'                             { $$ = $2;   }
;

class.statement.list
	: class.statement                                          { $$ = makeList( FALSE, FALSE, NULL, $1 ); SAVE($$, @1); }
	| class.statement.list class.statement                     { $$ = makeList( FALSE, FALSE,   $1, $2 ); SAVE($$, @1); }
;

class.statement
	: declaration                                              { $$ = $1; }
	| method.definition                                        { $$ = $1; }
;

method.definition
	: T_METHOD symbol '(' parameter.list ')' compound.statement
		{ $$ = makeMethod( FALSE, $2, $4, $6 ); if ($6) $6->vStmtList.retVal = TRUE; SAVE($$, @1); }
	| T_METHOD keyword.parameter.list compound.statement
		{ $$ = makeMethod( FALSE, makeSymbol( $2.sym ), makeParamList( FALSE, $2.list ), $3 );
		  if ($3) $3->vStmtList.retVal = TRUE; SAVE($$, @1); }
	| T_METHOD keyword.parameter.list T_ELLIPSIS compound.statement
		{ $$ = makeMethod( FALSE, makeSymbol( $2.sym ), makeParamList( TRUE, $2.list ), $4 );
		  if ($4) $4->vStmtList.retVal = TRUE; SAVE($$, @1); }
	| T_CLASS T_METHOD symbol '(' parameter.list ')' compound.statement
		{ $$ = makeMethod( TRUE, $3, $5, $7 ); if ($7) $7->vStmtList.retVal = TRUE; SAVE($$, @1); }
	| T_CLASS T_METHOD keyword.parameter.list compound.statement
		{ $$ = makeMethod( TRUE, makeSymbol( $3.sym ), makeParamList( FALSE, $3.list ), $4 );
		  if ($4) $4->vStmtList.retVal = TRUE; SAVE($$, @1); }
	| T_CLASS T_METHOD keyword.parameter.list T_ELLIPSIS compound.statement
		{ $$ = makeMethod( TRUE, makeSymbol( $3.sym ), makeParamList( TRUE, $3.list ), $5 );
		  if ($5) $5->vStmtList.retVal = TRUE; SAVE($$, @1); }
;

/* Declarations */

declaration
	: T_LOCAL   decl.list ';'                                  { $$ = makeDecl( SymbolLocal,   $2 ); SAVE($$, @1); }
	| T_STATIC  decl.list ';'                                  { $$ = makeDecl( SymbolStatic,  $2 ); SAVE($$, @1); }
	| T_PRIVATE decl.list ';'                                  { $$ = makeDecl( SymbolPrivate, $2 ); SAVE($$, @1); }
	| T_PUBLIC  decl.list ';'                                  { $$ = makeDecl( SymbolPublic,  $2 ); SAVE($$, @1); }
;

decl.atom
	: symbol initialization                                    { $$ = makeDeclAtom( $1, $2 ); SAVE($$, @1); }
;

decl.list
	: decl.atom                                                { $$ = makeList( FALSE, FALSE, NULL, $1 ); SAVE($$, @1); }
	| decl.list ',' decl.atom                                  { $$ = makeList( FALSE, FALSE,   $1, $3 ); SAVE($$, @1); }
;

initialization
	: /* EMPTY */                                              { $$ = NULL; }
	| '=' expression                                           { $$ = $2;   }
;

variable
	: qualified.symbol                                         { $$ = makeVariable( $1 ); SAVE($$, @1); }
;

qualified.symbol
	: symbol                                                   { $$ = makeQualifiedSymbol( NULL, $1 ); SAVE($$, @1); }
	| qualified.symbol T_QUALIFIER symbol                      { $$ = makeQualifiedSymbol( $1,   $3 ); SAVE($$, @1); }
;

symbol
	: T_SYMBOL                                                 { $$ = makeSymbol( EcInternSymbol( $1 ) );
								     ec_free( $1 ); SAVE($$, @1); }
;

symbol.list
	: symbol                                                   { $$ = makeList( FALSE, FALSE, NULL, $1 ); SAVE($$, @1); }
	| symbol.list ',' symbol                                   { $$ = makeList( FALSE, FALSE,   $1, $3 ); SAVE($$, @1); }
;

/* Package structure */

package.declaration
	: T_PACKAGE qualified.symbol ';'                           { $$ = makePackage( $2 ); }
;

package.statement
	: package.declaration					   { $$ = $1; }
	| statement                                                { $$ = $1; }
	| class.definition                                         { $$ = $1; }
;

package.statement.list
	: package.statement                                        { $$ = makeList( FALSE, FALSE, NULL, $1 ); SAVE($$, @1); }
	| package.statement.list package.statement                 { $$ = makeList( FALSE, FALSE,   $1, $2 ); SAVE($$, @1); }
;

/* Main Translation Unit */

translation.unit
	: package.statement.list                                   { $$ = $1; PRIVATE(parse_result) = $$; }
;

/* End of grammar */
%%

int yyerror( char *s )  /* Called by yyparse on error */
{
	fprintf( stderr, "ERROR: (LINE: %ld, COLUMN: %ld): %s\n", (long)PRIVATE(line), (long)PRIVATE(column), s );
	return 0;
}

static EcUInt symconcat( EcUInt sym1, EcUInt sym2 )
{
	const char *bsym1;
	const char *bsym2;
	char *buf;

	bsym1 = EcSymbolAt( sym1 );
	bsym2 = EcSymbolAt( sym2 );
	buf = alloca( strlen( bsym1 ) + strlen( bsym2 ) + 1 );
	strcpy( buf, bsym1 );
	strcat( buf, bsym2 );
	return EcInternSymbol( buf );
}

static void saveLoc( ASTLocation *dst, void *vsrc )
{
	YYLTYPE *src;

	src = vsrc;

	if (dst)
	{
		dst->first_line   = src->first_line;
		dst->first_column = src->first_column;
		dst->last_line    = src->last_line;
		dst->last_column  = src->last_column;
	}
}

static ASTNode listconcat( ASTNode l1, ASTNode l2 )
{
	ASTNode node;

	/* Get to the last position in prefix list */
	node = l1;
	while (node)
	{
		if (! node->vStmtList.next) break;

		node = node->vStmtList.next;
	}

	if (node)
	{
		node->vStmtList.next = l2;
	}

	return l1;
}
