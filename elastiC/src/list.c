/* ==========================================================================
 * list.c
 * ==========================================================================
 * Simple list of keyed objects. Keys and data are of type EcAny.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat Dec  4 10:01:34 MET 1999
 *
 *   $Id: list.c,v 1.1 2002/05/23 21:17:23 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1999-2000 Marco Pantaleoni. All rights reserved.
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

#include <stdio.h>
#include <stdlib.h>

#include "basic.h"
#include "debug.h"
#include "private.h"
#include "list.h"
#include "memory.h"
#include "utility.h"

#define LIST_MAGIC	0xbadbadff
#define NODE_MAGIC	0xda14fe81
#define ITER_MAGIC	0x1e2d3a74
#define MAGIC(p)	((p)->magic)

/* ========================================================================
 * T Y P E S
 * ------------------------------------------------------------------------ */

struct ec_list_struct
{
	EcDWord magic;
	ec_list_node head;
};

#define HEAD(list)	((list)->head)

struct ec_list_node_struct
{
	EcDWord magic;
	EcAny        key;
	EcAny        data;
	ec_list_node next;
};

#define KEY(node)	((node)->key)
#define DATA(node)	((node)->data)
#define NEXT(node)	((node)->next)

struct ec_list_iterator_struct
{
	EcDWord magic;
	ec_list      list;
	ec_list_node curpos;
};

#define ITLIST(it)	((it)->list)
#define ITPOS(it)	((it)->curpos)

/* ========================================================================
 * I M P L E M E N T A T I O N
 * ------------------------------------------------------------------------ */

static inline void   make_magic_list(ec_list p)          { MAGIC(p) = LIST_MAGIC; }
static inline void   make_magic_node(ec_list_node p)     { MAGIC(p) = NODE_MAGIC; }
static inline void   make_magic_iter(ec_list_iterator p) { MAGIC(p) = ITER_MAGIC; }
static inline void   del_magic_list(ec_list p)           { MAGIC(p) = 0; }
static inline void   del_magic_node(ec_list_node p)      { MAGIC(p) = 0; }
static inline void   del_magic_iter(ec_list_iterator p)  { MAGIC(p) = 0; }

static inline EcBool check_list(ec_list p)
{
	if (MAGIC(p) != LIST_MAGIC)
	{
		fprintf( stderr, "CORRUPTED/NOT A LIST: 0x%08lX\n", (unsigned long)p );
		return FALSE;
	}
	return TRUE;
}

static inline EcBool check_node(ec_list_node p)
{
	if (MAGIC(p) != NODE_MAGIC)
	{
		fprintf( stderr, "CORRUPTED/NOT A NODE: 0x%08lX\n", (unsigned long)p );
		return FALSE;
	}
	return TRUE;
}

static inline EcBool check_iter(ec_list_iterator p)
{
	if (MAGIC(p) != ITER_MAGIC)
	{
		fprintf( stderr, "CORRUPTED/NOT AN ITERATOR: 0x%08lX\n", (unsigned long)p );
		return FALSE;
	}
	return TRUE;
}

static ec_list_node alloc_node( EcAny key, EcAny data )
{
	ec_list_node node;

	node = (ec_list_node) ec_malloc( sizeof(struct ec_list_node_struct) );
	if (! node) return NULL;

	make_magic_node(node);
	NEXT(node) = NULL;
	KEY(node)  = key;
	DATA(node) = data;
	return node;
}

static void free_node( ec_list_node node )
{
	check_node(node);
	del_magic_node(node);
	ec_free( node );
}

EC_API ec_list ec_list_create( void )
{
	ec_list list;

	list = (ec_list) ec_malloc( sizeof(struct ec_list_struct) );
	if (! list) return NULL;

	make_magic_list(list);
	HEAD(list) = NULL;
	check_list(list);
	return list;
}

EC_API ec_list ec_list_copy( ec_list list )
{
	ec_list          newlist;
	ec_list_iterator iter;
	ec_list_node     node, newnode, last;

	check_list(list);

	newlist = ec_list_create();
	if (! newlist) return NULL;

	last = NULL;
	iter = ec_list_iterator_create( list );
	while ((node = ec_list_iterator_next( iter )))
	{
		check_node(node);
		newnode = alloc_node( KEY(node), DATA(node) );
		ASSERT( newnode );
		if (! newnode) return NULL;								/* argh ! */
		if (last)
			NEXT(last) = newnode;
		else
		{
			HEAD(newlist) = newnode;
			last          = newnode;
		}
	}
	ec_list_iterator_destroy( iter );

	check_list(newlist);
	return newlist;
}

