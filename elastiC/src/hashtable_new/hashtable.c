/* ==========================================================================
 * hashtable.c
 * ==========================================================================
 * Generic hash table.h
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Thu Jun 24 10:34:09 MEST 1999
 *
 *   $Id: hashtable.c,v 1.1 2002/05/23 21:20:29 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1999-2001 Marco Pantaleoni. All rights reserved.
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

#include "hashfunc.h"

#include "hashtable.h"

struct ec_hash_struct
{
	ec_hash_def def;											/* Operations                              */
	EcUInt      entries;
	EcUInt      capacity;
	EcUInt      mincapacity;

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
#define H_ENTRYBASE(h)			((h)->entry)
#define H_ENTRY(h,i)			(&(H_ENTRYBASE(h)[i]))

static ec_hash       hash_alloc( void );
static void          hash_free( ec_hash table );
static EcBool        hash_init( ec_hash table, EcUInt minSize, ec_hash_def *def );
static EcBool        hash_cleanup( ec_hash table );
static EcBool        hash_expand( ec_hash table );
static ec_hash_entry find( ec_hash table, EcAny key );

#ifdef EC_DEBUG
static EcBool verify( ec_hash table, EcAny key, EcAny value );
#endif

/* ========================================================================
 * A P I
 * ======================================================================== */

