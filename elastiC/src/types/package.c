/* ==========================================================================
 * package.c
 * ==========================================================================
 * Package.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat May 23 11:02:52 MEST 1998
 *
 *   $Id: package.c,v 1.2 2002/05/25 19:37:45 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1998-2001 Marco Pantaleoni. All rights reserved.
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

#include "elastic.h"
#include "private.h"
#include "compat.h"
#include "debug.h"

#include "hashfunc.h"

EC_API EcInt tc_package = -1;

static EC_OBJ package_copy( EC_OBJ obj, EcCopyType type );

static void   package_mark( EC_OBJ obj );
static void   package_free( EC_OBJ obj );
static EcInt  package_print( ec_string *str, EC_OBJ obj, EcBool detailed );

static EcUInt package_hash( EC_OBJ obj, EcInt recur );
static EcBool package_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res );

static EcBool package_check( EC_OBJ obj );

/* C API */

EC_API EC_OBJ EcMakePackage( const char *name )
{
	EC_OBJ obj;
	char *pkgcodename;

	if (name)
	{
		pkgcodename = alloca( strlen( name ) + strlen( CODE_SUFFIX ) + 1 );
		if (! pkgcodename) return Ec_ERROR;

		strcpy( pkgcodename, name );
		strcat( pkgcodename, CODE_SUFFIX );
	} else
		pkgcodename = NULL;

	obj = EcMakeUser( tc_package, NULL );
	if (EC_ERRORP(obj)) return obj;

	EC_PACKAGE(obj) = ec_malloc( sizeof(EcPackage) );
	if (! EC_PACKAGE(obj)) return Ec_ERROR;

	EC_PACKAGENAME(obj)     = name ? EcMakeString( name, 0 ) : EC_NIL;
	EC_PACKAGECODE(obj)     = EcMakeCompiled( obj, pkgcodename, 0, 0, FALSE, FALSE ); /* ... a package has no args */
	EC_PACKAGEFRAME(obj)    = EcMakeArray( 0 );
	EC_PACKAGEIMPORT(obj)   = EcMakeArray( 0 );
	EC_PACKAGENEXPORT(obj)  = 0;
	EC_PACKAGEEXPORT(obj)   = NULL;
	EC_PACKAGESOURCE(obj)   = EC_NIL;

#if 0
	/* Every package import basic package (with the noteworthy exception of the basic package itself) */
	if (PRIVATE(npackages) > 0)
		EcPackageImport( obj, "basic", EC_NIL );
#endif

	return obj;
}

/* elastiC API */


/* Private */

static EC_OBJ package_copy( EC_OBJ obj, EcCopyType type )
{
	/*******************************************************************
	 * Packages are *SPECIAL* !                                        *
	 * The must not be copied, since they are sort of metaobjects      *
	 * (as classes, see class.c for further comments.                  *
	 *******************************************************************/
	return obj;
}

static void package_mark( EC_OBJ obj )
{
	ASSERT( EC_PACKAGEP(obj) );

	if (EC_NNULLP(EC_PACKAGENAME(obj)))   EcMarkObject( EC_PACKAGENAME(obj) );
	if (EC_NNULLP(EC_PACKAGECODE(obj)))   EcMarkObject( EC_PACKAGECODE(obj) );
	if (EC_NNULLP(EC_PACKAGEFRAME(obj)))  EcMarkObject( EC_PACKAGEFRAME(obj) );
	if (EC_NNULLP(EC_PACKAGEIMPORT(obj))) EcMarkObject( EC_PACKAGEIMPORT(obj) );
	if (EC_NNULLP(EC_PACKAGESOURCE(obj))) EcMarkObject( EC_PACKAGESOURCE(obj) );
}

static void package_free( EC_OBJ obj )
{
	ASSERT( EC_PACKAGEP(obj) );

	EC_PACKAGENAME(obj)   = EC_NIL;
	EC_PACKAGECODE(obj)   = EC_NIL;
	EC_PACKAGEFRAME(obj)  = EC_NIL;
	EC_PACKAGEIMPORT(obj) = EC_NIL;
	ec_free( EC_PACKAGEEXPORT(obj) );
	EC_PACKAGEEXPORT(obj)  = NULL;
	EC_PACKAGENEXPORT(obj) = 0;
	EC_PACKAGESOURCE(obj)  = EC_NIL;

	ec_free( EC_PACKAGE(obj) );
	EC_PACKAGE(obj) = NULL;
}

