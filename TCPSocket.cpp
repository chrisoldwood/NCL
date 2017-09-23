/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TCPSOCKET.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CTCPSocket class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "TCPSocket.hpp"
#include "SocketException.hpp"
#include "WinSock.hpp"
#include <Core/AnsiWide.hpp>

#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 2)) // GCC 4.2+
// missing initializer for member 'X'
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CTCPSocket::CTCPSocket(Mode eMode)
	: CSocket(eMode)
{
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CTCPSocket::~CTCPSocket()
{
}

/******************************************************************************
** Methods:		Type() & Protocol()
**
** Description:	Template methods to get the socket type and protocol.
**
** Parameters:	None.
**
** Returns:		SOCK_* & IPPROTO_*.
**
*******************************************************************************
*/

int CTCPSocket::Type() const
{
	return SOCK_STREAM;
}

int CTCPSocket::Protocol() const
{
	return IPPROTO_TCP;
}

/******************************************************************************
** Method:		PeerAddress()
**
** Description:	Gets the IP address of the peer.
**
** Parameters:	None.
**
** Returns:		The address in numeric form.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

CString CTCPSocket::PeerAddress() const
{
	ASSERT(m_hSocket != INVALID_SOCKET);

	sockaddr_in  addr      = { 0 };;
	int          nAddrSize = sizeof(addr);

	// Get the peer host address and port number.
	if (getpeername(m_hSocket, reinterpret_cast<sockaddr*>(&addr), &nAddrSize) == SOCKET_ERROR)
		throw CSocketException(CSocketException::E_QUERY_FAILED, CWinSock::LastError());

	return A2T(inet_ntoa(addr.sin_addr));
}
