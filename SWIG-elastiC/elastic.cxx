/* ==========================================================================
 * elastic.cxx
 * ==========================================================================
 * Definitions for adding functions to elastiC.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: 19 Aug 1999
 *
 *   $Id: elastic.cxx,v 1.1 2002/05/31 12:30:00 panta Exp $
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

/***********************************************************************
 * $Header: /home/xubuntu/berlios_backup/github/tmp-cvs/elastic/Repository/SWIG-elastiC/elastic.cxx,v 1.1 2002/05/31 12:30:00 panta Exp $
 *
 * elastic.cxx
 *
 * Definitions for adding functions to elastiC
 ***********************************************************************/

#include "swig.h"
#include "elastic.h"

static char *elastic_usage = "\
elastiC Options (available with -elastic)\n\
-module name    - Set name of module\n\n";

// ---------------------------------------------------------------------
// ELASTIC::parse_args(int argc, char *argv[])
//
// Parse arguments.
// ---------------------------------------------------------------------

void ELASTIC::parse_args(int argc, char *argv[]) {

	int i;
  
	sprintf(LibDir,"%s",elastic_path);

	// Look for certain command line options

	// Look for additional command line options.
	for (i = 1; i < argc; i++) {  
		if (argv[i]) {
			if (strcmp(argv[i],"-module") == 0) {
				if (argv[i+1]) {
					set_module(argv[i+1], 0);
					mark_arg(i);
					mark_arg(i+1);
					i++;
				} else {
					arg_error();
				}
			} else if (strcmp(argv[i],"-help") == 0) {
				fprintf(stderr, "%s\n", elastic_usage);
				SWIG_exit(0);
			}
		}
	}
	// Add a symbol for this module

	add_symbol("SWIGELASTIC",0,0);

	// Set name of typemaps

	typemap_lang = "elastic";

}

// --------------------------------------------------------------------
// ELASTIC::parse()
//
// Parse the input file
// --------------------------------------------------------------------

void ELASTIC::parse()
{

	printf("Generating wrappers for elastiC\n");

	// Print out ELASTIC specific headers
  
	headers();

	// Tell the parser to first include a typemap definition file

/*	if (include_file("lang.map") == -1) {
		fprintf(stderr, "Unable to find lang.map !\n");
		SWIG_exit(1);
		}*/

	// Run the parser
  
	yyparse();

}

// ---------------------------------------------------------------------
// ELASTIC::set_module(char *mod_name)
//
// Sets the module name.
// Does nothing if it's already set (so it can be overridden as a command
// line option).
//
//----------------------------------------------------------------------

void ELASTIC::set_module(char *mod_name, char **) {

	if (module) return;
  
	module = new char[strlen(mod_name)+1];
	strcpy(module,mod_name);

}

// ---------------------------------------------------------------------
// ELASTIC::set_init(char *iname)
//
// Sets the initialization function name.
// Does nothing if it's already set
//
//----------------------------------------------------------------------

void ELASTIC::set_init(char *iname) {
	set_module(iname,0);
}

// ----------------------------------------------------------------------
// ELASTIC::add_primitive(char *name, char *function)
//
// Add some symbols to the primitive table
// ----------------------------------------------------------------------

void ELASTIC::add_primitive(char *name, char *function)
{
	Primitive *n;

	n = new Primitive;
	n->name = new char[strlen(name)+1];
	strcpy(n->name,name);
	n->function = new char[strlen(function)+1];
	strcpy(n->function, function);

	n->next = head;
	head = n;
}

// ---------------------------------------------------------------------
// ELASTIC::print_primitives()
//
// Prints out the primitive array.
// ---------------------------------------------------------------------

void ELASTIC::print_primitives()
{
	Primitive *n;

	fprintf(f_init,"\n");
	n = head;
	while (n) {
		fprintf(f_init,"\tEcAddPrimitive( \"%s.%s\", %s );\n", module, n->name, n->function);
		n = n->next;
	}
	fprintf(f_init,"\n");
}

// ---------------------------------------------------------------------
// ELASTIC::headers(void)
//
// Generate the appropriate header files for ELASTIC interface.
// ----------------------------------------------------------------------

