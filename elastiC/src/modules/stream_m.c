/* ==========================================================================
 * stream_m.c
 * ==========================================================================
 * elastiC language stream module.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Wed Jun 12 10:52:30 CEST 2002
 *
 *   $Id: stream_m.c,v 1.3 2002/06/13 17:44:18 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 2002 Marco Pantaleoni. All rights reserved.
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
#include "elastic.h"
#include "private.h"
#include "compat.h"
#include "debug.h"

#include "stream_t.h"

static EcUInt s_ronly = 0, s_wonly = 0, s_rw = 0;
static EcUInt s_start = 0, s_current = 0, s_end = 0;

/* elastiC API */

EC_API EC_OBJ EcLibStream_Close( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ res;

	res = EcParseStackFunction( "stream.close", TRUE, stack, "O!", tc_stream, &obj );
	if (EC_ERRORP(res))
		return res;

	return EcLibStreamClose( obj );
}

EC_API EC_OBJ EcLibStream_Mode( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ         obj;
	EC_OBJ         res;
	ec_stream_mode mode;

	res = EcParseStackFunction( "stream.mode", TRUE, stack, "O!", tc_stream, &obj );
	if (EC_ERRORP(res))
		return res;

	if (! EC_STREAM_STR(obj))
	{
		/* TODO: Throw an exception */
		return EcUnimplementedError("unimplemented exception");
	}

	ec_stream_exception_clear( EC_STREAM_STR(obj) );
	mode = ec_stream_getmode( EC_STREAM_STR(obj) );
	if (EC_ERRORP(EC_STREAM_STR(obj)->exc))
		return EC_STREAM_STR(obj)->exc;
	switch (mode)
	{
	case ec_stream_mode_unknown:
		return EcUnimplementedError("unknown stream mode");		/* :TODO: better exception please */
	case ec_stream_mode_read:
		return EcMakeSymbolFromId( s_ronly );
	case ec_stream_mode_write:
		return EcMakeSymbolFromId( s_wonly );
	case ec_stream_mode_readwrite:
		return EcMakeSymbolFromId( s_rw );
	}

	ASSERT( FALSE );
	return EC_NIL;
}

EC_API EC_OBJ EcLibStream_Eof( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ res;
	EcBool rv;

	res = EcParseStackFunction( "stream.eof", TRUE, stack, "O!", tc_stream, &obj );
	if (EC_ERRORP(res))
		return res;

	if (! EC_STREAM_STR(obj))
	{
		/* TODO: Throw an exception */
		return EcUnimplementedError( "unimplemented exception" );
	}

	ec_stream_exception_clear( EC_STREAM_STR(obj) );
	rv = ec_stream_eof( EC_STREAM_STR(obj) );
	if (EC_ERRORP(EC_STREAM_STR(obj)->exc))
		return EC_STREAM_STR(obj)->exc;

	return rv ? EcTrueObject : EcFalseObject;
}

EC_API EC_OBJ EcLibStream_Flush( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ res;
	EcBool rv;

	res = EcParseStackFunction( "stream.flush", TRUE, stack, "O!", tc_stream, &obj );
	if (EC_ERRORP(res))
		return res;

	if (! EC_STREAM_STR(obj))
	{
		/* TODO: Throw an exception */
		return EcUnimplementedError( "unimplemented exception" );
	}

	ec_stream_exception_clear( EC_STREAM_STR(obj) );
	rv = ec_stream_flush( EC_STREAM_STR(obj) );
	if (EC_ERRORP(EC_STREAM_STR(obj)->exc))
		return EC_STREAM_STR(obj)->exc;
	return EcTrueObject;
}

EC_API EC_OBJ EcLibStream_Read( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ   obj;
	EC_OBJ   res;
	EcInt    dim;
	EcBool   exactly = FALSE;
	char    *buf;
	ssize_t  nread;

	res = EcParseStackFunction( "stream.read", TRUE, stack, "O!i|b", tc_stream, &obj, &dim, &exactly );
	if (EC_ERRORP(res))
		return res;

	if (! EC_STREAM_STR(obj))
	{
		/* TODO: Throw an exception */
		return EcUnimplementedError( "unimplemented exception" );
	}

	buf = alloca( (size_t) (dim + 1) );
	if (! buf)
		return EcMemoryError();

	ec_stream_exception_clear( EC_STREAM_STR(obj) );
	if (exactly)
		nread = ec_stream_readn( EC_STREAM_STR(obj), buf, (ssize_t) dim );
	else
		nread = ec_stream_read( EC_STREAM_STR(obj), buf, (ssize_t) dim );
	if (EC_ERRORP(EC_STREAM_STR(obj)->exc))
		return EC_STREAM_STR(obj)->exc;
	if (nread >= 0)
	{
		ASSERT( nread >= 0 );
		ASSERT( nread <= dim );
		ASSERT( nread == dim );
		return EcMakeString( buf, nread );
	}

	return EcUnimplementedError("internal error");				/* :TODO: better exception */
}

