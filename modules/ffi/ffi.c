/* ==========================================================================
 * ffi.c
 * ==========================================================================
 * FFI (Foreign Function Interface) module for elastiC.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Thu Sep 27 00:51:40 CEST 2001
 *
 *   $Id: ffi.c,v 1.1 2002/05/30 17:00:15 panta Exp $
 * --------------------------------------------------------------------------
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

#include "config.h"

#include <stdio.h>
#include <stdint.h>
#include <ffi.h>
#if HAVE_STRING_H
#include <string.h>
#endif
#if HAVE_STRINGS_H
#include <strings.h>
#endif
/* #include <dlfcn.h> */

#include <elastic/basic.h>
#include <elastic/debug.h>
#include <elastic/elastic.h>
#include <elastic/memory.h>

#include "ecconfig.h"
#include "ffi.h"

/*
 * Dynamic Library Handle type
 *
 * elastiC objects of this type contain the handle
 * needed to obtain the address of symbols in shared
 * objects (like dynamically loaded libraries).
 */

static EcInt tc_dlh = -1;										/* dynamic library handle type */

#define EC_DLHP(obj)			(EC_TYPEP(obj,tc_dlh))
#define EC_DLH(obj)				(EC_USER0(obj, ec_dlhandle))			/* dl handle */
#define EC_DLH_NEEDSCLOSE(obj)	(EC_USER1(obj, EcBool))					/* handle needs to be dlclose()d ? */

static EC_OBJ dlh_copy( EC_OBJ obj, EcCopyType type );
static void   dlh_mark( EC_OBJ obj );
static void   dlh_free( EC_OBJ obj );
static EcInt  dlh_print( ec_string *str, EC_OBJ obj, EcBool detailed );
static EcUInt dlh_hash( EC_OBJ obj, EcInt recur );

/*
 * FFI Hadle type
 *
 * elastiC objects of this type contain information about
 * a C function, such as its prototype, address (function pointer)
 * and ffi cif.
 */

/*
 * We use libffi. Our equivalent of a libffi cif is a ffih (FFI Handle)
 * However an ffih contains also the pointer to the function to call.
 */
static EcInt tc_ffih = -1;										/* FFI handle type */

#define EC_FFIHP(obj)		(EC_TYPEP(obj,tc_ffih))
#define EC_FFI_CIF(obj)		(EC_USER0(obj, ffi_cif *))			/* libffi cif                       */
#define EC_FFI_FCN(obj)		(EC_USER1(obj, ec_dlptr))			/* libffi function pointer          */
#define EC_FFI_PROT(obj)	(EC_USER2(obj, EC_OBJ))				/* prototype (as an elastiC object) */

static EC_OBJ ffih_copy( EC_OBJ obj, EcCopyType type );
static void   ffih_mark( EC_OBJ obj );
static void   ffih_free( EC_OBJ obj );
static EcInt  ffih_print( ec_string *str, EC_OBJ obj, EcBool detailed );
static EcUInt ffih_hash( EC_OBJ obj, EcInt recur );

static EcUInt t_void_id       = 0;
static EcUInt t_uint8_id      = 0;
static EcUInt t_sint8_id      = 0;
static EcUInt t_uint16_id     = 0;
static EcUInt t_sint16_id     = 0;
static EcUInt t_uint32_id     = 0;
static EcUInt t_sint32_id     = 0;
static EcUInt t_uint64_id     = 0;
static EcUInt t_sint64_id     = 0;
static EcUInt t_float_id      = 0;
static EcUInt t_double_id     = 0;
static EcUInt t_longdouble_id = 0;
static EcUInt t_pointer_id    = 0;
static EcUInt t_ushort_id     = 0;
static EcUInt t_sint_id       = 0;
static EcUInt t_uint_id       = 0;
static EcUInt t_slong_id      = 0;
static EcUInt t_ulong_id      = 0;

