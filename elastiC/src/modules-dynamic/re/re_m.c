/* ==========================================================================
 * re_m.c
 * ==========================================================================
 * Regular expression module.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Wed Nov 24 12:58:41 MET 1999
 *
 *   $Id: re_m.c,v 1.2 2002/05/25 19:37:45 panta Exp $
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
#include "compat.h"

#include "elastic.h"
#include "re_m.h"

#include "pcre/pcre.h"

static EcInt tc_regexp = -1;
static EcInt tc_match  = -1;

/*
 * ovector integer
 *
 * pcre as of 2.08 uses real 'int's, but we'd like 'EcInt's or at least
 * 'long's. I'm too lazy to change the things now, so for the moment
 * we'll resort to a typedef
 */
typedef int ov_int;

static EcUInt s_anchored = 0, s_caseless = 0, s_dollar_endonly = 0,
	s_dotall = 0, s_extended = 0, s_extra = 0, s_multiline = 0, s_ungreedy = 0, s_utf8 = 0;
static EcUInt s_notbol = 0, s_noteol = 0, s_notempty = 0;
static EcUInt s_capturecount = 0, s_backrefmax = 0, s_firstchar = 0;

static EC_OBJ EcReErrorClass = EC_NIL;

struct match_struct
{
	EC_OBJ  regexp;												/* regexp object */
	EC_OBJ  subject;											/* match subject */
	ov_int *ovector;											/* ovector       */
	EcInt   ovecsize;											/* ovector size  */
	EcInt   nsub;												/* number of subpatterns found, or 1 for a global
																   (un-parenthesized) match */
};

/* ========================================================================
 * M A C R O S
 * ======================================================================== */

#define EC_REGEXPP(obj)		(EC_TYPEP(obj,tc_regexp))
#define EC_PCRE(obj)		(EC_USER0(obj, pcre *))
#define EC_PCREXTRA(obj)	(EC_USER1(obj, pcre_extra *))
#define EC_REGEXPSRC(obj)	(EC_USER2(obj, EC_OBJ))

#define EC_PCRE_SET(obj,val)		do { EC_USER0_SET(obj, pcre *, val); } while (0)
#define EC_PCREXTRA_SET(obj,val)	do { EC_USER1_SET(obj, pcre_extra *, val); } while (0)
#define EC_REGEXPSRC_SET(obj,val)	do { EC_USER2_SET(obj, EC_OBJ, val); } while (0)

#define EC_MATCHP(obj)			(EC_TYPEP(obj,tc_match))
#define EC_MATCH(obj)			((struct match_struct *) EC_USER(obj))
#define EC_MATCH_REGEXP(obj)	(EC_MATCH(obj)->regexp)
#define EC_MATCH_SUBJECT(obj)	(EC_MATCH(obj)->subject)
#define EC_MATCH_OVECTOR(obj)	(EC_MATCH(obj)->ovector)
#define EC_MATCH_OVECSIZE(obj)	(EC_MATCH(obj)->ovecsize)
#define EC_MATCH_NSUB(obj)		(EC_MATCH(obj)->nsub)


/* ========================================================================
 * P R O T O T Y P E S
 * ======================================================================== */

/* elastiC API */

static EC_OBJ EcLibRe_Compile( EC_OBJ stack, EcAny userdata );
static EC_OBJ EcLibRe_Study( EC_OBJ stack, EcAny userdata );
static EC_OBJ EcLibRe_Match( EC_OBJ stack, EcAny userdata );
static EC_OBJ EcLibRe_Info( EC_OBJ stack, EcAny userdata );

static EC_OBJ EcLibRe_MatchRe( EC_OBJ stack, EcAny userdata );
static EC_OBJ EcLibRe_MatchSubject( EC_OBJ stack, EcAny userdata );
static EC_OBJ EcLibRe_Number( EC_OBJ stack, EcAny userdata );
static EC_OBJ EcLibRe_Group( EC_OBJ stack, EcAny userdata );
static EC_OBJ EcLibRe_Range( EC_OBJ stack, EcAny userdata );

static EC_OBJ EcLibReErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
static EC_OBJ EcLibReError_GetPosition( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );

static EC_OBJ EcReError( const char *message, EcInt position );

/* private */

static EC_OBJ options2obj( int options );
static EC_OBJ obj2options( const char *fname, EcInt pindex, EC_OBJ obj, int *options );

static EC_OBJ make_regexp( pcre *code, pcre_extra *extra, const char *pattern, EcInt plen, int options );
static EC_OBJ make_match( EC_OBJ regexp, EC_OBJ subject, ov_int *ovector, EcInt ovecsize, EcInt nsub );

