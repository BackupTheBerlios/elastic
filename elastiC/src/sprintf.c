/* ==========================================================================
 * sprintf.c
 * ==========================================================================
 * elastiC printf-like support.
 * Partially derived from source code in BSD.
 * See below for original licensing.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created            : 1999
 *   Second modification: 15 Jan 1999
 *   Third modification : Tue Oct 23 12:24:57 CEST 2001
 *
 *   $Id: sprintf.c,v 1.3 2002/06/13 17:44:18 panta Exp $
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

/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * NOTE: As stated in
 *  ftp://ftp.cs.berkeley.edu/pub/4bsd/README.Impt.License.Change
 * the so called `advertising clause' in the above BSD license doesn't
 * apply anymore.
 */

/*
 * TODO:
 *  1. handle some error conditions more gracefully
 *  2. raise exceptions where needed
 *  3. implement conversions for base 2 integers
 */

#include "basic.h"
#include "elastic.h"
#include "dstring.h"
#include "private.h"
#include "compat.h"

/*
 * Actual printf innards.
 *
 * This code is large and complicated...
 */

#if HAVE_STDIO_H
#include <stdio.h>
#endif
#if HAVE_STDLIB_H
#include <stdlib.h>
#endif


EC_API EcInt ec_sprintf( ec_string *ds, const char *fmt, ... )
{
	va_list args;
	EcInt i;

	va_start( args, fmt );
	i = ec_vsprintf( ds, fmt, args );
	va_end( args );
	return i;
}


/* Define FLOATING_POINT to get floating point. */
#define	FLOATING_POINT

#if (HAVE_QUAD_T && HAVE_U_QUAD_T)

#define QUAD_T		quad_t
#define U_QUAD_T	u_quad_t

#if (defined(__GNUC__) && defined(__STRICT_ANSI__))
#ifndef QUAD_MAX
#define QUAD_MAX LONG_MAX
#endif
#else /* ! (__GNUC__ && __STRICT_ANSI__) */
#ifndef QUAD_MAX
#ifdef LONG_LONG_MAX
#define QUAD_MAX LONG_LONG_MAX
#else
#define QUAD_MAX LONG_MAX
#endif
#endif
#endif /* ! (__GNUC__ && __STRICT_ANSI__) */

#else /* not (HAVE_QUAD_T && HAVE_U_QUAD_T) */

#if defined(__GNUC__) && (! defined(__STRICT_ANSI__))
/* GNU has `long long' */
#define QUAD_T		long long
#define U_QUAD_T	unsigned long long

#ifndef QUAD_MAX
#ifdef LONG_LONG_MAX
#define QUAD_MAX LONG_LONG_MAX
#else
#define QUAD_MAX LONG_MAX
#endif
#endif

#else /* not (__GNUC__ && (! __STRICT_ANSI__)) */
#define QUAD_T		long
#define U_QUAD_T	unsigned long
#define QUAD_MAX	LONG_MAX
#endif /* not (__GNUC__ && (! __STRICT_ANSI__)) */

#endif /* not (HAVE_QUAD_T && HAVE_U_QUAD_T) */

typedef struct
{
	unsigned char type_id;										/* T_UNUSED, T_SHORT, ... */
	union {
		int             v_short;								/* T_SHORT        */
		unsigned int    v_u_short;								/* T_U_SHORT      */
		short          *v_shortp;								/* TP_SHORT       */

		int             v_int;									/* T_INT          */
		unsigned int    v_u_int;								/* T_U_INT        */
		int            *v_intp;									/* TP_INT         */

		long            v_long;									/* T_LONG         */
		unsigned long   v_u_long;								/* T_U_LONG       */
		long           *v_longp;								/* TP_LONG        */

		QUAD_T          v_quad;									/* T_QUAD         */
		U_QUAD_T        v_u_quad;								/* T_U_QUAD       */
		QUAD_T         *v_quadp;								/* TP_QUAD        */

		double          v_double;								/* T_DOUBLE       */
		long double     v_long_double;							/* T_LONG_DOUBLE  */

		char           *v_charp;								/* TP_CHAR        */
		void           *v_voidp;								/* TP_VOID        */

		EcBool          v_ecbool;								/* T_ECBOOL       */
		EcBool         *v_ecboolp;								/* TP_ECBOOL      */
		EcInt           v_ecint;								/* T_ECINT        */
		EcUInt          v_ecuint;								/* T_ECUINT       */
		EcInt          *v_ecintp;								/* TP_ECINT       */
		EC_OBJ          v_ecobj;								/* T_ECOBJ        */
		EC_OBJ         *v_ecobjp;								/* TP_ECOBJ       */
		ec_string      *v_ecstringp;							/* TP_ECSTRING    */
	} v;
	void *addr;
} arg_entry_t;

#define STATIC_ARG_TBL_SIZE 8									/* Size of static argument table. */

typedef struct
{
	int          size;											/* arg table size */
	arg_entry_t *arg;

	arg_entry_t  stattable[STATIC_ARG_TBL_SIZE];				/* static storage */
} argtable_t;

static char *__ultoa (unsigned long, char *, int, int, char *);
static char *__uqtoa (U_QUAD_T, char *, int, int, char *);
static void	 __find_arguments (const char *fmt0, va_list ap, argtable_t *argtable);

static void  __argtable_init(argtable_t *table);
static void  __argtable_cleanup(argtable_t *table);
static int   __argtable_expand(argtable_t *table, int nextarg);


/*
 * Macros for converting digits to letters and vice versa
 */
#define	to_digit(c)	((c) - '0')
#define is_digit(c)	((unsigned)to_digit(c) <= 9)
#define	to_char(n)	((n) + '0')

/*
 * Convert an unsigned long to ASCII for printf purposes, returning
 * a pointer to the first character of the string representation.
 * Octal numbers can be forced to have a leading zero; hex numbers
 * use the given digits.
 */
static char *__ultoa(unsigned long val, char *endp, int base, int octzero, char *xdigs)
{
	register char *cp = endp;
	register long sval;

	/*
	 * Handle the three cases separately, in the hope of getting
	 * better/faster code.
	 */
	switch (base) {
	case 10:
		if (val < 10) {	/* many numbers are 1 digit */
			*--cp = to_char(val);
			return (cp);
		}
		/*
		 * On many machines, unsigned arithmetic is harder than
		 * signed arithmetic, so we do at most one unsigned mod and
		 * divide; this is sufficient to reduce the range of
		 * the incoming value to where signed arithmetic works.
		 */
		if (val > LONG_MAX) {
			*--cp = to_char(val % 10);
			sval = val / 10;
		} else
			sval = val;
		do {
			*--cp = to_char(sval % 10);
			sval /= 10;
		} while (sval != 0);
		break;

	case 8:
		do {
			*--cp = to_char(val & 7);
			val >>= 3;
		} while (val);
		if (octzero && *cp != '0')
			*--cp = '0';
		break;

	case 16:
		do {
			*--cp = xdigs[val & 15];
			val >>= 4;
		} while (val);
		break;

	default:			/* oops */
		abort();												/* TODO XXX: handle this case gracefully */
	}
	return (cp);
}

/* Identical to __ultoa, but for quads. */
static char *__uqtoa(U_QUAD_T val, char *endp, int base, int octzero, char *xdigs)
{
	char *cp = endp;
	QUAD_T sval;

	/* quick test for small values; __ultoa is typically much faster */
	/* (perhaps instead we should run until small, then call __ultoa?) */
	if (val <= ULONG_MAX)
		return (__ultoa((unsigned long)val, endp, base, octzero, xdigs));
	switch (base) {
	case 10:
		if (val < 10) {
			*--cp = to_char(val % 10);
			return (cp);
		}
		if (val > QUAD_MAX) {
			*--cp = to_char(val % 10);
			sval = val / 10;
		} else
			sval = val;
		do {
			*--cp = to_char(sval % 10);
			sval /= 10;
		} while (sval != 0);
		break;

	case 8:
		do {
			*--cp = to_char(val & 7);
			val >>= 3;
		} while (val);
		if (octzero && *cp != '0')
			*--cp = '0';
		break;

	case 16:
		do {
			*--cp = xdigs[val & 15];
			val >>= 4;
		} while (val);
		break;

	default:
		abort();												/* TODO XXX: handle this case gracefully */
	}
	return (cp);
}

#ifdef FLOATING_POINT
#include <math.h>
#include "floatio.h"

#define	BUF		(MAXEXP+MAXFRACT+1)	/* + decimal point */
#define	DEFPREC		6

static char *cvt (double, int, int, char *, int *, int, int *, char **);
static int   exponent (char *, int, int);

#else /* no FLOATING_POINT */

#define	BUF		68

#endif /* FLOATING_POINT */

/*
 * Flags used during conversion.
 */
#define	ALT			0x001		/* alternate form                 */
#define	HEXPREFIX	0x002		/* add 0x or 0X prefix            */
#define	LADJUST		0x004		/* left adjustment                */
#define	LONGDBL		0x008		/* long double                    */
#define	LONGINT		0x010		/* long integer                   */
#define	QUADINT		0x020		/* quad integer                   */
#define	SHORTINT	0x040		/* short integer                  */
#define	ZEROPAD		0x080		/* zero (as opposed to blank) pad */
#define FPT			0x100		/* Floating point number          */

/*
 * BEWARE, these `goto error' on error, and PAD uses `n'.
 */

#define	PRINT(ptr, len)                   \
do {                                      \
	if (! ec_strcat( ds, (ptr), (len) ))  \
		goto error;                       \
} while (0)

#define	PAD(howmany, with)         \
do {                               \
	if ((n = (howmany)) > 0) {     \
		while (n > PADSIZE) {      \
			PRINT(with, PADSIZE);  \
			n -= PADSIZE;          \
		}                          \
		PRINT(with, n);            \
	}                              \
} while (0)

#define	FLUSH() do { } while (0)

