/* ==========================================================================
 * user.h
 * ==========================================================================
 * User defined types handling.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat Dec 12 11:39:17 MET 1998
 *
 *   $Id: user.h,v 1.1 2002/05/23 21:17:41 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1998-2000 Marco Pantaleoni. All rights reserved.
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

#ifndef __USER_H
#define __USER_H

#include <elastic/basic.h>
#include <elastic/dstring.h>

EC_BEGIN_DECLS

typedef enum { EcShallowCopyType, EcDeepCopyType } EcCopyType;

/* typedef EC_OBJ (*ec_cons_fcn)( EcInt nargs, ... ); */
typedef EC_OBJ (*ec_copy_fcn)( EC_OBJ obj, EcCopyType type );

typedef void   (*ec_mark_fcn)( EC_OBJ obj );
typedef void   (*ec_free_fcn)( EC_OBJ obj );
typedef EcInt  (*ec_print_fcn)( ec_string *str, EC_OBJ obj, EcBool detailed );

typedef EC_OBJ (*ec_getitem_fcn)( EC_OBJ obj, EC_OBJ index );
typedef EC_OBJ (*ec_setitem_fcn)( EC_OBJ obj, EC_OBJ index, EC_OBJ value );
typedef EC_OBJ (*ec_delitem_fcn)( EC_OBJ obj, EC_OBJ index );
typedef EC_OBJ (*ec_length_fcn)(  EC_OBJ obj );

typedef EcUInt (*ec_hash_fcn)( EC_OBJ obj, EcInt recursion_level );
typedef EcBool (*ec_compare_fcn)( EC_OBJ obj1, EC_OBJ obj2, EcInt *res );

typedef EC_OBJ (*ec_binop_fcn)( EC_OBJ obj1, EC_OBJ obj2 );
typedef EC_OBJ (*ec_unaryop_fcn)( EC_OBJ obj );

typedef EcBool (*ec_check_fcn)( EC_OBJ obj );

/*typedef void   (*ec_store_fcn)( EcStream *stream, EC_OBJ obj, EcPassivationMode mode );
typedef EC_OBJ (*ec_restore_fcn)( EcStream *stream, EcPassivationMode mode );*/

typedef struct EcSequenceCallbacks
{
	ec_getitem_fcn getitem_fcn;
	ec_setitem_fcn setitem_fcn;
	ec_delitem_fcn delitem_fcn;
	ec_length_fcn  length_fcn;
} EcSequenceCallbacks;

typedef struct EcNumericCallbacks
{
	ec_binop_fcn   promote_fcn;
	ec_binop_fcn   add_fcn;
	ec_binop_fcn   radd_fcn;
	ec_binop_fcn   sub_fcn;
	ec_binop_fcn   rsub_fcn;
	ec_binop_fcn   mul_fcn;
	ec_binop_fcn   rmul_fcn;
	ec_binop_fcn   div_fcn;
	ec_binop_fcn   rdiv_fcn;
	ec_binop_fcn   mod_fcn;
	ec_binop_fcn   rmod_fcn;
	ec_binop_fcn   pow_fcn;
	ec_binop_fcn   rpow_fcn;
	ec_binop_fcn   lshift_fcn;
	ec_binop_fcn   rlshift_fcn;
	ec_binop_fcn   rshift_fcn;
	ec_binop_fcn   rrshift_fcn;
	ec_binop_fcn   and_fcn;
	ec_binop_fcn   rand_fcn;
	ec_binop_fcn   or_fcn;
	ec_binop_fcn   ror_fcn;
	ec_binop_fcn   xor_fcn;
	ec_binop_fcn   rxor_fcn;
	ec_unaryop_fcn neg_fcn;
	ec_unaryop_fcn pos_fcn;
	ec_unaryop_fcn abs_fcn;
	ec_unaryop_fcn invert_fcn;
	ec_unaryop_fcn int_fcn;
	ec_unaryop_fcn float_fcn;
	ec_unaryop_fcn true_fcn;
} EcNumericCallbacks;

typedef struct EcTypespec
{
	EcInt  type;
	char  *name;

	/* Construction */
	/* NOTE: I've not yet decided on how to do preper
	 *       generic construction. Stay tuned.        */
	/*ec_cons_fcn    cons_fcn;*/
	ec_copy_fcn    copy_fcn;

	/* Misc */
	ec_mark_fcn    mark_fcn;
	ec_free_fcn    free_fcn;
	ec_print_fcn   print_fcn;

	/* Hash & Compare */
	ec_hash_fcn    hash_fcn;
	ec_compare_fcn compare_fcn;

	/* Debugging */
	ec_check_fcn   check_fcn;

	/* Streaming */
/*	ec_store_fcn   store_fcn;
	ec_restore_fcn restore_fcn; */

	/* Sequence */
	EcSequenceCallbacks *sequence_cb;

	/* Numeric */
	EcNumericCallbacks *numeric_cb;
} EcTypespec;

EC_END_DECLS

#endif /* __USER_H */
