/* ==========================================================================
 * object.h
 * ==========================================================================
 * Object handling.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat May 23 11:02:52 MEST 1998
 *
 *   $Id: object.h,v 1.4 2005/03/22 10:09:34 panta Exp $
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

#ifndef __OBJECT_H
#define __OBJECT_H

#include <elastic/basic.h>

EC_BEGIN_DECLS

#define EC_INLINED_TYPES 1

typedef struct _obj *EC_OBJ;
typedef struct EcPrivateStruct EcPrivate;
typedef EC_OBJ (*EcCPrimitive)( EC_OBJ stack, EcAny userdata );
typedef EC_OBJ (*EcCMethod)( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
typedef EC_OBJ (*EcCCallable)( EC_OBJ self, EC_OBJ at_class, EC_OBJ compiled, EC_OBJ stack ); /* #JP */

#include <elastic/basic.h>
#include <elastic/user.h>
#include <elastic/elastic.h>
#include <elastic/bytecode.h>
#include <elastic/oop.h>

typedef struct _stack
{
	EC_OBJ *sp;													/* Stack pointer                             */
	EC_OBJ *bp;													/* Base Pointer                              */
	EC_OBJ *base;												/* Array that constitutes the stack          */
	EcUInt  dim;												/* Stack dimension (for passivation)         */

	EC_OBJ  up;													/* Caller stack frame                        */
	EC_OBJ  lexical;											/* Lexically upper stack                     */

	EcUInt  PC;													/* Program Counter to return to              */

	/*
	 * nargs
	 *
	 * Flavors of n. of args:
	 *
	 *  REAL   : real # of stack slots occupied by args
	 *  LOGICAL: logical # of arguments passed
	 *
	 * Distinction:
	 *  For C primitives & C methods there is no distinction,
	 *  but for bytecode functions & methods:
	 *   REAL    = req. nargs + (0 or 1 for varargs)
	 *   LOGICAL = req. nargs + # of varargs
	 *
	 * P.S. req. nargs (and so real and logical) don't take
	 *      into account self and at_class
	 */
	EcInt   nargs_real;											/* Number of *REAL* arguments passed in      */
	EcInt   nargs_logical;										/* Number of *LOGICAL* arguments passed in   */

	/* Inlined code blocks */
	EC_OBJ  immediateUp;										/* 'up' stack for inlined return      */
	EC_OBJ  immediateCaller;									/* 'caller' object for inlined return */
	EcUInt  immediatePC;										/* 'PC' for inlined return            */

	EC_OBJ  called;												/* Called object */

#if EC_STACK_RECYCLE
	int ref;													/* reference count for stack recycling */
#endif

	/* Static space for small stack */
#if EC_STACK_USES_STATIC
	EC_OBJ static_space[EC_STACK_STATIC_SIZE];
#endif
} EcStack;

typedef struct _compiled
{
	EcBytecode *code;											/* Bytecodes				       */
	EcInt       ncode;

	/*
	 * Actual number of arguments could be greater than nargs.
	 *
	 * At run-time stack is allocated with a dimension calculated following the formula:
	 *  stackdim =            1 +          for the caller bytecode
	 *                        2 +          for (self, at_class)
	 *             passed args. +          arguments *actually* passed
	 *                  nlocals +
	 *                 maxtemps
	 */
	EcInt  nargs;												/* (Minimum) number of parameters                 */
	EcInt  nargs_def;											/* Of the above #, these many have default values */
	EcBool vargs;												/* Can accept variable n. of args                 */
	EcInt  nlocals;												/* Number of locals	                              */
	EcInt  maxtemps;											/* (Maximum) number of temporaries                */

	EC_OBJ lexical;												/* Lexical environment (tc_stack)  */

	/* Literal frame
	 *
	 * The literal frame maintains constants and static variables
	 */
	EC_OBJ lframe;												/* Literal Frame (tc_array)	*/

	EC_OBJ handler;												/* Array (tc_array) of handlers (tc_handler) */

	EC_OBJ package;												/* Package containing this code    */
	EC_OBJ name;												/* Name, if applicable             */
	EcBool ismethod;											/* TRUE if method                  */

	/*
	 * Debug/info data
	 *
	 * The debugging/info data is kept in an elastiC hash.
	 * This hash keeps different types of information as elastiC
	 * objects. Each one is indexed with a specific name (as a symbol).
	 *   #line       - line number information
	 *                 elastiC array of tuples (bytecode index, line number)
	 */
	EC_OBJ info;												/* info object*/

	EC_OBJ docstring;											/* documentation string */

	EcCCallable ccallable;										/* C callable #JP */
} EcCompiled;

