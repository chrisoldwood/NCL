/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SOCKET.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CSocket class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "Socket.hpp"
#include "NetBuffer.hpp"
#include "WinSock.hpp"
#include "SocketException.hpp"
#include "IClientSocketListener.hpp"
#include <limits.h>
#include <algorithm>

// Conditional expression is constant.
// Caused by FD_SET().
#pragma warning ( disable : 4127 )

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CSocket::CSocket(Mode eMode)
	: m_hSocket(INVALID_SOCKET)
	, m_eMode(eMode)
	, m_strHost("")
	, m_nPort(0)
	, m_pSendBuffer(NULL)
	, m_pRecvBuffer(NULL)
{
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CSocket::~CSocket()
{
	Close();

	// Free resources.
	delete m_pSendBuffer;
	delete m_pRecvBuffer;
}

/******************************************************************************
** Method:		Close()
**
** Description:	Close the socket.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSocket::Close()
{
	// Socket still open?
	if (m_hSocket != INVALID_SOCKET)
	{
		// If async mode, end select.
		if (m_eMode == ASYNC)
			CWinSock::EndAsyncSelect(this);

		closesocket(m_hSocket);
	}

	// Reset members.
	m_hSocket = INVALID_SOCKET;

	if (m_pSendBuffer != NULL)
		m_pSendBuffer->Clear();

	if (m_pRecvBuffer != NULL)
		m_pRecvBuffer->Clear();
}

/******************************************************************************
** Method:		Create()
**
** Description:	Create the socket.
**
** Parameters:	See socket().
**
** Returns:		Nothing.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

void CSocket::Create(int nAF, int nType, int nProtocol)
{
	ASSERT(m_hSocket == INVALID_SOCKET);

	// Allocate the socket.
	m_hSocket = socket(nAF, nType, nProtocol);

	if (m_hSocket == INVALID_SOCKET)
		throw CSocketException(CSocketException::E_CREATE_FAILED, CWinSock::LastError());
}

/******************************************************************************
** Method:		Available()
**
** Description:	Queries the number of bytes that can be read without blocking.
**				NB: recv() may return less than this.
**
** Parameters:	None.
**
** Returns:		The number of bytes available.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

uint CSocket::Available()
{
	ASSERT(m_hSocket != INVALID_SOCKET);

	ulong lAvailable = 0;

	// Blocking socket?
	if (m_eMode == BLOCK)
	{
		fd_set  aoSockets;
		TIMEVAL oWaitTime  = { 0 };

		FD_ZERO(&aoSockets);
		FD_SET(m_hSocket, &aoSockets);

		// Check for readability on the socket.
		if (select(1, &aoSockets, NULL, NULL, &oWaitTime) == SOCKET_ERROR)
			throw CSocketException(CSocketException::E_SELECT_FAILED, CWinSock::LastError());

		// Anything to read?
		if (FD_ISSET(m_hSocket, &aoSockets))
		{
			// How much data is available?
			int nResult = ::ioctlsocket(m_hSocket, FIONREAD, &lAvailable);

			if (nResult == SOCKET_ERROR)
				throw CSocketException(CSocketException::E_PEEK_FAILED, CWinSock::LastError());

			if (lAvailable == 0)
				throw CSocketException(CSocketException::E_DISCONNECTED, WSAESHUTDOWN);
		}
	}
	// Async socket.
	else // (eMode == ASYNC)
	{
		if (m_pRecvBuffer != NULL)
			lAvailable = m_pRecvBuffer->Size();
	}

	return lAvailable;
}

/******************************************************************************
** Method:		Send()
**
** Description:	Send the data.
**
** Parameters:	pBuffer		The buffer to send.
**				nBufSize	The buffer size.
**
** Returns:		The number of bytes sent.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

uint CSocket::Send(const void* pBuffer, uint nBufSize)
{
	ASSERT(pBuffer != NULL);

	// Ignore, if nothing to send.
	if (nBufSize == 0)
		return 0;

	// Socket closed?
	if (m_hSocket == INVALID_SOCKET)
		throw CSocketException(CSocketException::E_SEND_FAILED, WSAENOTCONN);

	int nResult = 0;

	// Blocking socket?
	if (m_eMode == BLOCK)
	{
		// Send the entire buffer.
		nResult = send(m_hSocket, (const char*)pBuffer, nBufSize, 0);

		if (nResult == SOCKET_ERROR)
			throw CSocketException(CSocketException::E_SEND_FAILED, CWinSock::LastError());

		ASSERT((uint)nResult == nBufSize);
	}
	// Async socket.
	else // (eMode == ASYNC)
	{
		// Allocate send buffer, on first call.
		if (m_pSendBuffer == NULL)
			m_pSendBuffer = new CNetBuffer();

		// Append new data to existing send data.
		if (m_pSendBuffer->Append(pBuffer, nBufSize) > 0)
		{
			// Try and send the entire buffer.
			nResult = send(m_hSocket, (const char*)m_pSendBuffer->Ptr(), m_pSendBuffer->Size(), 0);

			if (nResult != SOCKET_ERROR)
			{
				// Remove amount sent.
				m_pSendBuffer->Discard(nResult);
			}
			else // (nResult == SOCKET_ERROR)
			{
				int nLastErr = CWinSock::LastError();

				// Only an error, if not because of lack of buffer space.
				if (nLastErr != WSAEWOULDBLOCK)
					throw CSocketException(CSocketException::E_SEND_FAILED, nLastErr);
			}
		}
	}

	return nResult;
}

/******************************************************************************
** Method:		Recv()
**
** Description:	Read the incoming data.
**
** Parameters:	pBuffer		The buffer to write to.
**				nBufSize	The buffer size.
**
** Returns:		The number of bytes read.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

uint CSocket::Recv(void* pBuffer, uint nBufSize)
{
	ASSERT(pBuffer  != NULL);
	ASSERT(nBufSize >= 0);

	// Socket closed?
	if (m_hSocket == INVALID_SOCKET)
		throw CSocketException(CSocketException::E_RECV_FAILED, WSAENOTCONN);

	int nResult = 0;

	// Blocking socket?
	if (m_eMode == BLOCK)
	{
		nResult = recv(m_hSocket, (char*)pBuffer, nBufSize, 0);

		if (nResult == SOCKET_ERROR)
			throw CSocketException(CSocketException::E_RECV_FAILED, CWinSock::LastError());
	}
	// Async socket.
	else // (eMode == ASYNC)
	{
		if (m_pRecvBuffer != NULL)
		{
			nBufSize = min(nBufSize, m_pRecvBuffer->Size());

			if (nBufSize != 0)
			{
				memcpy(pBuffer, m_pRecvBuffer->Ptr(), nBufSize);

				m_pRecvBuffer->Discard(nBufSize);
			}

			nResult = nBufSize;
		}
	}

	return nResult;
}

/******************************************************************************
** Method:		Peek()
**
** Description:	Peek at the incoming data.
**
** Parameters:	pBuffer		The buffer to write to.
**				nBufSize	The buffer size.
**
** Returns:		The number of bytes left to read.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

uint CSocket::Peek(void* pBuffer, uint nBufSize)
{
	ASSERT(m_hSocket != INVALID_SOCKET);
	ASSERT(pBuffer   != NULL);

	int nResult = 0;

	// Blocking socket?
	if (m_eMode == BLOCK)
	{
		nResult = recv(m_hSocket, (char*)pBuffer, nBufSize, MSG_PEEK);

		if (nResult == SOCKET_ERROR)
			throw CSocketException(CSocketException::E_PEEK_FAILED, CWinSock::LastError());
	}
	// Async socket.
	else // (eMode == ASYNC)
	{
		if (m_pRecvBuffer != NULL)
		{
			nBufSize = min(nBufSize, m_pRecvBuffer->Size());

			if (nBufSize != 0)
				memcpy(pBuffer, m_pRecvBuffer->Ptr(), nBufSize);

			nResult = nBufSize;
		}
	}

	return nResult;
}

/******************************************************************************
** Method:		Connect()
**
** Description:	Open a connection.
**
** Parameters:	pszHost		The host name.
**				nPort		The port number.
**
** Returns:		Nothing.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

void CSocket::Connect(const char* pszHost, uint nPort)
{
	ASSERT(m_hSocket == INVALID_SOCKET);
	ASSERT(pszHost   != NULL);
	ASSERT(nPort     <= USHRT_MAX);

	// Save parameters.
	m_strHost = pszHost;
	m_nPort   = nPort;

	sockaddr_in	addr = { 0 };

	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = inet_addr(pszHost);
	addr.sin_port        = htons((ushort)nPort);

	// Host name isn't an IP Address?
	if (addr.sin_addr.s_addr == INADDR_NONE)
	{
		// Resolve host name.
		hostent* pHost = gethostbyname(pszHost);

		if (pHost == NULL)
			throw CSocketException(CSocketException::E_RESOLVE_FAILED, CWinSock::LastError());

		memcpy(&addr.sin_addr, pHost->h_addr_list[0], pHost->h_length);
	}

	// Create the socket.
	Create(AF_INET, Type(), Protocol());

	// Connect to host.
	if (connect(m_hSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		throw CSocketException(CSocketException::E_CONNECT_FAILED, CWinSock::LastError());

	// If async mode, do select.
	if (m_eMode == ASYNC)
		CWinSock::BeginAsyncSelect(this, (FD_READ | FD_WRITE | FD_CLOSE));
}

/******************************************************************************
** Method:		IsAddress()
**
** Description:	Queries if the host name is a raw IP address.
**
** Parameters:	pszHost		The host name.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CSocket::IsAddress(const char* pszHost)
{
	return (inet_addr(pszHost) != INADDR_NONE);
}

/******************************************************************************
** Method:		Resolve()
**
** Description:	Resolves the host name into an IP address.
**
** Parameters:	pszHost		The host name.
**
** Returns:		The IP address.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

in_addr CSocket::Resolve(const char* pszHost)
{
	in_addr addr;

	// Is already an IP address?
	addr.s_addr = inet_addr(pszHost);

	if (addr.s_addr == INADDR_NONE)
	{
		// Resolve host name.
		hostent* pHost = gethostbyname(pszHost);

		if (pHost == NULL)
			throw CSocketException(CSocketException::E_RESOLVE_FAILED, CWinSock::LastError());

		memcpy(&addr, pHost->h_addr_list[0], pHost->h_length);
	}

	return addr;
}

/******************************************************************************
** Method:		ResolveStr()
**
** Description:	Resolves the host name into an IP address string.
**
** Parameters:	pszHost		The host name.
**
** Returns:		The IP address as a string.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

CString CSocket::ResolveStr(const char* pszHost)
{
	// Is already an IP address?
	if (IsAddress(pszHost))
		return pszHost;

	in_addr addr;

	// Resolve host name.
	hostent* pHost = gethostbyname(pszHost);

	if (pHost == NULL)
		throw CSocketException(CSocketException::E_RESOLVE_FAILED, CWinSock::LastError());

	memcpy(&addr, pHost->h_addr_list[0], pHost->h_length);

	return inet_ntoa(addr);
}

/******************************************************************************
** Method:		AsyncEventStr()
**
** Description:	Convert the async event ID to a string symbol.
**
** Parameters:	nEvent		The event ID.
**
** Returns:		The event synbol.
**
*******************************************************************************
*/

CString CSocket::AsyncEventStr(int nEvent)
{
	// Decode event.
	switch (nEvent)
	{
		case FD_READ:		return "FD_READ";
		case FD_WRITE:		return "FD_WRITE";
		case FD_OOB:		return "FD_OOB";
		case FD_ACCEPT:		return "FD_ACCEPT";
		case FD_CONNECT:	return "FD_CONNECT";
		case FD_CLOSE:		return "FD_CLOSE";
	}

	ASSERT(false);

	return "FD_?";
}

/******************************************************************************
** Method:		AddClientListener()
**
** Description:	Add a socket event listener.
**
** Parameters:	pListener	The listener.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSocket::AddClientListener(IClientSocketListener* pListener)
{
	ASSERT(pListener != NULL);

	m_aoCltListeners.push_back(pListener);
}

/******************************************************************************
** Method:		RemoveClientListener()
**
** Description:	Remove a socket event listener.
**
** Parameters:	pListener	The listener.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSocket::RemoveClientListener(IClientSocketListener* pListener)
{
	ASSERT(pListener != NULL);

	CCltListeners::iterator it = std::find(m_aoCltListeners.begin(), m_aoCltListeners.end(), pListener);

	if (it != m_aoCltListeners.end())
		m_aoCltListeners.erase(it);
}

/******************************************************************************
** Method:		OnAsyncSelect()
**
** Description:	Async socket event handler.
**
** Parameters:	nEvent		The event.
**				nError		The error.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSocket::OnAsyncSelect(int nEvent, int nError)
{
	// Error occured BUT not due to socket closure?
	if ( (nError != 0) && ((nEvent & FD_CLOSE) == 0) )
	{
		OnError(nEvent, nError);
		return;
	}

	// Socket closed?
	if (nEvent & FD_CLOSE)
	{
		OnClosed(nError);
		return;
	}

	// Read/Write now available?
	if (nEvent & FD_READ)
		OnReadReady();

	if (nEvent & FD_WRITE)
		OnWriteReady();
}

/******************************************************************************
** Method:		OnReadReady()
**
** Description:	The socket has data available to read.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSocket::OnReadReady()
{
	typedef CCltListeners::const_iterator iter;

	const uint TMP_BUF_SIZE = USHRT_MAX;

	// Allocate receive buffer, on first call.
	if (m_pRecvBuffer == NULL)
		m_pRecvBuffer = new CNetBuffer();

	// Allocate temporary read buffer.
	byte* pBuffer = (byte*) alloca(TMP_BUF_SIZE);

	// Read as much as possible.
	int nResult = recv(m_hSocket, (char*)pBuffer, TMP_BUF_SIZE, 0);

	if (nResult == SOCKET_ERROR)
	{
		int nLastErr = CWinSock::LastError();

		ASSERT(nLastErr != WSAEWOULDBLOCK);

		// Notify listeners of error.
		for (iter it = m_aoCltListeners.begin(); it != m_aoCltListeners.end(); ++it)
			(*it)->OnError(this, FD_READ, nLastErr);

		return;
	}

	ASSERT(nResult != 0);

	// Append to receive buffer.
	m_pRecvBuffer->Append(pBuffer, nResult);

	// Notify listeners of data.
	for (iter it = m_aoCltListeners.begin(); it != m_aoCltListeners.end(); ++it)
		(*it)->OnReadReady(this);
}

/******************************************************************************
** Method:		OnWriteReady()
**
** Description:	The socket has space available to write.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSocket::OnWriteReady()
{
	typedef CCltListeners::const_iterator iter;

	// Anything still to send?
	if ( (m_pSendBuffer != NULL) && (m_pSendBuffer->Size() > 0) )
	{
		// Try and send the entire buffer.
		int nResult = send(m_hSocket, (const char*)m_pSendBuffer->Ptr(), m_pSendBuffer->Size(), 0);

		if (nResult != SOCKET_ERROR)
		{
			// Remove amount sent.
			m_pSendBuffer->Discard(nResult);
		}
		else // (nResult != SOCKET_ERROR)
		{
			int nLastErr = CWinSock::LastError();

			// Only an error, if not because of lack of buffer space.
			if (nLastErr != WSAEWOULDBLOCK)
			{
				// Notify listeners of error.
				for (iter it = m_aoCltListeners.begin(); it != m_aoCltListeners.end(); ++it)
					(*it)->OnError(this, FD_WRITE, nLastErr);

				return;
			}
		}
	}
}

/******************************************************************************
** Method:		OnClosed()
**
** Description:	The socket was closed.
**
** Parameters:	nReason		The error code.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSocket::OnClosed(int nReason)
{
	typedef CCltListeners::const_iterator iter;

	// Cleanup.
	Close();

	// Notify listeners.
	for (iter it = m_aoCltListeners.begin(); it != m_aoCltListeners.end(); ++it)
		(*it)->OnClosed(this, nReason);
}

/******************************************************************************
** Method:		OnError()
**
** Description:	An async error occurred for an event.
**
** Parameters:	nEvent		The event.
**				nError		The error.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSocket::OnError(int nEvent, int nError)
{
	typedef CCltListeners::const_iterator iter;

	// Notify listeners.
	for (iter it = m_aoCltListeners.begin(); it != m_aoCltListeners.end(); ++it)
		(*it)->OnError(this, nEvent, nError);
}
