/* ==========================================================================
 * stream.c
 * ==========================================================================
 * elastiC streams - I/O fundamental entity.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sun Jun  9 20:16:02 CEST 2002
 *
 *   $Id: stream.c,v 1.6 2002/06/13 11:06:21 panta Exp $
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

/* default stream methods (private) */


/* C API */

EC_API const ec_streamdef *ec_stream_register( ec_streamdef *streamdef )
{
	/*
	 * returns a pointer to an ec_streamdef with
	 * an updated streamtype member.
	 */

	ec_streamtype streamidx;
	ec_streamdef *new_streamspec;
	EcInt         i;

	ASSERT( streamdef );

	streamdef->streamtype = -1;

	for (i = 0; i < (int)PRIVATE(streamtype_next); i++)
	{
		if (memcmp(PRIVATE(streamspec)[i].magic, streamdef->magic, sizeof(streamdef->magic)) == 0)
			return NULL;										/* a stream type with the same magic is already registered */
	}

	streamidx      = PRIVATE(streamtype_next);
	new_streamspec = ec_realloc( PRIVATE(streamspec), (streamidx + 1) * sizeof(struct ec_streamdef_struct) );
	if (! new_streamspec) return NULL;

	PRIVATE(streamspec) = new_streamspec;

	memcpy( &STREAMDEF(streamidx), streamdef, sizeof(struct ec_streamdef_struct) );
	STREAMDEF(streamidx).name = NULL;
	if (streamdef->name)
	{
		STREAMDEF(streamidx).name = ec_stringdup( streamdef->name );
		if (! STREAMDEF(streamidx).name)
		{
			memset( &STREAMDEF(streamidx), 0x00, sizeof(struct ec_streamdef_struct) );
			streamdef->streamtype = -1;
			return NULL;
		}
	}

	/* fill with default function pointers if necessary */
	/* :TODO: default_flush */
	/* :TODO: default_unread */
	/* :TODO: default_gets */

	/* :TODO: default_mark */
	/* :TODO: default_print */
	/* :TODO: default_hash */

	streamdef->streamtype = streamidx;
	PRIVATE(streamtype_next) = streamidx + 1;
	return &STREAMDEF(streamidx);
}

static ec_stream *ec_stream_create_va( const ec_streamdef *streamdef, EC_OBJ *excp, va_list ap )
{
	ec_stream *stream;
	EcBool     ok = FALSE;

	stream = ec_malloc( sizeof(struct ec_stream_struct) );
	if (! stream)
	{
		if (excp)
			*excp = EcMemoryError();
		return NULL;
	}

	if (excp) *excp = NULL;

	stream->streamdef = streamdef;
	stream->userdata1.v_int   = 0;
	stream->userdata1.v_dword = 0;
	stream->userdata1.v_any   = NULL;
	stream->userdata1.v_obj   = EC_NIL;
	stream->userdata2         = NULL;
	stream->exc               = EC_NIL;
	stream->delegate          = NULL;

	if (streamdef->create_fcn)
	{
		ok = streamdef->create_fcn( stream, &stream->exc, ap );
		if (! ok)
		{
			if (excp) *excp = stream->exc;
			ec_free( stream );
			return NULL;
		}
	}

	return stream;
}

EC_API ec_stream *ec_stream_create( const ec_streamdef *streamdef, EC_OBJ *excp, ... )
{
	va_list    ap;
	ec_stream *stream;

	va_start( ap, excp );
	stream = ec_stream_create_va( streamdef, excp, ap );
	va_end( ap );
	return stream;
}

EC_API ec_stream *ec_stream_create_from_type( ec_streamtype streamtype, EC_OBJ *excp, ... )
{
	va_list    ap;
	ec_stream *stream;

	va_start( ap, excp );
	if ((streamtype <  0) ||
		(streamtype >= PRIVATE(streamtype_next)))
	{
		if (excp) *excp = EcUnimplementedError("stream type out of range");	/* :TODO: raise a RangeError exception or
																			   something more appropriate */
		va_end( ap );
		return NULL;
	}
	stream = ec_stream_create_va( &STREAMDEF(streamtype), excp, ap );
	va_end( ap );
	return stream;
}

