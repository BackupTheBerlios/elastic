/* ==========================================================================
 * gc.c
 * ==========================================================================
 * Garbage Collection.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Wed Dec  9 10:24:32 MET 1998
 *
 *   $Id: gc.c,v 1.4 2002/06/13 17:44:18 panta Exp $
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

#include "basic.h"
#include "object.h"
#include "compat.h"
#include "debug.h"

#include "strtable.h"
#include "memory.h"

#include "private.h"

static EcUInt allocated = 0;
static EcUInt marked    = 0, stackmarked = 0;
static EcUInt collected = 0;
static EcUInt freed     = 0;

#if MEM_STATS
EcInt _ec_mark_stats[EC_MAX_TYPES];
#endif

#if MEM_STATS
#define LOG_MARK(type)	do { _ec_mark_stats[(type)]++; } while (0)
#else
#define LOG_MARK(type)	do {} while (0)
#endif

/* ========================================================================
 * P R I V A T E   F U N C T I O N S    P R O T O T Y P E S
 * ======================================================================== */

static inline void   collect_obj( EC_OBJ obj );
static        void   objrelease( EC_OBJ obj );

static        void   heap_init( struct heap *heap );
static        void   heap_release( struct heap *heap );
static        EcBool heap_grow( struct heap *heap );



/* ========================================================================
 * I N T E R N A L
 * ======================================================================== */

void _ec_initializeOT( void )
{
	heap_init( &PRIVATE(heap) );

	PRIVATE(mark) = 0;
}

void _ec_releaseOT( void )
{
	struct heap_chunk *chunk;
	EC_OBJ             obj;
	EcUInt             i;
#ifdef EC_DEBUG
	EcInt              nfreed;
#endif

	/* Scan the object table */
#ifdef EC_DEBUG
	nfreed = 0;
#endif
	chunk = PRIVATE(heap).first;
	while (chunk)
	{
		obj = (EC_OBJ)(((char *)chunk) + sizeof(struct heap_chunk));
		for (i = 0; i < chunk->size; i++)
		{
			if (! EC_TYPEP(obj, tc_none))
			{
				objrelease( obj++ );
#ifdef EC_DEBUG
				nfreed++;
#endif
			} else
				obj++;
		}

		chunk = chunk->next;
	}

	heap_release( &(PRIVATE(heap)) );
#ifdef EC_DEBUG
	ASSERT( nfreed == 0 );
#endif
	PRIVATE(mark) = 0;
}


/* ========================================================================
 * P R I V A T E   F U N C T I O N S
 * ======================================================================== */

static inline void collect_obj( EC_OBJ obj )
{
	/*if (EC_NULLP(obj)) return;*/
#if EC_INLINED_TYPES
	if (EC_INLINED(obj)) return;
#endif

	collected++;
	if (EC_MARKVALUE(obj) != PRIVATE(mark))								/* EcObject has old mark */
		objrelease( obj );
}


/* ========================================================================
 * A P I
 * ======================================================================== */

EC_API EcBool EcCheckForGC( void )
{
	/* (free_top - free_base) is the number for free objects */
	if ((PRIVATE(heap).free_top - PRIVATE(heap).free_base) > PRIVATE(heap).gc_level)
		return FALSE;

	/*
	 * If we have allocated only a few objects since last GC, try
	 * to expand before GCing.
	 */
	if (allocated < EC_GC_THRESHOLD)
	{
		if (heap_grow( &(PRIVATE(heap)) ))
			return TRUE;
	}

	/* Try to GC */
	if (PRIVATE(heap).first) EcGC();

	if (PRIVATE(heap).free_top <= PRIVATE(heap).free_base)
	{
		/* GC was unsuccessful, try to expand */
		if (! heap_grow( &(PRIVATE(heap)) ))
		{
			/* OUT OF MEMORY ! */
			ASSERT( FALSE );
			return TRUE;
		}
	}

	return TRUE;
}

