/* ==========================================================================
 * filestream.c
 * ==========================================================================
 * stdlib-based I/O streams.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Tue Jun 11 17:34:18 CEST 2002
 *
 *   $Id: filestream.c,v 1.1 2002/06/11 18:45:10 panta Exp $
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

#include "basic.h"
#include "debug.h"

#include "elastic.h"
#include "compat.h"

#include "private.h"

#include "stream.h"

#include <stdio.h>
#include <stdlib.h>


EC_API const ec_streamdef *ec_filestream_def( void )
{
	return PRIVATE(filestream_def);
}

/* Private */

#define FS_FH(stream)				((FILE *) (stream)->userdata2)
#define FS_FH_SET(stream, v)		do { (stream)->userdata2 = (v); } while(0)

#define FS_POPENED(stream)			((EcBool) (stream)->userdata1.v_int)
#define FS_POPENED_SET(stream, v)	do { (stream)->userdata1.v_int = (EcInt)(v); } while(0)

static EcBool filestream_create( ec_stream *s, EC_OBJ *excp, va_list ap )
{
	FILE   *fh      = NULL;
	EcBool  popened = FALSE;

	fh      = va_arg( ap, FILE* );
	popened = va_arg( ap, int );

	FS_FH_SET(s, fh);
	FS_POPENED_SET(s, popened);

	return TRUE;
}

static EcInt filestream_close( ec_stream *s );

static EcBool filestream_destroy( ec_stream *s )
{
	EcInt rv;

	s->exc = EC_NIL;
	rv = filestream_close( s );
	if (EC_ERRORP(s->exc))
		return FALSE;
	return TRUE;
}

static EcInt filestream_close( ec_stream *s )
{
	if (FS_FH(s))
	{
		if (FS_POPENED(s))
		{
#if HAVE_PCLOSE
			int rv;
			rv = pclose( FS_FH(s) );
			FS_FH_SET(s, NULL);
			FS_POPENED_SET(s, FALSE);
			if (rv == -1)
			{
				s->exc = _ec_errno2exception( errno, EC_NIL, "in pclose" );	/* :TODO: it would be nice to have the associated EC_OBJ *if any* */
				return (EcInt) rv;
			}
#else
			s->exc = EcUnimplementedError( "`pclose' function not available" );
			
#endif
		} else
			fclose( FS_FH(s) );

		FS_FH_SET(s, NULL);
		FS_POPENED_SET(s, FALSE);
	}

	return 0;
}

static ec_stream_mode filestream_mode( ec_stream *s )
{
	int mode;

	mode = ec_file_getmode( FS_FH(s) );
	switch (mode)
	{
	case O_RDONLY:
		return ec_stream_mode_read;
	case O_WRONLY:
		return ec_stream_mode_write;
	case O_RDWR:
		return ec_stream_mode_readwrite;
	default:
		s->exc = EcUnimplementedError( "unknown file mode" );	/* :TODO: raise a more sensible exception */
		return ec_stream_mode_unknown;
	}

	ASSERT( FALSE );
	return ec_stream_mode_unknown;
}

static EcBool filestream_eof( ec_stream *s )
{
	return feof( FS_FH(s) ) ? TRUE : FALSE;
}

static EcBool filestream_flush( ec_stream *s )
{
	int rv;
	rv =fflush( FS_FH(s) );
	if (rv == 0) return TRUE;
	s->exc = _ec_errno2exception( errno, EC_NIL, "in fflush" );	/* :TODO: it would be nice to have the associated EC_OBJ *if any* */
	return FALSE;
}

static ssize_t filestream_read( ec_stream *s, void *buf, ssize_t count )
{
	size_t nread;

	clearerr( FS_FH(s) );
	nread = fread( buf, 1, count, FS_FH(s) );
	if (nread < count)
	{
		if (ferror( FS_FH(s) ))
			s->exc = _ec_errno2exception( errno, EC_NIL, "in fread" );	/* :TODO: it would be nice to have the associated EC_OBJ *if any* */
	}

	return (ssize_t) nread;
}

static ssize_t filestream_write( ec_stream *s, const void *buf, ssize_t count )
{
	size_t nwritten;

	clearerr( FS_FH(s) );
	nwritten = fwrite( buf, 1, count, FS_FH(s) );
	if (nwritten < count)
		s->exc = _ec_errno2exception( errno, EC_NIL, "in fwrite" );	/* :TODO: it would be nice to have the associated EC_OBJ *if any* */

	return (ssize_t) nwritten;
}

static EcBool filestream_unread( ec_stream *s, const void *buf, ssize_t count )
{
	int ch_r, ch;

	if (count != 1)
	{
		s->exc = EcUnimplementedError( "file streams allow only 1-byte `unread'" );
		return FALSE;
	}

	ch   = ((char *)buf)[0];
	ch_r = ungetc( ch, FS_FH(s) );
	if (ch_r != ch)
	{
		s->exc = _ec_errno2exception( errno, EC_NIL, "in ungetc" );	/* :TODO: it would be nice to have the associated EC_OBJ *if any* */
		return FALSE;
	}

	return TRUE;
}