EC_API EcBool ec_stream_destroy( ec_stream *stream, EC_OBJ *excp )
{
	EcBool ok;

	ASSERT( stream );

	if (stream->streamdef->destroy_fcn)
	{
		ok = stream->streamdef->destroy_fcn( stream );
		if (! ok) return ok;
	}

	if (excp) *excp = EC_NIL;
	ec_free( stream );
	return TRUE;
}

/* EC_OBJ               EcMakeFileFromStream( ec_stream *stream ); */

#if 0
EC_API void ec_stream_exception_clear( ec_stream *stream )
{
	ASSERT( stream );
	stream->exc = EC_NIL;
}
#endif

EC_API EcInt ec_stream_close( ec_stream *stream )
{
	ASSERT( stream );
	if (stream->streamdef->close_fcn)
		return stream->streamdef->close_fcn( stream );
	stream->exc = EcUnimplementedError( "`close' not implemented for this stream type." );
	return -1;	/* unimportant */
}

EC_API ec_stream_mode ec_stream_getmode( ec_stream *stream )
{
	ASSERT( stream );
	if (stream->streamdef->mode_fcn)
		return stream->streamdef->mode_fcn( stream );
	stream->exc = EcUnimplementedError( "`mode' not implemented for this stream type." );
	return ec_stream_mode_unknown;
}

EC_API EcBool ec_stream_eof( ec_stream *stream )
{
	ASSERT( stream );
	if (stream->streamdef->eof_fcn)
		return stream->streamdef->eof_fcn( stream );
	stream->exc = EcUnimplementedError( "`eof' not implemented for this stream type." );
	return FALSE;	/* unimportant */
}

EC_API EcBool ec_stream_flush( ec_stream *stream )
{
	ASSERT( stream );
	if (stream->streamdef->flush_fcn)
		return stream->streamdef->flush_fcn( stream );
	stream->exc = EcUnimplementedError( "`flush' not implemented for this stream type." );
	return FALSE;
}

static ssize_t read_helper( ec_stream *stream, void *buf, ssize_t count )
{
	/*
	 * helper read function handling "short" reads.
	 * (needed for buffer streams and low-level OS "read").
	 */
	register ec_stream_read_fcn read_fcn;
	ssize_t nleft, nread;

	if (count <= 0)
		return 0;

	read_fcn = stream->streamdef->read_fcn;
	if (! read_fcn)
	{
		stream->exc = EcUnimplementedError( "`read' not implemented for this stream type." );
		errno = EINVAL;
		return -1;
	}

	ec_stream_exception_clear( stream );
	nleft = count;
	while (nleft > 0)
	{
		nread = read_fcn( stream, buf, nleft );

		if ((nread < 0) || EC_ERRORP(stream->exc))
			return nread;										/* error, return < 0 */
		else if (nread == 0)
			break;												/* EOF */

		nleft -= nread;
		buf   += nread;
	}

	return (count - nleft);										/* return >= 0 */
}

static ssize_t write_helper( ec_stream *stream, const void *buf, ssize_t count )
{
	/*
	 * helper write function handling "short" write.
	 * (needed for buffer streams and low-level OS "write").
	 */
	register ec_stream_write_fcn write_fcn;
	size_t nleft, nwritten;

	if (count <= 0)
		return 0;

	write_fcn = stream->streamdef->write_fcn;
	if (! write_fcn)
	{
		stream->exc = EcUnimplementedError( "`write' not implemented for this stream type." );
		errno = EINVAL;
		return -1;
	}

	ec_stream_exception_clear( stream );
	nleft = count;
	while (nleft > 0)
	{
		nwritten = write_fcn( stream, buf, nleft );
		if ((nwritten <= 0) || EC_ERRORP(stream->exc))
			return nwritten;									/* error */

		nleft -= nwritten;
		buf   += nwritten;
	}

	return (count - nleft);
}

