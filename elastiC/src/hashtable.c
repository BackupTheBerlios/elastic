/* ==========================================================================
 * hashtable.c
 * ==========================================================================
 * Generic hash table.h
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Thu Jun 24 10:34:09 MEST 1999
 *
 *   $Id: hashtable.c,v 1.3 2002/06/13 17:44:18 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1999-2002 Marco Pantaleoni. All rights reserved.
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
#include "debug.h"

#include "elastic.h"
#include "compat.h"

#include "hashfunc.h"
#include "primes.h"

#include "hashtable.h"

#ifdef EC_DEBUG
#define CHECK_PRIME		0
#endif

#ifdef EC_DEBUG
static EcBool debTable = FALSE;
/*static EcBool debTable = TRUE;*/
#endif

struct ec_hash_struct
{
	ec_hash_def def;											/* Operations                              */
	EcUInt entries;
	EcUInt capacity;
	EcUInt prime_index;											/* The index of the size in primes table   */

	EcUInt min_size;												/* Approximate minimum size                */
	EcUInt loLoad;
	EcUInt hiLoad;

	struct ec_hash_entry_struct *entry;							/* Array of hash table entries             */
};

struct ec_hash_iterator_struct
{
	ec_hash hash;
	EcUInt  next;
};

#define H_DEF(h)				((h)->def)
#define H_INVKEY(h)				(H_DEF(h).invalid_key)
#define H_INVVAL(h)				(H_DEF(h).invalid_value)
#define H_HASH(h,k)				(H_DEF(h).key_hash((k)))
#define H_KEYCOPY(h,o)			(H_DEF(h).key_copy((o)))
#define H_KEYCMP(h,o1,o2)		(H_DEF(h).key_compare((o1),(o2)))
#define H_KEYDESTROY(h,o)		(H_DEF(h).key_destroy((o)))
#define H_VALCOPY(h,o)			(H_DEF(h).value_copy((o)))
#define H_VALCMP(h,o1,o2)		(H_DEF(h).value_compare((o1),(o2)))
#define H_VALDESTROY(h,o)		(H_DEF(h).value_destroy((o)))

#define H_ENTRIES(h)			((h)->entries)
#define H_CAPACITY(h)			((h)->capacity)
#define H_PRIMEINDEX(h)			((h)->prime_index)
#define H_MINSIZE(h)			((h)->min_size)
#define H_LOLOAD(h)				((h)->loLoad)
#define H_HILOAD(h)				((h)->hiLoad)
#define H_ENTRYBASE(h)			((h)->entry)
#define H_ENTRY(h,i)			(&(H_ENTRYBASE(h)[i]))

#define FIRSTPRIME 3											/* Index of the first prime to use (3 -> prime 29) */

/* LOAD FACTOR
   ===========

   Load factor is the ratio between used slots and total slots in hash table.
   Load factor (lf) is a rational number between 0 and 1: 0 <= lf <= 1.
   With a good hashing function, it's the average number of elements with the same key.
   So better results are achieved with a low load factor.

   We express load factors multiplied by (1 << SHIFTAMOUNT).
   We define:

     slf = (table->entries << SHIFTAMOUNT) / table->capacity;

   Expansion occurs when (slf >= HILOAD), and the table is expanded enough to obtain (slf <= LOLOAD).
   For optimal results, maximum load factor should not exceed 0.75 (0.75 * 256 == 192)                 */

/* Default values for expansion/contraction */
#define SHIFTAMOUNT  8											/* HILOAD, LOLOAD must be < (1 << SHIFTAMOUNT)     */

#define LOLOAD    100L											/* When expansion occurs, must be lf <= LOLOAD     */
#define HILOAD    192L											/* Maximum load factor accepted                    */


static ec_hash       hash_alloc( void );
static void          hash_free( ec_hash table );
static EcBool        hash_init( ec_hash table, EcUInt min_size, EcUInt loLoad, EcUInt hiLoad, ec_hash_def *def );
static EcBool        hash_cleanup( ec_hash table );
static EcBool        hash_expand( ec_hash table );
static ec_hash_entry find( ec_hash table, EcAny key );
static ec_hash_entry find_or_insert( ec_hash table, EcAny key );

#ifdef EC_DEBUG
#if CHECK_PRIME
static EcBool isprime( EcUInt i );
#endif
static EcBool verify( ec_hash table, EcAny key, EcAny value );
#endif


