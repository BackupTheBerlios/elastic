/* ==========================================================================
 * string_m.c
 * ==========================================================================
 * String module.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sun Nov 21 14:02:53 MET 1999
 *
 *   $Id: string_m.c,v 1.2 2002/05/25 19:37:45 panta Exp $
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
#include "compat.h"
#include "private.h"

#include "string_m.h"

#include <ctype.h>

static EC_OBJ EcLibString_Length( EC_OBJ stack, EcAny userdata );
static EC_OBJ EcLibString_Sub( EC_OBJ stack, EcAny userdata  );
static EC_OBJ EcLibString_Range( EC_OBJ stack, EcAny userdata  );
static EC_OBJ EcLibString_LTrim( EC_OBJ stack, EcAny userdata  );
static EC_OBJ EcLibString_RTrim( EC_OBJ stack, EcAny userdata  );
static EC_OBJ EcLibString_Trim( EC_OBJ stack, EcAny userdata  );
static EC_OBJ EcLibString_IsUpper( EC_OBJ stack, EcAny userdata  );
static EC_OBJ EcLibString_IsLower( EC_OBJ stack, EcAny userdata  );
static EC_OBJ EcLibString_ToUpper( EC_OBJ stack, EcAny userdata  );
static EC_OBJ EcLibString_ToLower( EC_OBJ stack, EcAny userdata  );
static EC_OBJ EcLibString_Char( EC_OBJ stack, EcAny userdata  );
static EC_OBJ EcLibString_Ascii( EC_OBJ stack, EcAny userdata  );
static EC_OBJ EcLibString_MakeSymbol( EC_OBJ stack, EcAny userdata  );

static EC_OBJ EcLibString_Split( EC_OBJ stack, EcAny userdata  );
static EC_OBJ EcLibString_Join( EC_OBJ stack, EcAny userdata  );


/* ========================================================================
 * P R I V A T E   F U N C T I O N S    P R O T O T Y P E S
 * ======================================================================== */

#define NOSPACE -2
#define NOMATCH -1
static EcInt strfind( const EcChar *haystack, EcInt hslen,
					  const EcChar *needle, EcInt nlen );
static EcInt skipspaces( const EcChar *str, EcInt len );
static EcInt findspaces( const EcChar *str, EcInt len );

/* ========================================================================
 * string.* primitives
 * ======================================================================== */

static EC_OBJ EcLibString_Length( EC_OBJ stack, EcAny userdata  )
{
#if 0
	EC_OBJ  res;
	char   *str;
	EcInt   length;

	res = EcParseStackFunction( "string.length", TRUE, stack, "s#", &str, &length );
	if (EC_ERRORP(res))
		return res;

	return EcMakeInt( length );
#endif
	EC_OBJ str;

	EC_CHECKNARGS_F("string.length", 1);
	str = EC_STACKPOP(stack);
	EC_CHECKTYPE_F( "string.length", 1, str, tc_string );
	return EcMakeInt( EC_STRLEN(str) );
}