struct ec_public_sym
{
	EcUInt sym;													/* Symbol id    */
	EcBool isconst;												/* Constantness */
	EcInt  pos;													/* Position     */
};

/*
 * Package variable layout
 *
 * Private & public variables reside on EC_PACKAGEFRAME(package) (frame field)
 * Local variables reside on the stack of EC_PACKAGECODE(package)
 * Static variables reside on literal frame of EC_PACKAGECODE(package)
 */
typedef struct _package
{
	EC_OBJ name;												/* Package name             */
	EC_OBJ code;												/* Bytecode (tc_compiled)	*/

	/* Package frame
	 *
	 * The package frame maintains package-global variables (public variables, functions, ...)
	 * and variables private to the package.
	 */
	EC_OBJ frame;												/* Package frame (tc_array)	*/

	EC_OBJ import;												/* Import list              */

	EcInt  nexport;												/* Export list              */
	struct ec_public_sym *export;

	EC_OBJ source;												/* Source file              */
} EcPackage;

struct _obj
{
	EcWord type;
	EcWord flags;
	/*          15              0
	 *           ----------------
	 *  flags   |CxxxPGVOOOORRRRM|
	 *           ----------------
	 * ----------------------------------------------------------------------
	 * BIT     |   MEANING
	 * ----------------------------------------------------------------------
	 * 0    M  |   marking flag for GC
	 * 1-4  R  |   (tc_primitive) required number of parameters
	 * 5-8  O  |   (tc_primitive) optional number of parameters
	 * 9    V  |   (tc_primitive) flag: procedure receives remaining parameters
	 * 10   G  |   (tc_primitive) userdata is an elastiC object: needs memory
	 *         |                  handling (GC, ...)
	 * 11   P  |   (tc_primitive) userdata is a registered C pointer
	 * -    x  |   reserved
	 * 15   C  |   object is constant
	 * ----------------------------------------------------------------------
	 */

	union
	{
		EcBool  boolval;
#if ! EC_INLINED_TYPES
		EcInt   ival;
#endif
		EcFloat fval;
		EcChar  cval;
#if ! EC_INLINED_TYPES
		EcUInt  symbolid;
#endif

		/* String */
		struct {
			EcInt  len;
			char  *data;
		} strng;

		/* Array */
		struct {
			EC_OBJ *arr;
			EcInt   used;
			EcInt   real;
		} array;

		/* Stack */
		EcStack *stack;

		/* C Primitive */
		struct {
			EcCPrimitive  impl;									/* Implementation                   */
			const char   *name;
			EcAny         userdata;
		} primitive;

		/* C Method */
		struct {
			EcCMethod   impl;									/* Implementation                   */
			const char *name;
		} cmethod;

		/* Compiled function */
		EcCompiled *compiled;

		/* Handler */
		struct {
			EC_OBJ type;										/* Catch on this type               */
			EC_OBJ code;
		} handler;

		/* Package */
		EcPackage *package;

		/* Class */
		EcClass   *cls;

		/* Object */
		EcObject  *object;

		/* User defined type */
		struct {
			union {
				void  *user;
				EcAny  user0;
			} user;
			EcAny  user1;
			EcAny  user2;
		} usr;
	} v;
};

