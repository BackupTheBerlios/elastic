/* ==========================================================================
 * inet.c
 * ==========================================================================
 * Internet networking elastiC module.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Fri Dec 31 16:55:45 MET 1999
 *
 *   $Id: inet.c,v 1.1 2002/05/30 13:23:13 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1999-2000 Marco Pantaleoni. All rights reserved.
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
#include <elastic/debug.h>
#include <elastic/elastic.h>

#include "config.h"
#include "inet.h"

static EcInt tc_sockaddr = -1;

#define EC_SOCKADDRP(obj)	(EC_TYPEP(obj,tc_sockaddr))
#define EC_SOCKADDR(obj)	(EC_USER0(obj, struct sockaddr *))
#define EC_SOCKADDRLEN(obj)	(EC_USER1(obj, socklen_t))

static EC_OBJ sockaddr_copy( EC_OBJ obj, EcCopyType type );
static void   sockaddr_mark( EC_OBJ obj );
static void   sockaddr_free( EC_OBJ obj );
static EcInt  sockaddr_print( ec_string *str, EC_OBJ obj, EcBool detailed );
static EcUInt sockaddr_hash( EC_OBJ obj, EcInt recur );

/* C API */

static EC_OBJ EcMakeSocket( nw_socket sock )
{
	EC_OBJ res;

	res = EcLibFileFDOpen( sock, "w+" );
/*	if (EC_ERRORP(res)) return res;
	setvbuf( EC_FILEH(res), NULL, _IONBF, 0 );*/
	return res;
}

static EC_OBJ EcMakeSockaddr( struct sockaddr *addr, socklen_t addrlen )
{
	EC_OBJ res;
	struct sockaddr *copy;

	if (addrlen > 0)
	{
		copy = ec_malloc( addrlen );
		if (! copy) return EcMemoryError();
		memcpy( copy, addr, addrlen );
	} else
		copy = NULL;

	res = EcMakeUser( tc_sockaddr, NULL );
	if (EC_ERRORP(res)) return res;

	EC_SOCKADDR(res)    = copy;
	EC_SOCKADDRLEN(res) = addrlen;
	return res;
}

/* elastiC API */

static EC_OBJ EcLibInet_Open( EC_OBJ stack, EcAny userdata )
{
	char  *host;
	char  *service = NULL;
	EcInt  port    = 0;
	nw_socket sockfd = NW_INVALID_SOCKET;
	EC_OBJ res, obj;

	res = EcParseStackFunction( "inet.open", FALSE, stack, ".O", &obj );
	if (EC_ERRORP(res))
		return res;

	if ((! EC_STRINGP(obj)) && (! EC_INUMP(obj)))
	{
		res = EcTypeError( /* target   */ EC_NIL,
						   /* function */ EcGetQVariable( "inet.open" ),
						   2, tc_none, obj, TRUE, "expected a 'string' or an 'int'" );
		return res;
	}


	if (EC_STRINGP(obj))
		res = EcParseStackFunction( "inet.open", TRUE, stack, "zz", &host, &service );
	else
		res = EcParseStackFunction( "inet.open", TRUE, stack, "zi", &host, &port );
	if (EC_ERRORP(res))
		return res;

	if (service && (service[0] == '\0'))
		service = NULL;

	sockfd = nw_tcpopen( host, service, port );
	if (sockfd == NW_INVALID_SOCKET)
	{
		const char *msg;

		msg = nw_get_error();
		if (! msg)
			msg = "unknown error";
		return EcIOError( EC_NIL, msg );
	}
	return EcMakeSocket( sockfd );
}

static EC_OBJ EcLibInet_Socket( EC_OBJ stack, EcAny userdata )
{
	EcInt  port;
	nw_socket sockfd = NW_INVALID_SOCKET;
	EC_OBJ res;

	res = EcParseStackFunction( "inet.socket", TRUE, stack, "i", &port );
	if (EC_ERRORP(res))
		return res;

	sockfd = nw_make_socket( port );
	if (sockfd == NW_INVALID_SOCKET)
	{
		const char *msg;

		msg = nw_get_error();
		if (! msg)
			msg = "unknown error";
		return EcIOError( EC_NIL, msg );
	}
	return EcMakeInt( sockfd );
}

static EC_OBJ EcLibInet_Listen( EC_OBJ stack, EcAny userdata )
{
	EcInt  sockfd;
	EcInt  maxqueue = 5;
	EC_OBJ res;

	res = EcParseStackFunction( "inet.listen", TRUE, stack, "i|i", &sockfd, &maxqueue );
	if (EC_ERRORP(res))
		return res;

	if (listen( (nw_socket) sockfd, maxqueue ) < 0)
	{
		const char *msg;

		nw_set_error( NW_ERROR, "in 'listen' call." );

		msg = nw_get_error();
		if (! msg)
			msg = "unknown error";
		return EcIOError( EcMakeInt( (EcInt) sockfd ), msg );
	}
	return EcTrueObject;
}

