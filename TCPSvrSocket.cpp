/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TCPSVRSOCKET.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CTCPSvrSocket class definition.
**
*******************************************************************************
*/

#include "ncl.hpp"
#include <limits.h>

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

CTCPSvrSocket::CTCPSvrSocket()
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

CTCPSvrSocket::~CTCPSvrSocket()
{
}

/******************************************************************************
** Method:		Listen()
**
** Description:	Open the socket for listening on the given port.
**
** Parameters:	nPort		The port number.
**				nBackLog	The connection queue size.
**
** Returns:		Nothing.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

void CTCPSvrSocket::Listen(uint nPort, uint nBackLog)
{
	ASSERT(m_hSocket == INVALID_SOCKET);
	ASSERT(nPort     <= USHRT_MAX);

	sockaddr_in	addr = { 0 };

	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port        = htons((ushort)nPort);

	// Create the socket.
	Create(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Bind socket to port.
	if (bind(m_hSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		throw CSocketException(CSocketException::E_CONNECT_FAILED, CWinSock::LastError());

	// Start accepting client connections.
	if (listen(m_hSocket, nBackLog)  == SOCKET_ERROR)
		throw CSocketException(CSocketException::E_LISTEN_FAILED, CWinSock::LastError());
}

/******************************************************************************
** Method:		CanAccept()
**
** Description:	Queries if there is a client connection waiting.
**
** Parameters:	None.
**
** Returns:		true or false.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

bool CTCPSvrSocket::CanAccept() const
{
	fd_set  aoSockets;
	TIMEVAL oWaitTime = { 0 };

	FD_ZERO(&aoSockets);
	FD_SET(m_hSocket, &aoSockets);

	// Check for readability on the socket.
	if (select(1, &aoSockets, NULL, NULL, &oWaitTime) == SOCKET_ERROR)
		throw CSocketException(CSocketException::E_SELECT_FAILED, CWinSock::LastError());

	return FD_ISSET(m_hSocket, &aoSockets);
}

/******************************************************************************
** Method:		Accept()
**
** Description:	Accepts a client connection.
**
** Parameters:	None.
**
** Returns:		A socket to the client.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

CTCPCltSocket* CTCPSvrSocket::Accept()
{
	SOCKET       hSocket;
	sockaddr_in  addr      = { 0 };;
	int          nAddrSize = sizeof(addr);

	// Accept the next client connection.
	if ((hSocket = accept(m_hSocket, (sockaddr*)&addr, &nAddrSize)) == INVALID_SOCKET)
		throw CSocketException(CSocketException::E_ACCEPT_FAILED, CWinSock::LastError());
	
	return new CTCPCltSocket(hSocket);
}