static ssize_t filestream_gets( ec_stream *s, ec_string *dst, ssize_t maxchars )
{
	char   *dst_s;
	size_t  len;

	if (maxchars == 0)
		return 0;

	if (maxchars >= 0)
	{
		/* limited */

		dst_s = alloca( maxchars + 1 );
		if (! dst_s)
			return ec_stream_gets_slow( s, dst, maxchars );		/* revert to 1-byte read cycle */

		if (fgets( dst_s, maxchars, FS_FH(s) ) == NULL)
		{
			s->exc = _ec_errno2exception( errno, EC_NIL, "in fgets" );	/* :TODO: it would be nice to have the associated EC_OBJ *if any* */
			return 0;
		}
		dst_s[maxchars] = '\0';									/* against libc bugs */
		len = strlen( dst_s );
		ec_strcat( dst, dst_s, len );
		return (ssize_t) len;
	} else
	{
		/* unlimited */

		return ec_stream_gets_slow( s, dst, maxchars );			/* revert to 1-byte read cycle */
	}

	ASSERT( FALSE );
	return -1;
}

static EcBool filestream_ready( ec_stream *s )
{
	return ec_file_ready( FS_FH(s) ) ? TRUE : FALSE;
}

static EcBool filestream_seek( ec_stream *s, ssize_t offset, ec_stream_seek_type whence )
{
	int wh;

	switch (whence)
	{
	case ec_stream_seek_start:
		wh = SEEK_SET;
		break;
	case ec_stream_seek_current:
		wh = SEEK_CUR;
		break;
	case ec_stream_seek_end:
		wh = SEEK_END;
		break;
	}
	if (fseek( FS_FH(s), (long) offset, wh ) != 0)
	{
		s->exc = _ec_errno2exception( errno, EC_NIL, "in fseek" ); /* :TODO: it would be nice to have the associated EC_OBJ *if any* */
		return FALSE;
	}
	return TRUE;
}

static ssize_t filestream_tell( ec_stream *s )
{
	long rv;

	rv = ftell( FS_FH(s) );
	if (rv == -1)
	{
		s->exc = _ec_errno2exception( errno, EC_NIL, "in ftell" );	/* :TODO: it would be nice to have the associated EC_OBJ *if any* */
		return (ssize_t) -1;
	}

	return (ssize_t) rv;
}

/* :TODO: add seek64 */
/* :TODO: add tell64 */

#if 0
static ssize_t filestream_transform( ec_stream *s,
									 ec_stream_direction direction,
									 const void *src, size_t  src_len,
									 void       *dst, size_t *dst_len ); /* for `filter' streams' operations */
#endif

static EcUInt filestream_store( ec_stream *s, EC_OBJ object,
								ec_serializer *ser_delegate,
								const char *ser_mode )
{
	return (EcUInt) -1;
}

static EC_OBJ filestream_restore  ( ec_stream *s,
									ec_serializer *ser_delegate,
									const char *ser_mode )
{
	return EC_NIL;
}


#if ECMODULE_FILESTREAM_STATIC
EC_OBJ _ec_filestream_init( void )
#else
EC_API EC_OBJ ec_filestream_init( void )
#endif
{
	ec_streamdef def =
	{
		/* streamtype */ 0,

		/* create_fcn  */ filestream_create,
		/* destroy_fcn */ filestream_destroy,

		/* close_fcn   */ filestream_close,
		/* mode_fcn    */ filestream_mode,
		/* eof_fcn     */ filestream_eof,
		/* flush_fcn   */ filestream_flush,
		/* read_fcn    */ filestream_read,
		/* write_fcn   */ filestream_write,
		/* unread_fcn  */ filestream_unread,
		/* gets_fcn    */ filestream_gets,
		/* ready_fcn   */ filestream_ready,
		/* seek_fcn    */ filestream_seek,
		/* tell_fcn    */ filestream_tell,
		/* :TODO: add seek64 */
		/* :TODO: add tell64 */

		/* store_fcn   */ filestream_store,						/* serialization: passivation */
		/* restore_fcn */ filestream_restore,					/* serialization: activation  */

		/* user1_type  */ ec_type_int,							/* type in userdata1 union                      */
		/* name        */ "file",								/* stream type name (file, socket, ...)         */

		/* 128 bit magic uniquely identifying this type (MD5("filestream")) */
		/* magic[17]   */ "\xD3\xB9\x3A\x7A\x3F\xC1\x0A\xA4\x79\xCE\x31\xC4\x6E\x14\x5D\x8F"
	};

	const ec_streamdef *rdef;

	rdef = ec_stream_register( &def );
	if (! rdef) return EcMemoryError();

	PRIVATE(filestream_def) = rdef;

	return EC_NIL;												/* :TODO: return a package */
}

#if ECMODULE_FILESTREAM_STATIC
void _ec_filestream_cleanup( void )
#else
EC_API void ec_filestream_cleanup( void )
#endif
{
}
