/* ==========================================================================
 * getsetenv.c
 * ==========================================================================
 * getenv(3) and setenv(3) implementation.
 * Derived from source code in BSD. See below for original licensing.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Thu Oct  4 13:47:39 CEST 2001
 *
 *   $Id: getsetenv.c,v 1.1 2002/05/23 21:17:52 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 2001-2002 Marco Pantaleoni. All rights reserved.
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

/*
 * This file has been modified by Marco Pantaleoni (Oct 2001).
 * These modifications are Copyright (C) 2001 Marco Pantaleoni.
 *
 * The original is covered by the following copyright notice:
 */

/*
 * Copyright (c) 1987, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * NOTE: As stated in
 *  ftp://ftp.cs.berkeley.edu/pub/4bsd/README.Impt.License.Change
 * the so called `advertising clause' in the above BSD license doesn't
 * apply anymore.
 */

#include "config.h"

#include <stdlib.h>
#if HAVE_STRING_H
#include <string.h>
#endif

#if ! defined(MINGW32)
extern char **environ;
#endif

/*
 * Always use a pointer to `environ', to avoid possible bugs related
 * to interaction with shared libraries, or systems where the user
 * declares his own `environ'.
 */
#if defined(MINGW32)
static char **environ_c = NULL;
static char ***p_environ = &environ_c;
#else
static char ***p_environ = &environ;
#endif

/*
 * _findenv --
 *	Returns pointer to value associated with name, if any, else NULL.
 *	Sets offset to be the offset of the name/value combination in the
 *	environmental array, for use by setenv(3) and unsetenv(3).
 *	Explicitly removes '=' in argument name.
 *
 *	This routine *should* be a static; don't use it.
 */
static inline char *_findenv(register const char *name, int *offset)
{
	register int len, i;
	register const char *np;
	register char **p, *cp;

#if defined(MINGW32)
	if (! environ_c)
		environ_c = environ;
#endif

	if (name == NULL || *p_environ == NULL)
		return (NULL);
	for (np = name; *np && *np != '='; ++np)
		continue;
	len = np - name;
	for (p = *p_environ; (cp = *p) != NULL; ++p) {
		for (np = name, i = len; i && *cp; i--)
			if (*cp++ != *np++)
				break;
		if (i == 0 && *cp++ == '=') {
			*offset = p - *p_environ;
			return (cp);
		}
	}
	return (NULL);
}

/*
 * getenv --
 *	Returns ptr to value associated with name, if any, else NULL.
 */

#if ! HAVE_GETENV
/*
 * getenv --
 *	Returns ptr to value associated with name, if any, else NULL.
 */
char *getenv(const char *name)
{
	int offset;

	return (_findenv(name, &offset));
}
#endif

#if ! HAVE_SETENV
/*
 * setenv --
 *	Set the value of the environmental variable "name" to be
 *	"value".  If rewrite is set, replace any current value.
 */
int setenv(const char *name, const char *value, int rewrite)
{
	static char **alloced;										/* if allocated space before */
	register char *c;
	int l_value, offset;

#if defined(MINGW32)
	if (! environ_c)
		environ_c = environ;
#endif

	if (*value == '=')											/* no `=' in value */
		++value;
	l_value = strlen(value);
	if ((c = _findenv(name, &offset))) {						/* find if already exists */
		if (!rewrite)
			return (0);
		if (strlen(c) >= l_value) {								/* old larger; copy over */
			while ( (*c++ = *value++) );
			return (0);
		}
	} else {													/* create new slot */
		register int cnt;
		register char **p;

		for (p = *p_environ, cnt = 0; *p; ++p, ++cnt);
		if (alloced == *p_environ) {							/* just increase size */
			p = (char **)realloc((char *)(*p_environ),
			    (size_t)(sizeof(char *) * (cnt + 2)));
			if (!p)
				return (-1);
			alloced = *p_environ = p;
		}
		else {													/* get new space */
																/* copy old entries into it */
			p = malloc((size_t)(sizeof(char *) * (cnt + 2)));
			if (!p)
				return (-1);
			memcpy(p, *p_environ, cnt * sizeof(char *));
			alloced = *p_environ = p;
		}
		(*p_environ)[cnt + 1] = NULL;
		offset = cnt;
	}
	for (c = (char *)name; *c && *c != '='; ++c);				/* no `=' in name */
	if (!((*p_environ)[offset] =								/* name + `=' + value */
		  malloc((size_t)((int)(c - name) + l_value + 2))))
		return (-1);
	for (c = (*p_environ)[offset]; (*c = *name++) && *c != '='; ++c);
	for (*c++ = '='; (*c++ = *value++); );
	return (0);
}
#endif

#if ! HAVE_UNSETENV
/*
 * unsetenv(name) --
 *	Delete environmental variable "name".
 */
void unsetenv(const char *name)
{
	register char **p;
	int offset;

#if defined(MINGW32)
	if (! environ_c)
		environ_c = environ;
#endif

	while (_findenv(name, &offset))	/* if set multiple times */
		for (p = &(*p_environ)[offset];; ++p)
			if (!(*p = *(p + 1)))
				break;
}
#endif
