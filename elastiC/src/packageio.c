/* ==========================================================================
 * packageio.c
 * ==========================================================================
 * Package loading & saving.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: 
 *
 *   $Id: packageio.c,v 1.5 2003/05/29 10:36:00 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1998-2002 Marco Pantaleoni. All rights reserved.
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

#define EC_STACK_RECYCLE_OPS 1

#include "basic.h"
#include "debug.h"

#include "elastic.h"
#include "private.h"
#include "compat.h"

#include "hashtable.h"
#include "hashdefs.h"
#include "stackrecycle.h"

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_LIMITS_H
#include <limits.h>
#endif

#define SHOW_MAP 0
#if !defined(WITH_STDIO)
#undef SHOW_MAP
#define SHOW_MAP 0
#endif

#if defined(WITH_STDIO) && SHOW_MAP
static int s_lev = 0;
static void indent(void) { int i; for (i = 0; i < s_lev; i++) fprintf(stderr, "  "); }
#define S_IN	do { s_lev++; } while (0)
#define S_OUT	do { s_lev--; } while (0)
#define PKG_OK(obj)			(EC_PACKAGEP(obj) && (EC_PACKAGE(obj)))
#define SHOW_IF_PKGOK(obj)	(PKG_OK(obj) ? (obj) : EC_NIL)
#define SHOW_READ(id,type,obj,cached)	\
do {                                                     \
	indent();                                            \
	ec_fprintf( stderr, "R  %3ld  type: %15s %8s %W\n",  \
				(long)(id), EcTypeName( (type) ),        \
				(cached) ? "(CACHED)" : "",              \
				(cached) ? (obj) : SHOW_IF_PKGOK(obj) ); \
} while (0)
#define SHOW_WRITE(id,type,obj,cached)	\
do {                                                     \
	indent();                                            \
	ec_fprintf( stderr, "W  %3ld  type: %15s %8s %W\n",  \
				(long)(id), EcTypeName( (type) ),        \
				(cached) ? "(CACHED)" : "", (obj) );     \
} while (0)
#define SHOW_READ_LATER(id,obj)	\
do {                                    \
	indent();                           \
	ec_fprintf( stderr, "   %3ld  %W\n", \
				(long)(id), (obj) );    \
} while (0)
#define SHOW_PKGREAD(id,obj,cached)		 SHOW_READ(id,tc_package,obj,cached)
#define SHOW_PKGWRITE(id,obj,cached)	 SHOW_WRITE(id,tc_package,obj,cached)
#else
#define S_IN								do {} while (0)
#define S_OUT								do {} while (0)
#define SHOW_READ(id,type,obj,cached)		do {} while (0)
#define SHOW_WRITE(id,type,obj,cached)		do {} while (0)
#define SHOW_READ_LATER(id,obj)	            do {} while (0)
#define SHOW_PKGREAD(id,type,obj,cached)	do {} while (0)
#define SHOW_PKGWRITE(id,type,obj,cached)	do {} while (0)
#endif

/* ========================================================================
 * P A C K A G E   I / O   R O U T I N E S   L A Y O U T
 * ------------------------------------------------------------------------
 *  Levels:
 *
 *  API      EcPackageSave
 *  API      EcPackageLoadStream
 *  API      EcPackageLoadByName
 *  API      EcPackageLoadByPath
 *
 *  mid      package_save_helper
 *  mid      package_load_helper
 *  mid      find_package
 *
 *  low      write_package
 *  low      read_package
 *  low      patch_bytecode
 *
 *  low      write_object
 *  low      read_object
 *
 * ======================================================================== */


/* ------------------------------------------------------------------------
 * PRIVATE DATA STRUCTURES
 * ------------------------------------------------------------------------ */
#define ECCMAGIC1	0x45434331
#define ECCMAGIC2	0x4D504543

#define tc_id			((EcWord)0xFFFF)
#define INVALID_OBJECT	((EC_OBJ)0xFFFFFFFF)

struct objectmap_struct
{
	ec_hash obj2id;
	ec_hash id2obj;
	EcUInt  nextid;
};
/* ------------------------------------------------------------------------ */


/* ------------------------------------------------------------------------
 * PRIVATE FUNCTIONS
 * ------------------------------------------------------------------------ */

static EcBool package_save_helper( EC_OBJ package, const char *pathname );
static EC_OBJ package_load_helper( ec_stream *stream, const char *name, EcBool execute, EcBool executeImported );
static EcBool find_package( const char *name, const char *pathname, ec_stream **stream_p, EC_OBJ *obj );
static EC_OBJ load_shared_package( const char *pkgname, const char *filename, EcBool msg );

/* Package saving & loading */
static void    write_package( objectmap map, ec_stream *stream, EC_OBJ obj );
static EC_OBJ  read_package( objectmap map, ec_stream *stream, EcBool executeImported );

/* Bytecode back-patching */
static void       patch_bytecode( objectmap map, EC_OBJ obj, EcInt npkgs, EcInt *pkg_now_at );

/* Low-level object I/O */
static void       write_object( objectmap map, ec_stream *stream, EC_OBJ obj );
static EC_OBJ     read_object(  objectmap map, ec_stream *stream, EcBool execute );

/* Low-level file I/O */
static void       write_byte( ec_stream *stream, EcByte datum );
static void       write_word( ec_stream *stream, EcWord datum );
static void       write_dword( ec_stream *stream, EcDWord datum );
static void       write_float( ec_stream *stream, EcFloat datum );
static void       write_string( ec_stream *stream, const char *string );
static EcByte     read_byte( ec_stream *stream );
static EcWord     read_word( ec_stream *stream );
static EcDWord    read_dword( ec_stream *stream );
static EcFloat    read_float( ec_stream *stream );
static char      *read_string( ec_stream *stream, ec_string *dst );

/* Object map: to avoid object processing recursion */
static objectmap  objectmap_create( void );
static void       objectmap_destroy( objectmap map );
static EcUInt     remember_object( objectmap map, EC_OBJ obj );
static EcUInt     object2id( objectmap map, EC_OBJ obj );
static EC_OBJ     id2object( objectmap map, EcUInt id );

/* ========================================================================
 * A P I
 * ======================================================================== */

EC_API EcBool EcPackageSave( EC_OBJ package, const char *pathname )
{
	ec_string realpathname;										/* final pathname used */

#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
	fprintf( stderr, "EcPackageSave  PATH: '%s'\n",
			 pathname ? pathname : "--" );
#endif

	EC_CHECK( package );

	ec_string_init( &realpathname, NULL );

	/*
	 * The pathname can be NULL. In that case we must obtain a suitable
	 * one by ourselves.
	 */
	if ((! pathname) || (*pathname == '\0'))
	{
		ec_string pkgname;

		ec_string_init( &pkgname, EC_STRDATA(EC_PACKAGENAME(package)) );

		/* Get the final part of the package name */
		if (EcNameQualified( &pkgname ))
			EcNameSuffix( &realpathname, &pkgname );
		else
			ec_strcatd( &realpathname, &pkgname );
		ec_strcat( &realpathname, EC_COMPILEDSUFFIX, 0 );

		ec_string_cleanup( &pkgname );
	} else
	{
		/* Pathname specified */
		ec_strcat( &realpathname, pathname, 0 );
	}

	/* Save the beast */
	if (! package_save_helper( package, ec_strdata( &realpathname ) ))
		goto onError;

	/* OK */
	ec_string_cleanup( &realpathname );
	return TRUE;

onError:
	ec_string_cleanup( &realpathname );
	return FALSE;
}

EC_API EC_OBJ EcPackageLoadStream( ec_stream  *stream,			/* input stream      */
								   const char *name,			/* name can be null  */
								   EcBool execute,
								   EcBool executeImported )
{
	/* This function can be used *only* for compiled bytecode packages! */

	EC_OBJ  obj;

	ASSERT( stream );

#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
	fprintf( stderr, "LOADING PACKAGE  NAME: %s  (from stream) (exec: %s, %s)\n",
			 name ? name : "--",
			 execute ? "YES" : "NO",
			 executeImported ? "YES" : "NO" );
#endif

	if (name)
	{
		/* Package already present ? */
		obj = _ec_package_loaded( name );
		if (EC_PACKAGEP(obj))
		{
			EC_CHECK( obj );
#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
			fprintf( stderr, "PACKAGE '%s' LOADED. (ALREADY IN)\n\n", name ? name : "(no name)" );
#endif
			return obj;
		}
	}

	ASSERT( PRIVATE(patchmap) );

	obj = package_load_helper( stream, name, execute, executeImported );
	if (! EC_PACKAGEP(obj))
	{
		if (name)
			ec_msg_printf( "Can't load package \"%s\"\n", name );
		else
			ec_msg_printf( "Can't load package\n" );
		goto onError;
	}

	EC_CHECK( obj );

#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
	ec_msg_printf( "PACKAGE '%w' LOADED.\n\n", EC_PACKAGEP(obj) ? EC_PACKAGENAME(obj) : EC_NIL );
#endif

	EC_CHECKALL();
	EC_CHECK( obj );
	return obj;

onError:
	if (name)
		ec_msg_printf( "PACKAGE '%s' NOT LOADED. *ERROR*\n\n", name );
	else
		ec_msg_printf( "PACKAGE NOT LOADED. *ERROR*\n\n" );
	return Ec_ERROR;
}

