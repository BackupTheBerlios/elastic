/* ==========================================================================
 * skeleton.c
 * ==========================================================================
 * A 'skeleton' elastiC module.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Thu Nov 25 12:08:45 MET 1999
 *
 *   $Id: skeleton.c,v 1.1 2002/05/30 17:12:08 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1999 Marco Pantaleoni. All rights reserved.
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

#include <elastic/basic.h>
#include <elastic/elastic.h>
#include <elastic/private.h>

#include "config.h"
#include "skeleton.h"

EC_OBJ EcLibSkeleton_Show( EC_OBJ stack, EcAny userdata );

EC_OBJ EcLibSkeleton_Show( EC_OBJ stack, EcAny userdata )
{
	ec_fprintf( stdout, "This is a 'skeleton' module.\n" );
	return EC_NIL;
}

/* ------------------------------------------------------------------------
 * Initialization & cleanup
 * ------------------------------------------------------------------------ */

EC_OBJ ec_skeleton_init( void )
{
	EC_OBJ pkg;

	/* Register package */
	pkg = EcPackageIntroduce( "skeleton" );

	/* Register functions in package */
	EcAddPrimitive( "skeleton.show", EcLibSkeleton_Show );

	ec_fprintf( stderr, "'skeleton' module loaded.\n" );

	return pkg;
}

void ec_skeleton_cleanup( void )
{
	ec_fprintf( stderr, "'skeleton' module unloaded.\n" );
}