EC_API EC_OBJ EcLibStream_Write( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ    obj;
	EC_OBJ    res;
	char     *buf;
	EcInt     bufl;
	EcInt     dim = -1;
	EcBool    exactly = FALSE;
	ssize_t   nwritten;

	res = EcParseStackFunction( "stream.write", TRUE, stack, "O!s#|i", tc_stream, &obj, &buf, &bufl, &dim, &exactly );
	if (EC_ERRORP(res))
		return res;

	if (! EC_STREAM_STR(obj))
	{
		/* TODO: Throw an exception */
		return EcUnimplementedError( "unimplemented exception" );
	}

	if (dim == -1)
		dim = bufl;

	if (dim > bufl)
		return EcUnimplementedError( "range error" );			/* :TODO: better exception */

	ec_stream_exception_clear( EC_STREAM_STR(obj) );
	if (exactly)
		nwritten = ec_stream_writen( EC_STREAM_STR(obj), buf, (ssize_t) dim );
	else
		nwritten = ec_stream_write( EC_STREAM_STR(obj), buf, (ssize_t) dim );
	if (EC_ERRORP(EC_STREAM_STR(obj)->exc))
		return EC_STREAM_STR(obj)->exc;
	if (nwritten >= 0)
	{
		ASSERT( nwritten >= 0 );
		ASSERT( nwritten <= dim );
		ASSERT( nwritten <= bufl );
		ASSERT( nwritten == dim );
		return EcTrueObject;
	}

	return EcUnimplementedError("internal error");				/* :TODO: better exception */
}

EC_API EC_OBJ EcLibStream_Getc( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ res;
	EcByte ch;

	res = EcParseStackFunction( "stream.getc", TRUE, stack, "O!", tc_stream, &obj );
	if (EC_ERRORP(res))
		return res;

	if (! EC_STREAM_STR(obj))
	{
		/* TODO: Throw an exception */
		return EcUnimplementedError( "unimplemented exception" );
	}

	ec_stream_exception_clear( EC_STREAM_STR(obj) );
	ch = ec_stream_getch( EC_STREAM_STR(obj) );
	if (EC_ERRORP(EC_STREAM_STR(obj)->exc))
		return EC_STREAM_STR(obj)->exc;
	return EcMakeChar( (EcChar) ch );
}

EC_API EC_OBJ EcLibStream_Gets( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ    obj;
	EC_OBJ    res;
	EcInt     maxchars = -1;
	ec_string ds;
	ssize_t   nread;

	res = EcParseStackFunction( "stream.gets", TRUE, stack, "O!|i", tc_stream, &obj, &maxchars );
	if (EC_ERRORP(res)) return res;

	if (! EC_STREAM_STR(obj))
	{
		/* TODO: Throw an exception */
		return EcUnimplementedError( "unimplemented exception" );
	}

	ec_string_init( &ds, NULL );

	ec_stream_exception_clear( EC_STREAM_STR(obj) );
	nread = ec_stream_gets( EC_STREAM_STR(obj), &ds, (ssize_t) maxchars );
	if (EC_ERRORP(EC_STREAM_STR(obj)->exc))
	{
        ec_string_cleanup( &ds );
		return EC_STREAM_STR(obj)->exc;
	}

	res = EcMakeString( ec_strdata(&ds), -1 );
	ec_string_cleanup( &ds );
	return res;
}

EC_API EC_OBJ EcLibStream_Putc( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ  obj;
	EC_OBJ  res;
	EcChar  ch;
	EcBool  rv;

	res = EcParseStackFunction( "stream.putc", TRUE, stack, "O!c", tc_stream, &obj, &ch );
	if (EC_ERRORP(res))
		return res;

	if (! EC_STREAM_STR(obj))
	{
		/* TODO: Throw an exception */
		return EcUnimplementedError( "unimplemented exception" );
	}

	ec_stream_exception_clear( EC_STREAM_STR(obj) );
	rv = ec_stream_putch( EC_STREAM_STR(obj), (EcByte) ch );
	if (EC_ERRORP(EC_STREAM_STR(obj)->exc))
		return EC_STREAM_STR(obj)->exc;
	return EcTrueObject;
}

