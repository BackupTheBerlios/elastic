/* ==========================================================================
 * compile2c_head
 * ==========================================================================
 * elastiC C compiler head file.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Jacopo Pantaleoni         E-mail: jp@lightflowtech.com
 *
 *   Created: Sun May 25 08:05:50 MEST 2003
 *
 *   $Id: compile2c_head.h,v 1.1 2003/05/29 10:36:00 panta Exp $
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

	register EcBytecode instr;

	/* These are unnecessarily duplicated, but are kept here to speed up */
	EcBytecode *code, *codepc, *codeend;						/* Start and end of code   */
	EcInt       ncode;											/* Lenght of code          */
	EcPrivate  *lprivate;

	ASSERT( EC_COMPILEDP(compiled) );
	ASSERT( EC_STACKP(stack) );

	lprivate = &_ec_private;

	/* First-time initialization */
	/* Useful for tracing only... could be removed. #JP */
	code    = EC_COMPILEDCODE(compiled);
	ncode   = EC_COMPILEDNCODE(compiled);
	codepc  = code;
	codeend = code + ncode;

	EC_STACKCALLED(stack) = compiled;

	ASSERT( EC_NULLP(self) || EC_OBJECTP(self) );
	if (EC_NULLP(at_class) && EC_NNULLP(self))
		at_class = EC_OBJECTCLASS(self);

/*	LPRIVATE(rt.vm_level)++; */ /* We are not in the VM... #JP */

/*	fprintf( stderr, "VM LEVEL: %ld\n", LPRIVATE(rt.vm_level) );*/

#if TRACE_EXECUTION
	printf("\n");
	EcDumpCompiled( compiled, 0 );
	printf("\n");
	printf("LEXICALLY UPPER FRAME: 0x%08lX\n", (unsigned long)EC_STACKLEXICAL(stack));
#endif

	/* Set current compiled object & line number */
	LPRIVATE(rt.compiled)     = compiled;
	LPRIVATE(rt.line_num)     = 0;
	LPRIVATE(rt.exc_line_num) = -1;

	BACKTRACE( stack, compiled );