static EcUInt t_int8_id       = 0;
static EcUInt t_int16_id      = 0;
static EcUInt t_int32_id      = 0;
static EcUInt t_int64_id      = 0;
static EcUInt t_int_id        = 0;
static EcUInt t_short_id      = 0;
static EcUInt t_long_id       = 0;
static EcUInt t_string_id     = 0;
static EcUInt t_string_dyn_id = 0;

/* C API */

static EC_OBJ ffi_marshaler( EC_OBJ stack, EcAny userdata );
/* static EC_OBJ perform_ffi_call( EC_OBJ ffih, EC_OBJ stack ); */

static EC_OBJ EcMakeDLH( ec_dlhandle handle, EcBool needs_close )
{
	EC_OBJ res;

	res = EcMakeUser( tc_dlh, NULL );
	if (EC_ERRORP(res)) return res;

	EC_DLH(res) = handle;
	EC_DLH_NEEDSCLOSE(res) = needs_close;
	return res;
}

static EC_OBJ EcMakeFFIH( ffi_cif *cif, ec_dlptr fcn, EcUInt retval_id, EC_OBJ params )
{
	EC_OBJ res;

	res = EcMakeUser( tc_ffih, NULL );
	if (EC_ERRORP(res)) return res;

	EC_FFI_CIF(res) = cif;
	EC_FFI_FCN(res) = fcn;

	EC_FFI_PROT(res) = EcMakeArrayInit( 2,
										EcMakeSymbolFromId( retval_id ),
										EcDeepCopy( params ) );
	return res;
}

/* elastiC API */

static EC_OBJ EcLibFFI_Open( EC_OBJ stack, EcAny userdata )
{
	char *libname = NULL;
	EC_OBJ res;

	res = EcParseStackFunction( "ffi.open", TRUE, stack, "z", &libname );
	if (EC_ERRORP(res))
		return res;

	return EcMakeDLH( EcDLOpen( libname ),
					  (libname == NULL) ? FALSE : TRUE );
}

static EC_OBJ EcLibFFI_Close( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ dlh;
	EC_OBJ res;

	res = EcParseStackFunction( "ffi.close", TRUE, stack, "O!", tc_dlh, &dlh );
	if (EC_ERRORP(res))
		return res;

	if (EC_DLH(dlh) && EC_DLH_NEEDSCLOSE(dlh))
		EcDLClose( EC_DLH(dlh) );
	EC_DLH(dlh) = NULL;
	return EC_NIL;
}

static ffi_type *symbol_to_ffi_type( EcUInt symbol_id )
{
	if      (symbol_id == t_void_id)		return &ffi_type_void;
	else if (symbol_id == t_uint8_id)		return &ffi_type_uint8;
	else if (symbol_id == t_sint8_id)		return &ffi_type_sint8;
	else if (symbol_id == t_uint16_id)		return &ffi_type_uint16;
	else if (symbol_id == t_sint16_id)		return &ffi_type_sint16;
	else if (symbol_id == t_uint32_id)		return &ffi_type_uint32;
	else if (symbol_id == t_sint32_id)		return &ffi_type_sint32;
	else if (symbol_id == t_uint64_id)		return &ffi_type_uint64;
	else if (symbol_id == t_sint64_id)		return &ffi_type_sint64;
	else if (symbol_id == t_float_id)		return &ffi_type_float;
	else if (symbol_id == t_double_id)		return &ffi_type_double;
	else if (symbol_id == t_longdouble_id)	return &ffi_type_longdouble;
	else if (symbol_id == t_pointer_id)		return &ffi_type_pointer;
	else if (symbol_id == t_ushort_id)		return &ffi_type_ushort;
	else if (symbol_id == t_sint_id)		return &ffi_type_sint;
	else if (symbol_id == t_uint_id)		return &ffi_type_uint;
	else if (symbol_id == t_slong_id)		return &ffi_type_slong;
	else if (symbol_id == t_ulong_id)		return &ffi_type_ulong;

	else if (symbol_id == t_int8_id)		return &ffi_type_sint8;
	else if (symbol_id == t_int16_id)		return &ffi_type_sint16;
	else if (symbol_id == t_int32_id)		return &ffi_type_sint32;
	else if (symbol_id == t_int64_id)		return &ffi_type_sint64;
	else if (symbol_id == t_int_id)			return &ffi_type_sint;
	else if (symbol_id == t_short_id)
	{
#if SIZEOF_SHORT == 1
		return &ffi_type_sint8;
#elif SIZEOF_SHORT == 2
		return &ffi_type_sint16;
#elif SIZEOF_SHORT == 4
		return &ffi_type_sint32;
#elif SIZEOF_SHORT == 8
		return &ffi_type_sint64;
#else
		return &ffi_type_sint;
#endif
	}
	else if (symbol_id == t_long_id)		return &ffi_type_slong;
	else if (symbol_id == t_string_id)		return &ffi_type_pointer;
	else if (symbol_id == t_string_dyn_id)	return &ffi_type_pointer;

	return NULL;
}