static EC_OBJ regexp_copy( EC_OBJ obj, EcCopyType type );
static void   regexp_mark( EC_OBJ obj );
static void   regexp_free( EC_OBJ obj );
static EcInt  regexp_print( ec_string *str, EC_OBJ obj, EcBool detailed );
static EcUInt regexp_hash( EC_OBJ obj, EcInt recur );
static EcBool regexp_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res );

static EC_OBJ match_copy( EC_OBJ obj, EcCopyType type );
static void   match_mark( EC_OBJ obj );
static void   match_free( EC_OBJ obj );
static EcInt  match_print( ec_string *str, EC_OBJ obj, EcBool detailed );
static EcUInt match_hash( EC_OBJ obj, EcInt recur );
static EcBool match_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res );

/* ------------------------------------------------------------------------
 * ReError
 * ------------------------------------------------------------------------ */

static EcClassDef EcReErrorClassDef =
{
	"ReError",													/* class name         */
	"re",														/* package name       */

	EC_NIL,														/* super class        */

	NULL,														/* class methods      */
	NULL,														/* class variables    */

	NULL,														/* instance methods   */
	NULL														/* instance variables */
};

static EcCMethodDef EcReErrorClass_ClassMethods[] =			/* Class Methods   */
{
	{ "new",			EcLibReErrorClass_New    },
	{ NULL,             NULL                     }
};

static EcCMethodDef EcReErrorClass_Methods[] =				/* Instance Methods   */
{
	{ "getPosition",	EcLibReError_GetPosition },
	{ NULL,				NULL                     }
};

static EcVariableDef EcReErrorClass_Variables[] =			/* Instance Variables */
{
	{ "position"                                 },
	{ NULL                                       }
};

/* ========================================================================
 * I M P L E M E N T A T I O N
 * ------------------------------------------------------------------------ */

/* C API */

static EC_OBJ EcLibRe_Compile( EC_OBJ stack, EcAny userdata )
{
	const char *pattern;
	EcInt       patlen;
	EC_OBJ      opts = EC_NIL;
	int         options;
	EcBool      study = FALSE;
	pcre       *code;
	pcre_extra *extra = NULL;
	const char *errptr  = NULL;
	int         erroffs = 0;
	EC_OBJ      res;

	res = EcParseStackFunction( "re.compile", TRUE, stack, "s#|Ob", &pattern, &patlen, &opts, &study );
	if (EC_ERRORP(res)) return res;

	opts = obj2options( "re.compile", 2, opts, &options );
	if (EC_ERRORP(opts)) return opts;

	code = pcre_compile( pattern, options,
						 &errptr, &erroffs,
						 /* tableptr */ NULL );
	if (! code) return EcReError( errptr, erroffs );

	if (study)
	{
		extra = pcre_study( code,
							0,									/* currently pcre has no options for study */
							&errptr );
		if (! extra) return EcReError( errptr, -1 );
	}

	return make_regexp( code, extra, pattern, patlen, options );
}

static EC_OBJ EcLibRe_Study( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ      obj;
	pcre_extra *extra;
	const char *errptr  = NULL;
	EC_OBJ      res;

	res = EcParseStackFunction( "re.study", TRUE, stack, "O!", tc_regexp, &obj );
	if (EC_ERRORP(res)) return res;

	if (EC_PCREXTRA(obj)) return obj;

	extra = pcre_study( EC_PCRE(obj),
						0,									/* currently pcre has no options for study */
						&errptr );
	if (! extra) return EcReError( errptr, -1 );
	EC_PCREXTRA_SET(obj, extra);
	return obj;
}