static EC_OBJ EcLibString_Sub( EC_OBJ stack, EcAny userdata  )
{
#if 0
	EC_OBJ  obj;
	EC_OBJ  res;
	char   *str;
	EcInt   length, start, len;
	EcInt   s1, s2;
	char   *buf;

	/* Parse two times to get string length */

	res = EcParseStackFunction( "string.sub", FALSE, stack, "s#i|i", &str, &length, &start, &len );
	if (EC_ERRORP(res))
		return res;

	if (start >= 0)
		s1 = start;
	else
		s1 = (length - 1) - start;
	len = length - s1;

	res = EcParseStackFunction( "string.sub", FALSE, stack, "s#i|i", &str, &length, &start, &len );
	if (EC_ERRORP(res))
		return res;

	res = EcParseStackFunction( "string.sub", TRUE, stack, "O.|.", &obj );
	if (EC_ERRORP(res))
	return res;
#endif
	EC_OBJ str_o;
	EC_OBJ start_o, len_o = EC_NIL;
	char   *str;
	EcInt   length, start, len;
	EcInt   s1, s2;
	char   *buf;

	EC_CHECKNARGS_OPT_F("string.sub", 2, 3);
	if (EC_STACKNARGS_LOGICAL(stack) == 2)
	{
		start_o = EC_STACKPOP(stack);
		str_o   = EC_STACKPOP(stack);
	} else
	{
		len_o   = EC_STACKPOP(stack);
		start_o = EC_STACKPOP(stack);
		str_o   = EC_STACKPOP(stack);

		EC_CHECKTYPE_F( "string.sub", 3, len_o, tc_inum );
	}

	EC_CHECKTYPE_F( "string.sub", 1, str_o,   tc_string );
	EC_CHECKTYPE_F( "string.sub", 2, start_o, tc_inum );
	str    = EC_STRDATA(str_o);
	length = EC_STRLEN(str_o);
	start  = EC_INUM(start_o);

	if (start >= 0)
		s1 = start;
	else
		s1 = (length - 1) - start;
	len = length - s1;

	if (EC_INUMP(len_o))
		len = EC_INUM(len_o);

	if (start >= 0)
		s1 = start;
	else
		s1 = (length - 1) + start;

	if (len >= 0)
		s2 = s1 + len - 1;
	else
	{
		s2 = (length - 1) + len;
		len = s2 - s1 + 1;
	}

	if ((s1 < 0) || (s1 >= length))
		return EcIndexError( str_o, EcMakeInt( s1 ) );

	if ((s2 < 0) || (s2 >= length))
		return EcIndexError( str_o, EcMakeInt( s2 ) );

	ASSERT(len >= 0);
	buf = alloca( (size_t) (len + 1) );
	if (! buf)
		return EcMemoryError();
	memcpy( buf, str + s1, (size_t) len );
	buf[len] = '\0';

	return EcMakeString( buf, len );
}

static EC_OBJ EcLibString_Range( EC_OBJ stack, EcAny userdata  )
{
	EC_OBJ str_o;
	EC_OBJ start_o, end_o;
	char   *str;
	EcInt   length, start, end;
	EcInt   s1, s2;
	char   *buf;

	EC_CHECKNARGS_F("string.range", 3);
	end_o   = EC_STACKPOP(stack);
	start_o = EC_STACKPOP(stack);
	str_o   = EC_STACKPOP(stack);

	EC_CHECKTYPE_F( "string.sub", 1, str_o,   tc_string );
	EC_CHECKTYPE_F( "string.sub", 2, start_o, tc_inum );
	EC_CHECKTYPE_F( "string.sub", 3, end_o,   tc_inum );
	str    = EC_STRDATA(str_o);
	length = EC_STRLEN(str_o);
	start  = EC_INUM(start_o);
	end    = EC_INUM(end_o);

	if (start >= 0)
		s1 = start;
	else
		s1 = (length - 1) + start;

	if (end >= 0)
		s2 = end;
	else
		s2 = (length - 1) + end;

	if ((s1 < 0) || (s1 >= length))
		return EcIndexError( str_o, EcMakeInt( s1 ) );

	if ((s2 < 0) || (s2 >= length))
		return EcIndexError( str_o, EcMakeInt( s2 ) );

	if (s2 < s1)
		return EC_NIL;

	buf = alloca( (size_t) (s2 - s1 + 1) + 1 );
	if (! buf)
		return EcMemoryError();
	memcpy( buf, str + s1, (size_t) (s2 - s1 + 1) );
	buf[s2 - s1 + 1] = '\0';

	return EcMakeString( buf, (s2 - s1 + 1) );
}

static EC_OBJ EcLibString_LTrim( EC_OBJ stack, EcAny userdata  )
{
	char   *str;
	EcInt   length;
	EcInt   i;
	EC_OBJ  res;

	res = EcParseStackFunction( "string.ltrim", TRUE, stack, "s#", &str, &length );
	if (EC_ERRORP(res))
		return res;

	/* skip initial spaces */
	i = 0;
	while ((i < length) && isspace((int)str[i]))
		i++;
	ASSERT( i <= length );

	if (i < length)
		return EcMakeString( str + i, (length - i) );
	else
		return EcMakeString( "", 0 );
}

static EC_OBJ EcLibString_RTrim( EC_OBJ stack, EcAny userdata  )
{
	char   *str;
	EcInt   length;
	EcInt   j;
	EC_OBJ  res;

	res = EcParseStackFunction( "string.rtrim", TRUE, stack, "s#", &str, &length );
	if (EC_ERRORP(res))
		return res;

	/* skip trailing spaces */
	j = length-1;
	while ((j > 0) && isspace((int)str[j]))
		j--;
	ASSERT( j >= 0 );

	if (j >= 0)
		return EcMakeString( str, (j + 1) );
	else
		return EcMakeString( "", 0 );
}

