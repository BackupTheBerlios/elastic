/* ==========================================================================
 * dl-ltdl.c
 * ==========================================================================
 * elastiC Dynamic Loading Interface implementation for libltdl.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Wed Jan 19 10:52:17 MET 2000
 *
 *   $Id: dl-ltdl.c,v 1.1 2002/05/23 21:17:57 panta Exp $
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

#include "basic.h"
#include "debug.h"

#include "elastic.h"

#include <ltdl.h>

EC_API EcBool EcDLInit( void )
{
	if (lt_dlinit() != 0)
		return FALSE;
	return TRUE;
}

EC_API void EcDLCleanup( void )
{
	lt_dlexit();
}

EC_API ec_dlhandle EcDLOpen( const char *filename )
{
	lt_dlhandle dlhandle = NULL;

	dlhandle = lt_dlopen( filename );							/* for dlopen was: RTLD_LAZY | RTLD_GLOBAL */
	return (ec_dlhandle) dlhandle;
}

EC_API void EcDLClose( ec_dlhandle handle )
{
	lt_dlclose( (lt_dlhandle) handle );
}

EC_API ec_dlptr EcDLSymbol( ec_dlhandle handle, const char *symname )
{
	return (ec_dlptr) lt_dlsym( (lt_dlhandle) handle, symname );
}

EC_API const char *EcDLError( void )
{
	return lt_dlerror();
}