static EC_OBJ EcLibRe_Match( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ      regexp;
	EC_OBJ      subject_obj;
	const char *subject;
	EcInt       length;
	EcInt       startoffset = 0;
	EC_OBJ      opts    = EC_NIL;
	int         options;
	pcre       *code  = NULL;
	pcre_extra *extra = NULL;
	EcBool      compiledhere = FALSE;
	ov_int     *ovector;
	EcInt       ovecsize;
	EcInt       ncapsub;
	int         exec_res;
	int         rc;
	EC_OBJ      res;

	res = EcParseStackFunction( "re.match", TRUE, stack, "OO!|iO",
								&regexp, tc_string, &subject_obj, &startoffset, opts );
	if (EC_ERRORP(res)) return res;

	if (!(EC_REGEXPP(regexp) || (EC_STRINGP(regexp))))
	{
		return EC_TYPEERROR_F( "re.match", 1, tc_none, regexp, "expected 'regexp' or 'string'" );
	}

	if (EC_STRINGP(regexp))
	{
		/* compile the pattern */

		const char *errptr  = NULL;
		int         erroffs = 0;

		code = pcre_compile( EC_STRDATA(regexp), 0,				/* we'll use options at matching time */
							 &errptr, &erroffs,
							 /* tableptr */ NULL );
		if (! code) return EcReError( errptr, erroffs );
		extra = NULL;
		compiledhere = TRUE;
	} else
	{
		code  = EC_PCRE(regexp);
		extra = EC_PCREXTRA(regexp);
		compiledhere = FALSE;
	}

	subject = EC_STRDATA(subject_obj);
	length  = EC_STRLEN(subject_obj);

	opts = obj2options( "re.match", 4, opts, &options );
	if (EC_ERRORP(opts)) return opts;

	rc = pcre_fullinfo( code, extra, PCRE_INFO_CAPTURECOUNT, &ncapsub );
	if (rc < 0)
		return EcReError( "internal error: pcre_fullinfo", -1 );

	ovecsize = (ncapsub + 1) * 3;
	ovector  = ec_malloc( sizeof(ov_int) * ovecsize );
	if (! ovector) return EcMemoryError();
	memset( ovector, 0x00, sizeof(ov_int) * ovecsize );

	exec_res = pcre_exec( code, extra,
						  subject, length, startoffset,
						  options, ovector, ovecsize );
	if (compiledhere)
	{
		if (code)  free( code );
		if (extra) free( extra );
	}
	switch (exec_res)
	{
	case PCRE_ERROR_NOMATCH:
		ec_free( ovector );
		return EC_NIL;
		break;
	case PCRE_ERROR_NULL:
		ec_free( ovector );
		return EcReError( "internal error: PCRE_ERROR_NULL", -1 );
		break;
	case PCRE_ERROR_BADOPTION:
		ec_free( ovector );
		return EcReError( "internal error: PCRE_ERROR_BADOPTION", -1 );
		break;
	case PCRE_ERROR_BADMAGIC:
		ec_free( ovector );
		return EcReError( "internal error: PCRE_ERROR_BADMAGIC", -1 );
		break;
	case PCRE_ERROR_UNKNOWN_NODE:
		ec_free( ovector );
		return EcReError( "internal error: PCRE_ERROR_UNKNOWN_MODE", -1 );
		break;
	case PCRE_ERROR_NOMEMORY:
		ec_free( ovector );
		return EcMemoryError();
		break;
	}
#if 0
	if (exec_res == 0)
	{
		ec_free( ovector );
		return EC_NIL;
		}
#endif
	ASSERT( exec_res >= 0 );
	return make_match( regexp, subject_obj, ovector, ovecsize, exec_res );
}

static EC_OBJ EcLibRe_Info( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ regexp;
	EcUInt infosym;
	EC_OBJ res;

	pcre       *code;
	pcre_extra *extra;
	int         rc;

	res = EcParseStackFunction( "re.info", TRUE, stack, "O!k", tc_regexp, &regexp, &infosym );
	if (EC_ERRORP(res)) return res;

	code  = EC_PCRE(regexp);
	extra = EC_PCREXTRA(regexp);

	if (infosym == s_capturecount)
	{
		int capturecount;

		rc = pcre_fullinfo( code, extra, PCRE_INFO_CAPTURECOUNT, &capturecount );
		if (rc < 0)
			return EcReError( "internal error: pcre_fullinfo", -1 );

		return EcMakeInt( capturecount );
	} else if (infosym == s_backrefmax)
	{
		int backrefmax;

		rc = pcre_fullinfo( code, extra, PCRE_INFO_BACKREFMAX, &backrefmax );
		if (rc < 0)
			return EcReError( "internal error: pcre_fullinfo", -1 );

		return EcMakeInt( backrefmax );
	} else if (infosym == s_firstchar)
	{
		int firstchar = 0;

		rc = pcre_fullinfo( code, extra, PCRE_INFO_FIRSTCHAR, &firstchar );
		if (rc < 0)
			return EcReError( "internal error: pcre_fullinfo", -1 );

		return EcMakeInt( firstchar );
	} else
	{
		EC_TYPEERROR_F( "re.info", 2, tc_symbol,
					    EcMakeSymbolFromId( infosym ), "expected an info option symbol" );
		return Ec_ERROR;
	}
}

static EC_OBJ EcLibRe_MatchRe( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ match;
	EC_OBJ res;

	res = EcParseStackFunction( "re.match_re", TRUE, stack, "O!",
								tc_match, &match );
	if (EC_ERRORP(res)) return res;

	return EC_MATCH_REGEXP(match);
}

