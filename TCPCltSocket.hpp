/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TCPCLTSOCKET.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CTCPCltSocket class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef TCPCLTSOCKET_HPP
#define TCPCLTSOCKET_HPP

/******************************************************************************
** 
** A client side TCP socket.
**
*******************************************************************************
*/

class CTCPCltSocket : public CTCPSocket
{
public:
	//
	// Constructors/Destructor.
	//
	CTCPCltSocket(Mode eMode = BLOCK);
	virtual ~CTCPCltSocket();

	//
	// Properties.
	//
	CString	Host() const;
	uint    Port() const;
	
	//
	// Methods.
	//
	void Connect(const char* pszHost, uint nPort);

protected:
	//
	// Members.
	//

	// For use by CTCPSvrSocket.
	CTCPCltSocket(SOCKET hSocket, Mode eMode);

	// Friends.
	friend class CTCPSvrSocket;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CString CTCPCltSocket::Host() const
{
	return m_strHost;
}

inline uint CTCPCltSocket::Port() const
{
	return m_nPort;
}

inline void CTCPCltSocket::Connect(const char* pszHost, uint nPort)
{
	CSocket::Connect(pszHost, nPort);
}

#endif // TCPCLTSOCKET_HPP
