/* ==========================================================================
 * dstring.h
 * ==========================================================================
 * Dynamic Strings.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sun May  3 10:26:04 MEST 1998
 *
 *   $Id: dstring.h,v 1.1 2002/05/23 21:16:54 panta Exp $
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

#ifndef __DSTRING_H
#define __DSTRING_H

#include <elastic/basic.h>
#include <elastic/memory.h>

EC_BEGIN_DECLS

/* ===================================================================================================
 * T Y P E S
 * =================================================================================================== */

#define EC_DSTRING_STATIC_SIZE 32

typedef struct
{
	char  *strng;												/* Dynamically allocated space         */
	EcInt  length;												/* Used chars in space (without NULL)  */
	EcInt  avail;												/* Available chars for string and NULL */

	char   static_space[EC_DSTRING_STATIC_SIZE];				/* Static space for small strings      */
} ec_string;

#define ec_strlen(s)		((s)->length)
#define ec_strdata(s)		((s)->strng)
#define ec_str_ok(s)		((s)->avail >= 0)

/* ===================================================================================================
 * P R O T O T Y P E S
 * =================================================================================================== */

EC_API ec_string *ec_string_init( ec_string *ds, const char *init );
EC_API ec_string *ec_string_prepare( ec_string *ds, EcInt size );
EC_API ec_string *ec_string_cleanup( ec_string *ds );
EC_API ec_string *ec_string_create( const char *init );
EC_API void       ec_string_destroy( ec_string *ds );
EC_API char      *ec_strcat( ec_string *ds, const char *string, EcInt length );
EC_API char      *ec_strcatc( ec_string *ds, char c );
EC_API char      *ec_strcatd( ec_string *ds, const ec_string *string );

EC_END_DECLS

#endif /* __DSTRING_H */
