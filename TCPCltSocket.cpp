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

CTCPCltSocket::CTCPCltSocket()
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

CTCPCltSocket::CTCPCltSocket(SOCKET hSocket)
{
	ASSERT(hSocket != INVALID_SOCKET);

	m_hSocket = hSocket;
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