EC_API void EcGC( void )
{
	EcInt i;

	struct heap_chunk *chunk;
	EC_OBJ             obj;

	marked      = 0;
	stackmarked = 0;
	collected   = 0;
	freed       = 0;

	/*
	 * No GC if we are not at top-level.
	 * This because there could be objects on the C stack.
	 */
	if (PRIVATE(rt.vm_level) > 0)
		return;

#if EC_NOTIFY_GC
	/* fprintf( stderr, "[GC started...]\n" ); */
	ec_msg_printf( "[GC started...]\n" );
#endif

	/* ==================================================
	 * 1.  M A R K
	 *
	 * Mark all reachable object with current marking.
	 * ================================================== */

#if EC_NOTIFY_GC
	/* fprintf( stderr, "[MARKING]\n" ); */
	ec_msg_printf( "[MARKING]\n" );
#endif

	/* Scan the active stack(s) */
	if (EC_NNULLP(PRIVATE(rt.activeFrame))) EcMarkObject( PRIVATE(rt.activeFrame) );

	/* Scan the global stack frame */
	if (EC_NNULLP(PRIVATE(globalFrame))) EcMarkObject( PRIVATE(globalFrame) );

	stackmarked = marked;

	/* Scan globals & packages */
	if (EC_NNULLP(PRIVATE(mainTarget))) EcMarkObject( PRIVATE(mainTarget) );
	for (i = 0; i < PRIVATE(nglobals); i++)
	{
		if (EC_NNULLP(PRIVATE(global[i]))) EcMarkObject( PRIVATE(global[i]) );
	}
	for (i = 0; i < PRIVATE(npackages); i++)
	{
		if (EC_NNULLP(PRIVATE(package[i].obj))) EcMarkObject( PRIVATE(package[i].obj) );
	}

#if EC_STACK_RECYCLE
	/* Scan stacks in the pool */
	for (i = 0; i < EC_STACK_POOL_SIZE; i++)
		if (EC_NNULLP(PRIVATE(stack_pool)[i])) EcMarkObject( PRIVATE(stack_pool)[i] );
#endif

	/* ==================================================
	 * 2.  C O L L E C T
	 *
	 * Collect all objects with old mark.
	 * ================================================== */

#if EC_NOTIFY_GC
	/* fprintf( stderr, "[COLLECTING]\n" ); */
	ec_msg_printf( "[COLLECTING]\n" );
#endif

	/* Scan the object table */
	chunk = PRIVATE(heap).first;
	while (chunk)
	{
		obj = (EC_OBJ)(((char *)chunk) + sizeof(struct heap_chunk));
		for (i = 0; i < chunk->size; i++)
			if (EC_NNULLP(obj)) collect_obj( obj++ );

		chunk = chunk->next;
	}

	/* ==================================================
	 * 3.  I N V E R T   M A R K
	 * ================================================== */

	/* Invert the mark */
	PRIVATE(mark) = 1 - PRIVATE(mark);


#if EC_NOTIFY_GC
	/* fprintf( stderr,
			 "[GC finished. Allocated since last GC %ld, marked %ld (%ld through stack), collected %ld, freed %ld.]\n",
			 (long)allocated, (long)marked, (long)stackmarked, (long)collected, (long)freed ); */
	ec_msg_printf( "[GC finished. Allocated since last GC %ld, marked %ld (%ld through stack), collected %ld, freed %ld.]\n",
				   (long)allocated, (long)marked, (long)stackmarked, (long)collected, (long)freed );
#endif
	allocated   = 0;
	marked      = 0;
	stackmarked = 0;
	collected   = 0;
	freed       = 0;
}

#define CHECK_AND_ALLOC	\
do {                                                                           \
	/* Have we enough space ? */                                               \
	if (LPRIVATE(heap).free_top > LPRIVATE(heap).free_base)			/* Yes */  \
	{                                                                          \
		ASSERT( LPRIVATE(heap).free_top > LPRIVATE(heap).free_base );          \
		res = *(--LPRIVATE(heap).free_top);                                    \
		EC_MARK(res, 1 - LPRIVATE(mark));                                      \
		allocated++;                                                           \
		return res;                                                            \
	}                                                                          \
} while (0)

inline EC_API EC_OBJ EcAllocObject( void )
{
	EC_OBJ res;
	register EcPrivate *lprivate;

	lprivate = &_ec_private;

	/* Check and try */
	CHECK_AND_ALLOC;

	if (! heap_grow( &(PRIVATE(heap)) ))
		return EcMemoryError();

	/* Re-Check and try */
	CHECK_AND_ALLOC;

	return EcMemoryError();
}

EC_API void EcMarkObject( EC_OBJ obj )
{
	ASSERT( EC_NNULLP(obj) );

#if EC_INLINED_TYPES
	if (EC_INLINED(obj)) return;
#endif
	if (EC_MARKVALUE(obj) == PRIVATE(mark)) return;
	EC_MARK(obj, PRIVATE(mark));
	marked++;

	LOG_MARK( EC_TYPE(obj) );

	/* Mark sub-objects */
	switch (EC_TYPE(obj))
	{
	case tc_none:
	case tc_undefined:
	case tc_error:
	case tc_bool:
	case tc_inum:
	case tc_fnum:
	case tc_symbol:
	case tc_cmethod:
		break;

	case tc_primitive:
		if (EC_UDOBJP(obj))
			EcMarkObject(EC_PRIMITIVEUSERDATA(obj));
		break;

	default:
		if (EC_NNULLP(obj))
		{
			if (EC_USERP(obj))
			{
				/* if (USERTYPE(EC_TYPE(obj)).mark_fcn) */
				USERTYPE(EC_TYPE(obj)).mark_fcn( obj );
			}
		}
		break;
	}
}

