/* ==========================================================================
 * private.h
 * ==========================================================================
 * Private things. Stay away.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Mon May 25 16:50:11 MEST 1998
 *
 *   $Id: private.h,v 1.2 2002/05/25 19:37:44 panta Exp $
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

#ifndef __PRIVATE_H
#define __PRIVATE_H

/* #include <elastic/cnf.h> */
#include <elastic/basic.h>										/* this will also include the proper cnf.h */
#include <elastic/conf.h>
#include <elastic/object.h>
#include <elastic/elastic.h>
#include <elastic/memory.h>
#include <elastic/strtable.h>
#include <elastic/compile.h>
#include <elastic/ast.h>
#include <elastic/bitstring.h>


EC_BEGIN_DECLS

typedef EC_OBJ (*package_init_fcn)( void );
typedef void   (*package_cleanup_fcn)( void );

/* Private Stuff */

#define EC_HEAPCHUNK_BASESIZE        8192L
#define EC_HEAPCHUNK_EXPANSIONFACTOR 2L
#define EC_BASEGCLEVEL               64L
#define EC_GC_THRESHOLD              1024L

#define CODE_SUFFIX "_code"

#define DEFAULT_PACKAGENAME "default"

/*
 * In a future, when multiple threads of execution will be there,
 * this structure will contain a Program Counter.
 * Also this structure will be contained in multiple copies (one
 * per thread) in EcPrivateStruct.
 */
struct runtime
{
	EC_OBJ activeFrame;											/* Active Stack frame                  */
	EC_OBJ exc;													/* Exception object                    */
	EcInt  vm_level;											/* Nesting level of execution          */
	EC_OBJ compiled;											/* Current compiled object             */
	EcInt  line_num;											/* Current line number (last executed) */
	EcInt  exc_line_num;										/* Exception line number               */
};

/* ===========================================================================================
 * HEAP
 *
 * The heap is composed by a chain of memory blocks (chunks), from
 * which objects are extracted for allocation, and a free object table.
 * All these structures are expandable.
 * ------------------------------------------------------------------------------------------- */
struct heap
{
	/* --------------------------------------
	 * List of chunks
	 * -------------------------------------- */
	struct heap_chunk *first;

	/* --------------------------------------
	 * Free List
	 * -------------------------------------- */
	EC_OBJ           *free_base;								/* Array of free objects               */
	EC_OBJ           *free_top;									/* Pointer to the top of the free list */

	/* --------------------------------------
	 * Misc information
	 * -------------------------------------- */
	EcInt             n_objects;								/* Number of objects in the system */

	EcInt             gc_level;									/* Minimum number of free objects to cause GC */

	EcUInt            current_chunk_size;						/* Last chunk dimension  */
};

/* ===========================================================================================
 * HEAP CHUNK
 * ------------------------------------------------------------------------------------------- */
struct heap_chunk												/* Head of a chunk */
{
	struct heap_chunk *next;									/* next chunk in the list */
	EcUInt             size;									/* size of this chunk     */
};

struct package
{
	ec_string           *name;									/* Full package name */
	EC_OBJ               obj;									/* Package object    */
	ec_dlhandle         *dlhandle;
	package_init_fcn     dlinit_fcn;
	package_cleanup_fcn  dlcleanup_fcn;
};

typedef void *EcCPointer;

typedef struct objectmap_struct *objectmap;

struct EcPrivateStruct
{
#if 0
	EcInt   argc;
	char  **argv;
#endif

	/* ==========================================================================================
	 * C O M P I L A T I O N
	 * ========================================================================================== */

	/* Compilation flags */
	EcBool debug_enable;

	/* Scanning & Parsing */
	char *sourceOrigin;											/* Category of source file */
	char *fileSource;											/* Source file */
	char *fileOutput;											/* Desired output filename or NULL */
	EcInt startLine, startColumn;								/* Token start */
	EcInt endLine,   endColumn;									/* Token end */

	EcInt line, column, charnum;								/* scanner current position */
	EcInt line_start, col_start, charnum_start;					/* scanner start position   */

	ASTNode parse_result;										/* the ast produced by parsing */

	/* Symbol table */
	ec_strtable  *symTable;
	EcUInt       currentId;
	char       **symName;
	EcInt        symNameSize;

