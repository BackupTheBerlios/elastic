/* ==========================================================================
 * utility.c
 * ==========================================================================
 * Utility functions & structures.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Tue Feb 24 10:03:50 MET 1998
 *
 *   $Id: utility.c,v 1.1 2002/05/23 21:17:42 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1997-2000 Marco Pantaleoni. All rights reserved.
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
#include "utility.h"
#include "memory.h"
#include "debug.h"

#define DELTA 10

EC_API EcBool ec_arena_create( ec_arena *arena, EcUInt initial_size )
{
	ASSERT(arena);

	arena->mem  = NULL;
	arena->last = NULL;
	arena->head = NULL;
	arena->tail = NULL;
	arena->size = 0;

	if (initial_size)
	{
		arena->mem = (EcUInt *) ec_malloc( sizeof(EcUInt) * initial_size );
		if (! arena->mem) return FALSE;
		arena->size = initial_size;
		arena->last = arena->mem + (initial_size - 1);
		arena->head = arena->mem;
		arena->tail = arena->mem;
	}

	return TRUE;
}

EC_API void ec_arena_destroy( ec_arena *arena )
{
	ASSERT(arena);

	ec_free( arena->mem );
	arena->mem  = NULL;
	arena->last = NULL;
	arena->head = NULL;
	arena->tail = NULL;
	arena->size = 0;
}

EC_API EcUInt ec_arena_get( ec_arena *arena, EcUInt at )
{
	ASSERT( at < arena->size );
	return arena->mem[at];
}

EC_API void ec_arena_set( ec_arena *arena, EcUInt at, EcUInt x )
{
	ASSERT( at < arena->size );
	arena->mem[at] = x;
}

EC_API EcBool ec_stack_empty( ec_stack *stack )
{
	ASSERT(stack);

	return (stack->tail == stack->mem);
}

EC_API EcBool ec_stack_push( ec_stack *stack, EcUInt x )
{
	ASSERT(stack);

	if (stack->tail >= stack->last)
	{
		EcUInt *new_mem;

		new_mem = (EcUInt *) ec_realloc( stack->mem, stack->size + DELTA );
		if (! new_mem) return FALSE;
		stack->last = new_mem + (stack->last - stack->mem);
		stack->head = new_mem + (stack->head - stack->mem);
		stack->tail = new_mem + (stack->tail - stack->mem);
		stack->size = stack->size + DELTA;
		stack->mem  = new_mem;
	}

	ASSERT(stack->mem);

	*(++stack->tail) = x;
	return TRUE;
}

EC_API EcUInt ec_stack_pop( ec_stack *stack )
{
	ASSERT(stack);

	ASSERT(! (stack->tail == stack->mem));

	return *(--stack->tail + 1);
}


EC_API EcBool ec_queue_empty( ec_queue *queue )
{
	ASSERT(queue);

	return (queue->head == queue->tail);
}

EC_API EcBool ec_queue_enqueue( ec_queue *queue, EcUInt x )
{
	ASSERT(queue);

	*(queue->tail) = x;
	if (queue->tail == queue->last)
		queue->tail = queue->mem;
	else
		queue->tail++;
	if (queue->tail == queue->head)
	{
		EcUInt *new_mem;
		EcUInt *src, *dst;

		new_mem = (EcUInt *) ec_realloc( queue->mem, queue->size + DELTA );
		if (! new_mem) return FALSE;

		src = queue->mem;
		dst = new_mem;
		while (src < queue->tail)
			*(dst++) = *(src++);

		dst += DELTA;
		while (src <= queue->last)
			*(dst++) = *(src++);

		queue->last = new_mem + (queue->last - queue->mem);
		queue->head = (new_mem + (queue->head - queue->mem)) + DELTA;
		queue->tail = new_mem + (queue->tail - queue->mem);
		queue->size = queue->size + DELTA;
		queue->mem  = new_mem;
	}

	return TRUE;
}

EC_API EcUInt ec_queue_dequeue( ec_queue *queue )
{
	EcUInt x;

	ASSERT(queue);

	ASSERT(! (queue->head == queue->tail));

	x = *(queue->head);
	if (queue->head == queue->last)
		queue->head = queue->mem;
	else
		queue->head++;
	return x;
}

EC_API const char *ec_string_sub( char *dst, const char *str, EcInt start, EcInt len )
{
	char       *d;
	const char *p;

	d  = dst;
	*d = '\0';

	if (! str)
		return NULL;
	p = str + start;
	while (len--)
		*d++ = *p++;
	*d = '\0';
	return dst;
}

EC_API const char *ec_string_tokenize( const char **sepp, EcInt *tokenlen, char delim )
{
	const char *tok_s;
	EcBool      last;

#define CHARPTR(pptr)		(*(pptr))
#define CHARAT(pptr)		(*CHARPTR(pptr))

	if (! sepp)
		return NULL;

	if (! CHARPTR(sepp))										/* we have finished */
		return NULL;

	tok_s         = CHARPTR(sepp);
	last          = FALSE;
	*tokenlen     = 0;

	/* search */
	if (CHARAT(sepp) == '\0')
	{
		/* empty last token */
		last = TRUE;
	} else
	{
		CHARPTR(sepp) = strchr( CHARPTR(sepp), delim );
		if (! CHARPTR(sepp))
		{
			/* last token */
			CHARPTR(sepp) = tok_s;
			while (CHARAT(sepp)) CHARPTR(sepp)++;
			last = TRUE;
		}
	}

	/* use token */
	*tokenlen = CHARPTR(sepp) - tok_s;

	if (last)
	{
		CHARPTR(sepp) = NULL;									/* indicate this is the last */
		return tok_s;
	}

	/* update for next cycle */
	CHARPTR(sepp)++;
	return tok_s;
}