/*
 * Inlined types pointer (EC_OBJ) structure:
 *
 *  31                              0
 *   ddddddddddddddddddddddddddddddii
 *
 * where:
 *   dd...d     - data
 *   ii         - type identifier
 *
 *  ii            Type
 *  --------------------------------
 *  00            not inlined object
 *  01            integer
 *  10            symbol
 *  11            character (INLINING NOT IMPLEMENTED RIGHT NOW)
 *
 * EC_TYPE() implementation requires that inlined types have adjacent
 * tc_??? codes (now only tc_inum and tc_symbol).
 */

#define tc_none			0x0000
#define tc_undefined	0x0001
#define tc_error		0x0002
#define tc_bool			0x0003
#define tc_inum			0x0004
#define tc_symbol		0x0005
#define tc_fnum			0x0006
#define tc_primitive	0x0007
#define tc_cmethod		0x0008
#define tc_userbase		0x0009

/*
#define tc_char			0x0005
#define tc_array		0x0007
#define tc_stack		0x0008
#define tc_primitive	0x0009
#define tc_cmethod		0x000A
#define tc_compiled		0x000B
#define tc_handler		0x000C
#define tc_package		0x000D
#define tc_class		0x000E
#define tc_object		0x000F
#define tc_userbase		0x0010*/

extern EC_API EcInt tc_array;
extern EC_API EcInt tc_char;
extern EC_API EcInt tc_string;
extern EC_API EcInt tc_stack;
extern EC_API EcInt tc_compiled;
extern EC_API EcInt tc_handler;
extern EC_API EcInt tc_package;
extern EC_API EcInt tc_class;
extern EC_API EcInt tc_object;


/* ========================================================================
 * M A C R O S
 * ======================================================================== */

#define EC_NIL						((EC_OBJ)NULL)

#define EC_NULLP(obj)				((int)(EcPointerInteger) !(obj))
#define EC_NNULLP(obj)				((int)(EcPointerInteger) (obj))

#if EC_INLINED_TYPES

#define EC_INLINED_BITS				2
#define EC_INLINED_MASK				0x03L
#define EC_INLINED(obj)				(((unsigned long)(obj)) & EC_INLINED_MASK)

#define EC_TYPEID_INT				0x01L
#define EC_TYPEID_SYMBOL			0x02L
#define EC_TYPEID_CHAR				0x03L						/* UNUSED NOW */
#define EC_INLINED_OFFSET			3

#define EC_OBJTYPE(obj)				((obj)->type)
#define EC_TYPE(obj)				(EC_INLINED(obj) ? (EC_INLINED(obj) + EC_INLINED_OFFSET) : EC_OBJTYPE(obj))

#define EC_INUMP(obj)				(((unsigned long)(obj)) & EC_TYPEID_INT)
#define EC_SYMBOLP(obj)				(((unsigned long)(obj)) & EC_TYPEID_SYMBOL)
#define EC_MAKE_INT(value)			((EC_OBJ)((((signed long)(value)) << 2) | EC_TYPEID_INT))
#define EC_MAKE_SYMBOL(symbolid)	((EC_OBJ)((((EcUInt)(symbolid)) << 2) | EC_TYPEID_SYMBOL))
#define EC_INUM(obj)				((EcInt) (((signed long)(obj)) >> 2))
#define EC_SYMBOL(obj)				((EcUInt) (((EcPointerInteger)(obj)) >> 2))

#define EC_INUM_ADD(obj1, obj2)		((EC_OBJ) (((((signed long)obj1) + ((signed long)obj2)) & (~EC_INLINED_MASK)) | EC_TYPEID_INT))
#define EC_INUM_SUB(obj1, obj2)		((EC_OBJ) ((((signed long)obj1) - ((signed long)obj2)) | EC_TYPEID_INT))
#define EC_INUM_AND(obj1, obj2)		((EC_OBJ) ((((signed long)obj1) & ((signed long)obj2))))
#define EC_INUM_OR(obj1, obj2)		((EC_OBJ) ((((signed long)obj1) | ((signed long)obj2))))
#define EC_INUM_XOR(obj1, obj2)		((EC_OBJ) (((((signed long)obj1) ^ ((signed long)obj2)) & (~EC_INLINED_MASK)) | EC_TYPEID_INT))

