/* ==========================================================================
 * symbol.c
 * ==========================================================================
 * Symbol handling utility functions.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Fri Dec  4 14:35:43 MET 1998
 *
 *   $Id: symbol.c,v 1.3 2005/03/08 13:40:05 panta Exp $
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

#include "basic.h"
#include "debug.h"
#include "elastic.h"
#include "utility.h"
#include "compat.h"

#include "private.h"


/* ========================================================================
 * P R I V A T E
 * ======================================================================== */

#define SYMNAMEQUANTUM 64

static EcInt qualified_len( EcQualifiedSymbol *qsymbol );


/* ========================================================================
 * P U B L I C
 * ======================================================================== */

/* Symbol handling */

EC_API EcUInt EcInternSymbol( const char *string )
{
	EcUInt id;

	id = EcSymbolId( string );
	if (id) return id;

	ec_strtable_set( PRIVATE(symTable), string, ++PRIVATE(currentId) );

	if (PRIVATE(currentId) >= PRIVATE(symNameSize))
	{
		EcUInt newsize;

		newsize = PRIVATE(symNameSize) + SYMNAMEQUANTUM;
		newsize /= SYMNAMEQUANTUM;
		newsize *= SYMNAMEQUANTUM;

		if (ec_resizeblock( &PRIVATE(symName), newsize, sizeof(char *) ))
		{
			PRIVATE(symNameSize) = newsize;
		}
	}

	ASSERT( PRIVATE(symNameSize) > PRIVATE(currentId) );

	PRIVATE(symName)[PRIVATE(currentId)] = (char *)ec_stringdup( string );
	ASSERT( PRIVATE(symName)[PRIVATE(currentId)] );
	if (! PRIVATE(symName)[PRIVATE(currentId)])
		return 0;

	/*ec_stderr_printf( "EcInternSymbol %3d <- %s\n", PRIVATE(currentId), string );*/
	return PRIVATE(currentId);
}

EC_API EcUInt EcSymbolId( const char *string )
{
	EcUInt res;

	if (ec_strtable_get( PRIVATE(symTable), string, &res ))
		return res;
	else
		return 0;
}

EC_API const char *EcSymbolAt( EcUInt symbolid )
{
	return (const char *) PRIVATE(symName)[symbolid];
}

EC_API EcBool EcSymbolExists( EcUInt symbolid )
{
	if (symbolid > PRIVATE(currentId))
		return FALSE;
	if (! PRIVATE(symName)[symbolid])							/* this should never happen */
		return FALSE;
	return TRUE;
}


EC_API EcBool EcNameQualified( const ec_string *name )
{
	const char *start;

	start = ec_strdata( name );
	if (! start) return FALSE;

	if (index( ec_strdata( name ), '.' ))
		return TRUE;

	return FALSE;
}

EC_API char *EcQualifyName( ec_string *dst, const ec_string *prefix, const ec_string *suffix )
{
	ec_string_cleanup( dst );

	ec_strcatd( dst, prefix );
	ec_strcatc( dst, '.' );
	ec_strcatd( dst, suffix );

	return ec_strdata( dst );
}

EC_API char *EcNamePrefix( ec_string *dst, const ec_string *name )
{
	const char *start, *at;

	ec_string_cleanup( dst );

	start = ec_strdata( name );
	if (! start) return NULL;

	at = rindex( start, '.' );
	if (! at)
		return NULL;

	ec_strcat( dst, start, at - start );

	return ec_strdata( dst );
}

EC_API char *EcNameSuffix( ec_string *dst, const ec_string *name )
{
	const char  *start, *p, *at;
	EcInt        len;

	ec_string_cleanup( dst );

	start = ec_strdata( name );
	if (! start) return NULL;

	at = 0;
	p = start;
	while (p && *p)
	{
		if (*p == '.')
			at = p;

		p++;
	}

	if (! at)
		return NULL;

	ASSERT( p > at );
	len = (p - at) - 1;

	if (len > 0)
		ec_strcat( dst, at + 1, len );

	return ec_strdata( dst );
}



