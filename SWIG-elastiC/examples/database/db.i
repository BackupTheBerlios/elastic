// Simple database-like module.
// This shows how one can convert to and from C structures
// and elastiC arrays.

%module db

%{
#include "db.h"
%}

// We use a typemap to define how we want to parse User records

// Define how a User record is to be passed into a C function
// This works for both "User" and "User *"

%typemap(elastic,in) User * {
	static User temp;                /* A temporary holding place */
	char       *login,*name,*home;   /* Temporary variables       */
	EC_OBJ      obj;

	if (EC_NULLP($source)) return NULL;
	if (! EC_ARRAYP($source))
	{
		EcAlert(EcError, "Bad type passed to %s.", $name);
		return NULL;
	}
	if (EC_ARRAYLEN($source) != 5)
	{
		EcAlert(EcError, "Wrong # of fields in argument to %s.", $name);
		return NULL;
	}

	obj = EcArrayGet($source, 0);
	if (! EC_STRINGP(obj))
	{
		EcAlert(EcError, "Wrong types for field 1 in argument to %s.", $name);
		return NULL;
	}
	login = EC_STRDATA(obj);

	obj = EcArrayGet($source, 1);
	if (! EC_INUMP(obj))
	{
		EcAlert(EcError, "Wrong types for field 2 in argument to %s.", $name);
		return NULL;
	}
	temp.uid = EC_INUM(obj);

	obj = EcArrayGet($source, 2);
	if (! EC_INUMP(obj))
	{
		EcAlert(EcError, "Wrong types for field 3 in argument to %s.", $name);
		return NULL;
	}
	temp.gid = EC_INUM(obj);

	obj = EcArrayGet($source, 3);
	if (! EC_STRINGP(obj))
	{
		EcAlert(EcError, "Wrong types for field 3 in argument to %s.", $name);
		return NULL;
	}
	name = EC_STRDATA(obj);

	obj = EcArrayGet($source, 4);
	if (! EC_STRINGP(obj))
	{
		EcAlert(EcError, "Wrong types for field 4 in argument to %s.", $name);
		return NULL;
	}
	home = EC_STRDATA(obj);

	/* Safely copy passed values into our structure */

	strncpy(temp.login, login, 16);
	strncpy(temp.name, name, 32);
	strncpy(temp.home, home, 256);

	/* Set the User * pointer to our temporary structure */

	$target = &temp;
}

// Describe how we want to return a user record

%typemap(elastic,out) User * {
	if ($source)
	{
		$target = EcBuildValue("(siiss)",
							   $source->login, $source->uid,
							   $source->gid,   $source->name, $source->home);
	}
	else
		$target = EC_NIL;
}

extern void  add_user(User u);
extern void  sort_users(void);
extern User *lookup_user(char *name);
extern User *get_user(int index);
extern int   num_users(void);

/* A global variable */
extern float myv;