	/* AST & scopes */
	ec_mempool nodePool;

	/*
	 * Global & current scope
	 *
	 * Current scope is the current compile-time environment, so through it we can
	 * reach the current target bytecode.
	 * Global scope determines the main bytecode.
	 * We also maintain a reference to main bytecode object, to prevent its garbage collection.
	 */
	Scope globalScope, currentScope;

	EC_OBJ mainTarget;

	/* Flags to determine:
	 *  a. if after compilation we must execute the resulting code
	 *  b. if during compilation we must execute imported package
	 */
	EcBool execute;
	EcBool executeImported;

	/* ==========================================================================================
	 * E X E C U T I O N
	 * ========================================================================================== */

	EC_OBJ         globalFrame;									/* Main stack frame           */
	struct runtime rt;											/* Data necessary at run time */

	/* ==========================================================================================
	 * O B J E C T   T A B L E
	 * ========================================================================================== */

	struct heap heap;											/* Object heap                */

	/* For Garbage Collection */
	int mark;													/* (Current) Marking flag     */

	/* Global objects */
	EC_OBJ   *global;
	EcInt     nglobals;
	EcInt     ncoreglobals;										/* # of globals defined in core */

	/* Stack pool */
#if EC_STACK_RECYCLE
	EC_OBJ      stack_pool[EC_STACK_POOL_SIZE];					/* stack pool            */
	ec_bitstr_t ec_bit_decl(stack_pool_bmap, EC_STACK_POOL_SIZE);	/* bitmap for stack pool */

#if EC_STACK_RECYCLE_STATS
	EcInt n_makestack;
	EcInt n_recycle_put_attempts;
	EcInt n_recycle_put;
	EcInt n_recycle_get_attempts;
	EcInt n_recycle_get;

	EcInt n_pool_empty;
	EcInt n_pool_full;
	EcInt pool_max_fill;

	EcInt n_ref_inc;
	EcInt n_ref_dec;
#endif /* EC_STACK_RECYCLE_STATS */
#endif /* EC_STACK_RECYCLE */

	EcBool in_cleanup;											/* True if final cleanup is ongoing */

	/* ==========================================================================================
	 * U S E R   T Y P E S
	 * ========================================================================================== */

	EcInt       usertypes;
	EcTypespec *typespec;

	/* ==========================================================================================
	 * M I S C
	 * ========================================================================================== */

	/* Package list (used both at compile-time and run-time) */
	EcInt           npackages;
	struct package *package;

	/* Patch map (used on package loading) */
	objectmap patchmap;

	/* Registered C pointers (C primitive functions, method & data) */
	/*
	 * XXXX TODO XXXX
	 * Please note that this system is INADEQUATE !
	 * The saved objects work only on the same version of the tools that saved them.
	 * We'll need a full table with (unique) pointer names:
	 *   cpointer -> name
	 *   name     -> cpointer
	 */
	EcInt       ncpointers;
	EcCPointer *cpointer;

	ec_hash builtin_name;										/* registered builtin -> name */

	/*
	 * Symbols
	 */
	/* Variables */
	EcUInt selfID;
	EcUInt at_classID;

	/* Methods */
	EcUInt initID;
	EcUInt newID;
	EcUInt copyID;
	EcUInt compareID;
	EcUInt hashID;
	EcUInt getitemID, setitemID, delitemID, lengthID;
	EcUInt gcmarkID, gcfreeID;
	EcUInt printID;
	EcUInt doesNotUnderstandID;
	EcUInt promoteID;
	EcUInt addID,    raddID;
	EcUInt subID,    rsubID;
	EcUInt mulID,    rmulID;
	EcUInt divID,    rdivID;
	EcUInt modID,    rmodID;
	EcUInt powID,    rpowID;
	EcUInt lshiftID, rlshiftID;
	EcUInt rshiftID, rrshiftID;
	EcUInt andID,    randID;
	EcUInt orID,     rorID;
	EcUInt xorID,    rxorID;
	EcUInt negID;
	EcUInt posID;
	EcUInt absID;
	EcUInt invertID;
	EcUInt intID;
	EcUInt floatID;
	EcUInt trueID;
	EcUInt cmpID,    rcmpID;
	EcUInt lineID;												/* debugging info in bytecode */


