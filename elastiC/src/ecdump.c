/* ==========================================================================
 * ecdump.c
 * ==========================================================================
 * ElastiC disassembler.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Thu Nov 31 15:32: MEST 1998
 *
 *   $Id: ecdump.c,v 1.4 2005/03/03 23:51:43 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1999-2002 Marco Pantaleoni. All rights reserved.
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
#include "private.h"
#include "compat.h"
#include "debug.h"


#if defined(MINGW32) || defined(EC_CCOMPILER_VC)
char *__progname = "ecdump";
#endif

/* ========================================================================
 * P R O T O T Y P E S
 * ======================================================================== */

int         main( int argc, char *argv[] );
static int  doDump( const char *name );
static void banner( void );
static void usage( void );
static void version( void );
static void error( const char *msg );

static void dump( FILE *sourcefh, EcInt level, EC_OBJ obj );
static void dump_bytecode( FILE *sourcefh, EcInt level, EC_OBJ compiled, EcInt at );

/* ========================================================================
 * P R I V A T E   D A T A
 * ======================================================================== */

static char ecdump_version[] = "0.03";

static EcBool option_all = FALSE;
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
	int   ch;
	char *file;

	/* Scan options */
	while ((ch = getopt( argc, argv, "ahvV" )) != -1)
	{
		switch ((char) ch)
		{
		case 'a':
			option_all = TRUE;
			break;

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

	file = argv[optind];

#ifdef EC_THREADING
	if (! EcThreadingInit() || ! EcInit())
#else
	if (! EcInit())
#endif
		error( "can't initialize elastiC environment" );

	doDump( file );

	EcCleanup();
	exit( EXIT_SUCCESS );
}

#define MAXLINES 8192
#define MAXLINE  8192
static EcInt lineoffs[MAXLINES];
static char line[MAXLINE];

static int doDump( const char *filename )
{
	EC_OBJ  package;
	char   *source;
	FILE   *sourcefh;
	EcInt   linenum;

	if (option_verbose)
		fprintf( stderr, "Loading `%s'\n", filename );
	package = EcPackageLoadByPath( filename, /* execute: */ FALSE, /* executeImported: */ FALSE );
	if (! EC_PACKAGEP(package))
	{
		EcAlert( EcError, "bad package binary file specified: %s", filename );
		return 0;
	}
	if (option_verbose)
		fprintf( stderr, "Load completed.\n" );

	/*
	 * Read source file
	 */
	if (! EC_STRINGP(EC_PACKAGESOURCE(package)))
	{
		EcAlert( EcError, "unknown source file." );
		return 0;
	}

	source = EC_STRDATA(EC_PACKAGESOURCE(package));
	sourcefh = fopen( source, "rb" );
	if (! sourcefh)
	{
		EcAlert( EcError, "can't open source file `%s'.", source );
		return 0;
	}
	linenum = 0;
	for (;;)
	{
		lineoffs[linenum++] = ftell( sourcefh );
		if (! fgets( line, MAXLINE, sourcefh )) break;
	}

	/* Dump */
	dump( sourcefh, 0, package );

	fclose( sourcefh );
	return 1;
}

static void indent( EcInt level )
{
	EcInt i;

	for (i = 0; i < level; i++)
		fprintf( stdout, "    " );
	fflush( stdout );
}

#define IND0	do { indent( level );     } while (0)
#define IND1	do { indent( level + 1 ); } while (0)

static void dump( FILE *sourcefh, EcInt level, EC_OBJ obj )
{
	EC_OBJ sub;
	EcInt  i;

	if (EC_NULLP(obj))
		return;

	if (EC_ARRAYP(obj))
	{
		IND0; ec_fprintf( stdout, "ARRAY %r:\n", obj ); fflush( stdout );
		if (option_all)
		{
			for (i = 0; i < EC_ARRAYLEN(obj); i++)
			{
				sub = EcArrayGet( obj, i );
				IND1; ec_fprintf( stdout, "ELEMENT %ld: %w\n", i, sub ); fflush( stdout );
				dump( sourcefh, level + 1, sub ); fflush( stdout );
			}
		}
	} else if (EC_PACKAGEP(obj))
	{
		IND0; ec_fprintf( stdout, "PACKAGE %r:\n", obj ); fflush( stdout );
		dump( sourcefh, level + 1, EC_PACKAGECODE(obj) ); fflush( stdout );
		if (option_all)
		{
			for (i = 0; i < EC_PACKAGENEXPORT(obj); i++)
			{
				ec_fprintf( stdout, "\n" ); fflush( stdout );
				sub = EcArrayGet( EC_PACKAGEFRAME(obj), EC_PACKAGEEXPORT(obj)[i].pos );
				IND1; ec_fprintf( stdout, "EXPORTED %k at %ld: %w\n", EC_PACKAGEEXPORT(obj)[i].sym, (long)i, sub ); fflush( stdout );
				dump( sourcefh, level + 1, sub ); fflush( stdout );
			}

			dump( sourcefh, level + 1, EC_PACKAGEFRAME(obj) ); fflush( stdout );
		}
	} else if (EC_COMPILEDP(obj))
	{
		IND0; ec_fprintf( stdout, "BYTECODE %r:\n", obj ); fflush( stdout );
		dump_bytecode( sourcefh, level + 1, obj, 0 ); fflush( stdout );
	} else if (EC_HANDLERP(obj))
	{
		IND0; ec_fprintf( stdout, "HANDLER %r:\n", obj ); fflush( stdout );
		if (option_all)
		{
			dump( sourcefh, level + 1, EC_HANDLERCODE(obj) ); fflush( stdout );
		}
	} else if (EC_CLASSP(obj))
	{
		IND0; ec_fprintf( stdout, "CLASS %r:\n", obj ); fflush( stdout );
		if (option_all)
		{
			EcUInt symid;

			for (i = 0; i < EC_CLASSNMETHODS(obj); i++)
			{
				symid = EC_CLASSMTABLE(obj)[i].symid;
				sub   = EC_CLASSMTABLE(obj)[i].impl;

				IND1; ec_fprintf( stdout, "METHOD %k: %w\n", symid, sub ); fflush( stdout );
				dump( sourcefh, level + 1, sub ); fflush( stdout );
			}

			for (i = 0; i < EC_CLASSNCMETHODS(obj); i++)
			{
				symid = EC_CLASSCMTABLE(obj)[i].symid;
				sub   = EC_CLASSCMTABLE(obj)[i].impl;

				IND1; ec_fprintf( stdout, "CLASS METHOD %k: %w\n", symid, sub ); fflush( stdout );
				dump( sourcefh, level + 1, sub ); fflush( stdout );
			}
		}
	}
	fflush( stdout );
}

static void banner( void )
{
	fprintf( stderr, "ecdump version %s (elastiC %s)\n", ecdump_version, EcVersionString() );
	fprintf( stderr, "Copyright (C) 1999-2000 Marco Pantaleoni. All rights reserved.\n" );
}

static void usage( void )
{
	banner();
	fprintf( stderr, "\nUsage: ecdump [options] objectfile\n" );
	fprintf( stderr, "Options:\n" );
	fprintf( stderr, "   -a                    Dump all\n" );
	fprintf( stderr, "   -h                    Show usage information\n" );
	fprintf( stderr, "   -v                    Be verbose\n" );
	fprintf( stderr, "   -V                    Display version information\n" );
	fprintf( stderr, "\n" );
}

static void version( void )
{
	fprintf( stderr, "ecdump version: %s\n", ecdump_version );
	fprintf( stderr, "elastiC version: %s (dec: %lu)\n", EcVersionString(), (unsigned long)EcVersionNumber() );
}

static void error( const char *msg )
{
	fprintf( stderr, "ERROR: %s\n\n", msg );
	exit( EXIT_FAILURE );
}


static void dump_bytecode( FILE *sourcefh, EcInt level, EC_OBJ compiled, EcInt at )
{
	EcInt       i, j;
	EcBytecode  bc, op[2];
	const char *name;
	int         npar;
	char       *compname;
	EC_OBJ      info, line_array = EC_NIL, line_tp;
	EcInt       ln_len = 0, lni;

	ASSERT( EC_COMPILEDP(compiled) );

	info = EC_COMPILEDINFO(compiled);
	if (EC_NNULLP(info))
	{
		EcHashGet( info, EcMakeSymbol( "line" ), &line_array );
		if (EC_ARRAYP(line_array))
			ln_len = EC_ARRAYLEN(line_array);
	}

	if (EC_STRINGP(EC_COMPILEDNAME(compiled)))
		compname = EC_STRDATA(EC_COMPILEDNAME(compiled));
	else
		compname = "$ANONYMOUS$";

	IND0; printf( "== Compiled   %-20s  0x%08lX ========\n", compname, (unsigned long)compiled );
	IND0; printf( "Bytecode len.       : %ld\n", (long)EC_COMPILEDNCODE(compiled) );
	IND0; printf( "# req. arguments    : %ld\n", (long)EC_COMPILEDNARG(compiled) );
	IND0; printf( " of which, defaulted: %ld\n", (long)EC_COMPILEDNARG_DEF(compiled) );
	IND0; printf( "         varargs    : %s\n",  EC_COMPILEDVARG(compiled) ? "yes" : "no" );
	IND0; printf( "# locals            : %ld\n", (long)EC_COMPILEDNLOC(compiled) );
	IND0; printf( "# max. temps        : %ld\n", (long)EC_COMPILEDMAXTEMPS(compiled) );
	IND0; printf( "\n" );
	IND0; printf( "-- Literal Frame --------------------------------------\n" ); fflush( stdout );
	for (i = 0; i < EC_ARRAYLEN(EC_COMPILEDLFRAME(compiled)); i++)
	{
		IND0; printf( "%5ld  ", (long)i );
		ec_fprintf( stdout, "%r\n", EcArrayGet( EC_COMPILEDLFRAME(compiled), i ) );
		dump( sourcefh, level + 1, EcArrayGet( EC_COMPILEDLFRAME(compiled), i ) );
	}
	IND0; printf( "\n" );
	IND0; printf( "-- Listing --------------------------------------------------------\n" );
	IND0; printf( "   ADDR            BYTECODE           OP1           OP2       STACK\n" );
	IND0; printf( "-------------------------------------------------------------------\n" );
	for (i = 0; i < EC_COMPILEDNCODE(compiled); i++)
	{
		if (ln_len > 0)
		{
			EcInt bc_idx, l_num, line_num;

			line_num = -1;
			for (lni = 0; lni < ln_len; lni++)
			{
				line_tp = EcArrayGet( line_array, lni );
				bc_idx = EC_INUM(EcArrayGet( line_tp, 0 ));
				l_num  = EC_INUM(EcArrayGet( line_tp, 1 ));

				if ((EcUInt)bc_idx == i) {
					line_num = l_num;
					break;
				} else
					line_num = -1;
			}

			if (line_num >= 0)
			{
				fseek( sourcefh, lineoffs[line_num - 1], SEEK_SET );
				fgets( line, MAXLINE, sourcefh );
				IND0; printf( "\n%3ld: %s\n", (long)line_num, line );
			}
		}

		bc = EC_COMPILEDCODE(compiled)[i];
		name = EcBytecodeName( bc );
		npar = EcBytecodeParams( bc );

		if ((at >= 0) && (at == i)) {
			IND0; printf( " *%5ld  ", (long)i );
		} else {
			IND0; printf( "  %5ld  ", (long)i );
		}
		printf( "%18s", name );
		for (j = i+1; j <= i+npar; j++)
		{
			op[j - (i+1)] = EC_COMPILEDCODE(compiled)[j];
			printf( "  %12ld", (long)EC_COMPILEDCODE(compiled)[j] );
		}
		if (npar < 1)
			printf( "  %12s", "" );
		if (npar < 2)
			printf( "  %12s", "" );

		switch (bc)
		{
		case CallOP:
		case InlinedCallOP:
			printf( "%12d", (int)(EcBytecodeStackgrow( bc ) - op[0]));
			break;
		case CallMethodOP:
			printf( "%12d", (int)(EcBytecodeStackgrow( bc ) - op[1]));
			break;
		default:
			printf( "%12d", (int)EcBytecodeStackgrow( bc ) );
			break;
		}
		i += npar;
		printf( "\n" );
	}
}
