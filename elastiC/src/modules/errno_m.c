/* ==========================================================================
 * errno_m.c
 * ==========================================================================
 * errno symbols and values module.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Fri Jun  7 18:53:58 CEST 2002
 *
 *   $Id: errno_m.c,v 1.1 2002/06/07 17:34:05 panta Exp $
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
#include "private.h"
#include "compat.h"

#if HAVE_ERRNO_H
#include <errno.h>
#endif

#if ECMODULE_ERRNO_STATIC
EC_OBJ _ec_moderrno_init( void )
#else
EC_API EC_OBJ ec_errno_init( void )
#endif
{
	EC_OBJ pkg;
	EC_OBJ value_h, symbol_h;

	pkg = EcPackageIntroduce( "errno" );
	if (EC_ERRORP(pkg))
		return pkg;

	/* Variables */

	value_h = EcMakeHash();
	if (EC_ERRORP(value_h)) return value_h;
	symbol_h = EcMakeHash();
	if (EC_ERRORP(symbol_h)) return symbol_h;

#include "errno_m_body.c.incl"

	EcPackageVariable( pkg, "value",							/* hash mapping symbol to value */
					   value_h,
					   TRUE, TRUE );
	EcPackageVariable( pkg, "symbol",							/* hash mapping value to symbol */
					   symbol_h,
					   TRUE, TRUE );
	return pkg;
}

#if ECMODULE_ERRNO_STATIC
void _ec_moderrno_cleanup( void )
#else
EC_API void ec_errno_cleanup( void )
#endif
{
}
