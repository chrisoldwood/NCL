/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SOCKET.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CSocket class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SOCKET_HPP
#define SOCKET_HPP

/******************************************************************************
** 
** The base class for all sockets.
**
*******************************************************************************
*/

class CSocket
{
public:
	virtual void Close();
	virtual ~CSocket();

	//
	// Properties.
	//
	SOCKET	Handle() const;
	bool	IsOpen() const;

	virtual int Type()     const = 0;
	virtual int Protocol() const = 0;

	//
	// Methods.
	//
	int Send(const void* pBuffer, int nBufSize);
	int Send(const CBuffer& oBuffer);
	int Send(const char* pszString);

	int Recv(void* pBuffer, int nBufSize);
	int Recv(CBuffer& oBuffer);

	int Available();
	int Peek(void* pBuffer, uint nBufSize);

	//
	// Class methods.
	//

	static bool    IsAddress(const char* pszHost);
	static in_addr Resolve(const char* pszHost);
	static CString ResolveStr(const char* pszHost);

protected:
	//
	// Members.
	//
	SOCKET	m_hSocket;		// Socket handle.
	CString	m_strHost;		// Host, If connected.
	uint	m_nPort;		// Port, If connected.

	// Protect creation etc.
	CSocket();
	CSocket(const CSocket&);
	void operator=(const CSocket&);

	//
	// Internal methods.
	//
	void Create(int nAF, int nType, int nProtocol);
	void Connect(const char* pszHost, uint nPort);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline SOCKET CSocket::Handle() const
{
	return m_hSocket;
}

inline bool CSocket::IsOpen() const
{
	return (m_hSocket != INVALID_SOCKET);
}

inline int CSocket::Send(const CBuffer& oBuffer)
{
	return Send(oBuffer.Buffer(), oBuffer.Size());
}

inline int CSocket::Send(const char* pszString)
{
	return Send(pszString, strlen(pszString));
}

inline int CSocket::Recv(CBuffer& oBuffer)
{
	return Recv(oBuffer.Buffer(), oBuffer.Size());
}

#endif // SOCKET_HPP
