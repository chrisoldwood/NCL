/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TCPCLTSOCKET.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CTCPCltSocket class definition.
**
*******************************************************************************
*/

#include "ncl.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
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

CTCPCltSocket::CTCPCltSocket(Mode eMode)
	: CTCPSocket(eMode)
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

CTCPCltSocket::CTCPCltSocket(SOCKET hSocket, Mode eMode)
	: CTCPSocket(eMode)
{
	ASSERT(hSocket != INVALID_SOCKET);

	m_hSocket = hSocket;

	sockaddr_in  addr      = { 0 };;
	int          nAddrSize = sizeof(addr);

	// Get the peer host address and port number.
	if (getpeername(m_hSocket, (sockaddr*)&addr, &nAddrSize) != SOCKET_ERROR)
	{
		m_strHost = inet_ntoa(addr.sin_addr);
		m_nPort   = addr.sin_port;
	}

	// If async mode, do select.
	if (m_eMode == ASYNC)
		CWinSock::BeginAsyncSelect(this, (FD_READ | FD_WRITE | FD_CLOSE));
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
