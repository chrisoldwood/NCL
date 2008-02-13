/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TCPCLTSOCKET.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CTCPCltSocket class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "TCPCltSocket.hpp"
#include "WinSock.hpp"
#include <Core/AnsiWide.hpp>

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

CTCPCltSocket::CTCPCltSocket(Mode eMode)
	: CTCPSocket(eMode)
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

CTCPCltSocket::~CTCPCltSocket()
{
}

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

void CTCPCltSocket::Attach(SOCKET hSocket, Mode eMode)
{
	ASSERT(hSocket   != INVALID_SOCKET);
	ASSERT(m_hSocket == INVALID_SOCKET);
	ASSERT(m_eMode   == eMode);

	m_hSocket = hSocket;
	m_eMode   = eMode;

	sockaddr_in  addr      = { 0 };;
	int          nAddrSize = sizeof(addr);

	// Get the peer host address and port number.
	if (getpeername(m_hSocket, reinterpret_cast<sockaddr*>(&addr), &nAddrSize) != SOCKET_ERROR)
	{
		m_strHost = A2T(inet_ntoa(addr.sin_addr));
		m_nPort   = addr.sin_port;
	}

	// If async mode, do select.
	if (m_eMode == ASYNC)
		CWinSock::BeginAsyncSelect(this, (FD_READ | FD_WRITE | FD_CLOSE));
}
