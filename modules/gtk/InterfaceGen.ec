/* ==========================================================================
 * InterfaceGen.ec
 * ==========================================================================
 * C Interface Generator module for elastiC.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Fri Nov 26 15:33:58 MET 1999
 *
 *   $Id: InterfaceGen.ec,v 1.1 2002/05/30 12:41:45 panta Exp $
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
 * Main application
 */

package InterfaceGen;

from   basic  import *;
import string;
import file;
import hash;
import array;
import sys;

import Data;											// Abstraction layer
import CGenerator;										// Glue code generator

private function visitlist(defdata, visitor, fh)
{
	local el;

	[visitor previsit fh];
	for (el in defdata)
		[el visit: visitor with: fh];
	[visitor postvisit fh];
}


/*
 * Module class
 *
 * Implements the actual elastiC C module (i.e. generates the two C
 * source files implementing the elastiC module).
 */

public class ModuleInstance extends Object
{
	// Module name
	local name;													// string

	// Module prefix
	local prefix;												// string

	// Module definition (parsed and converted to OO form from .def)
	local defdata;												// array of objects

	// Parent (toplevel) module
	local toplevel;												// ToplevelModule

	// Prefix to remove (if any)
	local removePrefix;											// string

	/* ------------------------------------------------------------------------
	 * METHODS
	 * ------------------------------------------------------------------------ */

	// CONSTRUCTOR

	method init(moduleName, modulePrefix, toplev)
	{
		name      = moduleName;
		prefix    = modulePrefix;
		defdata   = #[];
		toplevel  = toplev;
		removePrefix = "";
	}

	// GETTERS & SETTERS

	method getPrefix()
	{
		return prefix;
	}

	method getCPrefix()
	{
		return "ec_" + prefix;
	}

	method getRemovePrefix()
	{
		return removePrefix;
	}

	method setRemovePrefix(rp)
	{
		removePrefix = rp;
	}

	// DATA MANIPULATION

	method addElement(object)
	{
		array.push( defdata, object );
	}

	// MODULE GENERATION

	method _emit()
	{
		// build type mapping using visitor pattern
		local visitor = [CGenerator.TypeMapperVisitor new toplevel];
		local el;
		for (el in defdata)
			[el visit: visitor];

		// create output files
		local file_c = name + "glue.c";
		local file_h = name + "glue.h";
		local fh_c = file.open( file_c, "w" );
		local fh_h = file.open( file_h, "w" );

		/*
		 * emit contents, using the visitor pattern
		 *
		 * We perform in many passes, using each time a
		 * different visitor.
		 */

		// type information

		// emit a preamble
		local preprocname = "__" + string.toupper( name ) + "GLUE_H";
		file.puts( fh_h, "/* " + file_h + " */\n" );
		file.puts( fh_h,
				   "#ifndef " + preprocname + "\n"
				   "#define " + preprocname + "\n" );

		// emit information for enumeration values
		visitor = [CGenerator.EnumValuesVisitor new toplevel, self, fh_h];
		visitlist( defdata, visitor, fh_h );

		// emit information for flags values
		visitor = [CGenerator.FlagsValuesVisitor new toplevel, self, fh_h];
		visitlist( defdata, visitor, fh_h );

		// emit information for enumeration typeinfo
		visitor = [CGenerator.EnumTypesVisitor new toplevel, self, fh_h];
		visitlist( defdata, visitor, fh_h );

		// emit information for flags typeinfo
		visitor = [CGenerator.FlagsTypesVisitor new toplevel, self, fh_h];
		visitlist( defdata, visitor, fh_h );

		// emit mapping for humans
		visitor = [CGenerator.SymbolicDefsVisitor new toplevel, self, fh_h];
		visitlist( defdata, visitor, fh_h );

		// emit information for boxed types
		visitor = [CGenerator.BoxedInfoVisitor new toplevel, self, fh_h];
		visitlist( defdata, visitor, fh_h );

		// emit information for object types (actually set only infostruct)
		visitor = [CGenerator.ObjectInfoVisitor new toplevel, self, fh_h];
		visitlist( defdata, visitor, fh_h );

		// emit symbol defines for Boxed/Object fields
		visitor = [CGenerator.BoxedObjectSymDefsVisitor new toplevel, self, fh_h];
		visitlist( defdata, visitor, fh_h );

		// emit symbolic initialization function
		visitor = [CGenerator.SymbolicInitVisitor new toplevel, self, fh_h];
		visitlist( defdata, visitor, fh_h );

		// emit postamble
		file.puts( fh_h, "#endif /* " + preprocname + " */" );
		file.close( fh_h );

		// glue functions

		// emit a preamble
		file.puts( fh_c, "/* " + file_c + " */\n" );

		// emit getters/setters for boxed types
		visitor = [CGenerator.BoxedGlueVisitor new toplevel, self, fh_c];
		visitlist( defdata, visitor, fh_c );

		// emit getters/setters for object types
		visitor = [CGenerator.ObjectGlueVisitor new toplevel, self, fh_c];
		visitlist( defdata, visitor, fh_c );

		// emit glue for functions
		visitor = [CGenerator.FunctionGlueVisitor new toplevel, self, fh_c];
		visitlist( defdata, visitor, fh_c );

		// emit initialization function
		visitor = [CGenerator.InitFunctionVisitor new self, fh_c];
		visitlist( defdata, visitor, fh_c );

		// emit postamble
		file.close( fh_c );

		return self;
	}
}