EC_API void ec_list_destroy( ec_list list )
{
	ec_list_node node, next;

	check_list(list);
	if (! list) return;

	node = HEAD(list);
	next = NULL;
	while (node)
	{
		check_node(node);
		next = NEXT(node);
		free_node( node );
		node = next;
	}

	del_magic_list(list);
	ec_free( list );
}

EC_API ec_list_node ec_list_head( ec_list list )
{
	check_list(list);
	return HEAD(list);
}

EC_API ec_list_node ec_list_tail( ec_list list )
{
	ec_list_node node;

	check_list(list);
	/* find the last node */
	for (node = HEAD(list); node && NEXT(node); node = NEXT(node))
		check_node(node)
		;

	return node;
}

EC_API ec_list_node ec_list_append( ec_list list, EcAny key, EcAny data )
{
	ec_list_node last, newnode;

	check_list(list);
	last = ec_list_tail( list );

	newnode = alloc_node( key, data );
	ASSERT( newnode );
	if (! newnode) return NULL;									/* argh ! */
	if (last)
	{
		check_node(last);
		NEXT(last) = newnode;
	}
	else
		HEAD(list) = newnode;
	check_node(newnode);
	return newnode;
}

EC_API ec_list_node ec_list_prepend( ec_list list, EcAny key, EcAny data )
{
	ec_list_node newnode;

	check_list(list);
	newnode = alloc_node( key, data );
	ASSERT( newnode );
	if (! newnode) return NULL;									/* argh ! */
	NEXT(newnode) = HEAD(list);
	HEAD(list)    = newnode;
	check_node(newnode);
	return newnode;
}

EC_API ec_list ec_list_concat( ec_list list1, ec_list list2 )
{
	ec_list_node last;

	check_list(list1);
	check_list(list2);
	last = ec_list_tail( list1 );
	check_node(last);

	if (last)
		NEXT(last) = HEAD(list2);
	else
		HEAD(list1) = HEAD(list2);
	check_list(list1);
	return list1;
}

EC_API ec_list ec_list_remove( ec_list list, ec_list_node node )
{
	ec_list_node prev, cur;

	check_list(list);
	check_node(node);
	if (! node) return list;

	prev = NULL;
	cur  = HEAD(list);
	while (cur && (cur != node))
	{
		prev = cur;
		cur  = NEXT(cur);
	}

	if (cur != node) return list;

	ASSERT( cur && (cur == node) );

	if (prev)
		NEXT(prev) = NEXT(node);
	else
		HEAD(list) = NEXT(node);
	NEXT(node) = NULL;

	check_list(list);
	return list;
}

EC_API ec_list_node ec_list_find( ec_list list, EcAny key )
{
	ec_list_node cur;

	check_list(list);
	cur = HEAD(list);
	if (! cur) return NULL;
	while (cur)
	{
		if (KEY(cur) == key) return cur;
		cur = NEXT(cur);
	}

	return NULL;
}

EC_API EcBool ec_list_finddata( ec_list list, EcAny key, EcAny *data )
{
	ec_list_node node;

	check_list(list);
	node = ec_list_find( list, key );
	if (! node) return FALSE;
	check_node(node);
	if (data)
		*data = DATA(node);
	return TRUE;
}

EC_API EcAny ec_list_key( ec_list_node node )
{
	check_node(node);
	return KEY(node);
}

EC_API EcAny ec_list_data( ec_list_node node )
{
	check_node(node);
	return DATA(node);
}

EC_API ec_list_iterator ec_list_iterator_create( ec_list list )
{
	ec_list_iterator iter;

	check_list(list);
	iter = (ec_list_iterator) ec_malloc( sizeof(struct ec_list_iterator_struct) );
	if (! iter) return NULL;

	make_magic_iter(iter);
	ITLIST(iter) = list;
	ITPOS(iter)  = HEAD(ITLIST(iter));
	check_iter(iter);
	return iter;
}

EC_API void ec_list_iterator_destroy( ec_list_iterator iterator )
{
	check_iter(iterator);
	del_magic_iter(iterator);
	ec_free( iterator );
}

EC_API void ec_list_iterator_rewind( ec_list_iterator iterator )
{
	check_iter(iterator);
	ITPOS(iterator) = HEAD(ITLIST(iterator));
}

EC_API ec_list_node ec_list_iterator_next( ec_list_iterator iterator )
{
	ec_list_node node;

	check_iter(iterator);
	node = NULL;
	if (ITPOS(iterator))
	{
		node = ITPOS(iterator);
		check_node(node);
		ITPOS(iterator) = NEXT(node);
	}

	return node;
}
