/* ==========================================================================
 * file.c
 * ==========================================================================
 * File type & operations.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Wed Dec 30 16:05:48 MET 1998
 *
 *   $Id: file.c,v 1.3 2002/06/07 16:09:56 panta Exp $
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

#include <stdio.h>
#include <stdlib.h>

#include "basic.h"
#include "elastic.h"
#include "private.h"
#include "compat.h"
#include "debug.h"


EC_API EcInt tc_file = -1;

static EC_OBJ file_copy( EC_OBJ obj, EcCopyType type );
static void   file_mark( EC_OBJ obj );
static void   file_free( EC_OBJ obj );
static EcInt  file_print( ec_string *str, EC_OBJ obj, EcBool detailed );
static EcUInt file_hash( EC_OBJ obj, EcInt recur );

static EcUInt s_ronly = 0, s_wonly = 0, s_rw = 0;
static EcUInt s_start = 0, s_current = 0, s_end = 0;

/* C API */

EC_API EC_OBJ EcMakeFile( FILE *fh, EcBool popened )
{
	EC_OBJ res;

	res = EcMakeUser( tc_file, fh );
	if (EC_ERRORP(res)) return res;
	EC_FILE_POPENED_SET(res, popened);
	return res;
}


EC_API EC_OBJ EcLibFileOpen( const char *name, const char *mode )
{
	FILE *fh;

	fh = fopen( name, mode );
	if (! fh)
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}
	return EcMakeFile( fh, FALSE );
}

EC_API EC_OBJ EcLibFileFDOpen( int filedes, const char *mode )
{
	FILE *fh;

	fh = fdopen( filedes, mode );
	if (! fh)
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}
	return EcMakeFile( fh, FALSE );
}

EC_API EC_OBJ EcLibFilePOpen( const char *command, const char *type )
{
#if HAVE_POPEN
	FILE *fh;

	fflush(stdout);
	fflush(stdin);
	fh = popen( command, type );
	if (! fh)
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}
	return EcMakeFile( fh, TRUE );
#else
	return EcUnimplementedError( "`popen' function not available" );
#endif
}

EC_API EC_OBJ EcLibFileClose( EC_OBJ obj )
{
	if (! EC_FILEP(obj))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return EcUnimplementedError( "unimplemented exception" );
	}

	if (! EC_FILEH(obj))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return EcUnimplementedError( "unimplemented exception" );
	}

	if (EC_FILE_POPENED(obj))
	{
#if HAVE_PCLOSE
		int rv;
		rv = pclose( EC_FILEH(obj) );
		EC_FILEH_SET(obj, NULL);
		EC_FILE_POPENED_SET(obj, NULL);
		if (rv == -1)
			return _ec_errno2exception( errno, obj, "in pclose" );
		return EcMakeInt( rv );
#else
		return EcUnimplementedError( "`pclose' function not available" );
#endif
	} else
		fclose( EC_FILEH(obj) );
	EC_FILEH_SET(obj, NULL);
	EC_FILE_POPENED_SET(obj, NULL);
	return EcTrueObject;
}

/* elastiC API */

EC_API EC_OBJ EcLibFile_Open( EC_OBJ stack, EcAny userdata )
{
	char *name;
	char *mode = "r";
	EC_OBJ   res;

	res = EcParseStackFunction( "file.open", TRUE, stack, "s|s", &name, &mode );
	if (EC_ERRORP(res))
		return res;

	return EcLibFileOpen( name, mode );
}

EC_API EC_OBJ EcLibFile_POpen( EC_OBJ stack, EcAny userdata )
{
	char   *command;
	char   *type = "r";
	EC_OBJ  res;

	res = EcParseStackFunction( "file.popen", TRUE, stack, "s|s", &command, &type );
	if (EC_ERRORP(res))
		return res;

	return EcLibFilePOpen( command, type );
}

EC_API EC_OBJ EcLibFile_Close( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ res;

	res = EcParseStackFunction( "file.close", TRUE, stack, "O!", tc_file, &obj );
	if (EC_ERRORP(res))
		return res;

	return EcLibFileClose( obj );
}