void ELASTIC::headers(void)
{
	emit_banner(f_header);

	fprintf(f_header,"/* Implementation : ELASTIC */\n\n");

	// Include header file code fragment into the output
	if (insert_file("header.swg",f_header) == -1) {
		fprintf(stderr,"Fatal Error. Unable to locate 'header.swg'.\n");
		SWIG_exit(1);
	}

	fprintf(f_header,"#define SWIGELASTIC\n");
	fprintf(f_header,"#include <stdio.h>\n");
	fprintf(f_header,"#include <string.h>\n");
	fprintf(f_header,"#include <stdlib.h>\n");

	// Emit the default SWIG pointer type-checker (for strings)
	if (insert_file("swigptr.swg",f_header) == -1) {
		fprintf(stderr,"Fatal Error. Unable to locate 'swigptr.swg'.\n");
		SWIG_exit(1);
	}

	// Write out hex conversion functions

/*	if (!NoInclude) {
		if (insert_file("elastic.swg", f_header) == -1) {
			fprintf(stderr,"SWIG : Fatal error.  Unable to locate 'elastic.swg' in SWIG library.\n");
			SWIG_exit(1);
		}
		emit_hex(f_header);
	} else {
		fprintf(f_header,"#ifdef __cplusplus\n");
		fprintf(f_header,"extern \"C\" {\n");
		fprintf(f_header,"#endif\n");
		fprintf(f_header,"extern void SWIG_MakePtr(char *, void *, char *);\n");
		fprintf(f_header,"extern void SWIG_RegisterMapping(char *, char *, void *(*)(void *));\n");
		fprintf(f_header,"extern char *SWIG_GetPtr(char *, void **, char *);\n");
		fprintf(f_header,"#ifdef __cplusplus\n");
		fprintf(f_header,"}\n");
		fprintf(f_header,"#endif\n");
		}*/
}

// --------------------------------------------------------------------
// ELASTIC::initialize()
//
// Output initialization code that registers functions with the
// interface.
// ---------------------------------------------------------------------
void ELASTIC::initialize()
{
	int i;
        
	if (!module) {
		module = "swig_init";
		fprintf(stderr,"SWIG : *** Warning. No module name specified.\n");
	}
  
	fprintf(f_header,"#define SWIG_init    ec_%s_init\n", module);
	fprintf(f_header,"#define SWIG_cleanup ec_%s_cleanup\n\n", module);

	// Start generating the initialization function
	fprintf(f_init, "EC_OBJ SWIG_init(void) {\n" );
	fprintf(f_init, "\tEC_OBJ pkg = EC_NIL;\n\n" );

	fprintf(f_init, "\tpkg = EcPackageIntroduce(\"%s\");\n\n", module );

	if (InitNames) {
		i = 0;
		while (InitNames[i]) {
			fprintf(f_init,"\t %s();\n",InitNames[i]);
			i++;
		}
	}
}

// ---------------------------------------------------------------------
// ELASTIC::close(void)
//
// Wrap things up.  Close initialization function.
// ---------------------------------------------------------------------

void ELASTIC::close(void)
{
	// Emit primitive registration
	print_primitives();

	// Dump the pointer equivalency table
	emit_ptr_equivalence(f_init);

	// Finish off our init function and print it to the init file
	fprintf(f_init, "\n\t return pkg;\n");
	fprintf(f_init, "}\n");

	fprintf(f_init, "\nvoid SWIG_cleanup(void) {}\n" );
}

// ----------------------------------------------------------------------
// ELASTIC::create_command(char *cname, char *iname)
//
// Creates an ELASTIC command from a C function.
// ----------------------------------------------------------------------

void ELASTIC::create_command(char *cname, char *iname) {
	char *wname;

	wname = name_wrapper(cname,"");

	fprintf(f_init,"\tEcAddPrimitive( \"%s.%s\", %s );\n", module, iname, wname);
}

// ----------------------------------------------------------------------
// ELASTIC::get_pointer(char *iname, char *srcname, char *src, char *target,
//                     DataType *t, WrapperFunction &f, char *ret)
//
// Emits code to get a pointer and do type checking.
//      iname = name of the function/method  (used for error messages)
//      srcname = Name of source (used for error message reporting).
//      src   = name of variable where source string is located.
//      dest  = name of variable where pointer value is stored.
//      t     = Expected datatype of the parameter
//      f     = Wrapper function object being used to generate code.
//      ret   = return code upon failure.
//
// Note : pointers are stored as strings so you first need to get
// a string and then call _swig_get_hex() to extract a point.
//
// This module is pretty ugly, but type checking is kind of nasty
// anyways.
// ----------------------------------------------------------------------

void ELASTIC::get_pointer(char *iname, char *srcname, char *src, char *dest,
						  DataType *t, String &f, char *ret)
{
	// Now get the pointer value from the string and save in dest
  
	f << tab4 << "if (" << src << ") {\n";
	f << tab8 << "if (SWIG_GetPtr(" << src << ",(void **) &" << dest << ",";

	// If we're passing a void pointer, we give the pointer conversion a NULL
	// pointer, otherwise pass in the expected type.
  
	if (t->type == T_VOID) f << "(char *) 0 )) {\n";
	else
		f << "\"" << t->print_mangle() << "\")) {\n";

  // This part handles the type checking according to three different
  // levels.   0 = no checking, 1 = warning message, 2 = strict.

	switch(TypeStrict) {
	case 0: // No type checking
		f << tab8 << "}\n";
		break;

	case 1: // Warning message only

		// Change this part to how you want to handle a type-mismatch warning.
		// By default, it will just print to stderr.

		f << tab8 << tab4 << "fprintf(stderr,\"Warning : type mismatch in " << srcname
		  << " of " << iname << ". Expected " << t->print_mangle()
		  << ", received %s\\n\"," << src << ");\n"
		  << tab8 << "}\n";

		break;
	case 2: // Super strict mode.

		// Change this part to return an error.

		f << tab8 << tab4 << "EcAlert(EcError,\"Type error in " << srcname
		  << " of " << iname << ". Expected " << t->print_mangle() << ".\");\n"
		  << tab8 << "return " << ret << ";\n"
		  << tab8 << "}\n";
		break;
    
	default :
		fprintf(stderr,"SWIG Error. Unknown strictness level\n");
		break;
	}
	f << tab4 << "}\n";
}

