/* ==========================================================================
 * filestream.h
 * ==========================================================================
 * stdlib-based I/O streams.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Wed Jun 12 13:32:20 CEST 2002
 *
 *   $Id: filestream.h,v 1.1 2002/06/12 16:00:05 panta Exp $
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

#ifndef __EC_FILESTREAM_H
#define __EC_FILESTREAM_H

#include <stdio.h>
#include <stdlib.h>

#include <elastic/elastic.h>

EC_BEGIN_DECLS

/* ========================================================================
 * P R O T O T Y P E S
 * ======================================================================== */

/* C API */

EC_API const ec_streamdef *ec_filestream_def( void );
EC_API ec_stream *ec_filestream_fopen( const char *name, const char *mode, EC_OBJ *excp );
EC_API ec_stream *ec_filestream_fdopen( int fildes, const char *mode, EC_OBJ *excp );
EC_API ec_stream *ec_filestream_popen( const char *command, const char *type, EC_OBJ *excp );
EC_API ec_stream *ec_filestream_make( FILE *fh, EcBool dontclose, EcBool popened, EC_OBJ *excp );

EC_API EC_OBJ EcLibFileStreamFOpen( const char *name, const char *mode );
EC_API EC_OBJ EcLibFileStreamFDOpen( int filedes, const char *mode );
EC_API EC_OBJ EcLibFileStreamPOpen( const char *command, const char *type );
EC_API EC_OBJ EcLibFileStreamMake( FILE *fh, EcBool dontclose, EcBool popened );

/* elastiC API */

EC_API EC_OBJ EcLibFileStream_Open( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibFileStream_FDOpen( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibFileStream_POpen( EC_OBJ stack, EcAny userdata );

EC_END_DECLS

#endif /* __EC_FILESTREAM_H */
