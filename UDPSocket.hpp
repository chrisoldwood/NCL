/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		UDPSOCKET.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CUDPSocket class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef UDPSOCKET_HPP
#define UDPSOCKET_HPP

/******************************************************************************
** 
** A UDP type socket.
**
*******************************************************************************
*/

class CUDPSocket : public CSocket
{
public:
	virtual ~CUDPSocket();

	//
	// Methods.
	//
	int SendTo(const void* pBuffer, int nBufSize, const in_addr& oAddr, int nPort);
	int SendTo(const CBuffer& oBuffer, const in_addr& oAddr, int nPort);
	int SendTo(const char* pszString, const in_addr& oAddr, int nPort);

	int RecvFrom(void* pBuffer, int nBufSize, in_addr& oAddr, int& nPort);
	int RecvFrom(CBuffer& oBuffer, in_addr& oAddr, int& nPort);

protected:
	//
	// Members.
	//

	// Protect creation etc.
	CUDPSocket();
	CUDPSocket(const CUDPSocket&);
	void operator=(const CUDPSocket&);

	//
	// Template methods.
	//
	virtual int Type()     const;
	virtual int Protocol() const;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline int CUDPSocket::SendTo(const CBuffer& oBuffer, const in_addr& oAddr, int nPort)
{
	return SendTo(oBuffer.Buffer(), oBuffer.Size(), oAddr, nPort);
}

inline int CUDPSocket::SendTo(const char* pszString, const in_addr& oAddr, int nPort)
{
	return SendTo(pszString, strlen(pszString), oAddr, nPort);
}

inline int CUDPSocket::RecvFrom(CBuffer& oBuffer, in_addr& oAddr, int& nPort)
{
	return RecvFrom(oBuffer.Buffer(), oBuffer.Size(), oAddr, nPort);
}

#endif // UDPSOCKET_HPP