EC_API EC_OBJ EcPackageLoadByName( const char *name,
								   EcBool execute, EcBool executeImported )
{
	EC_OBJ     obj;
	ec_stream *stream = NULL;

	ASSERT( name );

#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
	fprintf( stderr, "LOADING PACKAGE BY NAME: %s  (exec: %s, %s)\n",
			 name ? name : "--",
			 execute ? "YES" : "NO",
			 executeImported ? "YES" : "NO" );
#endif

	/* Package already present ? */
	obj = _ec_package_loaded( name );
	if (EC_PACKAGEP(obj))
	{
		EC_CHECK( obj );
#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
		fprintf( stderr, "PACKAGE '%s' LOADED. (ALREADY PRESENT)\n\n", name );
#endif
		return obj;
	}

	stream = NULL;
	obj = EC_NIL;
	if (! find_package( name, NULL, &stream, &obj ))
	{
		ec_msg_printf( "Can't find package \"%s\"\n", name );
		goto onError;
	}

	if (EC_NNULLP(obj))
	{
		/* Got a native C package */

	} else if (stream)
	{
		/* Got a compiled bytecode package */

		ASSERT( PRIVATE(patchmap) );

		obj = package_load_helper( stream, name, execute, executeImported );
		if (! EC_PACKAGEP(obj))
		{
			ec_msg_printf( "Can't load package \"%s\"\n", name );
			goto onError;
		}

		EC_CHECK( obj );

		ec_stream_close( stream );
	}
#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
	fprintf( stderr, "PACKAGE '%s' LOADED.\n\n", name );
#endif

	EC_CHECKALL();
	EC_CHECK( obj );
	return obj;

onError:
	if (stream) ec_stream_close( stream );
	ec_msg_printf( "PACKAGE '%s' NOT LOADED. *ERROR*\n\n", name );
	return Ec_ERROR;
}

EC_API EC_OBJ EcPackageLoadByPath( const char *pathname,
								   EcBool execute, EcBool executeImported )
{
	EC_OBJ     obj;
	ec_stream *stream = NULL;

	ASSERT( pathname );

#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
	fprintf( stderr, "LOADING PACKAGE BY PATH: %s  (exec: %s, %s)\n",
			 pathname ? pathname : "--",
			 execute ? "YES" : "NO",
			 executeImported ? "YES" : "NO" );
#endif

	stream = NULL;
	obj = EC_NIL;
	if (! find_package( NULL, pathname, &stream, &obj ))
	{
		ec_msg_printf( "Can't find package with path \"%s\"\n", pathname );
		goto onError;
	}

	if (EC_NNULLP(obj))
	{
		/* Got a native C package */

	} else if (stream)
	{
		/* Got a compiled bytecode package */

		ASSERT( PRIVATE(patchmap) );

		obj = package_load_helper( stream, NULL, execute, executeImported );
		if (! EC_PACKAGEP(obj))
		{
			ec_msg_printf( "Can't load package with path \"%s\"\n", pathname );
			goto onError;
		}

		EC_CHECK( obj );

		ec_stream_close( stream );
	}
#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
	if (EC_PACKAGEP(obj))
		ec_msg_printf( "PACKAGE '%w' LOADED.\n\n", EC_PACKAGENAME(obj) );
	else
		ec_msg_printf( stderr, "PACKAGE with path '%s' LOADED.\n\n", pathname );
#endif

	EC_CHECKALL();
	EC_CHECK( obj );
	return obj;

onError:
	if (stream) ec_stream_close( stream );
	ec_msg_printf( "PACKAGE with path '%s' NOT LOADED. *ERROR*\n\n", pathname );
	return Ec_ERROR;
}


/* ========================================================================
 * P R I V A T E
 * ======================================================================== */

/* ------------------------------------------------------------------------
 * PACKAGE I/O HELPER FUNCTIONS
 * ------------------------------------------------------------------------ */

static EcBool package_save_helper( EC_OBJ package, const char *pathname )
{
	ec_stream *stream;
	EcInt  i;
	EcInt  ncurpkg;
	objectmap map = NULL;
	EC_OBJ exc;

#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
	fprintf( stderr, "SAVING PACKAGE TO FILE: '%s'\n",
			 pathname ? pathname : "--" );
#endif

	EC_CHECK( package );

	stream = ec_filestream_fopen( pathname, "wb", &exc );
	if ((! stream) || EC_ERRORP(exc))
		return FALSE;

	/*
	 * Package header
	 *
	 * Each package has an import list (of packages needed).
	 *
	 * HEADER FORMAT:
	 *
	 *   magic
	 *   package name (string)
	 *   import list
	 *   current number of globals
	 *
	 * We use as the source of import list, the currently loaded packages
	 * in the interpreter (those used for the compilation).
	 *
	 *   IMPORT LIST FORMAT:
	 *      number of packages, ordered list of names
	 *      Each name: lenght, data
	 */

	/* MAGIC */
	write_dword( stream, ECCMAGIC1 );
	write_dword( stream, ECCMAGIC2 );

	/* package name */
	write_string( stream, EC_STRDATA(EC_PACKAGENAME(package)) );

	/* # of (core) globals */
	write_dword( stream, PRIVATE(ncoreglobals) );

	/* Find our position in loaded packages */
	ncurpkg = -1;
	for (i = 0; i < PRIVATE(npackages); i++)
	{
		if (package == PRIVATE(package)[i].obj) {
			ncurpkg = i;
			break;
		}
	}
	ASSERT( ncurpkg >= 0 );

	/* Import list */
#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
	fprintf(stderr, "SAVE HELPER:\n");
	fprintf(stderr, "   name     : %s\n",  EC_STRDATA(EC_PACKAGENAME(package)));
	fprintf(stderr, "   npackages: %ld\n", (long)PRIVATE(npackages));
	fprintf(stderr, "   ncurpkg  : %ld\n", (long)ncurpkg);
#endif
	write_dword( stream, PRIVATE(npackages) );
	write_dword( stream, ncurpkg );								/* ourselves */
	for (i = 0; i < PRIVATE(npackages); i++)
	{
#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
		fprintf(stderr, "   pkg[%ld]  : %s\n",  (long)i, ec_strdata( PRIVATE(package)[i].name ));
#endif
		write_string( stream, ec_strdata( PRIVATE(package)[i].name ) );
	}


	/* ------------------------------------------------------------
	 * Write the real stuff
	 * ------------------------------------------------------------ */

	map = objectmap_create();
	if (! map) goto onError;

	/* Put global objects in map */
	remember_object( map, EC_NIL );
	for (i = 0; i < PRIVATE(ncoreglobals); i++)
		remember_object( map, PRIVATE(global[i]) );

	/* write this package */
	write_package( map, stream, package );

	objectmap_destroy( map );
	map = NULL;

	ec_stream_close( stream );
	return TRUE;

onError:
	if (map) objectmap_destroy( map );
	ec_stream_close( stream );
	return FALSE;
}

static EC_OBJ package_load_helper( ec_stream *stream, const char *name, EcBool execute, EcBool executeImported )
{
	EC_OBJ             obj;
	char              *lname = NULL;							/* package (local) name  */
	ec_string          packagename;								/* saved package name    */

	EcInt              saved_ncoreglobals;						/* number of core globals at save time  */
	EcInt              saved_npackages;							/* number of packages at save time      */
	EcInt              saved_ncurpkg;							/* number of saved package at save time */

	EcInt             *pkg_now_at;								/* package translation table */
	objectmap          map = NULL;								/* object map                */

	ec_string          pkgname;
	EC_OBJ             pkg;
	EcInt i;

	char              *strp;
	struct package *newPackages = NULL;
	EcInt           n;
	EcBool          started = FALSE;

	ASSERT( stream );

	ec_string_init( &packagename, NULL );
	ec_string_init( &pkgname, NULL );

#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
	fprintf( stderr, "READING PACKAGE '%s'  (exec: %s, %s)\n",
			 name ? name : "UNSPECIFIED", execute ? "YES" : "NO", executeImported ? "YES" : "NO" );
#endif

	/* MAGIC */
	if ((read_dword( stream ) != ECCMAGIC1) ||
		(read_dword( stream ) != ECCMAGIC2))
		goto onError;

	/* package name */
	strp = read_string( stream, &packagename );

	if (name)
		lname = ec_stringdup( name );
	else
		lname = ec_stringdup( ec_strdata(&packagename) );

	if ((! ec_strdata(&packagename)) || (! ec_strdata(&packagename)[0]))
		goto onError;

	/* Package already present ? */
	obj = _ec_package_loaded( ec_strdata(&packagename) );
	if (EC_PACKAGEP(obj))
	{
		ec_string_cleanup( &pkgname );
        ec_string_cleanup( &packagename );
		EC_CHECK( obj );
		ec_free( lname );
		return obj;
	}

	if (strcmp( ec_strdata(&packagename), lname ) != 0)
	{
/*		fprintf( stderr, "REQUESTED: '%s'\n", lname );
		fprintf( stderr, "FOUND    : '%s'\n", ec_strdata(&packagename) ); */
		EcAlert( EcError, "loaded a package with a different name from requested" );
		goto onError;
	}

	/* # of globals */
	saved_ncoreglobals = read_dword( stream );
	if (saved_ncoreglobals != PRIVATE(ncoreglobals))
	{
		EcAlert( EcError, "loaded a package saved with a different version of the interpreter" );
		goto onError;
	}

	/* Import list */
	saved_npackages = read_dword( stream );
	saved_ncurpkg   = read_dword( stream );

#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
	fprintf(stderr, "LOAD HELPER:\n");
	fprintf(stderr, "   saved_name     : %s\n",  lname);
	fprintf(stderr, "   saved_npackages: %ld\n", (long)saved_npackages);
	fprintf(stderr, "   saved_ncurpkg  : %ld\n", (long)saved_ncurpkg);
#endif

	if (saved_npackages < saved_ncurpkg)
		goto onError;

	/*
	 * Add this package to package list
	 *
	 * If later we'll discover that we can't complete the
	 * operation, we'll remove this entry.
	 */
	n = PRIVATE(npackages) + 1;
	newPackages = ec_realloc( PRIVATE(package), n * sizeof(struct package) );
	if (! newPackages)
		goto onError;
	PRIVATE(package)   = newPackages;
	PRIVATE(npackages) = n;
	PRIVATE(package)[n-1].name     = ec_string_create( lname );
	PRIVATE(package)[n-1].obj      = EcTrueObject;				/* ... for the moment ... (we can't use EC_NIL) */
	PRIVATE(package)[n-1].dlhandle = NULL;
	started = TRUE;

	/* build package translation map */
	pkg_now_at = alloca( saved_npackages * sizeof(EcInt) );
	if (! pkg_now_at) goto onError;

	for (i = 0; i < saved_npackages; i++)
	{
		strp = read_string( stream, &pkgname );
		if (! strp) goto onError;

#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
		fprintf(stderr, "   pkg[%ld]  : %s\n",  (long)i, ec_strdata(&pkgname));
		fprintf( stderr, "IMP %s\n", ec_strdata(&pkgname) );
#endif
		if (i != saved_ncurpkg)
		{
			/*
			 * This is a different package from that we are loading.
			 * Import it.
			 */

			/* Already present ? */
			if (_ec_package_position( ec_strdata(&pkgname) ) >= 0)
			{
				pkg_now_at[i] = _ec_package_position( ec_strdata(&pkgname) );
				continue;
			}

			ASSERT( strcmp( ec_strdata(&pkgname), lname ) != 0 );

			pkg = EcPackageLoadByName( ec_strdata(&pkgname),
									   executeImported, executeImported ); /* TODO: CHECK EXECUTION UPON IMPORT WHEN COMPILING */
			if (! EC_PACKAGEP(pkg))
			{
				EcAlert( EcError, "can't import package '%s'", ec_strdata(&pkgname) );
				goto onError;
			}

			pkg_now_at[i] = _ec_package_position_o( pkg );
		} else
			pkg_now_at[i] = n-1;								/* last position */

#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
		fprintf( stderr, "Package %ld  now at  %ld (name: %s)\n", (long)i, (long)pkg_now_at[i], ec_strdata(&pkgname) );
#endif
	}


	map = objectmap_create();
	if (! map) goto onError;

	/* Put global objects in map */
	ASSERT( saved_ncoreglobals == PRIVATE(ncoreglobals) );
	remember_object( map, EC_NIL );
	for (i = 0; i < PRIVATE(ncoreglobals); i++)
		remember_object( map, PRIVATE(global[i]) );

	obj = read_package( map, stream, executeImported );
	if (! EC_PACKAGEP(obj))
		goto onError;
	PRIVATE(package)[n-1].obj = obj;

	objectmap_destroy( map );
	map = NULL;

	/*
	 * Backpatch imported bytecode for public symbol locations
	 *
	 * NOTE: It isn't sufficient to correct the bytecode of the package
	 *       but we must correct all the bytecode got from this loading
	 *       (read: scan & correct also in the literal frame).
	 */
	if (saved_npackages)
	{
		ASSERT( saved_ncoreglobals == PRIVATE(ncoreglobals) );

#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
		ec_fprintf(stderr, "@@ PATCH BYTECODE from PACKAGE_LOAD_HELPER(%s): %w\n", lname, obj);
#endif
		ASSERT( PRIVATE(patchmap) );
		patch_bytecode( PRIVATE(patchmap), obj, saved_npackages, pkg_now_at );
	}

	/* completed */
	started = FALSE;

	/* Ok, now execute the package code, if we have to */
	if (execute)
	{
#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
		fprintf( stderr, "Going to execute package '%s'.\n", lname );
#endif
		EC_CHECK( obj );
		EcMainExecute( obj );
		EC_CHECK( obj );
#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
		fprintf( stderr, "DONE.\n" );
#endif
	}

	EC_CHECK( obj );
	ec_free( lname );
	ec_string_cleanup( &pkgname );
	ec_string_cleanup( &packagename );
	return obj;

onError:
	if (started)
	{
		ASSERT( newPackages );
		if (newPackages)
		{
			/* We have not completed the operation. Undo it. */
			PRIVATE(npackages)--;
		}
		started = FALSE;
	}
	if (map) objectmap_destroy( map );
	ec_free( lname );
	ec_string_cleanup( &pkgname );
	ec_string_cleanup( &packagename );
	return Ec_ERROR;
}

