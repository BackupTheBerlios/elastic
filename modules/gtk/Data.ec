/* ==========================================================================
 * Data.ec
 * ==========================================================================
 * OO data models for InterfaceGen.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat Nov 27 18:22:35 MET 1999
 *
 *   $Id: Data.ec,v 1.1 2002/05/30 12:41:39 panta Exp $
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

/*
 * Object Oriented nodes used to represent module contents,
 * specified originally by the Scheme IDL file.
 */

package Data;

import basic;
import string;
import hash;
import array;
import file;

import scmparse;										// Scheme parser
import Util;

/*
 * Visitable class
 */

public class Visitable extends basic.Object
{
	method visit: visitor with: args ...
	{
		local methodsym;
		local newargs;
		local arg;

		methodsym = string.makesymbol( "visit" + [[self isA] name] );
		if ([visitor doesUnderstand: methodsym])
		{
			newargs = #[ self ];
			for (arg in args)
				newargs[basic.length(newargs)] = arg;
			return basic.sendwith( visitor, methodsym, newargs );
		}
		return @nil;
	}

	method visit: visitor
	{
		return [self visit: visitor with: @nil];
	}
}

/*
 * Symbolic class
 *
 * Implements a symbol collection, acting as abstract base
 * for Enum and Flags classes.
 */
public class Symbolic extends Visitable
{
	/* ------------------------------------------------------------------------
	 * INSTANCE DATA
	 * ------------------------------------------------------------------------ */

	local name;

	local infostruct;

	local dim;
	local index;

	// hash of mapping elastiC name -> C value
	local name2value;

	// inverse mapping
	local value2name;

	// hash of mapping elastiC name -> index in array
	local name2idx;

	// inverse mapping
	local idx2name;

	/* ------------------------------------------------------------------------
	 * METHODS
	 * ------------------------------------------------------------------------ */

	// CONSTRUCTOR

	method init( desc )
	{
		name  = desc[1];
		infostruct = @nil;
		index = -1;
		name2value = %[];
		value2name = %[];
		name2idx   = %[];
		idx2name   = %[];

		local i, len = basic.length(desc);
		local el;
		dim = 0;
		for (i = 2; i < len; i++)
		{
			el = desc[i];
			name2value[el[0]] = el[1];
			value2name[el[1]] = el[0];
			name2idx[el[0]]   = dim;
			idx2name[dim]     = el[0];
			dim++;
		}
	}

	// GETTERS & SETTERS

	method getName()
	{
		return name;
	}

	method getCName()
	{
		return "enum" + Util.canonicalize_name( name, @true );
	}

	method getInfoStruct()
	{
		return infostruct;
	}

	method setInfoStruct: istruct
	{
		infostruct = istruct;
	}

	method getNumber()
	{
		return dim;
	}

	method setIndex(idx)
	{
		index = idx;
	}

	method getIndex()
	{
		return index;
	}

	method getNames()
	{
		return hash.keys( name2value );
	}

	method nameToValue( sname )
	{
		return name2value[sname];
	}

	method valueToName( svalue )
	{
		return value2name[svalue];
	}

	method nameToIndex( sname )
	{
		return name2idx[sname];
	}

	method indexToName( sindex )
	{
		return idx2name[sindex];
	}

	// INFORMATION

	method getCType()
	{
		return name;
	}
}

/*
 * Enum class
 *
 * Implements a enum object, representing a C 'enum'.
 */
public class Enum extends Symbolic
{
	/* ------------------------------------------------------------------------
	 * METHODS
	 * ------------------------------------------------------------------------ */

	// CONSTRUCTOR

	method init( desc )
	{
		[super init desc];
	}

	// REPRESENTATION

	method _print( detailed )
	{
		return basic.sprintf( "<enum %r>", [super getName] );
	}

	method getType()
	{
		return "enum";
	}

	method getEcCvtString()
	{
		return "O&";
	}

	method getEcCvtFunc()
	{
		return "enum_cvt";
	}

	method getEcCvtNParams()
	{
		return "1";
	}

	method getEcCvtParams( cprefix, cvarname, nullok )
	{
		return #[[self getEcCvtFunc], "&" + cvarname, "(EcDWord)&" + [self getInfoStruct], nullok ? "TRUE" : "FALSE"];
	}

