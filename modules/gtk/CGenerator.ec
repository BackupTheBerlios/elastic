/* ==========================================================================
 * CGenerator.ec
 * ==========================================================================
 * Glue C code generator.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Thu Dec  2 14:40:02 MET 1999
 *
 *   $Id: CGenerator.ec,v 1.1 2002/05/30 12:41:39 panta Exp $
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

package CGenerator;

from   basic  import *;
import string;
import file;
import sys;
import hash;

import Util;
import CodeEmitter;
import Data;

/* ========================================================================
 * TYPE INFORMATION
 * ======================================================================== */

/*
 * Symbolic values
 */

public class SymbolicValuesVisitor extends basic.Object
{
	local toplevelmodule;
	local module;

	method init( tlmod, mod, fh )
	{
		toplevelmodule = tlmod;
		module         = mod;
	}

	method previsit( fh )
	{
		local prefix    = [module getCPrefix];
		file.printf( fh, "#if " + string.toupper( prefix ) + "_DATASTRUCTURES\n" );
		return self;
	}
	method postvisit( fh )
	{
		local prefix    = [module getCPrefix];
		file.printf( fh, "#endif /*" + string.toupper( prefix ) + "_DATASTRUCTURES */\n" );
		file.printf( fh, "\n" );
	}

	method visitSymbolic( obj, fh )
	{
		local modprefix = [toplevelmodule getCPrefix];
		local prefix    = [module getCPrefix];

		local dim = [obj getNumber];
		file.printf( fh,
					 "static %s_enum %s_%s[%d] = {\n",
					 modprefix,
					 prefix, [obj getCName], dim );

		local i, name;
		for (i = 0; i < dim; i++)
		{
			name = [obj indexToName i];
			file.printf( fh,
						 "\t{ %-30s%-30s 0 },\n",
						 "\"" + name + "\",", [obj nameToValue name] + "," );
		}
		file.write( fh, "};\n" );
	}
}

public class EnumValuesVisitor extends SymbolicValuesVisitor
{
	local toplevelmodule;
	local module;

	method init( tlmod, mod, fh )
	{
		[super init tlmod, mod, fh];

		toplevelmodule = tlmod;
		module         = mod;

		file.write( fh,
					"/* ------------------------------------------------------------------------\n"
					" *  Information for enumeration values\n"
					" * ------------------------------------------------------------------------ */\n\n" );
	}

	method visitEnum( obj, fh )
	{
		return [self visitSymbolic obj, fh];
	}
}

public class FlagsValuesVisitor extends SymbolicValuesVisitor
{
	local toplevelmodule;
	local module;

	method init( tlmod, mod, fh )
	{
		[super init tlmod, mod, fh];

		toplevelmodule = tlmod;
		module         = mod;

		file.write( fh,
					"/* ------------------------------------------------------------------------\n"
					" *  Information for flags values\n"
					" * ------------------------------------------------------------------------ */\n\n" );
	}

	method visitFlags( obj, fh )
	{
		return [self visitSymbolic obj, fh];
	}
}

/*
 * Symbolic type information
 */

public class SymbolicTypesVisitor extends basic.Object
{
	local module;
	local num;

	method init( mod, fh )
	{
		module = mod;
		num = 0;
	}

	method getNumber()
	{
		return num;
	}

	method visitSymbolic( obj, fh, index )
	{
		file.printf( fh,
					 "\t{ %-40s%4d, %-42s },\n",
					 "\"" + [obj getName] + "\",", [obj getNumber], [module getCPrefix] + "_" + [obj getCName] );
		[obj setIndex index];
		num++;
	}
}

public class EnumTypesVisitor extends SymbolicTypesVisitor
{
	local toplevelmodule;
	local module;
	local index;

	method init( tlmod, mod, fh )
	{
		[super init mod, fh];

		toplevelmodule = tlmod;
		module         = mod;
		index  = 0;
	}

	method previsit( fh )
	{
		local modprefix = [toplevelmodule getCPrefix];
		local prefix    = [module getCPrefix];

		file.write( fh,
					"/* ------------------------------------------------------------------------\n"
					" *  Information for enumerations as types\n"
					" * ------------------------------------------------------------------------ */\n\n" );
		file.printf( fh, "#if " + string.toupper( prefix ) + "_DATASTRUCTURES\n" );
		file.printf( fh,
					 "%s_enum_info %s_enums[] = {\n", modprefix, prefix );
	}

