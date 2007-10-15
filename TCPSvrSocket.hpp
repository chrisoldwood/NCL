/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TCPSVRSOCKET.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CTCPSvrSocket class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef TCPSVRSOCKET_HPP
#define TCPSVRSOCKET_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "TCPSocket.hpp"

// Forward declarations.
class CTCPCltSocket;
class IServerSocketListener;

/******************************************************************************
** 
** A server side TCP socket.
**
*******************************************************************************
*/

class CTCPSvrSocket : public CTCPSocket
{
public:
	//
	// Constructors/Destructor.
	//
	CTCPSvrSocket(Mode eMode = BLOCK);
	virtual ~CTCPSvrSocket();
	
	//
	// Properties.
	//
	uint Port() const;

	//
	// Methods.
	//
	void Listen(uint nPort, uint nBackLog = SOMAXCONN);

	bool CanAccept() const;
	CTCPCltSocket* Accept();
	void Accept(CTCPCltSocket* pCltSocket);

	//
	// Event listener methods.
	//
	void AddServerListener(IServerSocketListener* pListener);
	void RemoveServerListener(IServerSocketListener* pListener);

protected:
	// Template shorthands.
	typedef std::vector<IClientSocketListener*> CCltListeners;
	typedef std::vector<IServerSocketListener*> CSvrListeners;

	//
	// Members.
	//
	CSvrListeners	m_aoSvrListeners;	// The list of event listeners.

	//
	// Async event methods.
	//
	virtual void OnAsyncSelect(int nEvent, int nError);
	virtual void OnAcceptReady();
	virtual void OnClosed(int nReason);
	virtual void OnError(int nEvent, int nError);

	//
	// Template methods.
	//
	CTCPCltSocket* AllocCltSocket();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline uint CTCPSvrSocket::Port() const
{
	return m_nPort;
}

#endif // TCPSVRSOCKET_HPP
