/* ==========================================================================
 * memory.h
 * ==========================================================================
 * Safe memory handling routines.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: 28 Mar 1997
 *
 *   $Id: memory.h,v 1.2 2002/06/06 00:04:24 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1997-2000 Marco Pantaleoni. All rights reserved.
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

#ifndef __MEMORY_H
#define __MEMORY_H

#include <stdlib.h>

#include <elastic/basic.h>

EC_BEGIN_DECLS

typedef struct ec_mempool_struct *ec_mempool;

EC_API void *ec_malloc(size_t size);
EC_API void  ec_free(void *ptr);
EC_API void *ec_realloc(void *ptr, size_t size);

EC_API void *ec_memmove(void *dest, const void *src, size_t n);

EC_API void   *ec_allocblock( size_t nmemb, size_t size );
EC_API EcBool  ec_resizeblock( void *blockp, size_t nmemb, size_t size );
EC_API void    ec_freeblock( void *block );

EC_API ec_mempool  ec_allocpool( size_t size, int alignment );
EC_API void        ec_freepool(  ec_mempool pool );
EC_API void       *ec_palloc(    ec_mempool pool, size_t size );

EC_API char *ec_stringdup( const char *string );
EC_API char *ec_stringduppool( ec_mempool pool, const char *string );

EC_END_DECLS

#endif /* __MEMORY_H */
