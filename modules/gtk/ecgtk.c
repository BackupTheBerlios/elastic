/* ==========================================================================
 * ecgtk.c
 * ==========================================================================
 * elastiC gtk core.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sun Dec 26 10:32:20 MET 1999
 *
 *   $Id: ecgtk.c,v 1.1 2002/05/30 12:41:50 panta Exp $
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
#include <elastic/private.h>

#include "ecgtk.h"

/* Include generated data */
#include "gdkglue.h"
#include "gtkglue.h"

static EC_OBJ callback_cvt( EcInt i, EC_OBJ obj, void *into, EcDWord user1, EcDWord user2 );
static EC_OBJ file_cvt( EcInt i, EC_OBJ obj, void *into, EcDWord user1, EcDWord user2 );
static EC_OBJ enum_cvt( EcInt i, EC_OBJ obj, void *into, EcDWord user1, EcDWord user2 );
static EC_OBJ flags_cvt( EcInt i, EC_OBJ obj, void *into, EcDWord user1, EcDWord user2 );
static EC_OBJ boxed_cvt( EcInt i, EC_OBJ obj, void *into, EcDWord user1, EcDWord user2 );
static EC_OBJ object_cvt( EcInt i, EC_OBJ obj, void *into, EcDWord user1, EcDWord user2 );


static ec_gtk_enum_info  *find_enum_info( GtkType type );
static ec_gtk_enum_info  *find_flags_info( GtkType type );
static ec_gtk_boxed_info *find_boxed_info( GtkType type );

#if EC_GTK_MODULE
static void ec_gtk_callback_marshal( GtkObject *obj,
								  gpointer   data,
								  guint      nargs,
								  GtkArg    *args );
static void ec_gtk_callback_destroy( gpointer data );
#endif /* EC_GTK_MODULE */

static EC_OBJ arg2obj( GtkArg *a );
static void   obj2arg( GtkArg *a, EC_OBJ obj );


/*
 *  TYPEs
 */

/* =======
 * N O T E
 * =======
 *
 * gdk/gtk modules use a layered implementation: the gtk module 'stacks'
 * over gdk module.
 * The gdk module introduces two elastiC types: gtkboxed and gtkobject,
 * identified by tc_gtkboxed and tc_gtkobject.
 *
 * To work properly, gtk module *need* to know tc_gtkboxed and tc_gtkobject,
 * since these values are used by various functions in ecgtk.c
 *
 * SO, IT'S VITAL THAT THESE VALUES ARE *NOT* DECLARED STATIC. THEY MUST
 * BE GLOBAL !!!
 * (These are the only two shared variables. Note that we could also
 * implement a C API to pass these values from gdk to gtk module. It would
 * be better, since on other platforms the layered mechanism could break.)
 */

/* gtkboxed */
EcInt tc_gtkboxed = -1;

#if EC_GDK_MODULE
static void   gtkboxed_mark( EC_OBJ obj );
static void   gtkboxed_free( EC_OBJ obj );
static EcInt  gtkboxed_print( ec_string *str, EC_OBJ obj, EcBool detailed );
#endif /* EC_GDK_MODULE */

static EC_OBJ gtkboxed_new( gpointer ptr, ec_gtk_boxed_info *binfo );

/* gtkobject */
EcInt tc_gtkobject = -1;

#if EC_GDK_MODULE
static void   gtkobject_mark( EC_OBJ obj );
static void   gtkobject_free( EC_OBJ obj );
static EcInt  gtkobject_print( ec_string *str, EC_OBJ obj, EcBool detailed );
#endif /* EC_GDK_MODULE */

static EC_OBJ gtkobject_new( GtkObject *gtkobj );


/* ------------------------------------------------------------------------
 * gtkboxed
 * ------------------------------------------------------------------------ */

#if EC_GDK_MODULE

static void gtkboxed_mark( EC_OBJ obj )
{
	ASSERT( EC_GTKBOXEDP(obj) );
}

static void gtkboxed_free( EC_OBJ obj )
{
	ASSERT( EC_GTKBOXEDP(obj) );

	if (EC_GTKBOXED_INFO(obj)->destroy && EC_GTKBOXED(obj))
	{
		EC_GTKBOXED_INFO(obj)->destroy( EC_GTKBOXED(obj) );
		EC_GTKBOXED(obj) = NULL;
	}

	ec_free( EC_GTKBOXED_DATA(obj) );
	EC_GTKBOXED_DATA(obj) = NULL;
}

