/* ==========================================================================
 * compat.c
 * ==========================================================================
 * Compatibility stuff.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat Jan 22 16:05:38 MET 2000
 *
 *   $Id: compat.c,v 1.1 2002/05/23 21:16:43 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 2000-2001 Marco Pantaleoni. All rights reserved.
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

#include "basic.h"
#include "private.h"
#include "compat.h"

	/* ntoh* hton* */
#if ! HAVE_NETINET_IN_H
#if ! WORDS_BIGENDIAN
inline EcDWord invert_l(EcDWord vlong)
{
	EcDWord b1, b2, b3, b4;

	b1 = (vlong & 0xFF000000) >> 24;
	b2 = (vlong & 0x00FF0000) >> 8;
	b3 = (vlong & 0x0000FF00) << 8;
	b4 = (vlong & 0x000000FF) << 24;

	return (b1 | b2 | b3 | b4);
}

inline EcWord invert_s(EcWord vshort)
{
	EcWord b1, b2;

	b1 = (vshort & 0xFF00) >> 8;
	b2 = (vshort & 0x00FF) << 8;

	return (b1 | b2);
}

EcDWord ntohl(EcDWord netlong)
{
	return invert_l(netlong);
}

EcWord ntohs(EcWord netshort)
{
	return invert_s(netshort);
}

EcDWord htonl(EcDWord hostlong)
{
	return invert_l(hostlong);
}

EcWord htons(EcWord hostshort)
{
	return invert_s(hostshort);
}
#endif
#endif

	/* strcasecmp */
#if ! HAVE_STRCASECMP
#include "compat/strcasecmp.c"
#endif

	/* index */
#if ! HAVE_INDEX
#include "compat/index.c"
#endif

	/* rindex */
#if ! HAVE_RINDEX
#include "compat/rindex.c"
#endif

	/* getenv / setenv */
#if (! HAVE_GETENV) || (! HAVE_SETENV) || (! HAVE_UNSETENV)
#include "compat/getsetenv.c"
#endif

	/* strtol */
#if ! HAVE_STRTOL
#include "compat/strtol.c"
#endif

	/* strtoul */
#if ! HAVE_STRTOUL
#include "compat/strtoul.c"
#endif