static EC_OBJ EcLibRe_MatchSubject( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ match;
	EC_OBJ res;

	res = EcParseStackFunction( "re.match_subject", TRUE, stack, "O!",
								tc_match, &match );
	if (EC_ERRORP(res)) return res;

	return EC_MATCH_SUBJECT(match);
}

static EC_OBJ EcLibRe_Number( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ match;
	EC_OBJ res;

	res = EcParseStackFunction( "re.number", TRUE, stack, "O!",
								tc_match, &match );
	if (EC_ERRORP(res)) return res;

#if 0
	ovecsize = EC_MATCH_OVECSIZE(match);
	num = (ovecsize / 3) - 1;
#endif
	return EcMakeInt( EC_MATCH_NSUB(match) );
}

static EC_OBJ EcLibRe_Group( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ      match;
	EcInt       igroup;
	EC_OBJ      subject_obj;
	ov_int     *ovector;
	EcInt       ovecsize;
	const char *subject;
	EcInt       length;
	EcInt       dim;
	EcInt       l_i, r_i;
	EC_OBJ      res;

	res = EcParseStackFunction( "re.group", TRUE, stack, "O!i",
								tc_match, &match, &igroup );
	if (EC_ERRORP(res)) return res;

	ovector  = EC_MATCH_OVECTOR(match);
	ovecsize = EC_MATCH_OVECSIZE(match);
	subject_obj = EC_MATCH_SUBJECT(match);
	subject = EC_STRDATA(subject_obj);
	length  = EC_STRLEN(subject_obj);

	dim = ovecsize / 3;
	if ((igroup >= dim) || (igroup > EC_MATCH_NSUB(match)))
		return EcReError( "no substring with given index", -1 );

	l_i = ovector[igroup*2];
	r_i = ovector[igroup*2 + 1];
	if ((l_i == -1) && (r_i == -1))
		return EcMakeString( "", 0 );
	ASSERT( (l_i >= 0) && (r_i >= 0) );
	if (r_i - 1 <= l_i)
		return EcMakeString( "", 0 );
	return EcMakeString( subject + l_i, (r_i - l_i) );
}

static EC_OBJ EcLibRe_Range( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ      match;
	EcInt       igroup;
	EC_OBJ      subject_obj;
	ov_int     *ovector;
	EcInt       ovecsize;
	const char *subject;
	EcInt       length;
	EcInt       dim;
	EcInt       l_i, r_i;
	EC_OBJ      res;

	res = EcParseStackFunction( "re.range", TRUE, stack, "O!i",
								tc_match, &match, &igroup );
	if (EC_ERRORP(res)) return res;

	ovector  = EC_MATCH_OVECTOR(match);
	ovecsize = EC_MATCH_OVECSIZE(match);
	subject_obj = EC_MATCH_SUBJECT(match);
	subject = EC_STRDATA(subject_obj);
	length  = EC_STRLEN(subject_obj);

	dim = ovecsize / 3;
	if ((igroup >= dim) || (igroup > EC_MATCH_NSUB(match)))
		return EcReError( "no substring with given index", -1 );

	l_i = ovector[igroup*2];
	r_i = ovector[igroup*2 + 1];
	if ((l_i == -1) && (r_i == -1))
		return EC_NIL;
	ASSERT( (l_i >= 0) && (r_i >= 0) );

#if 0 /* Old style */
	return EcMakeArrayInit( 2, EcMakeInt( l_i ), EcMakeInt( r_i - l_i )  );
#endif
	return EcMakeArrayInit( 2, EcMakeInt( l_i ), EcMakeInt( r_i - 1 )  );
}

static EC_OBJ EcLibReErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ obj;
	EC_OBJ msg, position;

	EC_CHECKNARGS_CM( self, "new", 2 );

	position = EC_STACKPOP( stack );
	msg      = EC_STACKPOP( stack );

	/*
	 * This is a class method, so self is the class
	 */
	EC_STACKNARGS_REAL(stack) = EC_STACKNARGS_LOGICAL(stack) = 1;
	EC_STACKPUSH( stack, msg );
	obj = EcLibExceptionClass_NewWith( self, in_class, stack );
	EcSetInstanceVariable( obj, EcReErrorClass,
						   "position",
						   position );

	return obj;
}

static EC_OBJ EcLibReError_GetPosition( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getPosition", 0 );

	ASSERT( in_class == EcReErrorClass );
	res = EcGetInstanceVariable( self,
								 EcReErrorClass,
								 "position" );
	return res;
}

static EC_OBJ EcReError( const char *message, EcInt position )
{
	EcMakeExceptionWithArgs( EcReErrorClass, message, 1,
							 EcMakeInt( position ) );
	return Ec_ERROR;
}



