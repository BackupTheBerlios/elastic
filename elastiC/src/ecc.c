/* ==========================================================================
 * ecc.c
 * ==========================================================================
 * ElastiC compiler driver.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Thu Jul 16 15:05:56 MEST 1998
 *
 *   $Id: ecc.c,v 1.4 2005/03/08 13:40:05 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1998-2005 Marco Pantaleoni. All rights reserved.
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

#include <stdio.h>
#include <stdlib.h>

#include "basic.h"
#include "elastic.h"
#include "compat.h"


#if defined(MINGW32) || defined(EC_CCOMPILER_VC)
char *__progname = "ecc";
#endif

/* ========================================================================
 * P R O T O T Y P E S
 * ======================================================================== */

int           main( int argc, char *argv[] );
static EcBool doCompile( const char *name, const char *output, EcBool lazy );
static void   banner( void );
static void   usage( void );
static void   version( void );
static void   error( const char *msg );

/* ========================================================================
 * P R I V A T E   D A T A
 * ======================================================================== */

static char ecc_version[] = "0.03";

static char   *option_outputfile = NULL;
static EcBool  option_debug      = FALSE;
static EcBool  option_verbose    = FALSE;
static EcBool  option_lazy       = FALSE;

/* ========================================================================
 * I M P L E M E N T A T I O N
 * ======================================================================== */

	/* getopt */
#if ! HAVE_GETOPT
extern int   getopt(int argc, char * const argv[], const char *optstring);
extern char *optarg;
extern int   optind, opterr, optopt;

#include "compat/getopt.c"
#endif

int main( int argc, char *argv[] )
{
	int ch;
	char *source;

	/* Scan options */
	while ((ch = getopt( argc, argv, "dhvlVo:" )) != -1)
	{
		switch ((char) ch)
		{
		case 'd':
			option_debug = TRUE;
			break;

		case 'o':
			option_outputfile = ec_stringdup( optarg );
			break;

		case 'h':
			usage();
			exit( EXIT_SUCCESS );
			break;

		case 'v':
			option_verbose = TRUE;
			break;

		case 'l':
			option_lazy = TRUE;
			break;

		case 'V':
			version();
			exit( EXIT_SUCCESS );
			break;

		default:
			usage();
			exit( EXIT_FAILURE );
			break;
		}
	}

	if (optind >= argc)
	{
		usage();
		exit( EXIT_FAILURE );
	}

	source = argv[optind];

#ifdef EC_THREADING
	if (! EcThreadingInit() || ! EcInit())
#else
	if (! EcInit())
#endif
	{
		error( "can't initialize elastiC environment" );
		goto onError;
	}

	if (! doCompile( source, option_outputfile, option_lazy ))
	{
		error( "error during compilation" );
		goto onError;
	}

	/* OK */
	ec_free( option_outputfile );
	EcCleanup();
	exit( EXIT_SUCCESS );

onError:
	ec_free( option_outputfile );
	EcCleanup();
	exit( EXIT_FAILURE );
}

static EcBool doCompile( const char *name, const char *output, EcBool lazy )
{
	ec_compiler_ctxt    ctxt;
	ec_compiler_options comp_opts;

	EC_OBJ compiled;

	comp_opts.whole_package = TRUE;								/* it's a whole package   */
	comp_opts.in_package    = EC_NIL;
	comp_opts.save          = TRUE;								/* save compiled packages */
	comp_opts.outputfile    = (char *) output;					/* output filename        */
	comp_opts.lazy          = lazy;
	ctxt = EcCompilerContextCreate();

	compiled = EcCompile( ctxt, name, FALSE, FALSE, &comp_opts );
	EcCompilerContextDestroy( ctxt );
	if (EC_ERRORP(compiled))
		return FALSE;

	return TRUE;
}

static void banner( void )
{
	ec_stderr_printf( "ecc version %s (elastiC %s)\n", ecc_version, EcVersionString() );
	ec_stderr_printf( "Copyright (C) 1998-2000 Marco Pantaleoni. All rights reserved.\n" );
}

static void usage( void )
{
	banner();
	ec_stderr_printf( "\nUsage: ecc [options] sourcefile\n" );
	ec_stderr_printf( "Options:\n" );
	ec_stderr_printf( "   -o FILE               Set output file name\n" );
	ec_stderr_printf( "   -d                    Enable debugging information\n" );
	ec_stderr_printf( "   -h                    Show usage information\n" );
	ec_stderr_printf( "   -v                    Be verbose\n" );
	ec_stderr_printf( "   -l                    Lazy programmer. Allow implicit variable declaration\n" );
	ec_stderr_printf( "   -V                    Display version information\n" );
	ec_stderr_printf( "\n" );
}

static void version( void )
{
	ec_stderr_printf( "ecc version: %s\n", ecc_version );
	ec_stderr_printf( "elastiC version: %s (dec: %lu)\n", EcVersionString(), (unsigned long)EcVersionNumber() );
}

static void error( const char *msg )
{
	ec_stderr_printf( "ERROR: %s\n\n", msg );
}
