/* ==========================================================================
 * main.ec
 * ==========================================================================
 * Program entry point.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created : Fri Nov  2 19:11:30 CET 2001
 *
 *   $Id: main.ec,v 1.1 2002/05/31 15:14:14 panta Exp $
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

package main;

from basic import *;
import string;
import array;
import hash;
import file;
import sys;

import IDLModel;
import scmparse;

private function usage()
{
	print("Usage: ecidl SPEC-FILE\n");
	exit(#false);
}

private function main()
{
	local specfile;

	if (length(sys.args) == 1)
		specfile = sys.args[0];
	else
		usage();

	// Parse IDL file, obtaining a list of IDLModule instances
	local fh;
	local scmdata, module_list;
	print("Parsing IDL\n");
	fh = file.open(specfile);
	scmdata = scmparse.parsefile( fh );
	file.close( fh );
	module_list = IDLModel.idl_process_spec( scmdata );

	// Visit the module list, generating glue code
	// :TODO:
}

main();