	method postvisit( fh )
	{
		local prefix = [module getCPrefix];

		file.printf( fh, "};\n\n" );
		file.printf( fh, "EcInt %s_nenums = %d;\n", prefix, [self getNumber] );
		file.printf( fh, "#endif /* " + string.toupper( prefix ) + "_DATASTRUCTURES */\n" );
		file.printf( fh, "\n" );
	}

	method visitEnum( obj, fh )
	{
		return [self visitSymbolic obj, fh, index++];
	}
}

public class FlagsTypesVisitor extends SymbolicTypesVisitor
{
	local toplevelmodule;
	local module;
	local index;

	method init( tlmod, mod, fh )
	{
		[super init mod, fh];

		toplevelmodule = tlmod;
		module         = mod;
		index  = 0;

		file.write( fh,
					"/* ------------------------------------------------------------------------\n"
					" *  Information for flags as types\n"
					" * ------------------------------------------------------------------------ */\n\n" );
	}

	method previsit( fh )
	{
		local modprefix = [toplevelmodule getCPrefix];
		local prefix    = [module getCPrefix];

		file.printf( fh, "#if " + string.toupper( prefix ) + "_DATASTRUCTURES\n" );
		file.printf( fh,
					 "%s_enum_info %s_flags[] = {\n",
					 modprefix, prefix );
	}

	method postvisit( fh )
	{
		local prefix = [module getCPrefix];

		file.printf( fh, "};\n\n" );
		file.printf( fh, "EcInt %s_nflags = %d;\n", prefix, [self getNumber] );
		file.printf( fh, "#endif /* " + string.toupper( prefix ) + "_DATASTRUCTURES */\n" );
		file.printf( fh, "\n" );
	}

	method visitFlags( obj, fh )
	{
		return [self visitSymbolic obj, fh, index++];
	}
}

/*
 * Symbolic information mapping for humans
 */

public class SymbolicDefsVisitor extends basic.Object
{
	local toplevelmodule;
	local module;

	method init( tlmod, mod, fh )
	{
		toplevelmodule = tlmod;
		module         = mod;
	}

	method previsit( fh )
	{
		file.write( fh,
					"/* ------------------------------------------------------------------------\n"
					" *  Symbolic information mapping for humans\n"
					" * ------------------------------------------------------------------------ */\n\n" );
	}

	method postvisit( fh )
	{
		file.printf( fh, "\n" );
	}

	method visitSymbolic( obj, fh )
	{
		local typename;

		local prefix = [module getCPrefix];
		typename = prefix + ([obj getType] == "enum" ? "_enums" : "_flags");

		local infostruct = prefix + "_t" + Util.canonicalize_name( [obj getName], @true );
		file.printf( fh,
					 "#define %-50s (%s[%d])\n",
					 infostruct, typename, [obj getIndex] );
		[obj setInfoStruct: infostruct];
	}

	method visitEnum( obj, fh )
	{
		return [self visitSymbolic obj, fh];
	}

	method visitFlags( obj, fh )
	{
		return [self visitSymbolic obj, fh];
	}
}

/*
 * Boxed types
 */

public class BoxedInfoVisitor extends basic.Object
{
	local toplevelmodule;
	local module;
	local num;
	local index;

	method init( tlmod, mod, fh )
	{
		toplevelmodule = tlmod;
		module         = mod;
		num    = 0;
		index  = 0;
	}

	method previsit( fh )
	{
		local modprefix = [toplevelmodule getCPrefix];
		local prefix    = [module getCPrefix];

		file.write( fh,
					"/* ------------------------------------------------------------------------\n"
					" *  Boxed types information\n"
					" * ------------------------------------------------------------------------ */\n\n" );
		file.printf( fh, "#if " + string.toupper( prefix ) + "_DATASTRUCTURES\n" );
		file.printf( fh,
					 "%s_boxed_info %s_boxed[] = {\n",
					 modprefix, prefix );
	}