/*
static EC_OBJ ffi_marshaler( EC_OBJ stack, EcAny userdata )
{
	return perform_ffi_call( (EC_OBJ)userdata, stack );
}
*/

static EC_OBJ EcLibFFI_Declare( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ    dlh;
	char     *symbol = NULL;
	EcUInt    retval_id, arg_id;
	EC_OBJ    params, arg_obj;
	ec_dlptr  fcn;

	EcInt nargs, i;
	ffi_type  *ffiret;
	ffi_type **ffiargs;

	ffi_cif *cif;

	char *prim_name;

	EC_OBJ ffih;
	EC_OBJ res;


	res = EcParseStackFunction( "ffi.declare", TRUE, stack, "O!skO!",
								tc_dlh, &dlh,
								&symbol,
								&retval_id,
								tc_array, &params );
	if (EC_ERRORP(res))
		return res;

	fcn = EcDLSymbol( EC_DLH(dlh), symbol );
	if (! fcn)
	{
		char *errmsg;
		const char *dlerrmsg;

#define ERROR_PREFIX	"symbol not found :"
		dlerrmsg = EcDLError();
		errmsg = alloca( strlen(ERROR_PREFIX) + strlen( dlerrmsg ) + 1 );
		if (! errmsg)
			return EcUnimplementedError("symbol not found");	/* we should use the (non-existent) EcValueError() */

		strcpy(errmsg, ERROR_PREFIX);
		strcat(errmsg, dlerrmsg);
		return EcUnimplementedError(errmsg);					/* we should use the (non-existent) EcValueError() */
	}

	cif = ec_malloc( sizeof(ffi_cif) );
	if (! cif)
		return EcMemoryError();

	nargs = EC_ARRAYLEN(params);
	ffiargs = ec_malloc(sizeof(ffi_type *) * nargs);
	if (! ffiargs) return EcMemoryError();

	ffiret = symbol_to_ffi_type(retval_id);
	if (! ffiret)
		return EcUnimplementedError("invalid return type specified");		/* we should use the (non-existent) EcValueError() */
	for (i = 0; i < nargs; i++)
	{
		arg_obj = EcArrayGet( params, i );

		if (EC_SYMBOLP(arg_obj))
		{
			arg_id = EC_SYMBOL(arg_obj);
			ffiargs[i] = symbol_to_ffi_type(arg_id);
			if (! ffiargs[i])
				return EcUnimplementedError("invalid type specified");		/* we should use the (non-existent) EcValueError() */
		} else if (EC_ARRAYP(arg_obj))
		{
			return EcUnimplementedError("sub-structures not yet implemented");
			ASSERT( FALSE );									/* TODO */
		} else
			return EcUnimplementedError("expected a symbol or an array");	/* we should use the (non-existent) EcValueError() */
	}

	if (ffi_prep_cif( cif, FFI_DEFAULT_ABI, nargs, ffiret, ffiargs) != FFI_OK)
		return EcUnimplementedError( "ffi_prep_cif failed" );	/* TODO: fix exception */

	/* Make a new primitive function to call this FFI-accessed C function */
	ffih = EcMakeFFIH( cif, fcn, retval_id, params );
	if (EC_ERRORP(ffih)) return ffih;

	/* TODO: add to the primitive name a distinctive prefix based on the shared module */
	prim_name = alloca(5 + strlen(symbol) + 1);
	strcpy(prim_name, "ffi::");
	strcat(prim_name, symbol);
	return EcMakePrimitive( ffi_marshaler, prim_name, ffih, TRUE );
}