#define EC_INUM_LT(obj1, obj2)		(((signed long)obj1) < ((signed long)obj2))
#define EC_INUM_LE(obj1, obj2)		(((signed long)obj1) <= ((signed long)obj2))
#define EC_INUM_GT(obj1, obj2)		(((signed long)obj1) > ((signed long)obj2))
#define EC_INUM_GE(obj1, obj2)		(((signed long)obj1) >= ((signed long)obj2))
#define EC_INUM_EQ(obj1, obj2)		((obj1) == (obj2))

#else /* ! EC_INLINED_TYPES */

#define EC_OBJTYPE(obj)				((obj)->type)
#define EC_TYPE(obj)				((obj)->type)

#define EC_INUM_ADD(obj1, obj2)		(EcMakeInt(EC_INUM(obj1) + EC_INUM(obj2)))
#define EC_INUM_SUB(obj1, obj2)		(EcMakeInt(EC_INUM(obj1) - EC_INUM(obj2)))
#define EC_INUM_AND(obj1, obj2)		(EcMakeInt(EC_INUM(obj1) & EC_INUM(obj2)))
#define EC_INUM_OR(obj1, obj2)		(EcMakeInt(EC_INUM(obj1) | EC_INUM(obj2)))
#define EC_INUM_XOR(obj1, obj2)		(EcMakeInt(EC_INUM(obj1) ^ EC_INUM(obj2)))

#define EC_INUM_LT(obj1, obj2)		(EC_INUM(obj1) < EC_INUM(obj2))
#define EC_INUM_LE(obj1, obj2)		(EC_INUM(obj1) <= EC_INUM(obj2))
#define EC_INUM_GT(obj1, obj2)		(EC_INUM(obj1) > EC_INUM(obj2))
#define EC_INUM_GE(obj1, obj2)		(EC_INUM(obj1) >= EC_INUM(obj2))
#define EC_INUM_EQ(obj1, obj2)		(EC_INUM(obj1) == EC_INUM(obj2))

#endif /* ! EC_INLINED_TYPES */