/* private */

static EC_OBJ options2obj( int options )
{
	EC_OBJ res, obj;

	if (options == 0)
		return EC_NIL;

	res = EcMakeArray( /* minimum size */ 1 );
	if (EC_ERRORP(res)) return res;

	if (options & PCRE_ANCHORED)
	{
		obj = EcArraySet( res, EC_ARRAYLEN(res), EcMakeSymbolFromId( s_anchored ) );
		if (EC_ERRORP(obj)) return obj;
	}

	if (options & PCRE_CASELESS)
	{
		obj = EcArraySet( res, EC_ARRAYLEN(res), EcMakeSymbolFromId( s_caseless ) );
		if (EC_ERRORP(obj)) return obj;
	}

	if (options & PCRE_DOLLAR_ENDONLY)
	{
		obj = EcArraySet( res, EC_ARRAYLEN(res), EcMakeSymbolFromId( s_dollar_endonly ) );
		if (EC_ERRORP(obj)) return obj;
	}

	if (options & PCRE_DOTALL)
	{
		obj = EcArraySet( res, EC_ARRAYLEN(res), EcMakeSymbolFromId( s_dotall ) );
		if (EC_ERRORP(obj)) return obj;
	}

	if (options & PCRE_EXTENDED)
	{
		obj = EcArraySet( res, EC_ARRAYLEN(res), EcMakeSymbolFromId( s_extended ) );
		if (EC_ERRORP(obj)) return obj;
	}

	if (options & PCRE_EXTRA)
	{
		obj = EcArraySet( res, EC_ARRAYLEN(res), EcMakeSymbolFromId( s_extra ) );
		if (EC_ERRORP(obj)) return obj;
	}

	if (options & PCRE_MULTILINE)
	{
		obj = EcArraySet( res, EC_ARRAYLEN(res), EcMakeSymbolFromId( s_multiline ) );
		if (EC_ERRORP(obj)) return obj;
	}

	if (options & PCRE_UNGREEDY)
	{
		obj = EcArraySet( res, EC_ARRAYLEN(res), EcMakeSymbolFromId( s_ungreedy ) );
		if (EC_ERRORP(obj)) return obj;
	}

	if (options & PCRE_UTF8)
	{
		obj = EcArraySet( res, EC_ARRAYLEN(res), EcMakeSymbolFromId( s_utf8 ) );
		if (EC_ERRORP(obj)) return obj;
	}

	if (options & PCRE_NOTBOL)
	{
		obj = EcArraySet( res, EC_ARRAYLEN(res), EcMakeSymbolFromId( s_notbol ) );
		if (EC_ERRORP(obj)) return obj;
	}

	if (options & PCRE_NOTEOL)
	{
		obj = EcArraySet( res, EC_ARRAYLEN(res), EcMakeSymbolFromId( s_noteol ) );
		if (EC_ERRORP(obj)) return obj;
	}

	if (options & PCRE_NOTEMPTY)
	{
		obj = EcArraySet( res, EC_ARRAYLEN(res), EcMakeSymbolFromId( s_notempty ) );
		if (EC_ERRORP(obj)) return obj;
	}

	return res;
}

static EC_OBJ obj2options( const char *fname, EcInt pindex, EC_OBJ obj, int *options )
{
	EcInt  i;
	EC_OBJ el;

	ASSERT( options );

	*options = 0;

	if (EC_NULLP(obj))
		return EC_NIL;

#define TYPE_ERROR	\
EC_TYPEERROR_F(/* function name    */ fname,                                         \
			   /* parameter index  */ pindex,                                        \
			   /* expected         */ tc_none,                                       \
			   /* offending object */ obj,                                           \
			   /* reason           */ "expected an array of option symbols or @nil")

	if (! EC_ARRAYP(obj))
		return TYPE_ERROR;

	for (i = 0; i < EC_ARRAYLEN(obj); i++)
	{
		el = EcArrayGet( obj, i );
		if (EC_ERRORP(el)) return el;

		if (EC_SYMBOLP(el))
		{
			if (EC_SYMBOL(el) == s_anchored)
				*options |= PCRE_ANCHORED;
			else if (EC_SYMBOL(el) == s_caseless)
				*options |= PCRE_CASELESS;
			else if (EC_SYMBOL(el) == s_dollar_endonly)
				*options |= PCRE_DOLLAR_ENDONLY;
			else if (EC_SYMBOL(el) == s_dotall)
				*options |= PCRE_DOTALL;
			else if (EC_SYMBOL(el) == s_extended)
				*options |= PCRE_EXTENDED;
			else if (EC_SYMBOL(el) == s_extra)
				*options |= PCRE_EXTRA;
			else if (EC_SYMBOL(el) == s_multiline)
				*options |= PCRE_MULTILINE;
			else if (EC_SYMBOL(el) == s_ungreedy)
				*options |= PCRE_UNGREEDY;
			else if (EC_SYMBOL(el) == s_utf8)
				*options |= PCRE_UTF8;
			else if (EC_SYMBOL(el) == s_notbol)
				*options |= PCRE_NOTBOL;
			else if (EC_SYMBOL(el) == s_noteol)
				*options |= PCRE_NOTBOL;
			else if (EC_SYMBOL(el) == s_notempty)
				*options |= PCRE_NOTBOL;
		} else
			return TYPE_ERROR;
	}

	/* ok */
	return EC_NIL;
}

