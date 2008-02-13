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

	size_t Send(const void* pBuffer, size_t nBufSize);
	size_t Send(const CBuffer& oBuffer);

	size_t Recv(void* pBuffer, size_t nBufSize);
	size_t Recv(CBuffer& oBuffer);

	size_t Available();
	size_t Peek(void* pBuffer, size_t nBufSize);
	size_t Peek(CBuffer& oBuffer, size_t nBufSize);

	//
	// Class methods.
	//
	static bool    IsAddress(const tchar* pszHost);
	static in_addr Resolve(const tchar* pszHost);
	static CString ResolveStr(const tchar* pszHost);
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
	//! The collection of socket event handlers.
	typedef std::vector<IClientSocketListener*> CCltListeners;
	//! The buffer smart-pointer type.
	typedef Core::SharedPtr<CNetBuffer> NetBufferPtr;

	//
	// Members.
	//
	SOCKET			m_hSocket;			// Socket handle.
	Mode			m_eMode;			// 'Select' mode.
	CString			m_strHost;			// Host, If connected.
	uint			m_nPort;			// Port, If connected.
	CCltListeners	m_aoCltListeners;	// The list of event listeners.
	NetBufferPtr	m_pSendBuffer;		// Send buffer (async only).
	NetBufferPtr	m_pRecvBuffer;		// Receive buffer (async only).

	// Protect creation etc.
	CSocket(Mode eMode);
	CSocket(const CSocket&);
	void operator=(const CSocket&);

	//
	// Internal methods.
	//
	void Create(int nAF, int nType, int nProtocol);
	void Connect(const tchar* pszHost, uint nPort);

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

inline size_t CSocket::Send(const CBuffer& oBuffer)
{
	return Send(oBuffer.Buffer(), oBuffer.Size());
}

inline size_t CSocket::Recv(CBuffer& oBuffer)
{
	return Recv(oBuffer.Buffer(), oBuffer.Size());
}

inline size_t CSocket::Peek(CBuffer& oBuffer, size_t nBufSize)
{
	return Peek(oBuffer.Buffer(), nBufSize);
}

#endif // SOCKET_HPP