#define EC_TYPEP(obj,type)			(EC_NNULLP(obj) && (EC_TYPE(obj) == ((EcWord)(type))))
#define EC_NTYPEP(obj,type)			(EC_NULLP(obj) || (EC_TYPE(obj) != ((EcWord)(type))))
#define EC_UNDEFINEDP(obj)			(EC_TYPEP(obj,tc_undefined))
#define EC_ERRORP(obj)				(EC_TYPEP(obj,tc_error))
#define EC_BOOLP(obj)				(EC_TYPEP(obj,tc_bool))
#if ! EC_INLINED_TYPES
#define EC_INUMP(obj)				(EC_TYPEP(obj,tc_inum))
#endif
#define EC_FNUMP(obj)				(EC_TYPEP(obj,tc_fnum))
#define EC_NUMBERP(obj)				(EC_INUMP(obj) || EC_FNUMP(obj))
#define EC_CHARP(obj)				(EC_TYPEP(obj,tc_char))
#if ! EC_INLINED_TYPES
#define EC_SYMBOLP(obj)				(EC_TYPEP(obj,tc_symbol))
#endif
#define EC_STRINGP(obj)				(EC_TYPEP(obj,tc_string))
#define EC_ARRAYP(obj)				(EC_TYPEP(obj,tc_array))
#define EC_STACKP(obj)				(EC_TYPEP(obj,tc_stack))
#define EC_PRIMITIVEP(obj)			(EC_TYPEP(obj,tc_primitive))
#define EC_CMETHODP(obj)			(EC_TYPEP(obj,tc_cmethod))
#define EC_COMPILEDP(obj)			(EC_TYPEP(obj,tc_compiled))
#define EC_HANDLERP(obj)			(EC_TYPEP(obj,tc_handler))
#define EC_PACKAGEP(obj)			(EC_TYPEP(obj,tc_package))
#define EC_CLASSP(obj)				(EC_TYPEP(obj,tc_class))
#define EC_OBJECTP(obj)				(EC_TYPEP(obj,tc_object))
#define EC_USERP(obj)				(EC_NNULLP(obj) && (EC_TYPE(obj) >= tc_userbase))
#define EC_BOOLVAL(obj)				((obj)->v.boolval)
#define EC_ASBOOLVAL(obj)			(EC_BOOLP(obj) ? EC_BOOLVAL(obj) : ((EC_INUMP(obj) && EC_INUM(obj)) ? TRUE : (EcBool)EC_NNULLP(obj)))
#if ! EC_INLINED_TYPES
#define EC_INUM(obj)				((obj)->v.ival)
#endif
#define EC_FNUM(obj)				((obj)->v.fval)
#define EC_TOINUM(obj)				(EC_INUMP(obj) ? EC_INUM(obj) : ((EcInt)EC_FNUM(obj)))
#define EC_TOFNUM(obj)				(EC_FNUMP(obj) ? EC_FNUM(obj) : ((EcFloat)EC_INUM(obj)))
#define EC_CHAR(obj)				((obj)->v.cval)
#define EC_STRLEN(obj)				((obj)->v.strng.len)
#define EC_STRDATA(obj)				((obj)->v.strng.data)
#if ! EC_INLINED_TYPES
#define EC_SYMBOL(obj)				((obj)->v.symbolid)
#endif
#define EC_ARRAY(obj)				((obj)->v.array)
#define EC_ARRAYMEM(obj)			(EC_ARRAY(obj).arr)
#define EC_ARRAYLEN(obj)			(EC_ARRAY(obj).used)
#define EC_ARRAYREAL(obj)			(EC_ARRAY(obj).real)
#define EC_STACK(obj)				((obj)->v.stack)
#define EC_STACKPOINTER(obj)		(EC_STACK(obj)->sp)
#define EC_STACKBP(obj)				(EC_STACK(obj)->bp)
#define EC_STACKBASE(obj)			(EC_STACK(obj)->base)
#define EC_STACKDIM(obj)			(EC_STACK(obj)->dim)
#define EC_STACKUP(obj)				(EC_STACK(obj)->up)
#define EC_STACKLEXICAL(obj)		(EC_STACK(obj)->lexical)
#define EC_STACKPC(obj)				(EC_STACK(obj)->PC)
#define EC_STACKNARGS_REAL(obj)		(EC_STACK(obj)->nargs_real)
#define EC_STACKNARGS_LOGICAL(obj)	(EC_STACK(obj)->nargs_logical)
#define EC_STACKIMMUP(obj)			(EC_STACK(obj)->immediateUp)
#define EC_STACKIMMCALLER(obj)		(EC_STACK(obj)->immediateCaller)
#define EC_STACKIMMPC(obj)			(EC_STACK(obj)->immediatePC)
#define EC_STACKCALLED(obj)			(EC_STACK(obj)->called)

#if EC_STACK_RECYCLE
#define EC_STACKREF(obj)			(EC_STACK(obj)->ref)
#endif /* EC_STACK_RECYCLE */