static EcBool find_package( const char *name, const char *pathname, ec_stream **stream_p, EC_OBJ *objp )
{
	ec_string         pkgname, pathname_tail, realpathname;
	char             *b;
	int               seplen;
	char             *envres, *libpath;
	char             *tok;
	const char       *tok_s, *tokp;
	EcInt             tokl;
	ec_stream        *stream;
	EC_OBJ            obj;
	const char       *suffix,
		             *suffixes[] = { SO_SUFFIX, EC_COMPILEDSUFFIX, NULL };
	const int         nsuffixes = 2;
	int               i;
	EcBool            found = FALSE;

	EC_OBJ            exc = EC_NIL;

	if (stream_p) *stream_p  = NULL;
	if (objp)     *objp = EC_NIL;

	/* Pathname given ? */
	if (pathname)
	{
#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
		fprintf( stderr, "pathname given: '%s'\n", pathname );
#endif
		stream = ec_filestream_fopen( pathname, "rb", &exc );
		if ((! stream) || EC_ERRORP(exc))
			return FALSE;

		/* Try as a shared library */
		obj = load_shared_package( name, pathname, FALSE );
		if (EC_NNULLP(obj))
		{
			if (! EC_PACKAGEP(obj))
			{
				/* An exception has been raised, or no package has been loaded */
				ASSERT( EC_PACKAGEP(obj) );
				return FALSE;
			}
			ec_stream_close( stream );
			stream = NULL;
		}

		if (stream_p) *stream_p = stream;
		if (objp)     *objp = obj;

		return TRUE;
	}

	ASSERT( name );

#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
	fprintf( stderr, "pkg name: '%s'\n", name );
#endif

	ec_string_init( &pkgname, name );
	ec_string_init( &pathname_tail, NULL );
	ec_string_init( &realpathname, NULL );

	/* Try all known places */

	found = FALSE;
	obj   = EC_NIL;
	for (i = 0; i < nsuffixes; i++)
	{
		/* Try EC_ELASTICLIBVAR directories */
		envres = getenv( EC_ELASTICLIBVAR );
		if (! envres)
		{
			char buf[PATH_MAX];

			if (! getcwd( buf, PATH_MAX ))
				strncpy( buf, EC_CWD, strlen(EC_CWD) );

			libpath = alloca( strlen( buf ) + strlen( EC_ENVSEPARATOR ) + strlen( EC_DEFAULTLIBPATH ) + 1 );
			ASSERT( libpath );
			strcpy( libpath, buf );
			strcat( libpath, EC_ENVSEPARATOR );
			strcat( libpath, EC_DEFAULTLIBPATH );
		}
		else
		{
			libpath = alloca( strlen( envres ) + 1 );
			ASSERT( libpath );
			strcpy( libpath, envres );
		}

		seplen = strlen( EC_PATHSEPARATOR );

		suffix = suffixes[i];

		/* Build filename */
		EcPackagePathFromName( &pathname_tail, &pkgname, suffix );

		tok = alloca( strlen( libpath ) + 1 );
		ASSERT( tok );

		tokp = libpath;
		stream = NULL; obj = EC_NIL;
		while (TRUE)
		{
			tok_s = ec_string_tokenize( &tokp, &tokl, EC_ENVSEPARATOR[0] );
			if (! tok_s) break;

			ec_string_sub( tok, tok_s, 0, tokl );

			/* clear pathname */
			ec_string_init( &realpathname, NULL );

			ec_strcat( &realpathname, tok, 0 );
			b = ec_strdata( &realpathname );
			if (strcmp( b + strlen( b ) - seplen, EC_PATHSEPARATOR ) != 0)
				ec_strcat( &realpathname, EC_PATHSEPARATOR, 0 );
			ec_strcatd( &realpathname, &pathname_tail );

#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
			fprintf( stderr, "Trying file '%s'\n", ec_strdata( &realpathname ) );
#endif
			stream = ec_filestream_fopen( ec_strdata( &realpathname ), "rb", &exc );
			if ((! stream) || EC_ERRORP(exc))
				continue;

			/* GOT ! */
			found = TRUE;
			break;
		}

		if (found)
		{
			/* GOT ! */

			if (i == 0)											/* native C package */
			{
				ec_stream_close( stream );
				stream = NULL;
				obj = load_shared_package( name, ec_strdata( &realpathname ), TRUE );
				if (EC_NULLP(obj)) found = FALSE;
			}

			break;
		}
	}

	if (stream_p) *stream_p = stream;
	if (objp)     *objp     = obj;

	ec_string_cleanup( &pathname_tail );
	ec_string_cleanup( &realpathname );
	return found;
}

static EC_OBJ load_shared_package( const char *pkgname, const char *filename, EcBool msg )
{
	EC_OBJ               pkg         = EC_NIL;
	ec_dlhandle          dlhandle    = NULL;
	package_init_fcn     init_fcn    = NULL;
	package_cleanup_fcn  cleanup_fcn = NULL;
	EcInt                i;
	char                 buf[256];
	EcInt                fnamelen;

	char                *pkgname_normalized, *p;
	EcInt                normalized_len;

	if (filename)
		fnamelen = strlen( filename );
	else
		return EC_NIL;
	if ((fnamelen >= strlen(EC_COMPILEDSUFFIX)) &&
		(strcmp(filename + fnamelen - strlen(EC_COMPILEDSUFFIX), EC_COMPILEDSUFFIX) == 0))
		return EC_NIL;
	dlhandle = EcDLOpen( filename );							/* for dlopen was: RTLD_LAZY | RTLD_GLOBAL */
	if (! dlhandle)
	{
		if (msg)
			ec_msg_printf( "dlopen: %s\n", EcDLError() );
		return EC_NIL;
	}

	/*
	 * Normalize pkgname
	 *
	 * We support nested C packages (with names like pkgpath.name)
	 * but C doesn't support such identifiers, so we have to normalize
	 * them (substituting double underscores: "__" for dots ".")
	 */
	normalized_len = 0;
	for (i = 0; i < strlen(pkgname); i++)
		if (pkgname[i] == '.')
			normalized_len += 2;
		else
			normalized_len++;
	pkgname_normalized = alloca( normalized_len + 1 );
	if (! pkgname_normalized)
	{
		/* Out of memory */
		if (msg)
			ec_msg_printf( "load_shared_package: out of memory\n" );
		return EcMemoryError();
	}
	p = pkgname_normalized;
	for (i = 0; i < strlen(pkgname); i++)
	{
		if (pkgname[i] == '.')
		{
			*p++ = '_';
			*p++ = '_';
		}
		else
			*p++ = pkgname[i];
	}
	*p++ = '\0';

	sprintf( buf, "ec_%s_init", pkgname_normalized );
	init_fcn = (package_init_fcn) EcDLSymbol( dlhandle, buf );
	if (! init_fcn)
		goto onerror;

	sprintf( buf, "ec_%s_cleanup", pkgname_normalized );
	cleanup_fcn = (package_cleanup_fcn) EcDLSymbol( dlhandle, buf );

	pkg = (*init_fcn)();
	if (! EC_PACKAGEP(pkg))
		goto onerror;

	/*
	 * Set the proper dlhandle, init & cleanup functions.
	 *
	 * The init_fcn on the library side should have set
	 * the proper fields through EcPackageIntroduce(),
	 * so we have only to scan the package list.
	 */
	for (i = 0; i < PRIVATE(npackages); i++)
	{
		if (PRIVATE(package)[i].obj == pkg)
		{
			PRIVATE(package)[i].dlhandle      = dlhandle;
			PRIVATE(package)[i].dlinit_fcn    = init_fcn;
			PRIVATE(package)[i].dlcleanup_fcn = cleanup_fcn;
		}
	}

	return pkg;

 onerror:
	if (dlhandle) EcDLClose( dlhandle );
	return EC_NIL;
}

