/* ==========================================================================
 * compile2c
 * ==========================================================================
 * elastiC C compiler base file.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Jacopo Pantaleoni         E-mail: jp@lightflowtech.com
 *
 *   Created: Sun May 25 08:05:50 MEST 2003
 *
 *   $Id: compile2c.c,v 1.1 2003/05/29 10:36:00 panta Exp $
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

#if EC_COMPILE2C

#include "basic.h"
#include "debug.h"
#include "private.h"
#include "elastic.h"
#include "bytecode.h"

#include "compile2c.h"

#define PRINT_OP 0

#define iFETCH	(*codepc++)
#define iPC		(codepc - code)

struct ec_compiler2c_ctxt_struct
{
	EC_OBJ package;
	EcInt  level;
	EcBool literal;
};

#if PRINT_OP

#define EMIT_0(name) \
{\
	fprintf(file, "\tprintf(\"%s\\n\");\n", name);\
	fprintf(file, "\tfflush(stdout);\n");\
	fprintf(file, "\t%s\n", name);\
}

#define EMIT_1(name) \
{\
	op1 = (EcInt)iFETCH;\
	fprintf(file, "\tprintf(\"%s( %d )\\n\");\n", name, op1);\
	fprintf(file, "\tfflush(stdout);\n");\
	fprintf(file, "\t%s( %d )\n", name, op1);\
}

#define EMIT_2(name) \
{\
	op1 = (EcInt)iFETCH;\
	op2 = (EcInt)iFETCH;\
	fprintf(file, "\tprintf(\"%s( %d, %d )\\n\");\n", name, op1, op2);\
	fprintf(file, "\tfflush(stdout);\n");\
	fprintf(file, "\t%s( %d, %d )\n", name, op1, op2);\
}

#else

#define EMIT_0(name) \
{\
	fprintf(file, "\t%s\n", name);\
}

#define EMIT_1(name) \
{\
	op1 = (EcInt)iFETCH;\
	fprintf(file, "\t%s( %d )\n", name, op1);\
}

#define EMIT_2(name) \
{\
	op1 = (EcInt)iFETCH;\
	op2 = (EcInt)iFETCH;\
	fprintf(file, "\t%s( %d, %d )\n", name, op1, op2);\
}

#endif

EC_API ec_compiler2c_ctxt EcCompiler2CContextCreate( void )
{
	ec_compiler2c_ctxt ctxt;

	ctxt = ec_malloc( sizeof(struct ec_compiler2c_ctxt_struct) );
	if (! ctxt) return NULL;

	ctxt->package = NULL;
	ctxt->level = 0;
	ctxt->literal = 0;
	return ctxt;
}

EC_API void EcCompiler2CContextDestroy( ec_compiler2c_ctxt ctxt )
{
	ec_free( ctxt );
}

EC_API void EcMainCompile2C( ec_compiler2c_ctxt ctxt, EC_OBJ package, FILE* file )
{
	EC_OBJ pkgcode;
	const char* pkgname;

	ASSERT( EC_PACKAGEP(package) );

	pkgcode = EC_PACKAGECODE(package);
	pkgname = EC_STRDATA(EC_PACKAGENAME(package));

	ctxt->package = package;

	EcCompile2C( ctxt, pkgcode, pkgname, file );

	fprintf(file, "EC_OBJ ec_%s_init( void )\n", pkgname);
	fprintf(file, "{\n");
	fprintf(file, "\tEC_OBJ package;\n");
	fprintf(file, "\tEC_OBJ pkgcode;\n\n");
	fprintf(file, "\tpackage = EcPackageLoadByPath( \"%s.ecc\", FALSE, TRUE );\n\n", pkgname);
	fprintf(file, "\tpkgcode = EC_PACKAGECODE(package);\n\n");
	fprintf(file, "\tec2c_%s_register( pkgcode );\n", pkgname);
	{
		EC_OBJ      frame;
		EC_OBJ      obj;
		EcInt       i;
		EcInt       exported = 0;

		frame = EC_PACKAGEFRAME(ctxt->package);
		for (i = 0; i < EC_ARRAYLEN(frame); i++)
		{
			obj = EC_ARRAYGET( frame, i );
			if (! EC_COMPILEDP(obj))
				continue;

			exported++;
		}
		if (exported)
		{
			fprintf(file, "\t{\n");
			fprintf(file, "\t\tEC_OBJ frame = EC_PACKAGEFRAME(package);\n\n");
			for (i = 0; i < EC_ARRAYLEN(frame); i++)
			{
				obj = EC_ARRAYGET( frame, i );
				if (! EC_COMPILEDP(obj))
					continue;

				fprintf(file, "\t\tec2c_%s_exported_%d_register( EC_ARRAYGET(frame, %d) );\n",
						pkgname, i, i);
			}
			fprintf(file, "\t}\n");
		}
	}
	fprintf(file, "\tEcCall( EC_NIL, EC_NIL, EC_NIL, pkgcode, 0 );\n\n");
	fprintf(file, "\treturn package;\n" );
	fprintf(file, "//\tEC_OBJ pkg;\n\n");
	fprintf(file, "//\tpkg = EcPackageIntroduce( %s );\n\n", pkgname );
	fprintf(file, "//\tpkgcode = ec2c_%s_code();\n\n", pkgname);
	fprintf(file, "//\tec2c_%s_register( pkgcode );\n\n", pkgname);
	fprintf(file, "//\tEcCall( EC_NIL, EC_NIL, EC_NIL, pkgcode, 0 );\n\n");
	fprintf(file, "//\treturn pkg;\n" );
	fprintf(file, "}\n\n");
	fprintf(file, "void ec_%s_cleanup( void )\n", pkgname);
	fprintf(file, "{\n");
	fprintf(file, "}\n");
}

EC_API void EcCompile2C( ec_compiler2c_ctxt ctxt, EC_OBJ compiled, const char* moduleName, FILE* file )
{
	register EcBytecode instr;
	register EcInt      op1, op2;

	/* These are unnecessarily duplicated, but are kept here to speed up */
	EcBytecode *code, *codepc, *codeend;						/* Start and end of code   */
	EcInt       ncode;											/* Lenght of code          */
	EC_OBJ      lframe;
	EC_OBJ      obj;
	EcInt       i;
	char        moduleNameLit[1024];
	EcInt       literals = 0;

	ASSERT( EC_COMPILEDP(compiled) );

	if (ctxt->level == 0)
	{
		fprintf(file, "#include <elastic/compile2c_base.h>\n");
		fprintf(file, "#include <elastic/compile2c_macros.h>\n");
		fprintf(file, "\n");

		if (ctxt->package)
		{
			lframe = EC_PACKAGEFRAME(ctxt->package);
			for (i = 0; i < EC_ARRAYLEN(lframe); i++)
			{
				EC_OBJ obj = EC_ARRAYGET( lframe, i );
				if (! EC_COMPILEDP(obj))
					continue;

				sprintf(moduleNameLit, "%s_exported_%d", moduleName, i);

				ctxt->level++;

				EcCompile2C( ctxt, obj, moduleNameLit, file );

				ctxt->level--;
			}
		}
	}

	/* First-time initialization */
	code    = EC_COMPILEDCODE(compiled);
	ncode   = EC_COMPILEDNCODE(compiled);
	codepc  = code;
	codeend = code + ncode;

	lframe = EC_COMPILEDLFRAME(compiled);
	for (i = 0; i < EC_ARRAYLEN(lframe); i++)
	{
		obj = EC_ARRAYGET( lframe, i );
		if (! EC_COMPILEDP(obj))
			continue;

		literals++;

		sprintf(moduleNameLit, "%s_lit_%d", moduleName, i);

		ctxt->level++;
		ctxt->literal++;
		EcCompile2C( ctxt, obj, moduleNameLit, file );
		ctxt->literal--;
		ctxt->level--;
	}

	if (ctxt->level)
		fprintf(file, "static ");
	fprintf(file, "EC_OBJ ec2c_%s( EC_OBJ self, EC_OBJ at_class, EC_OBJ compiled, EC_OBJ stack )\n", moduleName);
	fprintf(file, "{\n");
	fprintf(file, "#include <elastic/compile2c_head.h>\n");
	fprintf(file, "\n");
	for (;;)
	{
		fprintf(file, "label_%d:\n", iPC);

		if (codepc >= codeend)
			break;

//		fprintf(file, "\tC_PRE_OP\n");

		/* Fetch */
		instr = iFETCH;

		/* Decode & Execute */
		switch (instr)
		{
			/* Stack bytecodes      */

		case PushHereOP:
			EMIT_1("C_PUSH_HERE_OP");
			break;

		case PopHereOP:
			EMIT_1("C_POP_HERE_OP");
			break;

		case PushParamOP:
			EMIT_2("C_PUSH_PARAM_OP");
			break;

		case PopParamOP:
			EMIT_2("C_POP_PARAM_OP");
			break;

		case PushOP:
			EMIT_2("C_PUSH_OP");
			break;

		case PushLiteralOP:
			EMIT_2("C_PUSH_LITERAL_OP");
			break;

		case PushPackageOP:
			EMIT_2("C_PUSH_PACKAGE_OP");
			break;

		case PopOP:
			EMIT_2("C_POP_OP");
			break;

		case PopLiteralOP:
			EMIT_2("C_POP_LITERAL_OP");
			break;

		case PopPackageOP:
			EMIT_2("C_POP_PACKAGE_OP");
			break;

		case PushInstanceOP:
			EMIT_1("C_PUSH_INSTANCE_OP");
			break;

		case PopInstanceOP:
			EMIT_1("C_POP_INSTANCE_OP");
			break;

		case PushClassOP:
			EMIT_1("C_PUSH_CLASS_OP");
			break;

		case PopClassOP:
			EMIT_1("C_POP_CLASS_OP");
			break;

		case DupOP:
			EMIT_0("C_DUP_OP");
			break;

		case DiscardOP:
			EMIT_0("C_DISCARD_OP");
			break;

		case PushStackOP:
			EMIT_0("C_PUSH_STACK_OP");
			break;

		case GetOP:
			EMIT_1("C_GET_OP");
			break;

			/* Control bytecodes    */

		case BranchIfTrueOP:
			EMIT_1("C_BRANCH_IF_TRUE_OP");
			break;

		case BranchIfFalseOP:
			EMIT_1("C_BRANCH_IF_FALSE_OP");
			break;

		case BranchIfDefinedOP:
			EMIT_1("C_BRANCH_IF_DEFINED_OP");
			break;

		case JumpOP:
			EMIT_1("C_JUMP_OP");
			break;

		case CallOP:
			EMIT_1("C_CALL_OP");
			break;

		case CallMethodOP:
			EMIT_2("C_CALL_METHOD_OP");
			break;

		case CallSuperMethodOP:
			EMIT_2("C_CALL_SUPER_METHOD_OP");
			break;

		case InlinedCallOP:
			EMIT_1("C_INLINED_CALL_OP");
			break;

		case ReturnOP:
			EMIT_0("C_RETURN_OP");
			break;

		case InlinedReturnOP:
			EMIT_0("C_INLINED_RETURN_OP");
			break;

		case ThrowOP:
			EMIT_0("C_THROW_OP");
			break;

			/* Referencing bytecodes */

		case SequenceGetOP:
			EMIT_0("C_SEQUENCE_GET_OP");
			break;

		case SequenceSetOP:
			EMIT_0("C_SEQUENCE_SET_OP");
			break;

			/* Arithmetic bytecodes */

		case AddOP:
			EMIT_0("C_ADD_OP");
			break;

		case SubOP:
			EMIT_0("C_SUB_OP");
			break;

		case MulOP:
			EMIT_0("C_MUL_OP");
			break;

		case DivOP:
			EMIT_0("C_DIV_OP");
			break;

		case ModOP:
			EMIT_0("C_MOD_OP");
			break;

		case PowOP:
			EMIT_0("C_POW_OP");
			break;

		case LShiftOP:
			EMIT_0("C_LSHIFT_OP");
			break;

		case RShiftOP:
			EMIT_0("C_RSHIFT_OP");
			break;

		case AndOP:
			EMIT_0("C_AND_OP");
			break;

		case OrOP:
			EMIT_0("C_OR_OP");
			break;

		case XorOP:
			EMIT_0("C_XOR_OP");
			break;

		case NegOP:
			EMIT_0("C_NEG_OP");
			break;

		case PosOP:
			EMIT_0("C_POS_OP");
			break;

		case InvertOP:
			EMIT_0("C_INVERT_OP");
			break;

		case InOP:
			EMIT_0("C_IN_OP");
			break;

		case LtOP:
			EMIT_0("C_LT_OP");
			break;

		case GtOP:
			EMIT_0("C_GT_OP");
			break;

		case LeOP:
			EMIT_0("C_LE_OP");
			break;

		case GeOP:
			EMIT_0("C_GE_OP");
			break;

		case EqOP:
			EMIT_0("C_EQ_OP");
			break;

		case NeOP:
			EMIT_0("C_NE_OP");
			break;

		case LNotOP:
			EMIT_0("C_LNOT_OP");
			break;

		case SaveLexicalOP:
			EMIT_0("C_SAVE_LEXICAL_OP");
			break;

		case CopyOP:
			EMIT_1("C_COPY_OP");
			break;

		case ArrayConsOP:
			EMIT_1("C_ARRAY_CONS_OP");
			break;

		case HashConsOP:
			EMIT_1("C_HASH_CONS_OP");
			break;

		case IncOP:
			EMIT_0("C_INC_OP");
			break;

		case DecOP:
			EMIT_0("C_DEC_OP");
			break;
		}

//		fprintf(file, "\tC_POST_OP\n");
	}
	fprintf(file, "\n");
	fprintf(file, "#include <elastic/compile2c_tail.h>\n");
	fprintf(file, "}\n");
	fprintf(file, "\n");
	if (ctxt->level)
		fprintf(file, "static ");
	fprintf(file, "void ec2c_%s_register( EC_OBJ compiled )\n", moduleName);
	fprintf(file, "{\n");
	fprintf(file, "\tEC_COMPILEDCCALLABLE(compiled) = ec2c_%s;\n",
			moduleName);
	if (literals)
	{
		fprintf(file, "\t{\n");
		fprintf(file, "\t\tEC_OBJ lframe = EC_COMPILEDLFRAME(compiled);\n\n");
		lframe = EC_COMPILEDLFRAME(compiled);
		for (i = 0; i < EC_ARRAYLEN(lframe); i++)
		{
			obj = EC_ARRAYGET( lframe, i );
			if (! EC_COMPILEDP(obj))
				continue;

			fprintf(file, "\t\tec2c_%s_lit_%d_register( EC_ARRAYGET(lframe, %d) );\n",
					moduleName, i, i);
		}
		fprintf(file, "\t}\n");
	}
	fprintf(file, "}\n\n");
/*	fprintf(file, "\n");
	fprintf(file, "EC_OBJ ec2c_%s_exec( EC_OBJ compiled )\n", moduleName);
	fprintf(file, "{\n");
	fprintf(file, "#include <elastic/compile2c_exec.h>\n");
	fprintf(file, "\treturn ec2c_%s( EC_NIL, EC_NIL, compiled, stack )\n", moduleName);
	fprintf(file, "}\n");*/
}

#endif /* EC_COMPILE2C */