static EC_OBJ EcLibInet_Accept( EC_OBJ stack, EcAny userdata )
{
	EcInt              sockfd;
	struct sockaddr_in clientname;
	size_t             size;
	nw_socket          sock;
	EC_OBJ             res, obj;

	res = EcParseStackFunction( "inet.accept", TRUE, stack, "i", &sockfd );
	if (EC_ERRORP(res))
		return res;

	size = sizeof(clientname);
	sock = accept( sockfd, (struct sockaddr *) &clientname, &size );
	if ((sock == NW_INVALID_SOCKET) ||
		(sock < 0))
	{
		const char *msg;

		nw_set_error( NW_ERROR, "in 'accept' call." );

		msg = nw_get_error();
		if (! msg)
			msg = "unknown error";
		return EcIOError( EcMakeInt( (EcInt) sockfd ), msg );
	}

	res = EcMakeArray( 2 );
	if (EC_ERRORP(res)) return res;

	obj = EcMakeSocket( sock );
	if (EC_ERRORP(obj)) return obj;
	if (EcArrayPush( res, obj ) < 0)
		return Ec_ERROR;

	obj = EcMakeSockaddr( (struct sockaddr *) &clientname, size );
	if (EC_ERRORP(obj)) return obj;
	if (EcArrayPush( res, obj ) < 0)
		return Ec_ERROR;

	return res;
}

static EC_OBJ EcLibInet_NToA( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ              sockaddr_obj;
	struct sockaddr_in *clientname;
	EC_OBJ res, obj;

	res = EcParseStackFunction( "inet.ntoa", TRUE, stack, "O!", tc_sockaddr, &sockaddr_obj );
	if (EC_ERRORP(res))
		return res;

	res = EcMakeArray( 2 );
	if (EC_ERRORP(res)) return res;

	clientname = (struct sockaddr_in *)EC_SOCKADDR(sockaddr_obj);
	obj = EcMakeString( inet_ntoa( clientname->sin_addr ), 0 );
	if (EcArrayPush( res, obj ) < 0)
		return Ec_ERROR;

	obj = EcMakeInt( (EcInt) ntohs( clientname->sin_port ) );
	if (EcArrayPush( res, obj ) < 0)
		return Ec_ERROR;

	return res;
}

/* Private */

static EC_OBJ sockaddr_copy( EC_OBJ obj, EcCopyType type )
{
	ASSERT( EC_SOCKADDRP(obj) );

	return EcMakeSockaddr( EC_SOCKADDR(obj), EC_SOCKADDRLEN(obj) );
}

static void sockaddr_mark( EC_OBJ obj )
{
	ASSERT( EC_SOCKADDRP(obj) );
}

static void sockaddr_free( EC_OBJ obj )
{
	ASSERT( EC_SOCKADDRP(obj) );

	if (EC_SOCKADDR(obj))
	{
		ec_free( EC_SOCKADDR(obj) );
		EC_SOCKADDR(obj)    = NULL;
		EC_SOCKADDRLEN(obj) = 0;
	}
}

static EcInt sockaddr_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	ASSERT( EC_SOCKADDRP(obj) );

	if (detailed)
	{
		struct sockaddr_in *clientname;

		clientname = (struct sockaddr_in *)EC_SOCKADDR(obj);
		return ec_sprintf( str, "<sockaddr %s:%d>",
						   inet_ntoa( clientname->sin_addr ),
						   (int) ntohs( clientname->sin_port ) );
	}
	else
		return ec_sprintf( str, "<sockaddr>" );
}

static EcUInt sockaddr_hash( EC_OBJ obj, EcInt recur )
{
	ASSERT( EC_SOCKADDRP(obj) );

	/* Equivalent object MUST have the same hash */
	return EC_SOCKADDRLEN(obj);									/* this is all we can do ... */
}

/* ------------------------------------------------------------------------
 * Initialization & cleanup
 * ------------------------------------------------------------------------ */

EC_OBJ ec_inet_init( void )
{
	EcTypespec sockaddrspec = {
		/* type    */		0,
		/* name    */		"sockaddr",

		/* copy    */		sockaddr_copy,

		/* mark    */		sockaddr_mark,
		/* free    */		sockaddr_free,
		/* print   */		sockaddr_print,

		/* hash    */		sockaddr_hash,
		/* compare */		NULL,

		/* check   */		NULL,

		/* sequence */      NULL,
		/* numeric  */      NULL
	};
	EC_OBJ pkg;

	tc_sockaddr = EcAddType( &sockaddrspec );
	if (tc_sockaddr == 0)
		return FALSE;

	/* Register package */
	pkg = EcPackageIntroduce( "inet" );

	/* Register functions in package */
	EcAddPrimitive( "inet.open",   EcLibInet_Open );
	EcAddPrimitive( "inet.socket", EcLibInet_Socket );
	EcAddPrimitive( "inet.listen", EcLibInet_Listen );
	EcAddPrimitive( "inet.accept", EcLibInet_Accept );
	EcAddPrimitive( "inet.ntoa",   EcLibInet_NToA );

	nw_init();
	return pkg;
}

void ec_skeleton_cleanup( void )
{
	nw_cleanup();
}
