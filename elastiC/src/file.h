/* ==========================================================================
 * file.h
 * ==========================================================================
 * File type & operations.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Wed Dec 30 16:05:48 MET 1998
 *
 *   $Id: file.h,v 1.2 2002/06/07 16:09:56 panta Exp $
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

#ifndef __FILE_H
#define __FILE_H

#include <stdio.h>
#include <stdlib.h>

#include <elastic/elastic.h>

EC_BEGIN_DECLS

extern EC_API EcInt tc_file;

/* ========================================================================
 * M A C R O S
 * ======================================================================== */

#define EC_FILEP(obj)					(EC_TYPEP(obj,tc_file))
#define EC_FILEH(obj)					((FILE *) EC_USER(obj))
#define EC_FILEH_SET(obj,val)			do { EC_USER(obj) = (FILE *)(val); } while (0)
#define EC_FILE_POPENED(obj)			(EC_USER1(obj, EcBool))
#define EC_FILE_POPENED_SET(obj,val)	EC_USER1_SET(obj, EcBool, val)


/* ========================================================================
 * P R O T O T Y P E S
 * ======================================================================== */

/* C API */

EC_API EC_OBJ EcMakeFile( FILE *fh, EcBool popened );

EC_API EC_OBJ EcLibFileOpen( const char *name, const char *mode );
EC_API EC_OBJ EcLibFileFDOpen( int filedes, const char *mode );
EC_API EC_OBJ EcLibFilePOpen( const char *command, const char *type );
EC_API EC_OBJ EcLibFileClose( EC_OBJ obj );

/* elastiC API */

EC_API EC_OBJ EcLibFile_Open( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibFile_POpen( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibFile_Close( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibFile_Mode( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibFile_Eof( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibFile_Flush( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibFile_Read( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibFile_Write( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibFile_Getc( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibFile_Gets( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibFile_Putc( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibFile_Puts( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibFile_Printf( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibFile_Ungetc( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibFile_CharReady( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibFile_Seek( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibFile_Tell( EC_OBJ stack, EcAny userdata );

EC_END_DECLS

#endif /* __FILE_H */
