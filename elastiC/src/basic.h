/* ==========================================================================
 * basic.h
 * ==========================================================================
 * Basic data types.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: 28 Mar 1997
 *
 *   $Id: basic.h,v 1.3 2002/06/11 15:08:59 panta Exp $
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

#ifndef __BASIC_H
#define __BASIC_H

/* ==== cnf.h ============================================================= */

#if defined(WIN32) || defined(_WIN32) || defined(_MSC_VER)
	/* Win32 */
#if defined(MINGW32)
#include "win32/cnf.h"
#elif defined(_MSC_VER)
/*
 * In VisualC++ we'll use Custom Build to copy ${top_srcdir}/win32/cnf.h to
 * ${top_srcdir}/elastic/cnf.h
 */
#include "cnf.h"
#elif defined(CYGWIN)
#include "cnf.h"
#else
#error "unsupported win32 compiler"
#endif

#else  /* ! win32 */
	/* autoconf */
#include "cnf.h"
#endif /* ! win32 */

/* ======================================================================== */


/* ==== config.h ========================================================== */

#if defined(WIN32) || defined(_WIN32) || defined(_MSC_VER)
	/* Win32 */
#if defined(MINGW32)
#include "win32/config.h"
#elif defined(_MSC_VER)
/*
 * In VisualC++ we'll use Custom Build to copy ${top_srcdir}/win32/config.h to
 * ${top_srcdir}/elastic/config.h
 */
#include "config.h"
#elif  defined(CYGWIN)
#include "config.h"
#else
#error "unsupported win32 compiler"
#endif

#else  /* ! win32 */
	/* autoconf */
#include "config.h"
#endif /* ! win32 */

/* ======================================================================== */

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_ERRNO_H
#include <errno.h>
#endif

#if ! defined(HAVE_SSIZE_T)
typedef long ssize_t;
#endif

#include <elastic/tsize.h>

#ifndef _GNU_SOURCE
#ifdef __GNUC__
#define _GNU_SOURCE
#endif
#endif

#define EC_USE_CUSTOM_SIZE	0

#if EC_USE_CUSTOM_SIZE
typedef long            EcInt;
typedef unsigned long   EcUInt;
typedef double          EcFloat;
typedef char			EcChar;
typedef unsigned char   EcByte;
typedef unsigned short  EcWord;
typedef unsigned long   EcDWord;
typedef EcInt           EcBool;
typedef void*           EcAny;
#else /* AUTO */

#if   SIZEOF_INT   == 4
typedef int             EcInt;
typedef unsigned int    EcUInt;
#define EC_SIZEOF_ECINT	SIZEOF_INT
#define EC_ECINT_MIN	INT_MIN
#define EC_ECINT_MAX	INT_MAX
#elif SIZEOF_LONG  == 4
typedef long            EcInt;
typedef unsigned long   EcUInt;
#define EC_SIZEOF_ECINT	SIZEOF_LONG
#define EC_ECINT_MIN	LONG_MIN
#define EC_ECINT_MAX	LONG_MAX
#elif SIZEOF_SHORT == 4
typedef short           EcInt;
typedef unsigned short  EcUInt;
#define EC_SIZEOF_ECINT	SIZEOF_SHORT
#define EC_ECINT_MIN	SHRT_MIN
#define EC_ECINT_MAX	SHRT_MAX
#else
#error "can't find an integral type of size 4"
#endif

#if   SIZEOF_FLOAT  == 8
typedef float           EcFloat;
#define EC_SIZEOF_ECFLOAT	SIZEOF_FLOAT
#elif SIZEOF_DOUBLE == 8
typedef double          EcFloat;
#define EC_SIZEOF_ECFLOAT	SIZEOF_DOUBLE
#else
#error "can't find a floating type of size 8"
#endif

#if SIZEOF_CHAR == 1
typedef char            EcChar;
#else
#error "can't find an integral of size 1"
#endif

#if SIZEOF_UNSIGNED_CHAR == 1
typedef unsigned char   EcByte;
#else
#error "can't find an integral of size 1"
#endif

#if   SIZEOF_SHORT == 2
typedef unsigned short  EcWord;
#elif SIZEOF_INT   == 2
typedef unsigned int    EcWord;
#elif SIZEOF_LONG  == 2
typedef unsigned long   EcWord;
#else
#error "can't find an integral type of size 2"
#endif

#if   SIZEOF_SHORT == 4
typedef unsigned short  EcDWord;
#elif SIZEOF_INT   == 4
typedef unsigned int    EcDWord;
#elif SIZEOF_LONG  == 4
typedef unsigned long   EcDWord;
#else
#error "can't find an integral type of size 4"
#endif

typedef EcInt           EcBool;

/*
 * EcAny must be able to contain a void* or a long
 */

