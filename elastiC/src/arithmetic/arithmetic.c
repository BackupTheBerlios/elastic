/* ==========================================================================
 * arithmetic.c
 * ==========================================================================
 * Arithmetic subsystem.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sun Aug  1 18:53:27 MEST 1999
 *
 *   $Id: arithmetic.c,v 1.2 2002/05/25 19:37:45 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1999-2001 Marco Pantaleoni. All rights reserved.
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

#include <math.h>

#include "basic.h"
#include "elastic.h"
#include "debug.h"

#include "private.h"
/* #include "config.h" */
#include "user.h"

#include "arithmetic.h"


/* ------------------------------------------------------------------------
 * P R O T O T Y P E S
 * ------------------------------------------------------------------------ */

static EC_OBJ basic_add( EC_OBJ v, EC_OBJ w );
static EC_OBJ basic_sub( EC_OBJ v, EC_OBJ w );
static EC_OBJ basic_mul( EC_OBJ v, EC_OBJ w );
static EC_OBJ basic_div( EC_OBJ v, EC_OBJ w );
static EC_OBJ basic_mod( EC_OBJ v, EC_OBJ w );
static EC_OBJ basic_pow( EC_OBJ v, EC_OBJ w );
static EC_OBJ basic_lshift( EC_OBJ v, EC_OBJ w );
static EC_OBJ basic_rshift( EC_OBJ v, EC_OBJ w );
static EC_OBJ basic_and( EC_OBJ v, EC_OBJ w );
static EC_OBJ basic_or( EC_OBJ v, EC_OBJ w );
static EC_OBJ basic_xor( EC_OBJ v, EC_OBJ w );
static EC_OBJ basic_neg( EC_OBJ v );
static EC_OBJ basic_pos( EC_OBJ v );
static EC_OBJ basic_abs( EC_OBJ v );
static EC_OBJ basic_invert( EC_OBJ v );
static EC_OBJ basic_int( EC_OBJ v );
static EC_OBJ basic_float( EC_OBJ v );
static EC_OBJ basic_true( EC_OBJ v );


/* ========================================================================
 * M A C R O S
 * ======================================================================== */

#define BINOPFUNC(oper)	        ec_binop_##oper
#define BINOPUSERFUNC(oper)	    ec_binopuser_##oper
#define HALFBINOPUSERFUNC(oper)	halfbinop_##oper

#define OPNAME(oper)			"_"  #oper
#define ROPNAME(oper)			"_r" #oper

#define BINOP(oper, v, w, func)                 \
do {                                            \
	if (EC_USERP(v) || EC_USERP(w))             \
		return BINOPUSERFUNC(oper)(v, w, func); \
} while (0)

#define UNARYOPFUNC(oper)	    ec_unaryop_##oper


/* ========================================================================
 * E M I T T E R   M A C R O S
 * ======================================================================== */

