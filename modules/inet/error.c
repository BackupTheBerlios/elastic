/* ===========================================================================
 * error.c
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
 *   $Id: error.c,v 1.1 2002/05/30 13:23:09 panta Exp $
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

#if MINGW32
#define HAVE_SYS_ERRLIST 1
#endif

extern int               sys_nerr;
#if ! HAVE_SYS_ERRLIST
extern const char *const sys_errlist[];
#endif

#define MAXMSG 1024

static char              errmsg[MAXMSG];
static nw_error_severity sev = NW_NONE;

static void nw_perror( void );

#if WIN32
static const char *find_werror( int w_errno );
#endif

/* Error handling */

void nw_set_error( nw_error_severity sev, const char *fmt, ... )
{
	int  ml;
	va_list args;

	va_start( args, fmt );

	switch (sev)
	{
	case NW_NONE:
		errmsg[0] = '\0';
		break;

	case NW_WARNING:
		ml = sprintf( errmsg, "WARNING: " );
		ml += vsprintf( errmsg + ml, fmt, args );
		break;

	case NW_ERROR:
		ml = sprintf( errmsg, "ERROR: " );
		ml += vsprintf( errmsg + ml, fmt, args );
		nw_perror();
		break;

	case NW_FATAL:
		ml = sprintf( errmsg, "FATAL: " );
		ml += vsprintf( errmsg + ml, fmt, args );
		nw_perror();
		va_end( args );
		exit( EXIT_FAILURE );
		break;

	case NW_ABNORMAL:
		ml = sprintf( errmsg, "ABNORMAL: " );
		ml += vsprintf( errmsg + ml, fmt, args );
		nw_perror();
		va_end( args );
		abort();										/* dump core */
		exit( EXIT_FAILURE );
		break;
	}

	va_end( args );
}

nw_error_severity nw_check_error( void )
{
	return sev;
}

const char *nw_get_error( void )
{
	return errmsg;
}

static void nw_perror( void )
{
	char msg[MAXMSG];
#if WIN32
	int w_errno;
#endif

#if WIN32
	w_errno = WSAGetLastError();
	if ((w_errno > WSABASEERR) &&
		(w_errno <= WSANO_ADDRESS))
		sprintf( msg, " SYSTEM ERROR: %s\n", find_werror( w_errno ) );
#else
	if (errno)
	{
		if ((errno > 0) && (errno < sys_nerr))
			sprintf( msg,
					 " SYSTEM ERROR: %s  (errno = %d)\n",
					 sys_errlist[errno], errno );
		else
			sprintf( msg,
					 " SYSTEM ERROR: (errno = %d)\n",
					 errno );
	}
#endif
	strncat( errmsg, msg, MAXMSG );
}

#if WIN32
const char *find_werror( int w_errno )
{
	switch (w_errno)
	{
	case WSAEINTR:
		return "Interrupted function call";
	case WSAEBADF:
		return "Bad file descriptor";
	case WSAEACCES:
		return "Permission denied";
	case WSAEFAULT:
		return "Bad address";
	case WSAEINVAL:
		return "Invalid argument";
	case WSAEMFILE:
		return "Too many open files";
	case WSAEWOULDBLOCK:
		return "Operation would block";
	case WSAEINPROGRESS:
		return "Operation in progress";
	case WSAEALREADY:
		return "Operation already in progress";
	case WSAENOTSOCK:
		return "Socket operation on non-socket";
	case WSAEDESTADDRREQ:
		return "Destination address required";
	case WSAEMSGSIZE:
		return "Message too long";
	case WSAEPROTOTYPE:
		return "Protocol wrong type for socket";
	case WSAENOPROTOOPT:
		return "Protocol not available";
	case WSAEPROTONOSUPPORT:
		return "Protocol not supported";
	case WSAESOCKTNOSUPPORT:
		return "Socket type not supported";
	case WSAEOPNOTSUPP:
		return "Operation not supported on transport endpoint";
	case WSAEPFNOSUPPORT:
		return "Protocol family not supported";
	case WSAEAFNOSUPPORT:
		return "Address family not supported by protocol";
	case WSAEADDRINUSE:
		return "Address already in use";
	case WSAEADDRNOTAVAIL:
        return "Cannot assign requested address";
	case WSAENETDOWN:
		return "Network is down";
	case WSAENETUNREACH:
		return "Network is unreachable";
	case WSAENETRESET:
		return "Network dropped connection because of reset";
	case WSAECONNABORTED:
		return "Software caused connection abort";
	case WSAECONNRESET:
		return "Connection reset by peer";
	case WSAENOBUFS:
		return "No buffer space available";
	case WSAEISCONN:
		return "Transport endpoint is already connected";
	case WSAENOTCONN:
		return "Transport endpoint is not connected";
	case WSAESHUTDOWN:
		return "Cannot send after transport endpoint shutdown";
	case WSAETOOMANYREFS:
		return "Too many references: cannot splice";
	case WSAETIMEDOUT:
		return "Connection timed out";
	case WSAECONNREFUSED:
		return "Connection refused";
	case WSAELOOP:
		return "Too many symbolic links encountered";
	case WSAENAMETOOLONG:
		return "File name too long";
	case WSAEHOSTDOWN:
		return "Host is down";
	case WSAEHOSTUNREACH:
		return "No route to host";
	case WSASYSNOTREADY:
		return "Network subsytem unusable";
	case WSAVERNOTSUPPORTED:
		return "Requested networking version not supported by Windows Sockets DLL";
	case WSANOTINITIALISED:
		return "Network library not yet initialized";
	case WSAHOST_NOT_FOUND:
		return "Authoritative Answer Host not found";
	case WSATRY_AGAIN:
		return "Non-Authoritative Host not found, or name server failure";
	case WSANO_RECOVERY:
		return "Non recoverable errors, FORMERR, REFUSED, NOTIMP";
	case WSANO_DATA:
		return "Valid name, no data record of requested type";
	default:
		return "Unrecognized error";
	}
}
#endif