	/* Error */
};

extern EcPrivate _ec_private;

/* ========================================================================
 * M A C R O S
 * ======================================================================== */

#define PRIVATE(E) (_ec_private.E)
#define LPRIVATE(E) (lprivate->E)

#define USERTYPE(code)	(PRIVATE(typespec)[(code)])

/* Qualified symbols */

#define QSLEN(qs)		((qs)->n)
#define QSCOMP(qs, i)	((qs)->sym[(i)])


#ifdef EC_DEBUG
#define EC_CHECK(obj)	do { EcCheck( obj ); } while (0)
#define EC_CHECKALL()	do { EcCheckAll(); } while (0)
#else
#define EC_CHECK(obj)	do { } while (0)
#define EC_CHECKALL()	do { } while (0)
#endif

#define MEM_STATS 0

#if MEM_STATS
#define EC_MAX_TYPES 256
extern EcInt _ec_mark_stats[EC_MAX_TYPES];
extern EcInt _ec_make_stats[EC_MAX_TYPES];
#endif

/* User types */

#define TYPESPEC(obj)			(USERTYPE(EC_TYPE((obj))))
#define CB(obj)					(TYPESPEC(obj))
#define CB_SEQUENCE(obj)		(TYPESPEC(obj).sequence_cb)
#define CB_NUMERIC(obj)			(TYPESPEC(obj).numeric_cb)

#define IS_USER_SEQUENCE(obj)	(EC_USERP(obj) && CB_SEQUENCE(obj))
#define IS_USER_NUMERIC(obj)	(EC_USERP(obj) && CB_NUMERIC(obj))


/* ========================================================================
 * P R I V A T E   F U N C T I O N S
 * ======================================================================== */

/* Package loading */
EcBool    _ec_packageio_init( void );
void      _ec_packageio_cleanup( void );
EC_OBJ    _ec_package_loaded( const char *name );
EcInt     _ec_package_position( const char *name );
EcInt     _ec_package_position_o( EC_OBJ package );

EcBool _ec_package_add_public( EC_OBJ package,
							   EcUInt sym, EcInt pos, EcBool isconst );


/* C pointers */
EcInt      ec_register_cpointer( EcCPointer pntr );
EcInt      ec_cpointer2index( EcCPointer pntr );
EcCPointer ec_index2cpointer( EcInt index );
EcBool     ec_pointer_registered( EcCPointer pntr );

/* Basic library */
EcBool    _ec_lib_init( void );
void      _ec_lib_cleanup( void );

/* Builtin data-types, classes, functions */
EcBool    _ec_register_builtin( void );
void      _ec_cleanup_builtin( void );

EcBool    _ec_array_init( void );
void      _ec_array_cleanup( void );
EcBool    _ec_char_init( void );
void      _ec_char_cleanup( void );
EcBool    _ec_string_init( void );
void      _ec_string_cleanup( void );
EcBool    _ec_stack_init( void );
void      _ec_stack_cleanup( void );
EcBool    _ec_compiled_init( void );
void      _ec_compiled_cleanup( void );
EcBool    _ec_handler_init( void );
void      _ec_handler_cleanup( void );
EcBool    _ec_package_init( void );
void      _ec_package_cleanup( void );
EcBool    _ec_class_init( void );
void      _ec_class_cleanup( void );
EcBool    _ec_object_init( void );
void      _ec_object_cleanup( void );
EcBool    _ec_hash_init( void );
void      _ec_hash_cleanup( void );
EcBool    _ec_list_init( void );
void      _ec_list_cleanup( void );

/* File module */
EcBool    _ec_file_init( void );
void      _ec_file_cleanup( void );

/* String module */
EcBool    _ec_modstring_init( void );
void      _ec_modstring_cleanup( void );

/* Array module */
EcBool    _ec_modarray_init( void );
void      _ec_modarray_cleanup( void );

/* Misc */
#define MAX_HASH_RECUR 2
EcUInt _ec_hash( EC_OBJ obj, EcInt recur );

EC_END_DECLS

#endif /* __PRIVATE_H */
