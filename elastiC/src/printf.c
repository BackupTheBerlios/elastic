/* ==========================================================================
 * printf.c
 * ==========================================================================
 * printf-like functions.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat Dec 12 11:25:51 MET 1998
 *
 *   $Id: printf.c,v 1.3 2002/06/13 17:44:18 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1998-2002 Marco Pantaleoni. All rights reserved.
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

#include "strtable.h"
#include "memory.h"

#include "private.h"

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_STDIO_H
#include <stdio.h>
#endif
#if HAVE_STDARG_H
#include <stdarg.h>
#endif
#if HAVE_CTYPE_H
#include <ctype.h>
#endif


EC_API EcInt EcPrintObject( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	EcInt res = 0;

	ASSERT( str );

	/* TODO: handle errors in ec_sprintf */

	if (EC_NULLP(obj))
	{
		res = ec_sprintf( str, "@nil" );
		return res;
	}

	switch (EC_TYPE(obj))
	{
	case tc_none:
		res = ec_sprintf( str, "<NONE>" );
		break;

	case tc_undefined:
		res = ec_sprintf( str, "@undefined" );
		break;

	case tc_error:
		res = ec_sprintf( str, "<ERROR>" );
		break;

	case tc_bool:
		res = ec_sprintf( str, "%s", EC_BOOLVAL(obj) ? "@true" : "@false" );
		break;

	case tc_inum:
		res = ec_sprintf( str, "%ld", (long)EC_INUM(obj) );
		break;

	case tc_fnum:
		res = ec_sprintf( str, "%g", EC_FNUM(obj) );
		break;

	case tc_symbol:
		res = ec_sprintf( str, "#%s", EcSymbolAt( EC_SYMBOL(obj) ) );
		break;

	case tc_primitive:
		res = ec_sprintf( str, "<c-primitive %s>", EC_PRIMITIVENAME( obj ) );
		break;

	case tc_cmethod:
		res = ec_sprintf( str, "<c-method %s>", EC_CMETHODNAME( obj ) );
		break;

	default:
		if (EC_USERP(obj))
		{
			res = USERTYPE(EC_TYPE(obj)).print_fcn( str, obj, detailed );
		}
		break;
	}

	return res;
}

#if defined(WITH_STDIO)
EC_API EcInt ec_fprintf( FILE *fh, const char *format, ... )
{
	va_list args;
	EcInt i;

	va_start( args, format );
	i = ec_vfprintf( fh, format, args );
	va_end( args );
	return i;
}

EC_API EcInt ec_vfprintf( FILE *fh, const char *format, va_list ap )
{
	EcInt res;
	ec_string ds;

	ec_string_init( &ds, NULL );
	res = ec_vsprintf( &ds, format, ap );
	if (fwrite( ec_strdata( &ds ), 1, res, fh ) < res)
	{
		ec_string_cleanup( &ds );
		return -1;
	}
	ec_string_cleanup( &ds );
	return res;
}
#endif /* end of defined(WITH_STDIO) */

EC_API EcInt ec_asprintf(  char **sres, const char *format, ... )
{
	va_list args;
	EcInt i;

	va_start( args, format );
	i = ec_vasprintf( sres, format, args );
	va_end( args );
	return i;
}

EC_API EcInt ec_vasprintf( char **sres, const char *format, va_list ap )
{
	EcInt res;
	ec_string ds;

	ec_string_init( &ds, NULL );
	res = ec_vsprintf( &ds, format, ap );
	*sres = ec_malloc( ec_strlen( &ds ) );
	if (! *sres)
	{
		ec_string_cleanup( &ds );
		return -1;
	}
	memcpy( *sres, ec_strdata( &ds ), ec_strlen( &ds ) );
	ec_string_cleanup( &ds );
	return res;
}

EC_API EC_OBJ ec_oprintf( const char *format, ... )
{
	va_list args;
	EC_OBJ  res;

	va_start( args, format );
	res = ec_voprintf( format, args );
	va_end( args );
	return res;
}

EC_API EC_OBJ ec_voprintf( const char *format, va_list ap )
{
	EC_OBJ res;
	EcInt  n;
	ec_string ds;

	ec_string_init( &ds, NULL );
	n = ec_vsprintf( &ds, format, ap );
	res = EcMakeString( ec_strdata( &ds ), ec_strlen( &ds ) );
	ec_string_cleanup( &ds );

	return res;
}

EC_API EcInt ec_msg_printf ( const char *format, ... )
{
	/* for error msgs: use streams if available, otherwise stdio (if avail.) */

	va_list args;
	EcInt i;

	va_start( args, format );
	i = ec_msg_vprintf( format, args );
	va_end( args );
	return i;
}

#if HAVE_FFLUSH
#define FFLUSHERR()		do { fflush(stderr); } while(0)
#else
#define FFLUSHERR()		do { } while(0)
#endif

EC_API EcInt ec_msg_vprintf( const char *format, va_list ap )
{
	/* for error msgs: use streams if available, otherwise stdio (if avail.) */

	EcInt res;
	ec_string ds;

	ec_string_init( &ds, NULL );
	res = ec_vsprintf( &ds, format, ap );
	ASSERT( res == ec_strlen(&ds) );

	if (PRIVATE(stream_stderr))
	{
		ec_stream_writen( PRIVATE(stream_stderr), ec_strdata( &ds ), ec_strlen( &ds ) );
	} else
	{
#if defined(WITH_STDIO)
		if (fwrite( ec_strdata( &ds ), 1, res, stderr ) < res)
		{
			ec_string_cleanup( &ds );
			return -1;
		}
		FFLUSHERR();
#else /* start of ! defined(WITH_STDIO) */
#if HAVE_FWRITE
		if (fwrite( ec_strdata( &ds ), 1, res, stderr ) < res)
		{
			ec_string_cleanup( &ds );
			return -1;
		}
		FFLUSHERR();
#elif HAVE_FPUTS
		if (fputs( ec_strdata( &ds ), stderr ) < 0)
		{
			ec_string_cleanup( &ds );
			return -1;
		}
		FFLUSHERR();
#elif HAVE_FPRINTF
		fprintf( stderr, "%s", ec_strdata( &ds ) );
		FFLUSHERR();
#endif /* end of HAVE_FPRINTF */
#endif /* end of ! defined(WITH_STDIO) */
	}

	ec_string_cleanup( &ds );
	return res;
}
