/* ==========================================================================
 * builtin.c
 * ==========================================================================
 * Builtin primitives, classes, objects, ...
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sun Jan  3 12:13:42 MET 1999
 *
 *   $Id: builtin.c,v 1.7 2002/06/11 19:32:07 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1998-2001 Marco Pantaleoni. All rights reserved.
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

/* ========================================================================
 * C L A S S E S
 * ======================================================================== */

/* ------------------------------------------------------------------------
 * Object
 * ------------------------------------------------------------------------ */

static EcClassDef EcObjectClassDef =
{
	"Object",													/* class name         */
	"basic",													/* package name       */

	EC_NIL,														/* super class        */

	NULL,														/* class methods      */
	NULL,														/* class variables    */

	NULL,														/* instance methods   */
	NULL														/* instance variables */
};

static EcCMethodDef EcObjectClass_ClassMethods[] =				/* Class Methods      */
{
	{ "new",			EcLibObjectClass_New            },
	{ "name",			EcLibObjectClass_Name           },
	{ "fullname",		EcLibObjectClass_FullName       },
	{ NULL,				NULL                            }
};

static EcCMethodDef EcObjectClass_Methods[] =					/* Instance Methods   */
{
	{ "isA",				EcLibObject_IsA            },
	{ "isA:",				EcLibObject_IsAP           },
	{ "doesUnderstand:",	EcLibObject_DoesUnderstand },
	{ NULL,					NULL                       }
};

/* ------------------------------------------------------------------------
 * Exception
 * ------------------------------------------------------------------------ */

static EcClassDef EcExceptionClassDef =
{
	"Exception",												/* class name         */
	"basic",													/* package name       */

	EC_NIL,														/* super class        */

	NULL,														/* class methods      */
	NULL,														/* class variables    */

	NULL,														/* instance methods   */
	NULL														/* instance variables */
};

static EcCMethodDef EcExceptionClass_ClassMethods[] =			/* Class Methods      */
{
	{ "new:",			EcLibExceptionClass_NewWith },
	{ NULL,				NULL                        }
};

static EcCMethodDef EcExceptionClass_Methods[] =				/* Instance Methods   */
{
	{ "getSource",		EcLibException_GetSource      },
	{ "getIn",			EcLibException_GetIn          },
	{ "getLine",		EcLibException_GetLine        },
	{ "setMessage",		EcLibException_SetMessage     },
	{ "getMessage",		EcLibException_GetMessage     },
	{ "getFullMessage",	EcLibException_GetFullMessage },
	{ NULL,				NULL                          }
};

static EcVariableDef EcExceptionClass_Variables[] =				/* Instance Variables */
{
	{ "source" },
	{ "in"     },
	{ "line"   },
	{ "msg"    },
	{ NULL     }
};

/* ------------------------------------------------------------------------
 * UnimplementedError
 * ------------------------------------------------------------------------ */

static EcClassDef EcUnimplementedErrorClassDef =
{
	"UnimplementedError",										/* class name         */
	"basic",													/* package name       */

	EC_NIL,														/* super class        */

	NULL,														/* class methods      */
	NULL,														/* class variables    */

	NULL,														/* instance methods   */
	NULL														/* instance variables */
};

static EcCMethodDef EcUnimplementedErrorClass_ClassMethods[] =	/* Class Methods      */
{
	{ "new",			EcLibUnimplementedErrorClass_New },
	{ NULL,				NULL                      }
};

static EcCMethodDef EcUnimplementedErrorClass_Methods[] =		/* Instance Methods   */
{
	{ NULL,				NULL                      }
};

static EcVariableDef EcUnimplementedErrorClass_Variables[] =	/* Instance Variables */
{
	{ NULL                                        }
};

/* ------------------------------------------------------------------------
 * MathError
 * ------------------------------------------------------------------------ */

static EcClassDef EcMathErrorClassDef =
{
	"MathError",												/* class name         */
	"basic",													/* package name       */

	EC_NIL,														/* super class        */

	NULL,														/* class methods      */
	NULL,														/* class variables    */

	NULL,														/* instance methods   */
	NULL														/* instance variables */
};

static EcCMethodDef EcMathErrorClass_ClassMethods[] =			/* Class Methods      */
{
	{ "new",			EcLibMathErrorClass_New   },
	{ NULL,				NULL                      }
};

