/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TCPSOCKET.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CTCPSocket class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP

/******************************************************************************
** 
** A TCP type socket.
**
*******************************************************************************
*/

class CTCPSocket : public CSocket
{
public:
	virtual ~CTCPSocket();

	//
	// Properties.
	//
	virtual int Type()     const;
	virtual int Protocol() const;

	CString PeerAddress() const;

protected:
	//
	// Members.
	//

	// Protect creation etc.
	CTCPSocket(Mode eMode);
	CTCPSocket(const CTCPSocket&);
	void operator=(const CTCPSocket&);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // TCPSOCKET_HPP
