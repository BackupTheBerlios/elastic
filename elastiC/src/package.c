/* ==========================================================================
 * package.c
 * ==========================================================================
 * Package handling routines.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat Dec  5 15:08:45 MET 1998
 *
 *   $Id: package.c,v 1.6 2005/03/08 13:40:05 panta Exp $
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

#include "basic.h"
#include "debug.h"

#include "elastic.h"
#include "private.h"
#include "compat.h"

static EcInt _ec_package_get_public( EC_OBJ package, EcUInt sym );

/* ========================================================================
 * A P I
 * ======================================================================== */

EC_API char *EcPackagePathFromName( ec_string *path,
									const ec_string *packagename, const char *suffix )
{
	EcInt i;
	char  ch;

	ASSERT( path );
	ASSERT( packagename );

	/* clear the destination */
	ec_string_cleanup( path );

	for (i = 0; i < ec_strlen( packagename ); i++)
	{
		ch = ec_strdata( packagename )[i];

		if (ch == '.')
			ec_strcat( path, EC_PATHSEPARATOR, 0 );
		else
			ec_strcatc( path, ch );
	}

	if (suffix)
		ec_strcat( path, suffix, 0 );

	return ec_strdata( path );
}

EC_API EC_OBJ EcPackageIntroduce( const char *name )
{
	struct package *newPackages = NULL;
	EcInt n;
	EC_OBJ package;

	/* Create the package */
	package = EcMakePackage( name );

	/* Save package object in package list */
	n = PRIVATE(npackages) + 1;
	newPackages = ec_realloc( PRIVATE(package), n * sizeof(struct package) );
	if (! newPackages)
	{
		/* TODO: Should raise an exception */
		ASSERT( FALSE );

		return Ec_ERROR;
	}
	PRIVATE(package)   = newPackages;
	PRIVATE(npackages) = n;
	PRIVATE(package)[n-1].name     = ec_string_create( name );
	PRIVATE(package)[n-1].obj      = package;
	PRIVATE(package)[n-1].dlhandle = NULL;

	ASSERT( EC_ARRAYP(EC_PACKAGEIMPORT(package)) );
	return package;
}

EC_API EcInt EcPackageVariable( EC_OBJ package, const char *symbol, EC_OBJ value, EcBool pub, EcBool isconst )
{
	ec_string fullsymbol, symbolsuffix;
	ec_string packagename;
	EcInt  pos;
	EcUInt symid;
	EcBool qualified;

	ec_string_init( &fullsymbol, symbol );
	ec_string_init( &symbolsuffix, NULL );
	ec_string_init( &packagename, NULL );

	qualified = EcNamePrefix( &packagename,  &fullsymbol ) ? TRUE : FALSE;
	EcNameSuffix( &symbolsuffix, &fullsymbol );

/*	ec_stderr_printf( "EcPackageVariable   symbol: '%s'   package: '%s'   qualified: %s\n",
			 symbol, EC_NNULLP(package) ? EC_STRDATA(EC_PACKAGENAME(package)) : "--", qualified ? "YES" : "NO" );
	ec_stderr_printf( "       suffix: '%s'\n", ec_strdata( &symbolsuffix ) );
	ec_stderr_printf( "  packagename: '%s'\n", ec_strdata( &packagename ) );*/

	if (! qualified)
	{
		/* Symbol is unqualified. We need package */
		ASSERT( EC_NNULLP(package) );
	} else
	{
		/* Symbol is qualified. Package must be NIL */
		ASSERT( EC_NULLP(package) );

		package = _ec_package_loaded( ec_strdata( &packagename ) );
		if (EC_NULLP(package))
			goto onError;
	}

	/* Now we have in package the right package :-) */

	if (qualified)
		symid = EcInternSymbol( ec_strdata( &symbolsuffix ) );
	else
		symid = EcInternSymbol( symbol );

	/* Save value in object frame */
	pos = EcArrayPush( EC_PACKAGEFRAME(package), value );

	if (pub)
	{
		/* Add to public list */

		if (! _ec_package_add_public( package, symid, pos, isconst ))
			goto onError;
	}

	ec_string_cleanup( &fullsymbol );
	ec_string_cleanup( &symbolsuffix );
	ec_string_cleanup( &packagename );
	return pos;

onError:
	ec_string_cleanup( &fullsymbol );
	ec_string_cleanup( &symbolsuffix );
	ec_string_cleanup( &packagename );
	return -1;
}