	method getEcValueBuilder( cprefix, cvalue, info )
	{
		return cprefix + "_enum2obj( (long)" + cvalue + ", &" + [self getInfoStruct] + " )";
	}

	method getEcFromObj( cprefix, cvalue )
	{
		return "(" + [self getName] + ") " + cprefix + "_obj2enum( " + cvalue + ", &" + [self getInfoStruct] + " )";
	}
}

/*
 * Flags class
 *
 * Implements a flags object, representing a C OR-able 'enum' or #define's.
 */
public class Flags extends Symbolic
{
	/* ------------------------------------------------------------------------
	 * METHODS
	 * ------------------------------------------------------------------------ */

	// CONSTRUCTOR

	method init( desc )
	{
		[super init desc];
	}

	// REPRESENTATION

	method _print( detailed )
	{
		return basic.sprintf( "<flags %r>", [super getName] );
	}

	method getType()
	{
		return "flags";
	}

	method getEcCvtString()
	{
		return "O&";
	}

	method getEcCvtFunc()
	{
		return "flags_cvt";
	}

	method getEcCvtNParams()
	{
		return "1";
	}

	method getEcCvtParams( cprefix, cvarname, nullok )
	{
		return #[[self getEcCvtFunc], "&" + cvarname, "(EcDWord)&" + [self getInfoStruct], nullok ? "TRUE" : "FALSE"];
	}

	method getEcValueBuilder( cprefix, cvalue, info )
	{
		return cprefix + "_flags2obj( (long)" + cvalue + ", &" + [self getInfoStruct] + " )";
	}

	method getEcFromObj( cprefix, cvalue )
	{
		return "(" + [self getName] + ") " + cprefix + "_obj2flags( " + cvalue + ", &" + [self getInfoStruct] + " )";
	}
}

/*
 * Boxed class
 *
 * Implements a boxed object, representing a C 'struct'.
 */
public class Boxed extends Visitable
{
	/* ------------------------------------------------------------------------
	 * INSTANCE DATA
	 * ------------------------------------------------------------------------ */

	local name;
	local infostruct;

	local index;

	local copyFunc;
	local freeFunc;
	local size;
	local fields;
	local fieldType;

	local ec_getter_name;
	local c_getter_name;
	local ec_setter_name;
	local c_setter_name;

	/* ------------------------------------------------------------------------
	 * METHODS
	 * ------------------------------------------------------------------------ */

	// CONSTRUCTOR

