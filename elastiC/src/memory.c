/* ==========================================================================
 * memory.c
 * ==========================================================================
 * Safe memory handling routines.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: 28 Mar 1997
 *
 *   $Id: memory.c,v 1.3 2002/05/25 19:37:44 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1997-2002 Marco Pantaleoni. All rights reserved.
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

#include "basic.h"												/* this will also include the proper cnf.h */
#include "memory.h"
#include "compat.h"
#include "debug.h"
/* #include "cnf.h" */
#include "conf.h"


/* user has overridden with configure option */
#ifdef EC_DIRTY_MALLOC_ENABLE
#undef EC_DIRTY_MALLOC
#define EC_DIRTY_MALLOC 1
#endif


struct poolchunk												/* Head of a chunk */
{
	struct poolchunk *next;
};

struct ec_mempool
{
	struct  poolchunk *first;									/* poolchunk at the head of the list   */
	void   *p;													/* First free address in chunk or NULL */
	EcInt   fb;													/* Number of free bytes in chunk       */
	size_t  size;												/* Chunk size */
	int     alignment;											/* Alignment restriction
																   -1: use platform strictest
																    0: no alignment
																   >0: use the specified value
																*/
};


#ifdef EC_DEBUG
#if EC_DIRTY_MALLOC
static inline void memfill(void *dst, const void *pattern, size_t dst_size, size_t pattern_size)
{
	register char       *dst_p, *dst_end;
	register const char *pattern_p, *pattern_end;

	if (dst_size <= 0)
		return;
	if (pattern_size <= 0)
		return;

	dst_p     = (char *)dst;
	dst_end   = (char *)dst + dst_size - 1;
	pattern_p   = (const char *)pattern;
	pattern_end = (const char *)pattern + (pattern_size - 1);
	while (dst_p <= dst_end)
	{
		*dst_p++ = *pattern_p;
		if (pattern_p++ == pattern_end)
			pattern_p = pattern;
	}
}

static inline void memfill_dword(void *dst, EcDWord pattern, size_t dst_size)
{
	register EcDWord *dst_dw, *dst_dw_end;
	EcDWord           pat;

	pat = pattern;

	dst_dw     = (EcDWord *) dst;
	dst_dw_end = dst_dw + (dst_size / 4) - 1;
	while (dst_dw <= dst_dw_end)
		*dst_dw++ = pat;
	if (((char *)dst_dw_end) < ((char *)dst) + dst_size)
	{
		char *rem_s = (char *) dst_dw;							/* == (char *) (dst_dw_end + 1) */

		memfill( rem_s, &pat, ((char *)dst + dst_size) - rem_s, sizeof(EcDWord) );
	}
}
#endif
#endif

/* ---------------------------------------------------------------------------------------------------
 * ec_malloc
 *
 *   Allocates a block of memory checking for error conditions & initializing the memory.
 *
 * Input:
 *     NAME         DESCRIPTION
 *     size         - Size in bytes of memory block to allocate.
 *
 * Output:
 *     Pointer to newly allocated memory or 0 in case of failure.
 *
 * Assumptions:
 *     (size > 0)
 * --------------------------------------------------------------------------------------------------- */
EC_API void *ec_malloc(size_t size)
{
	void *mem;

	ASSERT(size > 0);

	if (size <= 0) return 0;

	mem = malloc(size);
	if (!mem)
	{
		errno = ENOMEM;
		return 0;
	}

#if EC_PARANOID_CODE
	memset(mem, 0, size);
#endif

#ifdef EC_DEBUG
#if EC_DIRTY_MALLOC
	/*
	{
		EcDWord pat = EC_DIRTY_PATTERN;
		memfill(mem, &pat, size, sizeof(EcDWord));
		}*/
	memfill_dword(mem, EC_DIRTY_PATTERN, size);
#endif
#endif

	return mem;
}

/* ---------------------------------------------------------------------------------------------------
 * ec_free
 *
 *   Safely frees a block allocated with ec_malloc.
 *
 * Input:
 *     NAME         DESCRIPTION
 *     ptr          - Pointer to the memory block.
 *
 * Output:
 *     --
 *
 * Assumptions:
 *     --
 * --------------------------------------------------------------------------------------------------- */
EC_API void ec_free(void *ptr)
{
	if (!ptr) return;

	/*
	 * :TODO: when EC_DIRTY_MALLOC, fill with dirty pattern.
	 * To do so, we need to store the size of the block in a header.
	 */

	free(ptr);
}

/* ---------------------------------------------------------------------------------------------------
 * ec_realloc
 *
 *   Reallocates a memory block, previously allocated with ec_malloc, changing its dimension.
 *   If size == 0 free the block. If ptr == 0, allocates the block.
 *
 * Input:
 *     NAME         DESCRIPTION
 *     ptr          - Pointer to the memory block.
 *     size         - New size in bytes.
 *
 * Output:
 *     Pointer to the new block or 0 in case of failure.
 *
 * Assumptions:
 *     (size >= 0)
 *     (size != 0) || ptr
 * --------------------------------------------------------------------------------------------------- */
