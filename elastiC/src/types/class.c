/* ==========================================================================
 * class.c
 * ==========================================================================
 * Class.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat May 23 11:02:52 MEST 1998
 *
 *   $Id: class.c,v 1.2 2002/05/25 19:37:45 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1998-2000 Marco Pantaleoni. All rights reserved.
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

EC_API EcInt tc_class = -1;

static EC_OBJ class_copy( EC_OBJ obj, EcCopyType type );
static void   class_mark( EC_OBJ obj );
static void   class_free( EC_OBJ obj );
static EcInt  class_print( ec_string *str, EC_OBJ obj, EcBool detailed );

static EcUInt class_hash( EC_OBJ obj, EcInt recur );
static EcBool class_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res );

static EcBool class_check( EC_OBJ obj );

/* C API */

EC_API EC_OBJ EcMakeClass( EC_OBJ package, EC_OBJ superclass, const char *name )
{
	EC_OBJ obj;
	char *clscodename;
	ec_string ds;

	clscodename = alloca( strlen( name ) + strlen( CODE_SUFFIX ) + 1 );
	if (! clscodename) return Ec_ERROR;

	strcpy( clscodename, name );
	strcat( clscodename, CODE_SUFFIX );

	obj = EcMakeUser( tc_class, NULL );
	if (EC_ERRORP(obj)) return obj;

	EC_CLASS(obj) = ec_malloc( sizeof(EcClass) );
	if (! EC_CLASS(obj)) return Ec_ERROR;

	EC_CLASSSUPER(obj)     = superclass;

	EC_CLASSNMETHODS(obj)  = 0;
	EC_CLASSMTABLE(obj)    = NULL;

	EC_CLASSNCMETHODS(obj) = 0;
	EC_CLASSCMTABLE(obj)   = NULL;

	EC_CLASSIOFFSET(obj)   = EC_NNULLP(superclass) ? EC_CLASSNIVARS(superclass) : 0;
	EC_CLASSNIVARS(obj)    = EC_NNULLP(superclass) ? EC_CLASSNIVARS(superclass) : 0;
	EC_CLASSIVTABLE(obj)   = NULL;

	EC_CLASSNCVARS(obj)    = 0;
	EC_CLASSCVTABLE(obj)   = NULL;

	ec_string_init( &ds, NULL );
	ec_sprintf( &ds, "%w.%s", EC_PACKAGENAME(package), name );
	EC_CLASSPACKAGE(obj)   = package;
	EC_CLASSNAME(obj)      = EcMakeString( ec_strdata( &ds ), ec_strlen( &ds ) );
	EC_CLASSSHORTNAME(obj) = EcMakeString( name, 0 );
	EC_CLASSCODE(obj)      = EcMakeCompiled( package, clscodename,
											 0,					/* req. parameters      */
											 0,					/* params with defaults */
											 FALSE,				/* varags               */
											 FALSE );			/* is a method ?        */
	ec_string_cleanup( &ds );

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

	return obj;
}

/* elastiC API */


/* Private */

static EC_OBJ class_copy( EC_OBJ obj, EcCopyType type )
{
	/*******************************************************************
	 * Classes are *SPECIAL* !                                         *
	 * The must not be copied, since they are sort of metaobjects      *
	 * When an object references a class, a copy of the object should  *
	 * reference the same class !                                      *
	 *******************************************************************/
	return obj;
}

static void class_mark( EC_OBJ obj )
{
	EcInt i;

	ASSERT( EC_CLASSP(obj) );

	if (EC_NNULLP(EC_CLASSSUPER(obj))) EcMarkObject( EC_CLASSSUPER(obj) );
	for (i = 0; i < EC_CLASSNMETHODS(obj); i++)
		if (EC_NNULLP(EC_CLASSMTABLE(obj)[i].impl))  EcMarkObject( EC_CLASSMTABLE(obj)[i].impl );
	for (i = 0; i < EC_CLASSNCMETHODS(obj); i++)
		if (EC_NNULLP(EC_CLASSCMTABLE(obj)[i].impl)) EcMarkObject( EC_CLASSCMTABLE(obj)[i].impl );
	if (EC_NNULLP(EC_CLASSPACKAGE(obj)))   EcMarkObject( EC_CLASSPACKAGE(obj) );
	if (EC_NNULLP(EC_CLASSNAME(obj)))      EcMarkObject( EC_CLASSNAME(obj) );
	if (EC_NNULLP(EC_CLASSSHORTNAME(obj))) EcMarkObject( EC_CLASSSHORTNAME(obj) );
	if (EC_NNULLP(EC_CLASSCODE(obj)))      EcMarkObject( EC_CLASSCODE(obj) );
}