EC_API ssize_t ec_stream_read( ec_stream *stream, void *buf, ssize_t count )
{
	/* see ec_stream_readn() for short-reads handling version */

	ASSERT( stream );
	ASSERT( count >= 0 );

	if (count <= 0)
		return 0;

	ec_stream_exception_clear( stream );
	if (stream->streamdef->read_fcn)
		return stream->streamdef->read_fcn( stream, buf, count );
	stream->exc = EcUnimplementedError( "`read' not implemented for this stream type." );
	errno = EINVAL;
	return -1;
}

EC_API ssize_t ec_stream_readn( ec_stream *stream, void *buf, ssize_t count )
{
	/* short-reads handling version */

	ASSERT( stream );
	ASSERT( count >= 0 );

	return read_helper( stream, buf, count );
}

EC_API ssize_t ec_stream_write( ec_stream *stream, const void *buf, ssize_t count )
{
	/* see ec_stream_writen() for short-writes handling version */

	ASSERT( stream );
	ASSERT( count >= 0 );

	if (count <= 0)
		return 0;

	ec_stream_exception_clear( stream );
	if (stream->streamdef->write_fcn)
		return stream->streamdef->write_fcn( stream, buf, count );
	stream->exc = EcUnimplementedError( "`write' not implemented for this stream type." );
	errno = EINVAL;
	return -1;
}

EC_API ssize_t ec_stream_writen( ec_stream *stream, const void *buf, ssize_t count )
{
	/* short-writes handling version */

	ASSERT( stream );
	ASSERT( count >= 0 );

	return write_helper( stream, buf, count );
}

EC_API EcBool ec_stream_unread( ec_stream *stream, const void *buf, ssize_t count )
{
	ASSERT( stream );
	ASSERT( count >= 0 );

	ec_stream_exception_clear( stream );
	if (stream->streamdef->unread_fcn)
		return stream->streamdef->unread_fcn( stream, buf, count );
	stream->exc = EcUnimplementedError( "`unread' not implemented for this stream type." );
	return FALSE;
}

EC_API EcByte ec_stream_getch( ec_stream *stream )
{
	EcByte val;

	ASSERT( stream );
	ec_stream_exception_clear( stream );
	/* we don't need to use ec_stream_readn() here, because we read only 1 byte */
	if (stream->streamdef->read_fcn)
	{
		stream->streamdef->read_fcn( stream, &val, 1 );
		return val;
	}
	stream->exc = EcUnimplementedError( "`read' not implemented for this stream type." );
	return 0;	/* unimportant */
}

EC_API ssize_t ec_stream_gets_slow( ec_stream *stream, ec_string *dst, ssize_t maxchars )
	/* maxchars == -1: unlimited, read up to NL or NUL */
{
	char    ch;
	ssize_t rem;
	ssize_t nread, nr;

	ASSERT( stream );
	if (maxchars == 0)
		return 0;

	if (! stream->streamdef->read_fcn)
	{
		stream->exc = EcUnimplementedError( "`read' not implemented for this stream type." );
		return -1;
	}

	ec_stream_exception_clear( stream );
	if (maxchars >= 0)
	{
		/* limited */

		rem = maxchars;
		while (rem > 0)
		{
			/* we don't need to use ec_stream_readn() here because we read only one byte */
			nr = stream->streamdef->read_fcn( stream, &ch, 1 );
			if (EC_ERRORP(stream->exc)) return nr;
			if (nr != 1)
				break;
			if (ch == '\0') break;								/* :TODO: ??? */
			ec_strcatc( dst, ch );
			rem--;
			nread += nr;
			if (ch == '\n') break;
		}
	} else
	{
		/* unlimited */

		while (TRUE)
		{
			/* we don't need to use ec_stream_readn() here because we read only one byte */
			nr = stream->streamdef->read_fcn( stream, &ch, 1 );
			if (EC_ERRORP(stream->exc)) return nr;
			if (nr != 1)
				break;
			if (ch == '\0') break;								/* :TODO: ??? */
			ec_strcatc( dst, ch );
			nread += nr;
			if (ch == '\n') break;
		}
	}

	ASSERT( rem >= 0 );
	return nread;
}

