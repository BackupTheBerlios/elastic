/* ==========================================================================
 * numeric.c
 * ==========================================================================
 * Numerical data-types handling.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Wed Dec 30 12:56:34 CET 1998
 *
 *   $Id: numeric.c,v 1.1 2002/05/23 21:17:24 panta Exp $
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

#include "basic.h"
#include "elastic.h"
#include "debug.h"

#include "private.h"

/* Numerical */

EC_API EcBool EcIsNumeric( EC_OBJ obj )
{
	if (EC_INUMP(obj))
		return TRUE;

	if (EC_FNUMP(obj))
		return TRUE;

	return FALSE;
}

EC_API EcBool EcPromote( EC_OBJ *num1, EC_OBJ *num2 )
{
	switch (EC_TYPE(*num1))
	{
	case tc_inum:
		switch (EC_TYPE(*num2))
		{
		case tc_inum:
			return TRUE;
			break;

		case tc_fnum:
			*num1 = EcMakeFloat( EC_INUM(*num1) );
			return TRUE;
			break;

		default:
			if (IS_USER_NUMERIC(*num2))
			{
				if (CB_NUMERIC(*num2)->promote_fcn)
				{
					EC_OBJ promoted;

					promoted = CB_NUMERIC(*num2)->promote_fcn( *num2, *num1 );
					if ((! EC_ARRAYP(promoted)) || (EC_ARRAYLEN(promoted) != 2))
						return FALSE;
					*num2 = EC_ARRAYGET(promoted, 0);
					*num1 = EC_ARRAYGET(promoted, 1);
					return TRUE;
				}
			}
			return FALSE;
			break;
		}
		break;

	case tc_fnum:
		switch (EC_TYPE(*num2))
		{
		case tc_inum:
			*num2 = EcMakeFloat( EC_INUM(*num2) );
			return TRUE;
			break;

		case tc_fnum:
			return TRUE;
			break;

		default:
			if (IS_USER_NUMERIC(*num2))
			{
				if (CB_NUMERIC(*num2)->promote_fcn)
				{
					EC_OBJ promoted;

					promoted = CB_NUMERIC(*num2)->promote_fcn( *num2, *num1 );
					if ((! EC_ARRAYP(promoted)) || (EC_ARRAYLEN(promoted) != 2))
						return FALSE;
					*num2 = EC_ARRAYGET(promoted, 0);
					*num1 = EC_ARRAYGET(promoted, 1);
					return TRUE;
				}
			}
			return FALSE;
			break;
		}
		break;

	default:
		if (IS_USER_NUMERIC(*num1))
		{
			if (CB_NUMERIC(*num1)->promote_fcn)
			{
				EC_OBJ promoted;

				promoted = CB_NUMERIC(*num1)->promote_fcn( *num1, *num2 );
				if (EC_ARRAYP(promoted) && (EC_ARRAYLEN(promoted) == 2))
				{
					*num1 = EC_ARRAYGET(promoted, 0);
					*num2 = EC_ARRAYGET(promoted, 1);
					return TRUE;
				}
			}
		}
		if (IS_USER_NUMERIC(*num2))
		{
			if (CB_NUMERIC(*num2)->promote_fcn)
			{
				EC_OBJ promoted;

				promoted = CB_NUMERIC(*num2)->promote_fcn( *num2, *num1 );
				if (EC_ARRAYP(promoted) && (EC_ARRAYLEN(promoted) == 2))
				{
					*num2 = EC_ARRAYGET(promoted, 0);
					*num1 = EC_ARRAYGET(promoted, 1);
					return TRUE;
				}
			}
		}
		return FALSE;
		break;
	}
}

EC_API EC_OBJ EcLibLt( EC_OBJ obj1, EC_OBJ obj2 )
{
	EcBool  ok;
	EcInt res;

	if (EC_INUMP(obj1) && EC_INUMP(obj2))
	{
		return EC_INUM(obj1) < EC_INUM(obj2) ? EcTrueObject : EcFalseObject;
	}

	ok = EcObjectCompare( obj1, obj2, &res );
	if (! ok)
	{
		return EcUnknownOperationError("<", 2, obj1, obj2);
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}
	return (res < 0) ? EcTrueObject : EcFalseObject;
}