/* ------------------------------------------------------------------------
 * PACKAGE SAVING & LOADING
 * ------------------------------------------------------------------------ */

static void write_package( objectmap map, ec_stream *stream, EC_OBJ obj )
{
	EcInt i, l;
	EcUInt id;

	S_IN;

	ASSERT( map );
	ASSERT( EC_PACKAGEP(obj) );

	EC_CHECK(obj);

	/* Object already in map ? */
	id = object2id( map, obj );
	if (id)
	{
		write_word( stream, tc_id );
		write_dword( stream, id );
		if (EC_NNULLP(obj))
			write_word( stream, EC_TYPE(obj) );
		else
			write_word( stream, tc_none );

		SHOW_WRITE(id, EC_NNULLP(obj) ? EC_TYPE(obj) : tc_none, obj, TRUE);
		S_OUT;
		return;
	}

	/* No, put it */
	id = remember_object( map, obj );

	SHOW_WRITE(id, EC_NNULLP(obj) ? EC_TYPE(obj) : tc_none, obj, FALSE);

	write_word( stream, EC_TYPE(obj) );

	write_dword( stream, (EcDWord)EcVersionNumber() );

	write_object( map, stream, EC_PACKAGENAME(obj) );
	write_object( map, stream, EC_PACKAGECODE(obj) );
	write_object( map, stream, EC_PACKAGEFRAME(obj) );
	write_object( map, stream, EC_PACKAGESOURCE(obj) );
	write_object( map, stream, EC_PACKAGEIMPORT(obj) );
	l = EC_PACKAGENEXPORT(obj);
	write_dword( stream, l );
	for (i = 0; i < l; i++)
	{
		write_string( stream, EcSymbolAt( EC_PACKAGEEXPORT(obj)[i].sym ) );
		write_byte( stream, EC_PACKAGEEXPORT(obj)[i].isconst );
		write_dword( stream, EC_PACKAGEEXPORT(obj)[i].pos );
	}
	S_OUT;
}

static EC_OBJ read_package( objectmap map, ec_stream *stream, EcBool executeImported )
{
	EcWord type;
	EcUInt id;
	EC_OBJ obj = EC_NIL;

	EC_OBJ name, code, frame, source, import;
	EcInt i, l, pos;
	EcBool isconst;
	ec_string str;
	EcUInt ec_ver;

	S_IN;

	ASSERT( map );

	type = read_word( stream );

	ASSERT( type != tc_none );

	if (type == tc_id)
	{
		/* Object already in map ? */
		id   = read_dword( stream );
		type = read_word( stream );
		obj  = id2object( map, id );
		ASSERT( obj != INVALID_OBJECT );

		SHOW_READ(id, type, obj, TRUE);

		ASSERT( EC_NNULLP(obj) ? EC_TYPEP(obj, type) : (type == tc_none) );

		EC_CHECK(obj);
		S_OUT;
		return obj;
	}

	if (type != tc_package)
	{
		S_OUT;
		return Ec_ERROR;
	}

	ec_ver = read_dword( stream );
	if (ec_ver != EcVersionNumber() )
	{
		EcAlert( EcWarning, "loading a package saved with a different version on the interpreter" );
		/* return Ec_ERROR; */
	}

	obj = EcAllocObject();
	if (EC_ERRORP(obj))
	{
		S_OUT;
		return obj;
	}
	EC_OBJTYPE(obj) = type;

	if (EC_NNULLP(obj))
	{
		id = remember_object( map, obj );

		SHOW_READ(id, type, obj, FALSE);
	}

	EC_PACKAGE(obj) = ec_malloc( sizeof(EcPackage) );
	ASSERT( EC_PACKAGE(obj) );

	name   = read_object( map, stream, executeImported );
	code   = read_object( map, stream, executeImported );
	frame  = read_object( map, stream, executeImported );
	source = read_object( map, stream, executeImported );
	import = read_object( map, stream, executeImported );

	EC_PACKAGENAME(obj)   = name;
	EC_PACKAGECODE(obj)   = code;
	EC_PACKAGEFRAME(obj)  = frame;
	EC_PACKAGESOURCE(obj) = source;
	EC_PACKAGEIMPORT(obj) = import;

	EC_PACKAGEEXPORT(obj)  = NULL;
	EC_PACKAGENEXPORT(obj) = 0;
	l = read_dword( stream );
	for (i = 0; i < l; i++)
	{
		ec_string_init( &str, NULL );
		read_string( stream, &str );
		isconst = read_byte( stream );
		pos     = read_dword( stream );
		_ec_package_add_public( obj, EcInternSymbol( ec_strdata(&str) ), pos, isconst );
		ec_string_cleanup( &str );
	}
	ASSERT( EC_PACKAGENEXPORT(obj) == l );

	S_OUT;
	return obj;
}


/* ------------------------------------------------------------------------
 * BYTECODE BACK-PATCHING
 * ------------------------------------------------------------------------ */

static void patch_bytecode( objectmap map, EC_OBJ obj, EcInt npkgs, EcInt *pkg_now_at )
{
	if (EC_NULLP(obj)) return;

	/* already visited ? */
	if (object2id( map, obj ))
		return;

	remember_object( map, obj );

	switch (EC_TYPE(obj))
	{
	case tc_none:
	case tc_undefined:
	case tc_error:
	case tc_bool:
	case tc_inum:
	case tc_fnum:
	case tc_symbol:
		break;

	case tc_primitive:
		break;

	case tc_cmethod:
		break;

	default:
		if (EC_PACKAGEP(obj))
		{
			patch_bytecode( map, EC_PACKAGENAME(obj),   npkgs, pkg_now_at );
			patch_bytecode( map, EC_PACKAGECODE(obj),   npkgs, pkg_now_at );
			patch_bytecode( map, EC_PACKAGEFRAME(obj),  npkgs, pkg_now_at );
			patch_bytecode( map, EC_PACKAGEIMPORT(obj), npkgs, pkg_now_at );
			patch_bytecode( map, EC_PACKAGESOURCE(obj), npkgs, pkg_now_at );
			break;
		}

		if (EC_COMPILEDP(obj))
		{
			EcInt i, l;
			EcBytecode bc;
			int  npar;
			EcInt pkg, pos;

			if (EC_COMPILEDCODE(obj))
			{
				ASSERT( EC_COMPILEDCODE(obj) );

				l = EC_COMPILEDNCODE(obj);
				for (i = 0; i < l; i++)
				{
					bc   = EC_COMPILEDCODE(obj)[i];
					npar = EcBytecodeParams( bc );
					switch (bc)
					{
					case PushPackageOP:
					case PopPackageOP:
						ASSERT( i + 2 < l );
						pkg = EC_COMPILEDCODE(obj)[i + 1];
						pos = EC_COMPILEDCODE(obj)[i + 2];

						ASSERT( pkg < npkgs );
#if defined(WITH_STDIO) && EC_DEBUG_PACKAGEIO
					    fprintf( stderr, "Patching %ld -> %ld (npkgs: %ld)\n", (long)pkg, (long)pkg_now_at[pkg], (long)npkgs );
#endif
						pkg = pkg_now_at[pkg];

						EC_COMPILEDCODE(obj)[i + 1] = pkg;
						break;

					default:
						break;
					}
					i += npar;
				}
			}
			patch_bytecode( map, EC_COMPILEDLEXICAL(obj), npkgs, pkg_now_at );
			patch_bytecode( map, EC_COMPILEDLFRAME(obj),  npkgs, pkg_now_at );
			patch_bytecode( map, EC_COMPILEDHANDLER(obj), npkgs, pkg_now_at );
			patch_bytecode( map, EC_COMPILEDPACKAGE(obj), npkgs, pkg_now_at );
			patch_bytecode( map, EC_COMPILEDNAME(obj),    npkgs, pkg_now_at );
			patch_bytecode( map, EC_COMPILEDINFO(obj),    npkgs, pkg_now_at );
			break;
		}

		if (EC_ARRAYP(obj))
		{
			EcInt i, l;

			l = EC_ARRAYLEN(obj);
			for (i = 0; i < l; i++)
/*				patch_bytecode( map, EC_ARRAYMEM(obj)[i], npkgs, pkg_now_at );*/
				patch_bytecode( map, EC_ARRAYGET(obj, i), npkgs, pkg_now_at );
			break;
		}

		if (EC_HASHP(obj))
		{
			ec_hash_iterator iter;
			EcAny key, value;

			iter = ec_hash_iterator_create( EC_HASH(obj) );
			ASSERT( iter );

			while (ec_hash_iterator_next( iter, &key ))
			{
				ec_hash_get( EC_HASH(obj), key, &value );
				patch_bytecode( map, (EC_OBJ) key,   npkgs, pkg_now_at );
				patch_bytecode( map, (EC_OBJ) value, npkgs, pkg_now_at );
			}

			ec_hash_iterator_destroy( iter );

			break;
		}

		if (EC_STACKP(obj))
		{
			EC_OBJ *o;

			/*
			 * TODO: ASSURE THAT ONLY LOADED CODE IS BACKPATCHED THROUGH STACK !!!
			 *
			 * Otherwise other code will get corrupted !!!
			 */
			for (o = EC_STACKBASE(obj); o < EC_STACKPOINTER(obj); o++)
				patch_bytecode( map, *o, npkgs, pkg_now_at );
			patch_bytecode( map, EC_STACKUP(obj),        npkgs, pkg_now_at );
			patch_bytecode( map, EC_STACKLEXICAL(obj),   npkgs, pkg_now_at );
			patch_bytecode( map, EC_STACKIMMUP(obj), npkgs, pkg_now_at );
			patch_bytecode( map, EC_STACKIMMCALLER(obj), npkgs, pkg_now_at );
			patch_bytecode( map, EC_STACKCALLED(obj), npkgs, pkg_now_at );
			break;
		}

		if (EC_HANDLERP(obj))
		{
			patch_bytecode( map, EC_HANDLERTYPE(obj), npkgs, pkg_now_at );
			patch_bytecode( map, EC_HANDLERCODE(obj), npkgs, pkg_now_at );
			break;
		}

		if (EC_CLASSP(obj))
		{
			EcInt i;

			patch_bytecode( map, EC_CLASSSUPER(obj),  npkgs, pkg_now_at );
			for (i = 0; i < EC_CLASSNMETHODS(obj); i++)
				patch_bytecode( map, EC_CLASSMTABLE(obj)[i].impl, npkgs, pkg_now_at );
			for (i = 0; i < EC_CLASSNCMETHODS(obj); i++)
				patch_bytecode( map, EC_CLASSCMTABLE(obj)[i].impl, npkgs, pkg_now_at );
			patch_bytecode( map, EC_CLASSPACKAGE(obj), npkgs, pkg_now_at );
			patch_bytecode( map, EC_CLASSNAME(obj), npkgs, pkg_now_at );
			patch_bytecode( map, EC_CLASSSHORTNAME(obj), npkgs, pkg_now_at );
			patch_bytecode( map, EC_CLASSCODE(obj), npkgs, pkg_now_at );
			break;
		}

		if (EC_OBJECTP(obj))
		{
			EcInt i;

			ASSERT( EC_NNULLP(EC_OBJECTCLASS(obj)) );
			patch_bytecode( map, EC_OBJECTCLASS(obj), npkgs, pkg_now_at );
			for (i = 0; i < EC_CLASSNIVARS(EC_OBJECTCLASS(obj)); i++)
				patch_bytecode( map, EC_OBJECTIVARS(obj)[i], npkgs, pkg_now_at );
			break;
		}


		/* XXXX TODO CHECKME XXXX */
/*		EcDump( obj );
		ASSERT( FALSE );*/
		break;
	}
}



