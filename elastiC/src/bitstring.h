/* ==========================================================================
 * bitstring.h
 * ==========================================================================
 * Bit string handling macros.
 * Derived from source code in BSD. See below for original licensing.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Tue Oct 16 01:12:58 CEST 2001
 *
 *   $Id: bitstring.h,v 1.1 2002/05/23 21:17:48 panta Exp $
 * --------------------------------------------------------------------------
 *  This code is derived from software which is:
 *
 *    Copyright (c) 1989 The Regents of the University of California.
 *    All rights reserved.
 *
 *  The original copyright notice and legale terms are reported below in the
 *  next comment.
 *
 *  Modifications are subject to the following:
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

/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Paul Vixie.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *	@(#)bitstring.h	5.2 (Berkeley) 4/4/90
 */

/*
 * NOTE: As stated in
 *  ftp://ftp.cs.berkeley.edu/pub/4bsd/README.Impt.License.Change
 * the so called `advertising clause' in the above BSD license doesn't
 * apply anymore.
 */

#ifndef __BITSTRING_H
#define __BITSTRING_H

/* modified for SV/AT and bitstring bugfix by M.R.Murphy, 11oct91
 * bitstr_size changed gratuitously, but shorter
 * bit_alloc   spelling error fixed
 * the following were efficient, but didn't work, they've been made to
 * work, but are no longer as efficient :-)
 * bit_nclear, bit_nset, bit_ffc, bit_ffs
 */
typedef	unsigned char ec_bitstr_t;

/* internal macros */
				/* byte of the bitstring bit is in */
#define	_ec_bit_byte(bit) \
	((bit) >> 3)

				/* mask for the bit within its byte */
#define	_ec_bit_mask(bit) \
	(1 << ((bit)&0x7))

/* external macros */
				/* bytes in a bitstring of nbits bits */
#define	ec_bitstr_size(nbits) \
	(((nbits) + 7) >> 3)

				/* allocate a bitstring */
#define	ec_bit_alloc(nbits) \
	(bitstr_t *)ec_calloc((size_t)ec_bitstr_size(nbits), sizeof(bitstr_t))

				/* allocate a bitstring on the stack */
#define	ec_bit_decl(name, nbits) \
	(name)[ec_bitstr_size(nbits)]

				/* is bit N of bitstring name set? */
#define	ec_bit_test(name, bit) \
	((name)[_ec_bit_byte(bit)] & _ec_bit_mask(bit))

				/* set bit N of bitstring name */
#define	ec_bit_set(name, bit) \
	(name)[_ec_bit_byte(bit)] |= _ec_bit_mask(bit)

				/* clear bit N of bitstring name */
#define	ec_bit_clear(name, bit) \
	(name)[_ec_bit_byte(bit)] &= ~_ec_bit_mask(bit)

				/* clear bits start ... stop in bitstring */
#define	ec_bit_nclear(name, start, stop) do { \
	register ec_bitstr_t *_name = name; \
	register int _start = start, _stop = stop; \
	while (_start <= _stop) { \
		ec_bit_clear(_name, _start); \
		_start++; \
		} \
} while (0)

				/* set bits start ... stop in bitstring */
#define	ec_bit_nset(name, start, stop) do { \
	register ec_bitstr_t *_name = name; \
	register int _start = start, _stop = stop; \
	while (_start <= _stop) { \
		ec_bit_set(_name, _start); \
		_start++; \
		} \
} while (0)

				/* find first bit clear in name */
#define	ec_bit_ffc(name, nbits, value) do { \
	register ec_bitstr_t *_name = name; \
	register int _bit, _nbits = nbits, _value = -1; \
	for (_bit = 0; _bit < _nbits; ++_bit) \
		if (! ec_bit_test(_name, _bit)) { \
			_value = _bit; \
			break; \
		} \
	*(value) = _value; \
} while (0)

				/* find first bit clear in name starting from sbit */
#define	ec_bit_ffc_offset(name, nbits, sbit, value) do { \
	register ec_bitstr_t *_name = name; \
	register int _bit, _nbits = nbits, _sbit = sbit, _value = -1; \
	for (_bit = _sbit; _bit < _nbits; ++_bit) \
		if (! ec_bit_test(_name, _bit)) { \
			_value = _bit; \
			break; \
		} \
	*(value) = _value; \
} while (0)

				/* find first bit set in name */
#define	ec_bit_ffs(name, nbits, value) do { \
	register ec_bitstr_t *_name = name; \
	register int _bit, _nbits = nbits, _value = -1; \
	for (_bit = 0; _bit < _nbits; ++_bit) \
		if (ec_bit_test(_name, _bit)) { \
			_value = _bit; \
			break; \
		} \
	*(value) = _value; \
} while (0)

				/* find first bit set in name starting from sbit */
#define	ec_bit_ffs_offset(name, nbits, sbit, value) do { \
	register ec_bitstr_t *_name = name; \
	register int _bit, _nbits = nbits, _sbit = sbit, _value = -1; \
	for (_bit = _sbit; _bit < _nbits; ++_bit) \
		if (ec_bit_test(_name, _bit)) { \
			_value = _bit; \
			break; \
		} \
	*(value) = _value; \
} while (0)

#endif /* __BITSTRING_H */