static EcCMethodDef EcMathErrorClass_Methods[] =				/* Instance Methods   */
{
	{ NULL,				NULL                      }
};

static EcVariableDef EcMathErrorClass_Variables[] =				/* Instance Variables */
{
	{ NULL                                        }
};

/* ------------------------------------------------------------------------
 * IOError
 * ------------------------------------------------------------------------ */

static EcClassDef EcIOErrorClassDef =
{
	"IOError",													/* class name         */
	"basic",													/* package name       */

	EC_NIL,														/* super class        */

	NULL,														/* class methods      */
	NULL,														/* class variables    */

	NULL,														/* instance methods   */
	NULL														/* instance variables */
};

static EcCMethodDef EcIOErrorClass_ClassMethods[] =				/* Class Methods      */
{
	{ "new",			EcLibIOErrorClass_New   },
	{ NULL,				NULL                    }
};

static EcCMethodDef EcIOErrorClass_Methods[] =					/* Instance Methods   */
{
	{ "getIOObject",	EcLibIOError_GetIOObject   },
	{ "getPosixError",	EcLibIOError_GetPosixError },
	{ NULL,				NULL                       }
};

static EcVariableDef EcIOErrorClass_Variables[] =				/* Instance Variables */
{
	{ "ioObject"   },											/* I/O object (file, ...) that caused or was interested by the error */
	{ "posixError" },											/* symbol of the posix error (#EEXIST, #ENOMEM, ...)                 */
	{ NULL         }
};

/* ------------------------------------------------------------------------
 * MemoryError
 * ------------------------------------------------------------------------ */

static EcClassDef EcMemoryErrorClassDef =
{
	"MemoryError",												/* class name         */
	"basic",													/* package name       */

	EC_NIL,														/* super class        */

	NULL,														/* class methods      */
	NULL,														/* class variables    */

	NULL,														/* instance methods   */
	NULL														/* instance variables */
};

static EcCMethodDef EcMemoryErrorClass_ClassMethods[] =			/* Class Methods      */
{
	{ "new",			EcLibMemoryErrorClass_New },
	{ NULL,				NULL                      }
};

static EcCMethodDef EcMemoryErrorClass_Methods[] =				/* Instance Methods   */
{
	{ NULL,				NULL                      }
};

static EcVariableDef EcMemoryErrorClass_Variables[] =			/* Instance Variables */
{
	{ NULL                                        }
};

/* ------------------------------------------------------------------------
 * UnknownMethodError
 * ------------------------------------------------------------------------ */

static EcClassDef EcUnknownMethodErrorClassDef =
{
	"UnknownMethodError",										/* class name         */
	"basic",													/* package name       */

	EC_NIL,														/* super class        */

	NULL,														/* class methods      */
	NULL,														/* class variables    */

	NULL,														/* instance methods   */
	NULL														/* instance variables */
};

static EcCMethodDef EcUnknownMethodErrorClass_ClassMethods[] =	/* Class Methods      */
{
	{ "new",			EcLibUnknownMethodErrorClass_New },
	{ NULL,				NULL                             }
};

static EcCMethodDef EcUnknownMethodErrorClass_Methods[] =		/* Instance Methods   */
{
	{ "getMethod",		EcLibUnknownMethodError_GetMethod },
	{ "getTarget",		EcLibUnknownMethodError_GetTarget },
	{ NULL,				NULL                              }
};

static EcVariableDef EcUnknownMethodErrorClass_Variables[] =	/* Instance Variables */
{
	{ "methodName" },
	{ "target"     },
	{ NULL         }
};

/* ------------------------------------------------------------------------
 * UnknownOperationError
 * ------------------------------------------------------------------------ */

static EcClassDef EcUnknownOperationErrorClassDef =
{
	"UnknownOperationError",									/* class name         */
	"basic",													/* package name       */

	EC_NIL,														/* super class        */

	NULL,														/* class methods      */
	NULL,														/* class variables    */

	NULL,														/* instance methods   */
	NULL														/* instance variables */
};

static EcCMethodDef EcUnknownOperationErrorClass_ClassMethods[] =	/* Class Methods      */
{
	{ "new",			EcLibUnknownOperationErrorClass_New },
	{ NULL,				NULL                                }
};