#define QUANTUM 128

EC_API EcInt EcGCProtect( EC_OBJ obj )
{
	EcInt i;
	EcInt curdim;

	for (i = 0; i < PRIVATE(nglobals); i++)
	{
		if (PRIVATE(global)[i] == obj)
			return i;
	}

	curdim  = PRIVATE(nglobals) / QUANTUM;
	curdim *= QUANTUM;

	if (PRIVATE(nglobals) + 1 >= curdim)
	{
		EC_OBJ *newglobal;
		EcInt newdim;

		newdim = curdim + QUANTUM;
		newglobal = (EC_OBJ *) ec_realloc( PRIVATE(global), newdim * sizeof(EC_OBJ) );
		if (! newglobal)
			return -1;

		PRIVATE(global) = newglobal;
	}

	i = PRIVATE(nglobals)++;
	PRIVATE(global)[i] = obj;
	return i;
}

EC_API void EcGCUnprotect( EC_OBJ obj )
{
	EcInt i;
	EcInt at;
	EcInt oldsize;

	at = -1;
	for (i = 0; i < PRIVATE(nglobals); i++)
	{
		if (PRIVATE(global)[i] == obj)
		{
			at = i;
			break;
		}
	}

	if (at >= 0)
	{
		PRIVATE(global)[at] = EC_NIL;
	}

	oldsize = PRIVATE(nglobals);
	PRIVATE(nglobals)--;

	if (at == oldsize - 1)
	{
		EcInt curdim;
		EcInt newdim;

		curdim  = oldsize / QUANTUM;
		curdim *= QUANTUM;
		newdim = oldsize / QUANTUM;
		newdim *= QUANTUM;

		if (newdim < curdim)
		{
			EC_OBJ *newglobal;

			if (newdim) {
				newglobal = (EC_OBJ *) ec_realloc( PRIVATE(global), newdim * sizeof(EC_OBJ) );
				if (! newglobal)
					return;
				PRIVATE(global) = newglobal;
			} else
			{
				newglobal = NULL;
				PRIVATE(global) = NULL;
			}
		}
	}
}

EC_API void EcGCUnprotectAll( void )
{
	ec_free( PRIVATE(global) );
	PRIVATE(global)   = NULL;
	PRIVATE(nglobals) = 0;
}



/* ========================================================================
 * P R I V A T E   F U N C T I O N S
 * ======================================================================== */

/* ========================================================================
 * HEAP handling
 * ------------------------------------------------------------------------ */

static void objrelease( EC_OBJ obj )
{
	if (EC_NULLP(obj)) return;

#if EC_INLINED_TYPES
	if (EC_INLINED(obj)) return;
#endif

/*	ASSERT( EC_MARKVALUE(obj) != PRIVATE(mark) ); */

	switch (EC_TYPE(obj))
	{
	case tc_none:
		break;

	case tc_undefined:
		break;

	case tc_error:
		break;

	case tc_bool:
		break;

	case tc_inum:
		break;

	case tc_fnum:
		break;

	case tc_symbol:
#if ! EC_INLINED_TYPES
		EC_SYMBOL(obj) = 0;
#endif
		break;

	case tc_primitive:
		EC_PRIMITIVE(obj) = NULL;
		ec_free( (void *)EC_PRIMITIVENAME(obj) );
		EC_PRIMITIVENAME(obj) = NULL;
		EC_PRIMITIVEUSERDATA(obj) = (EcAny)NULL;
		break;

	case tc_cmethod:
		EC_CMETHOD(obj) = NULL;
		ec_free( (void *)EC_CMETHODNAME(obj) );
		EC_CMETHODNAME(obj) = NULL;
		break;

	default:
		if (EC_USERP(obj))
		{
			/* if (USERTYPE(EC_TYPE(obj)).free_fcn) */
			ASSERT( USERTYPE(EC_TYPE(obj)).free_fcn );
			USERTYPE(EC_TYPE(obj)).free_fcn( obj );
		}
		break;
	}

	if (EC_TYPE(obj) != tc_none)
	{
		*(PRIVATE(heap).free_top++) = obj;
		freed++;
	}

	EC_OBJTYPE(obj) = tc_none;
}

