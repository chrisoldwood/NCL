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
	CTCPSvrSocket();
	virtual ~CTCPSvrSocket();
	
	//
	// Methods.
	//
	void Listen(uint nPort, uint nBackLog = SOMAXCONN);

	bool CanAccept() const;
	CTCPCltSocket* Accept();

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

#endif // TCPSVRSOCKET_HPP