static EcCMethodDef EcUnknownOperationErrorClass_Methods[] =		/* Instance Methods   */
{
	{ "getOperands",	EcLibUnknownOperationError_GetOperands  },
	{ "getOperation",	EcLibUnknownOperationError_GetOperation },
	{ NULL,				NULL                                    }
};

static EcVariableDef EcUnknownOperationErrorClass_Variables[] =	/* Instance Variables */
{
	{ "operands"  },
	{ "operation" },
	{ NULL        }
};

/* ------------------------------------------------------------------------
 * IndexError
 * ------------------------------------------------------------------------ */

static EcClassDef EcIndexErrorClassDef =
{
	"IndexError",												/* class name         */
	"basic",													/* package name       */

	EC_NIL,														/* super class        */

	NULL,														/* class methods      */
	NULL,														/* class variables    */

	NULL,														/* instance methods   */
	NULL														/* instance variables */
};

static EcCMethodDef EcIndexErrorClass_ClassMethods[] =			/* Class Methods      */
{
	{ "new",			EcLibIndexErrorClass_New },
	{ NULL,				NULL                     }
};

static EcCMethodDef EcIndexErrorClass_Methods[] =				/* Instance Methods   */
{
	{ "getIndex",		EcLibIndexError_GetIndex  },
	{ "getTarget",		EcLibIndexError_GetTarget },
	{ NULL,				NULL                      }
};

static EcVariableDef EcIndexErrorClass_Variables[] =			/* Instance Variables */
{
	{ "index"  },
	{ "target" },
	{ NULL     }
};

/* ------------------------------------------------------------------------
 * TypeError
 * ------------------------------------------------------------------------ */

static EcClassDef EcTypeErrorClassDef =
{
	"TypeError",												/* class name         */
	"basic",													/* package name       */

	EC_NIL,														/* super class        */

	NULL,														/* class methods      */
	NULL,														/* class variables    */

	NULL,														/* instance methods   */
	NULL														/* instance variables */
};

static EcCMethodDef EcTypeErrorClass_ClassMethods[] =			/* Class Methods      */
{
	{ "new",			EcLibTypeErrorClass_New },
	{ NULL,				NULL                    }
};

static EcCMethodDef EcTypeErrorClass_Methods[] =				/* Instance Methods   */
{
	{ "getTarget",		EcLibTypeError_GetTarget    },
	{ "getFunction",	EcLibTypeError_GetFunction  },
	{ "getIndex",		EcLibTypeError_GetIndex     },
	{ "getExpected",	EcLibTypeError_GetExpected  },
	{ "getOffensive",	EcLibTypeError_GetOffensive },
	{ NULL,				NULL                        }
};

static EcVariableDef EcTypeErrorClass_Variables[] =				/* Instance Variables */
{
	{ "target"    },
	{ "function"  },
	{ "index"     },
	{ "expected"  },
	{ "offensive" },
	{ NULL        }
};

/* ------------------------------------------------------------------------
 * TargetError
 * ------------------------------------------------------------------------ */

static EcClassDef EcTargetErrorClassDef =
{
	"TargetError",												/* class name         */
	"basic",													/* package name       */

	EC_NIL,														/* super class        */

	NULL,														/* class methods      */
	NULL,														/* class variables    */

	NULL,														/* instance methods   */
	NULL														/* instance variables */
};

static EcCMethodDef EcTargetErrorClass_ClassMethods[] =			/* Class Methods      */
{
	{ "new",			EcLibTargetErrorClass_New },
	{ NULL,				NULL                      }
};

static EcCMethodDef EcTargetErrorClass_Methods[] =				/* Instance Methods   */
{
	{ "getTarget",		EcLibTargetError_GetTarget },
	{ NULL,				NULL                       }
};

static EcVariableDef EcTargetErrorClass_Variables[] =			/* Instance Variables */
{
	{ "target"    },
	{ NULL        }
};

/* ------------------------------------------------------------------------
 * ParameterCountError
 * ------------------------------------------------------------------------ */

static EcClassDef EcParameterCountErrorClassDef =
{
	"ParameterCountError",										/* class name         */
	"basic",													/* package name       */

	EC_NIL,														/* super class        */

	NULL,														/* class methods      */
	NULL,														/* class variables    */

	NULL,														/* instance methods   */
	NULL														/* instance variables */
};