EC_API ssize_t ec_stream_getcstr( ec_stream *stream, char *dst, ssize_t maxchars )	/* maxchars includes terminating NUL */
{
	char   *dstp, ch;
	ssize_t rem;
	ssize_t nread, nr;

	ASSERT( stream );
	ASSERT( maxchars >= 0 );

	if (maxchars == 1)											/* space only for terminating NUL */
	{
		*dst = '\0';
		return 0;
	}
	if (maxchars < 1)
		return 0;

	if (stream->streamdef->gets_fcn)
	{
		ec_string ds;

        ec_string_init( &ds, NULL );
		nread = stream->streamdef->gets_fcn( stream, &ds, maxchars - 1 );
		if (EC_ERRORP(stream->exc)) return nread;
		if (nread > 0)
		{
			ASSERT( nread <= maxchars - 1 );
			memcpy( dst, ec_strdata(&ds), nread );
			dst[nread] = '\0';
		} else
			*dst = '\0';
        ec_string_cleanup( &ds );
		return nread;
	}

	if (! stream->streamdef->read_fcn)
	{
		stream->exc = EcUnimplementedError( "`read' not implemented for this stream type." );
		return -1;
	}

	ec_stream_exception_clear( stream );
	rem = maxchars;
	dstp = dst;
	while (rem > 1)
	{
		/* we don't need to use ec_stream_readn() here because we read only one byte */
		nr = stream->streamdef->read_fcn( stream, &ch, 1 );
		if (EC_ERRORP(stream->exc)) return nr;
		if (nr != 1)
			break;
		*dstp = ch;
		if (ch == '\0') break;									/* :TODO: ??? */
		dstp++;
		rem--;
		nread += nr;
		if (ch == '\n') break;
	}
	*dstp++ = '\0';
	rem--;
	ASSERT( rem >= 0 );
	return nread;
}

EC_API ssize_t ec_stream_gets( ec_stream *stream, ec_string *dst, ssize_t maxchars ) /* maxchars == -1: unlimited, read up to NL or NUL */
{
	ASSERT( stream );
	if (maxchars == 0)
		return 0;

	if (stream->streamdef->gets_fcn)
		return stream->streamdef->gets_fcn( stream, dst, maxchars );

	return ec_stream_gets_slow( stream, dst, maxchars );
}

EC_API EcBool ec_stream_putch( ec_stream *stream, EcByte c )
{
	EcByte  ch;
	ssize_t nwritten;

	ASSERT( stream );

	ec_stream_exception_clear( stream );
	if (stream->streamdef->write_fcn)
	{
		ch = c;
		/* we don't need to use ec_stream_writen() here because we write only one byte */
		nwritten = stream->streamdef->write_fcn( stream, &ch, 1 );
		if (EC_ERRORP(stream->exc)) return FALSE;
		return (nwritten == 1) ? TRUE : FALSE;
	}
	stream->exc = EcUnimplementedError( "`write' not implemented for this stream type." );
	return FALSE;
}

EC_API EcBool ec_stream_putcstr( ec_stream *stream, const char *src )
{
	size_t  len;
	ssize_t nwritten;
	char nl;

	ASSERT( stream );

	if (stream->streamdef->write_fcn)
	{
		len = src ? strlen(src) : 0;
		nwritten = write_helper( stream, src, len );
		if (EC_ERRORP(stream->exc)) return nwritten;
		if (nwritten != len)
		{
			stream->exc = EcUnimplementedError( "internal error: wrote different length" );
			return FALSE;
		}

		nl = '\n';
		nwritten = write_helper( stream, &nl, 1 );
		if (EC_ERRORP(stream->exc)) return nwritten;
		if (nwritten != 1)
		{
			stream->exc = EcUnimplementedError( "internal error: wrote different length" );
			return FALSE;
		}

		return TRUE;
	}
	stream->exc = EcUnimplementedError( "`write' not implemented for this stream type." );
	return FALSE;
}