/* ========================================================================
 * A P I
 * ======================================================================== */

EC_API ec_hash ec_hash_create( EcUInt min_size, EcUInt loLoad, EcUInt hiLoad, ec_hash_def *def )
{
	ec_hash table;

	table = hash_alloc();
	if (table)
	{
		if (! hash_init( table, min_size, loLoad, hiLoad, def ))
		{
			hash_free( table );
			return NULL;
		}
	}

	return table;
}

EC_API void ec_hash_destroy( ec_hash table )
{
	if (table)
	{
		hash_cleanup( table );
		hash_free( table );
	}
}

EC_API EcBool ec_hash_get_entry( ec_hash table, EcAny key, ec_hash_entry *entry )
{
	ec_hash_entry res;

	res = find( table, key );
	if (entry) *entry = res;

	return (res != NULL);
}

#if 0
EC_API EcBool ec_hash_get( ec_hash table, EcAny key, EcAny *value )
{
	ec_hash_entry entry;

	entry = find( table, key );
	if (entry)
	{
		if (value) *value = EC_HASH_ENTRY_VALUE(entry);
		return TRUE;
	}
	if (value)
		*value = H_INVVAL(table);
	return FALSE;
}
#endif

EC_API EcBool ec_hash_get( ec_hash table, EcAny key, EcAny *value )
{
	EcInt  hash, hash2;
	EcUInt M;
	EcAny  invkey;
	EcAny  entrykey;

	ec_hash_entry entry = NULL;

 start:
	ASSERT(table);

	if (H_ENTRIES(table) == 0)
	{
		if (value)
			*value = H_INVVAL(table);
		return FALSE;
	}

	invkey = H_INVKEY(table);

	M = H_CAPACITY(table);

	/*
	 * Double hashing
	 */
	ASSERT( M > 2 );
#if CHECK_PRIME
	ASSERT( isprime(M) );
#endif

	/* D1. First hash. */
	hash = H_HASH(table, key) % M;
	ASSERT( (hash >= 0) && (hash <= (M - 1)) );

	/* D2. First probe. */
	entry    = H_ENTRY(table, hash);
	entrykey = EC_HASH_ENTRY_KEY(entry);
	if (entrykey != invkey)
	{
		if (H_DEF(table).key_compare)
		{
			if (H_KEYCMP(table, key, entrykey) == 0)
			{
				if (value) *value = EC_HASH_ENTRY_VALUE(entry);
				return TRUE;
			}
		} else
		{
			if (key == entrykey)
			{
				if (value) *value = EC_HASH_ENTRY_VALUE(entry);
				return TRUE;
			}
		}
	} else
		goto D6;												/* empty here */

	/* D3. Second hash. */
	hash2 = 1 + (H_HASH(table, key) % (M - 2));
	ASSERT( (hash2 >= 1) && (hash2 <= (M - 1)) );


	if (! H_DEF(table).key_compare)
	{
		while (TRUE)
		{
			/* D4. Advance to next. */
			hash = hash - hash2;
			if (hash < 0)
				hash = hash + H_CAPACITY(table);
			hash %= H_CAPACITY(table);

			/* D5. Compare. */
			entry    = H_ENTRY(table, hash);
			entrykey = EC_HASH_ENTRY_KEY(entry);
			if (entrykey != invkey)
			{
				if (key == entrykey)
				{
					if (value) *value = EC_HASH_ENTRY_VALUE(entry);
					return TRUE;
				}
			} else
				goto D6;										/* empty here */
		}
	} else
	{
		while (TRUE)
		{
			/* D4. Advance to next. */
			hash = hash - hash2;
			if (hash < 0)
				hash = hash + H_CAPACITY(table);
			hash %= H_CAPACITY(table);

			/* D5. Compare. */
			entry    = H_ENTRY(table, hash);
			entrykey = EC_HASH_ENTRY_KEY(entry);
			if (entrykey != invkey)
			{
				if (H_KEYCMP(table, key, entrykey) == 0)
				{
					if (value) *value = EC_HASH_ENTRY_VALUE(entry);
					return TRUE;
				}
			} else
				goto D6;										/* empty here */
		}
	}

	/* D6. */
 D6:
	if (H_ENTRIES(table) == M - 1)
	{
		if (! hash_expand( table ))
		{
			if (value)
				*value = H_INVVAL(table);
			return FALSE;
		}
		goto start;
	}

	if (value)
		*value = H_INVVAL(table);
	return FALSE;
}