static EC_OBJ ec_to_c( EcInt arg_i, EcUInt type_id, EC_OBJ obj, void **c_dst, ec_mempool mpool )
{
#define ASSIGN_DST(c_dst, c_type, ptr)		(*(((c_type) **)(c_dst)) = (ptr))

#define PERFORM_ASSIGNMENT(c_type, c_value)					\
	do {													\
		c_type *ptr = ec_palloc( mpool, sizeof(c_type) );	\
		*ptr = (c_value);									\
		*((c_type **)c_dst) = ptr;							\
	} while (0)

	if (type_id == t_void_id)
	{
		if (! EC_NULLP(obj))
			return EcTypeError( EC_NIL, EC_NIL, arg_i,
								tc_none, obj, TRUE, "expected @nil" );
		*c_dst = NULL;
	}
	else if (type_id == t_uint8_id)
	{
		if (! EC_INUMP(obj))
			return EcTypeError( EC_NIL, EC_NIL, arg_i,
								tc_inum, obj, TRUE, NULL );
		PERFORM_ASSIGNMENT(uint8_t, EC_INUM(obj));
	}
	else if ((type_id == t_sint8_id) || (type_id == t_int8_id)
#if SIZEOF_SHORT == 1
			 || (type_id == t_short_id)
#endif
		)
	{
		if (! EC_INUMP(obj))
			return EcTypeError( EC_NIL, EC_NIL, arg_i,
								tc_inum, obj, TRUE, NULL );
		PERFORM_ASSIGNMENT(int8_t, EC_INUM(obj));
	}
	else if (type_id == t_uint16_id)
	{
		if (! EC_INUMP(obj))
			return EcTypeError( EC_NIL, EC_NIL, arg_i,
								tc_inum, obj, TRUE, NULL );
		PERFORM_ASSIGNMENT(uint16_t, EC_INUM(obj));
	}
	else if ((type_id == t_sint16_id) || (type_id == t_int16_id)
#if SIZEOF_SHORT == 2
			 || (type_id == t_short_id)
#endif
		)
	{
		if (! EC_INUMP(obj))
			return EcTypeError( EC_NIL, EC_NIL, arg_i,
								tc_inum, obj, TRUE, NULL );
		PERFORM_ASSIGNMENT(int16_t, EC_INUM(obj));
	}
	else if (type_id == t_uint32_id)
	{
		if (! EC_INUMP(obj))
			return EcTypeError( EC_NIL, EC_NIL, arg_i,
								tc_inum, obj, TRUE, NULL );
		PERFORM_ASSIGNMENT(uint32_t, EC_INUM(obj));
	}
	else if ((type_id == t_sint32_id) || (type_id == t_int32_id)
#if SIZEOF_SHORT == 4
			 || (type_id == t_short_id)
#endif
		)
	{
		if (! EC_INUMP(obj))
			return EcTypeError( EC_NIL, EC_NIL, arg_i,
								tc_inum, obj, TRUE, NULL );
		PERFORM_ASSIGNMENT(int32_t, EC_INUM(obj));
	}
	else if (type_id == t_uint64_id)
	{
		if (! EC_INUMP(obj))
			return EcTypeError( EC_NIL, EC_NIL, arg_i,
								tc_inum, obj, TRUE, NULL );
		PERFORM_ASSIGNMENT(uint64_t, EC_INUM(obj));
	}
	else if ((type_id == t_sint64_id) || (type_id == t_int64_id)
#if SIZEOF_SHORT == 8
			 || (type_id == t_short_id)
#endif
		)
	{
		if (! EC_INUMP(obj))
			return EcTypeError( EC_NIL, EC_NIL, arg_i,
								tc_inum, obj, TRUE, NULL );
		PERFORM_ASSIGNMENT(int64_t, EC_INUM(obj));
	}
	else if (type_id == t_float_id)
	{
		if (! EC_FNUMP(obj))
			return EcTypeError( EC_NIL, EC_NIL, arg_i,
								tc_fnum, obj, TRUE, NULL );
#if EC_SIZEOF_ECFLOAT == SIZEOF_FLOAT
		*((float **)c_dst) = &EC_FNUM(obj);
#else
		PERFORM_ASSIGNMENT(float, EC_FNUM(obj));
#endif
	}
	else if (type_id == t_double_id)
	{
		if (! EC_FNUMP(obj))
			return EcTypeError( EC_NIL, EC_NIL, arg_i,
								tc_fnum, obj, TRUE, NULL );
#if EC_SIZEOF_ECFLOAT == SIZEOF_DOUBLE
		*((double **)c_dst) = &EC_FNUM(obj);
#else
		PERFORM_ASSIGNMENT(double, EC_FNUM(obj));
#endif
	}
	else if (type_id == t_longdouble_id)
	{
		if (! EC_INUMP(obj))
			return EcTypeError( EC_NIL, EC_NIL, arg_i,
								tc_inum, obj, TRUE, NULL );
#if EC_SIZEOF_ECFLOAT == SIZEOF_LONG_DOUBLE
		*((long double **)c_dst) = &EC_FNUM(obj);
#else
		PERFORM_ASSIGNMENT(long double, EC_INUM(obj));
#endif
	}
	else if (type_id == t_pointer_id)
	{
		/* TODO: expect an array of values (or an integer ???) */
		ASSERT( FALSE );
	}
	else if (type_id == t_ushort_id)
	{
		if (! EC_INUMP(obj))
			return EcTypeError( EC_NIL, EC_NIL, arg_i,
								tc_inum, obj, TRUE, NULL );
		PERFORM_ASSIGNMENT(unsigned short, EC_INUM(obj));
	}
	else if ((type_id == t_sint_id) || (type_id == t_int_id))
	{
		if (! EC_INUMP(obj))
			return EcTypeError( EC_NIL, EC_NIL, arg_i,
								tc_inum, obj, TRUE, NULL );
		PERFORM_ASSIGNMENT(int, EC_INUM(obj));
	}
	else if (type_id == t_uint_id)
	{
		if (! EC_INUMP(obj))
			return EcTypeError( EC_NIL, EC_NIL, arg_i,
								tc_inum, obj, TRUE, NULL );
		PERFORM_ASSIGNMENT(unsigned int, EC_INUM(obj));
	}
	else if ((type_id == t_slong_id) || (type_id == t_long_id))
	{
		if (! EC_INUMP(obj))
			return EcTypeError( EC_NIL, EC_NIL, arg_i,
								tc_inum, obj, TRUE, NULL );
		PERFORM_ASSIGNMENT(long, EC_INUM(obj));
	}
	else if (type_id == t_ulong_id)
	{
		if (! EC_INUMP(obj))
			return EcTypeError( EC_NIL, EC_NIL, arg_i,
								tc_inum, obj, TRUE, NULL );
		PERFORM_ASSIGNMENT(unsigned long, EC_INUM(obj));
	}
	else if ((type_id == t_string_id) || (type_id == t_string_dyn_id))
	{
		if (! EC_STRINGP(obj))
			return EcTypeError( EC_NIL, EC_NIL, arg_i,
								tc_string, obj, TRUE, NULL );
		PERFORM_ASSIGNMENT(void *, EC_STRDATA(obj));
	}

	return EcTypeError( EC_NIL, EC_NIL, arg_i,
						tc_none, obj, TRUE, "unexpected type specifier" );
}