EC_API EcBool ec_stream_puts( ec_stream *stream, ec_string *src )
{
	ssize_t nwritten;
	char nl;

	ASSERT( stream );
	if (stream->streamdef->write_fcn)
	{
		nwritten = write_helper( stream, ec_strdata(src), ec_strlen(src) );
		if (EC_ERRORP(stream->exc)) return nwritten;
		if (nwritten != ec_strlen(src))
		{
			stream->exc = EcUnimplementedError( "internal error: wrote different length" );
			return FALSE;
		}

		nl = '\n';
		nwritten = write_helper( stream, &nl, 1 );
		if (EC_ERRORP(stream->exc)) return nwritten;
		if (nwritten != 1)
		{
			stream->exc = EcUnimplementedError( "internal error: wrote different length" );
			return FALSE;
		}

		return TRUE;
	}
	stream->exc = EcUnimplementedError( "`write' not implemented for this stream type." );
	return FALSE;
}

EC_API EcBool ec_stream_ungetch( ec_stream *stream, EcByte c )
{
	EcByte ch;

	ASSERT( stream );
	ch = c;
	if (stream->streamdef->unread_fcn)
		return stream->streamdef->unread_fcn( stream, &ch, 1 );
	stream->exc = EcUnimplementedError( "`unread' not implemented for this stream type." );
	return FALSE;
}

EC_API EcBool ec_stream_charready( ec_stream *stream )
{
	ASSERT( stream );
	if (stream->streamdef->ready_fcn)
		return stream->streamdef->ready_fcn( stream );
	stream->exc = EcUnimplementedError( "`ready' not implemented for this stream type." );
	return FALSE;	/* unimportant */
}

EC_API EcBool ec_stream_seek( ec_stream *stream, ssize_t offset, ec_stream_seek_type whence )
{
	ASSERT( stream );
	if (stream->streamdef->seek_fcn)
		return stream->streamdef->seek_fcn( stream, offset, whence );
	stream->exc = EcUnimplementedError( "`seek' not implemented for this stream type." );
	return FALSE;
}

EC_API ssize_t ec_stream_tell( ec_stream *stream )
{
	ASSERT( stream );
	if (stream->streamdef->tell_fcn)
		return stream->streamdef->tell_fcn( stream );
	stream->exc = EcUnimplementedError( "`tell' not implemented for this stream type." );
	return -1;
}

EC_API EcUInt ec_stream_store( ec_stream *stream, EC_OBJ object,
							   ec_serializer *serialization_delegate,
							   const char *serialization_mode )
{
	/* :TODO: implement it */
	return (EcUInt)-1;
}

EC_API EC_OBJ ec_stream_restore( ec_stream *stream,
								 ec_serializer *serialization_delegate,
								 const char *serialization_mode )
{
	/* :TODO: implement it */
	return EcUnimplementedError( "serialization not yet implemented." );
}

EC_API void ec_stream_mark( ec_stream *stream )
{
	ASSERT( stream );

	if ((stream->streamdef->user1_type == ec_type_ecobj) &&
		EC_NNULLP(stream->userdata1.v_obj))
		EcMarkObject( stream->userdata1.v_obj );

	if (EC_NNULLP(stream->exc)) EcMarkObject( stream->exc );

	if (stream->delegate) ec_stream_mark( stream->delegate );

	if (stream->streamdef->mark_fcn)
		return stream->streamdef->mark_fcn( stream );
}

EC_API EcInt ec_stream_print( ec_stream *stream, ec_string *str, EcBool detailed )
{
	ASSERT( stream );

	if (stream->streamdef->print_fcn)
		return stream->streamdef->print_fcn( stream, str, detailed );
	else
	{
		if (detailed)
			return ec_sprintf( str, "<stream type:%s>",
							   stream->streamdef->name ? stream->streamdef->name : "UNNAMED" );
		else
			return ec_sprintf( str, "<stream>" );
	}
}

EC_API EcUInt ec_stream_hash( ec_stream *stream, EcInt recursion_level )
{
	ASSERT( stream );

	if (stream->streamdef->hash_fcn)
		return stream->streamdef->hash_fcn( stream, recursion_level );

	/* Equivalent object MUST have the same hash */
	return 1;													/* this is all we can do ... */
}

EC_API ec_stream *ec_stream_stdin( void )
{
	return PRIVATE(stream_stdin);
}

EC_API ec_stream *ec_stream_stdout( void )
{
	return PRIVATE(stream_stdout);
}

EC_API ec_stream *ec_stream_stderr( void )
{
	return PRIVATE(stream_stderr);
}