/*
 * hash_add
 *     -- used to add elements to a new hash table during expansion
 *        For this reason, we don't have to call copy methods
 *        for (key, value) pairs.
 *        We also don't need to handle expansion.
 */
static EcBool hash_add( ec_hash table, ec_hash_entry oldentry )
{
	ec_hash_entry entry;
	EcAny invkey;

	ASSERT(table);
	ASSERT( H_CAPACITY(table) > 1 );

	invkey = H_INVKEY(table);

	entry = find_or_insert( table, EC_HASH_ENTRY_KEY(oldentry) );
	if (! entry) return FALSE;
	ASSERT( EC_HASH_ENTRY_KEY(entry) == invkey );

	EC_HASH_ENTRY_KEY(entry)   = EC_HASH_ENTRY_KEY(oldentry);
	EC_HASH_ENTRY_VALUE(entry) = EC_HASH_ENTRY_VALUE(oldentry);

	H_ENTRIES(table)++;

	/* In case we exceeded the load factor, expand the table */
	ASSERT(H_CAPACITY(table));
	ASSERT(! (((H_ENTRIES(table) << SHIFTAMOUNT) / H_CAPACITY(table)) >= H_HILOAD(table)) );

	ASSERT( verify( table, EC_HASH_ENTRY_KEY(entry), EC_HASH_ENTRY_VALUE(entry) ) );
	return TRUE;
}

EC_API EcBool ec_hash_set( ec_hash table, EcAny key, EcAny value )
{
	ec_hash_entry entry;
	EcAny invkey;

	ASSERT(table);
	ASSERT( H_CAPACITY(table) > 1 );

	invkey = H_INVKEY(table);

	entry = find_or_insert( table, key );
	if (! entry) return FALSE;
	if (EC_HASH_ENTRY_KEY(entry) == invkey)
	{
		/* fresh slot */

		EC_HASH_ENTRY_KEY(entry)   = H_DEF(table).key_copy   ? H_KEYCOPY(table, key)   : key;
		EC_HASH_ENTRY_VALUE(entry) = H_DEF(table).value_copy ? H_VALCOPY(table, value) : value;

		H_ENTRIES(table)++;

		/* In case we exceeded the load factor, expand the table */
		ASSERT(H_CAPACITY(table));
		if (((H_ENTRIES(table) << SHIFTAMOUNT) / H_CAPACITY(table)) >= H_HILOAD(table))
			hash_expand( table );

		ASSERT( verify( table, key, value ) );
		return TRUE;
	}
	else
	{
		/* already occupied slot */

		if (H_DEF(table).value_destroy && (EC_HASH_ENTRY_VALUE(entry) != H_INVVAL(table)))
			H_VALDESTROY(table, EC_HASH_ENTRY_VALUE(entry));
		EC_HASH_ENTRY_VALUE(entry) = H_DEF(table).value_copy ? H_VALCOPY(table, value) : value;

		ASSERT( verify( table, key, value ) );
		return TRUE;
	}

	ASSERT(0);													/* We couldn't get here */
	return FALSE;
}

EC_API EcBool ec_hash_del( ec_hash hash, EcAny key )
{
	/* TODO: To be implemented ... */
	ASSERT( FALSE );
	return FALSE;
}

EC_API EcBool ec_hash_has_key( ec_hash table, EcAny key )
{
	EcUInt i;
	ec_hash_entry entry;

	if (H_DEF(table).key_compare)
	{
		for (i = 0; i < H_CAPACITY(table); i++)
		{
			entry = H_ENTRY(table, i);
			if (entry)
			{
				if (EC_HASH_ENTRY_KEY(entry) != H_INVKEY(table))
				{
					if (H_DEF(table).key_compare)
					{
						if (H_KEYCMP(table, EC_HASH_ENTRY_KEY(entry), key) == 0)
							return TRUE;
					} else
					{
						if (EC_HASH_ENTRY_KEY(entry) == key)
							return TRUE;
					}
				}
			}
		}
	}
	else
	{
		for (i = 0; i < H_CAPACITY(table); i++)
		{
			entry = H_ENTRY(table, i);
			if (entry)
			{
				if ((EC_HASH_ENTRY_KEY(entry) != H_INVKEY(table)) &&
					(EC_HASH_ENTRY_KEY(entry) == key))
						return TRUE;
			}
		}
	}

	return FALSE;
}

