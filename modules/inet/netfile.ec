/* ==========================================================================
 * netfile.ec
 * ==========================================================================
 * Package providing transparent network access trough file package hooks.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat Jan 01 2000
 *
 *   This module allows to use the standard file package to access network
 *   streams. It is sufficient to open a pseudo-file, with
 *
 *      file.open( NETNAME )
 *
 *   where NETNAME has one of the following formats:
 *      "tcp://address"
 *      "tcp://address:port"
 *      "tcp://address:service"
 *   (service could be smtp, http, ...)
 *   The returned file handle then can be used as a regular file.
 *   This module was inspired by an analogous facility described by
 *   oleg@pobox.com (but I don't use his method, too OS dependent). See:
 *      http://pobox.com/~oleg/ftp/syscall-interpose.html
 *
 *   $Id: netfile.ec,v 1.1 2002/05/30 13:23:13 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 2000 Marco Pantaleoni. All rights reserved.
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

package netfile;

from basic import *;
import file;
import inet;
import string;

private oldopen = file.open, oldclose = file.close;

private function newopen(args ...)
{
	local filename, mode;

	local arglen = length(args);
	if (arglen >= 1)
		filename = args[0];
	if (arglen >= 2)
		mode = args[1];
	if ((arglen <= 0) || (arglen > 2))
	{
		local expected;
		if (arglen < 1)
			expected = 1;
		else
			expected = 2;
		throw [ParameterCountError new
								   sprintf( "Wrong # of parameters to file.open (got %d, expected %d)", arglen, expected ),
								   @nil, file.open, expected, args];
	}

	if (string.sub(filename, 0, 6) == "tcp://")
	{
		local addr, portOrService;
		local sep;

		filename = string.sub(filename, 6);
		sep = string.find(filename, ":");
		if (sep == @false)
			throw [IOError new "bad tcp filename format", @nil];
		addr          = string.sub(filename, 0, sep);
		portOrService = string.sub(filename, sep + 1);
		try {
			local numport = basic.int(portOrService);
			portOrService = numport;
		} catch (TypeError) { }
		return inet.open(addr, portOrService);
	}
	else
		return oldopen(filename, mode);
}

private function newclose(fh)
{
	return oldclose(fh);
}

file.open  = newopen;
file.close = newclose;