	method postvisit( fh )
	{
		local prefix = [module getCPrefix];

		file.printf( fh, "};\n\n" );
		file.printf( fh, "EcInt %s_nboxed = %d;\n", prefix, num );
		file.printf( fh, "#endif /* " + string.toupper( prefix ) + "_DATASTRUCTURES */\n" );
		file.printf( fh, "\n" );
	}

	method visitBoxed( obj, fh )
	{
		local modprefix = [toplevelmodule getCPrefix];
		local prefix    = [module getCPrefix];

		file.printf( fh,
					 "\t{ %-20s%-40s%-40s%-20s },\n",
					 "\"" + [obj getName] + "\",",
					 "(" + string.toupper(modprefix) + "_BOXED_COPY)"    + [obj getCopyFunc] + ",",
					 "(" + string.toupper(modprefix) + "_BOXED_DESTROY)" + [obj getFreeFunc] + ",",
					 [obj getSize] );
		[obj setInfoStruct: prefix + "_boxed[" + basic.str(index) + "]"];
		[obj setIndex index++];
		num++;
	}
}

/*
 * Object types
 */

public class ObjectInfoVisitor extends basic.Object
{
	local toplevelmodule;
	local module;

	method init( tlmod, mod, fh )
	{
		toplevelmodule = tlmod;
		module         = mod;
	}

	method previsit( fh )
	{
		return @nil;
	}

	method postvisit( fh )
	{
		return @nil;
	}

	method visitObject( obj, fh )
	{
		local prefix = [module getCPrefix];

		// TODO: gtk dependant ! Remove dependency.
		[obj setInfoStruct: "gtk_object_get_type()"];
	}
}

/*
 * Boxed/Object fields symbol defines
 */

private function field2CPP( prefix, symname )
{
	return string.toupper( prefix + "_COMPOUNDFIELD_" + symname );
}

public class BoxedObjectSymDefsVisitor extends basic.Object
{
	local toplevelmodule;
	local module;

	// for mapping Boxed fields to preprocessor symbols
	local field2cpp;
	local nfields;

	method init( tlmod, mod, fh )
	{
		toplevelmodule = tlmod;
		module         = mod;
		field2cpp      = [toplevelmodule getField2cpp];
		nfields        = 0;
	}

	method previsit( fh )
	{
		local prefix = [module getCPrefix];

		file.write( fh,
					"/* ------------------------------------------------------------------------\n"
					" *  Boxed & object field mapping\n"
					" * ------------------------------------------------------------------------ */\n\n" );
		file.printf( fh, "#if " + string.toupper( prefix ) + "_MODULE\n" );
	}

	method postvisit( fh )
	{
		local prefix = [module getCPrefix];
		file.printf( fh,
					 "\nstatic EcUInt %s_compoundfield[%d];\n",
					 [module getCPrefix], nfields );
		file.printf( fh, "#endif /* " + string.toupper( prefix ) + "_MODULE */\n" );
		file.printf( fh, "\n\n" );
	}

	method visitBoxed( obj, fh )
	{
		return [self visitBoxedObject obj, fh];
	}

	method visitObject( obj, fh )
	{
		return [self visitBoxedObject obj, fh];
	}

	method visitBoxedObject( obj, fh )
	{
		local prefix = [module getCPrefix];

		local field, fields = [obj getFields];
		for (field in fields)
		{
			if (! hash.has_key(field2cpp, field))
			{
				field2cpp[field] = field2CPP( prefix, field );
				file.printf( fh,
							 "#define %-30s %d\n",
							 field2cpp[field], nfields );
				nfields++;
			}
		}
	}
}

/*
 * Symbolic initialization
 */

public class SymbolicInitVisitor extends basic.Object
{
	local toplevelmodule;
	local module;

	// for mapping Boxed fields to symbols
	local field2cpp;

	method init( tlmod, mod, fh )
	{
		toplevelmodule = tlmod;
		module         = mod;
		field2cpp      = [toplevelmodule getField2cpp];
	}