EC_API ec_hash ec_hash_create( EcUInt minSize, EcUInt loLoad, EcUInt hiLoad, ec_hash_def *def )
{
	ec_hash table;

	table = hash_alloc();
	if (table)
	{
		if (! hash_init( table, minSize, def ))
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

#define KEY_EQP(table, key1, key2) \
	((H_DEF((table)).key_compare ? (H_KEYCMP((table), (key1), (key2)) == 0) : ((key1) == (key2))))

#define VALUE_RELEASE(table, entry) \
do { \
	if (H_DEF((table)).value_destroy && (EC_HASH_ENTRY_VALUE((entry)) != H_INVVAL((table)))) \
		H_VALDESTROY((table), EC_HASH_ENTRY_VALUE((entry))); \
} while (0)

#define VALUE_SET(table, entry, value) \
do { \
	VALUE_RELEASE((table), (entry)); \
	EC_HASH_ENTRY_VALUE((entry)) = H_DEF((table)).value_copy ? H_VALCOPY((table), (value)) : (value); \
} while (0)

EC_API EcBool ec_hash_get_entry( ec_hash table, EcAny key, ec_hash_entry *entry )
{
	ec_hash_entry res;

	res = find( table, key );
	if (entry) *entry = res;

	return (res != NULL);
}

EC_API EcBool ec_hash_get( ec_hash table, EcAny key, EcAny *value )
{
#if 0
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
#endif
	EcUInt        hashv;
	ec_hash_entry entry;
	EcAny         invkey;

	ASSERT(table);
	ASSERT( H_CAPACITY(table) > 1 );

	invkey = H_INVKEY(table);
	/* TODO: add delkey support */

	hashv = H_HASH(table, key) % H_CAPACITY(table);
	ASSERT( (hashv >= 0) && (hashv <= (H_CAPACITY(table) - 1)) );

	/*
	 * This search works *only* if the table is not full !
	 */
	ASSERT( H_ENTRIES(table) < H_CAPACITY(table) );

	if (! H_DEF(table).key_compare)
	{
		while (1)
		{
			entry = H_ENTRY(table, hashv);

			if (EC_HASH_ENTRY_KEY(entry) == invkey)				/* found free slot */ /* TODO: add delkey */
			{
				if (value)
					*value = H_INVVAL(table);
				return FALSE;
			}

			if (EC_HASH_ENTRY_KEY(entry) == key)
			{
				if (value) *value = EC_HASH_ENTRY_VALUE(entry);
				return TRUE;
			}

			hashv = (hashv + 1) % H_CAPACITY(table);
		}
	} else
	{
		while (1)
		{
			entry = H_ENTRY(table, hashv);

			if (EC_HASH_ENTRY_KEY(entry) == invkey)				/* found free slot */ /* TODO: add delkey */
			{
				if (value)
					*value = H_INVVAL(table);
				return FALSE;
			}

			if (H_KEYCMP(table, EC_HASH_ENTRY_KEY(entry), key) == 0)
			{
				if (value) *value = EC_HASH_ENTRY_VALUE(entry);
				return TRUE;
			}

			hashv = (hashv + 1) % H_CAPACITY(table);
		}
	}

	/* UNREACHABLE */
	ASSERT( FALSE );
	return FALSE;
}

/*
 * hash_add
 *    -- used to transfer (key, value) pairs from an hash table to another one
 *       Since it is used only for expansion, it doesn't need to perform
 *       copies of keys and values.
 */
static EcBool hash_add( ec_hash table, ec_hash_entry oldentry )
{
	EcUInt        hashv;
	ec_hash_entry entry;
	EcAny         invkey;

	ASSERT(table);

	ASSERT( H_CAPACITY(table) > 1 );

	invkey = H_INVKEY(table);
	/* TODO: add delkey support */

	hashv = H_HASH(table, EC_HASH_ENTRY_KEY(oldentry)) % H_CAPACITY(table);
	ASSERT( (hashv >= 0) && (hashv <= (H_CAPACITY(table) - 1)) );

	while (1)
	{
		entry = H_ENTRY(table, hashv);

		if (EC_HASH_ENTRY_KEY(entry) == invkey) break;			/* found free slot */ /* TODO: add delkey */

		hashv = (hashv + 1) % H_CAPACITY(table);
	}

	ASSERT( (hashv >= 0) && (hashv <= (H_CAPACITY(table) - 1)) );
	ASSERT( entry == H_ENTRY(table, hashv) );
	ASSERT( EC_HASH_ENTRY_KEY(entry) == invkey );

	EC_HASH_ENTRY_KEY(entry)   = EC_HASH_ENTRY_KEY(oldentry);
	EC_HASH_ENTRY_VALUE(entry) = EC_HASH_ENTRY_VALUE(oldentry);
	H_ENTRIES(table)++;

	ASSERT( (H_ENTRIES(table) * 2) < H_CAPACITY(table) );
	ASSERT( verify( table, EC_HASH_ENTRY_KEY(oldentry), EC_HASH_ENTRY_VALUE(oldentry) ) );

	return TRUE;
}

EC_API EcBool ec_hash_set( ec_hash table, EcAny key, EcAny value )
{
	EcUInt        hashv;
	ec_hash_entry entry;
	EcAny         invkey;

	ASSERT(table);

	ASSERT( H_CAPACITY(table) > 1 );

	invkey = H_INVKEY(table);
	/* TODO: add delkey support */

	hashv = H_HASH(table, key) % H_CAPACITY(table);
	ASSERT( (hashv >= 0) && (hashv <= (H_CAPACITY(table) - 1)) );

	/*
	 * This search works *only* if the table is not full !
	 */
	ASSERT( H_ENTRIES(table) < H_CAPACITY(table) );

	if (! H_DEF(table).key_compare)
	{
		while (1)
		{
			entry = H_ENTRY(table, hashv);

			if (EC_HASH_ENTRY_KEY(entry) == invkey) break;		/* found free slot */ /* TODO: add delkey */

			if (EC_HASH_ENTRY_KEY(entry) == key)
			{
				VALUE_SET(table, entry, value);
				return TRUE;
			}

			hashv = (hashv + 1) % H_CAPACITY(table);
		}
	} else
	{
		while (1)
		{
			entry = H_ENTRY(table, hashv);

			if (EC_HASH_ENTRY_KEY(entry) == invkey) break;		/* found free slot */ /* TODO: add delkey */

			if (H_KEYCMP(table, EC_HASH_ENTRY_KEY(entry), key) == 0)
			{
				VALUE_SET(table, entry, value);
				return TRUE;
			}

			hashv = (hashv + 1) % H_CAPACITY(table);
		}
	}

	ASSERT( (hashv >= 0) && (hashv <= (H_CAPACITY(table) - 1)) );
	ASSERT( entry == H_ENTRY(table, hashv) );
	ASSERT( EC_HASH_ENTRY_KEY(entry) == invkey );				/* TODO: add delkey */

	EC_HASH_ENTRY_KEY(entry)   = H_DEF(table).key_copy   ? H_KEYCOPY(table, key)   : key;
	EC_HASH_ENTRY_VALUE(entry) = H_DEF(table).value_copy ? H_VALCOPY(table, value) : value;
	H_ENTRIES(table)++;

	/* In case we exceeded the load factor, expand the table */
	if ((H_ENTRIES(table) * 2) >= H_CAPACITY(table))
	{
		if (! hash_expand( table ))
			return FALSE;
	}

	ASSERT( verify( table, key, value ) );
	return TRUE;
}

EC_API EcBool ec_hash_del( ec_hash hash, EcAny key )
{
	/* TODO: To be implemented ... */
	ASSERT( FALSE );
	return FALSE;
}

EC_API EcBool ec_hash_has_key( ec_hash table, EcAny key )
{
	return find( table, key ) ? TRUE : FALSE;
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
		if (EC_HASH_ENTRY_KEY(H_ENTRY(table, iterator->next)) != H_INVKEY(table)) /* TODO add delkey */
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

static EcBool hash_init( ec_hash table, EcUInt minSize, ec_hash_def *def )
{
	EcUInt capacity;
	EcUInt i;

	EcAny invkey;
	EcAny invval;

	ASSERT(table);

	memcpy( &H_DEF(table), def, sizeof(ec_hash_def) );

	H_ENTRIES(table)   = 0;
	H_CAPACITY(table)  = 0;
	H_ENTRYBASE(table) = NULL;

	capacity = minSize;
	if (capacity < 2)
		capacity = 2;

	H_ENTRYBASE(table) = (ec_hash_entry) ec_malloc( sizeof(struct ec_hash_entry_struct) * capacity );
	if (! H_ENTRYBASE(table))
		return FALSE;

	H_CAPACITY(table) = capacity;

	invkey = H_INVKEY(table);
	invval = H_INVVAL(table);

	/* IMPORTANT: initialize new slots */
	for (i = 0; i < capacity; i++)
	{
		EC_HASH_ENTRY_KEY(H_ENTRY(table, i))   = invkey;
		EC_HASH_ENTRY_VALUE(H_ENTRY(table, i)) = invval;
	}

	ASSERT( H_CAPACITY(table) > 1 );

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

	H_CAPACITY(table) = 0;
	H_ENTRIES(table)  = 0;

	return TRUE;
}

static EcBool hash_expand( ec_hash table )
{
	EcUInt new_capacity = 0;
	EcUInt i;
	ec_hash new_table = NULL;

	ec_hash_entry entry;
	struct ec_hash_entry_struct *old_entrybase;

	EcAny invkey;
	EcAny invval;

	ASSERT( table );
	ASSERT( !new_table );
	ASSERT( table != new_table );

	invkey = H_INVKEY(table);
	invval = H_INVVAL(table);

	new_capacity = H_CAPACITY(table) * 2;
	ASSERT(new_capacity >= 2);

	ASSERT( H_ENTRYBASE(table) );

	ASSERT( !new_table );
	new_table = hash_alloc();

	ASSERT( !H_ENTRYBASE(new_table) );
	memcpy( &H_DEF(new_table), &H_DEF(table), sizeof(ec_hash_def) );
	H_ENTRIES(new_table)  = 0;
	H_CAPACITY(new_table) = 0;

	H_ENTRYBASE(new_table) = (ec_hash_entry) ec_malloc( sizeof(struct ec_hash_entry_struct) * new_capacity );
	if (! H_ENTRYBASE(new_table)) return FALSE;

	H_ENTRIES(new_table)  = 0;
	H_CAPACITY(new_table) = new_capacity;

	/* IMPORTANT: initialize new slots */
	for (i = 0; i < new_capacity; i++)
	{
		EC_HASH_ENTRY_KEY(H_ENTRY(new_table, i))   = invkey;
		EC_HASH_ENTRY_VALUE(H_ENTRY(new_table, i)) = invval;
	}

	/* fill new table with old values */
	ASSERT( table != new_table );

	for (i = 0; i < H_CAPACITY(table); i++)
	{
		entry = H_ENTRY(table, i);
		if (EC_HASH_ENTRY_KEY(entry) != invkey)
			hash_add( new_table, entry );
	}

	ASSERT( H_ENTRIES(table) == H_ENTRIES(new_table) );

	old_entrybase = H_ENTRYBASE(table);

	H_ENTRIES(table)   = H_ENTRIES(new_table);
	H_CAPACITY(table)  = H_CAPACITY(new_table);
	H_ENTRYBASE(table) = H_ENTRYBASE(new_table);

	H_ENTRYBASE(new_table) = NULL;

	/* free new table space (not contents !) */
	ec_free( old_entrybase );
	hash_free( new_table );
	new_table = NULL;

	return TRUE;
}

static ec_hash_entry find( ec_hash table, EcAny key )
{
	EcUInt        hashv;
	ec_hash_entry entry;
	EcAny         invkey;

	ASSERT(table);

	ASSERT( H_CAPACITY(table) > 1 );

	invkey = H_INVKEY(table);
	/* TODO: add delkey support */

	hashv = H_HASH(table, key) % H_CAPACITY(table);
	ASSERT( (hashv >= 0) && (hashv <= (H_CAPACITY(table) - 1)) );

	/*
	 * This search works *only* if the table is not full !
	 */
	ASSERT( H_ENTRIES(table) < H_CAPACITY(table) );

	while (1)
	{
		entry = H_ENTRY(table, hashv);

		if (EC_HASH_ENTRY_KEY(entry) == invkey) return NULL;	/* found free slot */ /* TODO: add delkey */

		if (KEY_EQP(table, EC_HASH_ENTRY_KEY(entry), key))
			return entry;

		hashv = (hashv + 1) % H_CAPACITY(table);
	}

	/* UNREACHABLE */
	ASSERT( FALSE );
	return NULL;
}

#if EC_DEBUG
static EcBool verify( ec_hash table, EcAny key, EcAny value )
{
	EcAny realvalue;

	if (! ec_hash_get( table, key, &realvalue ))
	{
		fprintf( stderr, "[1]VERIFY FAILED\n" );
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

	fprintf( stderr, "[2]VERIFY FAILED\n" );
	ASSERT( FALSE );
	return FALSE;
}
#endif /* EC_DEBUG */
