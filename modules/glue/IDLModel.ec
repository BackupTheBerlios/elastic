/* ==========================================================================
 * IDLModel.ec
 * ==========================================================================
 * OO data model for elastiC C Glue generator.
 * This module also provides IDL -> OO conversion.
 * The IDL is parsed by the scmparse module.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created : Sat Nov 27 18:22:35 MET 1999
 *   Modified: Mon Apr 2 2001
 *
 *   $Id: IDLModel.ec,v 1.1 2002/05/31 15:14:14 panta Exp $
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
 * Object Oriented nodes used to represent module contents,
 * specified originally by the Scheme IDL file.
 */

package IDLModel;

import basic;
import string;
import array;
import hash;

import util;

/*
 * Visitable class
 *
 * Implements the visitor part in the visitor pattern.
 * For a detailed explanation of the visitor pattern, see GoF, p.331:
 * 
 *    "Design Patterns" - Elements of Reusable Object-Oriented Software
 *    E. Gamma, R. Helm, R. Johnson, J. Vlissides
 *    Addison Wesley
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
				newargs[basic.length(newargs)] = arg;			// TODO: use array.push()
			return basic.sendwith( visitor, methodsym, newargs );
		}
		return @nil;
	}

	method visit: visitor
	{
		return [self visit: visitor with: @nil];
	}
}

// ================================================================================
// O O    N O D E S
//
// All the nodes support the `Visitable' interface.
// They are visited by the code generating visitors contained in the `IDLGlue'
// module.
//
// Nodes:
//   IDLModule          - implements the IDL `module' construct
//   IDLCInclude        - implements the IDL `c-include' construct
//   IDLCLiteral        - implements the IDL `c-literal' construct
//   IDLInclude         - implements the IDL `include' construct
//   IDLImport          - implements the IDL `import' construct
//   IDLCAtomicType     - implements the IDL `c-atomic-type' construct
//   IDLSymbolic
//      IDLCEnum        - implements C enumerations
//      IDLCFlags       - implements C flags (OR-able flags, #defines, ...)
//   IDLStruct          - implements C structs
//   IDLFunction        - implements C functions
// --------------------------------------------------------------------------------

/*
 * IDLModule class
 *
 * Implements the IDL `module' construct
 */
public class IDLModule extends Visitable
{
	/* ------------------------------------------------------------------------
	 * INSTANCE DATA
	 * ------------------------------------------------------------------------ */

	local name;									// name of the module
	local c_prefix;								// C prefix to use for generated names
	local c_removeprefix;						// prefix to remove from described C functions
	local tree;									// array of Visitable IDL* instances
												// that constitute this module

	/* ------------------------------------------------------------------------
	 * METHODS
	 * ------------------------------------------------------------------------ */

	// CONSTRUCTOR

	method init( desc )
	{
		name = desc[1];
		if (basic.length(desc) >= 3)
			c_prefix = desc[2];
		else
			c_prefix = "";
		if (basic.length(desc) >= 4)
			c_removeprefix = desc[3];
		else
			c_removeprefix = "";
		tree = #[];
	}

	// REPRESENTATION

	method _print( detailed )
	{
		return basic.sprintf( "<IDLModule %r>", [self getName] );
	}

	method getName()
	{
		return name;
	}

	method getCPrefix()
	{
		return c_prefix;
	}

	method getCRemovePrefix()
	{
		return c_removeprefix;
	}

	// Node handling

	method addNode(node)
	{
		array.push( tree, node );
	}

	method getTree()
	{
		return tree;
	}

	method visitTree( visitor, visit_args )
		// visit the node tree emitting the glue code
		// (called by the IDLModel visitor visiting method)
	{
		local node;

		for (node in tree)
			[node visit: visitor with: visit_args];
	}
}

/*
 * IDLCInclude class
 *
 * Implements the IDL `c-include' construct
 */
public class IDLCInclude extends Visitable
{
	/* ------------------------------------------------------------------------
	 * INSTANCE DATA
	 * ------------------------------------------------------------------------ */

	local c_header;

	/* ------------------------------------------------------------------------
	 * METHODS
	 * ------------------------------------------------------------------------ */

	// CONSTRUCTOR

	method init( desc )
	{
		c_header = desc[1];
	}

	// REPRESENTATION

	method _print( detailed )
	{
		return basic.sprintf( "<IDLCInclude %r>", [self getCHeader] );
	}

	method getCHeader()
	{
		return c_header;
	}
}

/*
 * IDLCLiteral class
 *
 * Implements the IDL `c-literal' construct
 */
