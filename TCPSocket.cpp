/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TCPSOCKET.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CTCPSocket class definition.
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

CTCPSocket::CTCPSocket()
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
** Method:		Connect()
**
** Description:	Open a connection.
**
** Parameters:	pszHost		The host name.
**				nPort		The port number.
**
** Returns:		Nothing.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

void CTCPSocket::Connect(const char* pszHost, uint nPort)
{
	ASSERT(m_hSocket == INVALID_SOCKET);
	ASSERT(pszHost   != NULL);
	ASSERT(nPort     <= USHRT_MAX);

	sockaddr_in	addr = { 0 };

	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = inet_addr(pszHost);
	addr.sin_port        = htons((ushort)nPort);

	// Host name isn't an IP Address?
	if (addr.sin_addr.s_addr == INADDR_NONE)
	{
		// Resolve host name.
		hostent* pHost = gethostbyname(pszHost);

		if (pHost == NULL)
			throw CSocketException(CSocketException::E_RESOLVE_FAILED, CWinSock::LastError());

		memcpy(&addr.sin_addr, pHost->h_addr_list[0], pHost->h_length);
	}

	// Create the socket.
	Create(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Connect to host.
	if (connect(m_hSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		throw CSocketException(CSocketException::E_CONNECT_FAILED, CWinSock::LastError());
}

/******************************************************************************
** Method:		Send()
**
** Description:	Send the data.
**
** Parameters:	pBuffer		The buffer to send.
**				nBufSize	The buffer size.
**
** Returns:		The number of bytes sent.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

int CTCPSocket::Send(const void* pBuffer, int nBufSize)
{
	ASSERT(m_hSocket != INVALID_SOCKET);

	int nResult = send(m_hSocket, (const char*)pBuffer, nBufSize, 0);

	if (nResult == SOCKET_ERROR)
		throw CSocketException(CSocketException::E_SEND_FAILED, CWinSock::LastError());

	ASSERT(nResult == nBufSize);

	return nResult;
}

/******************************************************************************
** Method:		Recv()
**
** Description:	Read the incoming data.
**
** Parameters:	pBuffer		The buffer to write to.
**				nBufSize	The buffer size.
**
** Returns:		The number of bytes read.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

int CTCPSocket::Recv(void* pBuffer, int nBufSize)
{
	ASSERT(m_hSocket != INVALID_SOCKET);

	int nResult = recv(m_hSocket, (char*)pBuffer, nBufSize, 0);

	if (nResult == SOCKET_ERROR)
		throw CSocketException(CSocketException::E_RECV_FAILED, CWinSock::LastError());

	return nResult;
}

/******************************************************************************
** Method:		Peek()
**
** Description:	Peek at the incoming data.
**
** Parameters:	pBuffer		The buffer to write to.
**				nBufSize	The buffer size.
**
** Returns:		The number of bytes left to read.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

int CTCPSocket::Peek(void* pBuffer, uint nBufSize)
{
	ASSERT(m_hSocket != INVALID_SOCKET);

	int nResult = recv(m_hSocket, (char*)pBuffer, nBufSize, MSG_PEEK);

	if (nResult == SOCKET_ERROR)
		throw CSocketException(CSocketException::E_PEEK_FAILED, CWinSock::LastError());

	return nResult;
}