/* ------------------------------------------------------------------------
 * LOW LEVEL OBJECT I/O
 * ------------------------------------------------------------------------ */

static void write_object( objectmap map, ec_stream *stream, EC_OBJ obj )
{
	EcUInt id;

	S_IN;

	ASSERT( map );

	EC_CHECK(obj);

	/* Object already in map ? */
	id = object2id( map, obj );
	if (id)
	{
		write_word( stream, tc_id );
		write_dword( stream, id );
		if (EC_NNULLP(obj))
			write_word( stream, EC_TYPE(obj) );
		else
			write_word( stream, tc_none );

		SHOW_WRITE(id, EC_NNULLP(obj) ? EC_TYPE(obj) : tc_none, obj, TRUE);
		S_OUT;
		return;
	}

	/* No, put it */
#if EC_INLINED_TYPES
	if (! EC_INLINED(obj))
		id = remember_object( map, obj );
#else
	id = remember_object( map, obj );
#endif

	SHOW_WRITE(id, EC_NNULLP(obj) ? EC_TYPE(obj) : tc_none, obj, FALSE);

/*	if (EC_NULLP(obj))
	{
		write_word( stream, tc_none );
		fprintf( stderr, "%ld OBJECT   TYPE: %2d  (%s)\n", id, tc_none, EcTypeName( tc_none ) );
		return;
	}*/

/*	fprintf( stderr, "%ld OBJECT   TYPE: %2d  (%s)\n", (long)id, EC_TYPE(obj), EcTypeName( EC_TYPE(obj) ) );*/

	write_word( stream, EC_TYPE(obj) );
	switch (EC_TYPE(obj))
	{
	case tc_none:
	case tc_error:
		ASSERT( FALSE );
		break;

	case tc_undefined:
		/* nothing to write */
		break;

	case tc_bool:
		write_byte( stream, EC_BOOLVAL(obj) );
		break;

	case tc_inum:
		ASSERT( sizeof(EC_INUM(obj)) <= sizeof(EcDWord) );
		write_dword( stream, (EcDWord) EC_INUM(obj) );
		break;

	case tc_fnum:
		write_float( stream, EC_FNUM(obj) );
		break;

	case tc_symbol:
	{
		EcInt i, l;

		const char *sym;
		sym = EcSymbolAt( EC_SYMBOL(obj) );
		l = strlen( sym );
		write_dword( stream, l );
		for (i = 0; i < l; i++)
			write_byte( stream, sym[i] );
	}
	break;

	case tc_primitive:
		/* XXXX: WRONG ! */
		/* XXXX: handle userdata */
/*		ASSERT( FALSE );*/
		/* write the C pointer index */
		write_dword( stream, ec_cpointer2index( (EcCPointer) EC_PRIMITIVE(obj) ) );
		break;

	case tc_cmethod:
		/* XXXX: WRONG ! */
/*		ASSERT( FALSE );*/
		/* write the C pointer index */
		write_dword( stream, ec_cpointer2index( (EcCPointer) EC_CMETHOD(obj) ) );
		break;

	default:
		if (EC_PACKAGEP(obj))
		{
			ASSERT( EC_STRINGP(EC_PACKAGENAME(obj)) );
			write_string( stream, EC_STRDATA(EC_PACKAGENAME(obj)) );
			break;
		}

		if (EC_COMPILEDP(obj))
		{
			EcInt i, j, l;
			EcBytecode bc;
			int npar;
			EcUInt symid;
			EcUInt nargs;

			l = EC_COMPILEDNCODE(obj);
			write_dword( stream, l );
			for (i = 0; i < l; i++)
			{
				bc   = EC_COMPILEDCODE(obj)[i];
				npar = EcBytecodeParams( bc );
				write_dword( stream, bc );
				if (bc == CallMethodOP)
				{
					symid = EC_COMPILEDCODE(obj)[i + 1];
					nargs = EC_COMPILEDCODE(obj)[i + 2];
					write_string( stream, EcSymbolAt( symid ) );
					write_dword( stream, nargs );
				} else if (bc == CallSuperMethodOP)
				{
					symid = EC_COMPILEDCODE(obj)[i + 1];
					nargs = EC_COMPILEDCODE(obj)[i + 2];
					write_string( stream, EcSymbolAt( symid ) );
					write_dword( stream, nargs );
				} else
				{
					for (j = 1; j <= npar; j++)
						write_dword( stream, EC_COMPILEDCODE(obj)[i + j] );
				}
				i += npar;
			}
/*		    for (i = 0; i < l; i++)
		        write_dword( stream, EC_COMPILEDCODE(obj)[i] );*/
			write_dword( stream, EC_COMPILEDNARG(obj) );
			write_dword( stream, EC_COMPILEDNARG_DEF(obj) );
			write_byte( stream, EC_COMPILEDVARG(obj) );
			write_dword( stream, EC_COMPILEDNLOC(obj) );
			write_dword( stream, EC_COMPILEDMAXTEMPS(obj) );
			write_object( map, stream, EC_COMPILEDLEXICAL(obj) );
			write_object( map, stream, EC_COMPILEDLFRAME(obj) );
			write_object( map, stream, EC_COMPILEDHANDLER(obj) );
			ASSERT( EC_PACKAGEP(EC_COMPILEDPACKAGE(obj)) );
			write_object( map, stream, EC_COMPILEDPACKAGE(obj) );
			write_object( map, stream, EC_COMPILEDNAME(obj) );
			write_byte( stream, (EcByte)EC_COMPILEDISMETHOD(obj) );
			write_object( map, stream, EC_COMPILEDINFO(obj) );
			break;
		}

		if (EC_ARRAYP(obj))
		{
			EcInt i, l;

			l = EC_ARRAYLEN(obj);
			write_dword( stream, l );
			for (i = 0; i < l; i++)
/*				write_object( map, stream, EC_ARRAYMEM(obj)[i] );*/
				write_object( map, stream, EC_ARRAYGET(obj, i) );
			break;
		}

		if (EC_HASHP(obj))
		{
			ec_hash_iterator iter;
			EcAny key, value;
			EcInt nentries;

			nentries = ec_hash_size( EC_HASH(obj) );
			write_dword( stream, (EcDWord) nentries );

			iter = ec_hash_iterator_create( EC_HASH(obj) );
			ASSERT( iter );

			while (ec_hash_iterator_next( iter, &key ))
			{
				ec_hash_get( EC_HASH(obj), key, &value );
				write_object( map, stream, (EC_OBJ) key );
				write_object( map, stream, (EC_OBJ) value );
			}

			ec_hash_iterator_destroy( iter );
			break;
		}

		if (EC_CHARP(obj))
		{
			ASSERT( sizeof(EcChar) <= sizeof(EcDWord) );
			if (sizeof(EcChar) <= sizeof(EcByte))
				write_byte( stream, EC_CHAR(obj) );
			else if (sizeof(EcChar) <= sizeof(EcWord))
				write_word( stream, EC_CHAR(obj) );
			else
				write_dword( stream, EC_CHAR(obj) );
			break;
		}

		if (EC_STRINGP(obj))
		{
			EcInt i, l;

			l = EC_STRLEN(obj);
			write_dword( stream, l );
			for (i = 0; i < l; i++)
				write_byte( stream, EC_STRDATA(obj)[i] );

			break;
		}

		if (EC_STACKP(obj))
		{
			EcUInt i;

			/* ASSERT( FALSE ); */
			write_dword( stream, EC_STACKDIM(obj) );
			for (i = 0; i < EC_STACKDIM(obj); i++)
				write_object( map, stream, EC_STACKBASE(obj)[i] );
			write_dword( stream, (EcDWord) (EC_STACKPOINTER(obj) - EC_STACKBASE(obj)) );
			write_dword( stream, (EcDWord) (EC_STACKBP(obj) - EC_STACKBASE(obj)) );
			write_object( map, stream, EC_STACKUP(obj) );
			write_object( map, stream, EC_STACKLEXICAL(obj) );
			write_dword( stream, (EcDWord) EC_STACKPC(obj) );
			write_dword( stream, (EcDWord) EC_STACKNARGS_REAL(obj) );
			write_dword( stream, (EcDWord) EC_STACKNARGS_LOGICAL(obj) );
			write_object( map, stream, EC_STACKIMMUP(obj) );
			write_object( map, stream, EC_STACKIMMCALLER(obj) );
			write_dword( stream, (EcDWord) EC_STACKIMMPC(obj) );
			write_object( map, stream, EC_STACKCALLED(obj) );
			break;
		}

		if (EC_HANDLERP(obj))
		{
			write_object( map, stream, EC_HANDLERTYPE(obj) );
			write_object( map, stream, EC_HANDLERCODE(obj) );
			break;
		}

		if (EC_CLASSP(obj))
		{
			EcInt i, l;
			EcInt specific;
			EC_OBJ impl;

			write_object( map, stream, EC_CLASSSUPER(obj) );
			write_object( map, stream, EC_CLASSPACKAGE(obj) );
			write_object( map, stream, EC_CLASSNAME(obj) );
			write_object( map, stream, EC_CLASSSHORTNAME(obj) );
			write_object( map, stream, EC_CLASSCODE(obj) );

			l = EC_CLASSNMETHODS(obj);
			write_dword( stream, l );
			for (i = 0; i < l; i++) {
				impl = EC_CLASSMTABLE(obj)[i].impl;
				ASSERT( EC_CMETHODP(impl) || EC_COMPILEDP(impl) );
				write_string( stream, EcSymbolAt( EC_CLASSMTABLE(obj)[i].symid ) );
				write_object( map, stream, impl );
			}
			l = EC_CLASSNCMETHODS(obj);
			write_dword( stream, l );
			for (i = 0; i < l; i++) {
				impl = EC_CLASSCMTABLE(obj)[i].impl;
				ASSERT( EC_CMETHODP(impl) || EC_COMPILEDP(impl) );
				write_string( stream, EcSymbolAt( EC_CLASSCMTABLE(obj)[i].symid ) );
				write_object( map, stream, impl );
			}
			write_dword( stream, EC_CLASSIOFFSET(obj) );
			l = EC_CLASSNIVARS(obj);
			write_dword( stream, l );									/* nivars */
			specific = EC_CLASSNIVARS(obj) - EC_CLASSIOFFSET(obj);
			for (i = 0; i < specific; i++) {						/* write only specific vars */
				write_string( stream, EcSymbolAt( EC_CLASSIVTABLE(obj)[i].symid ) );
				write_dword( stream, EC_CLASSIVTABLE(obj)[i].offset );
			}
			l = EC_CLASSNCVARS(obj);
			write_dword( stream, l );
			for (i = 0; i < l; i++) {
				write_string( stream, EcSymbolAt( EC_CLASSCVTABLE(obj)[i].symid ) );
				write_dword( stream, EC_CLASSCVTABLE(obj)[i].offset );
			}
			break;
		}

		if (EC_OBJECTP(obj))
		{
			EcInt i;

			ASSERT( EC_NNULLP(EC_OBJECTCLASS(obj)) );
			write_object( map, stream, EC_OBJECTCLASS(obj) );
			for (i = 0; i < EC_CLASSNIVARS(EC_OBJECTCLASS(obj)); i++)
				write_object( map, stream, EC_OBJECTIVARS(obj)[i] );
			/* TODO XXX: C user data */
#if EC_SIZEOF_ECPOINTERINTEGER <= 4
			write_dword( stream, (EcDWord) EC_OBJECTUSER(obj) );
#else
			/* TODO XXX: FIXME */
			fwrite( EC_OBJECTUSER(obj), sizeof(void *), 1, stream );
#endif
			break;
		}

		ASSERT( FALSE );
		break;
	}
	S_OUT;
}