static EC_OBJ make_regexp( pcre *code, pcre_extra *extra, const char *pattern, EcInt plen, int options )
{
	EC_OBJ obj, str, opts, src;
	EC_OBJ res;

	obj = EcMakeUser( tc_regexp, code );
	if (EC_ERRORP(obj)) return obj;

	str = EcMakeString( pattern, plen );
	if (EC_ERRORP(str)) return str;

	opts = EcMakeInt( options );
	if (EC_ERRORP(opts)) return opts;

	src = EcMakeArray( 2 );
	res = EcArraySet( src, 0, str );
	if (EC_ERRORP(res)) return res;
	res = EcArraySet( src, 1, opts );
	if (EC_ERRORP(res)) return res;

	EC_PCRE_SET(obj,      code);
	EC_PCREXTRA_SET(obj,  extra);
	EC_REGEXPSRC_SET(obj, src);
	return obj;
}

static EC_OBJ make_match( EC_OBJ regexp, EC_OBJ subject, ov_int *ovector, EcInt ovecsize, EcInt nsub )
{
	EC_OBJ obj;
	struct match_struct *match_s;

	match_s = (struct match_struct *) ec_malloc( sizeof(struct match_struct) );
	if (! match_s) return EcMemoryError();

	match_s->regexp   = regexp;
	match_s->subject  = subject;
	match_s->ovector  = ovector;
	match_s->ovecsize = ovecsize;
	match_s->nsub     = nsub;

	obj = EcMakeUser( tc_match, match_s );
	if (EC_ERRORP(obj)) return obj;

	return obj;
}

static EC_OBJ regexp_copy( EC_OBJ obj, EcCopyType type )
{
	pcre       *code;
	pcre_extra *extra;
	EC_OBJ      src, sobj;
	EC_OBJ      res;

	const char *old_str;
	EcInt       old_slen;
	int         old_options;

	const char *errptr  = NULL;
	int         erroffs = 0;

	/* compile a new regexp with the same string and options of the old one */
	src = EC_REGEXPSRC(obj);
	ASSERT( EC_ARRAYP(src) );
	ASSERT( EC_ARRAYLEN(src) == 2 );
	sobj = EcArrayGet( src, 0 );
	res  = EcArrayGet( src, 1 );
	ASSERT( EC_STRINGP(sobj) );
	ASSERT( EC_INUMP(res) );

	old_str     = EC_STRDATA(sobj);
	old_slen    = EC_STRLEN(sobj);
	old_options = EC_INUM(res);

	code = pcre_compile( old_str, old_options,
						 &errptr, &erroffs,
						 /* tableptr */ NULL );
	if (! code) return EcReError( errptr, erroffs );

	extra = NULL;
	if (EC_PCREXTRA(obj))
	{
		extra = pcre_study( code,
							0,									/* currently pcre has no options for study */
							&errptr );
		if (! extra) return EcReError( errptr, -1 );
	}

	return make_regexp( code, extra,
						old_str,
						old_slen,
						old_options );
}

static void regexp_mark( EC_OBJ obj )
{
	if (EC_NNULLP(EC_REGEXPSRC(obj)))
		EcMarkObject( EC_REGEXPSRC(obj) );
}

static void regexp_free( EC_OBJ obj )
{
	if (EC_PCREXTRA(obj))
		free( EC_PCREXTRA(obj) );
	EC_PCREXTRA_SET(obj, NULL);
	if (EC_PCRE(obj))
		free( EC_PCRE(obj) );
	EC_PCRE_SET(obj, NULL);
}

