/* ==========================================================================
 * IDLTypeTable.ec
 * ==========================================================================
 * Type table - a table of C types.
 * The table is a set of registered types. Each type is an instance
 * supporting the IDLType interface.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created : Tue Oct 30 11:36:58 CET 2001
 *
 *   $Id: IDLTypeTable.ec,v 1.1 2002/05/31 15:14:14 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 2001 Marco Pantaleoni. All rights reserved.
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

/*
 * Type table and types.
 */

package IDLTypeTable;

import basic;
import string;
import array;
import hash;

import util;


/*
 * IDLType - Type
 *
 * Instances of this class represent fundamental types
 * (not fundamental in the C sense, but in the IDL sense: we
 *  consider derived types those who are pointers or const versions of
 *  other types, or similar combinations)
 */

public class IDLType extends basic.Object
{
	/* ------------------------------------------------------------------------
	 * METHODS
	 * ------------------------------------------------------------------------ */

	// `attributes' is a list of symbols
	// possible symbols are:
	//   #const
	//   #in
	//   #out
	//   #inout
	//   #pointer
	//   #nullok

	method getCType(attributes)
		// returns the C equivalent
		// (example: returns "char *" for c-string
		//           returns "const char *" for c-string with #const attrib.)
	{
	}

	method getEcParseStackCvtSpec(attributes)
		// returns the conversion specifier for EcParseStack*() API
		// (example: "i" for integers
		//           "O&" for types needing conversion callbacks)
	{
	}

	method getEcParseStackCvtCallback(attributes)
		// returns the conversion callback for EcParseStack*() API
		// (it makes sense only for "O&")
		// (example: "struct_cvt" for boxed types (C structs))
	{
	}

	method getEcParseStackCvtParams(cprefix, cvarname, nullok, attributes)
		// returns the list of parameters for EcParseStack*() API
		// (example: #["struct_cvt", &cvarname, (EcDWord)&infostruct, nullok ? "TRUE" : "FALSE"] for "O&" structs
		//           #[ &cvarname ] for fundamental types like integers)
	{
	}

	method getEcMakeValue( cprefix, cvalue, info )
		// return the string used to build the elastiC object from the C value
		// (example: cprefix + "_enum2obj( (long)" + cvalue + ", &" + [self getInfoStruct] + " )" for C enums
		//           "EcMakeInt(" + cvalue + ")" for integers)
	{
	}

	method getEcFromValue( cprefix, cvalue, info )
		// return the string used to obtain the C value from the elastiC object
		// (example: "(" + [self getName] + ") " + cprefix + "_obj2enum( " + cvalue + ", &" + [self getInfoStruct] + " )" for C enums
		//           "EC_INUM(" + cvalue + ")" for integers)
	{
	}
}

/*
 * IDLTypeTable - Type table
 */

public class IDLTypeTable extends basic.Object
{
	/* ------------------------------------------------------------------------
	 * INSTANCE DATA
	 * ------------------------------------------------------------------------ */

	local table;			// associate registered type names to instances (hash)

	/* ------------------------------------------------------------------------
	 * METHODS
	 * ------------------------------------------------------------------------ */

	// CONSTRUCTOR

	method init( desc )
	{
		table = %[];
	}

	// REPRESENTATION

	method _print( detailed )
	{
		return basic.sprintf( "<IDLTypeTable>" );
	}

	// Type handling

	method addType( name, typeobj )
		// add a fundamental type
		// typeobj is an instance of (a derived type of) IDLType
	{
		table[name] = typeobj;
	}

	method getType( name )
		// get a fundamental type
	{
		return table[name];
	}

	// Operations for C code generation
	// These are generic: they work both for fundamental types
	// and for derived ones.

	method getCType(typeobj, attributes)
		// returns the C equivalent
		// (example: returns "char *" for c-string
		//           returns "const char *" for c-string with #const attrib.)
	{
		local typ;
		local car;

		typ = basic.typeof(typeobj);
		if (typ == #object)
			return [typeobj getCType attributes];

		if ((typ == #symbol) || (typ == #string))
			return [[self getType typeobj] getCType attributes];

		// a sequence: handle modifiers
		car = typeobj[0];

		if (car 
	}

	method getEcParseStackCvtSpec(typeobj, attributes)
		// returns the conversion specifier for EcParseStack*() API
		// (example: "i" for integers
		//           "O&" for types needing conversion callbacks)
	{
	}

	method getEcParseStackCvtCallback(typeobj, attributes)
		// returns the conversion callback for EcParseStack*() API
		// (it makes sense only for "O&")
		// (example: "struct_cvt" for boxed types (C structs))
	{
	}

	method getEcParseStackCvtParams(typeobj, cprefix, cvarname, nullok, attributes)
		// returns the list of parameters for EcParseStack*() API
		// (example: #["struct_cvt", &cvarname, (EcDWord)&infostruct, nullok ? "TRUE" : "FALSE"] for "O&" structs
		//           #[ &cvarname ] for fundamental types like integers)
	{
	}

	method getEcMakeValue(typeobj, cprefix, cvalue, info)
		// return the string used to build the elastiC object from the C value
		// (example: cprefix + "_enum2obj( (long)" + cvalue + ", &" + [self getInfoStruct] + " )" for C enums
		//           "EcMakeInt(" + cvalue + ")" for integers)
	{
	}

	method getEcFromValue(typeobj, cprefix, cvalue, info)
		// return the string used to obtain the C value from the elastiC object
		// (example: "(" + [self getName] + ") " + cprefix + "_obj2enum( " + cvalue + ", &" + [self getInfoStruct] + " )" for C enums
		//           "EC_INUM(" + cvalue + ")" for integers)
	{
	}
}