EC_API EcInt ec_vsprintf( ec_string *ds, const char *fmt0, va_list ap )
{
	char *fmt;													/* format string                           */
	int ch;														/* character from fmt                      */
	EcInt n, n2;												/* handy integer (short term usage)        */
	char *cp;													/* handy char pointer (short term usage)   */
	int flags;													/* flags as above                          */
	EcInt ret;													/* return value accumulator                */
	int width;													/* width from format (%8d), or 0           */
	int prec;													/* precision from format (%.3d), or -1     */
	char sign;													/* sign prefix (' ', '+', '-', or \0)      */
#ifdef FLOATING_POINT
	char softsign;												/* temporary negative sign for floats      */
	double _double;												/* double precision arguments %[eEfgG]     */
	int expt;													/* integer value of exponent               */
	int expsize;												/* character count for expstr              */
	int ndig;													/* actual number of digits returned by cvt */
	char expstr[7];												/* buffer for exponent string              */
	char *dtoaresult;											/* buffer allocated by dtoa                */
#endif
	EC_OBJ obj;													/* elastiC object %[wWr]                   */
	EcUInt symid;												/* elastiC symbol %k                       */
	EcBool boolval;												/* boolean value %b                        */
	ec_string *dsval;											/* ec_string value %S                      */
	unsigned long ulval;										/* integer arguments %[diouxX]             */
	U_QUAD_T      uqval;										/* %q integers                             */
	int base;													/* base for [diouxX] conversion            */
	int dprec;													/* a copy of prec if [diouxX], 0 otherwise */
	int realsz;													/* field size expanded by dprec, sign, etc */
	int size;													/* size of converted field or string       */
	EcInt prsize;												/* max size of printed field               */
	char *xdigs;												/* digits for [xX] conversion              */
	char buf[BUF];												/* space for %c, %[diouxX], %[eEfgG]       */
	char ox[2];													/* space for 0x hex-prefix                 */
		argtable_t argtable;									/* args, built due to positional arg       */
		int using_argtable;										/* true if we are using argtable           */
        int nextarg;											/* 1-based argument index                  */
        va_list orgap;											/* original argument pointer               */

	ec_string subds;

	/*
	 * Choose PADSIZE to trade efficiency vs. size.  If larger printf
	 * fields occur frequently, increase PADSIZE and make the initialisers
	 * below longer.
	 */
#define	PADSIZE	16		/* pad chunk size */
	static char blanks[PADSIZE] =
	 {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
	static char zeroes[PADSIZE] =
	 {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};

	ec_string_init( &subds, NULL );

	/*
	 * Get the argument indexed by nextarg.   If the argument table is
	 * built, use it to get the argument.  If its not, get the next
	 * argument (and arguments must be gotten sequentially).
	 */
#undef GETARG
#define GETARG(type) \
        ((using_argtable) ? *((type*)(argtable.arg[nextarg++].addr)) : \
            (nextarg++, va_arg(ap, type)))

	/*
	 * To extend shorts properly, we need both signed and unsigned
	 * argument extraction methods.
	 */
#undef SARG
#define	SARG() \
	(flags&LONGINT ? GETARG(long) : \
	    flags&SHORTINT ? (long)(short)GETARG(int) : \
	    (long)GETARG(int))
#undef UARG
#define	UARG() \
	(flags&LONGINT ? GETARG(unsigned long) : \
	    flags&SHORTINT ? (unsigned long)(unsigned short)GETARG(int) : \
	    (unsigned long)GETARG(unsigned int))

	/*
	 * Get * arguments, including the form *nn$.  Preserve the nextarg
	 * that the argument can be gotten once the type is determined.
	 */
#undef GETASTER
#define GETASTER(val) \
	do {                                                     \
        n2 = 0;                                              \
        cp = fmt;                                            \
        while (is_digit(*cp)) {                              \
			n2 = 10 * n2 + to_digit(*cp);                    \
			cp++;                                            \
        }                                                    \
        if (*cp == '$') {                                    \
			int hold = nextarg;                              \
			if (! using_argtable) {                          \
				__find_arguments (fmt0, orgap, &argtable);   \
				using_argtable = 1;                          \
			}                                                \
			nextarg = n2;                                    \
			val = GETARG (int);                              \
			nextarg = hold;                                  \
			fmt = ++cp;                                      \
        } else {                                             \
			val = GETARG (int);                              \
        }                                                    \
	} while (0)

#ifdef FLOATING_POINT
	dtoaresult = NULL;
#endif

	fmt = (char *)fmt0;
		__argtable_init(&argtable);
		using_argtable = 0;
        nextarg = 1;
        EC_VA_COPY(orgap, ap);
	ret = (EcInt) 0;

	/*
	 * Scan the format for conversions (`%' character).
	 */
	for (;;) {
		ulval = 0;
		uqval = 0;
		xdigs = "0123456789ABCDEF";
#ifdef FLOATING_POINT
		_double = 0.0;
		expsize = 0;
#endif
		for (cp = fmt; (ch = *fmt) != '\0' && ch != '%'; fmt++)
			/* void */;
		if ((n = fmt - cp) != 0) {
			if ((unsigned)ret + n > EC_ECINT_MAX) {
				ret = EOF;
				goto error;
			}
			PRINT(cp, n);
			ret += n;
		}
		if (ch == '\0')
			goto done;
		fmt++;		/* skip over '%' */

		flags = 0;
		dprec = 0;
		width = 0;
		prec = -1;
		sign = '\0';

rflag:		ch = *fmt++;
reswitch:	switch (ch) {
		case ' ':
			/*
			 * ``If the space and + flags both appear, the space
			 * flag will be ignored.''
			 *	-- ANSI X3J11
			 */
			if (!sign)
				sign = ' ';
			goto rflag;
		case '#':
			flags |= ALT;
			goto rflag;
		case '*':
			/*
			 * ``A negative field width argument is taken as a
			 * - flag followed by a positive field width.''
			 *	-- ANSI X3J11
			 * They don't exclude field widths read from args.
			 */
			GETASTER (width);
			if (width >= 0)
				goto rflag;
			width = -width;
			/* FALLTHROUGH */
		case '-':
			flags |= LADJUST;
			goto rflag;
		case '+':
			sign = '+';
			goto rflag;
		case '.':
			if ((ch = *fmt++) == '*') {
				GETASTER (n);
				prec = n < 0 ? -1 : n;
				goto rflag;
			}
			n = 0;
			while (is_digit(ch)) {
				n = 10 * n + to_digit(ch);
				ch = *fmt++;
			}
			prec = n < 0 ? -1 : n;
			goto reswitch;
		case '0':
			/*
			 * ``Note that 0 is taken as a flag, not as the
			 * beginning of a field width.''
			 *	-- ANSI X3J11
			 */
			flags |= ZEROPAD;
			goto rflag;
		case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			n = 0;
			do {
				n = 10 * n + to_digit(ch);
				ch = *fmt++;
			} while (is_digit(ch));
			if (ch == '$') {
				nextarg = n;
                        	if (! using_argtable) {
									__argtable_init(&argtable);
                                	__find_arguments (fmt0, orgap,
													  &argtable);
									using_argtable = 1;
				}
				goto rflag;
                        }
			width = n;
			goto reswitch;
#ifdef FLOATING_POINT
		case 'L':
			flags |= LONGDBL;
			goto rflag;
#endif
		case 'h':
			flags |= SHORTINT;
			goto rflag;
		case 'l':
			if (flags & LONGINT)
				flags |= QUADINT;
			else
				flags |= LONGINT;
			goto rflag;
		case 'q':
			flags |= QUADINT;
			goto rflag;
		case 'c':
			*(cp = buf) = GETARG(int);
			size = 1;
			sign = '\0';
			break;
		case 'D':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'd':
		case 'i':
			if (flags & QUADINT) {
				uqval = GETARG(QUAD_T);
				if ((QUAD_T)uqval < 0) {
					uqval = -uqval;
					sign = '-';
				}
			} else {
				ulval = SARG();
				if ((long)ulval < 0) {
					ulval = -ulval;
					sign = '-';
				}
			}
			base = 10;
			goto number;
#ifdef FLOATING_POINT
		case 'e':
		case 'E':
		case 'f':
			goto fp_begin;
		case 'g':
		case 'G':
			if (prec == 0)
				prec = 1;
fp_begin:		if (prec == -1)
				prec = DEFPREC;
			if (flags & LONGDBL)
				/* XXX this loses precision. */
				_double = (double)GETARG(long double);
			else
				_double = GETARG(double);
			/* do this before tricky precision changes */
			if (isinf(_double)) {
				if (_double < 0)
					sign = '-';
				cp = "Inf";
				size = 3;
				break;
			}
			if (isnan(_double)) {
				cp = "NaN";
				size = 3;
				break;
			}
			flags |= FPT;
			if (dtoaresult != NULL) {
				free(dtoaresult);
				dtoaresult = NULL;
			}
			cp = cvt(_double, prec, flags, &softsign,
					 &expt, ch, &ndig, &dtoaresult);
			if (ch == 'g' || ch == 'G') {
				if (expt <= -4 || expt > prec)
					ch = (ch == 'g') ? 'e' : 'E';
				else
					ch = 'g';
			}
			if (ch <= 'e') {	/* 'e' or 'E' fmt */
				--expt;
				expsize = exponent(expstr, expt, ch);
				size = expsize + ndig;
				if (ndig > 1 || flags & ALT)
					++size;
			} else if (ch == 'f') {		/* f fmt */
				if (expt > 0) {
					size = expt;
					if (prec || flags & ALT)
						size += prec + 1;
				} else	/* "0.X" */
					size = prec + 2;
			} else if (expt >= ndig) {	/* fixed g fmt */
				size = expt;
				if (flags & ALT)
					++size;
			} else
				size = ndig + (expt > 0 ? 1 : 2 - expt);

			if (softsign)
				sign = '-';
			break;
#endif /* FLOATING_POINT */
		case 'n':
			if (flags & QUADINT)
				*GETARG(QUAD_T *) = ret;
			else if (flags & LONGINT)
				*GETARG(long *) = ret;
			else if (flags & SHORTINT)
				*GETARG(short *) = ret;
			else
				*GETARG(int *) = ret;
			continue;	/* no output */
		case 'O':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'o':
			if (flags & QUADINT)
				uqval = GETARG(U_QUAD_T);
			else
				ulval = UARG();
			base = 8;
			goto nosign;
		case 'p':
			/*
			 * ``The argument shall be a pointer to void.  The
			 * value of the pointer is converted to a sequence
			 * of printable characters, in an implementation-
			 * defined manner.''
			 *	-- ANSI X3J11
			 */
			ulval = (unsigned long)GETARG(void *);
			base = 16;
			xdigs = "0123456789abcdef";
			flags = (flags & ~QUADINT) | HEXPREFIX;
			ch = 'x';
			goto nosign;
		case 's':
			if ((cp = GETARG(char *)) == NULL)
				cp = "(null)";
 string_spec_process:
			if (prec >= 0) {
				/*
				 * can't use strlen; can only look for the
				 * NUL in the first `prec' characters, and
				 * strlen() will go further.
				 */
				char *p = memchr(cp, 0, (size_t)prec);

				if (p != NULL) {
					size = p - cp;
					if (size > prec)
						size = prec;
				} else
					size = prec;
			} else
				size = strlen(cp);
			sign = '\0';
			break;
		case 'S':												/* ec_string pointer */
			if ((dsval = GETARG(ec_string *)) == NULL) {
				cp = "(null)";
				goto string_spec_process;
			} else
				cp = ec_strdata(dsval);
			if (prec >= 0) {
				size = ec_strlen(dsval);
				if (size > prec)
					size = prec;
			} else
				size = ec_strlen(dsval);
			sign = '\0';
			break;
		case 'b':
			boolval = GETARG(EcBool);
			if (boolval)
				cp = "@true";
			else
				cp = "@false";
			if (prec >= 0) {
				/*
				 * can't use strlen; can only look for the
				 * NUL in the first `prec' characters, and
				 * strlen() will go further.
				 */
				char *p = memchr(cp, 0, (size_t)prec);

				if (p != NULL) {
					size = p - cp;
					if (size > prec)
						size = prec;
				} else
					size = prec;
			} else
				size = strlen(cp);
			sign = '\0';
			break;
		case 'k':
			symid = GETARG(EcUInt);
			if (! EcSymbolExists( symid ))
			{
				/* TODO XXX: raise an error */
				cp = "(inexistent-symbol)";
			}
			else
				cp = (char *)EcSymbolAt( symid );
			if (prec >= 0) {
				/*
				 * can't use strlen; can only look for the
				 * NUL in the first `prec' characters, and
				 * strlen() will go further.
				 */
				char *p = memchr(cp, 0, (size_t)prec);

				if (p != NULL) {
					size = p - cp;
					if (size > prec)
						size = prec;
				} else
					size = prec;
			} else
				size = strlen(cp);
			sign = '\0';
			break;
		case 'I':												/* EcInt */
			ulval = GETARG(EcInt);
			if ((long)ulval < 0) {
				ulval = -ulval;
				sign = '-';
			}
			base = 10;
			goto number;
			break;
		case 'w':
			obj = GETARG(EC_OBJ);
			if (ec_strlen(&subds))
				ec_string_init( &subds, NULL );
			if (! ec_str_ok( &subds )) goto error;
			if (EcPrintObject( &subds, obj, FALSE ) == EOF)
				goto error;
			cp = ec_strdata( &subds );
			if (prec >= 0) {
				EcInt objl;

				objl = ec_strlen( &subds );

				if (objl) {
					size = objl;
					if (size > prec)
						size = prec;
				} else
					size = prec;
			} else
				size = ec_strlen( &subds );
			sign = '\0';
			break;
		case 'W':
			obj = GETARG(EC_OBJ);
			if (ec_strlen(&subds))
				ec_string_init( &subds, NULL );
			if (! ec_str_ok( &subds )) goto error;
			if (EcPrintObject( &subds, obj, TRUE ) == EOF)
				goto error;
			cp = ec_strdata( &subds );
			if (prec >= 0) {
				EcInt objl;

				objl = ec_strlen( &subds );

				if (objl) {
					size = objl;
					if (size > prec)
						size = prec;
				} else
					size = prec;
			} else
				size = ec_strlen( &subds );
			sign = '\0';
			break;
		case 'r':
			obj = GETARG(EC_OBJ);
			if (ec_strlen(&subds))
				ec_string_init( &subds, NULL );
			if (! ec_str_ok( &subds )) goto error;
			if (EcPrintObject( &subds, obj, TRUE ) == EOF)
				goto error;
			cp = ec_strdata( &subds );
			if (prec >= 0) {
				EcInt objl;

				objl = ec_strlen( &subds );

				if (objl) {
					size = objl;
					if (size > prec)
						size = prec;
				} else
					size = prec;
			} else
				size = ec_strlen( &subds );
			sign = '\0';
			break;
		case 'U':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'u':
			if (flags & QUADINT)
				uqval = GETARG(U_QUAD_T);
			else
				ulval = UARG();
			base = 10;
			goto nosign;
		case 'X':
			xdigs = "0123456789ABCDEF";
			goto hex;
		case 'x':
			xdigs = "0123456789abcdef";
hex:			if (flags & QUADINT)
				uqval = GETARG(U_QUAD_T);
			else
				ulval = UARG();
			base = 16;
			/* leading 0x/X only if non-zero */
			if (flags & ALT &&
			    (flags & QUADINT ? uqval != 0 : ulval != 0))
				flags |= HEXPREFIX;

			/* unsigned conversions */
nosign:			sign = '\0';
			/*
			 * ``... diouXx conversions ... if a precision is
			 * specified, the 0 flag will be ignored.''
			 *	-- ANSI X3J11
			 */
number:			if ((dprec = prec) >= 0)
				flags &= ~ZEROPAD;

			/*
			 * ``The result of converting a zero value with an
			 * explicit precision of zero is no characters.''
			 *	-- ANSI X3J11
			 */
			cp = buf + BUF;
			if (flags & QUADINT) {
				if (uqval != 0 || prec != 0)
					cp = __uqtoa(uqval, cp, base,
								 flags & ALT, xdigs);
			} else {
				if (ulval != 0 || prec != 0)
					cp = __ultoa(ulval, cp, base,
								 flags & ALT, xdigs);
			}
			size = buf + BUF - cp;
			break;
		default:	/* "%?" prints ?, unless ? is NUL */
			if (ch == '\0')
				goto done;
			/* pretend it was %c with argument ch */
			cp = buf;
			*cp = ch;
			size = 1;
			sign = '\0';
			break;
		}

		/*
		 * All reasonable formats wind up here.  At this point, `cp'
		 * points to a string which (if not flags&LADJUST) should be
		 * padded out to `width' places.  If flags&ZEROPAD, it should
		 * first be prefixed by any sign or other prefix; otherwise,
		 * it should be blank padded before the prefix is emitted.
		 * After any left-hand padding and prefixing, emit zeroes
		 * required by a decimal [diouxX] precision, then print the
		 * string proper, then emit zeroes required by any leftover
		 * floating precision; finally, if LADJUST, pad with blanks.
		 *
		 * Compute actual size, so we know how much to pad.
		 * size excludes decimal prec; realsz includes it.
		 */
		realsz = dprec > size ? dprec : size;
		if (sign)
			realsz++;
		else if (flags & HEXPREFIX)
			realsz += 2;

		prsize = width > realsz ? width : realsz;
		if ((unsigned)ret + prsize > EC_ECINT_MAX) {
			ret = EOF;
			goto error;
		}

		/* right-adjusting blank padding */
		if ((flags & (LADJUST|ZEROPAD)) == 0)
			PAD(width - realsz, blanks);

		/* prefix */
		if (sign) {
			PRINT(&sign, 1);
		} else if (flags & HEXPREFIX) {
			ox[0] = '0';
			ox[1] = ch;
			PRINT(ox, 2);
		}

		/* right-adjusting zero padding */
		if ((flags & (LADJUST|ZEROPAD)) == ZEROPAD)
			PAD(width - realsz, zeroes);

		/* leading zeroes from decimal precision */
		PAD(dprec - size, zeroes);

		/* the string or number proper */
#ifdef FLOATING_POINT
		if ((flags & FPT) == 0) {
			PRINT(cp, size);
		} else {	/* glue together f_p fragments */
			if (ch >= 'f') {	/* 'f' or 'g' */
				if (_double == 0) {
					/* kludge for __dtoa irregularity */
					if (expt >= ndig &&
					    (flags & ALT) == 0) {
						PRINT("0", 1);
					} else {
						PRINT("0.", 2);
						PAD(ndig - 1, zeroes);
					}
				} else if (expt <= 0) {
					PRINT("0.", 2);
					PAD(-expt, zeroes);
					PRINT(cp, ndig);
				} else if (expt >= ndig) {
					PRINT(cp, ndig);
					PAD(expt - ndig, zeroes);
					if (flags & ALT)
						PRINT(".", 1);
				} else {
					PRINT(cp, expt);
					cp += expt;
					PRINT(".", 1);
					PRINT(cp, ndig-expt);
				}
			} else {	/* 'e' or 'E' */
				if (ndig > 1 || flags & ALT) {
					ox[0] = *cp++;
					ox[1] = '.';
					PRINT(ox, 2);
					if (_double) {
						PRINT(cp, ndig-1);
					} else	/* 0.[0..] */
						/* __dtoa irregularity */
						PAD(ndig - 1, zeroes);
				} else	/* XeYYY */
					PRINT(cp, 1);
				PRINT(expstr, expsize);
			}
		}
#else
		PRINT(cp, size);
#endif
		/* left-adjusting padding (always blank) */
		if (flags & LADJUST)
			PAD(width - realsz, blanks);

		/* finally, adjust ret */
		ret += prsize;

		FLUSH();	/* copy out the I/O vectors */
	}
done:
	FLUSH();
error:
#ifdef FLOATING_POINT
	if (dtoaresult != NULL)
		free(dtoaresult);
#endif
        if (using_argtable)
                __argtable_cleanup(&argtable);
	return (ec_str_ok(ds) ? ret : EOF);
	/* NOTREACHED */
}

EC_API EC_OBJ ec_printf_obj( const char *fmt0, EC_OBJ *args, EcInt nargs, EcInt parstart, EC_OBJ named_sequence )
{
	char *fmt;													/* format string                           */
	int ch;														/* character from fmt                      */
	EcInt n, n2;												/* handy integer (short term usage)        */
	char *cp;													/* handy char pointer (short term usage)   */
	int flags;													/* flags as above                          */
	EcInt ret;													/* return value accumulator                */
	int width;													/* width from format (%8d), or 0           */
	int prec;													/* precision from format (%.3d), or -1     */
	char sign;													/* sign prefix (' ', '+', '-', or \0)      */
#ifdef FLOATING_POINT
	char softsign;												/* temporary negative sign for floats      */
	double _double;												/* double precision arguments %[eEfgG]     */
	int expt;													/* integer value of exponent               */
	int expsize;												/* character count for expstr              */
	int ndig;													/* actual number of digits returned by cvt */
	char expstr[7];												/* buffer for exponent string              */
	char *dtoaresult;											/* buffer allocated by dtoa                */
#endif
	EC_OBJ obj = EC_NIL;										/* elastiC object %[wWr]                   */
	EcUInt symid;												/* elastiC symbol %k                       */
	EcBool boolval;												/* boolean value %b                        */
	unsigned long ulval;										/* integer arguments %[diouxX]             */
	U_QUAD_T      uqval;										/* %q integers                             */
	int base;													/* base for [diouxX] conversion            */
	int dprec;													/* a copy of prec if [diouxX], 0 otherwise */
	int realsz;													/* field size expanded by dprec, sign, etc */
	int size;													/* size of converted field or string       */
	EcInt prsize;												/* max size of printed field               */
	char *xdigs;												/* digits for [xX] conversion              */
	char buf[BUF];												/* space for %c, %[diouxX], %[eEfgG]       */
	char ox[2];													/* space for 0x hex-prefix                 */
        int nextarg;											/* 1-based argument index                  */

	EC_OBJ arg_o;
	EC_OBJ res = EC_NIL;
	EcInt  argnum, parnum;

	ec_string dstr, *ds;
	ec_string subds;

#define MAXNAMED 128
	char   named[MAXNAMED];										/* name of a named argument                     */
	EC_OBJ named_obj;											/* name of a named argument as elastiC object   */
	EcBool using_named;											/* TRUE if we've started using named args       */
	EcBool named_seq_in_args;									/* TRUE if named sequence was fetched from args */

	/*
	 * Choose PADSIZE to trade efficiency vs. size.  If larger printf
	 * fields occur frequently, increase PADSIZE and make the initialisers
	 * below longer.
	 */
#define	PADSIZE	16		/* pad chunk size */
	static char blanks[PADSIZE] =
	 {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
	static char zeroes[PADSIZE] =
	 {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};

	using_named       = FALSE;
	named_seq_in_args = FALSE;
#if 0
	named_hash  = EC_NIL;
#endif

	argnum = 0;
	parnum = parstart - 1;
	ec_string_init( &dstr, NULL );
	ec_string_init( &subds, NULL );

	ds = &dstr;

#define	OARG() (args[argnum++])

#define GETOARG()			do { arg_o = OARG(); parnum++; } while (0)
#define GETOARG_IDX(idx)	do { arg_o = args[(idx) - 1]; } while (0)

#define GETOARG_NA() \
do {                                                    \
	if (using_named && named && *named) {               \
		named_obj = EcMakeSymbol(named);                \
		arg_o = EcSequenceGetItem(named_sequence,       \
								  named_obj);           \
		if (EC_ERRORP(arg_o)) {                         \
			named_obj = EcMakeString(named, -1);        \
			arg_o = EcSequenceGetItem(named_sequence,   \
									  named_obj);       \
			if (EC_ERRORP(arg_o)) {                     \
				res = arg_o;                            \
				goto error;                             \
            }                                           \
		}                                               \
	} else {                                            \
		arg_o = args[nextarg - 1]; nextarg++;           \
    }                                                   \
} while (0)
#define GETOARG_IDX_NA(idx)	do { nextarg = (idx) + 1; arg_o = args[nextarg - 1]; nextarg++; } while (0)

#define ERRPOS_MSG() \
do {                                                                                        \
	ec_string_init( &errpos, NULL );                                                        \
	if (using_named && named && *named && EC_NNULLP(named_obj))                             \
		ec_sprintf( &errpos, "(pos: %I, key: %W)",                                          \
					(EcInt)(parstart + nextarg - 1 - 1), named_obj );                       \
	else                                                                                    \
		ec_sprintf( &errpos, "(pos: %I)",                                                   \
					(EcInt)(parstart + nextarg - 1 - 1) );                                  \
} while (0)

#define	GETARG_INT(rv, spec_ch, spec_str) \
do {                                                                                               \
	GETOARG_NA();                                                                                  \
	if (! EC_INUMP(arg_o))                                                                         \
	{                                                                                              \
		ec_string err, errpos;                                                                     \
		ec_string_init( &err, NULL );                                                              \
		ERRPOS_MSG();                                                                              \
		if (spec_str)                                                                              \
			ec_sprintf( &err,                                                                      \
						"Wrong argument type for %s specifier %S, expected an integer (got %W)",   \
						spec_str, &errpos, arg_o );                                                \
		else                                                                                       \
			ec_sprintf( &err,                                                                      \
						"Wrong argument type for %%%c specifier %S, expected an integer (got %W)", \
						spec_ch, &errpos, arg_o );                                                 \
		EcMakeExceptionWithArgs( EcTypeErrorClass, ec_strdata( &err ),                             \
								 5,                                                                \
								 EC_NIL, EcGetVariable( "basic", "repr" ),                         \
								 using_named ? named_obj : EcMakeInt( parstart + nextarg - 1 ),    \
								 EcMakeSymbol( EcTypeName( tc_inum ) ), arg_o );                   \
		ec_string_cleanup( &errpos );                                                              \
		ec_string_cleanup( &err );                                                                 \
		res = Ec_ERROR;                                                                            \
		goto error;                                                                                \
	}                                                                                              \
	rv = EC_INUM(arg_o);                                                                           \
} while (0)

#define	GETARG_UINT(rv, spec_ch, spec_str) \
do {                                                                                                        \
	GETOARG_NA();                                                                                           \
	if ((! EC_INUMP(arg_o)) || (EC_INUM(arg_o) < 0))                                                        \
	{                                                                                                       \
		ec_string err, errpos;                                                                              \
		ec_string_init( &err, NULL );                                                                       \
		ERRPOS_MSG();                                                                                       \
		if (spec_str)                                                                                       \
			ec_sprintf( &err,                                                                               \
						"Wrong argument type for %s specifier %S, expected an unsigned integer (got %W)",   \
						spec_str, &errpos, arg_o );                                                         \
		else                                                                                                \
			ec_sprintf( &err,                                                                               \
						"Wrong argument type for %%%c specifier %S, expected an unsigned integer (got %W)", \
						spec_ch, &errpos, arg_o );                                                          \
		EcMakeExceptionWithArgs( EcTypeErrorClass, ec_strdata( &err ),                                      \
								 5,                                                                         \
								 EC_NIL, EcGetVariable( "basic", "repr" ),                                  \
								 using_named ? named_obj : EcMakeInt( parstart + nextarg - 1 ),             \
								 EcMakeSymbol( EcTypeName( tc_inum ) ), arg_o );                            \
		ec_string_cleanup( &errpos );                                                                       \
		ec_string_cleanup( &err );                                                                          \
		res = Ec_ERROR;                                                                                     \
		goto error;                                                                                         \
	}                                                                                                       \
	rv = EC_INUM(arg_o);                                                                                    \
} while (0)

#define	GETARG_BOOL(rv, spec_ch) \
do {                                                                                            \
	GETOARG_NA();                                                                               \
	if (! EC_BOOLP(arg_o))                                                                      \
	{                                                                                           \
		ec_string err, errpos;                                                                  \
		ec_string_init( &err, NULL );                                                           \
		ERRPOS_MSG();                                                                           \
		ec_sprintf( &err,                                                                       \
					"Wrong argument type for %%%c specifier %S, expected a bool (got %W)",      \
					spec_ch, &errpos, arg_o );                                                  \
		EcMakeExceptionWithArgs( EcTypeErrorClass, ec_strdata( &err ),                          \
								 5,                                                             \
								 EC_NIL, EcGetVariable( "basic", "repr" ),                      \
								 using_named ? named_obj : EcMakeInt( parstart + nextarg - 1 ), \
								 EcMakeSymbol( EcTypeName( tc_bool ) ), arg_o );                \
		ec_string_cleanup( &errpos );                                                           \
		ec_string_cleanup( &err );                                                              \
		res = Ec_ERROR;                                                                         \
		goto error;                                                                             \
	}                                                                                           \
	rv = EC_BOOLVAL(arg_o);                                                                     \
} while (0)

#define	GETARG_CHAR(rv, spec_ch) \
do {                                                                                            \
	GETOARG_NA();                                                                               \
	if (! EC_CHARP(arg_o))                                                                      \
	{                                                                                           \
		ec_string err, errpos;                                                                  \
		ec_string_init( &err, NULL );                                                           \
		ERRPOS_MSG();                                                                           \
		ec_sprintf( &err,                                                                       \
					"Wrong argument type for %%%c specifier %S, expected a char (got %W)",      \
					spec_ch, &errpos, arg_o );                                                  \
		EcMakeExceptionWithArgs( EcTypeErrorClass, ec_strdata( &err ),                          \
								 5,                                                             \
								 EC_NIL, EcGetVariable( "basic", "repr" ),                      \
								 using_named ? named_obj : EcMakeInt( parstart + nextarg - 1 ), \
								 EcMakeSymbol( EcTypeName( tc_char ) ), arg_o );                \
		ec_string_cleanup( &errpos );                                                           \
		ec_string_cleanup( &err );                                                              \
		res = Ec_ERROR;                                                                         \
		goto error;                                                                             \
	}                                                                                           \
	rv = EC_CHAR(arg_o);                                                                        \
} while (0)

#define	GETARG_FLOAT(rv, spec_ch) \
do {                                                                                            \
	GETOARG_NA();                                                                               \
	if (! EC_FNUMP(arg_o))                                                                      \
	{                                                                                           \
		ec_string err, errpos;                                                                  \
		ec_string_init( &err, NULL );                                                           \
		ERRPOS_MSG();                                                                           \
		ec_sprintf( &err,                                                                       \
					"Wrong argument type for %%%c specifier %S, expected a float (got %W)",     \
					spec_ch, &errpos, arg_o );                                                  \
		EcMakeExceptionWithArgs( EcTypeErrorClass, ec_strdata( &err ),                          \
								 5,                                                             \
								 EC_NIL, EcGetVariable( "basic", "repr" ),                      \
								 using_named ? named_obj : EcMakeInt( parstart + nextarg - 1 ), \
								 EcMakeSymbol( EcTypeName( tc_fnum ) ), arg_o );                \
		ec_string_cleanup( &errpos );                                                              \
		ec_string_cleanup( &err );                                                              \
		res = Ec_ERROR;                                                                         \
		goto error;                                                                             \
	}                                                                                           \
	rv = EC_FNUM(arg_o);                                                                        \
} while (0)

#define	GETARG_STRING(rv, spec_ch) \
do {                                                                                            \
	GETOARG_NA();                                                                               \
	if (! EC_STRINGP(arg_o))                                                                    \
	{                                                                                           \
		ec_string err, errpos;                                                                  \
		ec_string_init( &err, NULL );                                                           \
		ERRPOS_MSG();                                                                           \
		ec_sprintf( &err,                                                                       \
					"Wrong argument type for %%%c specifier %S, expected a string (got %W)",    \
					spec_ch, &errpos, arg_o );                                                  \
		EcMakeExceptionWithArgs( EcTypeErrorClass, ec_strdata( &err ),                          \
								 5,                                                             \
								 EC_NIL, EcGetVariable( "basic", "repr" ),                      \
								 using_named ? named_obj : EcMakeInt( parstart + nextarg - 1 ), \
								 EcMakeSymbol( EcTypeName( tc_string ) ), arg_o );              \
		ec_string_cleanup( &errpos );                                                           \
		ec_string_cleanup( &err );                                                              \
		res = Ec_ERROR;                                                                         \
		goto error;                                                                             \
	}                                                                                           \
	rv = EC_STRDATA(arg_o);                                                                     \
} while (0)

#define	GETARG_SYMBOL(rv, spec_ch) \
do {                                                                                            \
	GETOARG_NA();                                                                               \
	if (! EC_SYMBOLP(arg_o))                                                                    \
	{                                                                                           \
		ec_string err, errpos;                                                                  \
		ec_string_init( &err, NULL );                                                           \
		ERRPOS_MSG();                                                                           \
		ec_sprintf( &err,                                                                       \
					"Wrong argument type for %%%c specifier %S, expected a symbol (got %W)",    \
					spec_ch, &errpos, arg_o );                                                  \
		EcMakeExceptionWithArgs( EcTypeErrorClass, ec_strdata( &err ),                          \
								 5,                                                             \
								 EC_NIL, EcGetVariable( "basic", "repr" ),                      \
								 using_named ? named_obj : EcMakeInt( parstart + nextarg - 1 ), \
								 EcMakeSymbol( EcTypeName( tc_symbol ) ), arg_o );              \
		ec_string_cleanup( &errpos );                                                           \
		ec_string_cleanup( &err );                                                              \
		res = Ec_ERROR;                                                                         \
		goto error;                                                                             \
	}                                                                                           \
	rv = EC_SYMBOL(arg_o);                                                                      \
} while (0)

	/*
	 * To extend shorts properly, we need both signed and unsigned
	 * argument extraction methods.
	 */
#undef SARG
#define	SARG(rv, ch_spec) \
do {                                                                                            \
	EcInt iv;                                                                                   \
	GETOARG_NA();                                                                               \
	if (! EC_INUMP(arg_o))                                                                      \
	{                                                                                           \
		ec_string err, errpos;                                                                  \
		ec_string_init( &err, NULL );                                                           \
		ERRPOS_MSG();                                                                           \
		ec_sprintf( &err,                                                                       \
					"Wrong argument type for %%%c specifier %S, expected an integer (got %W)",  \
					ch_spec, &errpos, arg_o );                                                  \
		EcMakeExceptionWithArgs( EcTypeErrorClass, ec_strdata( &err ),                          \
								 5,                                                             \
								 EC_NIL, EcGetVariable( "basic", "repr" ),                      \
								 using_named ? named_obj : EcMakeInt( parstart + nextarg - 1 ), \
								 EcMakeSymbol( EcTypeName( tc_inum ) ), arg_o );                \
		ec_string_cleanup( &errpos );                                                           \
		ec_string_cleanup( &err );                                                              \
		res = Ec_ERROR;                                                                         \
		goto error;                                                                             \
	}                                                                                           \
	iv = EC_INUM(arg_o);                                                                        \
	rv = (flags&LONGINT ? ((long)iv) :                                                          \
		  flags&SHORTINT ? (long)(short)((int)iv) :                                             \
		  (long)((int)iv));                                                                     \
} while (0)

#undef UARG
#define	UARG(rv, ch_spec) \
do {                                                                                                    \
	EcInt iv;                                                                                           \
	GETOARG_NA();                                                                                       \
	if ((! EC_INUMP(arg_o)) || (EC_INUM(arg_o) < 0))                                                    \
	{                                                                                                   \
		ec_string err, errpos;                                                                          \
		ec_string_init( &err, NULL );                                                                   \
		ERRPOS_MSG();                                                                                   \
		ec_sprintf( &err,                                                                               \
					"Wrong argument type for %%%c specifier %S, expected an unsigned integer (got %W)", \
					ch_spec, &errpos, arg_o );                                                          \
		EcMakeExceptionWithArgs( EcTypeErrorClass, ec_strdata( &err ),                                  \
								 5,                                                                     \
								 EC_NIL, EcGetVariable( "basic", "repr" ),                              \
								 using_named ? named_obj : EcMakeInt( parstart + nextarg - 1 ),         \
								 EcMakeSymbol( EcTypeName( tc_inum ) ), arg_o );                        \
		ec_string_cleanup( &err );                                                                      \
		res = Ec_ERROR;                                                                                 \
		goto error;                                                                                     \
	}                                                                                                   \
	iv = EC_INUM(arg_o);                                                                                \
	rv = (flags&LONGINT ? ((unsigned long)iv) :                                                         \
		  flags&SHORTINT ? (unsigned long)(unsigned short)((int)iv) :                                   \
		  (unsigned long)((unsigned int)iv));                                                           \
} while (0)

	/*
	 * Get * arguments, including the form *nn$.  Preserve the nextarg
	 * that the argument can be gotten once the type is determined.
	 */
#undef GETASTER
#define GETASTER(val, spec_ch, spec_str) \
	do {                                                     \
        n2 = 0;                                              \
        cp = fmt;                                            \
        while (is_digit(*cp)) {                              \
			n2 = 10 * n2 + to_digit(*cp);                    \
			cp++;                                            \
        }                                                    \
        if (*cp == '$') {                                    \
			int hold = nextarg;                              \
			nextarg = n2;                                    \
			GETARG_INT (val, spec_ch, spec_str);             \
			nextarg = hold;                                  \
			fmt = ++cp;                                      \
        } else {                                             \
			GETARG_INT (val, spec_ch, spec_str);             \
        }                                                    \
	} while (0)

#ifdef FLOATING_POINT
	dtoaresult = NULL;
#endif

	fmt = (char *)fmt0;
        nextarg = 1;
	ret = (EcInt) 0;

	/*
	 * Scan the format for conversions (`%' character).
	 */
	for (;;) {
		ulval = 0;
		uqval = 0;
		xdigs = "0123456789ABCDEF";
#ifdef FLOATING_POINT
		_double = 0.0;
		expsize = 0;
#endif
		*named    = '\0';
		named_obj = EC_NIL;

		for (cp = fmt; (ch = *fmt) != '\0' && ch != '%'; fmt++)
			/* void */;
		if ((n = fmt - cp) != 0) {
			if ((unsigned)ret + n > EC_ECINT_MAX) {
				/* TODO: handle this: we should (1) raise an exception or (2) ignore this */
				ret = EOF;
#if 0
				goto error;
#endif
			}
			PRINT(cp, n);
			ret += n;
		}
		if (ch == '\0')
			goto done;
		fmt++;		/* skip over '%' */

		flags = 0;
		dprec = 0;
		width = 0;
		prec = -1;
		sign = '\0';

rflag:		ch = *fmt++;
reswitch:	switch (ch) {
		case ' ':
			/*
			 * ``If the space and + flags both appear, the space
			 * flag will be ignored.''
			 *	-- ANSI X3J11
			 */
			if (!sign)
				sign = ' ';
			goto rflag;
		case '#':
			flags |= ALT;
			goto rflag;
		case '*':
			/*
			 * ``A negative field width argument is taken as a
			 * - flag followed by a positive field width.''
			 *	-- ANSI X3J11
			 * They don't exclude field widths read from args.
			 */
			GETASTER (width, '*', "'*'");
			if (width >= 0)
				goto rflag;
			width = -width;
			/* FALLTHROUGH */
		case '-':
			flags |= LADJUST;
			goto rflag;
		case '+':
			sign = '+';
			goto rflag;
		case '.':
			if ((ch = *fmt++) == '*') {
				GETASTER (n, '.', "'.'");
				prec = n < 0 ? -1 : n;
				goto rflag;
			}
			n = 0;
			while (is_digit(ch)) {
				n = 10 * n + to_digit(ch);
				ch = *fmt++;
			}
			prec = n < 0 ? -1 : n;
			goto reswitch;
		case '0':
			/*
			 * ``Note that 0 is taken as a flag, not as the
			 * beginning of a field width.''
			 *	-- ANSI X3J11
			 */
			flags |= ZEROPAD;
			goto rflag;
		case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			n = 0;
			do {
				n = 10 * n + to_digit(ch);
				ch = *fmt++;
			} while (is_digit(ch));
			if (ch == '$') {
				nextarg = n;
				goto rflag;
                        }
			width = n;
			goto reswitch;
		case '(':
			/* named argument */
			n = 0;
			while ((ch = *fmt++) != ')') {
				if (n >= MAXNAMED - 1) break;
				named[n++] = ch;
			}
			named[n] = '\0';
			if (using_named == FALSE)
			{
#if 0
				/* The next argument _must_ be the named args hash (sequence) */
				named_hash = OARG();
				if (! EcIsSequence(named_hash))
				{
					ec_string err;
					ec_string_init( &err, NULL );
					ec_sprintf( &err,
								"Named argument in conversion specifier requires a sequence (got %W)",
								named_hash );
					EcMakeExceptionWithArgs( EcTypeErrorClass, ec_strdata( &err ),
											 5,
											 EC_NIL, EcGetVariable( "basic", "repr" ),
											 EcMakeInt( parstart + nextarg - 1 ),
											 EC_NIL, named_hash );
					ec_string_cleanup( &err );
					res = Ec_ERROR;
					goto error;
				}
#endif
				if (EC_NULLP(named_sequence))
				{
					/*
					 * Sequence not specified as argument to the C function: the next
					 * argument _must_ be the named args hash (sequence)
					 */
					GETOARG_NA();
					named_sequence = arg_o;
					named_seq_in_args = TRUE;
				}
				if (! EcIsSequence(named_sequence))
				{
					ec_string err;
					ec_string_init( &err, NULL );
					ec_sprintf( &err,
								"Named argument in conversion specifier requires a sequence for arguments (got %W) [%I]",
								named_sequence, (EcInt) (parstart + nextarg - 1) );
					EcMakeExceptionWithArgs( EcTypeErrorClass, ec_strdata( &err ),
											 5,
											 EC_NIL, EcGetVariable( "basic", "repr" ),
											 named_seq_in_args ? EcMakeInt( parstart + nextarg - 1 ) : EC_NIL,
											 EC_NIL, named_sequence );
					ec_string_cleanup( &err );
					res = Ec_ERROR;
					goto error;
				}
			}
			using_named = TRUE;
			goto rflag;
#ifdef FLOATING_POINT
		case 'L':
			flags |= LONGDBL;
			goto rflag;
#endif
		case 'h':
			flags |= SHORTINT;
			goto rflag;
		case 'l':
			if (flags & LONGINT)
				flags |= QUADINT;
			else
				flags |= LONGINT;
			goto rflag;
		case 'q':
			flags |= QUADINT;
			goto rflag;
		case 'c':
			cp = buf;
			GETARG_CHAR(*cp, "%c");
			size = 1;
			sign = '\0';
			break;
		case 'D':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'd':
		case 'i':
			if (flags & QUADINT) {
				/* uqval = GETARG(QUAD_T); */
				GETARG_INT(uqval, ch, NULL);
				if ((QUAD_T)uqval < 0) {
					uqval = -uqval;
					sign = '-';
				}
			} else {
				/* ulval = SARG(); */
				SARG(ulval, ch);
				if ((long)ulval < 0) {
					ulval = -ulval;
					sign = '-';
				}
			}
			base = 10;
			goto number;
#ifdef FLOATING_POINT
		case 'e':
		case 'E':
		case 'f':
			goto fp_begin;
		case 'g':
		case 'G':
			if (prec == 0)
				prec = 1;
fp_begin:		if (prec == -1)
				prec = DEFPREC;
			if (flags & LONGDBL) {
				/* XXX this loses precision. */
				/* _double = (double)GETARG(long double); */
				GETARG_FLOAT(_double, ch);
			} else {
				/* _double = GETARG(double); */
				GETARG_FLOAT(_double, ch);
			}
			/* do this before tricky precision changes */
			if (isinf(_double)) {
				if (_double < 0)
					sign = '-';
				cp = "Inf";
				size = 3;
				break;
			}
			if (isnan(_double)) {
				cp = "NaN";
				size = 3;
				break;
			}
			flags |= FPT;
			if (dtoaresult != NULL) {
				free(dtoaresult);
				dtoaresult = NULL;
			}
			cp = cvt(_double, prec, flags, &softsign,
					 &expt, ch, &ndig, &dtoaresult);
			if (ch == 'g' || ch == 'G') {
				if (expt <= -4 || expt > prec)
					ch = (ch == 'g') ? 'e' : 'E';
				else
					ch = 'g';
			}
			if (ch <= 'e') {	/* 'e' or 'E' fmt */
				--expt;
				expsize = exponent(expstr, expt, ch);
				size = expsize + ndig;
				if (ndig > 1 || flags & ALT)
					++size;
			} else if (ch == 'f') {		/* f fmt */
				if (expt > 0) {
					size = expt;
					if (prec || flags & ALT)
						size += prec + 1;
				} else	/* "0.X" */
					size = prec + 2;
			} else if (expt >= ndig) {	/* fixed g fmt */
				size = expt;
				if (flags & ALT)
					++size;
			} else
				size = ndig + (expt > 0 ? 1 : 2 - expt);

			if (softsign)
				sign = '-';
			break;
#endif /* FLOATING_POINT */
		case 'n':
			/* TODO XXX: not supported: raise an exception */
			continue;	/* no output */
		case 'O':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'o':
			if (flags & QUADINT) {
				/* uqval = GETARG(U_QUAD_T); */
				GETARG_UINT(uqval, ch, NULL);
			} else {
				/* ulval = UARG(); */
				UARG(ulval, ch);
			}
			base = 8;
			goto nosign;
		case 'p':
			/*
			 * ``The argument shall be a pointer to void.  The
			 * value of the pointer is converted to a sequence
			 * of printable characters, in an implementation-
			 * defined manner.''
			 *	-- ANSI X3J11
			 */
			/* ulval = (unsigned long)GETARG(void *); */
			GETARG_INT(ulval, ch, NULL);
			base = 16;
			xdigs = "0123456789abcdef";
			flags = (flags & ~QUADINT) | HEXPREFIX;
			ch = 'x';
			goto nosign;
		case 's':
		case 'S':
			/* if ((cp = GETARG(char *)) == NULL)
			   cp = "(null)"; */
			/* GETARG_STRING(cp, ch); */
			GETOARG_NA();
			if (EC_STRINGP(arg_o))
				cp = EC_STRDATA(arg_o);
			else
			{
				/* Got an object different from a string.
				   No problem get a string representation of it. */
				if (ec_strlen(&subds))
					ec_string_init( &subds, NULL );
				if (! ec_str_ok( &subds )) goto error;
				if (EcPrintObject( &subds, arg_o, FALSE ) == EOF)
					goto error;
				cp = ec_strdata( &subds );
			}
			if (cp == NULL)
				cp = "(null)";
			if (prec >= 0) {
				/*
				 * can't use strlen; can only look for the
				 * NUL in the first `prec' characters, and
				 * strlen() will go further.
				 */
				char *p = memchr(cp, 0, (size_t)prec);

				if (p != NULL) {
					size = p - cp;
					if (size > prec)
						size = prec;
				} else
					size = prec;
			} else
				size = strlen(cp);
			sign = '\0';
			break;
		case 'b':
			/* boolval = GETARG(EcBool); */
			GETARG_BOOL(boolval, ch);
			if (boolval)
				cp = "@true";
			else
				cp = "@false";
			if (prec >= 0) {
				/*
				 * can't use strlen; can only look for the
				 * NUL in the first `prec' characters, and
				 * strlen() will go further.
				 */
				char *p = memchr(cp, 0, (size_t)prec);

				if (p != NULL) {
					size = p - cp;
					if (size > prec)
						size = prec;
				} else
					size = prec;
			} else
				size = strlen(cp);
			sign = '\0';
			break;
		case 'k':
			/* symid = GETARG(EcUInt); */
			GETARG_SYMBOL(symid, ch);
			if (! EcSymbolExists( symid ))
			{
				/* TODO XXX: raise an error */
				cp = "(inexistent-symbol)";
			}
			else
				cp = (char *)EcSymbolAt( symid );
			if (prec >= 0) {
				/*
				 * can't use strlen; can only look for the
				 * NUL in the first `prec' characters, and
				 * strlen() will go further.
				 */
				char *p = memchr(cp, 0, (size_t)prec);

				if (p != NULL) {
					size = p - cp;
					if (size > prec)
						size = prec;
				} else
					size = prec;
			} else
				size = strlen(cp);
			sign = '\0';
			break;
		case 'I':												/* EcInt */
			/* ulval = GETARG(EcInt); */
			GETARG_INT(ulval, ch, NULL);
			if ((long)ulval < 0) {
				ulval = -ulval;
				sign = '-';
			}
			base = 10;
			goto number;
			break;
		case 'w':
			/* obj = GETARG(EC_OBJ); */
			GETOARG_NA();
			obj = arg_o;
			if (ec_strlen(&subds))
				ec_string_init( &subds, NULL );
			if (! ec_str_ok( &subds )) goto error;
			if (EcPrintObject( &subds, obj, FALSE ) == EOF)
				goto error;
			cp = ec_strdata( &subds );
			if (prec >= 0) {
				EcInt objl;

				objl = ec_strlen( &subds );

				if (objl) {
					size = objl;
					if (size > prec)
						size = prec;
				} else
					size = prec;
			} else
				size = ec_strlen( &subds );
			sign = '\0';
			break;
		case 'W':
			/* obj = GETARG(EC_OBJ); */
			GETOARG_NA();
			obj = arg_o;
			if (ec_strlen(&subds))
				ec_string_init( &subds, NULL );
			if (! ec_str_ok( &subds )) goto error;
			if (EcPrintObject( &subds, obj, TRUE ) == EOF)
				goto error;
			cp = ec_strdata( &subds );
			if (prec >= 0) {
				EcInt objl;

				objl = ec_strlen( &subds );

				if (objl) {
					size = objl;
					if (size > prec)
						size = prec;
				} else
					size = prec;
			} else
				size = ec_strlen( &subds );
			sign = '\0';
			break;
		case 'r':
			/* obj = GETARG(EC_OBJ); */
			GETOARG_NA();
			obj = arg_o;
			if (ec_strlen(&subds))
				ec_string_init( &subds, NULL );
			if (! ec_str_ok( &subds )) goto error;
			if (EcPrintObject( &subds, obj, TRUE ) == EOF)
				goto error;
			cp = ec_strdata( &subds );
			if (prec >= 0) {
				EcInt objl;

				objl = ec_strlen( &subds );

				if (objl) {
					size = objl;
					if (size > prec)
						size = prec;
				} else
					size = prec;
			} else
				size = ec_strlen( &subds );
			sign = '\0';
			break;
		case 'U':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'u':
			if (flags & QUADINT) {
				/* uqval = GETARG(U_QUAD_T); */
				GETARG_UINT(uqval, ch, NULL);
			} else {
				/* ulval = UARG(); */
				UARG(ulval, ch);
			}
			base = 10;
			goto nosign;
		case 'X':
			xdigs = "0123456789ABCDEF";
			goto hex;
		case 'x':
			xdigs = "0123456789abcdef";
 hex:			if (flags & QUADINT) {
	 			/* uqval = GETARG(U_QUAD_T); */
				GETARG_UINT(uqval, ch, NULL);
 			} else {
				/* ulval = UARG(); */
				UARG(ulval, ch);
			}
			base = 16;
			/* leading 0x/X only if non-zero */
			if (flags & ALT &&
			    (flags & QUADINT ? uqval != 0 : ulval != 0))
				flags |= HEXPREFIX;

			/* unsigned conversions */
nosign:			sign = '\0';
			/*
			 * ``... diouXx conversions ... if a precision is
			 * specified, the 0 flag will be ignored.''
			 *	-- ANSI X3J11
			 */
number:			if ((dprec = prec) >= 0)
				flags &= ~ZEROPAD;

			/*
			 * ``The result of converting a zero value with an
			 * explicit precision of zero is no characters.''
			 *	-- ANSI X3J11
			 */
			cp = buf + BUF;
			if (flags & QUADINT) {
				if (uqval != 0 || prec != 0)
					cp = __uqtoa(uqval, cp, base,
								 flags & ALT, xdigs);
			} else {
				if (ulval != 0 || prec != 0)
					cp = __ultoa(ulval, cp, base,
								 flags & ALT, xdigs);
			}
			size = buf + BUF - cp;
			break;
		default:	/* "%?" prints ?, unless ? is NUL */
			if (ch == '\0')
				goto done;
			/* pretend it was %c with argument ch */
			cp = buf;
			*cp = ch;
			size = 1;
			sign = '\0';
			break;
		}

		/*
		 * All reasonable formats wind up here.  At this point, `cp'
		 * points to a string which (if not flags&LADJUST) should be
		 * padded out to `width' places.  If flags&ZEROPAD, it should
		 * first be prefixed by any sign or other prefix; otherwise,
		 * it should be blank padded before the prefix is emitted.
		 * After any left-hand padding and prefixing, emit zeroes
		 * required by a decimal [diouxX] precision, then print the
		 * string proper, then emit zeroes required by any leftover
		 * floating precision; finally, if LADJUST, pad with blanks.
		 *
		 * Compute actual size, so we know how much to pad.
		 * size excludes decimal prec; realsz includes it.
		 */
		realsz = dprec > size ? dprec : size;
		if (sign)
			realsz++;
		else if (flags & HEXPREFIX)
			realsz += 2;

		prsize = width > realsz ? width : realsz;
		if ((unsigned)ret + prsize > EC_ECINT_MAX) {
			/* TODO: handle this: we should (1) raise an exception or (2) ignore this */
			ret = EOF;
#if 0
			goto error;
#endif
		}

		/* right-adjusting blank padding */
		if ((flags & (LADJUST|ZEROPAD)) == 0)
			PAD(width - realsz, blanks);

		/* prefix */
		if (sign) {
			PRINT(&sign, 1);
		} else if (flags & HEXPREFIX) {
			ox[0] = '0';
			ox[1] = ch;
			PRINT(ox, 2);
		}

		/* right-adjusting zero padding */
		if ((flags & (LADJUST|ZEROPAD)) == ZEROPAD)
			PAD(width - realsz, zeroes);

		/* leading zeroes from decimal precision */
		PAD(dprec - size, zeroes);

		/* the string or number proper */
#ifdef FLOATING_POINT
		if ((flags & FPT) == 0) {
			PRINT(cp, size);
		} else {	/* glue together f_p fragments */
			if (ch >= 'f') {	/* 'f' or 'g' */
				if (_double == 0) {
					/* kludge for __dtoa irregularity */
					if (expt >= ndig &&
					    (flags & ALT) == 0) {
						PRINT("0", 1);
					} else {
						PRINT("0.", 2);
						PAD(ndig - 1, zeroes);
					}
				} else if (expt <= 0) {
					PRINT("0.", 2);
					PAD(-expt, zeroes);
					PRINT(cp, ndig);
				} else if (expt >= ndig) {
					PRINT(cp, ndig);
					PAD(expt - ndig, zeroes);
					if (flags & ALT)
						PRINT(".", 1);
				} else {
					PRINT(cp, expt);
					cp += expt;
					PRINT(".", 1);
					PRINT(cp, ndig-expt);
				}
			} else {	/* 'e' or 'E' */
				if (ndig > 1 || flags & ALT) {
					ox[0] = *cp++;
					ox[1] = '.';
					PRINT(ox, 2);
					if (_double) {
						PRINT(cp, ndig-1);
					} else	/* 0.[0..] */
						/* __dtoa irregularity */
						PAD(ndig - 1, zeroes);
				} else	/* XeYYY */
					PRINT(cp, 1);
				PRINT(expstr, expsize);
			}
		}
#else
		PRINT(cp, size);
#endif
		/* left-adjusting padding (always blank) */
		if (flags & LADJUST)
			PAD(width - realsz, blanks);

		/* finally, adjust ret */
		ret += prsize;

		FLUSH();	/* copy out the I/O vectors */
	}
done:
	FLUSH();
	res = EcMakeString( ec_strdata( &dstr ), ec_strlen( &dstr ) );
error:
#ifdef FLOATING_POINT
	if (dtoaresult != NULL)
		free(dtoaresult);
#endif
	ec_string_cleanup( &dstr );
	return res;
	/* NOTREACHED */
}

/*
 * Type ids for argument type table.
 */
#define T_UNUSED		0
#define T_SHORT			1
#define T_U_SHORT		2
#define TP_SHORT		3
#define T_INT			4
#define T_U_INT			5
#define TP_INT			6
#define T_LONG			7
#define T_U_LONG		8
#define TP_LONG			9
#define T_QUAD			10
#define T_U_QUAD		11
#define TP_QUAD			12
#define T_DOUBLE		13
#define T_LONG_DOUBLE	14
#define TP_CHAR			15
#define TP_VOID			16

#define T_ECBOOL		17
#define TP_ECBOOL		18
#define T_ECINT			19
#define T_ECUINT		20
#define TP_ECINT		21
#define T_ECOBJ			22
#define TP_ECOBJ		23
#define TP_DSTRING		24

/*
 * Find all arguments when a positional parameter is encountered.  Returns a
 * table, indexed by argument number, of pointers to each arguments.  The
 * initial argument table should be an array of STATIC_ARG_TBL_SIZE entries.
 * It will be replaces with a malloc-ed one if it overflows.
 */ 
static void __find_arguments (const char *fmt0, va_list ap, argtable_t *argtable)
{
	char *fmt;													/* format string                         */
	int ch;														/* character from fmt                    */
	EcInt n, n2;												/* handy integer (short term usage)      */
	char *cp;													/* handy char pointer (short term usage) */
	int flags;													/* flags as above                        */
	int width;													/* width from format (%8d), or 0         */
	int tablemax;												/* largest used index in table           */
	int nextarg;												/* 1-based argument index                */

	/*
	 * Add an argument type to the table, expanding if necessary.
	 */
#define ADDTYPE(type) \
	((nextarg >= argtable->size) ? \
		__argtable_expand(argtable, nextarg) : 0, \
	(nextarg > tablemax) ? tablemax = nextarg : 0, \
	argtable->arg[nextarg++].type_id = type)

#define	ADDSARG() \
	((flags&LONGINT) ? ADDTYPE(T_LONG) : \
		((flags&SHORTINT) ? ADDTYPE(T_SHORT) : ADDTYPE(T_INT)))

#define	ADDUARG() \
	((flags&LONGINT) ? ADDTYPE(T_U_LONG) : \
		((flags&SHORTINT) ? ADDTYPE(T_U_SHORT) : ADDTYPE(T_U_INT)))

	/*
	 * Add * arguments to the type array.
	 */
#define ADDASTER() \
	n2 = 0; \
	cp = fmt; \
	while (is_digit(*cp)) { \
		n2 = 10 * n2 + to_digit(*cp); \
		cp++; \
	} \
	if (*cp == '$') { \
		int hold = nextarg; \
		nextarg = n2; \
		ADDTYPE (T_INT); \
		nextarg = hold; \
		fmt = ++cp; \
	} else { \
		ADDTYPE (T_INT); \
	}

	fmt = (char *)fmt0;
	tablemax = 0; 
	nextarg = 1;
	/* __argtable_init(argtable); */
	/*
	 * Scan the format for conversions (`%' character).
	 */
	for (;;) {
		for (cp = fmt; (ch = *fmt) != '\0' && ch != '%'; fmt++)
			/* void */;
		if (ch == '\0')
			goto done;
		fmt++;		/* skip over '%' */

		flags = 0;
		width = 0;

rflag:		ch = *fmt++;
reswitch:	switch (ch) {
		case ' ':
		case '#':
			goto rflag;
		case '*':
			ADDASTER ();
			goto rflag;
		case '-':
		case '+':
			goto rflag;
		case '.':
			if ((ch = *fmt++) == '*') {
				ADDASTER ();
				goto rflag;
			}
			while (is_digit(ch)) {
				ch = *fmt++;
			}
			goto reswitch;
		case '0':
			goto rflag;
		case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			n = 0;
			do {
				n = 10 * n + to_digit(ch);
				ch = *fmt++;
			} while (is_digit(ch));
			if (ch == '$') {
				nextarg = n;
				goto rflag;
			}
			width = n;
			goto reswitch;
#ifdef FLOATING_POINT
		case 'L':
			flags |= LONGDBL;
			goto rflag;
#endif
		case 'h':
			flags |= SHORTINT;
			goto rflag;
		case 'l':
			if (flags & LONGINT)
				flags |= QUADINT;
			else
				flags |= LONGINT;
			goto rflag;
		case 'q':
			flags |= QUADINT;
			goto rflag;
		case 'c':
			ADDTYPE(T_INT);
			break;
		case 'D':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'd':
		case 'i':
			if (flags & QUADINT) {
				ADDTYPE(T_QUAD);
			} else {
				ADDSARG();
			}
			break;
#ifdef FLOATING_POINT
		case 'e':
		case 'E':
		case 'f':
		case 'g':
		case 'G':
			if (flags & LONGDBL)
				ADDTYPE(T_LONG_DOUBLE);
			else
				ADDTYPE(T_DOUBLE);
			break;
#endif /* FLOATING_POINT */
		case 'n':
			if (flags & QUADINT)
				ADDTYPE(TP_QUAD);
			else if (flags & LONGINT)
				ADDTYPE(TP_LONG);
			else if (flags & SHORTINT)
				ADDTYPE(TP_SHORT);
			else
				ADDTYPE(TP_INT);
			continue;	/* no output */
		case 'O':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'o':
			if (flags & QUADINT)
				ADDTYPE(T_U_QUAD);
			else
				ADDUARG();
			break;
		case 'p':
			ADDTYPE(TP_VOID);
			break;
		case 's':
			ADDTYPE(TP_CHAR);
			break;
		case 'S':
			ADDTYPE(TP_DSTRING);
			break;
		case 'b':
			ADDTYPE(T_ECBOOL);
			break;
		case 'k':
			ADDTYPE(T_ECUINT);
			break;
		case 'I':
			ADDTYPE(T_ECINT);
			break;
		case 'w':
		case 'W':
		case 'r':
			ADDTYPE(T_ECOBJ);
			break;
		case 'U':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'u':
			if (flags & QUADINT)
				ADDTYPE(T_U_QUAD);
			else
				ADDUARG();
			break;
		case 'X':
		case 'x':
			if (flags & QUADINT)
				ADDTYPE(T_U_QUAD);
			else
				ADDUARG();
			break;
		default:	/* "%?" prints ?, unless ? is NUL */
			if (ch == '\0')
				goto done;
			break;
		}
	}
done:
	if (tablemax >= argtable->size) {
		__argtable_expand(argtable, tablemax + 1);
	}

	/* memset(&(argtable->arg[0]), 0x00, sizeof(arg_entry_t));
	   argtable->arg[0].type_id = T_UNUSED; */
	argtable->arg[0].addr = NULL;
	for (n = 1; n <= tablemax; n++) {
		switch (argtable->arg[n].type_id) {
		    case T_UNUSED:
			argtable->arg[n].v.v_int = va_arg (ap, int);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_int);
			break;
		    case T_SHORT:
			argtable->arg[n].v.v_short = va_arg (ap, int);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_short);
			break;
		    case T_U_SHORT:
			argtable->arg[n].v.v_u_short = va_arg (ap, int);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_u_short);
			break;
		    case TP_SHORT:
			argtable->arg[n].v.v_shortp = va_arg (ap, short *);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_shortp);
			break;
		    case T_INT:
			argtable->arg[n].v.v_int = va_arg (ap, int);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_intp);
			break;
		    case T_U_INT:
			argtable->arg[n].v.v_u_int = va_arg (ap, unsigned int);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_u_int);
			break;
		    case TP_INT:
			argtable->arg[n].v.v_intp = va_arg (ap, int *);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_intp);
			break;
		    case T_LONG:
			argtable->arg[n].v.v_long = va_arg (ap, long);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_long);
			break;
		    case T_U_LONG:
			argtable->arg[n].v.v_u_long = va_arg (ap, unsigned long);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_u_long);
			break;
		    case TP_LONG:
			argtable->arg[n].v.v_longp = va_arg (ap, long *);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_longp);
			break;
		    case T_QUAD:
			argtable->arg[n].v.v_quad = va_arg (ap, QUAD_T);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_quad);
			break;
		    case T_U_QUAD:
			argtable->arg[n].v.v_u_quad = va_arg (ap, U_QUAD_T);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_u_quad);
			break;
		    case TP_QUAD:
			argtable->arg[n].v.v_quadp = va_arg (ap, QUAD_T *);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_quadp);
			break;
		    case T_DOUBLE:
			argtable->arg[n].v.v_double = va_arg (ap, double);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_double);
			break;
		    case T_LONG_DOUBLE:
			argtable->arg[n].v.v_long_double = va_arg (ap, long double);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_long_double);
			break;
		    case TP_CHAR:
			argtable->arg[n].v.v_charp = va_arg (ap, char *);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_charp);
			break;
		    case TP_VOID:
			argtable->arg[n].v.v_voidp = va_arg (ap, void *);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_voidp);
			break;

		    case T_ECBOOL:
			argtable->arg[n].v.v_ecbool = va_arg (ap, EcBool);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_ecbool);
			break;
		    case TP_ECBOOL:
			argtable->arg[n].v.v_ecboolp = va_arg (ap, EcBool *);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_ecboolp);
			break;
		    case T_ECINT:
			argtable->arg[n].v.v_ecint = va_arg (ap, EcInt);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_ecint);
			break;
		    case T_ECUINT:
			argtable->arg[n].v.v_ecuint = va_arg (ap, EcUInt);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_ecuint);
			break;
		    case TP_ECINT:
			argtable->arg[n].v.v_ecintp = va_arg (ap, EcInt *);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_ecintp);
			break;
		    case T_ECOBJ:
			argtable->arg[n].v.v_ecobj = va_arg (ap, EC_OBJ);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_ecobj);
			break;
		    case TP_ECOBJ:
			argtable->arg[n].v.v_ecobjp = va_arg (ap, EC_OBJ *);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_ecobjp);
			break;
		    case TP_DSTRING:
			argtable->arg[n].v.v_ecstringp = va_arg (ap, ec_string *);
			argtable->arg[n].addr = &(argtable->arg[n].v.v_ecstringp);
			break;
		}
	}
}

