/* ==========================================================================
 * dl-dl.c
 * ==========================================================================
 * elastiC Dynamic Loading Interface implementation for dlopen()/dlclose()/...
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Tue Apr  9 16:54:19 CEST 2002
 *
 *   $Id: dl-dl.c,v 1.1 2002/05/23 21:17:57 panta Exp $
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

#include <dlfcn.h>

EC_API EcBool EcDLInit( void )
{
	return TRUE;
}

EC_API void EcDLCleanup( void )
{
}

EC_API ec_dlhandle EcDLOpen( const char *filename )
{
	void *dlhandle = NULL;

	dlhandle = dlopen( filename, RTLD_LAZY | RTLD_GLOBAL );		/* for dlopen was: RTLD_LAZY | RTLD_GLOBAL */
	return (ec_dlhandle) dlhandle;
}

EC_API void EcDLClose( ec_dlhandle handle )
{
	dlclose( (void *) handle );
}

EC_API ec_dlptr EcDLSymbol( ec_dlhandle handle, const char *symname )
{
	return (ec_dlptr) dlsym( (void *) handle, symname );
}

EC_API const char *EcDLError( void )
{
	return dlerror();
}
