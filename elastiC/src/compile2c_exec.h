/* ==========================================================================
 * compile2c_exec
 * ==========================================================================
 * elastiC C compiler exec file.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Jacopo Pantaleoni         E-mail: jp@lightflowtech.com
 *
 *   Created: Sun May 25 08:05:50 MEST 2003
 *
 *   $Id: compile2c_exec.h,v 1.1 2003/05/29 10:36:00 panta Exp $
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

	EC_OBJ stack;

	/* Let's prepare a suitable stack */
	stack = EcMakeStack( EC_NIL,
						 1 + 2 +							/* space for 1 caller object,
															   plus called object (self) and
															   called class in its hierarchy */
						 0 +								/* ... no args ...               */
						 EC_COMPILEDNLOC(compiled) +			/* # of local variables          */
						 EC_COMPILEDMAXTEMPS(compiled) );	/* # of temporaries              */
	EC_STACKNARGS_REAL(stack)    = 0;						/* set actual # of passed args   */
	EC_STACKNARGS_LOGICAL(stack) = 0;						/* set logical # of passed args  */

	EC_STACKLEXICAL(stack) = EC_NIL;						/* lexical frame */

	EcStackPush( stack, EC_NIL );							/* caller bytecode               */
	EcStackPush( stack, EC_NIL );							/* called object (self)          */
	EcStackPush( stack, EC_NIL );							/* called at_class (self class)  */
	EC_STACKPC(stack) = 0;									/* caller PC                     */

	EC_STACKIMMUP(stack)     = EC_NIL;						/* inline up        */
	EC_STACKIMMCALLER(stack) = EC_NIL;						/* inline caller    */
	EC_STACKIMMPC(stack)     = 0;							/* inline caller PC */

	EC_STACKCALLED(stack)    = compiled;						/* called bytecode  */

	/* Set SP and BP */
	EC_STACKBP(stack)      = EC_STACKBASE(stack) + 1 + 2 + 0;/* ... no args ... */
	EC_STACKPOINTER(stack) = EC_STACKBP(stack) + EC_COMPILEDNLOC(compiled);

	PRIVATE(globalFrame) = stack;							/* this stack is the main (top-level) stack frame */