static EcCMethodDef EcParameterCountErrorClass_ClassMethods[] =	/* Class Methods      */
{
	{ "new",			EcLibParameterCountErrorClass_New },
	{ NULL,				NULL                              }
};

static EcCMethodDef EcParameterCountErrorClass_Methods[] =		/* Instance Methods   */
{
	{ "getTarget",		EcLibParameterCountError_GetTarget    },
	{ "getFunction",	EcLibParameterCountError_GetFunction  },
	{ "getExpected",	EcLibParameterCountError_GetExpected  },
	{ "getArguments",	EcLibParameterCountError_GetArguments },
	{ NULL,				NULL                                  }
};

static EcVariableDef EcParameterCountErrorClass_Variables[] =	/* Instance Variables */
{
	{ "target"     },
	{ "function"   },
	{ "expected"   },
	{ "arguments"  },
	{ NULL         }
};

/* ------------------------------------------------------------------------
 * CompileError
 * ------------------------------------------------------------------------ */

static EcClassDef EcCompileErrorClassDef =
{
	"CompileError",												/* class name         */
	"basic",													/* package name       */

	EC_NIL,														/* super class        */

	NULL,														/* class methods      */
	NULL,														/* class variables    */

	NULL,														/* instance methods   */
	NULL														/* instance variables */
};

static EcCMethodDef EcCompileErrorClass_ClassMethods[] =		/* Class Methods      */
{
	{ "new",			EcLibCompileErrorClass_New },
	{ NULL,				NULL                       }
};

static EcCMethodDef EcCompileErrorClass_Methods[] =				/* Instance Methods   */
{
	{ "getLine",		EcLibCompileError_GetLine   },
	{ "getColumn",		EcLibCompileError_GetColumn },
	{ NULL,				NULL                        }
};

static EcVariableDef EcCompileErrorClass_Variables[] =			/* Instance Variables */
{
	{ "line"   },
	{ "column" },
	{ NULL     }
};


/* ------------------------------------------------------------------------
 * R E G I S T E R I N G   F U N C T I O N
 * ------------------------------------------------------------------------ */