static EC_OBJ c_to_ec( EcUInt type_id, void *c_val)
{
	if (type_id == t_void_id)
	{
		return EC_NIL;
	}
	else if (type_id == t_uint8_id)
	{
		return EcMakeInt( *((uint8_t *)c_val) );
	}
	else if ((type_id == t_sint8_id) || (type_id == t_int8_id)
#if SIZEOF_SHORT == 1
			 || (type_id == t_short_id)
#endif
		)
	{
		return EcMakeInt( *((int8_t *)c_val) );
	}
	else if (type_id == t_uint16_id)
	{
		return EcMakeInt( *((uint16_t *)c_val) );
	}
	else if ((type_id == t_sint16_id) || (type_id == t_int16_id)
#if SIZEOF_SHORT == 2
			 || (type_id == t_short_id)
#endif
		)
	{
		return EcMakeInt( *((int16_t *)c_val) );
	}
	else if (type_id == t_uint32_id)
	{
		return EcMakeInt( *((uint32_t *)c_val) );
	}
	else if ((type_id == t_sint32_id) || (type_id == t_int32_id)
#if SIZEOF_SHORT == 4
			 || (type_id == t_short_id)
#endif
		)
	{
		return EcMakeInt( *((int32_t *)c_val) );
	}
	else if (type_id == t_uint64_id)
	{
		return EcMakeInt( *((uint64_t *)c_val) );
	}
	else if ((type_id == t_sint64_id) || (type_id == t_int64_id)
#if SIZEOF_SHORT == 8
			 || (type_id == t_short_id)
#endif
		)
	{
		return EcMakeInt( *((int64_t *)c_val) );
	}
	else if (type_id == t_float_id)
	{
		return EcMakeFloat( *((float *)c_val) );
	}
	else if (type_id == t_double_id)
	{
		return EcMakeFloat( *((double *)c_val) );
	}
	else if (type_id == t_longdouble_id)
	{
		return EcMakeFloat( *((long double *)c_val) );
	}
	else if (type_id == t_pointer_id)
	{
		/* TODO: expect an array of values (or an integer ???) */
		ASSERT( FALSE );
	}
	else if (type_id == t_ushort_id)
	{
		return EcMakeInt( *((unsigned short *)c_val) );
	}
	else if ((type_id == t_sint_id) || (type_id == t_int_id))
	{
		return EcMakeInt( *((int *)c_val) );
	}
	else if (type_id == t_uint_id)
	{
		return EcMakeInt( *((unsigned int *)c_val) );
	}
	else if ((type_id == t_slong_id) || (type_id == t_long_id))
	{
		return EcMakeInt( *((long *)c_val) );
	}
	else if (type_id == t_ulong_id)
	{
		return EcMakeInt( *((unsigned long *)c_val) );
	}
	else if (type_id == t_string_id)
	{
		return EcMakeString( *((char **)c_val), -1 );
	}
	else if (type_id == t_string_dyn_id)
	{
		EC_OBJ res = EcMakeString( *((char **)c_val), -1 );
		free( *((char **)c_val) );
		return res;
	} else
	{
		return EcUnimplementedError("unexpected type specifier"); /* TODO: fix exception */
	}

	return EcUnimplementedError("unexpected type specifier");	/* TODO: fix exception */
}