static void class_free( EC_OBJ obj )
{
	ASSERT( EC_CLASSP(obj) );

	EC_CLASSSUPER(obj)     = EC_NIL;
	EC_CLASSNMETHODS(obj)  = 0;
	ec_free( EC_CLASSMTABLE(obj) );
	EC_CLASSMTABLE(obj)    = NULL;
	EC_CLASSNCMETHODS(obj) = 0;
	ec_free( EC_CLASSCMTABLE(obj) );
	EC_CLASSCMTABLE(obj)   = NULL;
	EC_CLASSIOFFSET(obj)   = 0;
	EC_CLASSNIVARS(obj)    = 0;
	ec_free( EC_CLASSIVTABLE(obj) );
	EC_CLASSIVTABLE(obj)   = NULL;
	EC_CLASSNCVARS(obj)    = 0;
	ec_free( EC_CLASSCVTABLE(obj) );
	EC_CLASSCVTABLE(obj)   = NULL;
	EC_CLASSPACKAGE(obj)   = EC_NIL;
	EC_CLASSNAME(obj)      = EC_NIL;
	EC_CLASSSHORTNAME(obj) = EC_NIL;
	EC_CLASSCODE(obj)      = EC_NIL;
}

static EcInt class_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	EcInt res = 0;

	ASSERT( EC_CLASSP(obj) );

	res  = ec_sprintf( str, "<class %w>", EC_CLASSNAME(obj) );
	return res;
}

static EcUInt class_hash( EC_OBJ obj, EcInt recur )
{
	EcUInt res;

	ASSERT( EC_CLASSP(obj) );

	res = 0;
	res += _ec_hash( EC_CLASSSUPER(obj), recur );
	res += ec_hash_uint( (EcUInt)EC_CLASSNMETHODS(obj) );
	res += ec_hash_uint( (EcUInt)EC_CLASSNCMETHODS(obj) );
	res += ec_hash_uint( (EcUInt)EC_CLASSIOFFSET(obj) );
	res += ec_hash_uint( (EcUInt)EC_CLASSNIVARS(obj) );
	res += ec_hash_uint( (EcUInt)EC_CLASSNCVARS(obj) );
	res += _ec_hash( EC_CLASSPACKAGE(obj), recur );
	res += _ec_hash( EC_CLASSNAME(obj), recur );
/*	res += _ec_hash( EC_CLASSSHORTNAME(obj), recur );*/
	res += _ec_hash( EC_CLASSCODE(obj), recur );
	return res;
}