static EcInt gtkboxed_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	const char *typename;

	ASSERT( EC_GTKBOXEDP(obj) );

	typename = EC_GTKBOXED_INFO(obj)->name;
	if (detailed)
	{
		return ec_sprintf( str, "<gtkboxed '%s'>", typename ? typename : "UNKNOWN" );
	} else
	{
		return ec_sprintf( str, "<gtkboxed '%s'>", typename ? typename : "UNKNOWN" );
	}
}

#endif /* EC_GDK_MODULE */

static EC_OBJ gtkboxed_new( gpointer ptr, ec_gtk_boxed_info *binfo )
{
	struct ec_gtk_boxed_data *data;

	ASSERT( ptr );

	data = (struct ec_gtk_boxed_data *) ec_malloc( sizeof(struct ec_gtk_boxed_data) );
	if (! data)
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}

	data->ptr  = ptr;
	data->info = binfo;
	return EcMakeUser( tc_gtkboxed, data );
}

/* ------------------------------------------------------------------------
 * gtkobject
 * ------------------------------------------------------------------------ */

#if EC_GDK_MODULE

static void gtkobject_mark( EC_OBJ obj )
{
	ASSERT( EC_GTKOBJECTP(obj) );
}

static void gtkobject_free( EC_OBJ obj )
{
	ASSERT( EC_GTKOBJECTP(obj) );

	if (EC_GTKOBJECT(obj) && GTK_IS_OBJECT( EC_GTKOBJECT(obj) ))
	{
		if (GTK_IS_WIDGET( EC_GTKOBJECT(obj) ))
		{
			gtk_widget_destroy( GTK_WIDGET( EC_GTKOBJECT(obj) ) );
			EC_GTKOBJECT(obj) = NULL;
		}
		else
		{
			gtk_object_destroy( EC_GTKOBJECT(obj) );
			EC_GTKOBJECT(obj) = NULL;
		}
	}
}

static EcInt gtkobject_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	const char *typename;

	ASSERT( EC_GTKOBJECTP(obj) );

	if (! GTK_IS_OBJECT( EC_GTKOBJECT(obj) )) return 0;

	typename = gtk_type_name( EC_GTKOBJECT_TYPE(obj) );
	if (detailed)
	{
		return ec_sprintf( str, "<gtkobject '%s'>", typename ? typename : "UNKNOWN" );
	} else
	{
		return ec_sprintf( str, "<gtkobject '%s'>", typename ? typename : "UNKNOWN" );
	}
}

#endif /* EC_GDK_MODULE */

static EC_OBJ gtkobject_new( GtkObject *gtkobj )
{
	ASSERT( gtkobj );

	return EcMakeUser( tc_gtkobject, gtkobj );
}


/* ------------------------------------------------------------------------
 * Gtk to elastiC and vice-versa
 * ------------------------------------------------------------------------ */

/*
 *  ENUMs
 */

EcBool ec_gtk_valid_enum( EC_OBJ obj, ec_gtk_enum_info *einfo )
{
	EcUInt id;
	EcInt  i;
	ec_gtk_enum *evalue;

	if (! EC_SYMBOLP(obj))
		return FALSE;

	id = EC_SYMBOL(obj);
	evalue = einfo->info;

	for (i = 0; i < einfo->num; i++)
	{
		if (id == evalue[i].symid) return TRUE;
	}

	return FALSE;
}

EC_OBJ ec_gtk_enum2obj( gint value, ec_gtk_enum_info *einfo )
{
	EcInt  i;
	ec_gtk_enum *evalue;

	evalue = einfo->info;

	for (i = 0; i < einfo->num; i++)
	{
		if (value == evalue[i].value)
			return EcMakeSymbolFromId( evalue[i].symid );
	}

	return EC_NIL;
}

gint ec_gtk_obj2enum( EC_OBJ obj, ec_gtk_enum_info *einfo )
{
	EcUInt id;
	EcInt  i;
	ec_gtk_enum *evalue;

	if (! EC_SYMBOLP(obj))
		return -1;

	id = EC_SYMBOL(obj);
	evalue = einfo->info;

	for (i = 0; i < einfo->num; i++)
	{
		if (id == evalue[i].symid)
			return evalue[i].value;
	}

	return -1;
}