#if EC_STACK_USES_STATIC
#define EC_STACKSTATIC(obj)			(EC_STACK(obj)->static_space)
#endif
#define EC_PRIMITIVESTRUCT(obj)		((obj)->v.primitive)
#define EC_PRIMITIVE(obj)			(EC_PRIMITIVESTRUCT(obj).impl)
#define EC_PRIMITIVENAME(obj)		(EC_PRIMITIVESTRUCT(obj).name)
#define EC_PRIMITIVEUSERDATA(obj)	(EC_PRIMITIVESTRUCT(obj).userdata)
#define EC_CMETHODSTRUCT(obj)		((obj)->v.cmethod)
#define EC_CMETHOD(obj)				(EC_CMETHODSTRUCT(obj).impl)
#define EC_CMETHODNAME(obj)			(EC_CMETHODSTRUCT(obj).name)
#define EC_COMPILED(obj)			((obj)->v.compiled)
#define EC_COMPILEDCODE(obj)		(EC_COMPILED(obj)->code)
#define EC_COMPILEDNCODE(obj)		(EC_COMPILED(obj)->ncode)
#define EC_COMPILEDNARG(obj)		(EC_COMPILED(obj)->nargs)
#define EC_COMPILEDNARG_DEF(obj)	(EC_COMPILED(obj)->nargs_def)
#define EC_COMPILEDVARG(obj)		(EC_COMPILED(obj)->vargs)
#define EC_COMPILEDNLOC(obj)		(EC_COMPILED(obj)->nlocals)
#define EC_COMPILEDMAXTEMPS(obj)	(EC_COMPILED(obj)->maxtemps)
#define EC_COMPILEDLEXICAL(obj)		(EC_COMPILED(obj)->lexical)
#define EC_COMPILEDLFRAME(obj)		(EC_COMPILED(obj)->lframe)
#define EC_COMPILEDHANDLER(obj)		(EC_COMPILED(obj)->handler)
#define EC_COMPILEDPACKAGE(obj)		(EC_COMPILED(obj)->package)
#define EC_COMPILEDNAME(obj)		(EC_COMPILED(obj)->name)
#define EC_COMPILEDISMETHOD(obj)	(EC_COMPILED(obj)->ismethod)
#define EC_COMPILEDINFO(obj)		(EC_COMPILED(obj)->info)
#define EC_COMPILEDDOCSTRING(obj)	(EC_COMPILED(obj)->docstring)
#define EC_COMPILEDCCALLABLE(obj)	(EC_COMPILED(obj)->ccallable) /* #JP */
#define EC_HANDLER(obj)				((obj)->v.handler)
#define EC_HANDLERTYPE(obj)			(EC_HANDLER(obj).type)
#define EC_HANDLERCODE(obj)			(EC_HANDLER(obj).code)
#define EC_PACKAGE(obj)				((obj)->v.package)
#define EC_PACKAGENAME(obj)			(EC_PACKAGE(obj)->name)
#define EC_PACKAGECODE(obj)			(EC_PACKAGE(obj)->code)
#define EC_PACKAGEFRAME(obj)		(EC_PACKAGE(obj)->frame)
#define EC_PACKAGEIMPORT(obj)		(EC_PACKAGE(obj)->import)
#define EC_PACKAGENEXPORT(obj)		(EC_PACKAGE(obj)->nexport)
#define EC_PACKAGEEXPORT(obj)		(EC_PACKAGE(obj)->export)
#define EC_PACKAGESOURCE(obj)		(EC_PACKAGE(obj)->source)
#define EC_CLASS(obj)				((obj)->v.cls)
#define EC_CLASSSUPER(obj)			(EC_CLASS(obj)->superclass)
#define EC_CLASSNMETHODS(obj)		(EC_CLASS(obj)->nmethods)
#define EC_CLASSMTABLE(obj)			(EC_CLASS(obj)->mtable)
#define EC_CLASSNCMETHODS(obj)		(EC_CLASS(obj)->ncmethods)
#define EC_CLASSCMTABLE(obj)		(EC_CLASS(obj)->cmtable)
#define EC_CLASSIOFFSET(obj)		(EC_CLASS(obj)->ioffset)
#define EC_CLASSNIVARS(obj)			(EC_CLASS(obj)->nivars)
#define EC_CLASSIVTABLE(obj)		(EC_CLASS(obj)->ivtable)
#define EC_CLASSNCVARS(obj)			(EC_CLASS(obj)->ncvars)
#define EC_CLASSCVTABLE(obj)		(EC_CLASS(obj)->cvtable)
#define EC_CLASSPACKAGE(obj)		(EC_CLASS(obj)->package)
#define EC_CLASSNAME(obj)			(EC_CLASS(obj)->name)
#define EC_CLASSSHORTNAME(obj)		(EC_CLASS(obj)->shortname)
#define EC_CLASSCODE(obj)			(EC_CLASS(obj)->code)
#define EC_CLASSLFRAME(obj)			(EC_COMPILEDLFRAME(EC_CLASSCODE(obj)))
#define EC_OBJECT(obj)				((obj)->v.object)
#define EC_OBJECTCLASS(obj)			(EC_OBJECT(obj)->ofclass)
#define EC_OBJECTIVARS(obj)			(EC_OBJECT(obj)->ivars)
#define EC_OBJECTUSER(obj)			(EC_OBJECT(obj)->user)