EC_API EcInt ec_hash_size( ec_hash table )
{
	return H_ENTRIES(table);
}

EC_API ec_hash_iterator ec_hash_iterator_create( ec_hash table )
{
	ec_hash_iterator iterator;

	iterator = (ec_hash_iterator) ec_malloc( sizeof(struct ec_hash_iterator_struct) );
	if (! iterator) return NULL;

	iterator->hash = table;
	iterator->next = 0;

	return iterator;
}

EC_API void ec_hash_iterator_destroy( ec_hash_iterator iterator )
{
	iterator->hash = NULL;
	iterator->next = 0;
	ec_free( iterator );
}

EC_API void ec_hash_iterator_first( ec_hash_iterator iterator )
{
	iterator->next = 0;
}

EC_API EcBool ec_hash_iterator_next( ec_hash_iterator iterator, EcAny *key )
{
	ec_hash table;

	table = iterator->hash;
	while (iterator->next < H_CAPACITY(table))
	{
		if (EC_HASH_ENTRY_KEY(H_ENTRY(table, iterator->next)) != H_INVKEY(table))
		{
			*key = EC_HASH_ENTRY_KEY(H_ENTRY(table, iterator->next));
			++iterator->next;

			return TRUE;
		} else
		{
			++iterator->next;
			continue;
		}
	}

	*key = 0;
	return FALSE;
}

/* ------------------------------------------------------------------------
 * P R I V A T E
 * ------------------------------------------------------------------------ */

static ec_hash hash_alloc( void )
{
	ec_hash table;

	table = (ec_hash) ec_malloc( sizeof(struct ec_hash_struct) );
	return table;
}

static void hash_free( ec_hash table )
{
	ec_free( table );
}

static EcBool hash_init( ec_hash table, EcUInt min_size, EcUInt loLoad, EcUInt hiLoad, ec_hash_def *def )
{
	EcUInt i;
	EcUInt new_capacity, new_prime_index;

	EcAny invkey;
	EcAny invval;

	ASSERT(table);

	memcpy( &H_DEF(table), def, sizeof(ec_hash_def) );

	H_ENTRIES(table)    = 0;
	H_CAPACITY(table)   = 0;
	H_PRIMEINDEX(table) = 0;
	H_ENTRYBASE(table)  = NULL;

	H_MINSIZE(table)    = min_size;
	H_LOLOAD(table)     = loLoad;
	H_HILOAD(table)     = hiLoad;

	if (H_MINSIZE(table) <= primes[FIRSTPRIME])
		H_MINSIZE(table) = primes[FIRSTPRIME];

	if ((H_LOLOAD(table) <= 0) || (H_LOLOAD(table) >= (1 << SHIFTAMOUNT)))
		H_LOLOAD(table) = LOLOAD;

	if ((H_HILOAD(table) <= 0) || (H_HILOAD(table) >= (1 << SHIFTAMOUNT)))
		H_HILOAD(table) = HILOAD;

	new_prime_index = FIRSTPRIME;
	ASSERT(new_prime_index < nprimes);

	for (i = 1; i < nprimes; i++)
	{
		if (primes[i] >= H_MINSIZE(table))
		{
			new_prime_index    = i;
			new_capacity       = primes[i];
			H_MINSIZE(table)   = primes[i];						/* normalize minsize */
			break;
		}
	}

	ASSERT(H_MINSIZE(table) > 2);
	ASSERT(new_capacity >= H_MINSIZE(table));
	ASSERT(H_LOLOAD(table) < (1 << SHIFTAMOUNT));
	ASSERT(H_HILOAD(table) < (1 << SHIFTAMOUNT));
	ASSERT(H_LOLOAD(table) < H_HILOAD(table));

	new_capacity = primes[new_prime_index];
#if CHECK_PRIME
	ASSERT( isprime( new_capacity ) );
#endif

	H_ENTRYBASE(table) = (ec_hash_entry) ec_malloc( sizeof(struct ec_hash_entry_struct) * new_capacity );
	if (! H_ENTRYBASE(table)) return FALSE;

	H_CAPACITY(table)   = new_capacity;
	H_PRIMEINDEX(table) = new_prime_index;

	/* IMPORTANT: initialize new slots */
	invkey = H_INVKEY(table);
	invval = H_INVVAL(table);
	for (i = 0; i < new_capacity; i++)
	{
		EC_HASH_ENTRY_KEY(H_ENTRY(table, i))   = invkey;
		EC_HASH_ENTRY_VALUE(H_ENTRY(table, i)) = invval;
	}

#if CHECK_PRIME
	ASSERT( isprime(H_CAPACITY(table)) );
#endif

	return TRUE;
}

