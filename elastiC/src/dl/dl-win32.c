/* ==========================================================================
 * dl-win32.c
 * ==========================================================================
 * elastiC Dynamic Loading Interface implementation for win32.
 * --------------------------------------------------------------------------
 *   AUTHOR:  George McBay         E-mail: gmcbay@hotmail.com
 *
 *   Created: Sun May 19 12:02:27 PDT 2002
 *
 *   $Id: dl-win32.c,v 1.1 2002/05/25 19:37:45 panta Exp $
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

/* VC++ */
#include <elastic/basic.h>
#include <elastic/debug.h>

#include <elastic/elastic.h>

#include <windows.h>


EC_API EcBool EcDLInit( void )
{
	return TRUE;
}


EC_API void EcDLCleanup( void )
{

}


EC_API ec_dlhandle EcDLOpen( const char *filename )
{
	HMODULE dlhandle;

	dlhandle = LoadLibrary(filename);

	if(dlhandle == NULL)
	{
		return NULL;
	}

	return (ec_dlhandle) dlhandle;
}


EC_API void EcDLClose( ec_dlhandle handle )
{

	FreeLibrary( (HMODULE) handle );
}


EC_API ec_dlptr EcDLSymbol( ec_dlhandle handle, const char *symname )
{
	FARPROC procAddress;

	procAddress = GetProcAddress( (HMODULE)handle, symname );

	if(procAddress == NULL)
	{
		return NULL;
	}

	return (ec_dlptr) procAddress;
}


EC_API const char *EcDLError( void )
{
	static char errormsg[256];

	FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(), 
		0,
		errormsg,
		sizeof(errormsg),
		NULL );

	return errormsg;
}

