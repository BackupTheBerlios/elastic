/* ==========================================================================
 * array_m.c
 * ==========================================================================
 * Array module.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat Dec 11 09:05:19 MET 1999
 *
 *   $Id: array_m.c,v 1.1 2002/05/23 21:18:38 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1999-2000 Marco Pantaleoni. All rights reserved.
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
#include "debug.h"

#include "elastic.h"
#include "private.h"

#include "array_m.h"

static EC_OBJ EcLibArray_Length( EC_OBJ stack, EcAny userdata );
static EC_OBJ EcLibArray_Push( EC_OBJ stack, EcAny userdata );
static EC_OBJ EcLibArray_Pop( EC_OBJ stack, EcAny userdata );
static EC_OBJ EcLibArray_Shift( EC_OBJ stack, EcAny userdata );
static EC_OBJ EcLibArray_Unshift( EC_OBJ stack, EcAny userdata );
static EC_OBJ EcLibArray_Sub( EC_OBJ stack, EcAny userdata );
static EC_OBJ EcLibArray_Find( EC_OBJ stack, EcAny userdata );

/* ========================================================================
 * array.* primitives
 * ======================================================================== */

static EC_OBJ EcLibArray_Length( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ seq;
	EC_OBJ res;

	res = EcParseStackFunction( "array.length", TRUE, stack, "O",
								&seq );
	if (EC_ERRORP(res))
		return res;

	return EcSequenceLength( seq );
}

static EC_OBJ EcLibArray_Push( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ seq, obj, len;
	EC_OBJ res;

	res = EcParseStackFunction( "array.push", TRUE, stack, "OO",
								&seq, &obj );
	if (EC_ERRORP(res))
		return res;

	len = EcSequenceLength( seq );
	if (EC_ERRORP(len)) return len;

	return EcSequenceSetItem( seq, len, obj );
}

static EC_OBJ EcLibArray_Pop( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ seq, obj, len;
	EC_OBJ res;

	res = EcParseStackFunction( "array.pop", TRUE, stack, "O",
								&seq );
	if (EC_ERRORP(res))
		return res;

	len = EcSequenceLength( seq );
	if (EC_ERRORP(len)) return len;

	ASSERT( EC_INUMP(len) );
	obj = EcSequenceGetElement( seq, EC_INUM(len) - 1 );
	if (EC_ERRORP(obj)) return obj;

	res = EcSequenceDelElement( seq, EC_INUM(len) - 1 );
	if (EC_ERRORP(res)) return res;

	return obj;
}

static EC_OBJ EcLibArray_Shift( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ seq, obj, len;
	EC_OBJ res;
	EcInt  i, l;

	res = EcParseStackFunction( "array.shift", TRUE, stack, "OO",
								&seq, &obj );
	if (EC_ERRORP(res))
		return res;

	len = EcSequenceLength( seq );
	if (EC_ERRORP(len)) return len;

	/* Add an object to the tail expanding the sequence */
	res = EcSequenceSetItem( seq, len, EC_NIL );

	/* Now shift up all previuous elements */
	ASSERT( EC_INUMP(len) );
	l = EC_INUM(len);
	for (i = 0; i < l; i++)
	{
		res = EcSequenceGetElement( seq, i );
		if (EC_ERRORP(res)) return res;
		res = EcSequenceSetElement( seq, i + 1, res );
		if (EC_ERRORP(res)) return res;
	}

	/* Set first element */
	res = EcSequenceSetElement( seq, 0, obj );
	if (EC_ERRORP(res)) return res;

	return seq;
}

