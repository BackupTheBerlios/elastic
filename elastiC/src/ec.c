/* ==========================================================================
 * ec.c
 * ==========================================================================
 * ElastiC interpreter.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Thu Nov 31 15:32: MEST 1998
 *
 *   $Id: ec.c,v 1.5 2005/03/08 13:40:05 panta Exp $
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

#include <stdio.h>
#include <stdlib.h>

#include "basic.h"
#include "elastic.h"
#include "compat.h"
#include "conf.h"


#if defined(MINGW32) || defined(EC_CCOMPILER_VC)
char *__progname = "ec";
#endif

/* ========================================================================
 * P R O T O T Y P E S
 * ======================================================================== */

int         main( int argc, char *argv[] );
static int  doExecute( const char *name, int argc, char *argv[] );
static void banner( void );
static void usage( void );
static void version( void );
static void error( const char *msg );

/* ========================================================================
 * P R I V A T E   D A T A
 * ======================================================================== */

static char ec_version[] = "0.03";

static EcBool option_verbose = FALSE;
static EcBool option_lazy    = FALSE;

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
	const char *name;
	int ch;

	/* Scan options */
	while ((ch = getopt( argc, argv, "hvlV" )) != -1)
	{
		switch ((char) ch)
		{
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

	/* skip options */
	argc -= optind;
	argv += optind;

	if (argc < 1)
	{
		usage();
		exit( EXIT_FAILURE );
	}

	/* skip package */
	argc--; name = *argv++;

#ifdef EC_THREADING
	if (! EcThreadingInit() || ! EcInit())
#else
	if (! EcInit())
#endif
		error( "can't initialize elastiC environment" );

	doExecute( name, argc, argv );

	EcCleanup();
	exit( EXIT_SUCCESS );
}

static int doExecute( const char *name, int argc, char *argv[] )
{
	ec_compiler_ctxt    ctxt;
	ec_compiler_options comp_opts;

	EC_OBJ package, res;
	char *pkgname;
	int l;

	if (option_verbose)
		ec_stderr_printf( "Loading `%s'\n", name );
	l = strlen( name );
	pkgname = alloca( l + 1 );
	if (! pkgname) return 0;
	if ((l >= 4) &&
		(strcmp( &name[l - strlen(EC_COMPILEDSUFFIX)], EC_COMPILEDSUFFIX ) == 0))
	{
		strcpy( pkgname, name );
		pkgname[l - strlen(EC_COMPILEDSUFFIX)] = '\0';
		package = EcPackageLoadByPath( name, /* execute: */ FALSE, /* executeImported: */ TRUE );
		if (! EC_PACKAGEP(package))
			error( "Bad package binary file specified." );
	} else if ((l >= 3) &&
			   (strcmp( &name[l - strlen(EC_SOURCESUFFIX)], EC_SOURCESUFFIX ) == 0))
	{
		if (option_verbose)
			ec_stderr_printf( "Compiling `%s'\n", name );

		comp_opts.whole_package = TRUE;							/* it's a whole package         */
		comp_opts.in_package    = EC_NIL;
		comp_opts.save          = FALSE;						/* don't save compiled packages */
		comp_opts.outputfile    = NULL;							/* output filename              */
		comp_opts.lazy          = option_lazy;					/* lazy programmer              */
		ctxt = EcCompilerContextCreate();
		package = EcCompile( ctxt,
							 name,
							 /* execute: */ FALSE, /* executeImported: */ TRUE,
							 &comp_opts );
		EcCompilerContextDestroy( ctxt );

		if (! EC_PACKAGEP(package))
			error( "Bad package source file specified." );
		if (option_verbose)
			ec_stderr_printf( "Compilation done.\n" );
	} else
	{
		strcpy( pkgname, name );
		package = EcPackageLoadByName( pkgname, /* execute: */ FALSE, /* executeImported: */ TRUE );
		if (! EC_PACKAGEP(package))
			error( "Bad package binary file specified." );
	}
	if (option_verbose)
		ec_stderr_printf( "Load completed.\n" );

	EcSetArgs( argc, argv );
	res = EcMainExecute( package );

	return 1;
}

static void banner( void )
{
	ec_stderr_printf( "ec version %s (elastiC %s)\n", ec_version, EcVersionString() );
	ec_stderr_printf( "Copyright (C) 1998-2001 Marco Pantaleoni. All rights reserved.\n" );
}

static void usage( void )
{
	banner();
	ec_stderr_printf( "\nUsage: ec [options] objectfile [program options]\n" );
	ec_stderr_printf( "Options:\n" );
	ec_stderr_printf( "   -h                    Show usage information\n" );
	ec_stderr_printf( "   -v                    Be verbose\n" );
	ec_stderr_printf( "   -l                    Lazy programmer. Allow implicit variable declaration\n" );
	ec_stderr_printf( "   -V                    Display version information\n" );
	ec_stderr_printf( "\n" );
}

static void version( void )
{
	ec_stderr_printf( "ec version: %s\n", ec_version );
	ec_stderr_printf( "elastiC version: %s (dec: %lu)\n", EcVersionString(), (unsigned long)EcVersionNumber() );
}

static void error( const char *msg )
{
	ec_stderr_printf( "ERROR: %s\n\n", msg );
	exit( EXIT_FAILURE );
}
