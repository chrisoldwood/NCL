/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		UDPSVRSOCKET.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CUDPSvrSocket class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef UDPSVRSOCKET_HPP
#define UDPSVRSOCKET_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "UDPSocket.hpp"

/******************************************************************************
** 
** A server side UDP socket.
**
*******************************************************************************
*/

class CUDPSvrSocket : public CUDPSocket
{
public:
	//
	// Constructors/Destructor.
	//
	CUDPSvrSocket(Mode eMode = BLOCK);
	virtual ~CUDPSvrSocket();
	
	//
	// Methods.
	//
	void Listen(uint nPort);

protected:
	//
	// Members.
	//
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // UDPSVRSOCKET_HPP
