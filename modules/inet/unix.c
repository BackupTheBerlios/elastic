/* ===========================================================================
 * unix.c
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
 *   $Id: unix.c,v 1.1 2002/05/30 13:23:17 panta Exp $
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

#ifdef WIN32
size_t socket_read( nw_socket fd, void *buf, size_t count );
size_t socket_write( nw_socket fd, void *buf, size_t count );
#else
#define socket_read  read
#define socket_write write
#endif

/* Unix utility functions */

size_t nw_readn( nw_socket fd, void *buf, size_t count )
{
	size_t nleft, nread;

	nleft = count;
	while (nleft > 0)
	{
		nread = socket_read( fd, buf, nleft );

		if (nread < 0)
			return nread;								/* error, return < 0 */
		else if (nread == 0)
			break;										/* EOF */

		nleft -= nread;
		buf   += nread;
	}

	return (count - nleft);								/* return >= 0 */
}

size_t nw_writen( nw_socket fd, const void *buf, size_t count )
{
	size_t nleft, nwritten;

	nleft = count;
	while (nleft > 0)
	{
		nwritten = socket_write( fd, (void *) buf, nleft );
		if (nwritten <= 0)
			return nwritten;							/* error */

		nleft -= nwritten;
		buf   += nwritten;
	}

	return (count - nleft);
}

size_t nw_readline( nw_socket fd, void *buf, size_t maxlen )
{
	size_t n, rc;
	char   ch;

	for (n = 1; n < maxlen; n++)
	{
		rc = socket_read( fd, &ch, 1 );
		if (rc == 1)
		{
			*((char *) buf)++ = ch;
			if (ch == '\n')
				break;
		}
		else if (rc == 0)
		{
			if (n == 1)
				return 0;							/* EOF, no data read   */
			else
				break;								/* EOF, some data read */
		}
		else
			return -1;								/* error               */
	}

	*((char *) buf) = '\0';
	return n;
}

size_t nw_writeline( nw_socket fd, const char *buf )
{
	const char *e;
	size_t      count;
	size_t      res = 0;

	e = buf;
	while (e && (*e != '\0') && (*e != '\n'))
		e++;
	count = e - buf;
	if (count)
	{
		res = nw_writen( fd, buf, count );
		if (res <= 0) return 0;
	}
	if (nw_writen( fd, "\n", 1 ) <= 0)
		return 0;

	return (res + 1);
}

size_t nw_readbyte( nw_socket fd,  nw_byte  *d )
{
	size_t rc;

	rc = socket_read( fd, d, 1 );
	if (rc == 1)
		return 1;
	return rc;
}

size_t nw_readword( nw_socket fd,  nw_word  *d )
{
	size_t  rc;

	rc = socket_read( fd, d, sizeof(nw_word) );
	if (rc == sizeof(nw_word))
	{
		*d = (nw_word) ntohs( (unsigned short int) *d );
		return 1;
	}
	else if (rc == 0)
		return 0;
	return -1;
}

size_t nw_readdword( nw_socket fd, nw_dword *d )
{
	size_t   rc;

	rc = socket_read( fd, d, sizeof(nw_dword) );
	if (rc == sizeof(nw_dword))
	{
		*d = (nw_dword) ntohl( (unsigned long int) *d );
		return 1;
	}
	else if (rc == 0)
		return 0;
	return -1;
}

size_t nw_writebyte( nw_socket fd,  nw_byte  d )
{
	size_t wc;

	wc = socket_write( fd, &d, sizeof(nw_byte) );

	if (wc != sizeof(nw_byte))
		return -1;
	return 1;
}

size_t nw_writeword( nw_socket fd,  nw_word  d )
{
	size_t wc;
	nw_word data;

	data = (nw_word) htons( (unsigned short int) d );
	wc = socket_write( fd, &data, sizeof(nw_word) );

	if (wc != sizeof(nw_word))
		return -1;
	return 1;
}

size_t nw_writedword( nw_socket fd, nw_dword d )
{
	size_t wc;
	nw_dword data;

	data = (nw_dword) htonl( (unsigned long int) d );

	wc = socket_write( fd, &data, sizeof(nw_dword) );

	if (wc != sizeof(nw_dword))
		return -1;
	return 1;
}


/* Uncompressed (raw) data streams */

#ifndef EWOULDBLOCK
#define EWOULDBLOCK EAGAIN
#endif

int nw_recvn( nw_socket s, void *buf, int len, unsigned int flags )
{
	size_t nleft, nread;

	nleft = len;
	while (nleft > 0)
	{
		errno = 0;
		nread = recv( s, buf, nleft, flags );

		/* if this is a non-blocking recv, act as it were blocking */
		if ((nread == -1) && (errno == EWOULDBLOCK))
		{
			/* wait for more data */
			fd_set rfs;

			FD_ZERO(&rfs);
			FD_SET(s, &rfs);
#ifdef WIN32
			select( 1, &rfs, NULL, NULL, NULL );				/* first argument ignored */
#else
			select( s+1, &rfs, NULL, NULL, NULL );
#endif

			continue;
		}

		if (nread < 0)
			return nread;								/* error, return < 0 */
		else if (nread == 0)
			break;										/* EOF */

		nleft -= nread;
		buf   += nread;
	}

	return (len - nleft);								/* return >= 0 */
}

int nw_sendn( nw_socket s, const void *buf, int len, unsigned int flags )
{
	size_t nleft, nsent;

	nleft = len;
	while (nleft > 0)
	{
		nsent = send( s, buf, nleft, flags );

		/* if this is a non-blocking send, act as it were blocking */
		if ((nsent == -1) && (errno == EWOULDBLOCK))
		{
			/* wait for more data */
			fd_set wfs;

			FD_ZERO(&wfs);
			FD_SET(s, &wfs);
#ifdef WIN32
			select( 1, NULL, &wfs, NULL, NULL );				/* first argument ignored */
#else
			select( s+1, NULL, &wfs, NULL, NULL );
#endif

			continue;
		}

		if (nsent <= 0)
			return nsent;								/* error */

		nleft -= nsent;
		buf   += nsent;
	}

	return (len - nleft);
}

#ifdef WIN32
size_t socket_read( nw_socket fd, void *buf, size_t count )
{
	int res;
	res = recv( fd, buf, (int) count, 0 );
	if (res == SOCKET_ERROR)
		return -1;
	return res;
}

size_t socket_write( nw_socket fd, void *buf, size_t count )
{
	int res;
	res = send( fd, buf, (int) count, 0 );
	if (res == SOCKET_ERROR)
		return -1;
	return res;
}
#endif