EC_API EC_OBJ EcLibStream_Puts( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ  obj;
	EC_OBJ  res;
	char   *str;
	EcBool  rv;

	res = EcParseStackFunction( "stream.puts", TRUE, stack, "O!s", tc_stream, &obj, &str );
	if (EC_ERRORP(res))
		return res;

	if (! EC_STREAM_STR(obj))
	{
		/* TODO: Throw an exception */
		return EcUnimplementedError( "unimplemented exception" );
	}

	ec_stream_exception_clear( EC_STREAM_STR(obj) );
	rv = ec_stream_putcstr( EC_STREAM_STR(obj), str );
	if (EC_ERRORP(EC_STREAM_STR(obj)->exc))
		return EC_STREAM_STR(obj)->exc;
	return EcTrueObject;
}

EC_API EC_OBJ EcLibStream_Printf( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ  f;
	char   *fmt;
	EcInt   fmtlen;
	EC_OBJ *obj;
	EcInt   num = 0;
	EC_OBJ  res;
	ssize_t nwritten;

	res = EcParseStackFunction( "stream.printf", TRUE, stack, "O!s#|o", tc_stream, &f, &fmt, &fmtlen, &num, &obj );
	if (EC_ERRORP(res))
		return res;

	if (! EC_STREAM_STR(f))
	{
		/* TODO: Throw an exception */
		return EcUnimplementedError( "unimplemented exception" );
	}

	res = ec_printf_obj( fmt, obj, num, 2, EC_NIL );
	if (EC_ERRORP(res))
		return res;

	ASSERT( EC_STRINGP(res) );
	ec_stream_exception_clear( EC_STREAM_STR(f) );
	nwritten = ec_stream_write( EC_STREAM_STR(f), EC_STRDATA(res), (ssize_t) EC_STRLEN(res) );
	if (EC_ERRORP(EC_STREAM_STR(f)->exc))
		return EC_STREAM_STR(f)->exc;
	if (nwritten >= 0)
	{
		ASSERT( nwritten >= 0 );
		ASSERT( nwritten <= EC_STRLEN(res) );
		ASSERT( nwritten == EC_STRLEN(res) );
		return EcTrueObject;
	}

	return EcUnimplementedError("internal error");				/* :TODO: better exception */
}

EC_API EC_OBJ EcLibStream_Ungetc( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ res;
	EcChar ch;
	EcBool rv;

	res = EcParseStackFunction( "stream.ungetc", TRUE, stack, "O!c", tc_stream, &obj, &ch );
	if (EC_ERRORP(res))
		return res;

	if (! EC_STREAM_STR(obj))
	{
		/* TODO: Throw an exception */
		return EcUnimplementedError( "unimplemented exception" );
	}

	ec_stream_exception_clear( EC_STREAM_STR(obj) );
	rv = ec_stream_ungetch( EC_STREAM_STR(obj), (EcByte)ch );
	if (EC_ERRORP(EC_STREAM_STR(obj)->exc))
		return EC_STREAM_STR(obj)->exc;
	return EcTrueObject;
}

EC_API EC_OBJ EcLibStream_CharReady( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ res;
	EcBool rv;

	res = EcParseStackFunction( "stream.charready", TRUE, stack, "O!", tc_stream, &obj );
	if (EC_ERRORP(res))
		return res;

	if (! EC_STREAM_STR(obj))
	{
		/* TODO: Throw an exception */
		return EcUnimplementedError( "unimplemented exception" );
	}

	ec_stream_exception_clear( EC_STREAM_STR(obj) );
	rv = ec_stream_charready( EC_STREAM_STR(obj) );
	if (EC_ERRORP(EC_STREAM_STR(obj)->exc))
		return EC_STREAM_STR(obj)->exc;
	return rv ? EcTrueObject : EcFalseObject;
}

EC_API EC_OBJ EcLibStream_Seek( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ              obj;
	EcInt               offset;
	EcUInt              symmode;
	EC_OBJ              res;
	ec_stream_seek_type wh;
	EcBool              rv;

	symmode = s_start;

	res = EcParseStackFunction( "stream.seek", TRUE, stack, "O!i|k", tc_stream, &obj, &offset, &symmode );
	if (EC_ERRORP(res))
		return res;

	if (! EC_STREAM_STR(obj))
	{
		/* TODO: Throw an exception */
		return EcUnimplementedError( "unimplemented exception" );
	}

	if (symmode == s_start)
		wh = ec_stream_seek_start;
	else if (symmode == s_current)
		wh = ec_stream_seek_current;
	else if (symmode == s_end)
		wh = ec_stream_seek_end;
	else
	{
		/* TODO: Throw an exception */
		return EcUnimplementedError( "bad seek mode" );			/* :TODO: better exception */
	}

	ec_stream_exception_clear( EC_STREAM_STR(obj) );
	rv = ec_stream_seek( EC_STREAM_STR(obj), (ssize_t) offset, wh );
	if (EC_ERRORP(EC_STREAM_STR(obj)->exc))
		return EC_STREAM_STR(obj)->exc;
	return rv ? EcTrueObject : EcFalseObject;
}

