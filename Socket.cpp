/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SOCKET.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CSocket class definition.
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

CSocket::CSocket()
	: m_hSocket(INVALID_SOCKET)
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

CSocket::~CSocket()
{
	Close();
}

/******************************************************************************
** Method:		Close()
**
** Description:	Close the socket.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSocket::Close()
{
	// Socket still open?
	if (m_hSocket != INVALID_SOCKET)
		closesocket(m_hSocket);

	// Reset members.
	m_hSocket = INVALID_SOCKET;
}

/******************************************************************************
** Method:		Create()
**
** Description:	Create the socket.
**
** Parameters:	See socket().
**
** Returns:		Nothing.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

void CSocket::Create(int nAF, int nType, int nProtocol)
{
	ASSERT(m_hSocket == INVALID_SOCKET);

	m_hSocket = socket(nAF, nType, nProtocol);

	// Failed?
	if (m_hSocket == INVALID_SOCKET)
		throw CSocketException(CSocketException::E_CREATE_FAILED, CWinSock::LastError());
}