	method init( desc )
	{
		name  = desc[1];
		infostruct = @nil;
		index = -1;

		fields    = #[];
		fieldType = %[];

		copyFunc = "NULL";
		freeFunc = "NULL";
		size     = "0";

		if ((basic.length(desc) >=  3) && (basic.typeof( desc[2] ) == #array))
		{
			// new format

			local i, len = basic.length(desc);
			local el;
			for (i = 2; i < len; i++)
			{
				el = desc[i];
				if (el[0] == "copy")
					copyFunc = el[1];
				else if (el[0] == "free")
					freeFunc = el[1];
				else if (el[0] == "size")
					size = string.sub( el[1], 1, -1 );
				else if (el[0] == "fields")
					[self processFields el];
			}
		} else
		{
			// old format

			local len = basic.length(desc) - 2;
			if (len >= 1)
				copyFunc = desc[2];
			if (len >= 2)
				freeFunc = desc[3];
			if (len >= 3)
				size = desc[4];
			if (len >= 4)
				[self processFields desc[5]];
		}
	}

	// REPRESENTATION

	method _print( detailed )
	{
		return basic.sprintf( "<boxed %r %r>", name, fields );
	}

	// GETTERS & SETTERS

	method getName()
	{
		return name;
	}

	method getInfoStruct()
	{
		return infostruct;
	}

	method setInfoStruct: istruct
	{
		infostruct = istruct;
	}

	method setIndex(idx)
	{
		index = idx;
	}

	method getIndex()
	{
		return index;
	}

	method getCopyFunc()
	{
		return copyFunc;
	}

	method getFreeFunc()
	{
		return freeFunc;
	}

	method getSize()
	{
		return size;
	}

	method getFields()
	{
		return fields;
	}

	method getFieldType( field )
	{
		return fieldType[field];
	}

	method getEcGetter()
	{
		return ec_getter_name;
	}

	method setEcGetter(name)
	{
		ec_getter_name = name;
	}

	method getEcSetter()
	{
		return ec_setter_name;
	}

	method setEcSetter(name)
	{
		ec_setter_name = name;
	}

	method getCGetter()
	{
		return c_getter_name;
	}

	method setCGetter(name)
	{
		c_getter_name = name;
	}

	method getCSetter()
	{
		return c_setter_name;
	}

	method setCSetter(name)
	{
		c_setter_name = name;
	}

	// INFORMATION

	method getCType()
	{
		return name + " *";
	}

	method getEcCvtString()
	{
		return "O&";
	}

	method getEcCvtFunc()
	{
		return "boxed_cvt";
	}

	method getEcCvtNParams()
	{
		return "1";
	}

	method getEcCvtParams( cprefix, cvarname, nullok )
	{
		return #[[self getEcCvtFunc], "&" + cvarname, "(EcDWord)&" + [self getInfoStruct], nullok ? "TRUE" : "FALSE"];
	}

	method getEcValueBuilder( cprefix, cvalue, info )
	{
		return cprefix + "_boxed2obj( (gpointer)" + cvalue + ", &" + [self getInfoStruct] + ", TRUE )";
	}

	method getEcFromObj( cprefix, cvalue )
	{
		return "(" + [self getCType] + ") " + cprefix + "_obj2boxed( " + cvalue + " )";
	}

	// PRIVATE

	method processFields( lfields )
	{
		local i, len = basic.length(lfields);
		local start;
		local flddesc;
		local fldtype, fldname;

		if ((len >= 1) && (lfields[0] == "fields"))
			start = 1;
		else
			start = 0;
		for (i = start; i < len; i++)
		{
			flddesc = lfields[i];
			fldtype = flddesc[0];
			fldname = flddesc[1];

			fields[basic.length(fields)] = fldname;
			fieldType[fldname] = fldtype;
		}
	}
}

/*
 * Function class
 *
 * Implements a function object, representing a C function.
 */
public class Function extends Visitable
{
	/* ------------------------------------------------------------------------
	 * INSTANCE DATA
	 * ------------------------------------------------------------------------ */

	local name;
	local infostruct;

	local retType;
	local parameters;
	local parType;
	local parDefault;
	local parNullOk;

	local ec_function_name;
	local c_function_name;

	/* ------------------------------------------------------------------------
	 * METHODS
	 * ------------------------------------------------------------------------ */

	// CONSTRUCTOR

	method init( desc )
	{
		name = desc[1];

		infostruct = @nil;

		retType = (desc[2] == "none") ? @nil : desc[2];

		parameters = #[];
		parType    = %[];
		parDefault = %[];
		parNullOk  = %[];

		local params, p;
		local pname, ptype, pdef = @nil, pnullok = @false;
		local i, l;

		params = desc[3];
		for (p in params)
		{
			ptype = p[0];
			pname = p[1];
			l = basic.length(p);
			pdef    = @nil;
			pnullok = @false;
			for (i = 2; i < l; i++)
			{
				if (basic.typeof(p[i]) == #array)
				{
					if (p[i][0] == "=")
						pdef = p[i][1];
					else if (p[i][0] == "null-ok")
						pnullok = @true;
				}
				else
					pdef = p[2];
			}
			parameters[basic.length(parameters)] = pname;
			parType[pname]    = ptype;
			parDefault[pname] = pdef;
			parNullOk[pname]  = pnullok;
		}
	}

	// REPRESENTATION

	method _print( detailed )
	{
		return basic.sprintf( "<function %r ret: %r params: %r>", name, retType, parameters );
	}

	// GETTERS & SETTERS

	method getName()
	{
		return name;
	}

	method getInfoStruct()
	{
		return infostruct;
	}

	method setInfoStruct: istruct
	{
		infostruct = istruct;
	}

	method getReturnType()
	{
		return retType;
	}

	method getParameters()
	{
		return parameters;
	}

	method getParameterType( nameOrNum )
	{
		return parType[[self normParam nameOrNum]];
	}

	method getParameterDefault( nameOrNum )
	{
		return parDefault[[self normParam nameOrNum]];
	}

	method getParameterNullOk( nameOrNum )
	{
		return parNullOk[[self normParam nameOrNum]];
	}

	method getEcFunctionName()
	{
		return ec_function_name;
	}

	method setEcFunctionName: fname
	{
		ec_function_name = fname;
	}

	method getCFunctionName()
	{
		return c_function_name;
	}

	method setCFunctionName: fname
	{
		c_function_name = fname;
	}

	// PRIVATE

	method normParam( nameOrNum )
	{
		if (basic.typeof(nameOrNum) == #int)
			return parameters[nameOrNum];
		else
			return nameOrNum;
	}
}

/*
 * Object class
 *
 * Implements an object object :), representing a C object.
 */
public class Object extends Visitable
{
	/* ------------------------------------------------------------------------
	 * INSTANCE DATA
	 * ------------------------------------------------------------------------ */

	local name;
	local infostruct;

	local bases;
	local fields;
	local fieldType;

	local ec_getter_name;
	local c_getter_name;
	local ec_setter_name;
	local c_setter_name;

	/* ------------------------------------------------------------------------
	 * METHODS
	 * ------------------------------------------------------------------------ */

	// CONSTRUCTOR

	method init( desc )
	{
		name = desc[1];

		infostruct = @nil;

		bases = desc[2];

		fields    = #[];
		fieldType = %[];

		local i, len = basic.length(desc);
		local el;
		for (i = 3; i < len; i++)
		{
			el = desc[i];

			if ((basic.typeof(el) == #array) &&
				(el[0] == "fields"))
			{
				local j, el_len;
				local fld, fldname, fldtype;

				el_len = basic.length(el);
				for (j = 1; j < el_len; j++)
				{
					fld = el[j];
					fldtype = fld[0];
					fldname = fld[1];

					fields[basic.length(fields)] = fldname;
					fieldType[fldname] = fldtype;
				}
			}
		}
	}

	// REPRESENTATION

	method _print( detailed )
	{
		return basic.sprintf( "<object %r bases: %r fields: %r>", name, bases, fields );
	}

	// GETTERS & SETTERS

	method getName()
	{
		return name;
	}

	method getInfoStruct()
	{
		return infostruct;
	}

	method setInfoStruct: istruct
	{
		infostruct = istruct;
	}

	method getBases()
	{
		return bases;
	}

	method getFields()
	{
		return fields;
	}

	method getFieldType( name )
	{
		return fieldType[name];
	}

	method getEcGetter()
	{
		return ec_getter_name;
	}

	method setEcGetter(name)
	{
		ec_getter_name = name;
	}

	method getEcSetter()
	{
		return ec_setter_name;
	}

	method setEcSetter(name)
	{
		ec_setter_name = name;
	}

	method getCGetter()
	{
		return c_getter_name;
	}

	method setCGetter(name)
	{
		c_getter_name = name;
	}

	method getCSetter()
	{
		return c_setter_name;
	}

	method setCSetter(name)
	{
		c_setter_name = name;
	}

	// INFORMATION

	method getCType()
	{
		return name + " *";
	}

	method getEcCvtString()
	{
		return "O&";
	}

	method getEcCvtFunc()
	{
		return "object_cvt";
	}

	method getEcCvtNParams()
	{
		return "1";
	}

	method getEcCvtParams( cprefix, cvarname, nullok )
	{
		return #[[self getEcCvtFunc], "&" + cvarname, "(EcDWord)" + [self getInfoStruct], nullok ? "TRUE" : "FALSE"];
	}

	method getEcValueBuilder( cprefix, cvalue, info )
	{
		return cprefix + "_object2obj( (GtkObject *)" + cvalue + " )";
	}

	method getEcFromObj( cprefix, cvalue )
	{
		return "(" + [self getCType] + ") " + cprefix + "_obj2object( " + cvalue + " )";
	}
}

public function ctype( module, datum )
{
	local tobj = @nil;

	if (basic.typeof(datum) == #string)
	{
		local res = "UNKNOWN";

		if      (datum == "none")          res = "void";
		else if (datum == "short")         res = "EcInt";
		else if (datum == "ushort")        res = "EcInt";
		else if (datum == "int")           res = "EcInt";
		else if (datum == "uint")          res = "EcInt";
		else if (datum == "long")          res = "EcInt";
		else if (datum == "ulong")         res = "EcInt";
		else if (datum == "char")          res = "EcChar";
		else if (datum == "uchar")         res = "EcChar";
		else if (datum == "float")         res = "EcFloat";
		else if (datum == "double")        res = "EcFloat";
		else if (datum == "bool")          res = "EcBool";
		else if (datum == "string")        res = "EcChar *";
		else if (datum == "static_string") res = "const EcChar *";
		else {
			tobj = [module getType: datum];
			if (tobj)
				res = [tobj getCType];
		}

		if (res == "UNKNOWN")
			basic.print( "WARNING: Unknown type '", datum, "'\n" );
		return res;
	}

	return [datum getCType];
}

public function cvt_info( toplevelmodule, module, datum, cvarname, defval, nullok )
{
	/*
	 * return a list #[ctype, cs, params, post, defval, nullok, datum]
	 *
	 * where:
	 *   ctype   is the equivalent C type
	 *   cs      is the EcParseStackFunction conversion string
	 *   params  is the parameter list for EcParseStackFunction
	 *   post    is the post-conversion code to insert (or @nil)
	 *   datum   is the object itself
	 */

	local cprefix = [toplevelmodule getCPrefix];
	local tobj;

	local pvarname = "&" + cvarname;

	if (basic.typeof(datum) == #string)
	{
		local res = "UNKNOWN";

		if      (datum == "none")          res = #["void",           "",  @nil,        @nil, defval, nullok, datum];
		else if (datum == "short")         res = #["EcInt",          "i", #[pvarname], @nil, defval, nullok, datum];
		else if (datum == "ushort")        res = #["EcInt",          "i", #[pvarname], @nil, defval, nullok, datum];
		else if (datum == "int")           res = #["EcInt",          "i", #[pvarname], @nil, defval, nullok, datum];
		else if (datum == "uint")          res = #["EcInt",          "i", #[pvarname], @nil, defval, nullok, datum];
		else if (datum == "long")          res = #["EcInt",          "i", #[pvarname], @nil, defval, nullok, datum];
		else if (datum == "ulong")         res = #["EcInt",          "i", #[pvarname], @nil, defval, nullok, datum];
		else if (datum == "char")          res = #["EcChar",         "c", #[pvarname], @nil, defval, nullok, datum];
		else if (datum == "uchar")         res = #["EcChar",         "c", #[pvarname], @nil, defval, nullok, datum];
		else if (datum == "float")         res = #["EcFloat",        "f", #[pvarname], @nil, defval, nullok, datum];
		else if (datum == "double")        res = #["EcFloat",        "f", #[pvarname], @nil, defval, nullok, datum];
		else if (datum == "bool")          res = #["EcBool",         "b", #[pvarname], @nil, defval, nullok, datum];
		else if (datum == "string")        res = #["EcChar *",       "s", #[pvarname], @nil, defval, nullok, datum];
		else if (datum == "static_string") res = #["const EcChar *", "s", #[pvarname], @nil, defval, nullok, datum];
		else if (datum == "callback")      res = #["EC_OBJ",         "O", #[pvarname], "EcGCProtect( " + cvarname + " );", defval, nullok, datum];
		else if (datum == "symbol")        res = #["EcUInt",         "k", #[pvarname], @nil, defval, nullok, datum];
		else if (datum == "EC_OBJ")        res = #["EC_OBJ",         "O", #[pvarname], @nil, defval, nullok, datum];
		else {
			tobj = [toplevelmodule getType: datum];
			if (tobj)
				res = #[[tobj getCType], [tobj getEcCvtString], [tobj getEcCvtParams cprefix, cvarname, nullok], @nil, defval, nullok, tobj];
		}

		if (res == "UNKNOWN")
			basic.print( "WARNING: Unknown type '", datum, "'\n" );
		return res;
	}

	return #[[datum getCType], [datum getEcCvtString], [datum getEcCvtParams cprefix, cvarname, nullok], @nil, defval, nullok, datum];
}

public function cvt_atomic_info( toplevelmodule, module, datum, objvar, cvar, fname, argnum )
{
	/*
	 * Return information needed to convert a single EC_OBJ object to
	 * corresponding C value (outside of EcParseStackFunction).
	 */

	local modprefix = [toplevelmodule getCPrefix];
	local cprefix   = [module getCPrefix];
	local tobj;

	/*
	 * return a list #[cvar, cvartype, cvtcode]
	 */

	if (basic.typeof(datum) == #string)
	{
		local res = "UNKNOWN";
		local code;

		argnum = basic.str(argnum);
		if (datum == "none")
			res = #[cvar, "void", ""];
		else if ((datum == "short")  ||
				 (datum == "ushort") ||
				 (datum == "int")    ||
				 (datum == "uint")   ||
				 (datum == "long")   ||
				 (datum == "ulong")) {
			code =
				"\t\tEcInt\t" + cvar + ";\n"
				"\t\tEC_CHECKTYPE_F(\"" + fname + "\", " + argnum + ", " + objvar + ", tc_inum);\n" +
				"\t\t" + cvar + " = EC_INUM(" + objvar + ");\n";
			res = #[cvar, "EcInt", code];
		}
		else if ((datum == "char") ||
				 (datum == "uchar")) {
			code =
				"\t\tEcChar\t" + cvar + ";\n"
				"\t\tEC_CHECKTYPE_F(\"" + fname + "\", " + argnum + ", " + objvar + ", tc_char);\n" +
				"\t\t" + cvar + " = EC_CHAR(" + objvar + ");\n";
			res = #[cvar, "EcChar", code];
		}
		else if ((datum == "float") ||
				 (datum == "double")) {
			code =
				"\t\tEcFloat\t" + cvar + ";\n"
				"\t\tEC_CHECKTYPE_F(\"" + fname + "\", " + argnum + ", " + objvar + ", tc_fnum);\n" +
				"\t\t" + cvar + " = EC_FNUM(" + objvar + ");\n";
			res = #[cvar, "EcFloat", code];
		}
		else if (datum == "bool") {
			code =
				"\t\tEcBool\t" + cvar + ";\n" +
				"\t\t" + cvar + " = EC_ASBOOLVAL(" + objvar + ");\n";
			res = #[cvar, "EcBool", code];
		}
		else if ((datum == "string") ||
				 (datum == "static_string")) {
			code =
				"\t\tconst char *\t" + cvar + ";\n"
				"\t\tEC_CHECKTYPE_F(\"" + fname + "\", " + argnum + ", " + objvar + ", tc_string);\n" +
				"\t\t" + cvar + " = ec_stringdup(EC_STRDATA(" + objvar + "));\n";
			res = #[cvar, "const char *", code];
		}
		else if (datum == "callback") {
			// unsupported
			res = @nil;
		}
		else if (datum == "symbol") {
			code =
				"\t\tconst char *\t" + cvar + ";\n"
				"\t\tEC_CHECKTYPE_F(\"" + fname + "\", " + argnum + ", " + objvar + ", tc_symbol);\n" +
				"\t\t" + cvar + " = ec_stringdup(EcSymbolAt(EC_SYMBOL(" + objvar + ")));\n";
			res = #[cvar, "const char *", code];
		}
		else if (datum == "EC_OBJ") {
			code =
				"\t\tEC_OBJ\t" + cvar + ";\n" +
				"\t\t" + cvar + " = (" + objvar + ");\n";
			res = #[cvar, "EC_OBJ", code];
		}
		else {
			tobj = [toplevelmodule getType: datum];
			if (tobj)
			{
				code =
					"\t\t" + [tobj getCType] + "\t" + cvar + ";\n" +
					"\t\t" + cvar + " = " + [tobj getEcFromObj modprefix, objvar] + ";\n";
				res = #[cvar, datum + " *", code];
			}
			else res = @nil;
		}

		if (res == "UNKNOWN")
			basic.print( "WARNING: Unknown type '", datum, "'\n" );
		return res;
	}

	local code =
		"\t\t" + [datum getCType] + "\t" + cvar + ";\n" +
		"\t\t" + cvar + " = " + [datum getEcFromObj modprefix, objvar] + ";\n";
	return #[cvar, datum + " *", code];
}

public function buildvalue( toplevelmodule, datum, cvalue, info )
{
	/*
	 * return a C expression returning the elastiC object build from cvalue
	 */

	local cprefix = [toplevelmodule getCPrefix];

	if (basic.typeof(datum) == #string)
	{
		local res = "UNKNOWN";

		if      (datum == "none")          res = "EC_NIL";
		else if (datum == "short")         res = "EcMakeInt( (EcInt)(" + cvalue + ") )";
		else if (datum == "ushort")        res = "EcMakeInt( (EcInt)(" + cvalue + ") )";
		else if (datum == "int")           res = "EcMakeInt( (EcInt)(" + cvalue + ") )";
		else if (datum == "uint")          res = "EcMakeInt( (EcInt)(" + cvalue + ") )";
		else if (datum == "long")          res = "EcMakeInt( (EcInt)(" + cvalue + ") )";
		else if (datum == "ulong")         res = "EcMakeInt( (EcInt)(" + cvalue + ") )";
		else if (datum == "char")          res = "EcMakeChar( (EcChar)(" + cvalue + ") )";
		else if (datum == "uchar")         res = "EcMakeChar( (EcChar)(" + cvalue + ") )";
		else if (datum == "float")         res = "EcMakeFloat( (EcFloat)(" + cvalue + ") )";
		else if (datum == "double")        res = "EcMakeFloat( (EcFloat)(" + cvalue + ") )";
		else if (datum == "bool")          res = "EcMakeBool( (EcBool)(" + cvalue + ") )";
		else if (datum == "string")        res = "EcMakeString( (const char *)(" + cvalue + "), 0 )";
		else if (datum == "static_string") res = "EcMakeString( (const char *)(" + cvalue + "), 0 )";
		else if (datum == "callback")      res = "UNKNOWN"; // unsupported
		else if (datum == "symbol")        res = "EcMakeSymbol( (const char *)(" + cvalue + ") )";
		else if (datum == "EC_OBJ")        res = "(" + cvalue + ")";
		else {
			local tobj = [toplevelmodule getType: datum];
			if (tobj)
				res = [tobj getEcValueBuilder cprefix, cvalue, info];
			else res = @nil;
		}

		return res;
	}

	if (datum)
		return [datum getEcValueBuilder cprefix, cvalue, info];
	else
		return "UNKNOWN";
}

public parsespec_l;
public build;

private topModule = @nil;

public function parsespec( toplevelModule, specfile )
{
	topModule = toplevelModule;

	basic.print( basic.sprintf( "PARSING %r...\n", specfile ) );
	local fh = file.open( specfile );
	parsespec_l( fh );
	file.close( fh );
	basic.print( "DONE.\n" );
}

/*
 * Low-level version of the above.
 * This does the dirty work.
 */
public function parsespec_l( fh )
{
	// get plain representation
	local plaindata = scmparse.parsefile( fh );

	// transform in corresponding object-oriented data structure
	basic.print( basic.sprintf( "BUILDING OO representation...\n" ) );
	build( plaindata );
}

// current target
private currentModule = @nil;

public function build( plaindata )
{
	local el;
	local op;

	for (el in plaindata)
	{
		op = el[0];
		if (op == "import")
		{
			local specfile = string.sub(el[1], 1, -1);
			basic.print( "  Import \"", specfile, "\"\n" );
			local fh = file.open( specfile );
			parsespec_l( fh );
			file.close( fh );
		}
		else if (op == "module")
		{
			// 1. instance a new child Module and attach it to
			//    ToplevelModule
			local modname = string.sub(el[1], 1, -1);
			basic.print( "  Module \"", modname, "\"\n" );
			local module = [topModule createAndAddModule modname, modname];

			// 2. make it the current target
			[topModule setCurrentModule module];
			currentModule = module;
		}
		else if (op == "remove-prefix")
			[currentModule setRemovePrefix string.sub(el[1], 1, -1)];
		else if (op == "define-enum")
			[currentModule addElement [Enum new el]];
		else if (op == "define-flags")
			[currentModule addElement [Flags new el]];
		else if (op == "define-boxed")
			[currentModule addElement [Boxed new el]];
		else if (op == "define-func")
			[currentModule addElement [Function new el]];
		else if (op == "define-object")
			[currentModule addElement [Object new el]];
	}
}