#if   (SIZEOF_VOID_P >= 4)  && (SIZEOF_VOID_P >= SIZEOF_LONG)
typedef void*           EcAny;
#define EC_SIZEOF_ECANY	SIZEOF_VOID_P
#elif (SIZEOF_INT    >= 4)  && (SIZEOF_INT >= SIZEOF_VOID_P) && (SIZEOF_INT >= SIZEOF_LONG)
typedef unsigned int    EcAny;
#define EC_SIZEOF_ECANY	SIZEOF_INT
#elif (SIZEOF_LONG    >= 4) && (SIZEOF_LONG >= SIZEOF_VOID_P)
typedef unsigned long   EcAny;
#define EC_SIZEOF_ECANY	SIZEOF_LONG
#else
#error "can't find an integral/pointer type big enough for EcAny"
#endif

/*
 * EcPointerInteger is an unsigned integer big enough to contain a pointer
 */

#if   (SIZEOF_INT >= SIZEOF_VOID_P)
typedef unsigned int  EcPointerInteger;
#define EC_SIZEOF_ECPOINTERINTEGER	SIZEOF_INT
#elif (SIZEOF_LONG >= SIZEOF_VOID_P)
typedef unsigned long EcPointerInteger;
#define EC_SIZEOF_ECPOINTERINTEGER	SIZEOF_LONG
#else
#error "can't find an integral type big enough to contain a pointer"
#endif


#if EC_SIZEOF_ECANY < EC_SIZEOF_ECINT
#error "sizeof(EcAny) < sizeof(EcInt)"
#endif

#if EC_SIZEOF_ECANY < SIZEOF_LONG
#error "sizeof(EcAny) < sizeof(long)"
#endif

#if EC_SIZEOF_ECANY < SIZEOF_VOID_P
#error "sizeof(EcAny) < sizeof(void *)"
#endif

#if EC_SIZEOF_ECPOINTERINTEGER < EC_SIZEOF_ECANY
#error "sizeof(EcPointerInteger) < sizeof(EcAny)"
#endif

#endif /* AUTO */

typedef enum
{
	ec_type_none,												/* no type        */
	ec_type_char,												/* char           */
	ec_type_schar,												/* signed char    */
	ec_type_uchar,												/* unsigned char  */
	ec_type_short,												/* short          */
	ec_type_ushort,												/* unsigned short */
	ec_type_int,												/* int            */
	ec_type_uint,												/* unsigned int   */
	ec_type_long,												/* long           */
	ec_type_ulong,												/* unsigned long  */
	ec_type_quad,												/* quad_t         */
	ec_type_uquad,												/* uquad_t        */

	ec_type_int8,												/* 8 bit signed integer    */
	ec_type_uint8,												/* 8 bit unsigned integer  */
	ec_type_int16,												/* 16 bit signed integer   */
	ec_type_uint16,												/* 16 bit unsigned integer */
	ec_type_int32,												/* 32 bit signed integer   */
	ec_type_uint32,												/* 32 bit unsigned integer */
	ec_type_int64,												/* 64 bit signed integer   */
	ec_type_uint64,												/* 64 bit unsigned integer */

	ec_type_float,												/* float                   */
	ec_type_double,												/* double                  */
	ec_type_longdouble,											/* long double             */

	ec_type_charp,												/* char *                  */

	ec_type_ecbool,												/* EcBool                  */
	ec_type_ecchar,												/* EcChar                  */
	ec_type_ecint,												/* EcInt                   */
	ec_type_ecuint,												/* EcUInt                  */
	ec_type_ecfloat,											/* EcFloat                 */
	ec_type_ecany,												/* EcAny                   */
	ec_type_ecpointerinteger,									/* EcPointerInteger        */
	ec_type_ecstring,											/* ec_string *             */
	ec_type_ecobj,												/* EC_OBJ                  */

	ec_type_voidp												/* void *                  */
} ec_type;

#define EcFalse 0
#define EcTrue  1

#ifdef _GNU_SOURCE
#define EC_UNUSED		__attribute__ ((unused))
#else
#define EC_UNUSED
#endif

/* This could cause problems... */

/* typedef char BOOL; */

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* Symbol exporting issues */

#ifdef WIN32
#define EC_APIEXPORT __declspec(dllexport)
#define EC_APIIMPORT __declspec(dllimport)
#if ELASTIC_EXPORTS
#define EC_API EC_APIEXPORT
#else
#define EC_API EC_APIIMPORT
#endif
#else
#define EC_APIEXPORT
#define EC_APIIMPORT
#define EC_API
#endif

/* C++ compatibility */

#undef EC_BEGIN_DECLS
#undef EC_END_DECLS
#ifdef __cplusplus
#define EC_BEGIN_DECLS extern "C" {
#define EC_END_DECLS   }
#else
#define EC_BEGIN_DECLS /* empty */
#define EC_END_DECLS   /* empty */
#endif

#endif /* __BASIC_H */