public class IDLCLiteral extends Visitable
{
	/* ------------------------------------------------------------------------
	 * INSTANCE DATA
	 * ------------------------------------------------------------------------ */

	local text;

	/* ------------------------------------------------------------------------
	 * METHODS
	 * ------------------------------------------------------------------------ */

	// CONSTRUCTOR

	method init( desc )
	{
		text = desc[1];
	}

	// REPRESENTATION

	method _print( detailed )
	{
		return basic.sprintf( "<IDLCLiteral>" );
	}

	method getText()
	{
		return text;
	}
}

/*
 * IDLInclude class
 *
 * Implements the IDL `include' construct
 */
public class IDLInclude extends Visitable
{
	/* ------------------------------------------------------------------------
	 * INSTANCE DATA
	 * ------------------------------------------------------------------------ */

	local filename;

	/* ------------------------------------------------------------------------
	 * METHODS
	 * ------------------------------------------------------------------------ */

	// CONSTRUCTOR

	method init( desc )
	{
		filename = desc[1];
	}

	// REPRESENTATION

	method _print( detailed )
	{
		return basic.sprintf( "<IDLInclude %r>", [self getFilename] );
	}

	method getFilename()
	{
		return filename;
	}
}

/*
 * IDLCAtomicType class
 *
 * Implements the IDL `c-atomic-type' construct
 */
public class IDLCAtomicType extends Visitable
{
	/* ------------------------------------------------------------------------
	 * INSTANCE DATA
	 * ------------------------------------------------------------------------ */

	local c_type;

	/* ------------------------------------------------------------------------
	 * METHODS
	 * ------------------------------------------------------------------------ */

	// CONSTRUCTOR

	method init( desc )
	{
		c_type = desc[1];
	}

	// REPRESENTATION

	method _print( detailed )
	{
		return basic.sprintf( "<IDLCAtomicType %r>", [self getCType] );
	}

	method getCType()
	{
		return c_type;
	}
}

/*
 * IDLSymbolic class  (abstract)
 *
 * Implements a symbol collection, acting as abstract base class
 * for IDLCEnum and IDLCFlags classes.
 */
public class IDLSymbolic extends Visitable
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
		return "enum" + util.canonicalize_name( name, @true );
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
 * IDLCEnum class
 *
 * Implements a enum object, representing a C 'enum'.
 */
public class IDLCEnum extends IDLSymbolic
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

	method getEcCvtParams( cprefix, cvarname )
	{
		return #[[self getEcCvtFunc], "&" + cvarname, "(EcDWord)&" + [self getInfoStruct], "FALSE"];
	}

	method getEcValueBuilder( cprefix, cvalue, info )
	{
		return cprefix + "_enum2obj( (long)" + cvalue + ", &" + [self getInfoStruct] + " )";
	}
}

/*
 * IDLCFlags class
 *
 * Implements a flags object, representing a C OR-able 'enum' or #define's.
 */
public class IDLCFlags extends IDLSymbolic
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

	method getEcCvtParams( cprefix, cvarname )
	{
		return #[[self getEcCvtFunc], "&" + cvarname, "(EcDWord)&" + [self getInfoStruct], "FALSE"];
	}

	method getEcValueBuilder( cprefix, cvalue, info )
	{
		return cprefix + "_flags2obj( (long)" + cvalue + ", &" + [self getInfoStruct] + " )";
	}
}

/*
 * IDLCStruct class
 *
 * Implements the IDL `c-struct' construct, representing a C 'struct'.
 */
public class IDLCStruct extends Visitable
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
		return basic.sprintf( "<IDLCStruct %r %r>", name, fields );
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
		return "struct_cvt";
	}

	method getEcCvtNParams()
	{
		return "1";
	}

	method getEcCvtParams( cprefix, cvarname )
	{
		return #[[self getEcCvtFunc], "&" + cvarname, "(EcDWord)&" + [self getInfoStruct], "FALSE"];
	}

	method getEcValueBuilder( cprefix, cvalue, info )
	{
		return cprefix + "_struct2obj( (gpointer)" + cvalue + ", &" + [self getInfoStruct] + ", TRUE )";
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
 * IDLCFunction class
 *
 * Implements a function object, representing a C function.
 */
public class IDLCFunction extends Visitable
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

	method getEcCvtParams( cprefix, cvarname )
	{
		return #[[self getEcCvtFunc], "&" + cvarname, "(EcDWord)" + [self getInfoStruct], "FALSE"];
	}

	method getEcValueBuilder( cprefix, cvalue, info )
	{
		return cprefix + "_object2obj( (GtkObject *)" + cvalue + " )";
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

public function cvt_info( module, datum, cvarname, defval, nullok )
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

	local cprefix = [module getCPrefix];
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
			tobj = [module getType: datum];
			if (tobj)
				res = #[[tobj getCType], [tobj getEcCvtString], [tobj getEcCvtParams cprefix, cvarname], @nil, defval, nullok, tobj];
		}

		if (res == "UNKNOWN")
			basic.print( "WARNING: Unknown type '", datum, "'\n" );
		return res;
	}

	return #[[datum getCType], [datum getEcCvtString], [datum getEcCvtParams cprefix, cvarname], @nil, defval, nullok, datum];
}

