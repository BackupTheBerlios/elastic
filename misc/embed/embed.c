/* ==========================================================================
 * embed.c
 * ==========================================================================
 * Example of an application embedding elastiC.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sun Sep 23 13:18:08 CEST 2001
 *
 *   $Id: embed.c,v 1.1 2002/05/30 17:46:05 panta Exp $
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

#include <elastic/basic.h>
#include <elastic/elastic.h>
#include <elastic/bytecode.h>
#include <elastic/debug.h>

#include <elastic/private.h>


int main( int argc, char *argv[] );
static void error( const char *msg );

static EC_OBJ perform( const char *package_filename, const char *function_name, EcBool execute_package );


/*
 * The following function:
 *    - loads the specified precompiled package in memory
 *    - executes the specified function in the elastiC package
 *    - returns the result
 */
static EC_OBJ perform( const char *package_filename, const char *function_name, EcBool execute_package )
{
	EC_OBJ package;
	EC_OBJ pkgcode;
	EC_OBJ stack;

	EC_OBJ function_bytecode;

	EC_OBJ res;

	const char *package_name;

	package = EcPackageLoad( NULL, package_filename,
							 FALSE,								/* don't execute package global code (now) */
							 TRUE );							/* execute code in imported packages       */

	if (! EC_PACKAGEP(package))
		return package;

	/* Do we have to execute package global code ? */
	if (execute_package)
	{
		pkgcode = EC_PACKAGECODE(package);

		/* Let's prepare a suitable stack */
		stack = EcMakeStack( EC_NIL,
							 1 + 2 +							/* space for 1 caller object,
																   plus called object (self) and
																   called class in its hierarchy */
							 0 +								/* ... no args ...               */
							 EC_COMPILEDNLOC(pkgcode) +			/* # of local variables          */
							 EC_COMPILEDMAXTEMPS(pkgcode) );	/* # of temporaries              */
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

		EC_STACKCALLED(stack)    = pkgcode;						/* called bytecode  */

		/* Set SP and BP */
		EC_STACKBP(stack)      = EC_STACKBASE(stack) + 1 + 2 + 0;/* ... no args ... */
		EC_STACKPOINTER(stack) = EC_STACKBP(stack) + EC_COMPILEDNLOC(pkgcode);

		PRIVATE(globalFrame) = stack;							/* this stack is the main (top-level) stack frame */

		res = EcExecute( EC_NIL, EC_NIL, pkgcode, stack );

		if (EC_ERRORP(res))
			return res;
	}

	/* Get actual package name */
	package_name = EC_STRDATA(EC_PACKAGENAME(package));

	/* Set some globals */

	EcSetVariable(package_name, "myvar1", EcMakeInt(12));
	EcSetVariable(package_name, "myvar2", EcMakeArrayInit(4,
														  EcMakeFloat(4.0),
														  EcMakeInt(7),
														  EcMakeSymbol("aSymbol"),
														  EcMakeString("aString", -1)));

	/* Call the function in `function_name' */

	/*
	 * Functions in elastiC are first class objects, and their namespace is
	 * the very same of all other variables...
	 * This means that we can use functions to access variables to obtain their
	 * compiled code objects.
	 */
	function_bytecode = EcGetVariable(package_name, function_name);

	res = EcCall( EC_NIL,										/* no caller stack             */
				  EC_NIL,										/* (called object) self        */
				  EC_NIL,										/* (called class) at_class     */
				  function_bytecode,							/* called bytecode             */
				  3,											/* # of args                   */
				  EcMakeInt( 5 ),								/* pass an integer...          */
				  EcMakeString( "Hello World", -1 ),			/* a string (calc. the length) */
				  EcMakeBool( TRUE )							/* ... and a boolean value     */
		);

	if (EC_ERRORP(res))
		return res;

	ec_fprintf(stderr, "\nReturn value: %r\n", res);

	return res;
}

int main( int argc, char *argv[] )
{
	EC_OBJ res;

	if (! EcInit())
		error( "can't initialize elastiC environment" );

	EcSetArgs( argc, argv );

	res = perform( "embedded.ecc", "runme", TRUE );

	EcCleanup();
	exit( EXIT_SUCCESS );
}

static void error( const char *msg )
{
	fprintf( stderr, "ERROR: %s\n\n", msg );
	exit( EXIT_FAILURE );
}
