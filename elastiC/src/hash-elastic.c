#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <elastic/basic.h>
#include <elastic/hashtable.h>

#include <glib.h>

static EcAny  string_copy( EcAny obj );
static EcInt  string_compare( EcAny obj1, EcAny obj2 );
static void   string_destroy( EcAny obj );

static EC_API ec_hash_def string2int =
{
	(ec_lambda_hash)    ec_hash_pjw_string,

	(ec_lambda_copy)    string_copy,
	(ec_lambda_compare) string_compare,
	(ec_lambda_destroy) string_destroy,

	(ec_lambda_copy)    NULL,
	(ec_lambda_compare) NULL,
	(ec_lambda_destroy) NULL,

	(EcAny)             0xFFFFFFFF,
	(EcAny)             0xFFFFFFFF
};

int main(int argc, char *argv[])
{
	GSList list;

	if (! EcInit())
	{
		fprintf( stderr, "Damn!\n" );
		exit( EXIT_FAILURE );
	}

	EcCleanup();
	exit( EXIT_SUCCESS );
}

EcBool test_hash(GSList *list)
{
	ec_hash *hash;
	GSList  *tmp = list;

	hash = ec_hash_create( 0, 0, 0, &string2int );
	if (! hash) return FALSE;

	while (tmp)
	{
		ec_hash_set( hash, (EcAny) tmp->data, (EcAny) 1 );
		tmp = tmp->next;
	}

	ec_hash_destroy( hash );
	return TRUE;
}

static EcAny string_copy( EcAny obj )
{
	return (EcAny)ec_stringdup( (const char *)obj );
}

static EcInt string_compare( EcAny obj1, EcAny obj2 )
{
	if (obj1 == obj2)
		return 0;
	if (! obj1) return 1;
	if (! obj2) return 1;
	return (EcInt)strcmp( (const char *)obj1, (const char *)obj2 );
}

static void string_destroy( EcAny obj )
{
	ec_free( (void *)obj );
}
