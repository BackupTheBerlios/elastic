/* ==========================================================================
 * bytecode.c
 * ==========================================================================
 * Bytecode interpretation.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat May 23 18:06:28 MEST 1998
 *
 *   $Id: bytecode.c,v 1.2 2002/05/25 19:37:44 panta Exp $
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

#include "private.h"
#include "compat.h"
#include "bytecode.h"
#include "debug.h"

static struct
{
	EcBytecode  bc;
	const char *name;
	int         params;
	EcInt       stackgrow;										/* SP displacement after execution (worst case: major increment) */
} bcInfo[] =
  {	{ PushHereOP,		"PushHereOP",		1,  1 },
	{ PopHereOP,		"PopHereOP",		1, -1 },
	{ PushParamOP,		"PushParamOP",		2,  1 },
	{ PopParamOP,		"PopParamOP",		2, -1 },
	{ PushOP,			"PushOP",			2,  1 },
	{ PushLiteralOP,	"PushLiteralOP",	2,  1 },
	{ PushPackageOP,	"PushPackageOP",	2,  1 },
	{ PopOP,			"PopOP",			2, -1 },
	{ PopLiteralOP,		"PopLiteralOP",		2, -1 },
	{ PopPackageOP,		"PopPackageOP",		2, -1 },
	{ PushInstanceOP,	"PushInstanceOP",	1,  1 },
	{ PopInstanceOP,	"PopInstanceOP",	1, -1 },
	{ PushClassOP,		"PushClassOP",		1,  1 },
	{ PopClassOP,		"PopClassOP",		1, -1 },
	{ DupOP,			"DupOP",			0,  1 },
	{ DiscardOP,		"DiscardOP",		0, -1 },
	{ PushStackOP,		"PushStackOP",		0,  1 },
	{ GetOP,			"GetOP",			1,  1 },
	{ BranchIfTrueOP,	"BranchIfTrueOP",	1, -1 },
	{ BranchIfFalseOP,	"BranchIfFalseOP",	1, -1 },
	{ BranchIfDefinedOP,"BranchIfDefinedOP",1, -1 },
	{ JumpOP,			"JumpOP",			1,  0 },
	{ CallOP,			"CallOP",			1,  0 },			/* worst case (biggest value) */
	{ CallMethodOP,		"CallMethodOP",		2,  1 },			/* worst case (biggest value) */
	{ CallSuperMethodOP,"CallSuperMethodOP",2,  1 },			/* worst case (biggest value) */
	{ InlinedCallOP,	"InlinedCallOP",	1,  0 },			/* worst case (biggest value) */
	{ ReturnOP,			"ReturnOP",			0, -1 },
	{ InlinedReturnOP,	"InlinedReturnOP",	0, -1 },
	{ ThrowOP,			"ThrowOP",			0, -1 },
	{ SequenceGetOP,	"SequenceGetOP",	0, -1 },
	{ SequenceSetOP,	"SequenceSetOP",	0, -2 },
	{ AddOP,			"AddOP",			0, -1 },
	{ SubOP,			"SubOP",			0, -1 },
	{ MulOP,			"MulOP",			0, -1 },
	{ DivOP,			"DivOP",			0, -1 },
	{ ModOP,			"ModOP",			0, -1 },
	{ PowOP,			"PowOP",			0, -1 },
	{ LShiftOP,			"LShiftOP",			0, -1 },
	{ RShiftOP,			"RShiftOP",			0, -1 },
	{ AndOP,			"AndOP",			0, -1 },
	{ OrOP,				"OrOP",				0, -1 },
	{ XorOP,			"XorOP",			0, -1 },
	{ NegOP,			"NegOP",			0,  0 },
	{ PosOP,			"PosOP",			0,  0 },
	{ InvertOP,			"InvertOP",			0,  0 },
	{ InOP,				"InOP",				0, -1 },
	{ LtOP,				"LtOP",				0, -1 },
	{ GtOP,				"GtOP",				0, -1 },
	{ LeOP,				"LeOP",				0, -1 },
	{ GeOP,				"GeOP",				0, -1 },
	{ EqOP,				"EqOP",				0, -1 },
	{ NeOP,				"NeOP",				0, -1 },
	{ LNotOP,			"LNotOP",			0, -1 },
	{ SaveLexicalOP,	"SaveLexicalOP",	0,  0 }, /* was -1 */
	{ CopyOP,			"CopyOP",			1,  0 },
	{ ArrayConsOP,		"ArrayConsOP",		1,  1 },			/* worst case (biggest value) */
	{ HashConsOP,		"HashConsOP",		1,  1 },			/* worst case (biggest value) */
	{ IncOP,			"IncOP",			0,  0 },
	{ DecOP,			"DecOP",			0,  0 },
	{ -1,				NULL,				0,  0 } };

static int searchBCInfo( EcBytecode bc )
{
	int i;

	i = 0;
	while (bcInfo[i].bc != -1)
	{
		if (bcInfo[i].bc == bc) return i;
		i++;
	}
	return 0;
}

static int searchBCInfoName( const char *name )
{
	int i;

	i = 0;
	while (bcInfo[i].bc != -1)
	{
		if (strcasecmp( bcInfo[i].name, name ) == 0) return i;
		i++;
	}
	return -1;
}


/* ========================================================================
 * A P I
 * ======================================================================== */

EC_API const char *EcBytecodeName( EcBytecode bc )
{
	int i;

	i = searchBCInfo( bc );
	if (i >= 0)
		return bcInfo[i].name;
	else
		return NULL;
}

EC_API EcBytecode EcBytecodeFromName( const char *name )
{
	int i;

	i = searchBCInfoName( name );
	if (i >= 0)
		return bcInfo[i].bc;
	else
		return UnknownOP;
}

EC_API int EcBytecodeParams( EcBytecode bc )
{
	int i;

	i = searchBCInfo( bc );
	if (i >= 0)
		return bcInfo[i].params;
	else
		return -1;
}

EC_API EcInt EcBytecodeStackgrow( EcBytecode bc )
{
	int i;

	i = searchBCInfo( bc );
	if (i >= 0)
		return bcInfo[i].stackgrow;
	else
		return -1000;
}