#define EC_USER(obj)				((obj)->v.usr.user.user)
#define EC_USER0(obj,type)			((type)((obj)->v.usr.user.user0))
#define EC_USER1(obj,type)			((type)((obj)->v.usr.user1))
#define EC_USER2(obj,type)			((type)((obj)->v.usr.user2))

#define EC_USER_SET(obj,type,val)	do { (obj)->v.usr.user.user  = (void *) (type)(val); } while (0)
#define EC_USER0_SET(obj,type,val)	do { (obj)->v.usr.user.user0 = (EcAny) (type)(val); } while (0)
#define EC_USER1_SET(obj,type,val)	do { (obj)->v.usr.user1      = (EcAny) (type)(val); } while (0)
#define EC_USER2_SET(obj,type,val)	do { (obj)->v.usr.user2      = (EcAny) (type)(val); } while (0)

#define EC_GCMASK			0x0001
/* Future use ?
#define EC_PRIM_MASK		0x03FE
#define EC_REQ_MASK			0x001E
#define EC_OPT_MASK			0x01E0
#define EC_REM_MASK			0x0200*/
#define EC_UDOBJ_MASK		0x0400								/* userdata is an elastiC object      */
#define EC_UDREGP_MASK		0x0800								/* userdata is a registered C pointer */
#define EC_CONST_MASK		0x8000

#define EC_FLAGS(obj)				((obj)->flags)

#define EC_MARKVALUE(obj)			(EC_FLAGS(obj) & EC_GCMASK)
#define EC_MARK(obj,mark)			((mark) ? (EC_FLAGS(obj) |= EC_GCMASK) : (EC_FLAGS(obj) &= (~EC_GCMASK)))
#define EC_INVERTMARK(obj)			(EC_FLAGS(obj) ^= EC_GCMASK)
#define EC_MARKEDP(obj,mark)		(EC_MARKVALUE(obj) ? (mark) : !(mark))

#define EC_SET_UDOBJ(obj)			(EC_FLAGS(obj) |= EC_UDOBJ_MASK)
#define EC_CLEAR_UDOBJ(obj)			(EC_FLAGS(obj) &= (~EC_UDOBJ_MASK))
#define EC_UDOBJ(obj, val)			((val) ? EC_SET_UDOBJ(obj) : EC_CLEAR_UDOBJ(obj))
#define EC_UDOBJP(obj)				(EC_FLAGS(obj)  & EC_UDOBJ_MASK)

#define EC_SET_UDREGP(obj)			(EC_FLAGS(obj) |= EC_UDREGP_MASK)
#define EC_CLEAR_UDREGP(obj)		(EC_FLAGS(obj) &= (~EC_UDREGP_MASK))
#define EC_UDREGP(obj, val)			((val) ? EC_SET_UDREGP(obj) : EC_CLEAR_UDREGP(obj))
#define EC_UDREGPP(obj)				(EC_FLAGS(obj)  & EC_UDREGP_MASK)

/* Future use ?
#define EC_PRIM_REQ(obj)				((EC_FLAGS(obj)  & EC_REQ_MASK) >> 1)
#define EC_PRIM_OPT(obj)				((EC_FLAGS(obj)  & EC_OPT_MASK) >> 5)
#define EC_PRIM_REM(obj)				(((EC_FLAGS(obj) & EC_REM_MASK) >> 9) & 0x01)
#define EC_PRIM_TYPE(obj)				(EC_FLAGS(obj)   & EC_PRIM_MASK)
#define EC_PRIM_MAKETYPE(req,opt,rem)	(((req & 0x0F) << 1) | ((opt & 0x0F) << 5) | ((rem & 0x01) << 9))*/

#define EC_MKCONST(obj)					(EC_FLAGS(obj) |= EC_CONST_MASK)
#define EC_CONSTP(obj)					(EC_FLAGS(obj)  & EC_CONST_MASK)

EC_END_DECLS

#endif /* __OBJECT_H */