EC_API EcBool EcPackageImport( EC_OBJ inPackage, const char *name, EC_OBJ array_of_syms, EcBool execute )
{
	EC_OBJ import_pair;
	EC_OBJ pkg;

	/*
	 * array_of_syms can be:
	 *
	 *  1. An array of symbols to import from package in package scope. Only those symbols
	 *     will be imported.
	 *  2. EC_NIL. The package will be imported, but no symbols will be imported in package
	 *     scope.
	 *  3. @true. The package will be imported, and all symbols will be imported in package
	 *     scope.
	 *
	 *  P.S. The meaning is not assigned here, but in the compilation phase, by symbol
	 *       resolutors.
	 */

	ASSERT( EC_PACKAGEP(inPackage) );
	ASSERT( EC_ARRAYP(EC_PACKAGEIMPORT(inPackage)) );
	ASSERT( name );
	ASSERT( EC_NULLP(array_of_syms) || EC_ARRAYP(array_of_syms) || (EC_BOOLP(array_of_syms) && EC_BOOLVAL(array_of_syms)) );

	/* Load the requested package */
	pkg = EcPackageLoadByName( name, execute, execute );
	if (EC_NULLP(pkg)) return FALSE;

	ASSERT( EC_PACKAGEP(inPackage) );
	ASSERT( EC_ARRAYP(EC_PACKAGEIMPORT(inPackage)) );

	import_pair = EcMakeArray( 2 );
	EcArraySet( import_pair, 0, pkg );
	EcArraySet( import_pair, 1, array_of_syms );

	ASSERT( EC_PACKAGEP(inPackage) );
	ASSERT( EC_ARRAYP(EC_PACKAGEIMPORT(inPackage)) );

	EcArrayPush( EC_PACKAGEIMPORT(inPackage), import_pair );
	return TRUE;
}

EC_API const char *EcPackageShortName( ec_string *dst, EC_OBJ package )
{
	const char *pkgname;
	ec_string   packagename;

	ASSERT( EC_STRINGP(EC_PACKAGENAME(package)) );
	pkgname = EC_STRDATA(EC_PACKAGENAME(package));

	ec_string_init( &packagename, pkgname );
	ec_string_cleanup( dst );

	EcNameSuffix( dst, &packagename );
	return ec_strdata( dst );
}

EC_API EC_OBJ EcPackageFromName( const char *name )
{
	EC_OBJ packageObj;

	packageObj = _ec_package_loaded( name );
	return packageObj;
}

EC_API EC_OBJ EcGetVariable( const char *packagename, const char *symbol )
{
	ec_string fullsymbol;
	EcInt     pos;
	EcUInt    symid;
	EC_OBJ    package;

	ec_string_init( &fullsymbol, packagename );
	ec_strcat( &fullsymbol, ".", 0 );
	ec_strcat( &fullsymbol, symbol, 0 );

	package = EcPackageFromName( packagename );
	if (EC_NULLP(package))
		goto onError;											/* Package not present ! */

	symid = EcInternSymbol( symbol );

	pos = _ec_package_get_public( package, symid );
	if (pos < 0)
		goto onError;											/* Symbol not found !    */

	ec_string_cleanup( &fullsymbol );
	return EcArrayGet( EC_PACKAGEFRAME(package), pos );

 onError:
	ec_string_cleanup( &fullsymbol );
	return Ec_ERROR;
}

EC_API EC_OBJ EcSetVariable( const char *packagename, const char *symbol, EC_OBJ value )
{
	ec_string fullsymbol;
	EcInt     pos;
	EcUInt    symid;
	EC_OBJ    package;

	ec_string_init( &fullsymbol, packagename );
	ec_strcat( &fullsymbol, ".", 0 );
	ec_strcat( &fullsymbol, symbol, 0 );

	package = EcPackageFromName( packagename );
	if (EC_NULLP(package))
		goto onError;											/* Package not present ! */

	symid = EcInternSymbol( symbol );

	pos = _ec_package_get_public( package, symid );
	if (pos < 0)
		goto onError;											/* Symbol not found !    */

	ec_string_cleanup( &fullsymbol );
	EcArraySet( EC_PACKAGEFRAME(package), pos, value );
	return value;

 onError:
	ec_string_cleanup( &fullsymbol );
	return Ec_ERROR;
}