// ----------------------------------------------------------------------
// ELASTIC::convert_self()
//
// Called during the function generation process, to determine what to
// use as the "self" variable during the call.  Derived classes may emit code
// to convert the real self pointer into a usable pointer.
//
// Returns the name of the variable to use as the self pointer
// ----------------------------------------------------------------------
char *ELASTIC::convert_self(WrapperFunction &)
{
  // Default behaviour is no translation
  return "";
}

// ----------------------------------------------------------------------
// ELASTIC::create_function(char *name, char *iname, DataType *d,
//                             ParmList *l)
//
// Create a function declaration and register it with the interpreter.
// ----------------------------------------------------------------------

void ELASTIC::create_function(char *name, char *iname, DataType *d, ParmList *l)
{
	String           source, target;
	String           wname, self_name, call_name;
	char             temp[256], argnum[20];
	char             *usage = 0;
	String           arglist;
	String           get_pointers;
	String           check;
	String           build;
	char             *tm;
	String           cleanup, outarg;
	int              have_build = 0;
	Parm *p;

	// A new wrapper function object

	WrapperFunction  f;
	String           parse_args;

	// Make a wrapper name for this function
  
	wname = name_wrapper(iname,"");

	// Now write the wrapper function itself....this is pretty ugly

	f.def << "static EC_OBJ " << wname << "(EC_OBJ stack, EcAny _unused_userdata) {\n";

	f.add_local("EC_OBJ","_resultobj");

	// Get the function usage string for later use
  
	usage = usage_func(iname,d,l);

	// Write code to extract function parameters.
	// This is done in one pass, but we need to construct three independent
	// pieces.
	//      1.    elastiC format string such as "iis"
	//      2.    The actual arguments to put values into
	//      3.    Pointer conversion code.
	//
	// If there is a type mapping, we will extract the elastiC argument
	// as a raw EC_OBJ and let the user deal with it.
	//

	// Emit all of the local variables for holding arguments.
	int pcount = emit_args(d,l,f), i, j;

	parse_args << tab4 << "_resultobj = EcParseStackFunction(\"" << module << "." << iname << "\", TRUE, stack, \"";
	i = 0;
	j = 0;

	// Get number of optional/default arguments
	int numopt = l->numopt();
	if (numopt) have_defarg = 1;

	p = l->get_first();
	while (p != 0)
	{
		// Generate source and target strings
		sprintf(source,"_obj%d",i);
		sprintf(target,"_arg%d",i);
		sprintf(argnum,"%d",j+1);

		// Only consider this argument if it's not ignored

		if (!p->ignore) {
			arglist << ",";
			// Add an optional argument separator if needed
    
			if (j == pcount-numopt) {  
				parse_args << "|";
			}

			// Look for input typemap

			if ((tm = typemap_lookup("in","elastic",p->t,p->name,source,target,&f))) {
				parse_args << "O";        // Grab the argument as a raw EC_OBJ
				f.add_local("EC_OBJ",source,"0");
				arglist << "&" << source;
				if (i >= (pcount-numopt))
					get_pointers << tab4 << "if (" << source << ")\n";
				get_pointers << tm << "\n";
				get_pointers.replace("$argnum", argnum);
				get_pointers.replace("$arg",source);
			} else {

				// Check if this parameter is a pointer.  If not, we'll get values

				if (!p->t->is_pointer) {
					// Extract a parameter by "value"
        
					switch(p->t->type) {
          
						// Handle integers here.  Usually this can be done as a single
						// case if you appropriate cast things.   However, if you have
						// special cases, you'll need to add more code.  
            
					case T_INT:
					case T_UINT:
					case T_SINT: 
						parse_args << "i";
						break;
					case T_SHORT:
					case T_USHORT:
					case T_SSHORT:
						parse_args << "i";
						break;
					case T_LONG:
					case T_ULONG:
					case T_SLONG:
						parse_args << "i";
						break;
					case T_SCHAR:
					case T_UCHAR:
					case T_CHAR:
						parse_args << "c";
						break;
					case T_FLOAT:
					case T_DOUBLE:
						parse_args << "f";
						break;
					case T_BOOL:
						parse_args << "b";
						break;

                        // Void.. Do nothing.
					case T_VOID:
						break;
            
						// User defined.   This is usually invalid.   No way to pass a
						// complex type by "value".  We'll just pass into the unsupported
						// datatype case.
					case T_USER:
                        // Unsupported data type

					default :
						fprintf(stderr,"%s : Line %d. Unable to use type %s as a function argument.\n",input_file, line_number, p->t->print_type());
						break;
					}

					// Emit code for parameter list

					if (p->t->type != T_VOID)
						arglist << "&_arg" << i;

				} else {
					// Is some other kind of variable.   
	  
					parse_args << "s";
	  
					if ((p->t->type == T_CHAR) && (p->t->is_pointer == 1)) {
						arglist << "&_arg" << i;
					} else {
						// Have some sort of pointer variable.  Create a temporary local
						// variable for the string and read the pointer value into it.

						sprintf(source,"_argc%d", i);
						sprintf(target,"_arg%d", i);
						sprintf(temp,"argument %d",i+1);
	    
						f.add_local("char *", source,"0");
						arglist << "&" << source;
						get_pointer(iname, temp, source, target, p->t, get_pointers, "NULL");
					}
				}
			}
			j++;
		}
		// Check if there was any constraint code
		if ((tm = typemap_lookup("check","elastic",p->t,p->name,source,target))) {
			check << tm << "\n";
			check.replace("$argnum", argnum);
		}
		// Check if there was any cleanup code
		if ((tm = typemap_lookup("freearg","elastic",p->t,p->name,target,source))) {
			cleanup << tm << "\n";
			cleanup.replace("$argnum", argnum);
			cleanup.replace("$arg",source);
		}
		if ((tm = typemap_lookup("argout","elastic",p->t,p->name,target,"_resultobj"))) {
			outarg << tm << "\n";
			outarg.replace("$argnum", argnum);
			outarg.replace("$arg",source);
			have_output++;
		}
		if ((tm = typemap_lookup("build","elastic",p->t,p->name,source,target))) {
			build << tm << "\n";
			have_build = 1;
		}
		p = l->get_next();
		i++;
	}

	parse_args << "\"";						// No additional arguments
	parse_args << arglist << ");\n";
	parse_args << tab4 << "if (EC_ERRORP(_resultobj)) return _resultobj;\n";

	self_name = convert_self(f);

	/* Now slap the whole first part of the wrapper function together */

	f.code << parse_args << get_pointers << check;

	// Special handling for build values

	if (have_build) {
		char temp1[256];
		char temp2[256];
		l->sub_parmnames(build);            // Replace all parameter names
		for (i = 0; i < l->nparms; i++) {
			p = l->get(i);
			if (strlen(p->name) > 0) {
				sprintf(temp1,"_in_%s", p->name);
			} else {
				sprintf(temp1,"_in_arg%d", i);
			}
			sprintf(temp2,"_obj%d",i);
			build.replaceid(temp1,temp2);
		}
		f.code << build;
	}

	// This function emits code to call the real function.  Assuming you read
	// the parameters in correctly, this will work.

	call_name = "";
	call_name << self_name << name;
	emit_func_call(call_name,d,l,f);

	// Now emit code to return the functions return value (if any).
	// If there was a result, it was saved in _result.
	// If the function is a void type, don't do anything.

	if ((tm = typemap_lookup("out","elastic",d,iname,"_result","_resultobj"))) {
		// Yep.  Use it instead of the default
		f.code << tm << "\n";
	} else {
		if ((d->type != T_VOID) || (d->is_pointer)) {
			// Now have return value, figure out what to do with it.

			if (!d->is_pointer) {
				// Function returns a "value"

				switch(d->type) {
	  
					// Return an integer type
	  
				case T_INT:
				case T_SINT:
				case T_UINT:
				case T_SHORT:
				case T_SSHORT:
				case T_USHORT:
				case T_LONG:
				case T_SLONG:
				case T_ULONG:
					f.code << tab4 << "_resultobj = EcBuildValue(\"i\",_result);\n";
					break;
				case T_BOOL:
					f.code << tab4 << "_resultobj = EcBuildValue(\"b\",_result);\n";
					break;

				case T_SCHAR:
				case T_UCHAR:
				case T_CHAR:
					f.code << tab4 << "_resultobj = EcBuildValue(\"c\",_result);\n";
					break;
	  
					// Return a floating point value

				case T_DOUBLE:
				case T_FLOAT:
					f.code << tab4 << "_resultobj = EcBuildValue(\"f\",_result);\n";
					break;
	  
					// Return a single ASCII value.  Usually we need to convert
					// it to a NULL-terminate string and return that instead.
	  
				case T_USER :
	  
					// Return something by value
					// We're living dangerously here, but life is short...play hard
	  
					// Oops.  Need another local variable
					f.add_local("char","_ptemp[128]");
	  
					d->is_pointer++;
					f.code << tab4 << "SWIG_MakePtr(_ptemp, (void *) _result,\""
						   << d->print_mangle() << "\");\n";
					d->is_pointer--;
					// Return a character string containing our pointer.
	  
					f.code << tab4 << "_resultobj = EcBuildValue(\"s\",_ptemp);\n";
					break;

				default:
					fprintf(stderr,"%s: Line %d. Unable to use return type %s in function %s.\n", input_file, line_number, d->print_type(), name);
					break;
				}
			} else {
	
				// Return type is a pointer.   We'll see if it's a char * and return
				// a string. Otherwise, we'll convert it into a SWIG pointer and return
				// that.
	
				if ((d->type == T_CHAR) && (d->is_pointer == 1)) {
	  
					// Return a character string
					f.code << tab4 << "_resultobj = EcBuildValue(\"s\", _result);\n";

					// If declared as a new object, free the result

				} else {
	  
					// Build a SWIG pointer.
					f.add_local("char","_ptemp[128]");
	  
					f.code << tab4 << "SWIG_MakePtr(_ptemp, (char *) _result,\""
						   << d->print_mangle() << "\");\n";
	  
					// Return a character string containing our pointer.
					f.code << tab4 << "_resultobj = EcBuildValue(\"s\",_ptemp);\n";
				}
			}
		} else {
			// no return value and no output args
			//if (!have_output) {
			f.code << tab4 << "_resultobj = EC_NIL;\n";
			//} 
		}
	}

	// Check to see if there were any output arguments, if so we're going to
	// create an elastiC list object out of the current result

	f.code << outarg;

	// If there was any other cleanup needed, do that

	f.code << cleanup;

	// Look to see if there is any newfree cleanup code

	if (NewObject) {
		if ((tm = typemap_lookup("newfree","elastic",d,iname,"_result",""))) {
			f.code << tm << "\n";
		}
	}

	// See if there is any argument cleanup code

	if ((tm = typemap_lookup("ret","elastic",d,iname,"_result",""))) {
		// Yep.  Use it instead of the default
		f.code << tm << "\n";
	}
  
	f.code << tab4 << "return _resultobj;\n";
	f.code << "}\n";

	// Substitute the cleanup code
	f.code.replace("$cleanup",cleanup);

	// Substitute the function name
	f.code.replace("$name",iname);

	// Dump the function out
	f.print(f_wrappers);

	// Now register the function with the interpreter.  

	add_primitive(iname, wname);

	// Create a documentation entry for this

	if (doc_entry) {
		static DocEntry *last_doc_entry = 0;
		doc_entry->usage << usage;
		if (last_doc_entry != doc_entry) {
			doc_entry->cinfo << "returns " << d->print_type();
			last_doc_entry = doc_entry;
		}
	}
}