EC_API EC_OBJ EcLibGt( EC_OBJ obj1, EC_OBJ obj2 )
{
	EcBool  ok;
	EcInt res;

	ok = EcObjectCompare( obj1, obj2, &res );
	if (! ok)
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}
	return (res > 0) ? EcTrueObject : EcFalseObject;
}

EC_API EC_OBJ EcLibLe( EC_OBJ obj1, EC_OBJ obj2 )
{
	EcBool  ok;
	EcInt res;

	ok = EcObjectCompare( obj1, obj2, &res );
	if (! ok)
	{
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}
	return (res <= 0) ? EcTrueObject : EcFalseObject;
}

EC_API EC_OBJ EcLibGe( EC_OBJ obj1, EC_OBJ obj2 )
{
	EcBool  ok;
	EcInt res;

	ok = EcObjectCompare( obj1, obj2, &res );
	if (! ok)
	{
		return EcUnknownOperationError(">=", 2, obj1, obj2);
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
	}
	return (res >= 0) ? EcTrueObject : EcFalseObject;
}

EC_API EC_OBJ EcLibEq( EC_OBJ obj1, EC_OBJ obj2 )
{
	EcBool r;

	if (obj1 == obj2)
		return EcTrueObject;

	if (EC_NULLP(obj1) || EC_NULLP(obj2))
		return EcFalseObject;

	switch (EC_TYPE(obj1))
	{
	case tc_inum:
		switch (EC_TYPE(obj2))
		{
		case tc_inum:
			return EC_INUM(obj1) == EC_INUM(obj2) ? EcTrueObject : EcFalseObject;
			break;

		case tc_fnum:
			return (EcFloat)EC_INUM(obj1) == EC_FNUM(obj2) ? EcTrueObject : EcFalseObject;
			break;

		default:
			return EcObjectEqual( obj1, obj2 ) ? EcTrueObject : EcFalseObject;
			break;
		}
		break;

	case tc_fnum:
		switch (EC_TYPE(obj2))
		{
		case tc_inum:
			return EC_FNUM(obj1) == (EcFloat)EC_INUM(obj2) ? EcTrueObject : EcFalseObject;
			break;

		case tc_fnum:
			return EC_FNUM(obj1) == EC_FNUM(obj2) ? EcTrueObject : EcFalseObject;
			break;

		default:
			return EcObjectEqual( obj1, obj2 ) ? EcTrueObject : EcFalseObject;
			break;
		}
		break;

	default:
		r = EcObjectEqual( obj1, obj2 );
		return r ? EcTrueObject : EcFalseObject;
		break;
	}
}

EC_API EC_OBJ EcLibNe( EC_OBJ obj1, EC_OBJ obj2 )
{
	EC_OBJ res;

	res = EcLibEq( obj1, obj2 );
	if (EC_ERRORP(res))
		return res;
	return res == EcTrueObject ? EcFalseObject : EcTrueObject;
}

EC_API EC_OBJ EcLibInc( EC_OBJ obj )
{
	switch (EC_TYPE(obj))
	{
	case tc_inum:
		return EcMakeInt( EC_INUM(obj) + 1  );
		break;

	case tc_fnum:
		return EcMakeFloat( EC_FNUM(obj) + 1.0 );
		break;

	default:
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
		break;
	}
}

EC_API EC_OBJ EcLibDec( EC_OBJ obj )
{
	switch (EC_TYPE(obj))
	{
	case tc_inum:
		return EcMakeInt( EC_INUM(obj) - 1  );
		break;

	case tc_fnum:
		return EcMakeFloat( EC_FNUM(obj) - 1.0 );
		break;

	default:
		/* TODO: Throw an exception */
		ASSERT( FALSE );
		return Ec_ERROR;
		break;
	}
}
