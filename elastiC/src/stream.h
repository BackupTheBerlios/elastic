/* ==========================================================================
 * stream.h
 * ==========================================================================
 * elastiC streams - I/O fundamental entity.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sun Jun  9 20:16:02 CEST 2002
 *
 *   $Id: stream.h,v 1.3 2002/06/11 19:32:07 panta Exp $
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

#ifndef __STREAM_H
#define __STREAM_H

#include <stdarg.h>

#include <elastic/elastic.h>
#include <elastic/dstring.h>

EC_BEGIN_DECLS

typedef enum
{
	ec_stream_mode_unknown,
	ec_stream_mode_read,										/* read only    */
	ec_stream_mode_write,										/* write only   */
	ec_stream_mode_readwrite									/* read / write */
} ec_stream_mode;

typedef enum
{
	ec_stream_seek_start,
	ec_stream_seek_current,
	ec_stream_seek_end
} ec_stream_seek_type;

typedef enum													/* for `filter' streams' operations */
{
	ec_stream_direction_none,
	ec_stream_direction_in,
	ec_stream_direction_out
} ec_stream_direction;

/*
 * ec_streamtype
 *
 * Unique id associated to a stream type.
 * Returned by ec_stream_register()
 */
typedef EcInt ec_streamtype;

typedef struct ec_streamdef_struct  ec_streamdef;
typedef struct ec_stream_struct     ec_stream;
typedef struct ec_serializer_struct ec_serializer;				/* :TODO: define */

typedef EcBool           (*ec_stream_create_fcn)   ( ec_stream *s, EC_OBJ *excp, va_list ap );
typedef EcBool           (*ec_stream_destroy_fcn)  ( ec_stream *s );
typedef EcInt            (*ec_stream_close_fcn)    ( ec_stream *s );
typedef ec_stream_mode   (*ec_stream_mode_fcn)     ( ec_stream *s );
typedef EcBool           (*ec_stream_eof_fcn)	   ( ec_stream *s );
typedef EcBool			 (*ec_stream_flush_fcn)    ( ec_stream *s );
typedef ssize_t          (*ec_stream_read_fcn)     ( ec_stream *s, void *buf, ssize_t count );
typedef ssize_t          (*ec_stream_write_fcn)    ( ec_stream *s, const void *buf, ssize_t count );
typedef EcBool           (*ec_stream_unread_fcn)   ( ec_stream *s, const void *buf, ssize_t count );
typedef ssize_t          (*ec_stream_gets_fcn)     ( ec_stream *s, ec_string *dst, ssize_t maxchars );
typedef EcBool           (*ec_stream_charready_fcn)( ec_stream *s );
typedef EcBool           (*ec_stream_seek_fcn)     ( ec_stream *s, ssize_t offset, ec_stream_seek_type whence );
typedef ssize_t          (*ec_stream_tell_fcn)     ( ec_stream *s );
/* :TODO: add seek64 */
/* :TODO: add tell64 */

#if 0
typedef ssize_t          (*ec_stream_transform_fcn)( ec_stream *s,
													 ec_stream_direction direction,
													 const void *src, size_t  src_len,
													 void       *dst, size_t *dst_len ); /* for `filter' streams' operations */
#endif

typedef EcUInt           (*ec_stream_store_fcn)    ( ec_stream *s, EC_OBJ object,
													 ec_serializer *ser_delegate,
													 const char *ser_mode );
typedef EC_OBJ           (*ec_stream_restore_fcn)  ( ec_stream *s,
													 ec_serializer *ser_delegate,
													 const char *ser_mode );

/*
 * ec_streamdef
 *
 * Structure defining a stream type.
 * Among other things, this will provide function pointers to concrete
 * implementations of I/O operations.
 */
struct ec_streamdef_struct
{
	ec_streamtype streamtype;									/* type index returned by ec_stream_register() */

	ec_stream_create_fcn        create_fcn;
	ec_stream_destroy_fcn       destroy_fcn;

	ec_stream_close_fcn         close_fcn;
	ec_stream_mode_fcn			mode_fcn;
	ec_stream_eof_fcn			eof_fcn;
	ec_stream_flush_fcn			flush_fcn;
	ec_stream_read_fcn			read_fcn;
	ec_stream_write_fcn			write_fcn;
    ec_stream_unread_fcn        unread_fcn;
    ec_stream_gets_fcn          gets_fcn;
	ec_stream_charready_fcn		ready_fcn;
	ec_stream_seek_fcn			seek_fcn;
	ec_stream_tell_fcn			tell_fcn;
	/* :TODO: add seek64 */
	/* :TODO: add tell64 */

#if 0
	ec_stream_transform_fcn     transform_fcn;					/* for `filter' streams */
#endif