static EC_OBJ ffi_marshaler( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ ffih = (EC_OBJ)userdata;

	EC_OBJ rettype;
	EC_OBJ params;
	EcInt  nparams, nargs, i;

	EC_OBJ *stack_argp;
	EC_OBJ arg;

	EcUInt rettype_id, type_id;

	void **values;
	union {
		uint64_t u64;
		long double ld;
	} rvalue __attribute__((aligned(8)));

	ec_mempool mpool;

	rettype = EC_ARRAYMEM(EC_FFI_PROT(ffih))[0];
	params  = EC_ARRAYMEM(EC_FFI_PROT(ffih))[1];

	rettype_id = EC_SYMBOL(rettype);
	nparams = EC_ARRAYLEN(params);

	nargs = EC_STACKNARGS_REAL(stack);
	stack_argp = EC_STACKPOINTER(stack) - nargs;

	if (nargs != nparams)
		return EcParameterCountError( EC_NIL, EC_NIL, nparams, stack );

	values = malloc(sizeof(void *) * nargs);
	if (! values) return EcMemoryError();

	/* TODO: get correct size for all parameters */
	mpool = ec_allocpool( nparams * 4, -1 );					/* approx. */
	if (! mpool) return EcMemoryError();

	for (i = 0; i < nparams; i++)
	{
		arg = *stack_argp++;
		type_id = EC_SYMBOL(EC_ARRAYMEM(params)[i]);
		ec_to_c( i, type_id, arg, &values[i], mpool );
	}

	/* consume used arguments */
	EC_STACKPOINTER(stack) -= nparams;

	ffi_call( EC_FFI_CIF(ffih), EC_FFI_FCN(ffih),
			  &rvalue, values );

	ec_freepool( mpool );

	return c_to_ec( rettype_id, &rvalue );
}