#define CB_FCN(x) x##_fcn
#define EMIT_BINOP_FUNC(oper) \
static EC_OBJ BINOPUSERFUNC(oper)(EC_OBJ v, EC_OBJ w, ec_binop_fcn func); \
static EcInt  HALFBINOPUSERFUNC(oper)(EC_OBJ v, EC_OBJ w, const char *opname, EC_OBJ *result, ec_binop_fcn func, EcBool reversed); \
static EcInt  HALFBINOPUSERFUNC(r##oper)(EC_OBJ v, EC_OBJ w, const char *opname, EC_OBJ *result, ec_binop_fcn func, EcBool reversed); \
EC_OBJ BINOPFUNC(oper)(EC_OBJ v, EC_OBJ w)                                     \
{                                                                              \
	BINOP(oper, v, w, BINOPFUNC(oper));                                        \
	if (! EcPromote(&v, &w)) return EcUnknownOperationError( #oper, 2, v, w ); \
	return basic_##oper(v, w);                                                 \
}

#define EMIT_BINOP_USERFUNC(oper) \
static EC_OBJ BINOPUSERFUNC(oper)(EC_OBJ v, EC_OBJ w, ec_binop_fcn func) \
{                                                                                              \
	EC_OBJ result = EC_NIL;                                                                    \
	if (HALFBINOPUSERFUNC(oper)(v, w,  OPNAME(oper), &result, func, 0) <= 0) return result;    \
	if (HALFBINOPUSERFUNC(r##oper)(w, v, ROPNAME(oper), &result, func, 1) <= 0) return result; \
	return EcUnknownOperationError( #oper, 2, v, w );                                          \
}

#define EMIT_HALFBINOP_USERFUNC(oper) \
static EcInt HALFBINOPUSERFUNC(oper)(EC_OBJ v, EC_OBJ w, const char *opname, EC_OBJ *result, ec_binop_fcn func, EcBool reversed) \
{                                                                                           \
	EC_OBJ promoted, new_v;                                                                 \
                                                                                            \
    if (! IS_USER_NUMERIC(v)) return 1;                                                     \
                                                                                            \
	if (CB_NUMERIC(v)->promote_fcn)                                                         \
	{                                                                                       \
		promoted = CB_NUMERIC(v)->promote_fcn(v, w);                                        \
		if (EC_ERRORP(promoted)) return -1;                                                 \
		if (EC_NULLP(promoted))  return  1;                                                 \
		if (EC_BOOLP(promoted) && (! EC_BOOLVAL(promoted))) goto goon;                      \
		if ((! EC_ARRAYP(promoted)) || (EC_ARRAYLEN(promoted) != 2))                        \
		{                                                                                   \
			/* TODO: Throw an exception */                                                  \
			ASSERT( FALSE );                                                                \
			return -1;                                                                      \
		}                                                                                   \
		new_v = EC_ARRAYGET(promoted, 0);                                                   \
		w     = EC_ARRAYGET(promoted, 1);                                                   \
		if (v != new_v)                                                                     \
		{                                                                                   \
			v = new_v;                                                                      \
			if ((! EC_USERP(v)) && (! EC_USERP(w)))                                         \
			{                                                                               \
				if (reversed)                                                               \
					*result = (*func)(w, v);                                                \
				else                                                                        \
					*result = (*func)(v, w);                                                \
				return EC_ERRORP(*result) ? -1 : 0;                                         \
			}                                                                               \
		}                                                                                   \
	}                                                                                       \
goon:                                                                                       \
	if (CB_NUMERIC(v)->CB_FCN(oper))                                                        \
	{                                                                                       \
		*result = CB_NUMERIC(v)->CB_FCN(oper)(v, w);                                        \
   		if (EC_ERRORP(*result) && EcIsOfClass( PRIVATE(rt).exc, EcTypeErrorClass ))         \
			return 1;                                                                       \
		return EC_ERRORP(*result) ? -1 : 0;                                                 \
	}                                                                                       \
                                                                                            \
	return 1;                                                                               \
}

#define EMIT_HALFBINOP_USERFUNCS(oper) \
EMIT_HALFBINOP_USERFUNC(oper)          \
EMIT_HALFBINOP_USERFUNC(r##oper)

#define EMIT_BINOP(oper) \
EMIT_BINOP_FUNC(oper)          \
EMIT_BINOP_USERFUNC(oper)      \
EMIT_HALFBINOP_USERFUNCS(oper)

#define EMIT_UNARYOP_FUNC(oper) \
EC_OBJ UNARYOPFUNC(oper)(EC_OBJ v)                    \
{                                                     \
    if (! IS_USER_NUMERIC(v)) return basic_##oper(v); \
	if (CB_NUMERIC(v)->CB_FCN(oper))                  \
		return CB_NUMERIC(v)->CB_FCN(oper)(v);        \
	return EcUnknownOperationError( #oper, 1, v );    \
}

#define EMIT_UNARYOP(oper) \
EMIT_UNARYOP_FUNC(oper)    \


/* ------------------------------------------------------------------------
 * EMISSION OF FUNCTIONS
 * ------------------------------------------------------------------------ */

EMIT_BINOP(add)
EMIT_BINOP(sub)
EMIT_BINOP(mul)
EMIT_BINOP(div)
EMIT_BINOP(mod)
EMIT_BINOP(pow)
EMIT_BINOP(lshift)
EMIT_BINOP(rshift)
EMIT_BINOP(and)
EMIT_BINOP(or)
EMIT_BINOP(xor)
EMIT_UNARYOP(neg)
EMIT_UNARYOP(pos)
EMIT_UNARYOP(abs)
EMIT_UNARYOP(invert)
EMIT_UNARYOP(int)
EMIT_UNARYOP(float)
EMIT_UNARYOP(true)


/* ------------------------------------------------------------------------
 * BASIC ARITHMETIC FUNCTIONS
 * ------------------------------------------------------------------------ */

static EC_OBJ basic_add( EC_OBJ obj1, EC_OBJ obj2 )
{
	switch (EC_TYPE(obj1))
	{
	case tc_inum:
		ASSERT( EC_INUMP(obj2) );
#if EC_INLINED_TYPES
		return EC_INUM_ADD(obj1, obj2);
#else
		return EcMakeInt( EC_INUM(obj1) + EC_INUM(obj2) );
#endif
		break;

	case tc_fnum:
		ASSERT( EC_FNUMP(obj2) );
		return EcMakeFloat( EC_FNUM(obj1) + EC_FNUM(obj2) );
		break;

	default:
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
		break;
	}
}

static EC_OBJ basic_sub( EC_OBJ obj1, EC_OBJ obj2 )
{
	switch (EC_TYPE(obj1))
	{
	case tc_inum:
		ASSERT( EC_INUMP(obj2) );
#if EC_INLINED_TYPES
		return EC_INUM_SUB(obj1, obj2);
#else
		return EcMakeInt( EC_INUM(obj1) - EC_INUM(obj2) );
#endif
		break;

	case tc_fnum:
		ASSERT( EC_FNUMP(obj2) );
		return EcMakeFloat( EC_FNUM(obj1) - EC_FNUM(obj2) );
		break;

	default:
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
		break;
	}
}

static EC_OBJ basic_mul( EC_OBJ obj1, EC_OBJ obj2 )
{
	switch (EC_TYPE(obj1))
	{
	case tc_inum:
		ASSERT( EC_INUMP(obj2) );
		return EcMakeInt( EC_INUM(obj1) * EC_INUM(obj2) );
		break;

	case tc_fnum:
		ASSERT( EC_FNUMP(obj2) );
		return EcMakeFloat( EC_FNUM(obj1) * EC_FNUM(obj2) );
		break;

	default:
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
		break;
	}
}

static EC_OBJ basic_div( EC_OBJ obj1, EC_OBJ obj2 )
{
	/*
	 * XXX TODO: Check for division by zero (and throw an exception)
	 */
	switch (EC_TYPE(obj1))
	{
	case tc_inum:
		ASSERT( EC_INUMP(obj2) );
		return EcMakeInt( EC_INUM(obj1) / EC_INUM(obj2) );
		break;

	case tc_fnum:
		ASSERT( EC_FNUMP(obj2) );
		return EcMakeFloat( EC_FNUM(obj1) / EC_FNUM(obj2) );
		break;

	default:
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
		break;
	}
}

static EcFloat _mod( EcFloat a, EcFloat b )
{
	EcInt q;

	if ((a >= 0.0) && (a <= b))
		return a;

	q = (EcInt) (a / b);

	a -= (EcFloat)q * b;
	if (a < 0.0)
		a += b;

	return a;
}

static EC_OBJ basic_mod( EC_OBJ obj1, EC_OBJ obj2 )
{
	/*
	 * XXX TODO: Check for division by zero (and throw an exception)
	 */
	switch (EC_TYPE(obj1))
	{
	case tc_inum:
		ASSERT( EC_INUMP(obj2) );
		return EcMakeInt( (EcInt)_mod( (EcFloat)EC_INUM(obj1), (EcFloat)EC_INUM(obj2) ) );
		break;

	case tc_fnum:
		ASSERT( EC_FNUMP(obj2) );
		return EcMakeFloat( _mod( EC_FNUM(obj1), EC_FNUM(obj2) ) );
		break;

	default:
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
		break;
	}
}

static EC_OBJ basic_pow( EC_OBJ obj1, EC_OBJ obj2 )
{
	switch (EC_TYPE(obj1))
	{
	case tc_inum:
		ASSERT( EC_INUMP(obj2) );
		return EcMakeInt( (EcInt) pow( (double)EC_INUM(obj1), (double)EC_INUM(obj2) ) );
		break;

	case tc_fnum:
		ASSERT( EC_FNUMP(obj2) );
		return EcMakeFloat( pow( EC_FNUM(obj1), EC_FNUM(obj2) ) );
		break;

	default:
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
		break;
	}
}

static EC_OBJ basic_lshift( EC_OBJ obj1, EC_OBJ obj2 )
{
	switch (EC_TYPE(obj1))
	{
	case tc_inum:
		ASSERT( EC_INUMP(obj2) );
		return EcMakeInt( EC_INUM(obj1) << EC_INUM(obj2) );
		break;

	default:
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
		break;
	}
}

static EC_OBJ basic_rshift( EC_OBJ obj1, EC_OBJ obj2 )
{
	switch (EC_TYPE(obj1))
	{
	case tc_inum:
		ASSERT( EC_INUMP(obj2) );
		return EcMakeInt( EC_INUM(obj1) >> EC_INUM(obj2) );
		break;

	default:
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
		break;
	}
}

static EC_OBJ basic_and( EC_OBJ obj1, EC_OBJ obj2 )
{
	switch (EC_TYPE(obj1))
	{
	case tc_inum:
		ASSERT( EC_INUMP(obj2) );
#if EC_INLINED_TYPES
		return EC_INUM_AND(obj1, obj2);
#else
		return EcMakeInt( EC_INUM(obj1) & EC_INUM(obj2) );
#endif
		break;

	default:
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
		break;
	}
}

static EC_OBJ basic_or( EC_OBJ obj1, EC_OBJ obj2 )
{
	switch (EC_TYPE(obj1))
	{
	case tc_inum:
		ASSERT( EC_INUMP(obj2) );
#if EC_INLINED_TYPES
		return EC_INUM_OR(obj1, obj2);
#else
		return EcMakeInt( EC_INUM(obj1) | EC_INUM(obj2) );
#endif
		break;

	default:
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
		break;
	}
}

static EC_OBJ basic_xor( EC_OBJ obj1, EC_OBJ obj2 )
{
	switch (EC_TYPE(obj1))
	{
	case tc_inum:
		ASSERT( EC_INUMP(obj2) );
#if EC_INLINED_TYPES
		return EC_INUM_XOR(obj1, obj2);
#else
		return EcMakeInt( EC_INUM(obj1) ^ EC_INUM(obj2) );
#endif
		break;

	default:
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
		break;
	}
}

static EC_OBJ basic_neg( EC_OBJ obj )
{
	switch (EC_TYPE(obj))
	{
	case tc_inum:
		return EcMakeInt( - EC_INUM(obj) );
		break;

	case tc_fnum:
		return EcMakeFloat( - EC_FNUM(obj) );
		break;

	default:
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
		break;
	}
}

static EC_OBJ basic_pos( EC_OBJ obj )
{
	switch (EC_TYPE(obj))
	{
	case tc_inum:
	case tc_fnum:
		return obj;
		break;

	default:
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
		break;
	}
}

static EC_OBJ basic_abs( EC_OBJ obj )
{
	switch (EC_TYPE(obj))
	{
	case tc_inum:
		return EcMakeInt( EC_INUM(obj) >= 0 ? EC_INUM(obj) : -EC_INUM(obj) );
		break;

	case tc_fnum:
		return EcMakeFloat( EC_FNUM(obj) >= 0 ? EC_FNUM(obj) : -EC_FNUM(obj) );
		break;

	default:
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
		break;
	}
}

static EC_OBJ basic_invert( EC_OBJ obj )
{
	switch (EC_TYPE(obj))
	{
	case tc_inum:
		return EcMakeInt( ~ EC_INUM(obj) );
		break;

	default:
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
		break;
	}
}

static EC_OBJ basic_int( EC_OBJ obj )
{
	if (EC_NULLP(obj))
		return EcMakeInt( 0 );

	switch (EC_TYPE(obj))
	{
	case tc_none:
		ASSERT( FALSE );
		return EcMakeInt( 0 );
		break;

	case tc_error:
		ASSERT( FALSE );
		return EcMakeInt( 0 );
		break;

	case tc_bool:
		return EcMakeInt( EC_BOOLVAL(obj) );
		break;

	case tc_inum:
		return obj;
		break;

	case tc_fnum:
		return EcMakeInt( (EcInt) EC_FNUM(obj) );
		break;

	default:
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
		break;
	}
}

static EC_OBJ basic_float( EC_OBJ obj )
{
	if (EC_NULLP(obj))
		return EcMakeFloat( 0.0 );

	switch (EC_TYPE(obj))
	{
	case tc_none:
		ASSERT( FALSE );
		return EcMakeFloat( 0.0 );
		break;

	case tc_error:
		ASSERT( FALSE );
		return EcMakeFloat( 0.0 );
		break;

	case tc_bool:
		return EcMakeFloat( (EcFloat) EC_BOOLVAL(obj) );
		break;

	case tc_inum:
		return EcMakeFloat( (EcFloat) EC_INUM(obj) );
		break;

	case tc_fnum:
		return obj;
		break;

	default:
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
		break;
	}
}

static EC_OBJ basic_true( EC_OBJ obj )
{
	if (EC_NULLP(obj)) return EcFalseObject;

	switch (EC_TYPE(obj))
	{
	case tc_none:
	case tc_error:
		return EcFalseObject;
		break;

	case tc_bool:
		return EC_BOOLVAL(obj) ? EcTrueObject : EcFalseObject;
		break;

	case tc_inum:
		return EC_INUM(obj) ? EcTrueObject : EcFalseObject;
		break;

	case tc_fnum:
		return (EC_FNUM(obj) != 0.0) ? EcTrueObject : EcFalseObject;
		break;

	default:
		return EcTrueObject;
		break;
	}
}