EC_API EC_OBJ EcLibFile_Mode( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ res;
	int mode;
/*	int fd; */

	res = EcParseStackFunction( "file.mode", TRUE, stack, "O!", tc_file, &obj );
	if (EC_ERRORP(res))
		return res;

	if (! EC_FILEH(obj))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

/*	fd = fileno( EC_FILEH(obj) );
	mode = fcntl( fd, F_GETFL, 0 ); */
	mode = ec_file_getmode( EC_FILEH(obj) );
	switch (mode)
	{
	case O_RDONLY:
		return EcMakeSymbolFromId( s_ronly );
	case O_WRONLY:
		return EcMakeSymbolFromId( s_wonly );
	case O_RDWR:
		return EcMakeSymbolFromId( s_rw );
	default:
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	ASSERT( FALSE );
	return EC_NIL;
}

EC_API EC_OBJ EcLibFile_Eof( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ res;

	res = EcParseStackFunction( "file.eof", TRUE, stack, "O!", tc_file, &obj );
	if (EC_ERRORP(res))
		return res;

	if (! EC_FILEH(obj))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	return feof( EC_FILEH(obj) ) ? EcTrueObject : EcFalseObject;
}

EC_API EC_OBJ EcLibFile_Flush( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ r;
	int res;

	r = EcParseStackFunction( "file.flush", TRUE, stack, "O!", tc_file, &obj );
	if (EC_ERRORP(r))
		return r;

	if (! EC_FILEH(obj))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	res = fflush( EC_FILEH(obj) );
	if (res == 0)
		return EcTrueObject;

	/* TODO: or use an exception ? */
	return EcFalseObject;
}

EC_API EC_OBJ EcLibFile_Read( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ   obj;
	EC_OBJ   res;
	EcInt dim;
	char *buf;

	res = EcParseStackFunction( "file.read", TRUE, stack, "O!i", tc_file, &obj, &dim );
	if (EC_ERRORP(res))
		return res;

	if (! EC_FILEH(obj))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	buf = alloca( (size_t) dim );
	if (! buf)
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	if (fread( buf, (size_t) dim, (size_t) 1, EC_FILEH(obj) ) != 1)
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	return EcMakeString( buf, dim );
}

EC_API EC_OBJ EcLibFile_Write( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ    obj;
	EC_OBJ    res;
	char  *str;
	EcInt  strl;
	EcInt  dim = -1;

	res = EcParseStackFunction( "file.write", TRUE, stack, "O!s#|i", tc_file, &obj, &str, &strl, &dim );
	if (EC_ERRORP(res))
		return res;

	if (! EC_FILEH(obj))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	if (dim == -1)
	{
		dim = strl;
	}

	if (fwrite( str, dim, 1, EC_FILEH(obj) ) != 1)
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	return obj;
}

EC_API EC_OBJ EcLibFile_Getc( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ r;
	int res;

	r = EcParseStackFunction( "file.getc", TRUE, stack, "O!", tc_file, &obj );
	if (EC_ERRORP(r))
		return r;

	if (! EC_FILEH(obj))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	res = fgetc( EC_FILEH(obj) );
	if (res == EOF)
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	return EcMakeChar( (EcChar) res );
}

EC_API EC_OBJ EcLibFile_Gets( EC_OBJ stack, EcAny userdata )
{
#define MAXL	4096
	EC_OBJ obj;
	EC_OBJ res;
	char buf[MAXL];

	res = EcParseStackFunction( "file.gets", TRUE, stack, "O!", tc_file, &obj );
	if (EC_ERRORP(res)) return res;

	if (! EC_FILEH(obj))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	if (! fgets( buf, MAXL, EC_FILEH(obj) ))
	{
		if (feof( EC_FILEH(obj) ))
			return EC_NIL;

		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	return EcMakeString( buf, 0 );
}

EC_API EC_OBJ EcLibFile_Putc( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ    obj;
	EcChar ch;
	EC_OBJ    r;
	int    res;

	r = EcParseStackFunction( "file.putc", TRUE, stack, "O!c", tc_file, &obj, &ch );
	if (EC_ERRORP(r))
		return r;

	if (! EC_FILEH(obj))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	res = fputc( ch, EC_FILEH(obj) );
	if (res == EOF)
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	return obj;
}

EC_API EC_OBJ EcLibFile_Puts( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ    obj;
	char  *str;
	EC_OBJ    r;
	int    res;

	r = EcParseStackFunction( "file.puts", TRUE, stack, "O!s", tc_file, &obj, &str );
	if (EC_ERRORP(r))
		return r;

	if (! EC_FILEH(obj))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	res = fwrite( str, strlen( str ), 1, EC_FILEH(obj) );
	if (res == 1)
	{
		res = fputc( '\n', EC_FILEH(obj) );
	} else
		res = EOF;

	if (res == EOF)
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	return obj;
}

EC_API EC_OBJ EcLibFile_Printf( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ  f;
	char   *fmt;
	EcInt   fmtlen;
	EC_OBJ *obj;
	EcInt   num = 0;
	EC_OBJ  res;
	EcInt   r;

	res = EcParseStackFunction( "file.printf", TRUE, stack, "O!s#|o", tc_file, &f, &fmt, &fmtlen, &num, &obj );
	if (EC_ERRORP(res))
		return res;

	if (! EC_FILEH(f))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	res = ec_printf_obj( fmt, obj, num, 2, EC_NIL );
	if (EC_ERRORP(res))
		return res;

	ASSERT( EC_STRINGP(res) );
/*	fprintf( stderr, "WRITTEN %ld %d string: '%s'\n", EC_STRLEN(res), strlen( EC_STRDATA(res) ), EC_STRDATA(res) );*/
	r = fwrite( EC_STRDATA(res), 1, EC_STRLEN(res), EC_FILEH(f) );
	if (r != EC_STRLEN(res))
	{
		if (ferror( EC_FILEH(f) ))
			fprintf( stderr, "FILE ERROR: %d %s\n", errno, strerror( errno ) );
		fprintf( stderr, "ARGH: %ld\n", (long)r );
		r = EOF;
	}

	if (r == EOF)
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	return f;
}

EC_API EC_OBJ EcLibFile_Ungetc( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ    obj;
	EcChar ch;
	EC_OBJ    r;
	int    res;

	r = EcParseStackFunction( "file.ungetc", TRUE, stack, "O!c", tc_file, &obj, &ch );
	if (EC_ERRORP(r))
		return r;

	if (! EC_FILEH(obj))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	res = ungetc( ch, EC_FILEH(obj) );
	if (res == EOF)
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	return obj;
}

EC_API EC_OBJ EcLibFile_CharReady( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ res;

	res = EcParseStackFunction( "file.charready", TRUE, stack, "O!", tc_file, &obj );
	if (EC_ERRORP(res))
		return res;

	if (! EC_FILEH(obj))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	return ec_file_ready( EC_FILEH(obj) ) ? EcTrueObject : EcFalseObject;
}

EC_API EC_OBJ EcLibFile_Seek( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ  obj;
	EcInt   offset;
	EcUInt  symmode;
	EC_OBJ  r;
	int     wh;
	int     res;

	symmode = s_start;

	r = EcParseStackFunction( "file.seek", TRUE, stack, "O!i|k", tc_file, &obj, &offset, &symmode );
	if (EC_ERRORP(r))
		return r;

	if (! EC_FILEH(obj))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	if (symmode == s_start)
		wh = SEEK_SET;
	else if (symmode == s_current)
		wh = SEEK_CUR;
	else if (symmode == s_end)
		wh = SEEK_END;
	else
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	res = fseek( EC_FILEH(obj), offset, wh );
	if (res == 0)
		return EcTrueObject;

	/* TODO: Or throw an exception ? */
	return EcFalseObject;
}

EC_API EC_OBJ EcLibFile_Tell( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ obj;
	EC_OBJ res;

	res = EcParseStackFunction( "file.tell", TRUE, stack, "O!", tc_file, &obj );
	if (EC_ERRORP(res))
		return res;

	if (! EC_FILEH(obj))
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	return EcMakeInt( ftell( EC_FILEH(obj) ) );
}

/* Private */

static EC_OBJ file_copy( EC_OBJ obj, EcCopyType type )
{
	ASSERT( EC_FILEP(obj) );

	return EcMakeFile( EC_FILEH(obj), EC_FILE_POPENED(obj) );
}

static void file_mark( EC_OBJ obj )
{
	ASSERT( EC_FILEP(obj) );
}

static void file_free( EC_OBJ obj )
{
	ASSERT( EC_FILEP(obj) );

	if (EC_FILEH(obj))
	{
		if ((EC_FILEH(obj) != stdin)  &&
			(EC_FILEH(obj) != stdout) &&
			(EC_FILEH(obj) != stderr))
		{
			if (EC_FILE_POPENED(obj))
				pclose( EC_FILEH(obj) );
			else
				fclose( EC_FILEH(obj) );
		}
		EC_FILEH_SET(obj, NULL);
		EC_FILE_POPENED_SET(obj, FALSE);
	}
}

static EcInt file_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	ASSERT( EC_FILEP(obj) );

	if (detailed)
		return ec_sprintf( str, "<file %d%s>",
						   EC_FILEH(obj) ? fileno( EC_FILEH(obj) ) : -1,
						   EC_FILE_POPENED(obj) ? " (pipe)" : "" );
	else
		return ec_sprintf( str, "<file>" );
}

static EcUInt file_hash( EC_OBJ obj, EcInt recur )
{
	ASSERT( EC_FILEP(obj) );

	/* Equivalent object MUST have the same hash */
	return 1;													/* this is all we can do ... */
}

EcBool _ec_file_init( void )
{
	EcTypespec filespec = {
		/* type    */		0,
		/* name    */		"file",

		/* copy    */		file_copy,

		/* mark    */		file_mark,
		/* free    */		file_free,
		/* print   */		file_print,

		/* hash    */		file_hash,
		/* compare */		NULL,

		/* check   */		NULL,

		/* sequence */      NULL,
		/* numeric  */      NULL
	};
	EC_OBJ pkg;

	tc_file = EcAddType( &filespec );
	if (tc_file == 0)
		return FALSE;

	pkg = EcPackageIntroduce( "file" );
	EcAddPrimitive( "file.open",		EcLibFile_Open );
	EcAddPrimitive( "file.popen",		EcLibFile_POpen );
	EcAddPrimitive( "file.close",		EcLibFile_Close );
	EcAddPrimitive( "file.mode",		EcLibFile_Mode );
	EcAddPrimitive( "file.eof",			EcLibFile_Eof );
	EcAddPrimitive( "file.flush",		EcLibFile_Flush );
	EcAddPrimitive( "file.read",		EcLibFile_Read );
	EcAddPrimitive( "file.write",		EcLibFile_Write );
	EcAddPrimitive( "file.getc",		EcLibFile_Getc );
	EcAddPrimitive( "file.gets",		EcLibFile_Gets );
	EcAddPrimitive( "file.putc",		EcLibFile_Putc );
	EcAddPrimitive( "file.puts",		EcLibFile_Puts );
	EcAddPrimitive( "file.printf",		EcLibFile_Printf );
	EcAddPrimitive( "file.ungetc",		EcLibFile_Ungetc );
	EcAddPrimitive( "file.charready",	EcLibFile_CharReady );
	EcAddPrimitive( "file.seek",		EcLibFile_Seek );
	EcAddPrimitive( "file.tell",		EcLibFile_Tell );

	/* Constant values */

	s_ronly = EcInternSymbol( "readonly" );
	s_wonly = EcInternSymbol( "writeonly" );
	s_rw    = EcInternSymbol( "readwrite" );
	EcPackageVariable( EC_NIL, "file.readonly",  EcMakeSymbolFromId( s_ronly ), TRUE, TRUE );
	EcPackageVariable( EC_NIL, "file.writeonly", EcMakeSymbolFromId( s_wonly ), TRUE, TRUE );
	EcPackageVariable( EC_NIL, "file.readwrite", EcMakeSymbolFromId( s_rw ),    TRUE, TRUE );

	/* Variables */

	EcPackageVariable( EC_NIL, "file.stdin",  EcMakeFile( stdin,  FALSE ), TRUE, FALSE );
	EcPackageVariable( EC_NIL, "file.stdout", EcMakeFile( stdout, FALSE ), TRUE, FALSE );
	EcPackageVariable( EC_NIL, "file.stderr", EcMakeFile( stderr, FALSE ), TRUE, FALSE );

	/* Misc */

	s_start   = EcInternSymbol( "start" );
	s_current = EcInternSymbol( "current" );
	s_end     = EcInternSymbol( "end" );

	return TRUE;
}

void _ec_file_cleanup( void )
{
}