/* ========================================================================
 * heap_init
 *
 * Initializes a heap.
 * ------------------------------------------------------------------------ */
static void heap_init( struct heap *heap )
{
	heap->first     = NULL;

	heap->free_base = NULL;
	heap->free_top  = NULL;

	heap->n_objects          = 0;
	heap->gc_level           = EC_BASEGCLEVEL;
	heap->current_chunk_size = 0;
}

/* ========================================================================
 * heap_release
 *
 * Release heap, heap chunks and free list.
 * This doesn't release objects on the heap: this must be accomplished
 * by someone else (_ec_releaseOT).
 * ------------------------------------------------------------------------ */
static void heap_release( struct heap *heap )
{
	struct heap_chunk *chunk, *next;

	/* release chunks    */
	chunk = heap->first;
	while (chunk)
	{
		next = chunk->next;

		ec_free( chunk );
		chunk = next;
	}

	heap->first = NULL;

	/* release free list */
	ec_free( heap->free_base );
	heap->free_base = NULL;
	heap->free_top  = NULL;

	/* reset information */
	heap->n_objects          = 0;
	heap->gc_level           = EC_BASEGCLEVEL;
	heap->current_chunk_size = 0;
}

/* ========================================================================
 * heap_grow
 *
 * Expands the heap. This is performed by (unconditionally) adding a
 * heap chunk to the list and new objects to the free list.
 * ------------------------------------------------------------------------ */
static EcBool heap_grow( struct heap *heap )
{
	struct heap_chunk *chunk;

	EcUInt             c_size;
	EcInt              new_gclevel;

	EcUInt             new_nobjects;

	EC_OBJ            *new_free_base;
	EcInt              nfree_old;

	EC_OBJ             obj;
	EcUInt i;

	/*
	 * 1. Allocate another heap chunk
	 *
	 *    a. try to expand the chunk size
	 *    b. if not successful, try the base chunk size
	 *    c. if yet not successful, surrender !
	 *
	 *    Set accordingly heap gc level (low watermark for GC).
	 */
	c_size      = heap->current_chunk_size * EC_HEAPCHUNK_EXPANSIONFACTOR;
	new_gclevel = heap->gc_level * EC_HEAPCHUNK_EXPANSIONFACTOR;
	if (c_size < EC_HEAPCHUNK_BASESIZE)
		c_size = EC_HEAPCHUNK_BASESIZE;
	if (new_gclevel < EC_BASEGCLEVEL)
		new_gclevel = EC_BASEGCLEVEL;
	chunk = ec_malloc( sizeof(struct heap_chunk) + c_size * sizeof(struct _obj) );
	if (! chunk)
	{
		c_size      = EC_HEAPCHUNK_BASESIZE;
		new_gclevel = EC_BASEGCLEVEL;
		chunk = ec_malloc( sizeof(struct heap_chunk) + c_size * sizeof(struct _obj) );
	}
	if (! chunk)
		return FALSE;

	/* :TODO: do we need to clear memory in chunk ? */
	/*
	 * Answer: yes, we need to clean memory in chunk, since
	 * later on (when releasing the object table) we'll test pointers
	 * in chunk, releasing non-NULL ones.
	 */
	/* memset( ((char *)chunk) + sizeof(struct heap_chunk),
			0x00, c_size * sizeof(struct _obj) ); */
	memset( chunk, 0x00, sizeof(struct heap_chunk) + c_size * sizeof(struct _obj) );

	new_nobjects = heap->n_objects + c_size;

	/*
	 * 2. Resize free list
	 *
	 *    Free List *must* be big enough to contain *all* objects in the system
	 *    since we don't want expensive run-time checks when releasing objects.
	 */
	nfree_old = heap->free_top - heap->free_base;
	new_free_base = ec_realloc( heap->free_base, new_nobjects * sizeof(EC_OBJ) );
	if (! new_free_base)
	{
		ec_free( chunk );
		return FALSE;
	}

	chunk->next              = heap->first;						/* new chunk to the head of the list */
	chunk->size              = c_size;

	heap->first              = chunk;
	heap->free_base          = new_free_base;
	heap->free_top           = heap->free_base + nfree_old;
	heap->n_objects          = new_nobjects;
	heap->gc_level           = new_gclevel;
	heap->current_chunk_size = c_size;

	/*
	 * 3. Update free list
	 *
	 *    Added c_size new objects.
	 */
	obj = (EC_OBJ)(((char *)chunk) + sizeof(struct heap_chunk));
	for (i = 0; i < c_size; i++)
	{
		*(heap->free_top++) = obj++;
	}

	return TRUE;
}