/* Private */

/* dlh */

static EC_OBJ dlh_copy( EC_OBJ obj, EcCopyType type )
{
	ASSERT( EC_DLHP(obj) );

	/* We can't make a copy, because a GC would dlclose() the thing multiple times ... */
	return obj;													/* ???? Is it correct to return the *same* object ??? */
}

static void dlh_mark( EC_OBJ obj )
{
	ASSERT( EC_DLHP(obj) );
}

static void dlh_free( EC_OBJ obj )
{
	ASSERT( EC_DLHP(obj) );

	if (EC_DLH(obj) && EC_DLH_NEEDSCLOSE(obj))
	{
		EcDLClose( EC_DLH(obj) );
		EC_DLH(obj) = NULL;
	}
	EC_DLH(obj) = NULL;
}

static EcInt dlh_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	ASSERT( EC_DLHP(obj) );

	if (detailed)
	{
		return ec_sprintf( str, "<ffi-dlh %p>",
						   EC_DLH(obj) );
	}
	else
		return ec_sprintf( str, "<ffi-dlh>" );
}

static EcUInt dlh_hash( EC_OBJ obj, EcInt recur )
{
	ASSERT( EC_DLHP(obj) );

	/* Equivalent object MUST have the same hash */
	return (EcUInt)EC_DLH(obj);									/* this is all we can do ... */
}

/* ffih */

static EC_OBJ ffih_copy( EC_OBJ obj, EcCopyType type )
{
	ASSERT( EC_FFIHP(obj) );

	/*
	 * We can't make a copy, because a ffi_cif is malloc()ed by
	 * the caller of ffi_prep_cif and free()d by ffih_free().
	 * We can't malloc() and memcpy() here neither.
	 */
	return obj;													/* ???? Is it correct to return the *same* object ??? */
}

static void ffih_mark( EC_OBJ obj )
{
	ASSERT( EC_FFIHP(obj) );
	if (EC_NNULLP(EC_FFI_PROT(obj)))
		EcMarkObject( EC_FFI_PROT(obj) );
}

static void ffih_free( EC_OBJ obj )
{
	ASSERT( EC_FFIHP(obj) );

	if (EC_FFI_CIF(obj))
	{
		ec_free( EC_FFI_CIF(obj) );
		EC_FFI_CIF(obj)  = NULL;
		EC_FFI_PROT(obj) = EC_NIL;
	}
}

