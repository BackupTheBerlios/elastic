/* ==========================================================================
 * strtable.c
 * ==========================================================================
 * String table.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat May 23 17:02:16 MEST 1998
 *
 *   $Id: strtable.c,v 1.1 2002/05/23 21:17:40 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1998-2000 Marco Pantaleoni. All rights reserved.
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

#include <limits.h>

#include "basic.h"
#include "memory.h"
#include "object.h"
#include "hashfunc.h"
#include "debug.h"

typedef struct _entry
{
	const char *key;
	EcUInt      value;

	struct _entry *next;
} StrTableEntry;

typedef struct _strtable
{
	StrTableEntry **slot;
	EcInt nslots;
	EcInt nentries;
} ec_strtable;


/* ========================================================================
 * S T A T I C
 * ======================================================================== */

/* ========================================================================
 * A P I
 * ======================================================================== */

EC_API ec_strtable *ec_strtable_create( EcInt nslots )
{
	ec_strtable *res;
	EcInt i;

	res = (ec_strtable *) ec_malloc( sizeof(struct _strtable) );
	if (! res) return NULL;

	res->slot = (StrTableEntry **) ec_malloc( sizeof(struct _entry *) * nslots );
	if (! res->slot)
	{
		ec_free( res );
		return NULL;
	}

	for (i = 0; i < nslots; i++)
		res->slot[i] = NULL;

	res->nslots   = nslots;
	res->nentries = 0;

	return res;
}

EC_API void ec_strtable_destroy( ec_strtable *table )
{
	EcInt i;
	StrTableEntry *entry, *next;

	for (i = 0; i < table->nslots; i++)
	{
		entry = table->slot[i];

		while (entry)
		{
			next = entry->next;

			ec_free( (char *)(entry->key) );
			entry->key   = NULL;
			entry->value = 0;
			ec_free( entry );

			entry = next;
		}

		table->slot[i] = NULL;
	}

	ec_free( table->slot );
	table->slot = NULL;
	table->nslots   = 0;
	table->nentries = 0;

	ec_free( table );
}

static StrTableEntry *search( ec_strtable *table, const char *key )
{
	StrTableEntry *entry;
	EcUInt hash;

	hash = ec_hash_pjw_string( key );
	hash %= table->nslots;

	entry = table->slot[hash];

	while (entry)
	{
		if (strcmp( entry->key, key ) == 0) return entry;

		entry = entry->next;
	}

	return NULL;
}

EC_API EcBool ec_strtable_set( ec_strtable *table, const char *key, EcUInt value )
{
	StrTableEntry *entry;
	EcUInt hash;

	entry = search( table, key );
	if (entry)
	{
		entry->value = value;
		return TRUE;
	}

	hash = ec_hash_pjw_string( key );
	hash %= table->nslots;

	entry = (StrTableEntry *) ec_malloc( sizeof(StrTableEntry) );
	if (! entry)
		return FALSE;
	entry->next = NULL;
	entry->key = ec_stringdup( key );
	if (! entry->key)
	{
		ec_free( entry );
		return FALSE;
	}
	entry->value = value;
	entry->next  = table->slot[hash];
	table->slot[hash] = entry;
	table->nentries++;

	return TRUE;
}

EC_API EcBool ec_strtable_get( ec_strtable *table, const char *key, EcUInt *value )
{
	StrTableEntry *entry;

	entry = search( table, key );
	if (entry)
	{
		if (value) *value = entry->value;
		return TRUE;
	}
	return FALSE;
}