// ---------------------------------------------------------------------------
// ELASTIC::usage_func(char *iname, DataType *t, ParmList *l)
// 
// Produces a string indicating how to call a function in the target
// language.
//
// ---------------------------------------------------------------------------

char *ELASTIC::usage_func(char *iname, DataType *, ParmList *l) {

  static String temp;
  Parm  *p;
  int    i;

  temp = "";
  temp << iname << "(";
  
  // Now go through and print parameters 
  // You probably don't need to change this

  i = 0;
  p = l->get_first();
  while (p != 0) {
    if (!p->ignore) {
      i++;
      /* If parameter has been named, use that.   Otherwise, just print a type  */

      if ((p->t->type != T_VOID) || (p->t->is_pointer)) {
	if (strlen(p->name) > 0) {
	  temp << p->name;
	} else {
	  temp << p->t->print_type();
	}
      }
      p = l->get_next();
      if (p != 0) {
	if (!p->ignore)
	  temp << ",";
      }
    } else {
      p = l->get_next();
      if (p) {
	if ((!p->ignore) && (i > 0))
	  temp << ",";
      }
    }
  }

  temp << ")";

  // Create result. Don't change this

  return temp.get();

}

// -----------------------------------------------------------------------
// ELASTIC::link_variable(char *name, char *iname, DataType *d)
//
// Create a link to a C variable.
// This creates a single function _wrap_elastic_var_varname().
// This function takes a single optional argument.   If supplied, it means
// we are setting this variable to some value.  If ommitted, it means we are
// simply evaluating this variable.  Either way, we return the variables
// value.
// -----------------------------------------------------------------------

