/* ==========================================================================
 * geturl.c
 * ==========================================================================
 * Fetch a resource given URL.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat Jan  1 22:02:08 MET 2000
 *
 *   $Id: geturl.ec,v 1.1 2002/05/30 13:23:09 panta Exp $
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

package geturl;

from basic import *;
import sys;
import netfile;
import file;
import string;

public function geturl(url)
{
	local host, proxy;
	local resource;
	local target;
	local fh;

	local split_url = string.split(url, "/", 4);
	host     = split_url[2];
	resource = split_url[3];

	proxy = sys.getenv("http_proxy");
	target = proxy ? proxy : host;

	fh = file.open( "tcp://" +
					target   +
					((string.find(target, ":") != @false) ? "" : ":http"), "r+" );
	local request =
		"GET " + (proxy ? proxy : "/" + resource) + " HTTP/1.0\r\n" +
		"Host: " + host + "\r\n" +
		"User-agent: elastiC-geturl/0.1\r\n" +
		"\r\n";
	file.write( fh, request );
	file.flush( fh );

	local res = "", line;
	while (line = file.gets( fh ))
		res = res + line;
	file.close( fh );
	return res;
}
