/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		ISERVERSOCKETLISTENER.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The IServerSocketListener interface declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef ISERVERSOCKETLISTENER_HPP
#define ISERVERSOCKETLISTENER_HPP

// Forward declarations
class CSocket;
class CTCPSvrSocket;

/******************************************************************************
** 
** The callback interface for server socket events.
**
*******************************************************************************
*/

class IServerSocketListener
{
public:
	//
	// Methods.
	//
	virtual void OnAcceptReady(CTCPSvrSocket* pSocket) = 0;
	virtual void OnClosed(CSocket* pSocket, int nReason) = 0;
//	virtual void OnError(CSocket* pSocket, int nEvent, int nError) = 0;
	
protected:
	// Make interface.
	virtual ~IServerSocketListener() = 0 {}; 
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // ISERVERSOCKETLISTENER_HPP
