/* ===========================================================================
 * socket.c
 * ===========================================================================
 * Unix Network Utility Library.
 * ---------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Thu Mar 25 18:34:16 MET 1999
 *
 *   Note: some functions are adapted from:
 *      W. Richard Stevens, "Unix Network Programming", Prentice Hall
 *
 *   $Id: socket.c,v 1.1 2002/05/30 13:23:17 panta Exp $
 * ---------------------------------------------------------------------------
 *    Copyright (C) 1999 Marco Pantaleoni. All rights reserved.
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
 * ===========================================================================
 */

#include "nwlib.h"

#ifndef	INADDR_NONE
#define	INADDR_NONE	0xffffffff			/* should be in <netinet/in.h> */
#endif

/* Socket functions */

nw_socket nw_make_socket( unsigned short int port )
{
	nw_socket sock;
	struct sockaddr_in name;
     
	/* Create the socket. */
	sock = socket( PF_INET, SOCK_STREAM, 0 );
	if (sock < 0)
	{
		nw_set_error( NW_ERROR, "in 'socket' call" );
		return NW_INVALID_SOCKET;
	}
     
	/* Give the socket a name. */
	memset( &name, 0x00, sizeof(struct sockaddr_in) );
	name.sin_family      = AF_INET;
	name.sin_port        = htons( port );
	name.sin_addr.s_addr = htonl( INADDR_ANY );
	if (bind( sock,
			  (struct sockaddr *) &name,
			  sizeof(struct sockaddr_in) ) < 0)
	{
		nw_set_error( NW_ERROR, "in 'bind' call" );
		return NW_INVALID_SOCKET;
	}
     
	return sock;
}

int nw_init_sockaddr( struct sockaddr_in *name,
					   const char *hostname,
					   unsigned short int port )
{
	struct hostent *hostinfo;
     
	name->sin_family = AF_INET;
	name->sin_port = htons( port );
	hostinfo = gethostbyname( hostname );
	if (hostinfo == NULL)
	{
		nw_set_error( NW_ERROR, "unknown host %s.", hostname );
		return FALSE;
	}
	name->sin_addr = *(struct in_addr *) hostinfo->h_addr;

	return TRUE;
}


/*
 * The following globals are available to the caller, if desired.
 */

struct sockaddr_in	nw_tcp_srv_addr;	/* server's Internet socket addr */
struct servent		nw_tcp_serv_info;	/* from getservbyname() */
struct hostent		nw_tcp_host_info;	/* from gethostbyname() */

#ifndef NETDB_INTERNAL
#define NETDB_INTERNAL (-1)
#endif

#ifndef NETDB_SUCCESS
#define NETDB_SUCCESS (0)
#endif

static const char *host_err_str( void )
{
	switch (h_errno)
	{
	case NETDB_INTERNAL:
		return strerror( errno );
		break;

	case NETDB_SUCCESS:
		return "OK";
		break;

	case HOST_NOT_FOUND:
		return "Authoritative answer Host not found";
		break;

	case TRY_AGAIN:
		return "Non-Authoritative Host not found, or SERVERFAIL";
		break;

	case NO_RECOVERY:
		return "Non recoverable errors, FORMERR, REFUSED, NOTIMP";
		break;

	case NO_DATA:
		return "Valid name, no data record of requested type";
		break;

	default:
		return "UNKNOWN";
		break;
	}
}

/*
 * nw_tcpopen  - Open a TCP connection with another host
 *
 * SYNOPSIS
 *
 *  int nw_tcpopen( const char *host, const char *service, int port )
 *
 *   host     - name or dotted-decimal addr of other host
 *   service  - name of service being requested (can be NULL, iff port > 0)
 *   port     - if == 0, nothing special: use port# of service
 *              if < 0,  bind a local reserved port
 *              if > 0,  it's the port# of server (host-byte-order)
 *
 * RETURN VALUE
 *  return socket descriptor if OK, else -1 on error
 *
 * NOTES
 *  this function is a slightly modified version of the one
 *  presented in:
 *      W. Richard Stevens, "Unix Network Programming", Prentice Hall
 *  
 */
nw_socket nw_tcpopen( const char *host, const char *service, int port )
{
	nw_socket       fd;
	int		        resvport;
	unsigned long	inaddr;
	struct servent *sp;
	struct hostent *hp;

	/*
	 * Initialize the server's Internet address structure.
	 * We'll store the actual 4-byte Internet address and the
	 * 2-byte port# below.
	 */

	memset( &nw_tcp_srv_addr, 0x00, sizeof(nw_tcp_srv_addr) );
	nw_tcp_srv_addr.sin_family = AF_INET;

	if (service != NULL)
	{
		if ((sp = getservbyname(service, "tcp")) == NULL)
		{
			nw_set_error( NW_ERROR,
						  "nw_tcpopen: unknown service: %s/tcp",
						  service );
			return NW_INVALID_SOCKET;
		}
		nw_tcp_serv_info = *sp;			/* structure copy */
		if (port > 0)
			nw_tcp_srv_addr.sin_port = htons( port );	/* caller's value */
		else
			nw_tcp_srv_addr.sin_port = sp->s_port;		/* service's value */
	} else
	{
		if (port <= 0)
		{
			nw_set_error( NW_ERROR,
						  "nw_tcpopen: must specify either service or port" );
			return NW_INVALID_SOCKET;
		}
		nw_tcp_srv_addr.sin_port = htons( port );
	}

	/*
	 * First try to convert the host name as a dotted-decimal number.
	 * Only if that fails do we call gethostbyname().
	 */

	if ((inaddr = inet_addr( host )) != INADDR_NONE)
	{
		/* it's dotted-decimal */
		memcpy( &nw_tcp_srv_addr.sin_addr, &inaddr,
				sizeof(inaddr) );
		nw_tcp_host_info.h_name = NULL;
	} else
	{
		if ((hp = gethostbyname( host )) == NULL)
		{
			nw_set_error( NW_ERROR, "nw_tcpopen: host name error: %s %s",
						  host, host_err_str() );
			return NW_INVALID_SOCKET;
		}
		nw_tcp_host_info = *hp;			/* found it by name, structure copy */
		memcpy( &nw_tcp_srv_addr.sin_addr, hp->h_addr,
				hp->h_length );
	}

	if (port >= 0)
	{
		if ((fd = socket( AF_INET, SOCK_STREAM, 0 )) < 0)
		{
			nw_set_error( NW_ERROR, "nw_tcpopen: can't create TCP socket" );
			return NW_INVALID_SOCKET;
		}
	} else if (port < 0)
	{
		resvport = IPPORT_RESERVED - 1;
#if WIN32
		/* What we have to do here ??? */
		return NW_INVALID_SOCKET;
#else
		if ((fd = rresvport( &resvport )) < 0)
		{
			nw_set_error( NW_ERROR, "nw_tcpopen: can't get a reserved TCP port" );
			return NW_INVALID_SOCKET;
		}
#endif
	}

	/*
	 * Connect to the server.
	 */

	if (connect( fd, (struct sockaddr *) &nw_tcp_srv_addr,
				 sizeof(nw_tcp_srv_addr) ) < 0)
	{
		nw_set_error( NW_ERROR, "nw_tcpopen: can't connect to server" );
		socket_close( fd );
		return NW_INVALID_SOCKET;
	}

	return fd;	/* all OK */
}
