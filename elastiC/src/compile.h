/* ==========================================================================
 * compile.h
 * ==========================================================================
 * Bytecode Compiler.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat May 23 18:12:33 MEST 1998
 *
 *   $Id: compile.h,v 1.1 2002/05/23 21:16:53 panta Exp $
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

#ifndef __COMPILE_H
#define __COMPILE_H

typedef enum { SymbolParam, SymbolLocal, SymbolStatic, SymbolPrivate, SymbolPublic } SymbolClass;
typedef enum { PackageScope, FunctionScope, BlockScope, ClassScope } ScopeType;
typedef enum { NormalFunction, MethodFunction, ClassMethodFunction } FunctionType;
typedef struct ScopeStruct       *Scope;
typedef struct STableEntryStruct *STableEntry;

#include <elastic/object.h>
#include <elastic/elastic.h>
#include <elastic/ast.h>
#include <elastic/strtable.h>
#include <elastic/utility.h>

EC_BEGIN_DECLS

/* ========================================================================
 * DATA TYPES FOR COMPILATION PROCESS
 * ======================================================================== */

/*
 * Scope
 *
 * A scope is a compile-time environment frame.
 * An environment is defined by a chain of scopes (from inner scope to outer scope).
 * Location of a symbol in an environment is determined by a pair of integers
 * (up, pos), where 'up' says how many scopes to pass over in the chain from the current,
 * and pos is the position in that scope of the symbol. This pair parallels the location
 * of a variable in a run-time context.
 *
 * A scope also keeps track of current target bytecode.
 */
struct ScopeStruct
{
	ScopeType type;												/* Package or function scope                    */
	Scope upper;

	EC_OBJ  package;											/* Current package                              */
	EC_OBJ  target;												/* Current target                               */
	EC_OBJ  bytecode;											/* Current bytecode target                      */
	EC_OBJ  lframe;												/* Current literal frame                        */

	FunctionType functype;										/* Function type (normal, method, class method) */

	EcInt stackdim;												/* Current stack dimension                      */
	EcInt maxstackdim;											/* The stack will need to be as little as this  */

	STableEntry symtab;											/* Symbols (parameters, locals, statics)        */

	/*
	 * Functional state
	 *
	 * Used during compilation to keep track of cycle entry points
	 * and locations to backpatch for unconditional jumps.
	 * Relevant for functions and packages.
	 */
	ec_arena *break_pos;										/* locations to patch for 'break'     */
	ec_arena *break_lev;										/* associated 'break' cycle levels    */
	ec_arena *continue_pos;										/* locations to patch for 'continue'  */
	ec_arena *continue_lev;										/* associated 'continue' cycle levels */
	EcInt cycle_nesting;										/* Cycle nesting level                */

	/* Service variable info */
	EcUInt next_srv_var;
};


/*
 * Symbol table for an open scope
 */
struct STableEntryStruct
{
	EcUInt      symid;								/* Symbol id                            */
	SymbolClass symclass;							/* Symbol class (local,static, ...)		*/
	EcBool      isconst;							/* Constantness of symbol               */
	EcBool      initialized;						/* Initialized                          */
	EcInt       position;							/* Allocated position                   */

	STableEntry next;
};


/*
 * Variable reference information
 *
 * WARNING:
 *   1. pkg & up are mutually exclusive (i.e. they could be grouped in a union).
 *   2. indexed is NOT set by getSymbol(), but by variable compilation
 *      (see compileArrayRef()).
 */
typedef struct
{
	SymbolClass vclass;											/* variable class (parameter, local, ...)        */
	EcInt       pkg;											/* package index                                 */
	EcInt       up;												/* # of levels up in lexical environment         */
	EcInt       pos;											/* position in [stack frame|literal frame|...]   */
	EcBool      cnst;											/* TRUE if variable is constant ;)               */
	EcBool      initialized;									/* TRUE if variable is (heuristically) initial.  */
	EcBool      indexed;										/* TRUE if variable is indexed (array-like ref.) */
	Scope       scope;											/* Scope of declaration                          */
	STableEntry entry;											/* symbol table entry                            */
} EcVarRefStruct, *EcVarRef;

#define V_CLASS(v)			((v)->vclass)
#define V_PKG(v)			((v)->pkg)
#define V_UP(v)				((v)->up)
#define V_POS(v)			((v)->pos)
#define V_CONST(v)			((v)->cnst)
#define V_INITIALIZED(v)	((v)->initialized)
#define V_INDEXED(v)		((v)->indexed)
#define V_SCOPE(v)			((v)->scope)
#define V_ENTRY(v)			((v)->entry)


/* ========================================================================
 * A P I
 * ======================================================================== */


EC_END_DECLS

#endif /* __COMPILE_H */