static EcInt regexp_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	EC_OBJ src, pat, opts;

	src  = EC_REGEXPSRC(obj);
	ASSERT( EC_ARRAYP(src) );
	ASSERT( EC_ARRAYLEN(src) == 2 );
	pat  = EcArrayGet( src, 0 );
	opts = EcArrayGet( src, 1 );
	ASSERT( EC_STRINGP(pat) );
	ASSERT( EC_INUMP(opts) );
	opts = options2obj( EC_INUM(opts) );
	if (EC_ERRORP(opts))
	{
		/* ARGH ! */
		ASSERT( FALSE );
		return 0;
	}

	return ec_sprintf( str, "<regexp %W, %w>",
					   pat, opts );
}

static EcUInt regexp_hash( EC_OBJ obj, EcInt recur )
{
	return 1;
}

static EcBool regexp_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res )
{
	EC_OBJ src, pat, opts;
	EC_OBJ pat1, pat2;
	int    opt1, opt2;

	src = EC_REGEXPSRC(obj1);
	ASSERT( EC_ARRAYP(src) );
	ASSERT( EC_ARRAYLEN(src) == 2 );
	pat  = EcArrayGet( src, 0 );
	opts = EcArrayGet( src, 1 );
	ASSERT( EC_STRINGP(pat) );
	ASSERT( EC_INUMP(opts) );
	pat1 = pat;
	opt1 = EC_INUM(opts);

	src = EC_REGEXPSRC(obj2);
	ASSERT( EC_ARRAYP(src) );
	ASSERT( EC_ARRAYLEN(src) == 2 );
	pat  = EcArrayGet( src, 0 );
	opts = EcArrayGet( src, 1 );
	ASSERT( EC_STRINGP(pat) );
	ASSERT( EC_INUMP(opts) );
	pat2 = pat;
	opt2 = EC_INUM(opts);

	if (! EcObjectCompare( pat1, pat2, res ))
		return FALSE;

	if (*res != 0)
		return FALSE;											/* strings not equal => different regexps */

	if (opt1 != opt2)
		return FALSE;

	*res = 0;
	return TRUE;
}

static EC_OBJ match_copy( EC_OBJ obj, EcCopyType type )
{
	ov_int *ovector;
	EcInt   ovecsize;
	EC_OBJ  regexp_copy, subject_copy;

	if (type == EcShallowCopyType)
	{
		regexp_copy  = EC_MATCH_REGEXP(obj);
		subject_copy = EC_MATCH_SUBJECT(obj);
	}
	else
	{
		regexp_copy = EcCopy( EC_MATCH_REGEXP(obj), EcDeepCopyType );
		if (EC_ERRORP(regexp_copy)) return regexp_copy;
		subject_copy = EcCopy( EC_MATCH_SUBJECT(obj), EcDeepCopyType );
		if (EC_ERRORP(subject_copy)) return subject_copy;
	}

	/* copy the ovector */
	ovecsize = EC_MATCH_OVECSIZE(obj);
	ovector = ec_malloc( sizeof(ov_int) * ovecsize );
	if (! ovector)
		return EcMemoryError();
	memcpy( ovector, EC_MATCH_OVECTOR(obj), sizeof(ov_int) * ovecsize );

	return make_match( regexp_copy, subject_copy, ovector, ovecsize, EC_MATCH_NSUB(obj) );
}

static void match_mark( EC_OBJ obj )
{
	if (EC_NNULLP(EC_MATCH_REGEXP(obj)))
		EcMarkObject( EC_MATCH_REGEXP(obj) );
	if (EC_NNULLP(EC_MATCH_SUBJECT(obj)))
		EcMarkObject( EC_MATCH_SUBJECT(obj) );
}

static void match_free( EC_OBJ obj )
{
	if (! EC_MATCH(obj)) return;

	EC_MATCH_NSUB(obj) = 0;
	if (EC_MATCH_OVECTOR(obj))
		ec_free( EC_MATCH_OVECTOR(obj) );
	EC_MATCH_OVECTOR(obj) = NULL;
	EC_MATCH_OVECSIZE(obj) = 0;
	EC_MATCH_SUBJECT(obj) = EC_NIL;
	EC_MATCH_REGEXP(obj)  = EC_NIL;

	ec_free( EC_MATCH(obj) );
	EC_MATCH(obj) = NULL;
}

static EcInt match_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	EcInt nsub;

	nsub = EC_MATCH_NSUB(obj);
	return ec_sprintf( str, "<match groups: %I subject:%W regexp:%W>",
					   nsub, EC_MATCH_SUBJECT(obj), EC_MATCH_REGEXP(obj) );
}

static EcUInt match_hash( EC_OBJ obj, EcInt recur )
{
	return EC_MATCH_OVECSIZE(obj) + EC_MATCH_NSUB(obj);
}