static EcBool hash_cleanup( ec_hash table )
{
	EcUInt i;
	ec_hash_entry entry;

	ASSERT(table);

	ASSERT( (H_CAPACITY(table) == 0) || H_ENTRYBASE(table) );
	for (i = 0; i < H_CAPACITY(table); i++)
	{
		entry = H_ENTRY(table, i);
		ASSERT( entry );

		if (H_DEF(table).key_destroy && (EC_HASH_ENTRY_KEY(entry) != H_INVKEY(table)))
			H_KEYDESTROY(table, EC_HASH_ENTRY_KEY(entry));
		if (H_DEF(table).value_destroy && (EC_HASH_ENTRY_VALUE(entry) != H_INVVAL(table)))
			H_VALDESTROY(table, EC_HASH_ENTRY_VALUE(entry));
	}

	ec_free( H_ENTRYBASE(table) );
	H_ENTRYBASE(table) = NULL;

	H_CAPACITY(table)   = 0;
	H_PRIMEINDEX(table) = 0;
	H_ENTRIES(table)    = 0;

	return TRUE;
}

static EcBool hash_expand( ec_hash table )
{
	EcUInt new_capacity = 0, new_prime_index;
	EcUInt load;
	EcUInt i;
	ec_hash new_table;

	ec_hash_entry entry;
	struct ec_hash_entry_struct *old_entrybase;

	EcAny invkey;
	EcAny invval;

	ASSERT( table );

	invkey = H_INVKEY(table);
	invval = H_INVVAL(table);

	ASSERT( H_CAPACITY(table) > 1 );
	ASSERT( H_PRIMEINDEX(table) >= 1 );
#if CHECK_PRIME
	ASSERT( isprime(H_CAPACITY(table)) );
#endif

	new_prime_index = H_PRIMEINDEX(table);

	load  = H_ENTRIES(table) << SHIFTAMOUNT;
	load /= H_CAPACITY(table);

	if (load >= H_HILOAD(table))
	{
		do
		{
			new_prime_index++;
			ASSERT(new_prime_index < nprimes);
			new_capacity = primes[new_prime_index];
			ASSERT(new_capacity);
#if CHECK_PRIME
			ASSERT( isprime( new_capacity ) );
#endif

			load  = H_ENTRIES(table) << SHIFTAMOUNT;
			load /= new_capacity;
		} while (load > H_LOLOAD(table));
	}

	ASSERT(new_capacity >= H_MINSIZE(table));
#if CHECK_PRIME
	ASSERT( isprime( new_capacity ) );
#endif

#if EC_DEBUG && EC_HAS_VARARGS_MACRO
	VADEBMESSAGE( debTable, "Expanding table to %ld elements", (long)new_capacity );
#endif
	new_table = hash_alloc();

	/* ASSERT( !H_ENTRYBASE(new_table) ); */
	memcpy( &H_DEF(new_table), &H_DEF(table), sizeof(ec_hash_def) );
	H_ENTRIES(new_table)    = 0;
	H_CAPACITY(new_table)   = 0;
	H_PRIMEINDEX(new_table) = 0;
	H_ENTRYBASE(new_table)  = 0;
/* 	new_table.loLoad     = (1 << SHIFTAMOUNT); */     /* BUG CORRECTED */
/* 	new_table.hiLoad     = (1 << SHIFTAMOUNT); */
	H_MINSIZE(new_table) = H_MINSIZE(table);
	H_LOLOAD(new_table)  = H_LOLOAD(table);
	H_HILOAD(new_table)  = H_HILOAD(table);

	H_ENTRYBASE(new_table) = (ec_hash_entry) ec_malloc( sizeof(struct ec_hash_entry_struct) * new_capacity );
	if (! H_ENTRYBASE(new_table)) return FALSE;

	H_ENTRIES(new_table)    = 0;
	H_CAPACITY(new_table)   = new_capacity;
	H_PRIMEINDEX(new_table) = new_prime_index;

#if EC_DEBUG && EC_HAS_VARARGS_MACRO
	VADEBMESSAGE( debTable, "new_capacity: %ld, new_prime_index: %ld", (long)new_capacity, (long)new_prime_index );
#endif

	/* IMPORTANT: initialize new slots */
	for (i = 0; i < new_capacity; i++)
	{
		EC_HASH_ENTRY_KEY(H_ENTRY(new_table, i))   = invkey;
		EC_HASH_ENTRY_VALUE(H_ENTRY(new_table, i)) = invval;
	}

#if EC_DEBUG && EC_HAS_VARARGS_MACRO
	VADEBMESSAGE( debTable, "oldTable   capacity: %ld  entries: %ld\n", (long)H_CAPACITY(table), (long)H_ENTRIES(table) );
#endif
	/* fill new table with old values */
	ASSERT( table != new_table );
	for (i = 0; i < H_CAPACITY(table); i++)
	{
		entry = H_ENTRY(table, i);
		if (EC_HASH_ENTRY_KEY(entry) != invkey)
			hash_add( new_table, entry );
	}

#if EC_DEBUG && EC_HAS_VARARGS_MACRO
	VADEBMESSAGE( debTable, "newTable   capacity: %ld  entries: %ld\n", (long)H_CAPACITY(new_table), (long)H_ENTRIES(new_table) );
#endif
	ASSERT( H_ENTRIES(table) == H_ENTRIES(new_table) );

	old_entrybase = H_ENTRYBASE(table);

	H_ENTRIES(table)   = H_ENTRIES(new_table);
	H_CAPACITY(table)  = H_CAPACITY(new_table);
	H_ENTRYBASE(table) = H_ENTRYBASE(new_table);

	H_ENTRYBASE(new_table) = NULL;

	/* free new table space and old entry array (not contents !) */
	ec_free( old_entrybase );
	hash_free( new_table );
	new_table = NULL;

#if CHECK_PRIME
	ASSERT( isprime(H_CAPACITY(table)) );
#endif

	return TRUE;
}

