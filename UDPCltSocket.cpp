/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		UDPCLTSOCKET.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CUDPCltSocket class definition.
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

CUDPCltSocket::CUDPCltSocket(Mode eMode)
	: CUDPSocket(eMode)
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

CUDPCltSocket::~CUDPCltSocket()
{
}