	ec_stream_store_fcn			store_fcn;						/* serialization: passivation */
	ec_stream_restore_fcn		restore_fcn;					/* serialization: activation  */

	/*
	 * :TODO:
	 *   select/poll
	 *   sync (force synchronous operation: always commit)
	 *   async I/O ?
	 */

	ec_type  user1_type;										/* type in userdata1 union                             */
	char    *name;												/* stream type name (file, socket, ...)                */
	EcByte   magic[17];											/* 128 bit magic uniquely identifying this type (+NUL) */
};

/*
 * ec_stream
 *
 * The real thing: a stream.
 */
struct ec_stream_struct
{
	const ec_streamdef  *streamdef;

	union {
		EcInt   v_int;
		EcDWord v_dword;
		EC_OBJ  v_obj;
		EcAny   v_any;
	} userdata1;

	void *userdata2;

	/* exception, if any */
	EC_OBJ exc;

	/* `filter' streams (chain of streams) */
	struct ec_stream_struct *delegate;							/* for `filter' streams */

	/* Serialization data */
	/* :TODO: */
};

EC_API const ec_streamdef  *ec_stream_register( ec_streamdef *streamdef );		/* returns a pointer to an ec_streamdef with
																				   an updated streamtype member */
EC_API ec_stream           *ec_stream_create( const ec_streamdef *streamdef, EC_OBJ *excp, ... );
EC_API ec_stream           *ec_stream_create_from_type( ec_streamtype streamtype, EC_OBJ *excp, ... );
EC_API EcBool               ec_stream_destroy( ec_stream *stream, EC_OBJ *excp );

/* EC_API EC_OBJ               EcMakeFileFromStream( ec_stream *stream ); */
EC_API EC_OBJ               EcMakeStream( ec_stream *stream );

/* EC_API EC_OBJ               ec_stream_exception( ec_stream *stream ); */
#define                     ec_stream_exception(stream)		((stream)->exc)
EC_API void                 ec_stream_exception_clear( ec_stream *stream );

EC_API EcInt                ec_stream_close( ec_stream *stream );
EC_API ec_stream_mode       ec_stream_getmode( ec_stream *stream );
EC_API EcBool               ec_stream_eof( ec_stream *stream );
EC_API EcBool			    ec_stream_flush( ec_stream *stream );
EC_API ssize_t              ec_stream_read( ec_stream *stream, void *buf, ssize_t count );
EC_API ssize_t              ec_stream_write( ec_stream *stream, const void *buf, ssize_t count );
EC_API EcBool               ec_stream_unread( ec_stream *stream, const void *buf, ssize_t count );
EC_API EcByte               ec_stream_getch( ec_stream *stream );
EC_API ssize_t              ec_stream_gets_slow( ec_stream *stream,
												 ec_string *dst, ssize_t maxchars ); /* maxchars == -1: unlimited, read up to NL or NUL */
EC_API ssize_t              ec_stream_getcstr( ec_stream *stream,
											   char *dst, ssize_t maxchars );		/* maxchars includes terminating NUL */
EC_API ssize_t              ec_stream_gets( ec_stream *stream,
											ec_string *dst, ssize_t maxchars );		/* maxchars == -1: unlimited, read up to NL or NUL */
/* :TODO: getline (doesn't store newline) */
EC_API EcBool               ec_stream_putch( ec_stream *stream, EcByte c );
EC_API EcBool               ec_stream_putcstr( ec_stream *stream, const char *src );
EC_API EcBool               ec_stream_puts( ec_stream *stream, ec_string *src );
EC_API EcBool               ec_stream_ungetch( ec_stream *stream, EcByte c );
EC_API EcBool               ec_stream_charready( ec_stream *stream );
EC_API EcBool               ec_stream_seek( ec_stream *stream, ssize_t offset, ec_stream_seek_type whence );
EC_API ssize_t              ec_stream_tell( ec_stream *stream );
/* :TODO: add seek64 */
/* :TODO: add tell64 */
EC_API EcUInt               ec_stream_store( ec_stream *stream, EC_OBJ object,
											 ec_serializer *serialization_delegate,
											 const char *serialization_mode );
EC_API EC_OBJ               ec_stream_restore( ec_stream *stream,
											   ec_serializer *serialization_delegate,
											   const char *serialization_mode );

EC_END_DECLS

#endif __STREAM_H
