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
	if (getpeername(m_hSocket, (sockaddr*)&addr, &nAddrSize) == SOCKET_ERROR)
		throw CSocketException(CSocketException::E_QUERY_FAILED, CWinSock::LastError());

	return inet_ntoa(addr.sin_addr);
}
