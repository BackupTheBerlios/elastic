/* ==========================================================================
 * lib.h
 * ==========================================================================
 * Library functions.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sun Jul 26 17:04:46 MEST 1998
 *
 *   $Id: lib.h,v 1.4 2002/06/04 20:08:27 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1998-2002 Marco Pantaleoni. All rights reserved.
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

#ifndef __LIB_H
#define __LIB_H

#include <elastic/elastic.h>

EC_BEGIN_DECLS

EC_API EC_OBJ EcLibStr( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibRepr( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibInt( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibFloat( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibPrint( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibPrintNL( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibSPrintf( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibPrintf( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibCopy( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibShallowCopy( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibDeepCopy( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibExit( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibArray( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibTypeOf( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibClassCode( EC_OBJ stack, EcAny userdata );

EC_API EC_OBJ EcLibSend( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibSendWith( EC_OBJ stack, EcAny userdata );

#if 0
EC_API EC_OBJ EcLibCall( EC_OBJ stack, EcAny userdata );
#endif
EC_API EC_OBJ EcLibCompile( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibApply( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibArity( EC_OBJ stack, EcAny userdata );

EC_API EC_OBJ EcLibLength( EC_OBJ stack, EcAny userdata );

EC_API EC_OBJ EcLibHash( EC_OBJ stack, EcAny userdata );

EC_API EC_OBJ EcLibTime( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibTimes( EC_OBJ stack, EcAny userdata );

EC_API EC_OBJ EcLibGetEnv( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibSetEnv( EC_OBJ stack, EcAny userdata );

EC_API EC_OBJ EcLibMath_Sin( EC_OBJ stack, EcAny userdata );
EC_API EC_OBJ EcLibMath_Cos( EC_OBJ stack, EcAny userdata );

EC_API EC_OBJ EcLibObjectClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibObjectClass_Name( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibObjectClass_FullName( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibObject_IsA( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibObject_IsAP( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibObject_DoesUnderstand( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );

EC_API EC_OBJ EcLibExceptionClass_NewWith( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibException_GetSource( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibException_GetIn( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibException_GetLine( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibException_SetMessage( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibException_GetMessage( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibException_GetFullMessage( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );

EC_API EC_OBJ EcLibUnimplementedErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );

EC_API EC_OBJ EcLibMathErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );

EC_API EC_OBJ EcLibIOErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibIOError_GetIOObject( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibIOError_GetPosixError( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );

EC_API EC_OBJ EcLibMemoryErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );

EC_API EC_OBJ EcLibUnknownMethodErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibUnknownMethodError_GetMethod( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibUnknownMethodError_GetTarget( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );

EC_API EC_OBJ EcLibUnknownOperationErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibUnknownOperationError_GetOperands( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibUnknownOperationError_GetOperation( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );

EC_API EC_OBJ EcLibIndexErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibIndexError_GetIndex( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibIndexError_GetTarget( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );

EC_API EC_OBJ EcLibTypeErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibTypeError_GetTarget( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibTypeError_GetFunction( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibTypeError_GetIndex( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibTypeError_GetExpected( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibTypeError_GetOffensive( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );

EC_API EC_OBJ EcLibTargetErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibTargetError_GetTarget( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );

EC_API EC_OBJ EcLibParameterCountErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibParameterCountError_GetTarget( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibParameterCountError_GetFunction( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibParameterCountError_GetExpected( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibParameterCountError_GetArguments( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );

EC_API EC_OBJ EcLibCompileErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibCompileError_GetLine( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
EC_API EC_OBJ EcLibCompileError_GetColumn( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );

EC_API EC_OBJ EcLibLt( EC_OBJ obj1, EC_OBJ obj2 );
EC_API EC_OBJ EcLibGt( EC_OBJ obj1, EC_OBJ obj2 );
EC_API EC_OBJ EcLibLe( EC_OBJ obj1, EC_OBJ obj2 );
EC_API EC_OBJ EcLibGe( EC_OBJ obj1, EC_OBJ obj2 );
EC_API EC_OBJ EcLibEq( EC_OBJ obj1, EC_OBJ obj2 );
EC_API EC_OBJ EcLibNe( EC_OBJ obj1, EC_OBJ obj2 );

EC_API EC_OBJ EcLibInc( EC_OBJ obj );
EC_API EC_OBJ EcLibDec( EC_OBJ obj );

EC_API EC_OBJ EcLibArrayCons( EC_OBJ stack, EcAny userdata );

EC_API EC_OBJ EcLibHashCons( EC_OBJ stack, EcAny userdata );

EC_END_DECLS

#endif /* __LIB_H */