EcBool _ec_register_builtin( void )
{
	const ec_streamdef *sdef;
	ec_stream          *str;
	EC_OBJ              exc;

	if (! (_ec_array_init()    &&
		   _ec_char_init()     &&
		   _ec_string_init()   &&
		   _ec_stack_init()    &&
		   _ec_compiled_init() &&
		   _ec_handler_init()  &&
		   _ec_package_init()  &&
		   _ec_class_init()    &&
		   _ec_object_init()   &&
		   _ec_hash_init()     &&
		   _ec_list_init()
		))
		return FALSE;

	/* Introduce packages */
	EcPackageIntroduce( "basic" );
	EcPackageIntroduce( "sys" );
	EcPackageIntroduce( "math" );

#if ECMODULE_FILESTREAM_STATIC
	if (EC_ERRORP(_ec_filestream_init()))
		return FALSE;
#endif

	/* ... and now create stdio streams */
	sdef = ec_filestream_def();

	str = ec_stream_create( sdef, &exc,
							stdin,
							/* don't close */ TRUE,
							/* popen()-ed  */ FALSE );
	PRIVATE(stream_stdin)  = str;

	str = ec_stream_create( sdef, &exc,
							stdout,
							/* don't close */ TRUE,
							/* popen()-ed  */ FALSE );
	PRIVATE(stream_stdout) = str;

	str = ec_stream_create( sdef, &exc,
							stderr,
							/* don't close */ TRUE,
							/* popen()-ed  */ FALSE );
	PRIVATE(stream_stderr) = str;

	if (! _ec_lib_init())
		return FALSE;

	if (! _ec_file_init())
		return FALSE;

	if (! _ec_modstring_init())
		return FALSE;

	if (! _ec_modarray_init())
		return FALSE;

#if ECMODULE_POSIX_STATIC
	if (EC_ERRORP(_ec_modposix_init()))
		return FALSE;											/* :TODO: do something with exception */
#endif

#if ECMODULE_ERRNO_STATIC
	if (EC_ERRORP(_ec_moderrno_init()))
		return FALSE;											/* :TODO: do something with exception */
#endif

	/* Object class */

	EcObjectClassDef.superclass   = EC_NIL;
	EcObjectClassDef.classmethods = EcObjectClass_ClassMethods;
	EcObjectClassDef.methods      = EcObjectClass_Methods;
	EcObjectClass = EcAddClassDef( &EcObjectClassDef );
	EcRegisterBuiltin( "basic.Object", EcObjectClass );

	/* Exception class */

	EcExceptionClassDef.superclass   = EcObjectClass;
	EcExceptionClassDef.classmethods = EcExceptionClass_ClassMethods;
	EcExceptionClassDef.methods      = EcExceptionClass_Methods;
	EcExceptionClassDef.variables    = EcExceptionClass_Variables;
	EcExceptionClass = EcAddClassDef( &EcExceptionClassDef );
	EcRegisterBuiltin( "basic.Exception", EcExceptionClass );

	/* UnimplementedError class */

	EcUnimplementedErrorClassDef.superclass   = EcExceptionClass;
	EcUnimplementedErrorClassDef.classmethods = EcUnimplementedErrorClass_ClassMethods;
	EcUnimplementedErrorClassDef.methods      = EcUnimplementedErrorClass_Methods;
	EcUnimplementedErrorClassDef.variables    = EcUnimplementedErrorClass_Variables;
	EcUnimplementedErrorClass = EcAddClassDef( &EcUnimplementedErrorClassDef );
	EcRegisterBuiltin( "basic.UnimplementedError", EcUnimplementedErrorClass );

	/* MathError class */

	EcMathErrorClassDef.superclass   = EcExceptionClass;
	EcMathErrorClassDef.classmethods = EcMathErrorClass_ClassMethods;
	EcMathErrorClassDef.methods      = EcMathErrorClass_Methods;
	EcMathErrorClassDef.variables    = EcMathErrorClass_Variables;
	EcMathErrorClass = EcAddClassDef( &EcMathErrorClassDef );
	EcRegisterBuiltin( "basic.MathError", EcMathErrorClass );

	/* IOError class */

	EcIOErrorClassDef.superclass   = EcExceptionClass;
	EcIOErrorClassDef.classmethods = EcIOErrorClass_ClassMethods;
	EcIOErrorClassDef.methods      = EcIOErrorClass_Methods;
	EcIOErrorClassDef.variables    = EcIOErrorClass_Variables;
	EcIOErrorClass = EcAddClassDef( &EcIOErrorClassDef );
	EcRegisterBuiltin( "basic.IOError", EcIOErrorClass );

	/* MemoryError class */

	EcMemoryErrorClassDef.superclass   = EcExceptionClass;
	EcMemoryErrorClassDef.classmethods = EcMemoryErrorClass_ClassMethods;
	EcMemoryErrorClassDef.methods      = EcMemoryErrorClass_Methods;
	EcMemoryErrorClassDef.variables    = EcMemoryErrorClass_Variables;
	EcMemoryErrorClass = EcAddClassDef( &EcMemoryErrorClassDef );
	EcRegisterBuiltin( "basic.MemoryError", EcMemoryErrorClass );

	/* UnknownMethodError class */

	EcUnknownMethodErrorClassDef.superclass   = EcExceptionClass;
	EcUnknownMethodErrorClassDef.classmethods = EcUnknownMethodErrorClass_ClassMethods;
	EcUnknownMethodErrorClassDef.methods      = EcUnknownMethodErrorClass_Methods;
	EcUnknownMethodErrorClassDef.variables    = EcUnknownMethodErrorClass_Variables;
	EcUnknownMethodErrorClass = EcAddClassDef( &EcUnknownMethodErrorClassDef );
	EcRegisterBuiltin( "basic.UnknownMethodError", EcUnknownMethodErrorClass );

	/* UnknownOperationError class */

	EcUnknownOperationErrorClassDef.superclass   = EcExceptionClass;
	EcUnknownOperationErrorClassDef.classmethods = EcUnknownOperationErrorClass_ClassMethods;
	EcUnknownOperationErrorClassDef.methods      = EcUnknownOperationErrorClass_Methods;
	EcUnknownOperationErrorClassDef.variables    = EcUnknownOperationErrorClass_Variables;
	EcUnknownOperationErrorClass = EcAddClassDef( &EcUnknownOperationErrorClassDef );
	EcRegisterBuiltin( "basic.UnknownOperationError", EcUnknownOperationErrorClass );

	/* IndexError class */

	EcIndexErrorClassDef.superclass   = EcExceptionClass;
	EcIndexErrorClassDef.classmethods = EcIndexErrorClass_ClassMethods;
	EcIndexErrorClassDef.methods      = EcIndexErrorClass_Methods;
	EcIndexErrorClassDef.variables    = EcIndexErrorClass_Variables;
	EcIndexErrorClass = EcAddClassDef( &EcIndexErrorClassDef );
	EcRegisterBuiltin( "basic.IndexError", EcIndexErrorClass );

	/* TypeError class */

	EcTypeErrorClassDef.superclass   = EcExceptionClass;
	EcTypeErrorClassDef.classmethods = EcTypeErrorClass_ClassMethods;
	EcTypeErrorClassDef.methods      = EcTypeErrorClass_Methods;
	EcTypeErrorClassDef.variables    = EcTypeErrorClass_Variables;
	EcTypeErrorClass = EcAddClassDef( &EcTypeErrorClassDef );
	EcRegisterBuiltin( "basic.TypeError", EcTypeErrorClass );

	/* TargetError class */

	EcTargetErrorClassDef.superclass   = EcExceptionClass;
	EcTargetErrorClassDef.classmethods = EcTargetErrorClass_ClassMethods;
	EcTargetErrorClassDef.methods      = EcTargetErrorClass_Methods;
	EcTargetErrorClassDef.variables    = EcTargetErrorClass_Variables;
	EcTargetErrorClass = EcAddClassDef( &EcTargetErrorClassDef );
	EcRegisterBuiltin( "basic.TargetError", EcTargetErrorClass );

	/* ParameterCountError class */

	EcParameterCountErrorClassDef.superclass   = EcExceptionClass;
	EcParameterCountErrorClassDef.classmethods = EcParameterCountErrorClass_ClassMethods;
	EcParameterCountErrorClassDef.methods      = EcParameterCountErrorClass_Methods;
	EcParameterCountErrorClassDef.variables    = EcParameterCountErrorClass_Variables;
	EcParameterCountErrorClass = EcAddClassDef( &EcParameterCountErrorClassDef );
	EcRegisterBuiltin( "basic.ParameterCountError", EcParameterCountErrorClass );

	/* CompileError class */

	EcCompileErrorClassDef.superclass   = EcExceptionClass;
	EcCompileErrorClassDef.classmethods = EcCompileErrorClass_ClassMethods;
	EcCompileErrorClassDef.methods      = EcCompileErrorClass_Methods;
	EcCompileErrorClassDef.variables    = EcCompileErrorClass_Variables;
	EcCompileErrorClass = EcAddClassDef( &EcCompileErrorClassDef );
	EcRegisterBuiltin( "basic.CompileError", EcCompileErrorClass );

	/*
	 * Save global forever-living objects from GC
	 *
	 * Note that packages need not be GC protected.
	 */
	EcGCProtect( EcObjectClass );
	EcGCProtect( EcExceptionClass );
	EcGCProtect( EcUnimplementedErrorClass );
	EcGCProtect( EcMathErrorClass );
	EcGCProtect( EcIOErrorClass );
	EcGCProtect( EcMemoryErrorClass );
	EcGCProtect( EcUnknownMethodErrorClass );
	EcGCProtect( EcUnknownOperationErrorClass );
	EcGCProtect( EcIndexErrorClass );
	EcGCProtect( EcTypeErrorClass );
	EcGCProtect( EcTargetErrorClass );
	EcGCProtect( EcParameterCountErrorClass );
	EcGCProtect( EcCompileErrorClass );

#if 0
	/* This has moved to `elastic.c', after other globals have been added */
	PRIVATE(ncoreglobals) = PRIVATE(nglobals);
#endif

	/* INITIALIZE OTHER MODULES HERE (after having registered relevant classes) ! */

	return TRUE;
}

void _ec_cleanup_builtin( void )
{
#if ECMODULE_ERRNO_STATIC
	_ec_moderrno_cleanup();
#endif
#if ECMODULE_POSIX_STATIC
	_ec_modposix_cleanup();
#endif
	_ec_modarray_cleanup();
	_ec_modstring_cleanup();
	_ec_file_cleanup();
	_ec_lib_cleanup();
	_ec_filestream_cleanup();
}
