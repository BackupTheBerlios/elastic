/* ==========================================================================
 * CodeEmitter.ec
 * ==========================================================================
 * Generic C code generation.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat Dec 11 08:58:03 MET 1999
 *
 *   $Id: CodeEmitter.ec,v 1.1 2002/05/30 12:41:39 panta Exp $
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

package CodeEmitter;

import basic;
import string;
import array;
import file;

/*
 * Function
 */

public class Function extends basic.Object
{
	local fh;

	local name;
	local args;
	local rettype;
	local retvalue;
	local priv;

	local locvars;

	local body;

	method init( outfh, funcname, returnType, isstatic )
	{
		fh = outfh;

		name     = funcname;
		args     = #[];
		rettype  = returnType;
		retvalue = "";
		locvars  = #[];
		body     = "";
		priv     = isstatic;
	}

	method addArg: name typed: type
	{
		array.push( args, #[name, type] );
	}

	method addLocal: name typed: type
	{
		return [self addLocal: name typed: type init: @nil];
	}

	method addLocal: name typed: type init: init
	{
		array.push( locvars, #[type, name, init] );
	}

	method addBody: text
	{
		if ((body == "") || (body[-1] == "\n"))
		{
			body = body + "\t" + text;
		}
		else
			body = body + "\n\t" + text;
	}

	method addReturnValue: expr
	{
		retvalue = expr;
	}

	method emit()
	{
		[self emitHeading];
		[self emitBody];
		[self emitTail];
	}

	method emitHeading()
	{
		if (priv)
			file.printf( fh, "static " );
		file.printf( fh, rettype + " " );

		local argp, argt, argn;
		local argstring = "";
		for (argp in args)
		{
			argn = argp[0];
			argt = argp[1];

			if (argstring == "")
				argstring = argt;
			else
				argstring = argstring + ", " + argt;
			if ((argstring != "") && ((argstring[-1] == " ") ||
									  (argstring[-1] == "*")))
				argstring = argstring + argn;
			else
				argstring = argstring + " " + argn;
		}

		file.printf( fh, name + "( " + argstring + " )\n" );
		file.printf( fh, "{\n" );

		local locp, loct, locn, loci;
		local locstring = "";
		for (locp in locvars)
		{
			loct = locp[0];
			locn = locp[1];
			loci = locp[2];

			locstring = locstring + "\t" + basic.sprintf( "%-20s", loct );
			if ((locstring != "") && ((locstring[-1] == " ") ||
									  (locstring[-1] == "*")))
				locstring = locstring + locn;
			else
				locstring = locstring + " " + locn;
			if (loci)
				locstring = locstring + " = " + loci;
			locstring = locstring + ";\n";
		}
		if (locstring != "")
			file.printf( fh, locstring + "\n" );

	}

	method emitBody()
	{
		if (body != "")
			file.printf( fh, body );
		if ((body != "") && (body[-1] != "\n"))
			file.printf( fh, "\n" );
	}

	method emitTail()
	{
		if ((rettype  != "")     &&
			(rettype  != "void") &&
			(retvalue != ""))
			file.printf( fh, "\n\treturn " + retvalue + ";\n" );
		file.printf( fh, "}\n" );
	}

	/* For derived classes */

	method getFh()
	{
		return fh;
	}

	method getName()
	{
		return name;
	}
}

/*
 * elastiC primitive function
 */

public class ElasticPrimitiveFunction extends Function
{
	local fname;
	local elargs;
	local prefix;
	local rtypename;

	method init( outfh, funcname, ec_funcname, rettypename, px, isstatic )
	{
		[super init outfh, funcname, "EC_OBJ", isstatic];
		[super addArg:   "stack" typed: "EC_OBJ"];
		[super addArg:   "userdata" typed: "EcAny"];

		fname = ec_funcname;
		rtypename = rettypename;
		prefix = px;
		elargs = #[];
	}

	method addArg: name info: cvtinfo
	{
		array.push( elargs, #[name, cvtinfo[0], cvtinfo] );
		local defval = cvtinfo[4], nullok = cvtinfo[5];
		if (defval)
			[super addLocal: name typed: cvtinfo[0] init: string.sub(defval, 1, -1)];
		else if (nullok)
			[super addLocal: name typed: cvtinfo[0] init: "NULL"];
		else
			[super addLocal: name typed: cvtinfo[0]];
	}

	method emit()
	{
		if ((basic.length(elargs) != 0) || rtypename)
			[super addLocal: "res"   typed: "EC_OBJ"];
		[super emit];
	}

	method emitBody()
	{
		[self emitParseStack];
		[super emitBody];
	}

	method emitParseStack()
	{
		local ps_string      = "";
		local ps_args        = #[];
		local ps_args_string = "";

		if (basic.length(elargs) == 0)
			return @nil;

		// determine first optional argument such that all
		// subsequent arguments are optional also
		local arg, argi;
		local defval;
		local i, len = basic.length(elargs);
		local firstopt = len;
		for (i = len-1; i >= 0; i--)
		{
			arg    = elargs[i];
			argi   = arg[2];
			defval = argi[4];

			if (! defval) break;
			firstopt = i;
		}

		local argn, argt, argc, argparms, argpost;
		local nullok;
		local parm;
		local postcode = "";
		i = 0;
		for (arg in elargs)
		{
			argn     = arg[0];
			argt     = arg[1];
			argi     = arg[2];
			argc     = argi[1];
			argparms = argi[2];
			argpost  = argi[3];
			defval   = argi[4];
			nullok   = argi[5];

			if (i == firstopt)
				ps_string = ps_string + "|";

			ps_string = ps_string + argc;
			if (argparms)
			{
				if (basic.typeof(argparms) == #array)
					for (parm in argparms)
						array.push(ps_args, parm);
				else
					array.push(ps_args, argparms);
			} else
			{
				array.push(ps_args, argn);
			}

			if (argpost) {
				if (postcode != "")
					postcode = postcode + "\t";
				postcode = postcode + argpost + "\n";
			}

			i++;
		}

		for (arg in ps_args)
		{
			if (ps_args_string != "")
				ps_args_string = ps_args_string + ", ";
			else
				ps_args_string = ",\n\t                            ";
			ps_args_string = ps_args_string + arg;
		}

		local fh = [self getFh];
		file.printf( fh,
					 "\tres = EcParseStackFunction( \"%s.%s\", TRUE, stack, \"%s\"%s );\n",
					 prefix, fname,
					 ps_string,
					 ps_args_string );
		file.printf( fh,
					 "\tif (EC_ERRORP(res)) return res;\n\n" );
		if (postcode != "")
			file.printf( fh, "\t%s\n", postcode );

		return @nil;
	}
}
