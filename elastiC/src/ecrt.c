/* ==========================================================================
 * ecrt.c
 * ==========================================================================
 * ElastiC interpreter - run-time only version.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Tue Oct  2 16:24:25 CEST 2001
 *
 *   $Id: ecrt.c,v 1.2 2002/05/25 19:37:44 panta Exp $
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
char *__progname = "ecrt";
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

static char ecrt_version[] = "0.01";

static EcBool option_verbose = FALSE;

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
	while ((ch = getopt( argc, argv, "hvV" )) != -1)
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

	if (! EcInit())
		error( "can't initialize elastiC environment" );

	doExecute( name, argc, argv );

	EcCleanup();
	exit( EXIT_SUCCESS );
}

static int doExecute( const char *name, int argc, char *argv[] )
{
	EC_OBJ package, res;
	char *pkgname;
	int l;

	if (option_verbose)
		fprintf( stderr, "Loading `%s'\n", name );
	l = strlen( name );
	pkgname = alloca( l + 1 );
	if (! pkgname) return 0;
	if ((l >= 4) &&
		(strcmp( &name[l - strlen(EC_COMPILEDSUFFIX)], EC_COMPILEDSUFFIX ) == 0))
	{
		strcpy( pkgname, name );
		pkgname[l - strlen(EC_COMPILEDSUFFIX)] = '\0';
		package = EcPackageLoad( NULL, name, /* execute: */ FALSE, /* executeImported: */ TRUE );
		if (! EC_PACKAGEP(package))
			error( "Bad package binary file specified." );
	} else
	{
		strcpy( pkgname, name );
		package = EcPackageLoad( pkgname, NULL, /* execute: */ FALSE, /* executeImported: */ TRUE );
		if (! EC_PACKAGEP(package))
			error( "Bad package binary file specified." );
	}
	if (option_verbose)
		fprintf( stderr, "Load completed.\n" );

	EcSetArgs( argc, argv );
	res = EcMainExecute( package );

	return 1;
}

static void banner( void )
{
	fprintf( stderr, "ecrt version %s (elastiC %s)\n", ecrt_version, EcVersionString() );
	fprintf( stderr, "Copyright (C) 2001 Marco Pantaleoni. All rights reserved.\n" );
}

static void usage( void )
{
	banner();
	fprintf( stderr, "\nUsage: ecrt [options] objectfile [program options]\n" );
	fprintf( stderr, "Options:\n" );
	fprintf( stderr, "   -h                    Show usage information\n" );
	fprintf( stderr, "   -v                    Be verbose\n" );
	fprintf( stderr, "   -V                    Display version information\n" );
	fprintf( stderr, "\n" );
}

static void version( void )
{
	fprintf( stderr, "ecrt version: %s\n", ecrt_version );
	fprintf( stderr, "elastiC version: %s (dec: %lu)\n", EcVersionString(), (unsigned long)EcVersionNumber() );
}

static void error( const char *msg )
{
	fprintf( stderr, "ERROR: %s\n\n", msg );
	exit( EXIT_FAILURE );
}
