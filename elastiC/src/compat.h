/* ==========================================================================
 * compat.h
 * ==========================================================================
 * Compatibility stuff.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat Jan 22 16:05:38 MET 2000
 *
 *   $Id: compat.h,v 1.1 2002/05/23 21:16:43 panta Exp $
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

#ifndef __COMPAT_H
#define __COMPAT_H

#include <stdlib.h>

#include "basic.h"
#include "private.h"

/* Non standard header files */

/* for alloca() */
#if HAVE_MALLOC_H
#include <malloc.h>
#endif

#if HAVE_STRING_H
#include <string.h>
#endif

#if HAVE_STRINGS_H
#include <strings.h>
#endif

#if HAVE_SYS_TIME_H
#  if TIME_WITH_SYS_TIME
#    include <sys/time.h>
#  endif
#endif

#if HAVE_SYS_TIMES_H
#include <sys/times.h>
#endif

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#if HAVE_FCNTL_H
#include <fcntl.h>
#endif

#if HAVE_DIRECT_H
#include <direct.h>
#endif

#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#if HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#if HAVE_VALUES_H
#include <values.h>
#endif

#if HAVE_LIMITS_H
#include <limits.h>
#endif

#if WIN32
#  include <stdarg.h>
#else
#  if __STDC__
#    include <stdarg.h>
#  else
#    include <varargs.h>
#  endif
#endif

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

EC_BEGIN_DECLS

/* Prototypes */

	/* alloca */
#if C_ALLOCA
void *alloca(size_t size);
#endif

	/* ntoh* hton* */
#if ! HAVE_NETINET_IN_H
#if WORDS_BIGENDIAN
#define ntohl(x)	(x)
#define ntohs(x)	(x)
#define htonl(x)	(x)
#define htohs(x)	(x)
#else
EcDWord ntohl(EcDWord netlong);
EcWord  ntohs(EcWord  netshort);
EcDWord htonl(EcDWord hostlong);
EcWord  htons(EcWord  hostshort);
#endif
#endif

	/* strcasecmp */
#if ! HAVE_STRCASECMP
int strcasecmp(const char *s1, const char *s2);
int strncasecmp(const char *s1, const char *s2, size_t n);
#endif

	/* index */
#if ! HAVE_INDEX
char *index(const char *s, int c);
#endif

	/* rindex */
#if ! HAVE_RINDEX
char *rindex(const char *s, int c);
#endif

	/* getenv */
#if ! HAVE_GETENV
char *getenv(const char *name);
#endif

	/* setenv */
#if ! HAVE_SETENV
int setenv(const char *name, const char *value, int);
#endif

	/* unsetenv */
#if ! HAVE_UNSETENV
void unsetenv(const char *name);
#endif

#if ! HAVE_ISINF
#define isinf(fpval)	(0)
#endif

#if ! HAVE_ISNAN
#define isnan(fpval)	(0)
#endif

	/* strtol */
#if ! HAVE_STRTOL
long strtol(const char *nptr, char **endptr, int base);
#endif

	/* strtoul */
#if ! HAVE_STRTOUL
unsigned long strtoul(const char *nptr, char **endptr, int base);
#endif

EC_END_DECLS

#endif /* __COMPAT_H */
