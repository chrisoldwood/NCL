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
	//
	// Constructors/Destructor.
	//
	CTCPSocket();
	virtual ~CTCPSocket();
	
	//
	// Methods.
	//
	void Connect(const char* pszHost, uint nPort);

	int Send(const void* pBuffer, int nBufSize);
	int Send(const CBuffer& oBuffer);
	int Send(const char* pszString);

	int Recv(void* pBuffer, int nBufSize);
	int Recv(CBuffer& oBuffer);

	int Available();
	int Peek(void* pBuffer, uint nBufSize);

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

inline int CTCPSocket::Send(const CBuffer& oBuffer)
{
	return Send(oBuffer.Buffer(), oBuffer.Size());
}

inline int CTCPSocket::Send(const char* pszString)
{
	return Send(pszString, strlen(pszString));
}

inline int CTCPSocket::Recv(CBuffer& oBuffer)
{
	return Recv(oBuffer.Buffer(), oBuffer.Size());
}

inline int CTCPSocket::Available()
{
	ASSERT(m_hSocket != INVALID_SOCKET);

	ulong lAvailable = 0;

	int nResult = ::ioctlsocket(m_hSocket, FIONREAD, &lAvailable);

	if (nResult == SOCKET_ERROR)
		throw CSocketException(CSocketException::E_PEEK_FAILED, CWinSock::LastError());

	return lAvailable;
}

#endif // TCPSOCKET_HPP