/*
 *  FLAGs
 */

EcBool ec_gtk_valid_flags( EC_OBJ obj, ec_gtk_enum_info *finfo )
{
	EC_OBJ el;
	EcUInt id;
	EcInt  i, j;
	ec_gtk_enum *fvalue;
	EcBool found;

	if (EC_NULLP(obj))
		return TRUE;

	if (! EC_ARRAYP(obj))
		return FALSE;

	fvalue = finfo->info;

	for (j = 0; j < EC_ARRAYLEN(obj); j++)
	{
		el = EcArrayGet( obj, j );
		if (! EC_SYMBOLP(el)) return FALSE;

		id = EC_SYMBOL(el);

		found = FALSE;
		for (i = 0; i < finfo->num; i++)
		{
			if (id == fvalue[i].symid) { found = TRUE; break; }
		}
		if (! found) return FALSE;
	}

	return TRUE;
}

EC_OBJ ec_gtk_flags2obj( gint value, ec_gtk_enum_info *finfo )
{
	EcInt  i;
	ec_gtk_enum *fvalue;
	EC_OBJ res;

	res = EcMakeArray( finfo->num );
	if (! EC_ARRAYP(res)) return res;

	fvalue = finfo->info;

	for (i = 0; i < finfo->num; i++)
	{
		if (value & fvalue[i].value)
			EcArrayPush( res, EcMakeSymbolFromId( fvalue[i].symid ) );
	}

	return res;
}

gint ec_gtk_obj2flags( EC_OBJ obj, ec_gtk_enum_info *finfo )
{
	EC_OBJ el;
	EcUInt id;
	EcInt  i, j;
	ec_gtk_enum *fvalue;
	EcBool found;
	gint res;

	if (EC_NULLP(obj))
		return 0;

	if (! EC_ARRAYP(obj))
		return -1;

	fvalue = finfo->info;

	res = 0;
	for (j = 0; j < EC_ARRAYLEN(obj); j++)
	{
		el = EcArrayGet( obj, j );
		if (! EC_SYMBOLP(el)) return -1;

		id = EC_SYMBOL(el);

		found = FALSE;
		for (i = 0; i < finfo->num; i++)
		{
			if (id == fvalue[i].symid) { res |= fvalue[i].value; found = TRUE; break; }
		}
		if (! found) return -1;
	}

	return res;
}

/*
 *  BOXEDs
 */

EcBool ec_gtk_valid_boxed( EC_OBJ obj, ec_gtk_boxed_info *binfo, EcBool nullok )
{
	if (EC_NULLP(obj) && nullok) return TRUE;

	if (binfo)
		return EC_GTKBOXEDP(obj) && (EC_GTKBOXED_INFO(obj) == binfo);
	else
		return EC_GTKBOXEDP(obj);
}

EC_OBJ ec_gtk_boxed2obj( gpointer ptr, ec_gtk_boxed_info *binfo, EcBool copy )
{
	if (! ptr) return EC_NIL;

	if (copy)
	{
		return binfo->copy ?
			gtkboxed_new( binfo->copy( ptr ), binfo ) :
			gtkboxed_new( ptr, binfo );
	} else
	{
		return gtkboxed_new( ptr, binfo );
	}
}

gpointer ec_gtk_obj2boxed( EC_OBJ obj )
{
	if (EC_NULLP(obj))
		return NULL;

	ASSERT( EC_GTKBOXEDP(obj) );
	return EC_GTKBOXED( obj );
}

/*
 *  GtkObject's
 */

EcBool ec_gtk_valid_object( EC_OBJ obj, GtkType otype, EcBool nullok )
{
	if (EC_NULLP(obj) && nullok) return TRUE;

	if (! EC_GTKOBJECTP(obj)) return FALSE;
	if (! EC_GTKOBJECT(obj)) return FALSE;
	if (! GTK_IS_OBJECT( EC_GTKOBJECT(obj) )) return FALSE;
	if (GTK_CHECK_TYPE( EC_GTKOBJECT(obj), otype )) return TRUE;

	return FALSE;
}

EC_OBJ ec_gtk_object2obj( GtkObject *gtkobj )
{
	if (! gtkobj) return EC_NIL;

	return gtkobject_new( gtkobj );
}

GtkObject *ec_gtk_obj2object( EC_OBJ obj )
{
	if (EC_NULLP(obj))
		return (GtkObject *)NULL;

	ASSERT( EC_GTKOBJECTP(obj) );
	return EC_GTKOBJECT( obj );
}


