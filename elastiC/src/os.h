/* ==========================================================================
 * os.h
 * ==========================================================================
 * Operating System services access layer.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat Jan 22 15:56:53 MET 2000
 *
 *   $Id: os.h,v 1.1 2002/05/23 21:17:05 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 2000 Marco Pantaleoni. All rights reserved.
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

#ifndef __OS_H
#define __OS_H

#include <stdlib.h>
#include <elastic/basic.h>

EC_BEGIN_DECLS

typedef enum { ec_fmode_rdonly, ec_fmode_wronly, ec_fmode_rdwr, ec_fmode_unknown } ec_fmode;

typedef struct
{
	EcInt up_s;													/* uptime (seconds part)           */
	EcInt up_u;													/* uptime (microseconds part)      */

	EcInt user_s;												/* user time (seconds part)        */
	EcInt user_u;												/* user time (microseconds part)   */

	EcInt system_s;												/* system time (seconds part)      */
	EcInt system_u;												/* system time (microseconds part) */
} ec_times;

	/* file */
EC_API ec_fmode ec_file_getmode( FILE *fh );
EC_API EcBool   ec_file_ready( FILE *fh );

	/* time */
EC_API EcFloat ec_get_utime( void );							/* return a universal unspecified time */
EC_API void    ec_get_times( ec_times *tb );

	/* environment */
EC_API char   *ec_getenv( const char *name );
EC_API EcBool  ec_setenv( const char *name, const char *value, EcBool overwrite );
EC_API void    ec_unsetenv( const char *name );

EC_END_DECLS

#endif /* __OS_H */