static EcInt package_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	EcInt res = 0;

	ASSERT( EC_PACKAGEP(obj) );

	res = ec_sprintf( str, "<package `%w' nexport:%ld>",
					  EC_PACKAGENAME(obj),
					  (long)EC_PACKAGENEXPORT(obj) );
	return res;
}

static EcUInt package_hash( EC_OBJ obj, EcInt recur )
{
	EcUInt res;

	ASSERT( EC_PACKAGEP(obj) );

	res = 0;
	if (! EC_PACKAGE(obj))
		return res;
	res += _ec_hash( EC_PACKAGENAME(obj), recur );
	res += _ec_hash( EC_PACKAGECODE(obj), recur );
	res += _ec_hash( EC_PACKAGEFRAME(obj), recur );
	res += _ec_hash( EC_PACKAGEIMPORT(obj), recur );
	res += ec_hash_uint( (EcUInt)EC_PACKAGENEXPORT(obj) );
	if (EC_PACKAGEEXPORT(obj))
		res += ec_hash_pjw_memory( (const char *)EC_PACKAGEEXPORT(obj), EC_PACKAGENEXPORT(obj) * sizeof(struct ec_public_sym) );
	res += _ec_hash( EC_PACKAGESOURCE(obj), recur );
	return res;
}

static EcBool package_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res )
{
	if (obj1 == obj2)
	{
		*res = 0;
		return TRUE;
	}

	if (! EC_PACKAGEP(obj2))
		return FALSE;

	*res = 0;
	if (((EC_PACKAGE(obj1) && (! EC_PACKAGE(obj2))) ||
		 ((! EC_PACKAGE(obj1)) && EC_PACKAGE(obj2))))
		return FALSE;
	if (! EC_PACKAGE(obj1))
		return TRUE;
	if (! EcObjectEqual( EC_PACKAGENAME(obj1), EC_PACKAGENAME(obj2) ))
		return FALSE;
	if (! EcObjectEqual( EC_PACKAGECODE(obj1), EC_PACKAGECODE(obj2) ))
		return FALSE;
	if (! EcObjectEqual( EC_PACKAGEFRAME(obj1), EC_PACKAGEFRAME(obj2) ))
		return FALSE;
	if (! EcObjectEqual( EC_PACKAGEIMPORT(obj1), EC_PACKAGEIMPORT(obj2) ))
		return FALSE;
	if (EC_PACKAGENEXPORT(obj1) != EC_PACKAGENEXPORT(obj2))
		return FALSE;
	if (((EC_PACKAGEEXPORT(obj1) && (! EC_PACKAGEEXPORT(obj2))) ||
		 ((! EC_PACKAGEEXPORT(obj1)) && EC_PACKAGEEXPORT(obj2))))
		return FALSE;
	if (memcmp( EC_PACKAGEEXPORT(obj1), EC_PACKAGEEXPORT(obj2), EC_PACKAGENEXPORT(obj1) * sizeof(struct ec_public_sym) ) != 0)
		return FALSE;
	if (! EcObjectEqual( EC_PACKAGESOURCE(obj1), EC_PACKAGESOURCE(obj2) ))
		return FALSE;
	return TRUE;
}

static EcBool package_check( EC_OBJ obj )
{
/*	ASSERT( EC_PACKAGEP(obj) );
	ASSERTMEM( EC_PACKAGE(obj) );
	ASSERTFIELD( EC_PACKAGENAME(obj), tc_string );
	ASSERTFIELDN( EC_PACKAGECODE(obj), tc_compiled );
	ASSERTFIELD( EC_PACKAGEFRAME(obj), tc_array );
	ASSERTFIELDN( EC_PACKAGESOURCE(obj), tc_string );
	ASSERTFIELD( EC_PACKAGEIMPORT(obj), tc_array );
	ASSERTCOND( EC_PACKAGENEXPORT(obj) >= 0 );
*/
	return TRUE;
}

EcBool _ec_package_init( void )
{
	EcTypespec packagespec = {
		/* type    */		0,
		/* name    */		"package",

		/* copy    */		package_copy,

		/* mark    */		package_mark,
		/* free    */		package_free,
		/* print   */		package_print,

		/* hash    */		package_hash,
		/* compare */		package_compare,

		/* check   */		package_check,

		/* sequence */      NULL,
		/* numeric  */      NULL
	};

	tc_package = EcAddType( &packagespec );
	if (tc_package == 0)
		return FALSE;

	return TRUE;
}

void _ec_package_cleanup( void )
{
}