static EC_OBJ EcLibString_Trim( EC_OBJ stack, EcAny userdata )
{
	char   *str;
	EcInt   length;
	EcInt   i, j;
	EC_OBJ  res;

	res = EcParseStackFunction( "string.trim", TRUE, stack, "s#", &str, &length );
	if (EC_ERRORP(res))
		return res;

	/* skip initial spaces */
	i = 0;
	while ((i < length) && isspace((int)str[i]))
		i++;
	ASSERT( i <= length );

	/* skip trailing spaces */
	j = length-1;
	while ((j > 0) && isspace((int)str[j]))
		j--;
	ASSERT( j >= 0 );

	if (j >= i)
		return EcMakeString( str + i, (j - i + 1) );
	else
		return EcMakeString( "", 0 );
}

static EC_OBJ EcLibString_IsUpper( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ res;
	EcChar ch;

	res = EcParseStackFunction( "string.isupper", TRUE, stack, "c", &ch );
	if (EC_ERRORP(res))
		return res;

	return EcMakeBool( isupper( (int)ch ) );
}

static EC_OBJ EcLibString_IsLower( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ res;
	EcChar ch;

	res = EcParseStackFunction( "string.islower", TRUE, stack, "c", &ch );
	if (EC_ERRORP(res))
		return res;

	return EcMakeBool( islower( (int)ch ) );
}

static EC_OBJ EcLibString_ToUpper( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ res;
	EC_OBJ obj;

	res = EcParseStackFunction( "string.toupper", TRUE, stack, "O", &obj );
	if (EC_ERRORP(res))
		return res;

	if ((! EC_CHARP(obj)) && (! EC_STRINGP(obj)))
	{
		res = EcTypeError( /* target   */ EC_NIL,
						   /* function */ EcGetQVariable( "string.toupper" ),
						   1, tc_none, obj, TRUE, "expected a 'char' or a 'string'" );
		return res;
	}

	if (EC_CHARP(obj))
		return EcMakeChar( (EcChar) toupper( EC_CHAR(obj) ) );
	else
	{
		EcChar *newstr;
		EcInt   i;

		ASSERT( EC_STRINGP(obj) );

		if (EC_STRLEN(obj) == 0)
			return EcMakeString( "", 0 );

		newstr = alloca( (size_t) (EC_STRLEN(obj) + 1) );
		if (! newstr)
			return EcMemoryError();

		for (i = 0; i < EC_STRLEN(obj); i++)
			newstr[i] = (EcChar) toupper( EC_STRDATA(obj)[i] );
		newstr[i] = '\0';

		return EcMakeString( newstr, EC_STRLEN(obj) );
	}
}

static EC_OBJ EcLibString_ToLower( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ res;
	EC_OBJ obj;

	res = EcParseStackFunction( "string.tolower", TRUE, stack, "O", &obj );
	if (EC_ERRORP(res))
		return res;

	if ((! EC_CHARP(obj)) && (! EC_STRINGP(obj)))
	{
		res = EcTypeError( /* target   */ EC_NIL,
						   /* function */ EcGetQVariable( "string.tolower" ),
						   1, tc_none, obj, TRUE, "expected a 'char' or a 'string'" );
		return res;
	}

	if (EC_CHARP(obj))
		return EcMakeChar( (EcChar) tolower( EC_CHAR(obj) ) );
	else
	{
		EcChar *newstr;
		EcInt   i;

		ASSERT( EC_STRINGP(obj) );

		if (EC_STRLEN(obj) == 0)
			return EcMakeString( "", 0 );

		newstr = alloca( (size_t) (EC_STRLEN(obj) + 1) );
		if (! newstr)
			return EcMemoryError();

		for (i = 0; i < EC_STRLEN(obj); i++)
			newstr[i] = (EcChar) tolower( EC_STRDATA(obj)[i] );
		newstr[i] = '\0';

		return EcMakeString( newstr, EC_STRLEN(obj) );
	}
}

static EC_OBJ EcLibString_Char( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ res;
	EcInt  ascii;

	res = EcParseStackFunction( "string.char", TRUE, stack, "i", &ascii );
	if (EC_ERRORP(res))
		return res;

	if ((ascii < 0) ||
		(ascii > 255))
		return EcFalseObject;
	return EcMakeChar( (EcChar) ascii );
}