	method previsit( fh )
	{
		local prefix = [module getCPrefix];

		file.write( fh,
					"/* ------------------------------------------------------------------------\n"
					" *  Init. function for enumerations/flags symbols & boxed/object fields\n"
					" * ------------------------------------------------------------------------ */\n\n" );
		file.printf( fh, "#if " + string.toupper( prefix ) + "_MODULE\n" );
		file.printf( fh,
					 "static void %s_init_symbols( void )\n"
					 "{\n",
					 prefix );
	}

	method postvisit( fh )
	{
		local prefix = [module getCPrefix];

		file.printf( fh, "\n\t/* Boxed/Object fields */\n\n" );
		local field, fields = hash.keys(field2cpp);
		for (field in fields)
		{
			file.printf( fh,
						 "\t%-50s = EcInternSymbol( \"%s\" );\n",
						 prefix + "_compoundfield[" + field2cpp[field] + "]",
						 field );
		}

		file.printf( fh, "}\n" );
		file.printf( fh, "#endif /* " + string.toupper( prefix ) + "_MODULE */\n" );
		file.printf( fh, "\n" );
	}

	method visitSymbolic( obj, fh )
	{
		local cname;

		local prefix = [module getCPrefix];

		file.printf( fh,
					 "\t/* %-30s */\n",
					 [obj getName] );
		cname = [obj getCName];
		local dim, i, name;
		dim = [obj getNumber];
		for (i = 0; i < dim; i++)
		{
			name = [obj indexToName i];
			file.printf( fh,
						 "\t%-50s = EcInternSymbol( \"%s\" );\n",
						 basic.sprintf( "%s_%s[%d].symid", prefix, cname, i ),
						 name );
		}
		file.printf( fh, "\n" );
	}

	method visitEnum( obj, fh )
	{
		return [self visitSymbolic obj, fh];
	}

	method visitFlags( obj, fh )
	{
		return [self visitSymbolic obj, fh];
	}
}

/*
 * Type mapper
 *
 * Construct type mapping (C type name -> Data object) for ToplevelModule
 */

public class TypeMapperVisitor extends basic.Object
{
	local toplevelmodule;

	method init( tlmod )
	{
		toplevelmodule = tlmod;
	}

	method visitType( obj )
	{
		[toplevelmodule setType: [obj getName] def: obj];
	}

	method visitEnum( obj )
	{
		return [self visitType obj];
	}

	method visitFlags( obj )
	{
		return [self visitType obj];
	}

	method visitBoxed( obj )
	{
		return [self visitType obj];
	}

	method visitObject( obj )
	{
		return [self visitType obj];
	}
}

/* ========================================================================
 * GLUE CODE
 * ======================================================================== */

/*
 * Getters & setters for Boxed fields
 */

public class BoxedGlueVisitor extends basic.Object
{
	local toplevelmodule;
	local module;

	method init( tlmod, mod, fh )
	{
		toplevelmodule = tlmod;
		module         = mod;
	}

	method previsit( fh )
	{
		local prefix = [module getCPrefix];

		file.write( fh,
					"/* ------------------------------------------------------------------------\n"
					" *  Getters & setters for Boxed types\n"
					" * ------------------------------------------------------------------------ */\n\n" );
	}

	method postvisit( fh )
	{
		file.printf( fh, "\n" );
	}