static ec_hash_entry find( ec_hash table, EcAny key )
{
	EcInt  hash, hash2;
	EcUInt M;
	EcAny  invkey;
	EcAny  entrykey;

	ec_hash_entry entry = NULL;

 start:
	ASSERT(table);

	if (H_ENTRIES(table) == 0)
		return NULL;

	invkey = H_INVKEY(table);

	M = H_CAPACITY(table);

	/*
	 * Double hashing
	 */
	ASSERT( M > 2 );
#if CHECK_PRIME
	ASSERT( isprime(M) );
#endif

	/* D1. First hash. */
	hash = H_HASH(table, key) % M;
	ASSERT( (hash >= 0) && (hash <= (M - 1)) );

	/* D2. First probe. */
	entry    = H_ENTRY(table, hash);
	entrykey = EC_HASH_ENTRY_KEY(entry);
	if (entrykey != invkey)
	{
		if (H_DEF(table).key_compare)
		{
			if (H_KEYCMP(table, key, entrykey) == 0)
				return entry;
		} else
		{
			if (key == entrykey)
				return entry;
		}
	} else
		goto D6;												/* empty here */

	/* D3. Second hash. */
	hash2 = 1 + (H_HASH(table, key) % (M - 2));
	ASSERT( (hash2 >= 1) && (hash2 <= (M - 1)) );


	if (! H_DEF(table).key_compare)
	{
		while (TRUE)
		{
			/* D4. Advance to next. */
			hash = hash - hash2;
			if (hash < 0)
				hash = hash + H_CAPACITY(table);
			hash %= H_CAPACITY(table);

			/* D5. Compare. */
			entry    = H_ENTRY(table, hash);
			entrykey = EC_HASH_ENTRY_KEY(entry);
			if (entrykey != invkey)
			{
				if (key == entrykey)
					return entry;
			} else
				goto D6;										/* empty here */
		}
	} else
	{
		while (TRUE)
		{
			/* D4. Advance to next. */
			hash = hash - hash2;
			if (hash < 0)
				hash = hash + H_CAPACITY(table);
			hash %= H_CAPACITY(table);

			/* D5. Compare. */
			entry    = H_ENTRY(table, hash);
			entrykey = EC_HASH_ENTRY_KEY(entry);
			if (entrykey != invkey)
			{
				if (H_KEYCMP(table, key, entrykey) == 0)
					return entry;
			} else
				goto D6;										/* empty here */
		}
	}

	/* D6. */
 D6:
	if (H_ENTRIES(table) == M - 1)
	{
		if (! hash_expand( table ))
			return NULL;
		goto start;
	}

	return NULL;												/* slot not found */
}