void ELASTIC::link_variable(char *name, char *iname, DataType *t)
{

	char  var_name[256];
	char  *tm;

	// GETTER

	sprintf(var_name,"_wrap_elastic_var_get_%s",iname);

	if ((t->type != T_USER) || (t->is_pointer)) {

		fprintf(f_wrappers,"EC_OBJ %s(EC_OBJ stack, EcAny _unused_userdata) {\n", var_name);
		fprintf(f_wrappers,"\t EC_OBJ _resultobj;\n");

		if ((tm = typemap_lookup("varout","elastic",t,name,name,"_resultobj"))) {
			// Yep.  Use it instead of the default
			fprintf(f_wrappers,"%s\n",tm);
		} else {
			if (!t->is_pointer) {
				/* Return variable by value */
        
				switch(t->type) {
          
					// Signed Integer
          
				case T_INT:
				case T_SINT:
				case T_SHORT:
				case T_SSHORT:
				case T_LONG:
				case T_SLONG:
				case T_UINT:
				case T_USHORT:
				case T_ULONG:
					fprintf(f_wrappers,"\t _resultobj = EcMakeInt((EcInt) %s);\n", name);
					break;
          
					// Floats
          
				case T_DOUBLE:
				case T_FLOAT:
					fprintf(f_wrappers,"\t _resultobj = EcMakeFloat((EcFloat) %s);\n", name);
					break;

				case T_UCHAR:
				case T_SCHAR:
				case T_CHAR:
					fprintf(f_wrappers,"\t _resultobj = EcMakeChar((EcChar)%s);\n",name);
					break;
				default :
					/* Unknown type */
					break;
				}
			} else {
        
				// Is a pointer return type
        
				if ((t->type == T_CHAR) && (t->is_pointer == 1)) {
					fprintf(f_wrappers,"\t _resultobj = EcMakeString(%s, -1);\n",name);
				} else {
					// Is an ordinary pointer type.
					if ((t->type == T_CHAR) || (t->is_pointer)){
						fprintf(f_wrappers,"\t char _ptemp[128];\n");
					}
					fprintf(f_wrappers,"\t  SWIG_MakePtr(_ptemp, %s,\"%s\");\n",name,
							t->print_mangle());
					fprintf(f_wrappers,"\t _resultobj = EcMakeString(_ptemp, -1);\n");
				}
			}
		}
		fprintf(f_wrappers,"\t return _resultobj;\n");
		fprintf(f_wrappers,"}\n");

		// Now add symbol to the elastiC interpreter
    
		fprintf(f_init,"\tEcAddPrimitive(\"%s.get_%s\", %s);\n",module, iname, var_name);

	} else {
		fprintf(stderr,"%s : Line %d. ** Warning. Unable to link with type %s (ignored).\n",
				input_file, line_number, t->print_type());
	}

	// SETTER

	sprintf(var_name,"_wrap_elastic_var_set_%s",iname);

	if ((t->type != T_USER) || (t->is_pointer)) {

		fprintf(f_wrappers,"EC_OBJ %s(EC_OBJ stack, EcAny _unused_userdata) {\n", var_name);
		if ((t->type == T_CHAR) || (t->is_pointer)){
			fprintf(f_wrappers,"\t char *_temp, _ptemp[128];\n");
			fprintf(f_wrappers,"\t int  _len;\n");
		}
		fprintf(f_wrappers,"\t EC_OBJ _resultobj;\n");

		// Check for a setting of the variable value

		// Emit the target variable

		char *typechar = NULL;
		char *otype    = NULL;

		if ((tm = typemap_lookup("varin","elastic",t,name,"s_0",name))) {
		} else {
			if (!t->is_pointer) {
				
				switch(t->type) {
					// Signed Integer
					
				case T_INT:
				case T_SINT:
				case T_SHORT:
				case T_SSHORT:
				case T_LONG:
				case T_SLONG:
				case T_UINT:
				case T_USHORT:
				case T_ULONG:
					typechar = "i";
					otype    = "tc_inum";
					fprintf(f_wrappers,"\t EcInt s_0;\n");
					break;
					
					// Floating point
					
				case T_FLOAT:
				case T_DOUBLE:
					typechar = "f";
					otype    = "tc_fnum";
					fprintf(f_wrappers,"\t EcFloat s_0;\n");
					break;
            
					// Character value
					
				case T_UCHAR:
				case T_SCHAR:
				case T_CHAR:
					typechar = "c";
					otype    = "tc_char";
					fprintf(f_wrappers,"\t EcChar s_0;\n");
					break;
					
					// Unknown value 
					
				default:
					fprintf(stderr,"Line %d.  Error, unsupported data-type.\n", line_number);
					break;
				}
			} else {
				fprintf(f_wrappers,"\t EC_OBJ s_0;\n");
			}
		}

		if (Status & STAT_READONLY) {
			fprintf(f_wrappers,"\t EcAlert(EcError, \"Unable to set %s. Variable is read only.\");\n", iname);
			fprintf(f_wrappers,"\t return EC_NIL;\n");
		}

		if (typechar)
			fprintf(f_wrappers,"\t _resultobj = EcParseStackFunction(\"%s.set_%s\", TRUE, stack, \"%s\", &s_0);\n", module, iname, typechar);
		else
			fprintf(f_wrappers,"\t _resultobj = EcParseStackFunction(\"%s.set_%s\", TRUE, stack, \"O\", &s_0);\n", module, iname);
		fprintf(f_wrappers,"\t if (EC_ERRORP(_resultobj)) return _resultobj;\n\n");

		// Yup. Extract the type from s_0 and set variable value 
		if ((tm = typemap_lookup("varin","elastic",t,name,"s_0",name))) {
			// Yep.  Use it instead of the default
			fprintf(f_wrappers,"%s\n",tm);
		} else {
			if (!t->is_pointer) {
				
				switch(t->type) {
					// Signed Integer
					
				case T_INT:
				case T_SINT:
				case T_SHORT:
				case T_SSHORT:
				case T_LONG:
				case T_SLONG:
				case T_UINT:
				case T_USHORT:
				case T_ULONG:
					fprintf(f_wrappers,"\t %s = %s (s_0);\n",name, t->print_cast());
					break;
					
					// Floating point
					
				case T_FLOAT:
				case T_DOUBLE:
					fprintf(f_wrappers,"\t %s = %s (s_0);\n",name, t->print_cast());
					break;
            
					// Character value
					
				case T_UCHAR:
				case T_SCHAR:
				case T_CHAR:
					fprintf(f_wrappers,"\t %s = (s_0);\n", name);
					break;
					
					// Unknown value 
					
				default:
					fprintf(stderr,"Line %d.  Error, unsupported data-type.\n", line_number);
					break;
				}
			} else {
				
				// Have some sort of pointer type here, Process it differently
				
				if ((t->type == T_CHAR) && (t->is_pointer == 1)) {
					fprintf(f_wrappers,"\t _temp = EC_STRDATA(s_0);\n");
					fprintf(f_wrappers,"\t _len  = EC_STRLEN(s_0);\n");
					fprintf(f_wrappers,"\t if (%s) { free(%s);}\n", name,name);
					fprintf(f_wrappers,"\t %s = (char *) malloc((_len+1)*sizeof(char));\n",name);
					fprintf(f_wrappers,"\t strncpy(%s,_temp,_len);\n",name);
					fprintf(f_wrappers,"\t %s[_len] = 0;\n", name);
				} else {
					// Set the value of a pointer
					fprintf(f_wrappers,"\t _temp = ec_stringdup( EC_STRDATA(s_0) ); /* strdup necessary ???? */\n");
					fprintf(f_wrappers,"\t _len  = EC_STRLEN(s_0);\n");
					fprintf(f_wrappers,"\t if (SWIG_GetPtr(_temp, (void **) &%s,",name);
					if (t->type == T_VOID) fprintf(f_wrappers,"(char *) 0)) {\n");
					else 
						fprintf(f_wrappers,"\"%s\")) {\n", t->print_mangle());
					
					// Now emit code according to the level of strictness desired
					
					switch(TypeStrict) {
					case 0: // No type checking
						fprintf(f_wrappers,"\t}\n");
						break;
					case 1: // Warning message only
						fprintf(f_wrappers,
								"\t fprintf(stderr,\"Warning : type mismatch in variable %s. Expected %s, received %%s\\n\", _temp);\n", name, t->print_mangle());
						fprintf(f_wrappers,"\t }\n");
						break;
					case 2: // Super strict mode.
              
//            fprintf(f_wrappers,"\t\t EcAlert(EcError, \"Type error in variable %s. Expected %s.\", s_0);\n", name,t->print_mangle());
						fprintf(f_wrappers,"\t}\n");
						break;
						
					default :
						fprintf(stderr,"Unknown strictness level\n");
						break;
					}
				}
			}
		}
		fprintf(f_wrappers,"\n");

		// Now return the value of the variable (regardless of evaluating or setting)

		if ((tm = typemap_lookup("varout","elastic",t,name,name,"_resultobj"))) {
			// Yep.  Use it instead of the default
			fprintf(f_wrappers,"%s\n",tm);
		} else {
			if (!t->is_pointer) {
				/* Return variable by value */
        
				switch(t->type) {
          
					// Signed Integer
          
				case T_INT:
				case T_SINT:
				case T_SHORT:
				case T_SSHORT:
				case T_LONG:
				case T_SLONG:
				case T_UINT:
				case T_USHORT:
				case T_ULONG:
					fprintf(f_wrappers,"\t _resultobj = EcMakeInt((EcInt) %s);\n", name);
					break;
          
					// Floats
          
				case T_DOUBLE:
				case T_FLOAT:
					fprintf(f_wrappers,"\t _resultobj = EcMakeFloat((EcFloat) %s);\n", name);
					break;

				case T_UCHAR:
				case T_SCHAR:
				case T_CHAR:
					fprintf(f_wrappers,"\t _resultobj = EcMakeChar((EcChar)%s);\n",name);
					break;
				default :
					/* Unknown type */
					break;
				}
			} else {
        
				// Is a pointer return type
        
				if ((t->type == T_CHAR) && (t->is_pointer == 1)) {
					fprintf(f_wrappers,"\t _resultobj = EcMakeString(%s, -1);\n",name);
				} else {
					// Is an ordinary pointer type.
					fprintf(f_wrappers,"\t  SWIG_MakePtr(_ptemp, %s,\"%s\");\n",name,
							t->print_mangle());
					fprintf(f_wrappers,"\t _resultobj = EcMakeString(_ptemp, -1);\n");
				}
			}
		}
		fprintf(f_wrappers,"\t return _resultobj;\n");
		fprintf(f_wrappers,"}\n");
    
		// Now add symbol to the elastiC interpreter
    
		fprintf(f_init,"\tEcAddPrimitive(\"%s.set_%s\", %s);\n",module, iname, var_name);
    
	} else {
		fprintf(stderr,"%s : Line %d. ** Warning. Unable to link with type %s (ignored).\n",
				input_file, line_number, t->print_type());
	}

	// Add a documentation entry
  
	if (doc_entry) {
		char *usage = 0; 
		usage_var(iname,t,&usage);
		doc_entry->usage << usage;
		doc_entry->cinfo << "Global : " << t->print_type() << " " << name;
		delete usage;
	}

}