static EcBool match_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res )
{
#if 0
	/*
	 * Since matches have a meaning when considered only in the context:
	 *     (pattern, match, subject)
	 * and here we know only two components (match and subject), we effectively
	 * can't compare matches.
	 */
	return FALSE;
#endif
	EcInt i;

	if (EC_MATCH_OVECSIZE(obj1) != EC_MATCH_OVECSIZE(obj2))
		return FALSE;
	if (EC_MATCH_NSUB(obj1) != EC_MATCH_NSUB(obj2))
		return FALSE;
	if (! EcObjectEqual( EC_MATCH_REGEXP(obj1), EC_MATCH_REGEXP(obj2) ))
		return FALSE;
	if (! EcObjectEqual( EC_MATCH_SUBJECT(obj1), EC_MATCH_SUBJECT(obj2) ))
		return FALSE;

	for (i = 0; i < EC_MATCH_OVECSIZE(obj1); i++)
		if (EC_MATCH_OVECTOR(obj1)[i] != EC_MATCH_OVECTOR(obj2)[i])
			return FALSE;

	*res = 0;
	return TRUE;
}

/* Module initialization & cleanup */

EC_RE_MODULE_API EC_OBJ ec_re_init( void )
{
	EcTypespec regexpspec = {
		/* type    */		0,
		/* name    */		"regexp",

		/* copy    */		regexp_copy,

		/* mark    */		regexp_mark,
		/* free    */		regexp_free,
		/* print   */		regexp_print,

		/* hash    */		regexp_hash,
		/* compare */		regexp_compare,

		/* check   */		NULL,

		/* sequence */      NULL,
		/* numeric  */      NULL
	};
	EcTypespec matchspec = {
		/* type    */		0,
		/* name    */		"match",

		/* copy    */		match_copy,

		/* mark    */		match_mark,
		/* free    */		match_free,
		/* print   */		match_print,

		/* hash    */		match_hash,
		/* compare */		match_compare,

		/* check   */		NULL,

		/* sequence */      NULL,
		/* numeric  */      NULL
	};
	EC_OBJ pkg;

	tc_regexp = EcAddType( &regexpspec );
	if (tc_regexp == 0)
		return EC_NIL;
	tc_match = EcAddType( &matchspec );
	if (tc_match == 0)
		return EC_NIL;

	pkg = EcPackageIntroduce( "re" );

	EcAddPrimitive( "re.compile", EcLibRe_Compile );
	EcAddPrimitive( "re.study",   EcLibRe_Study );
	EcAddPrimitive( "re.match",   EcLibRe_Match );
	EcAddPrimitive( "re.info",    EcLibRe_Info );

	EcAddPrimitive( "re.match_re",        EcLibRe_MatchRe );
	EcAddPrimitive( "re.match_subject",   EcLibRe_MatchSubject );
	EcAddPrimitive( "re.number",          EcLibRe_Number );
	EcAddPrimitive( "re.group",           EcLibRe_Group );
	EcAddPrimitive( "re.range",           EcLibRe_Range );

	/* Symbols */
	s_anchored       = EcInternSymbol( "anchored" );
	s_caseless       = EcInternSymbol( "caseless" );
	s_dollar_endonly = EcInternSymbol( "dollar-endonly" );
	s_dotall         = EcInternSymbol( "dotall" );
	s_extended       = EcInternSymbol( "extended" );
	s_extra          = EcInternSymbol( "extra" );
	s_multiline      = EcInternSymbol( "multiline" );
	s_ungreedy       = EcInternSymbol( "ungreedy" );
	s_utf8           = EcInternSymbol( "utf8" );
	s_notbol         = EcInternSymbol( "notbol" );
	s_noteol         = EcInternSymbol( "noteol" );
	s_notempty       = EcInternSymbol( "notempty" );

	s_capturecount   = EcInternSymbol( "capture-count" );
	s_backrefmax     = EcInternSymbol( "backref-max" );
	s_firstchar      = EcInternSymbol( "first-char" );

	/* ReError exception */

	EcReErrorClassDef.superclass     = EcExceptionClass;
	EcReErrorClassDef.classmethods   = EcReErrorClass_ClassMethods;
	EcReErrorClassDef.classvariables = NULL;
	EcReErrorClassDef.methods        = EcReErrorClass_Methods;
	EcReErrorClassDef.variables      = EcReErrorClass_Variables;

	EcReErrorClass = EcAddClassDef( &EcReErrorClassDef );
	EcRegisterBuiltin( "re.ReError", EcReErrorClass );

	/*
	 * Save global forever-living objects from GC
	 */
	EcGCProtect( EcReErrorClass );

	return pkg;
}

EC_RE_MODULE_API void ec_re_cleanup( void )
{
}