	method visitBoxed( obj, fh )
	{
		local fields = [obj getFields];
		if ((! fields) || (basic.length(fields) == 0)) return;

		local prefix = [module getCPrefix];

		local removePrefix = [module getRemovePrefix];
		local fname = Util.canonicalize_name( [obj getName], @false );
		if (removePrefix != "")
		{
			local plen = string.length(removePrefix);
			if (string.tolower(string.sub(fname, 0, plen)) == string.tolower(removePrefix))
				fname = string.sub(fname, plen);
		}

		local ec_getter_name = fname + "_getf";
		local ec_setter_name = fname + "_setf";
		local ec_prefixed_getter_name = [module getPrefix] + "." + ec_getter_name;
		local ec_prefixed_setter_name = [module getPrefix] + "." + ec_setter_name;
		local getter_name = prefix + "_" + Util.canonicalize_name( [obj getName], @false ) + "_getf";
		local setter_name = prefix + "_" + Util.canonicalize_name( [obj getName], @false ) + "_setf";
		[obj setEcGetter ec_prefixed_getter_name];
		[obj setEcSetter ec_prefixed_setter_name];
		[obj setCGetter getter_name];
		[obj setCSetter setter_name];

		// getter
		local cinfo;
		local femitter = [CodeEmitter.ElasticPrimitiveFunction new fh, getter_name, ec_getter_name, "", [module getPrefix], @true];
		cinfo = Data.cvt_info(toplevelmodule, module, obj,      "c_obj", @nil, @false); [femitter addArg: "c_obj" info: cinfo];
		cinfo = Data.cvt_info(toplevelmodule, module, "symbol", "c_sym", @nil, @false); [femitter addArg: "c_sym" info: cinfo];
		local field, fieldType, cppname, cvar;
		local buildvalue;
		local line, first = 1;
		for (field in fields)
		{
			fieldType  = [obj getFieldType field];
			buildvalue = Data.buildvalue( toplevelmodule, fieldType, "c_obj->" + field, @nil );

			cppname = field2CPP( prefix, field );
			cvar = prefix + "_compoundfield[" + cppname + "]";
			if (first)
				line = "if";
			else
				line = "else if";
			line = line + " (c_sym == " + cvar + ")\n"
				"\t\tres = " + buildvalue + ";";
			[femitter addBody: line];
			first = 0;
		}
		[femitter addReturnValue: "res"];
		[femitter emit];

		// setter
		local cvtinfo;
		femitter = [CodeEmitter.ElasticPrimitiveFunction new fh, setter_name, ec_setter_name, "", [module getPrefix], @true];
		cinfo = Data.cvt_info(toplevelmodule, module, obj,      "c_obj", @nil, @false); [femitter addArg: "c_obj" info: cinfo];
		cinfo = Data.cvt_info(toplevelmodule, module, "symbol", "c_sym", @nil, @false); [femitter addArg: "c_sym" info: cinfo];
		cinfo = Data.cvt_info(toplevelmodule, module, "EC_OBJ", "c_val", @nil, @false); [femitter addArg: "c_val" info: cinfo];
		first = 1;
		for (field in fields)
		{
			fieldType  = [obj getFieldType field];

			cppname = field2CPP( prefix, field );
			cvar = prefix + "_compoundfield[" + cppname + "]";
			if (first)
				line = "if";
			else
				line = "else if";
			line = line + " (c_sym == " + cvar + ") {\n";
			cvtinfo = Data.cvt_atomic_info(toplevelmodule, module, fieldType, "c_val", "c_" + field, [module getPrefix] + "." + ec_setter_name, 3);
			line = line + cvtinfo[2] +
				"\t\tc_obj->" + field + " = c_" + field + ";\n" +
				"\t}";
			[femitter addBody: line];
			first = 0;
		}
		[femitter addReturnValue: "EcTrueObject"];
		[femitter emit];
	}
}

/*
 * Getters & setters for Object fields
 */

public class ObjectGlueVisitor extends basic.Object
{
	local toplevelmodule;
	local module;

	method init( tlmod, mod, fh )
	{
		toplevelmodule = tlmod;
		module         = mod;
	}

	method previsit( fh )
	{
		local prefix = [module getCPrefix];

		file.write( fh,
					"/* ------------------------------------------------------------------------\n"
					" *  Getters & setters for Object types\n"
					" * ------------------------------------------------------------------------ */\n\n" );
	}

	method postvisit( fh )
	{
		file.printf( fh, "\n" );
	}

