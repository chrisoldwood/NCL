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
	// Properties.
	//
	virtual int Type()     const;
	virtual int Protocol() const;

	//
	// Methods.
	//
	int SendTo(const void* pBuffer, int nBufSize, const in_addr& oAddr, uint nPort);
	int SendTo(const CBuffer& oBuffer, const in_addr& oAddr, uint nPort);
	int SendTo(const char* pszString, const in_addr& oAddr, uint nPort);

	int RecvFrom(void* pBuffer, int nBufSize, in_addr& oAddr, uint& nPort);
	int RecvFrom(CBuffer& oBuffer, in_addr& oAddr, uint& nPort);

protected:
	//
	// Members.
	//

	// Protect creation etc.
	CUDPSocket(Mode eMode);
	CUDPSocket(const CUDPSocket&);
	void operator=(const CUDPSocket&);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline int CUDPSocket::SendTo(const CBuffer& oBuffer, const in_addr& oAddr, uint nPort)
{
	return SendTo(oBuffer.Buffer(), oBuffer.Size(), oAddr, nPort);
}

inline int CUDPSocket::SendTo(const char* pszString, const in_addr& oAddr, uint nPort)
{
	return SendTo(pszString, strlen(pszString), oAddr, nPort);
}

inline int CUDPSocket::RecvFrom(CBuffer& oBuffer, in_addr& oAddr, uint& nPort)
{
	return RecvFrom(oBuffer.Buffer(), oBuffer.Size(), oAddr, nPort);
}

#endif // UDPSOCKET_HPP