static void *realloc_or_free(void *ptr, size_t size)
{
	void *nptr;

	nptr = realloc(ptr, size);
	if (!nptr && ptr)
		free(ptr);
	return (nptr);
}

static void  __argtable_init(argtable_t *table)
{
	int i;

	table->size = STATIC_ARG_TBL_SIZE;
	table->arg  = table->stattable;

	for (i = 0; i< table->size; i++)
	{
		memset(&(table->arg[i]), 0x00, sizeof(arg_entry_t));
		table->arg[i].type_id = T_UNUSED;
		table->arg[i].addr    = NULL;
	}
}

static void  __argtable_cleanup(argtable_t *table)
{
	if (! table) return;

	if (table->arg != table->stattable)
		free(table->arg);
}

/* Increase the size of the type/argument table */
static int __argtable_expand(argtable_t *table, int nextarg)
{
	arg_entry_t * const oldtable = table->arg;
	const int oldsize = table->size;
	arg_entry_t *newtable;
	int newsize = oldsize * 2;
	int i;

	if (newsize < nextarg + 1)
		newsize = nextarg + 1;
	if (oldsize == STATIC_ARG_TBL_SIZE) {
		if ((newtable = malloc(newsize * sizeof(arg_entry_t))) == NULL)
		{
			abort();			/* TODO XXX: handle this case gracefully */
			return 0;
		}
		memcpy(newtable, oldtable, oldsize * sizeof(arg_entry_t));
	} else {
		if ((newtable = realloc_or_free(oldtable, newsize * sizeof(arg_entry_t))) == NULL)
		{
			abort();			/* TODO XXX: handle this case gracefully */
			return 0;
		}
	}
	for (i = oldsize; i < newsize; i++)
	{
		memset(&newtable[i], 0x00, sizeof(arg_entry_t));
		newtable[i].type_id = T_UNUSED;
		newtable[i].addr    = NULL;
	}

	table->arg  = newtable;
	table->size = newsize;
	return 1;
}



