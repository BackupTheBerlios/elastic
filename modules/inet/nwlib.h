/* ===========================================================================
 * nwlib.h
 * ===========================================================================
 * Unix Network Utility Library.
 * ---------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Thu Mar 25 18:34:16 MET 1999
 *
 *   $Id: nwlib.h,v 1.1 2002/05/30 13:23:13 panta Exp $
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

#ifndef __NWLIB_H
#define __NWLIB_H 1

#include <elastic/basic.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifdef WIN32
/* WIN32 header files */
#include <winsock.h>
#else
/* Unix header files */
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

typedef enum { NW_NONE, NW_WARNING, NW_ERROR, NW_FATAL, NW_ABNORMAL } nw_error_severity;

typedef EcByte  nw_byte;
typedef EcWord  nw_word;
typedef EcDWord nw_dword;

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

#ifdef WIN32
typedef SOCKET nw_socket;
#define NW_INVALID_SOCKET	(INVALID_SOCKET)
typedef unsigned int socklen_t;
#else
typedef int nw_socket;
#define NW_INVALID_SOCKET	((nw_socket) -1)
#endif

#ifdef WIN32
#define socket_close closesocket
#else
#define socket_close close
#endif

/*
 * The following globals are available to the caller of nw_tcpopen, if desired.
 *
 * (Warning: this makes this library *not* MT-safe)
 */

extern struct sockaddr_in	nw_tcp_srv_addr;	/* server's Internet address */
extern struct servent		nw_tcp_serv_info;	/* from getservbyname()      */
extern struct hostent		nw_tcp_host_info;	/* from gethostbyname()      */


/* ========================================================================
 * ANSI C Prototypes
 * ======================================================================== */

/* Initialization & cleanup */

void nw_init( void );
void nw_cleanup( void );

/* Error handling */

void               nw_set_error( nw_error_severity sev, const char *fmt, ... );
nw_error_severity  nw_check_error( void );
const char        *nw_get_error( void );

/* Socket functions */

nw_socket nw_make_socket( unsigned short int port );
int nw_init_sockaddr( struct sockaddr_in *name,
					   const char *hostname,
					   unsigned short int port );
nw_socket nw_tcpopen( const char *host, const char *service, int port );

/* Unix utility functions */

size_t nw_readn( nw_socket fd, void *buf, size_t count );
size_t nw_writen( nw_socket fd, const void *buf, size_t count );
size_t nw_readline( nw_socket fd, void *buf, size_t maxlen );
size_t nw_writeline( nw_socket fd, const char *buf );

int    nw_recvn( nw_socket s, void *buf, int len, unsigned int flags );
int    nw_sendn( nw_socket s, const void *buf, int len, unsigned int flags );

size_t nw_readbyte( nw_socket fd,  nw_byte  *d );
size_t nw_readword( nw_socket fd,  nw_word  *d );
size_t nw_readdword( nw_socket fd, nw_dword *d );
size_t nw_writebyte( nw_socket fd,  nw_byte  d );
size_t nw_writeword( nw_socket fd,  nw_word  d );
size_t nw_writedword( nw_socket fd, nw_dword d );

#endif /* __NWLIB_H */
