/* ==========================================================================
 * ecgtk.h
 * ==========================================================================
 * elastiC gtk core (functions common to both gdk and gtk modules).
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sun Dec 26 10:32:20 MET 1999
 *
 *   $Id: ecgtk.h,v 1.1 2002/05/30 12:41:50 panta Exp $
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

#ifndef __ECGTK_H
#define __ECGTK_H

#include <elastic/elastic.h>

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gtk/gtkprivate.h>


/*
 * Description of each single value in
 * an enumeration/flag
 */
typedef struct
{
	const char *symbol;
	guint       value;
	EcUInt      symid;
} ec_gtk_enum;

/*
 * Description of an enumeration/flag
 */
typedef struct
{
	const char  *name;
	EcInt        num;
	ec_gtk_enum *info;
} ec_gtk_enum_info;

/*
 * Description of a boxed type
 */
typedef void *(*EC_GTK_BOXED_COPY)( void * );					/* Copy constructor pointer for a GTK_TYPE_BOXED */
typedef void  (*EC_GTK_BOXED_DESTROY)( void * );				/* Destructor pointer for a GTK_TYPE_BOXED       */

typedef struct
{
	const char           *name;
	EC_GTK_BOXED_COPY     copy;
	EC_GTK_BOXED_DESTROY  destroy;
	size_t                size;
} ec_gtk_boxed_info;


/* ========================================================================
 * E L A S T I C   T Y P E S
 * ======================================================================== */

/* gtkboxed */
extern EcInt tc_gtkboxed;

struct ec_gtk_boxed_data
{
	gpointer           ptr;
	ec_gtk_boxed_info *info;
};

#define EC_GTKBOXEDP(obj)		(EC_TYPEP(obj,tc_gtkboxed))
#define EC_GTKBOXED_DATA(obj)	((struct ec_gtk_boxed_data *) EC_USER(obj))
#define EC_GTKBOXED(obj)		(EC_GTKBOXED_DATA(obj)->ptr)
#define EC_GTKBOXED_INFO(obj)	(EC_GTKBOXED_DATA(obj)->info)

/* gtkobject */
extern EcInt tc_gtkobject;

#define EC_GTKOBJECTP(obj)		(EC_TYPEP(obj,tc_gtkobject))
#define EC_GTKOBJECT(obj)		((GtkObject *) EC_USER(obj))
#define EC_GTKOBJECT_TYPE(obj)	(GTK_OBJECT_TYPE( EC_GTKOBJECT(obj) ))


/* ========================================================================
 * A P I
 * ======================================================================== */

EcBool     ec_gtk_valid_enum( EC_OBJ obj, ec_gtk_enum_info *einfo );
EC_OBJ     ec_gtk_enum2obj( gint value, ec_gtk_enum_info *einfo );
gint       ec_gtk_obj2enum( EC_OBJ obj, ec_gtk_enum_info *einfo );

EcBool     ec_gtk_valid_flags( EC_OBJ obj, ec_gtk_enum_info *finfo );
EC_OBJ     ec_gtk_flags2obj( gint value, ec_gtk_enum_info *finfo );
gint       ec_gtk_obj2flags( EC_OBJ obj, ec_gtk_enum_info *finfo );

EcBool     ec_gtk_valid_boxed( EC_OBJ obj,   ec_gtk_boxed_info *binfo, EcBool nullok );
EC_OBJ     ec_gtk_boxed2obj( gpointer ptr, ec_gtk_boxed_info *binfo, EcBool copy );
gpointer   ec_gtk_obj2boxed( EC_OBJ obj );

EcBool     ec_gtk_valid_object( EC_OBJ obj, GtkType otype, EcBool nullok );
EC_OBJ     ec_gtk_object2obj( GtkObject *gtkobj );
GtkObject *ec_gtk_obj2object( EC_OBJ obj );

#endif __ECGTK_H