static ec_gtk_enum_info *find_enum_info( GtkType type )
{
	/* XXX - use a hash or something */
	int i;

	char *name = gtk_type_name( type );
	ec_gtk_enum_info *info;

	/* check in gdk */
	for (i = 0; i < ec_gdk_nenums; i++) {
		info = &ec_gdk_enums[i];

		if (strcmp( info->name, name ) == 0)
			return info;
	}

#if EC_GTK_DATASTRUCTURES
	/* check in gtk */
	for (i = 0; i < ec_gtk_nenums; i++) {
		info = &ec_gtk_enums[i];

		if (strcmp( info->name, name ) == 0)
			return info;
	}
#endif /* EC_GTK_DATASTRUCTURES */

	ASSERT( FALSE );
	return NULL;
}

static ec_gtk_enum_info *find_flags_info( GtkType type )
{
	/* XXX - use a hash or something */
	int i;

	char *name = gtk_type_name( type );
	ec_gtk_enum_info *info;

	/* check in gdk */
	for (i = 0; i < ec_gdk_nflags; i++) {
		info = &ec_gdk_flags[i];

		if (strcmp( info->name, name ) == 0)
			return info;
	}

#if EC_GTK_DATASTRUCTURES
	/* check in gtk */
	for (i = 0; i < ec_gtk_nflags; i++) {
		info = &ec_gtk_flags[i];

		if (strcmp( info->name, name ) == 0)
			return info;
	}
#endif /* EC_GTK_DATASTRUCTURES */

	ASSERT( FALSE );
	return NULL;
}

static ec_gtk_boxed_info *find_boxed_info( GtkType type )
{
	/* XXX - use a hash or something */
	int i;

	char *name = gtk_type_name( type );
	ec_gtk_boxed_info *info;

	/* check in gdk */
	for (i = 0; i < ec_gdk_nboxed; i++) {
		info = &ec_gdk_boxed[i];

		if (strcmp( info->name, name ) == 0)
			return info;
	}

#if EC_GTK_DATASTRUCTURES
	/* check in gtk */
	for (i = 0; i < ec_gtk_nboxed; i++) {
		info = &ec_gtk_boxed[i];

		if (strcmp( info->name, name ) == 0)
			return info;
	}
#endif /* EC_GTK_DATASTRUCTURES */

	ASSERT( FALSE );
	return NULL;
}

/* Parameter converters */

static EC_OBJ callback_cvt( EcInt i, EC_OBJ obj, void *into, EcDWord user1, EcDWord user2 )
{
	EC_OBJ *dst;

	dst = (EC_OBJ *) into;

	ASSERT( dst );

	if (! (EC_NULLP(obj) || EcIsCallable( obj )))
	{
		EcTypeError( EC_NIL, EC_NIL,							/* TODO: should pass function name */
					 i, tc_none, obj, TRUE, "expected a callable object (or nil)" );
		return Ec_ERROR;
	}

	return EC_NIL;
}

static EC_OBJ file_cvt( EcInt i, EC_OBJ obj, void *into, EcDWord user1, EcDWord user2 )
{
	int  *dst;
	FILE *fh;

	dst = (int *) into;

	ASSERT( dst );

	if (! EC_FILEP(obj))
	{
		EcTypeError( EC_NIL, EC_NIL,							/* TODO: should pass function name */
					 i, tc_none, obj, TRUE, "expected a file object" );
		return Ec_ERROR;
	}

	fh = EC_FILEH(obj);
	*dst = fileno( fh );
	return EC_NIL;
}

static EC_OBJ enum_cvt( EcInt i, EC_OBJ obj, void *into, EcDWord user1, EcDWord user2 )
{
	ec_gtk_enum_info *einfo;
	gint             *dst;

	dst   = (gint *) into;
	einfo = (ec_gtk_enum_info *) user1;

	ASSERT( dst );
	ASSERT( einfo );

	*dst = 0;

	if (! ec_gtk_valid_enum( obj, einfo ))
	{
		ec_string ds;
		ec_string_init( &ds, NULL );
		ec_sprintf( &ds, "expected a %s enumeration",
					einfo->name );
		EcTypeError( EC_NIL, EC_NIL,							/* TODO: should pass function name */
					 i, tc_none, obj, TRUE, ec_strdata( &ds ) );
		ec_string_cleanup( &ds );
		return Ec_ERROR;
	}

	*dst = ec_gtk_obj2enum( obj, einfo );
	return EC_NIL;
}