static EC_OBJ EcLibArray_Unshift( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ seq, obj, len;
	EC_OBJ res;
	EcInt  i, l;

	res = EcParseStackFunction( "array.unshift", TRUE, stack, "O",
								&seq );
	if (EC_ERRORP(res))
		return res;

	len = EcSequenceLength( seq );
	if (EC_ERRORP(len)) return len;

	obj = EcSequenceGetElement( seq, 0 );
	if (EC_ERRORP(obj)) return obj;

	/* Now shift down all other elements */
	ASSERT( EC_INUMP(len) );
	l = EC_INUM(len);
	for (i = 1; i < l; i++)
	{
		res = EcSequenceGetElement( seq, i );
		if (EC_ERRORP(res)) return res;
		res = EcSequenceSetElement( seq, i - 1, res );
		if (EC_ERRORP(res)) return res;
	}

	/* Remove last element */
	res = EcSequenceDelElement( seq, l-1 );
	if (EC_ERRORP(res)) return res;

	return obj;
}

static EC_OBJ EcLibArray_Sub( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ  seq, obj, slen;
	EC_OBJ  res;
	EcInt   length, start, len;
	EcInt   s1, s2, i;

	/*
	 * TODO:
	 * Without generic construction we can operate only on arrays here.
	 * The other method is to use the forthcoming getrange_fcn().
	 * Sorry.
	 */

	/* Parse two times to get sequence length */

	res = EcParseStackFunction( "array.sub", FALSE, stack, "O!i|i", tc_array, &seq, &start, &len );
	if (EC_ERRORP(res))
		return res;

	slen = EcSequenceLength( seq );
	if (EC_ERRORP(slen)) return slen;

	ASSERT( EC_INUMP(slen) );
	length = EC_INUM(slen);
	len    = length;

	res = EcParseStackFunction( "array.sub", TRUE, stack, "O!i|i", tc_array, &seq, &start, &len );
	if (EC_ERRORP(res))
		return res;

	if (start >= 0)
		s1 = start;
	else
		s1 = (length - 1) + start;

	if (len >= 0)
		s2 = s1 + len - 1;
	else
	{
		s2 = (length - 1) + len;
		len = s2 - s1 + 1;
	}

	if ((s1 < 0) || (s1 >= length))
		return EcIndexError( seq, EcMakeInt( s1 ) );

	if ((s2 < 0) || (s2 >= length))
		return EcIndexError( seq, EcMakeInt( s2 ) );

	obj = EcMakeArray( len );
	if (EC_ERRORP(obj)) return obj;

	for (i = s1; i <= s2; i++)
	{
		res = EcSequenceGetElement( seq, i );
		if (EC_ERRORP(res)) return res;
		res = EcSequenceSetElement( obj, i - s1, res );
		if (EC_ERRORP(res)) return res;
	}

	return obj;
}

static EC_OBJ EcLibArray_Find( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ seq, obj, len;
	EC_OBJ res;
	EcInt  i, l;

	res = EcParseStackFunction( "array.find", TRUE, stack, "OO",
								&seq, &obj );
	if (EC_ERRORP(res))
		return res;

	len = EcSequenceLength( seq );
	if (EC_ERRORP(len)) return len;

	ASSERT(EC_INUMP(len));
	l = EC_INUM(len);
	for (i = 0; i < l; i++)
	{
		res = EcSequenceGetElement( seq, i );
		if (EC_ERRORP(res)) return res;
		if (EcObjectEqual( res, obj )) return EcMakeInt( i );
	}

	return EcFalseObject;
}

/* Private */

EcBool _ec_modarray_init( void )
{
	EcPackageIntroduce( "array" );

	EcAddPrimitive( "array.length",     EcLibArray_Length );
	EcAddPrimitive( "array.push",       EcLibArray_Push );
	EcAddPrimitive( "array.pop",        EcLibArray_Pop );
	EcAddPrimitive( "array.shift",      EcLibArray_Shift );
	EcAddPrimitive( "array.unshift",    EcLibArray_Unshift );
	EcAddPrimitive( "array.sub",        EcLibArray_Sub );
	EcAddPrimitive( "array.find",       EcLibArray_Find );

	return TRUE;
}

void _ec_modarray_cleanup( void )
{
}