static EcInt ffih_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	ASSERT( EC_FFIHP(obj) );

	if (detailed)
	{
		return ec_sprintf( str, "<ffi-ffih %p function: %p %w>",
						   EC_FFI_CIF(obj), EC_FFI_FCN(obj), EC_FFI_PROT(obj) );
	}
	else
		return ec_sprintf( str, "<ffi-ffih>" );
}

static EcUInt ffih_hash( EC_OBJ obj, EcInt recur )
{
	ASSERT( EC_FFIHP(obj) );

	/* Equivalent object MUST have the same hash */
	return ((EcUInt)EC_FFI_CIF(obj) + (EcUInt)EC_FFI_FCN(obj));	/* this is all we can do ... */
}

/* ------------------------------------------------------------------------
 * Initialization & cleanup
 * ------------------------------------------------------------------------ */

EC_OBJ ec_ffi_init( void )
{
	EcTypespec dlhspec = {
		/* type    */		0,
		/* name    */		"dlh",

		/* copy    */		dlh_copy,

		/* mark    */		dlh_mark,
		/* free    */		dlh_free,
		/* print   */		dlh_print,

		/* hash    */		dlh_hash,
		/* compare */		NULL,

		/* check   */		NULL,

		/* sequence */      NULL,
		/* numeric  */      NULL
	};
	EcTypespec ffihspec = {
		/* type    */		0,
		/* name    */		"ffih",

		/* copy    */		ffih_copy,

		/* mark    */		ffih_mark,
		/* free    */		ffih_free,
		/* print   */		ffih_print,

		/* hash    */		ffih_hash,
		/* compare */		NULL,

		/* check   */		NULL,

		/* sequence */      NULL,
		/* numeric  */      NULL
	};
	EC_OBJ pkg;

	tc_dlh = EcAddType( &dlhspec );
	if (tc_dlh == 0)
		return FALSE;
	tc_ffih = EcAddType( &ffihspec );
	if (tc_ffih == 0)
		return FALSE;

	/* Register package */
	pkg = EcPackageIntroduce( "ffi" );

	/* Intern symbols */
	t_void_id       = EcInternSymbol( "void" );
	t_uint8_id      = EcInternSymbol( "uint8" );
	t_sint8_id      = EcInternSymbol( "sint8" );
	t_uint16_id     = EcInternSymbol( "uint16" );
	t_sint16_id     = EcInternSymbol( "sint16" );
	t_uint32_id     = EcInternSymbol( "uint32" );
	t_sint32_id     = EcInternSymbol( "sint32" );
	t_uint64_id     = EcInternSymbol( "uint64" );
	t_sint64_id     = EcInternSymbol( "sint64" );
	t_float_id      = EcInternSymbol( "float" );
	t_double_id     = EcInternSymbol( "double" );
	t_longdouble_id = EcInternSymbol( "longdouble" );
	t_pointer_id    = EcInternSymbol( "pointer" );
	t_ushort_id     = EcInternSymbol( "ushort" );
	t_sint_id       = EcInternSymbol( "sint" );
	t_uint_id       = EcInternSymbol( "uint" );
	t_slong_id      = EcInternSymbol( "slong" );
	t_ulong_id      = EcInternSymbol( "ulong" );

	t_int8_id       = EcInternSymbol( "int8" );
	t_int16_id      = EcInternSymbol( "int16" );
	t_int32_id      = EcInternSymbol( "int32" );
	t_int64_id      = EcInternSymbol( "int64" );
	t_int_id        = EcInternSymbol( "int" );
	t_short_id      = EcInternSymbol( "short" );
	t_long_id       = EcInternSymbol( "long" );
	t_string_id     = EcInternSymbol( "string" );
	t_string_dyn_id = EcInternSymbol( "string-dynamic" );

	/* Register functions in package */
	EcAddPrimitive( "ffi.open",    EcLibFFI_Open );
	EcAddPrimitive( "ffi.close",   EcLibFFI_Close );
	EcAddPrimitive( "ffi.declare", EcLibFFI_Declare );

	return pkg;
}

void ec_ffi_cleanup( void )
{
}