static EC_OBJ flags_cvt( EcInt i, EC_OBJ obj, void *into, EcDWord user1, EcDWord user2 )
{
	ec_gtk_enum_info *finfo;
	gint             *dst;

	dst   = (gint *) into;
	finfo = (ec_gtk_enum_info *) user1;

	ASSERT( dst );
	ASSERT( finfo );

	*dst = 0;

	if (! ec_gtk_valid_flags( obj, finfo ))
	{
		ec_string ds;
		ec_string_init( &ds, NULL );
		ec_sprintf( &ds, "expected %s flags",
					finfo->name );
		EcTypeError( EC_NIL, EC_NIL,							/* TODO: should pass function name */
					 i, tc_none, obj, TRUE, ec_strdata( &ds ) );
		ec_string_cleanup( &ds );
		return Ec_ERROR;
	}

	*dst = ec_gtk_obj2flags( obj, finfo );
	return EC_NIL;
}

static EC_OBJ boxed_cvt( EcInt i, EC_OBJ obj, void *into, EcDWord user1, EcDWord user2 )
{
	ec_gtk_boxed_info *binfo;
	gpointer          *dst;
	EcBool             nullok;

	dst    = (gpointer *) into;
	binfo  = (ec_gtk_boxed_info *) user1;
	nullok = (EcBool) user2;

	ASSERT( dst );
	ASSERT( binfo );

	*dst = NULL;

	if (! ec_gtk_valid_boxed( obj, binfo, nullok ))
	{
		ec_string ds;
		ec_string_init( &ds, NULL );
		ec_sprintf( &ds, "expected a %s",
					binfo->name );
		EcTypeError( EC_NIL, EC_NIL,							/* TODO: should pass function name */
					 i, tc_none, obj, TRUE, ec_strdata( &ds ) );
		ec_string_cleanup( &ds );
		return Ec_ERROR;
	}

	*dst = ec_gtk_obj2boxed( obj );
	return EC_NIL;
}

static EC_OBJ object_cvt( EcInt i, EC_OBJ obj, void *into, EcDWord user1, EcDWord user2 )
{
	GtkObject **dst;
	GtkType     otype;
	EcBool      nullok;

	dst    = (GtkObject **) into;
	otype  = (GtkType) user1;
	nullok = (EcBool) user2;

	ASSERT( dst );

	*dst = NULL;

	if (! ec_gtk_valid_object( obj, otype, nullok ))
	{
		ec_string ds;
		ec_string_init( &ds, NULL );
		ec_sprintf( &ds, "expected a %s%s", gtk_type_name( otype ), nullok ? " or null" : "" );
		EcTypeError( EC_NIL, EC_NIL,							/* TODO: should pass function name */
					 i, tc_none, obj, TRUE, ec_strdata( &ds ) );
		ec_string_cleanup( &ds );
		return Ec_ERROR;
	}

	*dst = ec_gtk_obj2object( obj );
	return EC_NIL;
}


/* ------------------------------------------------------------------------
 * Signal marshalling & related stuff
 * ------------------------------------------------------------------------ */

#if EC_GTK_MODULE

