/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		UDPSVRSOCKET.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CUDPSvrSocket class definition.
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

CUDPSvrSocket::CUDPSvrSocket()
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

CUDPSvrSocket::~CUDPSvrSocket()
{
}

/******************************************************************************
** Method:		Listen()
**
** Description:	Open the socket for listening on the given port.
**
** Parameters:	nPort		The port number.
**
** Returns:		Nothing.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

void CUDPSvrSocket::Listen(uint nPort)
{
	ASSERT(m_hSocket == INVALID_SOCKET);
	ASSERT(nPort     <= USHRT_MAX);

	// Save parameters.
	m_nPort = nPort;

	sockaddr_in	addr = { 0 };

	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port        = htons((ushort)nPort);

	// Create the socket.
	Create(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// Bind socket to port.
	if (bind(m_hSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		throw CSocketException(CSocketException::E_BIND_FAILED, CWinSock::LastError());
}