static EC_OBJ EcLibString_Ascii( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ res;
	EC_OBJ obj;

	res = EcParseStackFunction( "string.ascii", TRUE, stack, "O", &obj );
	if (EC_ERRORP(res))
		return res;

	if ((! EC_CHARP(obj)) && (! EC_STRINGP(obj)))
	{
		res = EcTypeError( /* target   */ EC_NIL,
						   /* function */ EcGetQVariable( "string.ascii" ),
						   1, tc_none, obj, TRUE, "expected a 'char' or a 'string'" );
		return res;
	}

	if (EC_CHARP(obj))
		return EcMakeInt( (char) EC_CHAR(obj) );
	else
	{
		ASSERT( EC_STRINGP(obj) );

		if (EC_STRLEN(obj) < 1)
			return EcFalseObject;

		return EcMakeInt( (char) EC_STRDATA(obj)[0] );
	}
}

static EC_OBJ EcLibString_MakeSymbol( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ  res;
	char   *str;
	EcInt   len;

	res = EcParseStackFunction( "string.makesymbol", TRUE, stack, "s#", &str, &len );
	if (EC_ERRORP(res))
		return res;

	return EcMakeSymbol( str );
}

static EC_OBJ EcLibString_Split( EC_OBJ stack, EcAny userdata )
{
	char   *str;
	EcInt   length;
	char   *sep = NULL;
	EcInt   seplength = 0;
	EcInt   maxfields = -1;
	EcInt   fields, where;
	EC_OBJ  obj;
	EC_OBJ  res;

	res = EcParseStackFunction( "string.split", TRUE, stack, "s#|s#i",
								&str, &length, &sep, &seplength, &maxfields );
	if (EC_ERRORP(res))
		return res;

	res = EcMakeArray( 0 );
	if (EC_ERRORP(res)) return res;

	fields = 0;
	while ((length > 0) && ((maxfields < 0) || (fields < maxfields)))
	{
		if ((maxfields >= 0) &&
			(fields == maxfields - 1))
			where = length;
		else
		{
			if (sep)
			{
				where = strfind( str, length, sep, seplength );
				if (where == NOSPACE)
					return EcMemoryError();
				if (where == NOMATCH)
					where = length;								/* get up to the end */
			} else
			{
				where = findspaces( str, length );
				if (where < 0)
					where = length;								/* get up to the end */

				seplength = skipspaces( str + where, length - where );
				if (seplength >= length)
					return res;
			}
		}

		if (where == 0)
			obj = EcMakeString( "", 0 );
		else
			obj = EcMakeString( str, where );
		if (EC_ERRORP(obj)) return obj;
		/* TODO: correct the following check ! */
		if (EcArrayPush( res, obj ) < 0)
			return EcMemoryError();
		fields++;

		str += (where + seplength); length -= (where + seplength);
	}

	return res;
}

static EC_OBJ EcLibString_Join( EC_OBJ stack, EcAny userdata )
{
	ec_string str;
	EcInt     length;
	EC_OBJ  list;
	char   *sep    = NULL;
	EcInt   seplen = 0;
	EcInt   max    = -1;
	EC_OBJ  res;

	EC_OBJ  obj;
	EcInt   len, i;
	ec_string ds;

	res = EcParseStackFunction( "string.join", TRUE, stack, "O|s#i",
								&list, &sep, &seplen, max );
	if (EC_ERRORP(res))
		return res;

	res = EcSequenceLength( list );
	if (EC_ERRORP(res)) return res;
	len = EC_INUM(res);
	if ((max >= 0) && (max < len))
		len = max;

	if (!sep)
	{
		sep    = " ";
		seplen = 1;
	}

	/* Round 1: calculate string length */
	length = 0;
	for (i = 0; i < len; i++)
	{
		obj = EcSequenceGetElement( list, i );
		if (EC_ERRORP(obj)) return obj;

		if (i != 0)
			length += seplen;

		if (EC_STRINGP(obj))
			length += EC_STRLEN(obj);
		else
		{
			ec_string_init( &ds, NULL );
			length += ec_sprintf( &ds, "%w", obj );
			ec_string_cleanup( &ds );
		}
	}

	if (! ec_string_prepare( &str, length ))
		return EcMemoryError();

	/* Round 2: print */
	for (i = 0; i < len; i++)
	{
		obj = EcSequenceGetElement( list, i );
		if (EC_ERRORP(obj)) return obj;

		if (i != 0)
			ec_strcat( &str, sep, seplen );

		if (EC_STRINGP(obj))
			ec_strcat( &str, EC_STRDATA(obj), EC_STRLEN(obj) );
		else
		{
			ec_string_init( &ds, NULL );
			ec_sprintf( &ds, "%w", obj );
			ec_strcatd( &str, &ds );
			ec_string_cleanup( &ds );
		}
	}

	res = EcMakeString( ec_strdata(&str), ec_strlen(&str) );
	ec_string_cleanup( &str );
	return res;
}