static EC_OBJ read_object( objectmap map, ec_stream *stream, EcBool executeImported )
{
	EcWord type;
	EcUInt id;
	EC_OBJ obj = EC_NIL;

	S_IN;

	ASSERT( map );

	id = 0;
	type = read_word( stream );

	if (type == tc_none)
	{
		id = 0;
	}

	ASSERT( type != tc_none );

	if (type == tc_id)
	{
		/* Object already in map ? */
		id   = read_dword( stream );
		type = read_word( stream );
		obj  = id2object( map, id );
		ASSERT( obj != INVALID_OBJECT );

		SHOW_READ(id, type, obj, TRUE);

#ifdef EC_DEBUG
		if (EC_NNULLP(obj))
		{ ASSERT( EC_TYPEP(obj, type) ); }
		else
		{ ASSERT( type == tc_none ); }
#endif
		ASSERT( EC_NNULLP(obj) ? EC_TYPEP(obj, type) : (type == tc_none) );

/*
  		We could be reading an INCOMPLETE package ! So we can't check packages here !
  		EC_CHECK(obj);*/
		S_OUT;
		return obj;
	}

	if (type == tc_undefined)
	{
		ec_msg_printf("\n\n\n\n\n\n**** Uhm, reading a tc_undefined object !\n\n\n\n\n\n");
	}

	if (type == tc_array)
	{
		obj = EcMakeArray( 0 );
	} else if (type == tc_package)
		obj = EC_NIL;
#if EC_INLINED_TYPES
	else if ((type == tc_inum) ||
			 (type == tc_symbol))
		obj = EC_NIL;
#endif
	else
	{
		obj = EcAllocObject();
		if (EC_ERRORP(obj))
		{
			S_OUT;
			return obj;
		}
	}

#if EC_INLINED_TYPES
	ASSERT( EC_NNULLP(obj) || (type == tc_package) || (type == tc_inum) || (type == tc_symbol) );
#else
	ASSERT( EC_NNULLP(obj) || (type == tc_package) );
#endif
	if (EC_NNULLP(obj))
	{
#if EC_INLINED_TYPES
		ASSERT( (type != tc_inum) && (type != tc_symbol) );
#endif
		EC_OBJTYPE(obj) = type;
		id = remember_object( map, obj );

		SHOW_READ(id, type, obj, FALSE);
	}

	switch (type)
	{
	case tc_none:
	case tc_error:
		ASSERT( FALSE );
		break;

	case tc_undefined:
		/* nothing to read */
		break;

	case tc_bool:
		EC_BOOLVAL(obj) = read_byte( stream );
		break;

	case tc_inum:
#if EC_INLINED_TYPES
		obj = EC_MAKE_INT( (EcInt) read_dword( stream ) );
#else
		ASSERT( sizeof(EC_INUM(obj)) <= sizeof(EcDWord) );
		EC_INUM(obj) = (EcInt) read_dword( stream );
#endif
		break;

	case tc_fnum:
		EC_FNUM(obj) = read_float( stream );
		break;

	case tc_symbol:
	{
		EcInt i, l;
		char *buf;

		l = read_dword( stream );
		buf = alloca( l + 1 );
		for (i = 0; i < l; i++)
			buf[i] = read_byte( stream );
		buf[i] = '\0';
#if EC_INLINED_TYPES
		obj = EcMakeSymbol( buf );
#else
		EC_SYMBOL(obj) = EcInternSymbol( buf );
#endif
	}
	break;

	case tc_primitive:
		/* XXXX: WRONG ! */
		/* XXXX: handle userdata */
/*		ASSERT( FALSE );*/
		EC_PRIMITIVE(obj) = (EcCPrimitive) ec_index2cpointer( read_dword( stream ) );
	break;

	case tc_cmethod:
		/* XXXX: WRONG ! */
/*		ASSERT( FALSE );*/
		EC_CMETHOD(obj) = (EcCMethod) ec_index2cpointer( read_dword( stream ) );
	break;

	default:
		if (type == tc_package)
		{
			ec_string pkgname;

			ec_string_init( &pkgname, NULL );
			read_string( stream, &pkgname );
			obj = EcPackageLoadByName( ec_strdata(&pkgname), executeImported, executeImported );
			ASSERT( EC_PACKAGEP(obj) );
			ASSERT( EC_STRINGP(EC_PACKAGENAME(obj)) );
			ASSERT( strcmp( EC_STRDATA(EC_PACKAGENAME(obj)), ec_strdata(&pkgname) ) == 0 );
			ec_string_cleanup( &pkgname );

			id = remember_object( map, obj );

			SHOW_READ(id, type, obj, FALSE);
			break;
		}

		if (type == tc_compiled)
		{
			EC_OBJ      lexical, lframe, handler, package, name, info;
			EcInt       i, j, l;
			EcBytecode  bc;
			int         npar;
			EcUInt      nargs;
			ec_string   str;
			EcBool      ismethod;

			EC_COMPILED(obj) = ec_malloc( sizeof(EcCompiled) );
			ASSERT( EC_COMPILED(obj) );

			EC_COMPILEDCODE(obj)  = NULL;
			EC_COMPILEDNCODE(obj) = 0;

			l = read_dword( stream );
			for (i = 0; i < l; i++)
			{
				bc   = read_dword( stream );
				npar = EcBytecodeParams( bc );
				EcCompiledPush( obj, bc );
				if (bc == CallMethodOP)
				{
					ec_string_init( &str, NULL );
					read_string( stream, &str );
					nargs = read_dword( stream );
					EcCompiledPush( obj, EcInternSymbol( ec_strdata(&str) ) );
					EcCompiledPush( obj, nargs );
					ec_string_cleanup( &str );
				} else if (bc == CallSuperMethodOP)
				{
					ec_string_init( &str, NULL );
					read_string( stream, &str );
					nargs = read_dword( stream );
					EcCompiledPush( obj, EcInternSymbol( ec_strdata(&str) ) );
					EcCompiledPush( obj, nargs );
					ec_string_cleanup( &str );
				} else
				{
					for (j = 1; j <= npar; j++)
						EcCompiledPush( obj, read_dword( stream ) );
				}
				i += npar;
			}
			ASSERT( EC_COMPILEDNCODE(obj) == l );
			EC_COMPILEDNCODE(obj) = l;
			EC_COMPILEDNARG(obj)     = read_dword( stream );
			EC_COMPILEDNARG_DEF(obj) = read_dword( stream );
			EC_COMPILEDVARG(obj)     = read_byte( stream );
			EC_COMPILEDNLOC(obj)     = read_dword( stream );
			EC_COMPILEDMAXTEMPS(obj) = read_dword( stream );

			lexical  = read_object( map, stream, executeImported );
			lframe   = read_object( map, stream, executeImported );
			handler  = read_object( map, stream, executeImported );
			package  = read_object( map, stream, executeImported );
			ASSERT( EC_PACKAGEP(package) );
			name     = read_object( map, stream, executeImported );
			ismethod = read_byte( stream );
			info     = read_object( map, stream, executeImported );

			EC_COMPILEDLEXICAL(obj)  = lexical;
#if EC_STACK_RECYCLE
			if (EC_STACKP(EC_COMPILEDLEXICAL(obj)))
				EC_STACKREF_INC(EC_COMPILEDLEXICAL(obj));
#endif
			EC_COMPILEDLFRAME(obj)   = lframe;
			EC_COMPILEDHANDLER(obj)  = handler;
			EC_COMPILEDPACKAGE(obj)  = package;
			EC_COMPILEDNAME(obj)     = name;
			EC_COMPILEDISMETHOD(obj) = ismethod;
			EC_COMPILEDINFO(obj)     = info;
			EC_COMPILEDCCALLABLE(obj) = NULL;
			break;
		}

		if (type == tc_array)
		{
			EcInt i, l;
			EC_OBJ el;

			l = read_dword( stream );
			for (i = 0; i < l; i++)
			{
				el = read_object( map, stream, executeImported );
				EcArraySet( obj, i, el );
			}
			ASSERT( EC_ARRAYLEN(obj) == l );
			break;
		}

		if (type == tc_hash)
		{
			ec_hash hash;
			EcInt nentries, i;
			EC_OBJ key, value;
			EcBool rv;

			hash = ec_hash_create( 0, 0, 0, &ec_hash_obj2obj );
			ASSERT( hash );

			EC_USER(obj) = (void *) hash;

			nentries = (EcInt) read_dword( stream );
			for (i = 0; i < nentries; i++)
			{
				key   = read_object( map, stream, executeImported );
				value = read_object( map, stream, executeImported );
				rv = ec_hash_set( EC_HASH(obj), (EcAny)key, (EcAny)value );
				ASSERT( rv );
			}

			break;
		}

		if (type == tc_char)
		{
			ASSERT( sizeof(EcChar) <= sizeof(EcDWord) );
			if (sizeof(EcChar) <= sizeof(EcByte))
				EC_CHAR(obj) = read_byte( stream );
			else if (sizeof(EcChar) <= sizeof(EcWord))
				EC_CHAR(obj) = read_word( stream );
			else
				EC_CHAR(obj) = read_dword( stream );
			break;
		}

		if (type == tc_string)
		{
			EcInt i, l;
			char *buf;

			l = read_dword( stream );
			buf = alloca( l + 1 );
			for (i = 0; i < l; i++)
				buf[i] = read_byte( stream );
			buf[i] = '\0';
			EC_STRDATA(obj) = ec_malloc( l + 1 );
			ASSERT( EC_STRDATA(obj) );
			EC_STRLEN(obj) = l;
			strcpy( EC_STRDATA(obj), buf ? buf : "" );

			break;
		}

		if (type == tc_stack)
		{
			EC_OBJ stackel, stackup, stacklexical, stackimmup, stackimmcaller, stackcalled;
			EcUInt dim, i;

			EC_STACK(obj) = ec_malloc( sizeof(EcStack) );
			ASSERT( EC_STACK(obj) );

			dim = read_dword( stream );
			EC_STACKDIM(obj) = dim;
#if EC_STACK_USES_STATIC
			if (dim <= EC_STACK_STATIC_SIZE)
				EC_STACKBASE(obj)  = EC_STACKSTATIC(obj);
			else
				EC_STACKBASE(obj)  = ec_malloc( dim * sizeof(EC_OBJ) );
#else
			EC_STACKBASE(obj) = ec_malloc( dim * sizeof(EC_OBJ) );
#endif
			ASSERT( EC_STACKBASE(obj) );
			for (i = 0; i < dim; i++)
			{
				stackel = read_object( map, stream, executeImported );
				EC_STACKBASE(obj)[i] = stackel;
			}
			EC_STACKPOINTER(obj) = EC_STACKBASE(obj) + read_dword( stream );
			EC_STACKBP(obj)      = EC_STACKBASE(obj) + read_dword( stream );
			stackup      = read_object( map, stream, executeImported );
			stacklexical = read_object( map, stream, executeImported );
			EC_STACKUP(obj)            = stackup;
			EC_STACKLEXICAL(obj)       = stacklexical;
#if EC_STACK_RECYCLE
			if (EC_STACKP(EC_STACKLEXICAL(obj)))
				EC_STACKREF_INC(EC_STACKLEXICAL(obj));
#endif
			EC_STACKPC(obj)            = read_dword( stream );
			EC_STACKNARGS_REAL(obj)    = read_dword( stream );
			EC_STACKNARGS_LOGICAL(obj) = read_dword( stream );
			stackimmup     = read_object( map, stream, executeImported );
			stackimmcaller = read_object( map, stream, executeImported );
			EC_STACKIMMUP(obj)         = stackimmup;
			EC_STACKIMMCALLER(obj)     = stackimmcaller;
			EC_STACKIMMPC(obj)         = read_dword( stream );
			stackcalled = read_object( map, stream, executeImported );
			EC_STACKCALLED(obj) = stackcalled;
#if EC_STACK_RECYCLE
			/* EC_STACKREF_SET(obj, 0); */
			EC_STACKREF_SET(obj, 1);
#if EC_STACK_RECYCLE_STATS
			PRIVATE(n_makestack)++;
#endif
#endif
			/* ASSERT( FALSE ); */
			break;
		}

		if (type == tc_handler)
		{
			EC_OBJ type, code;

			type = read_object( map, stream, executeImported );
			code = read_object( map, stream, executeImported );
			EC_HANDLERTYPE(obj) = type;
			EC_HANDLERCODE(obj) = code;
			break;
		}

		if (type == tc_class)
		{
			EC_OBJ super, package, name, shortname, code, impl;
			EcInt i, l, pos;
			EcInt specific;
			ec_string str;

			super     = read_object( map, stream, executeImported );
			package   = read_object( map, stream, executeImported );
			name      = read_object( map, stream, executeImported );
			shortname = read_object( map, stream, executeImported );
			code      = read_object( map, stream, executeImported );
			ASSERT( EC_NULLP(super) || EC_CLASSP(super) );
			ASSERT( EC_NULLP(code) || EC_COMPILEDP(code) );

			EC_CLASS(obj) = ec_malloc( sizeof(EcClass) );
			ASSERT( EC_CLASS(obj) );
			EC_CLASSSUPER(obj)     = super;
			EC_CLASSNMETHODS(obj)  = 0;
			EC_CLASSMTABLE(obj)    = NULL;
			EC_CLASSNCMETHODS(obj) = 0;
			EC_CLASSCMTABLE(obj)   = NULL;
			EC_CLASSIOFFSET(obj)   = EC_NNULLP(super) ? EC_CLASSNIVARS(super) : 0;
			EC_CLASSNIVARS(obj)    = EC_NNULLP(super) ? EC_CLASSNIVARS(super) : 0;
			EC_CLASSIVTABLE(obj)   = NULL;
			EC_CLASSNCVARS(obj)    = 0;
			EC_CLASSCVTABLE(obj)   = NULL;
			EC_CLASSPACKAGE(obj)   = package;
			EC_CLASSNAME(obj)      = name;
			EC_CLASSSHORTNAME(obj) = shortname;
			EC_CLASSCODE(obj)      = code;

			/* Set appropriately the various fields */
			if (EC_NNULLP(EC_CLASSSUPER(obj)))
			{
				EC_CLASSIOFFSET(obj) = EC_CLASSNIVARS(EC_CLASSSUPER(obj));
				EC_CLASSNIVARS(obj)  = EC_CLASSNIVARS(EC_CLASSSUPER(obj));
			}
			else
			{
				EC_CLASSIOFFSET(obj) = 0;
				EC_CLASSNIVARS(obj)  = 0;
			}

			l = read_dword( stream );
			EC_CLASSNMETHODS(obj) = 0;								/* set by EcAddMethod */
			for (i = 0; i < l; i++) {
				ec_string_init( &str, NULL );
				read_string( stream, &str );
				impl = read_object( map, stream, executeImported );
				ASSERT( EC_CMETHODP(impl) || EC_COMPILEDP(impl) );
				EcAddMethod( obj, ec_strdata(&str), impl );
				ec_string_cleanup( &str );
			}
			ASSERT( EC_CLASSNMETHODS(obj) == l );
			l = read_dword( stream );
			EC_CLASSNCMETHODS(obj) = 0;								/* set by EcAddClassMethod */
			for (i = 0; i < l; i++) {
				ec_string_init( &str, NULL );
				read_string( stream, &str );
				impl = read_object( map, stream, executeImported );
				ASSERT( EC_CMETHODP(impl) || EC_COMPILEDP(impl) );
				EcAddClassMethod( obj, ec_strdata(&str), impl );
				ec_string_cleanup( &str );
			}
			ASSERT( EC_CLASSNCMETHODS(obj) == l );
			EC_CLASSIOFFSET(obj) = read_dword( stream );
			l = read_dword( stream );									/* nivars */
			EC_CLASSNIVARS(obj) = l;
			specific = EC_CLASSNIVARS(obj) - EC_CLASSIOFFSET(obj);	/* specific instance variables */
			if (specific > 0)
			{
				EC_CLASSIVTABLE(obj) = ec_malloc( specific * sizeof(EcVariableEntry) );
				ASSERT( EC_CLASSIVTABLE(obj) );
				for (i = 0; i < specific; i++) {					/* read only specific vars */
					ec_string_init( &str, NULL );
					read_string( stream, &str );
					pos  = read_dword( stream );
					EC_CLASSIVTABLE(obj)[i].symid  = EcInternSymbol( ec_strdata(&str) );
					EC_CLASSIVTABLE(obj)[i].offset = pos;
					ec_string_cleanup( &str );
				}
			} else
			{
				EC_CLASSIVTABLE(obj) = NULL;
			}
			ASSERT( EC_CLASSNIVARS(obj) == l );
			l = read_dword( stream );
			if (l > 0)
			{
				EC_CLASSCVTABLE(obj) = ec_malloc( l * sizeof(EcVariableEntry) );
				ASSERT( EC_CLASSCVTABLE(obj) );
				EC_CLASSNCVARS(obj) = l;
				for (i = 0; i < l; i++) {
					ec_string_init( &str, NULL );
					read_string( stream, &str );
					pos  = read_dword( stream );
					EC_CLASSCVTABLE(obj)[i].symid  = EcInternSymbol( ec_strdata(&str) );
					EC_CLASSCVTABLE(obj)[i].offset = pos;
					ec_string_cleanup( &str );
				}
			}
			ASSERT( EC_CLASSNCVARS(obj) == l );

			/* call class code */
			EcCall( EC_NIL, EC_NIL, EC_NIL, EC_CLASSCODE(obj), 0 );
			break;
		}

		if (type == tc_object)
		{
			EC_OBJ objclass;
			EcInt i;
			EcInt nvars;

			objclass = read_object( map, stream, executeImported );
			EC_OBJECT(obj) = ec_malloc( sizeof(EcObject) );
			ASSERT( EC_OBJECT(obj) );
			EC_OBJECTCLASS(obj) = objclass;
			nvars = EC_CLASSNIVARS(objclass);
			if (nvars)
				EC_OBJECTIVARS(obj) = ec_malloc( nvars * sizeof(EC_OBJ) );
			else
				EC_OBJECTIVARS(obj) = NULL;
			EC_OBJECTUSER(obj) = NULL;

			for (i = 0; i < EC_CLASSNIVARS(EC_OBJECTCLASS(obj)); i++)
				EC_OBJECTIVARS(obj)[i] = read_object( map, stream, executeImported );
			/* TODO XXX: C user data */
#if EC_SIZEOF_ECPOINTERINTEGER <= 4
			EC_OBJECTUSER(obj) = (void *) read_dword( stream );
#else
			/* TODO XXX: FIXME */
			fread( EC_OBJECTUSER(obj), sizeof(void *), 1, stream );
#endif
			break;
		}

		ASSERT( FALSE );
		obj = EC_NIL;
		break;
	}

	SHOW_READ_LATER(id, obj);
	EC_CHECK(obj);
	S_OUT;
	return obj;
}


