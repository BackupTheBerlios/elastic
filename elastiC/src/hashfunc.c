/* ==========================================================================
 * hashfunc.c
 * ==========================================================================
 * Hash functions.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Wed Apr 30 21:07:36 MET DST 1997
 *
 *   $Id: hashfunc.c,v 1.3 2002/05/31 13:03:31 panta Exp $
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

#include "basic.h"
#include "hashfunc.h"
#include "private.h"
#include "compat.h"

#if HAVE_LIMITS_H
#include <limits.h>
#endif

/*
 * Hashing pointers:
 *
 *   http://www.cs.yorku.ca/~oz/hash.html
 */

#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif

#define DJB_HASH(rv, buf, len) \
do {                                            \
	register const unsigned char *bufp = (buf); \
	register int     i;                         \
	register EcDWord h;                         \
                                                \
	h = 5381;                                   \
	for (i = 0; i < (len); i++)                 \
		h = (h + (h << 5)) ^ *bufp++;           \
                                                \
	(rv) = h;                                   \
} while (0)

#define DJB_HASH_R(rv, buf, len)                            \
do {                                                        \
	register const unsigned char *bufp = (buf) + (len) - 1; \
	register int     i;                                     \
	register EcDWord h;                                     \
                                                            \
	h = 5381;                                               \
	for (i = 0; i < (len); i++)                             \
		h = (h + (h << 5)) ^ *bufp--;                       \
                                                            \
	(rv) = h;                                               \
} while (0)

#define SDBM_HASH(rv, buf, len)                             \
do {                                                        \
	register const unsigned char *bufp = (buf);             \
	register int i;                                         \
	register EcDWord hash = 0;                              \
                                                            \
	for (i = 0; i < (len); i++)                             \
		hash = *bufp++ + (hash << 6) + (hash << 16) - hash; \
                                                            \
	(rv) = hash;                                            \
} while (0)

#define SDBM_HASH_R(rv, buf, len)                           \
do {                                                        \
	register const unsigned char *bufp = (buf) + (len) = 1; \
	register int i;                                         \
	register EcDWord hash = 0;                              \
                                                            \
	for (i = 0; i < (len); i++)                             \
		hash = *bufp-- + (hash << 6) + (hash << 16) - hash; \
                                                            \
	(rv) = hash;                                            \
} while (0)

static inline EcDWord djb_hash(const unsigned char *buf, size_t len)
{
	EcDWord rv;

	DJB_HASH(rv, buf, len);
	return rv;
}


static inline EcDWord djb_hash_r(const unsigned char *buf, size_t len)
{
	EcDWord rv;

	DJB_HASH_R(rv, buf, len);
	return rv;
}

static inline EcDWord sdbm_hash(const unsigned char *buf, size_t len)
{
	EcDWord rv;

	SDBM_HASH(rv, buf, len);
	return rv;
}


static inline EcDWord sdbm_hash_r(const unsigned char *buf, size_t len)
{
	EcDWord rv;

	SDBM_HASH_R(rv, buf, len);
	return rv;
}

/* ------------------------------------------------------------------------------------------
 *   HashPJW
 *
 * An adaptation of Peter Weinberger's (PJW) generic hashing algorithm based on Allen Holub's
 * version.
 * This is taken from Dr. Dobb's Journal n. 246 April 1996
 * Modified to use EcUInt.
 * ------------------------------------------------------------------------------------------
 */

#define BITS_IN_EcUInt			( sizeof(EcUInt) * CHAR_BIT )
#define THREE_QUARTERS			( (EcUInt)((BITS_IN_EcUInt * 3) / 4) )
#define ONE_EIGHTH				( (EcUInt)(BITS_IN_EcUInt / 8) )
#define HIGH_BITS				( ~((EcUInt)(~(EcUInt)0) >> ONE_EIGHTH) )

EC_API EcUInt ec_hash_pjw_string(const char *key)
{
	EcUInt hash_value, i;

	for (hash_value = 0; *key; ++key)
	{
		hash_value = ( hash_value << ONE_EIGHTH ) + *key;
		if ( (i = hash_value & HIGH_BITS) != 0 )
			hash_value =
				( hash_value ^ ( i >> THREE_QUARTERS )) & ~HIGH_BITS;
	}

	return hash_value;
}

