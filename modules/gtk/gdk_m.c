/* ==========================================================================
 * gdk_m.c
 * ==========================================================================
 * elastiC gdk module core - gdk specific functions.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sun Dec 26 10:32:20 MET 1999
 *
 *   $Id: gdk_m.c,v 1.1 2002/05/30 12:41:58 panta Exp $
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
#include <elastic/debug.h>

#include "ecgtk.h"

/*
 * EC_G?K_MODULE          - indicates we are compiling g?k module
 * EC_G?K_DATASTRUCTURES  - enable inclusion of g?k module data descriptions
 *
 * These macros enable inclusion of data in g?kglue.[hc]
 */
#define EC_GDK_MODULE         1
#define EC_GDK_DATASTRUCTURES 1

#include "ecgtk.c"

/* Include generated data */
#include "gdkglue.c"

static void ec_gdk_register_special( void );

/* ------------------------------------------------------------------------
 * Initialization & cleanup
 * ------------------------------------------------------------------------ */

EC_OBJ ec_gdk_init( void )
{
	EcTypespec gtkboxed_spec = {
		/* type    */		0,
		/* name    */		"gtkboxed",

		/* copy    */		NULL,

		/* mark    */		gtkboxed_mark,
		/* free    */		gtkboxed_free,
		/* print   */		gtkboxed_print,

		/* hash    */		NULL,
		/* compare */		NULL,

		/* check   */		NULL,

		/* sequence */		NULL,
		/* numeric  */		NULL
	};

	EcTypespec gtkobject_spec = {
		/* type    */		0,
		/* name    */		"gtkobject",

		/* copy    */		NULL,

		/* mark    */		gtkobject_mark,
		/* free    */		gtkobject_free,
		/* print   */		gtkobject_print,

		/* hash    */		NULL,
		/* compare */		NULL,

		/* check   */		NULL,

		/* sequence */		NULL,
		/* numeric  */		NULL
	};

	EC_OBJ pkg_gdk = EC_NIL;

	/* Register enumeration & flag symbols */
	ec_gdk_init_symbols();

	/* Register types */

	tc_gtkboxed = EcAddType( &gtkboxed_spec );
	if (tc_gtkboxed == 0)
		return EC_NIL;

	tc_gtkobject = EcAddType( &gtkobject_spec );
	if (tc_gtkobject == 0)
		return EC_NIL;

	/* Register package */
	pkg_gdk = EcPackageIntroduce( "gdk" );

	/* Register automagically-generated functions in package */
	ec_gdk_register_functions();

	/* Register hand-written functions in packages */
	ec_gdk_register_special();

	/* Initialize gdk */
	gdk_init( NULL, NULL );

	return pkg_gdk;
}

void ec_gdk_cleanup( void )
{
	gdk_exit( 0 );
}

/* ------------------------------------------------------------------------
 * Hand-written functions
 * ------------------------------------------------------------------------ */

static void ec_gdk_register_special( void )
{
	/* Nothing right now ... */
}
