/* ==========================================================================
 * stockticker.ec
 * ==========================================================================
 * Fetch trade price for a stock though HTTP.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat Jan  1 23:00:53 MET 2000
 *
 *   $Id: stockticker.ec,v 1.1 2002/05/30 13:23:17 panta Exp $
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

from basic import *;
import sys;
import string;
import re;
import file;

import geturl;

private serviceURL = "http://www.newsalert.com/free/stocknews?Symbol=";

private function main( ticker )
{
	local response = geturl.geturl( serviceURL + ticker );
	local match = re.match( "arts\\?Symbol=" + string.toupper( ticker ) + "\">(.*?)<", response );
	if (match)
		print( re.group( match, 1 ), "\n" );
	else
		print( "NOT FOUND\n" );
}

if (length(sys.args) != 1)
{
	file.printf( file.stderr, "Usage: stockticker TICKER\n" );
	exit( @false );
}
main( sys.args[0] );
