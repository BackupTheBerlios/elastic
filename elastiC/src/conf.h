/* ==========================================================================
 * conf.h
 * ==========================================================================
 * elastiC settings.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sun Jan 17 11:07:49 MET 1999
 *
 *   $Id: conf.h,v 1.2 2002/05/24 17:28:10 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1999-2002 Marco Pantaleoni. All rights reserved.
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

#ifndef __CONF_H
#define __CONF_H

/* Configuration variables */

#define EC_ELASTICLIBVAR	"ELASTICPATH"
#ifndef WIN32	/* UNIX */
#define EC_PATHSEPARATOR	"/"
#define EC_ENVSEPARATOR		":"
#define EC_CWD				"."
#else			/* WIN32 */
#define EC_PATHSEPARATOR	"\\"
#define EC_ENVSEPARATOR		";"
#define EC_CWD				"."
#endif

#define EC_SOURCESUFFIX		".ec"
#define EC_COMPILEDSUFFIX	".ecc"

#define EC_NOTIFY_GC			0
#define EC_VERBOSE_COMPILATION	0
#define EC_DEBUG_COMPILATION	0
#define EC_DEBUG_PACKAGEIO		0

#define EC_MAXSYMBOLNESTING 64

/* #define EC_VARARG_UNPROMOTED 0 */

#define EC_STACK_USES_STATIC	1
#define EC_STACK_STATIC_SIZE	4

#define EC_STACK_RECYCLE		1
#define EC_STACK_POOL_SIZE		128
/* #define EC_STACK_POOL_SIZE		256 */

#if EC_STACK_RECYCLE
#define EC_STACK_RECYCLE_STATS				0
#define EC_STACK_RECYCLE_PARANOID_CHECKS	1
#define EC_STACK_RECYCLE_DEBUG_MSGS			0
#else
#define EC_STACK_RECYCLE_STATS				0
#define EC_STACK_RECYCLE_PARANOID_CHECKS	0
#define EC_STACK_RECYCLE_DEBUG_MSGS			0
#endif

#define EC_FORCE_LIBC 0

/* Define this to one if you want unnecessary initializations, ... */
#define EC_PARANOID_CODE	0

/*
 * EC_DIRTY_MALLOC
 *
 * Define this to one if you want ec_malloc() & friends to initialize
 * memory to a specific pattern (0xdeadbeef) after (de)allocation.
 * Useful to detect strange missing initialization problems.
 *
 * NOTE: This value has effect only when EC_DEBUG is defined.
 */
#define EC_DIRTY_MALLOC		1
#define EC_DIRTY_PATTERN	0xdeadbeef


/* Private */

#ifdef EC_DEBUG
#define EC_STACKCHECK_ENABLED 1
#define EC_ARRAYCHECK_ENABLED 1
#else
#define EC_STACKCHECK_ENABLED 0
#define EC_ARRAYCHECK_ENABLED 0
#endif


/* System specific include files */

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#if HAVE_MALLOC_H
#include <malloc.h>
#endif

#endif /* __CONF_H */
