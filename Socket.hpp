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

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/Buffer.hpp>

// Forward declarations.
class IClientSocketListener;
class CNetBuffer;

/******************************************************************************
** 
** The base class for all sockets.
**
*******************************************************************************
*/

class CSocket
{
public:
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
	virtual void Close();

	uint Send(const void* pBuffer, uint nBufSize);
	uint Send(const CBuffer& oBuffer);
	uint Send(const char* pszString);

	uint Recv(void* pBuffer, uint nBufSize);
	uint Recv(CBuffer& oBuffer);

	uint Available();
	uint Peek(void* pBuffer, uint nBufSize);
	uint Peek(CBuffer& oBuffer, uint nBufSize);

	//
	// Class methods.
	//
	static bool    IsAddress(const char* pszHost);
	static in_addr Resolve(const char* pszHost);
	static CString ResolveStr(const char* pszHost);
	static CString AsyncEventStr(int nEvent);

	// Socket modes.
	enum Mode
	{
		BLOCK,		// BSD style blocking mode.
		ASYNC,		// Windows style message mode.
	};

	//
	// Event listener methods.
	//
	void AddClientListener(IClientSocketListener* pListener);
	void RemoveClientListener(IClientSocketListener* pListener);

protected:
	// Template shorthands.
	typedef std::vector<IClientSocketListener*> CCltListeners;

	//
	// Members.
	//
	SOCKET			m_hSocket;			// Socket handle.
	Mode			m_eMode;			// 'Select' mode.
	CString			m_strHost;			// Host, If connected.
	uint			m_nPort;			// Port, If connected.
	CCltListeners	m_aoCltListeners;	// The list of event listeners.
	CNetBuffer*		m_pSendBuffer;		// Send buffer (async only).
	CNetBuffer*		m_pRecvBuffer;		// Receive buffer (async only).

	// Protect creation etc.
	CSocket(Mode eMode);
	CSocket(const CSocket&);
	void operator=(const CSocket&);

	//
	// Internal methods.
	//
	void Create(int nAF, int nType, int nProtocol);
	void Connect(const char* pszHost, uint nPort);

	//
	// Async event methods.
	//
	virtual void OnAsyncSelect(int nEvent, int nError);
	virtual void OnReadReady();
	virtual void OnWriteReady();
	virtual void OnClosed(int nReason);
	virtual void OnError(int nEvent, int nError);

	// Friends.
	friend class CWinSock;
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

inline uint CSocket::Send(const CBuffer& oBuffer)
{
	return Send(oBuffer.Buffer(), oBuffer.Size());
}

inline uint CSocket::Send(const char* pszString)
{
	return Send(pszString, strlen(pszString));
}

inline uint CSocket::Recv(CBuffer& oBuffer)
{
	return Recv(oBuffer.Buffer(), oBuffer.Size());
}

inline uint CSocket::Peek(CBuffer& oBuffer, uint nBufSize)
{
	return Peek(oBuffer.Buffer(), nBufSize);
}

#endif // SOCKET_HPP