static ec_hash_entry find_or_insert( ec_hash table, EcAny key )
{
	EcInt  hash, hash2;
	EcUInt M;
	EcAny  invkey;
	EcAny  entrykey;

	ec_hash_entry entry = NULL;

 start:
	ASSERT(table);

	invkey = H_INVKEY(table);

	M = H_CAPACITY(table);

	/*
	 * Double hashing
	 */
	ASSERT( M > 2 );
#if CHECK_PRIME
	ASSERT( isprime(M) );
#endif

	/* D1. First hash. */
	hash = H_HASH(table, key) % M;
	ASSERT( (hash >= 0) && (hash <= (M - 1)) );

	/* D2. First probe. */
	entry    = H_ENTRY(table, hash);
	entrykey = EC_HASH_ENTRY_KEY(entry);
	if (entrykey != invkey)
	{
		if (H_DEF(table).key_compare)
		{
			if (H_KEYCMP(table, key, entrykey) == 0)
				return entry;
		} else
		{
			if (key == entrykey)
				return entry;
		}
	} else
		goto D6;												/* empty here */

	/* D3. Second hash. */
	hash2 = 1 + (H_HASH(table, key) % (M - 2));
	ASSERT( (hash2 >= 1) && (hash2 <= (M - 1)) );


	if (! H_DEF(table).key_compare)
	{
		while (TRUE)
		{
			/* D4. Advance to next. */
			hash = hash - hash2;
			if (hash < 0)
				hash = hash + H_CAPACITY(table);
			hash %= H_CAPACITY(table);

			/* D5. Compare. */
			entry    = H_ENTRY(table, hash);
			entrykey = EC_HASH_ENTRY_KEY(entry);
			if (entrykey != invkey)
			{
				if (key == entrykey)
					return entry;
			} else
				goto D6;										/* empty here */
		}
	} else
	{
		while (TRUE)
		{
			/* D4. Advance to next. */
			hash = hash - hash2;
			if (hash < 0)
				hash = hash + H_CAPACITY(table);
			hash %= H_CAPACITY(table);

			/* D5. Compare. */
			entry    = H_ENTRY(table, hash);
			entrykey = EC_HASH_ENTRY_KEY(entry);
			if (entrykey != invkey)
			{
				if (H_KEYCMP(table, key, entrykey) == 0)
					return entry;
			} else
				goto D6;										/* empty here */
		}
	}

	/* D6. */
 D6:
	if (H_ENTRIES(table) == M - 1)
	{
		if (! hash_expand( table ))
			return NULL;
		goto start;
	}

	return entry;												/* return a fresh slot */
}

#if EC_DEBUG
#if CHECK_PRIME
static EcBool isprime( EcUInt i )
{
	EcUInt j;

	for (j = 2; j < i; j++)
	{
		if ((i % j) == 0)
			return FALSE;
	}
	return TRUE;
}
#endif /* CHECK_PRIME */

static EcBool verify( ec_hash table, EcAny key, EcAny value )
{
	EcAny realvalue;

	if (! ec_hash_get( table, key, &realvalue ))
	{
		ec_msg_printf( "[1]VERIFY FAILED\n" );
		ASSERT( FALSE );
		return FALSE;
	}

	if (H_DEF(table).value_compare)
	{
		if (H_VALCMP(table, realvalue, value) == 0) return TRUE;
	} else
	{
		if (realvalue == value) return TRUE;
	}

	ec_msg_printf( "[2]VERIFY FAILED\n" );
	ASSERT( FALSE );
	return FALSE;
}
#endif /* EC_DEBUG */