static EC_OBJ EcLibString_Find( EC_OBJ stack, EcAny userdata )
{
	char   *haystack;
	EcInt   hslen;
	char   *needle;
	EcInt   nlen;
	EcInt   where;
	EC_OBJ  res;

	res = EcParseStackFunction( "string.find", TRUE, stack, "s#|s#",
								&haystack, &hslen, &needle, &nlen );
	if (EC_ERRORP(res))
		return res;

	where = strfind( haystack, hslen, needle, nlen );
	if (where == NOSPACE)
		return EcMemoryError();
	if (where == NOMATCH)
		return EcFalseObject;
	return EcMakeInt( where );
}

/* ========================================================================
 * P R I V A T E   F U N C T I O N S
 * ======================================================================== */

/*
 * Knuth-Morris-Pratt algorithm
 *
 * (see Cormen, Leiserson, Rivest, "Introduction to Algorithms", p. 869)
 */
static EcInt *prefix = NULL;

static EcBool kmp_makeprefix( const char *needle, EcInt nlen )
{
	EcInt k;
	EcInt q;

	prefix = ec_malloc( sizeof(EcInt) * nlen );
	if (! prefix) return FALSE;

	prefix[0] = -1;
	k         = -1;
	for (q = 1; q < nlen; q++)
	{
		while ((k >= 0) && (needle[k+1] != needle[q]))
			k = prefix[k];
		if (needle[k+1] == needle[q])
			k++;
		prefix[q] = k;
	}

	return TRUE;
}

static EcInt strfind( const EcChar *haystack, EcInt hslen,
					  const EcChar *needle, EcInt nlen )
{
	EcInt q, i;
	EcInt where = NOMATCH;

	if (! kmp_makeprefix( needle, nlen ))
		return NOSPACE;

	q = -1;
	for (i = 0; i < hslen; i++)
	{
		while ((q >= 0) && (needle[q+1] != haystack[i]))
			q = prefix[q];
		if (needle[q+1] == haystack[i])
			q++;
		if (q == nlen-1)
		{
			where = i - nlen + 1;
			break;
		}
	}

	ec_free( prefix );
	prefix = NULL;

	return where;
}

static EcInt skipspaces( const EcChar *str, EcInt len )
{
	EcInt pos = 0;

	while ((pos < len) && isspace((int) *str++))
		pos++;

	return pos;
}

static EcInt findspaces( const EcChar *str, EcInt len )
{
	EcInt pos = 0;

	while ((pos < len) && !isspace((int) *str++))
		pos++;

	if ((pos >= len) || !isspace((int) *(str-1)))
		return -1;

	return pos;
}

/* Private */

EcBool _ec_modstring_init( void )
{
	EcPackageIntroduce( "string" );

	EcAddPrimitive( "string.length",     EcLibString_Length );
	EcAddPrimitive( "string.sub",        EcLibString_Sub );
	EcAddPrimitive( "string.range",      EcLibString_Range );
	EcAddPrimitive( "string.ltrim",      EcLibString_LTrim );
	EcAddPrimitive( "string.rtrim",      EcLibString_RTrim );
	EcAddPrimitive( "string.trim",       EcLibString_Trim );
	EcAddPrimitive( "string.isupper",    EcLibString_IsUpper );
	EcAddPrimitive( "string.islower",    EcLibString_IsLower );
	EcAddPrimitive( "string.toupper",    EcLibString_ToUpper );
	EcAddPrimitive( "string.tolower",    EcLibString_ToLower );
	EcAddPrimitive( "string.char",       EcLibString_Char );
	EcAddPrimitive( "string.ascii",      EcLibString_Ascii );
	EcAddPrimitive( "string.makesymbol", EcLibString_MakeSymbol );
	EcAddPrimitive( "string.split",      EcLibString_Split );
	EcAddPrimitive( "string.join",       EcLibString_Join );
	EcAddPrimitive( "string.find",       EcLibString_Find );

	return TRUE;
}

void _ec_modstring_cleanup( void )
{
}