/* ------------------------------------------------------------------------
 * LOW LEVEL FILE I/O
 * ------------------------------------------------------------------------ */

static void write_byte( ec_stream *stream, EcByte datum )
{
	ec_stream_writen( stream, &datum, sizeof(EcByte) );
}

static void write_word( ec_stream *stream, EcWord datum )
{
	EcWord w;

	w = htons( datum );
	ec_stream_writen( stream, &w, sizeof(EcWord) );
}

static void write_dword( ec_stream *stream, EcDWord datum )
{
	EcDWord dw;

	dw = htonl( datum );
	ec_stream_writen( stream, &dw, sizeof(EcDWord) );
}

static void write_float( ec_stream *stream, EcFloat datum )
{
	char buf[128];
	EcInt l;

	sprintf( buf, "%.16g", (double)datum );						/* :TODO: use ec_sprintf */
	l = strlen( buf );
	write_byte( stream, l );
	/*
	for (i = 0; i < l; i++)
	write_byte( stream, buf[i] ); */
	ec_stream_writen( stream, buf, l );
}

static void write_string( ec_stream *stream, const char *string )
{
	EcInt l;

	l = strlen( string );
	write_dword( stream, l );
	/* 
	for (i = 0; i < l; i++)
	   write_byte( stream, string[i] );
	*/
	ec_stream_writen( stream, string, l );
}