EC_API EcUInt ec_hash_pjw_memory(const char *key, EcUInt count)
{
	EcUInt hash_value, i, j;

	for (hash_value = 0, j = 0; j < count; ++key, j++)
	{
		hash_value = ( hash_value << ONE_EIGHTH ) + *key;
		if ( (i = hash_value & HIGH_BITS) != 0 )
			hash_value =
				( hash_value ^ ( i >> THREE_QUARTERS )) & ~HIGH_BITS;
	}

	return hash_value;
}

EC_API EcUInt ec_hash_pjw_memory_r(const char *key, EcUInt count)
{
	EcUInt hash_value, i, j;
	register const char *bufp = key + count - 1;

	for (hash_value = 0, j = 0; j < count; --bufp, j++)
	{
		hash_value = ( hash_value << ONE_EIGHTH ) + *bufp;
		if ( (i = hash_value & HIGH_BITS) != 0 )
			hash_value =
				( hash_value ^ ( i >> THREE_QUARTERS )) & ~HIGH_BITS;
	}

	return hash_value;
}

EC_API EcUInt ec_hash_djb_string( const char *key )
{
	register int     c;
	register EcDWord hash;

	hash = 5381;
	while (c = *key++)
		hash = (hash + (hash << 5)) ^ c;	/* (hash * 33) ^ c */

	return hash;
}

EC_API EcUInt ec_hash_djb_memory( const char *key, EcUInt count )
{
	EcDWord hash;

	DJB_HASH(hash, key, count);
	return hash;
}

EC_API EcUInt ec_hash_djb_memory_r(const char *key, EcUInt count)
{
	EcDWord hash;

	DJB_HASH_R(hash, key, count);
	return hash;
}

EC_API EcUInt ec_hash_sdbm_string( const char *key )
{
	register int c;
	register EcDWord hash = 0;

	while (c = *key++)
		hash = c + (hash << 6) + (hash << 16) - hash;

	return hash;
}

EC_API EcUInt ec_hash_sdbm_memory( const char *key, EcUInt count )
{
	EcDWord hash;

	SDBM_HASH(hash, key, count);
	return hash;
}

EC_API EcUInt ec_hash_sdbm_memory_r(const char *key, EcUInt count)
{
	EcDWord hash;

	SDBM_HASH_R(hash, key, count);
	return hash;
}

EC_API EcUInt ec_hash_float( EcFloat key )
{
#if 0
	EcUInt ikey;

	ikey = *((EcUInt *) &key);
	return ec_hash_uint( ikey );
#endif
	EcDWord rv;
#if WORDS_BIGENDIAN
	DJB_HASH_R(rv, (const unsigned char *)&key, sizeof(EcFloat));
#else
	DJB_HASH(rv, (const unsigned char *)&key, sizeof(EcFloat));
#endif
	return (EcUInt)rv;
}

EC_API EcUInt ec_hash_two_string(const char *key)
{
	EcUInt hash_value;

	for (hash_value = 0; *key; key++)
		hash_value = (hash_value << 1) ^ *key;

	return hash_value;
}

EC_API EcUInt ec_hash_two_memory(const char *key, EcUInt count)
{
	EcUInt hash_value;

	for (hash_value = 0; count; key++, count--)
		hash_value = (hash_value << 1) ^ *key;

	return hash_value;
}

EC_API EcUInt ec_hash_two_memory_r(const char *key, EcUInt count)
{
	EcUInt hash_value;
	register const char *bufp = key + count - 1;

	for (hash_value = 0; count; bufp--, count--)
		hash_value = (hash_value << 1) ^ *bufp;

	return hash_value;
}

EC_API EcUInt ec_hash_two_float( EcFloat key )
{
#if 0
	return ec_hash_two_memory( (const char *)&key, sizeof(EcFloat) );
#endif
#if WORDS_BIGENDIAN
	return ec_hash_pjw_memory_r( (const char *)&key, sizeof(EcFloat) );
#else
	return ec_hash_pjw_memory( (const char *)&key, sizeof(EcFloat) );
#endif
}