// -----------------------------------------------------------------------
// ELASTIC::declare_const(char *name, char *iname, DataType *type, char *value)
//
// Makes a constant.   Not sure how this is really supposed to work.
// I'm going to fake out SWIG and create a variable instead.
// ------------------------------------------------------------------------

void ELASTIC::declare_const(char *name, char *, DataType *type, char *value) {

	int OldStatus = Status;      // Save old status flags
	char   var_name[256];

	Status = STAT_READONLY;      // Enable readonly mode.

	// Make a static variable;

	sprintf(var_name,"_wrap_const_%s",name);

	if ((type->type == T_USER) && (!type->is_pointer)) {
		fprintf(stderr,"%s : Line %d.  Unsupported constant value.\n", input_file, line_number);
		return;
	}

	// Create variable and assign it a value

	fprintf(f_header,"static %s %s = ", type->print_type(), var_name);
	if ((type->type == T_CHAR) && (type->is_pointer <= 1)) {
		fprintf(f_header,"\"%s\";\n", value);
	} else {
		fprintf(f_header,"%s;\n", value);
	}

	// Now create a variable declaration

	link_variable(var_name, name, type);
	Status = OldStatus;

	if (doc_entry) {
		char *usage = 0;
		usage_const(name,type,value,&usage);
		doc_entry->usage = "";
		doc_entry->usage << usage;
		doc_entry->cinfo = "";
		doc_entry->cinfo << "Constant: " << type->print_type();
		delete usage;
	}

}