static EcByte read_byte( ec_stream *stream )
{
	EcByte b;

	ec_stream_readn( stream, &b, sizeof(EcByte) );
	return b;
}

static EcWord read_word( ec_stream *stream )
{
	EcWord w;

	ec_stream_readn( stream, &w, sizeof(EcWord) );
	return ntohs( w );
}

static EcDWord read_dword( ec_stream *stream )
{
	EcDWord dw;

	ec_stream_readn( stream, &dw, sizeof(EcDWord) );
	return ntohl( dw );
}

static EcFloat read_float( ec_stream *stream )
{
	char buf[128];
	EcInt l;
	double datum;

	l = read_byte( stream );
	/* for (i = 0; i < l; i++)
	   buf[i] = read_byte( stream );
	   buf[i] = '\0'; */
	ec_stream_readn( stream, buf, l );
	buf[l] = '\0';

	sscanf( buf, "%lg", &datum );								/* :TODO: use our _ec_strtod() */
	return (EcFloat)datum;
}

static char *read_string( ec_stream *stream, ec_string *dst )
{
	EcInt l;

	l = read_dword( stream );
	if (! ec_string_prepare( dst, l+1 )) return NULL;
	/* for (i = 0; i < l; i++)
	   buf[i] = read_byte( fh );
	   buf[i] = '\0'; */
	ASSERT( dst->avail >= l+1 );
	ec_stream_readn( stream, ec_strdata(dst), l );
	ec_strdata(dst)[l] = '\0';
	dst->length = l;
	ASSERT( dst->avail >= l+1 );
	ASSERT( ec_strlen(dst) == l );

	return ec_strdata(dst);
}


/* ------------------------------------------------------------------------
 * OBJECT MAP
 * ------------------------------------------------------------------------ */

static objectmap objectmap_create( void )
{
	objectmap res;

	res = (objectmap) ec_malloc( sizeof(struct objectmap_struct) );
	if (! res) return NULL;

	/*
	 * NOTE: an ec_hash_int2int is good enough to map integers to/from pointers (EC_OBJs) !
	 *       (Since it uses EcAny)
	 */

	res->nextid = 0;
	res->obj2id = ec_hash_create( 256, 0, 0, &ec_hash_int2int );
	if (! res->obj2id) {
		ec_free( res );
		return NULL;
	}
	res->id2obj = ec_hash_create( 256, 0, 0, &ec_hash_int2int );
	if (! res->id2obj) {
		ec_hash_destroy( res->obj2id );
		ec_free( res );
		return NULL;
	}

	return res;
}

static void objectmap_destroy( objectmap map )
{
	if (map->id2obj) ec_hash_destroy( map->id2obj );
	map->id2obj = NULL;
	if (map->obj2id) ec_hash_destroy( map->obj2id );
	map->obj2id = NULL;
	map->nextid = 0;
	ec_free( map );
}

static EcUInt remember_object( objectmap map, EC_OBJ obj )
{
	EcUInt id;

	/* already in map ? */
	if (ec_hash_get( map->obj2id, (EcAny)obj, (EcAny *)&id ))
		return id;

	if (! ec_hash_set( map->obj2id, (EcAny)obj, (EcAny)(EcPointerInteger)(map->nextid + 1) ))
		return 0;
	ec_hash_set( map->id2obj, (EcAny)(EcPointerInteger)(map->nextid + 1), (EcAny)obj );
	return ++(map->nextid);
}

static EcUInt object2id( objectmap map, EC_OBJ obj )
{
	EcUInt id;

	if (! ec_hash_get( map->obj2id, (EcAny)obj, (EcAny *)&id ))
		return 0;
	return id;
}

static EC_OBJ id2object( objectmap map, EcUInt id )
{
	EC_OBJ obj;

	if (! ec_hash_get( map->id2obj, (EcAny)(EcPointerInteger)id, (EcAny *)&obj ))
	{
#if defined(WITH_STDIO) && SHOW_MAP
		fprintf( stderr, "INVALID_OBJECT at id = %ld\n", (long)id );
#endif
		return INVALID_OBJECT;
	}
	return obj;
}

EcBool _ec_packageio_init( void )
{
	EcInt i;

	PRIVATE(patchmap) = objectmap_create();
	if (! PRIVATE(patchmap)) return FALSE;

	/* Put global objects in map */
	remember_object( PRIVATE(patchmap), EC_NIL );
	for (i = 0; i < PRIVATE(ncoreglobals); i++)
		remember_object( PRIVATE(patchmap), PRIVATE(global[i]) );

	return TRUE;
}

void _ec_packageio_cleanup( void )
{
	objectmap_destroy( PRIVATE(patchmap) );
	PRIVATE(patchmap) = NULL;
}