	method visitObject( obj, fh )
	{
		local fields = [obj getFields];
		if ((! fields) || (basic.length(fields) == 0)) return;

		local prefix = [module getCPrefix];

		local removePrefix = [module getRemovePrefix];
		local fname = Util.canonicalize_name( [obj getName], @false );
		if (removePrefix != "")
		{
			local plen = string.length(removePrefix);
			if (string.tolower(string.sub(fname, 0, plen)) == string.tolower(removePrefix))
				fname = string.sub(fname, plen);
		}

		local ec_getter_name = fname + "_getf";
		local ec_setter_name = fname + "_setf";
		local ec_prefixed_getter_name = [module getPrefix] + "." + ec_getter_name;
		local ec_prefixed_setter_name = [module getPrefix] + "." + ec_setter_name;
		local getter_name = prefix + "_" + Util.canonicalize_name( [obj getName], @false ) + "_getf";
		local setter_name = prefix + "_" + Util.canonicalize_name( [obj getName], @false ) + "_setf";
		[obj setEcGetter ec_prefixed_getter_name];
		[obj setEcSetter ec_prefixed_setter_name];
		[obj setCGetter getter_name];
		[obj setCSetter setter_name];

		// getter
		local cinfo;
		local femitter = [CodeEmitter.ElasticPrimitiveFunction new fh, getter_name, ec_getter_name, "", [module getPrefix], @true];
		cinfo = Data.cvt_info(toplevelmodule, module, obj,      "c_obj", @nil, @false); [femitter addArg: "c_obj" info: cinfo];
		cinfo = Data.cvt_info(toplevelmodule, module, "symbol", "c_sym", @nil, @false); [femitter addArg: "c_sym" info: cinfo];
		local field, fieldType, cppname, cvar;
		local buildvalue;
		local line, first = 1;
		for (field in fields)
		{
			fieldType  = [obj getFieldType field];
			buildvalue = Data.buildvalue( toplevelmodule, fieldType, "c_obj->" + field, @nil );

			cppname = field2CPP( prefix, field );
			cvar = prefix + "_compoundfield[" + cppname + "]";
			if (first)
				line = "if";
			else
				line = "else if";
			line = line + " (c_sym == " + cvar + ")\n"
				"\t\tres = " + buildvalue + ";";
			[femitter addBody: line];
			first = 0;
		}
		[femitter addReturnValue: "res"];
		[femitter emit];

		// setter
		local cvtinfo;
		femitter = [CodeEmitter.ElasticPrimitiveFunction new fh, setter_name, ec_setter_name, "", [module getPrefix], @true];
		cinfo = Data.cvt_info(toplevelmodule, module, obj,      "c_obj", @nil, @false); [femitter addArg: "c_obj" info: cinfo];
		cinfo = Data.cvt_info(toplevelmodule, module, "symbol", "c_sym", @nil, @false); [femitter addArg: "c_sym" info: cinfo];
		cinfo = Data.cvt_info(toplevelmodule, module, "EC_OBJ", "c_val", @nil, @false); [femitter addArg: "c_val" info: cinfo];
		first = 1;
		for (field in fields)
		{
			fieldType  = [obj getFieldType field];

			cppname = field2CPP( prefix, field );
			cvar = prefix + "_compoundfield[" + cppname + "]";
			if (first)
				line = "if";
			else
				line = "else if";
			line = line + " (c_sym == " + cvar + ") {\n";
			cvtinfo = Data.cvt_atomic_info(toplevelmodule, module, fieldType, "c_val", "c_" + field, [module getPrefix] + "." + ec_setter_name, 3);
			line = line + cvtinfo[2] +
				"\t\tc_obj->" + field + " = c_" + field + ";\n" +
				"\t}";
			[femitter addBody: line];
			first = 0;
		}
		[femitter addReturnValue: "EcTrueObject"];
		[femitter emit];
	}
}

/*
 * Function marshallers
 */

public class FunctionGlueVisitor extends basic.Object
{
	local toplevelmodule;
	local module;

	method init( tlmod, mod, fh )
	{
		toplevelmodule = tlmod;
		module         = mod;
	}

	method previsit( fh )
	{
		local prefix = [module getCPrefix];

		file.write( fh,
					"/* ------------------------------------------------------------------------\n"
					" *  Functions\n"
					" * ------------------------------------------------------------------------ */\n\n" );
	}

	method postvisit( fh )
	{
		file.printf( fh, "\n" );
	}

