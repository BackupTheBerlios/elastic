/* ==========================================================================
 * gtk_m.c
 * ==========================================================================
 * elastiC gtk module - gtk specific functions.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sun Dec 26 10:32:20 MET 1999
 *
 *   $Id: gtk_m.c,v 1.1 2002/05/30 12:41:58 panta Exp $
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
#define EC_GTK_MODULE         1
#define EC_GTK_DATASTRUCTURES 1

/* inherited from gdk module */
extern ec_gtk_enum_info ec_gdk_enums[];
extern EcInt ec_gdk_nenums;
extern ec_gtk_enum_info ec_gdk_flags[];
extern EcInt ec_gdk_nflags;
extern ec_gtk_boxed_info ec_gdk_boxed[];
extern EcInt ec_gdk_nboxed;

#include "ecgtk.c"

/* Include generated data */
#include "gtkglue.c"

static void ec_gtk_register_special( void );


/* ------------------------------------------------------------------------
 * Initialization & cleanup
 * ------------------------------------------------------------------------ */

EC_OBJ ec_gtk_init( void )
{
	EC_OBJ pkg_gtk = EC_NIL;

	/* Register package */
	pkg_gtk = EcPackageIntroduce( "gtk" );

	/* Import gdk package */
	if (! EcPackageImport( pkg_gtk, "gdk", EC_NIL, FALSE ))
	return EC_NIL;

	/* Register enumeration & flag symbols */
	ec_gtk_init_symbols();

	/* Register types */

	/* Register automagically-generated functions in package */
	ec_gtk_register_functions();

	/* Register hand-written functions in package */
	ec_gtk_register_special();

	/* Initialize gtk */
	gtk_init( NULL, NULL );

	return pkg_gtk;
}

void ec_gtk_cleanup( void )
{
	gtk_exit( 0 );
}


/* ------------------------------------------------------------------------
 * Hand-written functions
 * ------------------------------------------------------------------------ */

static void ec_gtk_register_special( void )
{
	/* Nothing right now ... */
}