EC_API void *ec_realloc(void *ptr, size_t size)
{
	char *memnew;

	ASSERT(size >= 0);
	ASSERT((size != 0) || ptr);

	if (!ptr) return ec_malloc(size);
	if (size == 0)
	{
		ec_free(ptr);
		return 0;
	}

	memnew = realloc(ptr, size);
	return memnew;
}

EC_API void *ec_memmove(void *dest, const void *src, size_t n)
{
	char       *dst_p = dest;
	const char *src_p = src;

	if (((const char *)src + n < (char *)dest) ||
		((char *)dest + n < (const char *)src))
    {
		memcpy( dest, src, n );
		return dest;
    }
	else if ((char *)dest <= (const char *)src)
    {
		while (n--)
			*(dst_p++) = *(src_p++);
    }
	else
    {
		dst_p += n;
		src_p += n;
		while (n--)
			*(--dst_p) = *(--src_p);
    }

	return dest;
}


EC_API void *ec_allocblock( size_t nmemb, size_t size )
{
	return ec_malloc( nmemb * size );
}

EC_API EcBool ec_resizeblock( void *blockp, size_t nmemb, size_t size )
{
	void *newblock;

	ASSERT( blockp );

	newblock = ec_realloc( *((void **)blockp), nmemb * size );
	if ((! newblock) && (size > 0))
		return FALSE;
	else
	{
		*((void **)blockp) = newblock;
		return TRUE;
	}
}

EC_API void ec_freeblock( void *block )
{
	ec_free( block );
}


EC_API ec_mempool ec_allocpool( size_t size, int alignment )
{
	ec_mempool res;

	res = (ec_mempool) ec_malloc( sizeof(struct ec_mempool) );
	res->first = NULL;
	res->p     = NULL;
	res->fb    = 0;
	res->size  = size;
	res->alignment = alignment;

	if (res->alignment < 0)
		res->alignment = EC_PLATFORM_ALIGNMENT;

	/* Adjust size according to alignment restrictions */
	if (res->alignment &&
		(res->size % res->alignment))
		res->size += res->alignment - (size % res->alignment);

	return (void *) res;
}

EC_API void ec_freepool( ec_mempool pool )
{
	struct poolchunk *chunk, *next;

	if (!pool) return;
	ASSERT( pool );

	chunk = pool->first;

	while (chunk)
	{
		next = chunk->next;
		ec_free( chunk );
		chunk = next;
	}

	pool->first = NULL;
	pool->p     = NULL;
	pool->fb    = 0;
	pool->size  = 0;
	ec_free( pool );
}

EC_API void *ec_palloc( ec_mempool pool, size_t size )
{
	struct  poolchunk *chunk;
	void   *res;
	EcInt   chunk_size, alignment_extra;

	ASSERT( pool );
	ASSERT( size > 0 );

	/* Adjust size according to alignment restrictions */
	if (pool->alignment &&
		(size % pool->alignment))
		size += pool->alignment - (size % pool->alignment);

	if (pool->p && (size <= (size_t)pool->fb))
	{
		res = pool->p;
		ASSERT( pool->alignment ? ((unsigned long)res % pool->alignment) == 0 : TRUE );
		/* ((char *)(pool->p)) += size; */
		pool->p = (void *) (((char *)(pool->p)) + size);
		pool->fb -= size;

		ASSERT( pool->alignment ? ((unsigned long)pool->p % pool->alignment) == 0 : TRUE );

		ASSERT( pool->fb >= 0 );
		if (pool->fb == 0) pool->p = NULL;

		return res;
	}

	/* Add a chunk */
	if (size < pool->size)
		chunk_size = (EcInt) pool->size;
	else
	{
		chunk_size  = (EcInt) ((size + pool->size) / pool->size);
		chunk_size *= pool->size;
	}

	alignment_extra = 0;
	if (pool->alignment > 0)
		alignment_extra = pool->alignment - (sizeof(struct poolchunk) % pool->alignment);
	chunk = (struct poolchunk *) ec_malloc( sizeof(struct poolchunk) + alignment_extra + chunk_size );
	if (! chunk) return NULL;

	res = ((char *)chunk) + sizeof(struct poolchunk);
	if (pool->alignment &&
		((unsigned long)res % pool->alignment))
		res = (void *) ((unsigned long)res + (pool->alignment - ((unsigned long)res % pool->alignment)));
	ASSERT( pool->alignment ? ((unsigned long)res % pool->alignment) == 0 : TRUE );

	chunk->next = pool->first;
	pool->first = chunk;
	pool->p     = ((char *)res) + size;
	pool->fb    = chunk_size - size;

	ASSERT( pool->alignment ? ((unsigned long)pool->p % pool->alignment) == 0 : TRUE );

	ASSERT( pool->fb >= 0 );
	if (pool->fb == 0) pool->p = NULL;

	return res;
}

EC_API char *ec_stringdup( const char *string )
{
	char *res;

	res = (char *) ec_malloc( string ? strlen( string ) + 1 : 1 );
	ASSERT( res );
	if (! res) return NULL;
	strcpy( res, string ? string : "" );

	return res;
}

EC_API char *ec_stringduppool( ec_mempool pool, const char *string )
{
	char *res;

	res = (char *) ec_palloc( pool, string ? strlen( string ) + 1 : 1 );
	if (! res) return NULL;
	strcpy( res, string ? string : "" );

	return res;
}