	method visitFunction( obj, fh )
	{
		local prefix = [module getCPrefix];

		local removePrefix = [module getRemovePrefix];
		local fname = Util.canonicalize_name( [obj getName], @false );
		local ec_function_name;
		if (removePrefix != "")
		{
			local plen = string.length(removePrefix);
			if (string.tolower(string.sub(fname, 0, plen)) == string.tolower(removePrefix))
				fname = string.sub(fname, plen);
		}
		ec_function_name = fname;
		local ec_prefixed_function_name = [module getPrefix] + "." + ec_function_name;
		local function_name = prefix + "_" + Util.canonicalize_name( [obj getName], @false );
		[obj setEcFunctionName: ec_prefixed_function_name];
		[obj setCFunctionName: function_name];

		local cinfo, cvtinfo;
		local params, param, ptype, pdef, pnullok;
		local pcname;
		local femitter = [CodeEmitter.ElasticPrimitiveFunction new fh, function_name, ec_function_name, [obj getReturnType], [module getPrefix], @true];
		params = [obj getParameters];
		for (param in params)
		{
			ptype   = [obj getParameterType    param];
			pdef    = [obj getParameterDefault param];
			pnullok = [obj getParameterNullOk  param];
			pcname  = "c_" + param;
			cinfo = Data.cvt_info(toplevelmodule, module, ptype, pcname, pdef, pnullok);
			[femitter addArg: pcname info: cinfo];
		}

		local fargs = "";
		for (param in params)
		{
			ptype = [obj getParameterType    param];

			if (fargs != "")
				fargs = fargs + ", ";
			pcname  = "c_" + param;
			if (ptype != "callback")
				fargs = fargs + pcname;
			else
				fargs = fargs +
					"NULL, " +
					[module getCPrefix] + "_callback_marshal, " +
					"(gpointer)" + pcname + ", " +
					[module getCPrefix] + "_callback_destroy";
		}

		local fcall = [obj getName] + "( " + fargs + " )";

		local buildvalue;
		local line;
		local rtypename = [obj getReturnType], rtype;
		rtype = [toplevelmodule getType: rtypename];
		if (rtypename && (rtype == @nil))
			rtype = rtypename;
		if (rtype) {
			buildvalue = Data.buildvalue( toplevelmodule, rtype, fcall, @nil );
			line = "res = " + buildvalue + ";";
			[femitter addBody: line];
			[femitter addReturnValue: "res"];
		}
		else {
			line = fcall + ";";
			[femitter addBody: line];
			[femitter addReturnValue: "EC_NIL"];
		}

		[femitter emit];
	}
}

/*
 * Initialization function
 */

public class InitFunctionVisitor extends basic.Object
{
	local module;

	method init( mod, fh )
	{
		module = mod;
	}

	method previsit( fh )
	{
		local prefix = [module getCPrefix];

		file.write( fh,
					"/* ------------------------------------------------------------------------\n"
					" *  Intialization function\n"
					" * ------------------------------------------------------------------------ */\n\n" );
		file.printf( fh,
					 "static void %s_register_functions( void )\n"
					 "{\n",
					 prefix );
	}

	method postvisit( fh )
	{
		file.printf( fh, "}\n\n" );
		file.printf( fh, "\n" );
	}

	method visitBoxed( obj, fh )
	{
		// Emit getters & setters registration

		local prefix = [module getCPrefix];
		if (basic.length([obj getFields]) > 0)
		{
			file.printf( fh,
						 "\tEcAddPrimitive( %-50s%-50s );\n",
						 "\"" + [obj getEcGetter] + "\",", [obj getCGetter] );
			file.printf( fh,
						 "\tEcAddPrimitive( %-50s%-50s );\n",
						 "\"" + [obj getEcSetter] + "\",", [obj getCSetter] );
		}
	}

	method visitObject( obj, fh )
	{
		// Emit getters & setters registration

		local prefix = [module getCPrefix];
		if (basic.length([obj getFields]) > 0)
		{
			file.printf( fh,
						 "\tEcAddPrimitive( %-50s%-50s );\n",
						 "\"" + [obj getEcGetter] + "\",", [obj getCGetter] );
			file.printf( fh,
						 "\tEcAddPrimitive( %-50s%-50s );\n",
						 "\"" + [obj getEcSetter] + "\",", [obj getCSetter] );
		}
	}

	method visitFunction( obj, fh )
	{
		// Emit function registration

		local prefix = [module getCPrefix];
		file.printf( fh,
					 "\tEcAddPrimitive( %-50s%-50s );\n",
					 "\"" + [obj getEcFunctionName] + "\",", [obj getCFunctionName] );
	}
}