// ----------------------------------------------------------------------
// ELASTIC::usage_var(char *iname, DataType *t, char **s)
//
// Produces a usage string for a elastiC variable.
// ----------------------------------------------------------------------

void ELASTIC::usage_var(char *iname, DataType *t, char **s) {

	char temp[1024], *c;

	sprintf(temp,"(%s)", iname);
	c = temp + strlen(temp);

	if (!((t->type != T_USER) || (t->is_pointer))) {
		sprintf(c," - unsupported");
	}

	if (*s == 0) 
		*s = new char[strlen(temp)+1];
	strcpy(*s,temp);
}

// ---------------------------------------------------------------------------
// ELASTIC::usage_func(char *iname, DataType *t, ParmList *l, char **s)
// 
// Produces a usage string for a function in elastiC
// ---------------------------------------------------------------------------

void ELASTIC::usage_func(char *iname, DataType *, ParmList *l,
						 char **s) {

	char temp[1024];
	char *c;
	int  i;
	Parm  *p;

	sprintf(temp,"(%s ", iname);
	c = temp + strlen(temp);
  
	/* Now go through and print parameters */

	p = l->get_first();
	while (p != 0) {
    
		/* If parameter has been named, use that.   Otherwise, just print a type  */

		if ((p->t->type != T_VOID) || (p->t->is_pointer)) {
			if (strlen(p->name) > 0) {
				sprintf(c,"%s ",p->name);
				c += strlen(p->name)+1;
			}
			else {
				sprintf(c,"%s",p->t->name);
				c += strlen(p->t->name);
				if (p->t->is_pointer) {
					for (i = 0; i < (p->t->is_pointer-p->t->implicit_ptr); i++) {
						sprintf(c,"*");
						c++;
					}
				}
			}
		}
		p = l->get_next();
		if (p != 0) {
			sprintf(c," ");
			c++;
		}
	}
	sprintf(c,")");
	if (*s == 0) 
		*s = new char[strlen(temp)+1];
	strcpy(*s,temp);
}


// ----------------------------------------------------------------------
// ELASTIC::usage_const(char *iname, DataType *type, char *value, char **s)
//
// Produces a usage string for a elastiC constant
// ----------------------------------------------------------------------

void ELASTIC::usage_const(char *iname, DataType *, char *value, char **s) {

	char temp[1024];

	sprintf(temp,"(%s %s)", iname, value);

	if (*s == 0) 
		*s = new char[strlen(temp)+1];
	strcpy(*s,temp);

}

/*********************************************************************************
 *
 * -- Revision History
 * $Log: elastic.cxx,v $
 * Revision 1.1  2002/05/31 12:30:00  panta
 * Initial revision
 *
 *
 ***********************************************************************/