public function cvt_atomic_info( module, datum, objvar, cvar, fname, argnum )
{
	/*
	 * Return information needed to convert a single EC_OBJ object to
	 * corresponding C value (outside of EcParseStackFunction).
	 */

	local cprefix = [module getCPrefix];
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
				"\t\tEC_CHECKTYPE_F(\"" + fname + "\", " + argnum + ", " + objvar + ", tc_float);\n" +
				"\t\t" + cvar + " = EC_FLOAT(" + objvar + ");\n";
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
			// NOT IMPLEMENTED (Not useful right now)
			tobj = [module getType: datum];
			return @nil;
		}

		if (res == "UNKNOWN")
			basic.print( "WARNING: Unknown type '", datum, "'\n" );
		return res;
	}

	// NOT IMPLEMENTED (Not useful right now)
	// return #[[datum getCType], [datum getEcCvtString], [datum getEcCvtParams cprefix, cvar], datum];
	return @nil;
}

public function buildvalue( module, datum, cvalue, info )
{
	/*
	 * return a C expression returning the elastiC object build from cvalue
	 */

	local cprefix = [module getCPrefix];

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

		return res;
	}

	if (datum)
		return [datum getEcValueBuilder cprefix, cvalue, info];
	else
		return "UNKNOWN";
}

/* ========================================================================
 * Parsed IDL object to OO IDL object
 * ------------------------------------------------------------------------ */

private idl_handler = %[];

private function idl_handler_register( idl_construct, idl_visitable )
{
	idl_handler[idl_construct] = idl_visitable;
}

private function idl_handler_get( idl_construct )
{
	local op = idl_construct[0];

	if (hash.has_key( idl_handler, op ))
		return idl_handler[op];
	else
		return @nil;
}

private function idl_process( idl_construct )
{
	local op = idl_construct[0];
	local handler;

	if (hash.has_key( idl_handler, op ))
	{
		handler = idl_handler[op];
		if (basic.typeof(handler) == #object)
			return [handler new idl_construct];
		else
			return handler(idl_construct);
	} else
		return @nil;
}

public function idl_process_spec( idl_spec )
{
	/*
	 * Traverses IDL parsed data (after processing by `scmparse' module)
	 * producing a list of IDLModule's.
	 * Each IDLModule contain a list of all its OO IDL nodes.
	 */

	local idl_construct;
	local node;
	local current_module = @nil;
	local module_list = #[];

	basic.print( "Building OO representation.\n" );
	for (idl_construct in idl_spec)
	{
		node = idl_process(idl_construct);
		if ([node isA: IDLModule])
		{
			current_module = node;
			array.push( module_list, current_module );
		}
		else if (current_module)
			[current_module addNode node];
		else
		{
			basic.print( "WARNING: node outside a module: '", node, "'\n" );
			array.push( module_list, node );
		}
	}
	basic.print( "FINISHED\n" );

	return module_list;
}

// module initialization

private function init()
{
	idl_handler_register("module",        IDLModule);			// introduce a module
	idl_handler_register("c-include",     IDLCInclude);			// generate an #include directive
	idl_handler_register("c-literal",     IDLCLiteral);			// include text in the generated code
	idl_handler_register("include",       IDLInclude);			// include a file in the generated code
	idl_handler_register("import",        IDLImport);			// process another idl file
	idl_handler_register("c-atomic-type", IDLCAtomicType);		// declare an atomic C type
	idl_handler_register("c-enum",        IDLCEnum);			// declare a C enumeration (enum)
	idl_handler_register("c-flags",       IDLCFlags);			// declare a C flag set (enum, or #define's)
	idl_handler_register("c-struct",      IDLCStruct);			// declare a C structure (struct)
	idl_handler_register("c-function",    IDLCFunction);		// declare a C function
}

init();
