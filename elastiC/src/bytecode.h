/* ==========================================================================
 * bytecode.h
 * ==========================================================================
 * Bytecode interpretation.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat May 23 18:06:28 MEST 1998
 *
 *   $Id: bytecode.h,v 1.1 2002/05/23 21:16:42 panta Exp $
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

#ifndef __BYTECODE_H
#define __BYTECODE_H

#include <elastic/basic.h>

EC_BEGIN_DECLS

typedef EcDWord EcBytecode;

/*
 * NOTE: I don't like the way the bytecodes are defined (i.e. with #define's)
 *       but enums are too limited.
 */

/* ========================================================================
 * B Y T E C O D E S
 * ======================================================================== */

/* Stack & literal bytecodes - RANGE  0:17 */
#define PushHereOP			0
#define PopHereOP			1
#define PushParamOP			2
#define PopParamOP			3
#define PushOP				4
#define PushLiteralOP		5
#define PushPackageOP		6
#define PopOP				7
#define PopLiteralOP		8
#define PopPackageOP		9
#define PushInstanceOP		10
#define PopInstanceOP		11
#define PushClassOP			12
#define PopClassOP			13
#define DupOP				14
#define DiscardOP			15
#define PushStackOP			16
#define GetOP				17

/* Control bytecodes         - RANGE  18:28 */
#define BranchIfTrueOP		18
#define BranchIfFalseOP		19
#define BranchIfDefinedOP	20
#define JumpOP				21
#define CallOP				22
#define CallMethodOP		23
#define CallSuperMethodOP	24
#define InlinedCallOP		25
#define ReturnOP			26
#define InlinedReturnOP		27
#define ThrowOP				28

/* Referencing bytecodes     - RANGE  29:30 */
#define SequenceGetOP		29
#define SequenceSetOP		30

/* Arithmetic bytecodes      - RANGE  31:52 */
#define AddOP				31
#define SubOP				32
#define MulOP				33
#define DivOP				34
#define ModOP				35
#define PowOP				36
#define LShiftOP			37
#define RShiftOP			38
#define AndOP				39
#define OrOP				40
#define XorOP				41
#define NegOP				42
#define PosOP				43
#define InvertOP			44
#define InOP				45
#define LtOP				46
#define GtOP				47
#define LeOP				48
#define GeOP				49
#define EqOP				50
#define NeOP				51
#define LNotOP				52

/* Misc bytecodes            - RANGE 53:58 */
#define SaveLexicalOP		53
#define CopyOP				54
#define ArrayConsOP			55
#define HashConsOP			56
#define IncOP				57
#define DecOP				58

#define UnknownOP			-1

/* ========================================================================
 * A P I
 * ======================================================================== */

EC_API const char *EcBytecodeName( EcBytecode bc );
EC_API EcBytecode  EcBytecodeFromName( const char *name );
EC_API int         EcBytecodeParams( EcBytecode bc );
EC_API EcInt       EcBytecodeStackgrow( EcBytecode bc );

EC_END_DECLS

#endif /* __BYTECODE_H */