EC_API EC_OBJ EcLibStream_Tell( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ  obj;
	EC_OBJ  res;
	ssize_t pos;

	res = EcParseStackFunction( "stream.tell", TRUE, stack, "O!", tc_stream, &obj );
	if (EC_ERRORP(res))
		return res;

	if (! EC_STREAM_STR(obj))
	{
		/* TODO: Throw an exception */
		return EcUnimplementedError( "unimplemented exception" );
	}

	ec_stream_exception_clear( EC_STREAM_STR(obj) );
	pos = ec_stream_tell( EC_STREAM_STR(obj) );
	if (EC_ERRORP(EC_STREAM_STR(obj)->exc))
		return EC_STREAM_STR(obj)->exc;
	return EcMakeInt( (EcInt) pos );
}

EC_API EC_OBJ EcLibStream_Stdin( EC_OBJ stack, EcAny userdata )
{
	EC_CHECKNARGS_F("stream.stdin", 0);

	return EcMakeStream( PRIVATE(stream_stdin) );
}

EC_API EC_OBJ EcLibStream_Stdout( EC_OBJ stack, EcAny userdata )
{
	EC_CHECKNARGS_F("stream.stdout", 0);

	return EcMakeStream( PRIVATE(stream_stdout) );
}

EC_API EC_OBJ EcLibStream_Stderr( EC_OBJ stack, EcAny userdata )
{
	EC_CHECKNARGS_F("stream.stderr", 0);

	return EcMakeStream( PRIVATE(stream_stderr) );
}

/* Private */

#if ECMODULE_STREAM_STATIC
EC_OBJ _ec_modstream_init( void )
#else
EC_API EC_OBJ ec_stream_init( void )
#endif
{
	EC_OBJ pkg;

	pkg = EcPackageIntroduce( "stream" );
	if (EC_ERRORP(pkg))
		return pkg;

	EcAddPrimitive( "stream.close",			EcLibStream_Close );
	EcAddPrimitive( "stream.mode",			EcLibStream_Mode );
	EcAddPrimitive( "stream.eof",			EcLibStream_Eof );
	EcAddPrimitive( "stream.flush",			EcLibStream_Flush );
	EcAddPrimitive( "stream.read",			EcLibStream_Read );
	EcAddPrimitive( "stream.write",			EcLibStream_Write );
	EcAddPrimitive( "stream.getc",			EcLibStream_Getc );
	EcAddPrimitive( "stream.gets",			EcLibStream_Gets );
	EcAddPrimitive( "stream.putc",			EcLibStream_Putc );
	EcAddPrimitive( "stream.puts",			EcLibStream_Puts );
	EcAddPrimitive( "stream.printf",		EcLibStream_Printf );
	EcAddPrimitive( "stream.ungetc",		EcLibStream_Ungetc );
	EcAddPrimitive( "stream.charready",		EcLibStream_CharReady );
	EcAddPrimitive( "stream.seek",			EcLibStream_Seek );
	EcAddPrimitive( "stream.tell",			EcLibStream_Tell );

	EcAddPrimitive( "stream.stdin",			EcLibStream_Stdin  );
	EcAddPrimitive( "stream.stdout",		EcLibStream_Stdout );
	EcAddPrimitive( "stream.stderr",		EcLibStream_Stderr );

	/* Constant values */

	s_ronly = EcInternSymbol( "readonly" );
	s_wonly = EcInternSymbol( "writeonly" );
	s_rw    = EcInternSymbol( "readwrite" );
	EcPackageVariable( EC_NIL, "stream.readonly",  EcMakeSymbolFromId( s_ronly ), TRUE, TRUE );
	EcPackageVariable( EC_NIL, "stream.writeonly", EcMakeSymbolFromId( s_wonly ), TRUE, TRUE );
	EcPackageVariable( EC_NIL, "stream.readwrite", EcMakeSymbolFromId( s_rw    ), TRUE, TRUE );

	/* Variables */

#if 0
	/* WARNING: these need the filestream module */
	EcPackageVariable( EC_NIL, "stream.stdin",  EcMakeStream( PRIVATE(stream_stdin)  ), TRUE, FALSE );
	EcPackageVariable( EC_NIL, "stream.stdout", EcMakeStream( PRIVATE(stream_stdout) ), TRUE, FALSE );
	EcPackageVariable( EC_NIL, "stream.stderr", EcMakeStream( PRIVATE(stream_stderr) ), TRUE, FALSE );
#endif

	/* Misc */

	s_start   = EcInternSymbol( "start" );
	s_current = EcInternSymbol( "current" );
	s_end     = EcInternSymbol( "end" );

	return pkg;
}

#if ECMODULE_STREAM_STATIC
void _ec_modstream_cleanup( void )
#else
EC_API void ec_stream_cleanup( void )
#endif
{
}