// We should _really_ use the Singleton pattern
public toplevelModule = @nil;

/*
 * Toplevel Module class
 *
 * Implements the main C module object (not really an elastiC module,
 * but the file responsible for the registration of those; see Module
 * for the actual ones).
 * It's returned by the scheme IDL parser.
 */
public class ToplevelModule extends Object
{
	/* ------------------------------------------------------------------------
	 * INSTANCE DATA
	 * ------------------------------------------------------------------------ */

	// Toplevel module name
	local name;													// string

	// Definition file
	local spec;													// string

	// List of modules
	local modules;												// array

	// Current module
	local curmod;												// Module

	/*
	 * Types
	 *
	 * hash:  C type name -> Data object
	 */
	local type;													// hash

	// Misc
	local field2cpp;											// hash

	/* ------------------------------------------------------------------------
	 * METHODS
	 * ------------------------------------------------------------------------ */

	// CONSTRUCTOR

	method init(toplevelModuleName, specFile)
	{
		name      = toplevelModuleName;
		spec      = specFile;
		modules   = #[];
		curmod    = @nil;

		type      = %[];
		field2cpp = %[];

		toplevelModule = self;
	}

	// CLASS METHODS

	class method getToplevel()
	{
		return toplevelModule;
	}

	// CHILD MODULES

	method addModule(module)
	{
		array.push( modules, module );
	}

	method createAndAddModule(moduleName, modulePrefix)
	{
		local module = [ModuleInstance new moduleName, modulePrefix, self];
		array.push( modules, module );
		return module;
	}

	method getModules()
	{
		return modules;
	}

	method setCurrentModule(module)
	{
		curmod = module;
	}

	method getCurrentModule(module)
	{
		return curmod;
	}

	// GETTERS & SETTERS

	method getCPrefix()
	{
		return "ec_" + name;
	}

	method getName()
	{
		return name;
	}

	method getType: name
	{
		if (hash.has_key(type, name))
			return type[name];
		else
			return @nil;
	}
	method setType: name def: def
	{
		type[name] = def;
		return self;
	}

	method getField2cpp()
	{
		return field2cpp;
	}

	// MODULE GENERATION

	method run()
	{
		[[self _parse] _emit];
		return self;
	}

	method _parse()
	{
		// Parse specification file, getting internal OO representation
		Data.parsespec( self, spec );

		return self;
	}

	method _emit()
	{
		// Emit all C source files

		local module;
		for (module in modules)
		{
			// re-initialize fields for every module
			field2cpp = %[];

			[module _emit];
		}
	}
}


private function main( name, specfile )
{
	// All happens here
	[ToplevelModule new name, specfile];
	[[ToplevelModule getToplevel] run];

	// FIXME: use a more consistent Singleton pattern
	toplevelModule = @nil;
}

if (length(sys.args) == 2)
	main( sys.args[0], sys.args[1] );