static void ec_gtk_callback_marshal( GtkObject *obj,
									 gpointer   data,
									 guint      nargs,
									 GtkArg    *args )
{
	EC_OBJ ecdata, callback, user;
	EC_OBJ res;
	EcInt  nuser = 0;
	EcInt  passed_nargs;
	EcInt  i;
	EC_OBJ *ec_args;

	ecdata = (EC_OBJ) data;

	if ((EC_ARRAYP(ecdata)) &&
		(EC_ARRAYLEN(ecdata) == 2))
	{
		callback = EC_ARRAYMEM(ecdata)[0];
		user     = EC_ARRAYMEM(ecdata)[1];
		nuser    = 1;
	} else
	{
		callback = ecdata;
		user     = EC_NIL;
		nuser    = 0;
	}

	if ((! EC_COMPILEDP(callback)) && (! EC_PRIMITIVEP(callback)))
	{
		res = EcTypeError( EC_NIL, EC_NIL,							/* TODO: should pass function name */
						   0, tc_none, callback, TRUE, "expected a valid callback" );
		goto finally;
	}

	/* Allocate space for arguments */
	passed_nargs = 1 + nargs + nuser;
	ec_args = alloca( passed_nargs * sizeof(EC_OBJ) );
	if (! ec_args)
	{
		res = EcMemoryError();
		goto finally;
	}

	/* Place arguments on frame */
	ec_args[0] = ec_gtk_object2obj( obj );
	for (i = 0; i < nargs; i++)
		ec_args[i + 1] = arg2obj( args + i );
	if (nuser)
		ec_args[nargs + 1] = user;

	/* Call */
	res = EcCallArgs( EC_NIL,									/* caller stack */
					  EC_NIL,									/* self         */
					  EC_NIL,									/* at_class     */
					  callback,									/* called obj   */
					  passed_nargs,								/* nargs        */
					  ec_args );								/* args         */

finally:
	if (EC_ERRORP(res))
	{
		/* Uncaught exception ! */
		if (EC_OBJECTP(PRIVATE(rt).exc) && EcIsOfClass( PRIVATE(rt).exc, EcExceptionClass ))
		{
			EC_OBJ msg;
			msg = EcGetInstanceVariable( PRIVATE(rt).exc, EcExceptionClass, "msg" );
			EcAlert( EcFatal, "uncaught callback exception %w: %w\n", PRIVATE(rt).exc, msg );
		} else
		{
			EcAlert( EcFatal, "uncaught callback exception %w\n", PRIVATE(rt).exc );
		}
	}

	if (args[nargs].type != GTK_TYPE_NONE)
		obj2arg( args + nargs, res );
}

void ec_gtk_callback_destroy( gpointer data )
{
	EcGCUnprotect( (EC_OBJ) data );
}

#endif /* EC_GTK_MODULE */

static EC_OBJ arg2obj( GtkArg *a )
{
	switch (GTK_FUNDAMENTAL_TYPE( a->type ))
    {
    case GTK_TYPE_CHAR:
		return EcMakeChar( (EcChar) GTK_VALUE_CHAR(*a) );
    case GTK_TYPE_INT:
		return EcMakeInt( (EcInt) GTK_VALUE_INT(*a) );
    case GTK_TYPE_POINTER:
		EcAlert( EcWarning, "gpointer in callback args" );
		return EcMakeInt( (EcInt) GTK_VALUE_POINTER(*a) );
    case GTK_TYPE_OBJECT:
		return ec_gtk_object2obj( GTK_VALUE_OBJECT(*a) );
    case GTK_TYPE_BOXED:
		return ec_gtk_boxed2obj( (gpointer) GTK_VALUE_BOXED(*a),
								 find_boxed_info( a->type ),
								 TRUE );
    case GTK_TYPE_FLAGS:
		return ec_gtk_flags2obj( (gint) GTK_VALUE_FLAGS(*a),
								 find_flags_info( a->type ) );
    default:
		EcAlert( EcWarning, "illegal type %s in callback args", gtk_type_name( a->type ) );
		return EC_NIL;
    }

	return EC_NIL;
}

static void obj2arg( GtkArg *a, EC_OBJ obj )
{
	switch (GTK_FUNDAMENTAL_TYPE( a->type ))
    {
    case GTK_TYPE_NONE:
		return;
    case GTK_TYPE_CHAR:
		*GTK_RETLOC_CHAR(*a) = (gchar) EC_CHAR(obj);
		break;
    case GTK_TYPE_INT:
		*GTK_RETLOC_INT(*a) = (gint) EC_INUM(obj);
		break;
    case GTK_TYPE_OBJECT:
		if (! (EC_GTKOBJECTP(obj) && GTK_CHECK_TYPE( EC_GTKOBJECTP(obj), a->type )))
		{
			EcAlert( EcWarning, "bad return type: expected '%s', found '%s'",
					 gtk_type_name( a->type ),
					 gtk_type_name( GTK_OBJECT_TYPE( EC_GTKOBJECT(obj) ) ) );
		}
		*GTK_RETLOC_OBJECT(*a) = EC_GTKOBJECT(obj);
		break;
    case GTK_TYPE_BOOL:
		*GTK_RETLOC_BOOL(*a) = EC_ASBOOLVAL(obj);
		break;
    default:
		EcAlert( EcWarning, "unhandled return type %s\n", gtk_type_name( a->type ) );
		break;
    }
}