static EcBool class_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res )
{
	EcInt i, specific;

	if (obj1 == obj2)
	{
		*res = 0;
		return TRUE;
	}

	if (! EC_CLASSP(obj2))
		return FALSE;

	*res = 0;
	if (! EcObjectEqual( EC_CLASSSUPER(obj1), EC_CLASSSUPER(obj2) ))
		return FALSE;
	if (EC_CLASSNMETHODS(obj1) != EC_CLASSNMETHODS(obj2))
		return FALSE;
	if (EC_CLASSNCMETHODS(obj1) != EC_CLASSNCMETHODS(obj2))
		return FALSE;
	if (EC_CLASSIOFFSET(obj1) != EC_CLASSIOFFSET(obj2))
		return FALSE;
	if (EC_CLASSNIVARS(obj1) != EC_CLASSNIVARS(obj2))
		return FALSE;
	if (EC_CLASSNCVARS(obj1) != EC_CLASSNCVARS(obj2))
		return FALSE;
	if (! EcObjectEqual( EC_CLASSPACKAGE(obj1), EC_CLASSPACKAGE(obj2) ))
		return FALSE;
	if (! EcObjectEqual( EC_CLASSNAME(obj1), EC_CLASSNAME(obj2) ))
		return FALSE;
	if (! EcObjectEqual( EC_CLASSSHORTNAME(obj1), EC_CLASSSHORTNAME(obj2) ))
		return FALSE;
	if (! EcObjectEqual( EC_CLASSCODE(obj1), EC_CLASSCODE(obj2) ))
		return FALSE;

	for (i = 0; i < EC_CLASSNMETHODS(obj1); i++)
	{
		if (EC_CLASSMTABLE(obj1)[i].symid != EC_CLASSMTABLE(obj2)[i].symid)
			return FALSE;
		if (! EcObjectEqual( EC_CLASSMTABLE(obj1)[i].impl, EC_CLASSMTABLE(obj2)[i].impl ))
			return FALSE;
	}
	for (i = 0; i < EC_CLASSNCMETHODS(obj1); i++)
	{
		if (EC_CLASSCMTABLE(obj1)[i].symid != EC_CLASSCMTABLE(obj2)[i].symid)
			return FALSE;
		if (! EcObjectEqual( EC_CLASSCMTABLE(obj1)[i].impl, EC_CLASSCMTABLE(obj2)[i].impl ))
			return FALSE;
	}

	specific = EC_CLASSNIVARS(obj1) - EC_CLASSIOFFSET(obj1);
	if (memcmp( EC_CLASSIVTABLE(obj1), EC_CLASSIVTABLE(obj2), specific * sizeof(EcVariableEntry) ) != 0)
		return FALSE;

	if (memcmp( EC_CLASSCVTABLE(obj1), EC_CLASSCVTABLE(obj2), EC_CLASSNCVARS(obj1) * sizeof(EcVariableEntry) ) != 0)
		return FALSE;

	return TRUE;
}

static EcBool class_check( EC_OBJ obj )
{
/*
		EC_OBJ impl;

		ASSERTMEM( EC_CLASS(obj) );
		ASSERTFIELDN( EC_CLASSSUPER(obj), tc_class );
		ASSERTCOND( EC_CLASSNMETHODS(obj) >= 0 );
		for (i = 0; i < EC_CLASSNMETHODS(obj); i++)
		{
			impl = EC_CLASSMTABLE(obj)[i].impl;
			ASSERTCOND( EC_CMETHODP(impl) || EC_COMPILEDP(impl) );
		}
		ASSERTCOND( EC_CLASSNCMETHODS(obj) >= 0 );
		for (i = 0; i < EC_CLASSNCMETHODS(obj); i++)
		{
			impl = EC_CLASSCMTABLE(obj)[i].impl;
			ASSERTCOND( EC_CMETHODP(impl) || EC_COMPILEDP(impl) );
		}
		ASSERTCOND( EC_CLASSIOFFSET(obj) >= 0 );
		ASSERTCOND( EC_CLASSNIVARS(obj) >= 0 );
		ASSERTCOND( EC_CLASSNCVARS(obj) >= 0 );
		ASSERTFIELD( EC_CLASSPACKAGE(obj), tc_package );
		ASSERTFIELD( EC_CLASSNAME(obj), tc_string );
		ASSERTFIELD( EC_CLASSSHORTNAME(obj), tc_string );
		ASSERTFIELD( EC_CLASSCODE(obj), tc_compiled );
		ASSERTFIELD( EC_CLASSLFRAME(obj), tc_array );
*/
	return TRUE;
}

EcBool _ec_class_init( void )
{
	EcTypespec classspec = {
		/* type     */		0,
		/* name     */		"class",

		/* copy     */		class_copy,

		/* mark     */		class_mark,
		/* free     */		class_free,
		/* print    */		class_print,

		/* hash     */		class_hash,
		/* compare  */		class_compare,

		/* check    */		class_check,

		/* sequence */      NULL,
		/* numeric  */      NULL
	};

	tc_class = EcAddType( &classspec );
	if (tc_class == 0)
		return FALSE;

	return TRUE;
}

void _ec_class_cleanup( void )
{
}