/* Qualified symbol handling */

EC_API void EcQualifiedInit( EcQualifiedSymbol *qsymbol )
{
	QSLEN(qsymbol) = 0;
}

EC_API EcQualifiedSymbol *EcQualifiedCopy( EcQualifiedSymbol *dst, EcQualifiedSymbol *src )
{
	EcInt i;

	ASSERT( dst );
	ASSERT( src );

	for (i = 0; i < QSLEN(src); i++)
		QSCOMP(dst, i) = QSCOMP(src, i);
	QSLEN(dst) = QSLEN(src);

	return dst;
}

EC_API void EcQualifiedAdd( EcQualifiedSymbol *qsymbol, EcUInt sym )
{
	ASSERT( qsymbol );
	ASSERT( QSLEN(qsymbol) + 1 < EC_MAXSYMBOLNESTING );

	QSCOMP(qsymbol, QSLEN(qsymbol)++) = sym;
}

EC_API void EcQualifiedCat( EcQualifiedSymbol *dst, EcQualifiedSymbol *qs1, EcQualifiedSymbol *qs2 )
{
	EcInt i;

	ASSERT( dst );
	ASSERT( qs1 );
	ASSERT( qs2 );
	ASSERT( QSLEN(qs1) + QSLEN(qs2) <= EC_MAXSYMBOLNESTING );

	if (dst != qs1)
	{
		for (i = 0; i < QSLEN(qs1); i++)
			QSCOMP(dst, i) = QSCOMP(qs1, i);

		QSLEN(dst) = QSLEN(qs1);
	}

	for (i = QSLEN(qs1); i < QSLEN(qs1) + QSLEN(qs2); i++)
		QSCOMP(dst, i) = QSCOMP(qs2, i - QSLEN(qs1));
	QSLEN(dst) = QSLEN(qs1) + QSLEN(qs2);
}

EC_API EcBool EcQualifiedEq( EcQualifiedSymbol *s1, EcQualifiedSymbol *s2 )
{
	EcInt i;

	if (s1 == s2)
		return TRUE;

	if ((! s1) || (! s2))
		return FALSE;

	if (QSLEN(s1) != QSLEN(s2))
		return FALSE;

	for (i = 0; i < QSLEN(s1); i++)
		if (QSCOMP(s1, i) != QSCOMP(s2, i)) return FALSE;

	return TRUE;
}

EC_API char *EcQualifiedString( EcQualifiedSymbol *qsymbol )
{
	EcInt i;
	EcInt len;
	char *string;

	len = qualified_len( qsymbol );
	string = ec_malloc( len + 1 );
	if (! string)
		return NULL;

	string[0] = '\0';
	for (i = 0; i < QSLEN(qsymbol); i++)
	{
		strcat( string, EcSymbolAt( QSCOMP(qsymbol, i) ) );

		if (i + 1 < QSLEN(qsymbol))
			strcat( string, "." );
	}

	return string;
}

EC_API void EcQualifiedFromString( EcQualifiedSymbol *dst, const char *string )
{
	char *str;
	char *tok;
	const char *tok_s;
	const char *sep;
	EcInt len;

	len = strlen( string );
	str = alloca( len + 1 );
	tok = alloca( len + 1 );
	ASSERT( str && tok );
	if (! (str && tok))
		return;
	strcpy( str, string );

	EcQualifiedInit( dst );
	sep = str;
	while ((tok_s = ec_string_tokenize( &sep, &len, '.' )))
	{
		ec_string_sub( tok, tok_s, 0, len );
		EcQualifiedAdd( dst, EcInternSymbol( tok ) );
	}
}

static EcInt qualified_len( EcQualifiedSymbol *qsymbol )
{
	EcInt len, i;

	/* Calculate length */
	for (len = 0, i = 0; i < QSLEN(qsymbol); i++)
	{
		len += strlen( EcSymbolAt( QSCOMP(qsymbol, i) ) );

		if (i + 1 < QSLEN(qsymbol))
			len++;
	}

	return len;
}