EC_API EC_OBJ EcGetQVariable( const char *qualifiedsymbol )
{
	ec_string fullsymbol, symbolsuffix;
	ec_string packagename;
	EcBool qualified;
	EC_OBJ res;

	ec_string_init( &fullsymbol, qualifiedsymbol );
	ec_string_init( &symbolsuffix, NULL );
	ec_string_init( &packagename, NULL );

	qualified = EcNamePrefix( &packagename,  &fullsymbol ) ? TRUE : FALSE;
	EcNameSuffix( &symbolsuffix, &fullsymbol );

	if (! qualified)
		goto onError;

	res = EcGetVariable( ec_strdata( &packagename ), ec_strdata( &symbolsuffix ) );

	ec_string_cleanup( &fullsymbol );
	ec_string_cleanup( &symbolsuffix );
	ec_string_cleanup( &packagename );
	return res;

onError:
	ec_string_cleanup( &fullsymbol );
	ec_string_cleanup( &symbolsuffix );
	ec_string_cleanup( &packagename );
	return EC_NIL;
}

EC_API EC_OBJ EcSetQVariable( const char *qualifiedsymbol, EC_OBJ value )
{
	ec_string fullsymbol, symbolsuffix;
	ec_string packagename;
	EcBool qualified;
	EC_OBJ res;

	ec_string_init( &fullsymbol, qualifiedsymbol );
	ec_string_init( &symbolsuffix, NULL );
	ec_string_init( &packagename, NULL );

	qualified = EcNamePrefix( &packagename,  &fullsymbol ) ? TRUE : FALSE;
	EcNameSuffix( &symbolsuffix, &fullsymbol );

	if (! qualified)
		goto onError;

	res = EcSetVariable( ec_strdata( &packagename ), ec_strdata( &symbolsuffix ), value );

	ec_string_cleanup( &fullsymbol );
	ec_string_cleanup( &symbolsuffix );
	ec_string_cleanup( &packagename );
	return res;

onError:
	ec_string_cleanup( &fullsymbol );
	ec_string_cleanup( &symbolsuffix );
	ec_string_cleanup( &packagename );
	return EC_NIL;
}


/* ========================================================================
 * P R I V A T E
 * ======================================================================== */

/* ------------------------------------------------------------------------
 * LIBRARY INTERNAL PACKAGE FUNCTIONS
 * ------------------------------------------------------------------------ */

EC_OBJ _ec_package_loaded( const char *name )
{
	EcInt i;

	i = _ec_package_position( name );
	if (i >= 0)
		return PRIVATE(package)[i].obj;

	return EC_NIL;
}

EcInt _ec_package_position( const char *name )
{
	EcInt i;

	for (i = 0; i < PRIVATE(npackages); i++)
	{
		if (strcmp( ec_strdata( PRIVATE(package)[i].name ), name ) == 0)
			return i;
	}
	return -1;
}

EcInt _ec_package_position_o( EC_OBJ package )
{
	EcInt i;

	for (i = 0; i < PRIVATE(npackages); i++)
	{
		if (PRIVATE(package)[i].obj == package)
			return i;
	}

	return -1;
}


#define EXPORTQUANTUM 8

EcBool _ec_package_add_public( EC_OBJ package, EcUInt sym, EcInt pos, EcBool isconst )
{
	EcInt curdim, newdim;

	if (_ec_package_get_public( package, sym ) >= 0)
		return FALSE;											/* Already present ! */

	curdim  = EC_PACKAGENEXPORT(package) / EXPORTQUANTUM;
	curdim *= EXPORTQUANTUM;

	if (EC_PACKAGENEXPORT(package) + 1 >= curdim)
	{
		struct ec_public_sym *newExport;

		newdim = curdim + EXPORTQUANTUM;
		newExport = (struct ec_public_sym *) ec_realloc( EC_PACKAGEEXPORT(package), newdim * sizeof(struct ec_public_sym) );
		if (! newExport)
			return FALSE;

		EC_PACKAGEEXPORT(package) = newExport;
	}

	EC_PACKAGEEXPORT(package)[ EC_PACKAGENEXPORT(package) ].sym     = sym;
	EC_PACKAGEEXPORT(package)[ EC_PACKAGENEXPORT(package) ].isconst = isconst;
	EC_PACKAGEEXPORT(package)[ EC_PACKAGENEXPORT(package) ].pos     = pos;
	EC_PACKAGENEXPORT(package)++;

	return TRUE;
}

static EcInt _ec_package_get_public( EC_OBJ package, EcUInt sym )
{
	EcInt i;

	ASSERT( EC_PACKAGEP(package) );

	for (i = 0; i < EC_PACKAGENEXPORT(package); i++)
	{
		if (EC_PACKAGEEXPORT(package)[i].sym == sym)
			return EC_PACKAGEEXPORT(package)[i].pos;			/* #JP bug fixed: it returned `i' */
			/* return i; */
	}

	return -1;
}
