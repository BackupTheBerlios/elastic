/* ==========================================================================
 * debug.h
 * ==========================================================================
 * Debugging facilities.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: 28 Mar 1997
 *
 *   $Id: debug.h,v 1.2 2005/03/03 23:51:43 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1997-2001 Marco Pantaleoni. All rights reserved.
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

#ifndef __DEBUG_H
#define __DEBUG_H

#include <elastic/private.h>

EC_BEGIN_DECLS

/* #undef EC_DEBUG */

#ifdef NDEBUG
#undef EC_DEBUG
#endif

#if defined(_GNU_SOURCE) && (! defined(__STRICT_ANSI__))
#define EC_HAS_VARARGS_MACRO 1
#endif

#ifdef EC_DEBUG

/*
#include <PalmOS.h>
#include <hostcontrol.h>
*/
#include <stdio.h>
#include <stdlib.h>

#define EC_PRINTFUNC \
do { \
	fprintf(stderr, "In function: %s (%s, %5d)\n", __PRETTY_FUNCTION__, __FILE__, __LINE__); \
	fflush(stderr); \
} while(0)

#define EC_DEBMESSAGE(condVar, A) \
do { \
    if ((condVar)) \
	{ \
		EC_PRINTFUNC; \
		fprintf(stderr, "\tMessage: %s\n\n", (A)); \
		fflush(stderr); \
	} \
} while(0)

#define EC_WARN_IF(condVar, A) \
do { \
	if ((condVar) && (A)) { \
		EC_PRINTFUNC; \
		fprintf(stderr, "\tWarning: %s is TRUE\n\n", #A);	\
		fflush(stderr); \
	} \
} while(0)

#if defined (ELASTIC_CORE) && defined (__PRIVATE_H)
#define EC_ASSERT(A) \
do { \
	if (!(A)) { \
		EC_PRINTFUNC; \
		fprintf(stderr, "\tAssertion failed: %s is false\n", #A);	\
		if (PRIVATE(rt).line_num > 0)	\
			fprintf(stderr, "\t(Interpreted program line %ld)\n", ((long)PRIVATE(rt).line_num));	\
		fprintf(stderr, "\n");	\
		fflush(stderr); \
		abort(); \
	} \
} while(0)
#else /* ! (ELASTIC_CORE && __PRIVATE_H) */
#define EC_ASSERT(A) \
do { \
	if (!(A)) { \
		EC_PRINTFUNC; \
		fprintf(stderr, "\tAssertion failed: %s is false\n\n", #A);	\
		fflush(stderr); \
		abort(); \
	} \
} while(0)
#endif /* ! (ELASTIC_CORE && __PRIVATE_H) */

#if EC_HAS_VARARGS_MACRO
#define EC_VADEBMESSAGE(condVar, format, args...) \
do { \
    if ((condVar)) \
	{ \
		  EC_PRINTFUNC; \
		  fprintf(stderr, "\tMessage: ");	\
		  fprintf(stderr, format, ## args); \
		  fprintf(stderr, "\n\n"); \
		  fflush(stderr); \
	} \
} while(0)
#else
#define EC_VADEBMESSAGE	do { } while (0)
#endif /* EC_HAS_VARARGS_MACRO */

#if defined(_GNU_SOURCE)
#define EC_INFUNC \
do { \
	fprintf(stderr, "%s entered\n\n", __PRETTY_FUNCTION__); \
	fflush(stderr); \
} while(0)

#define EC_OUTFUNC \
do { \
	fprintf(stderr, "%s left\n\n", __PRETTY_FUNCTION__); \
	fflush(stderr); \
} while(0)
#endif /* _GNU_SOURCE */

#else /* EC_DEBUG not defined */

#define EC_PRINTFUNC               do { } while(0)
#define EC_DEBMESSAGE(condVar, A)  do { } while(0)
#define EC_WARN_IF(condVar, A)     do { } while(0)
#define EC_ASSERT(A)               do { } while(0)
#define EC_INFUNC                  do { } while(0)
#define EC_OUTFUNC                 do { } while(0)

#if defined(_GNU_SOURCE) && (! defined(__STRICT_ANSI__))
#define EC_VADEBMESSAGE(condVar, format, args...) do { } while(0)
#else
#define EC_VADEBMESSAGE do { } while(0)
#endif /* _GNU_SOURCE && (! __STRICT_ANSI__) */

#endif /* EC_DEBUG */

#ifndef EC_PREFIX_DEBUG
#define PRINTFUNC								EC_PRINTFUNC
#define DEBMESSAGE								EC_DEBMESSAGE
#define WARN_IF									EC_WARN_IF
#define ASSERT									EC_ASSERT
#define INFUNC									EC_INFUNC
#define OUTFUNC									EC_OUTFUNC

#define VADEBMESSAGE							EC_VADEBMESSAGE
#endif /* EC_PREFIX_DEBUG */

EC_END_DECLS

#endif /* __DEBUG_H */