#ifdef FLOATING_POINT

extern char *_ec_dtoa (double, int, int, int *, int *, char **, char **);

static char *cvt(double value, int ndigits, int flags, char *sign, int *decpt,
				 int ch, int *length, char **dtoaresultp)
{
	int mode, dsgn;
	char *digits, *bp, *rve;

	if (ch == 'f')
		mode = 3;		/* ndigits after the decimal point */
	else {
		/*
		 * To obtain ndigits after the decimal point for the 'e'
		 * and 'E' formats, round to ndigits + 1 significant
		 * figures.
		 */
		if (ch == 'e' || ch == 'E')
			ndigits++;
		mode = 2;		/* ndigits significant digits */
	}
	if (value < 0) {
		value = -value;
		*sign = '-';
	} else
		*sign = '\000';
	digits = _ec_dtoa(value, mode, ndigits, decpt, &dsgn, &rve,
					  dtoaresultp);
	if ((ch != 'g' && ch != 'G') || flags & ALT) {
		/* print trailing zeros */
		bp = digits + ndigits;
		if (ch == 'f') {
			if (*digits == '0' && value)
				*decpt = -ndigits + 1;
			bp += *decpt;
		}
		if (value == 0)	/* kludge for __dtoa irregularity */
			rve = bp;
		while (rve < bp)
			*rve++ = '0';
	}
	*length = rve - digits;
	return (digits);
}

static int exponent(char *p0, int exp, int fmtch)
{
	char *p, *t;
	char expbuf[MAXEXP];

	p = p0;
	*p++ = fmtch;
	if (exp < 0) {
		exp = -exp;
		*p++ = '-';
	}
	else
		*p++ = '+';
	t = expbuf + MAXEXP;
	if (exp > 9) {
		do {
			*--t = to_char(exp % 10);
		} while ((exp /= 10) > 9);
		*--t = to_char(exp);
		for (; t < expbuf + MAXEXP; *p++ = *t++);
	}
	else {
		*p++ = '0';
		*p++ = to_char(exp);
	}
	return (p - p0);
}
#endif /* FLOATING_POINT */
