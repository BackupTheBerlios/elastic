/* ==========================================================================
 * scmparse.ec
 * ==========================================================================
 * Simple scheme parser for elastiC.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Aug 1999
 *
 *   $Id: scmparse.ec,v 1.1 2002/05/31 15:14:13 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1999-2001 Marco Pantaleoni. All rights reserved.
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
 * This package parses files in a simplified Scheme dialect,
 * constructing corresponding elastiC data structures.
 * Scheme lists are translated to elastiC arrays, strings to strings, ...
 */

package scmparse;

from   basic  import *;
import string;
import file;
import sys;
import array;

private class Stack extends Object
{
	local st, sp;

	method init ( ) {
		st = #[];
		sp = 0;
	}

	method _getitem( i ) {
		return st[i];
	}

	method _setitem( i, v ) {
		st[i] = v;
		return self;
	}

	method _length( ) {
		return array.length( st );
	}

	method push: el {
		array.push( st, el );
		return self;
	}

	method pop( ) {
		return array.pop( st );
	}
}

private function tail( aString, start )
{
	/* Return the tail of `aString' starting at `start' */

	if (start == length(aString))
		return "";
	return string.sub( aString, start, length( aString ) - start );
}

private digits = "0123456789";

public function parse( fh )
{
	local stack, line;
	local closed, pos, str;
	local level = 0;

	stack = [Stack new];
	[stack push: #[]];

	line = file.gets( fh );
	if (line == @nil)
		return @nil;											// EOF
	while (line && (line != ""))
	{
		//print( "LINE: \"", line, "\"\n" );
		while (line && (line != ""))
		{
			line = string.ltrim( line );
			if (line == "")
				break;
			else if (line[0] == '(') {
				level++;
				[stack push: #[]];
				line = tail( line, 1 );
			} else if (line[0] == ')') {
				level--;
				closed = [stack pop];
				stack[-1] = stack[-1] + #[ closed ];
				line = tail( line, 1 );
				if (level == 0)
					break 2;
			} else if (line[0] == '"') {
				pos = string.find( tail( line, 1 ), "\"" );
				stack[-1] = stack[-1] + #[ string.sub( line, 0, pos+2 ) ];
				line = tail( line, pos+2 );
			} else if (line[0] in digits) {
				str = "";
				while (line && (line != "") && (line[0] in "0123456789+-.")) {
					str = str + line[0];
					line = tail( line, 1 );
				}
				stack[-1] = stack[-1] + #[ basic.float( str ) ];
			} else if (line[0] == ';')
			{
				break;
			}
			else {
				str = "";
				while (line && (line != "") && (! (line[0] in " \t();\n"))) {
					str = str + line[0];
					line = tail( line, 1 );
				}
				stack[-1] = stack[-1] + #[ str ];
			}
		}

		line = file.gets( fh );
		if (line == @nil)
			return @nil;										// EOF
	}

	if (length( stack ) != 1) {
		print( "parentheses don't match\n" );
		return @nil;
	}
	return [stack pop][0];
}

public function parsefile( fh )
{
	local stack, expr;

	stack = [Stack new];

	while ((expr = parse( fh )) != @nil)
		[stack push: expr];

	return stack;
}

private function test( file )
{
	local fh;
	local statements;

	print( sprintf( "LOADING %r...\n", file ) );
	fh = file.open( file );
	statements = parsefile( fh );
	file.close( fh );

	local s;
	for (s in statements)
		print( s, "\n" );
}

if ((length(sys.args) == 2) &&
	(sys.args[0] == "test"))
	test( sys.args[1] );
