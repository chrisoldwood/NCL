/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SOCKETEXCEPTION.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CSocketException class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "SocketException.hpp"
#include "WinSock.hpp"

/******************************************************************************
** Method:		Constructor.
**
** Description:	Formats an exception which includes GetLastError().
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CSocketException::CSocketException(int eErrCode, int nWSACode)
	: m_nWSACode(nWSACode)
{
	// Get error code symbol.
	CString strSymbol = CWinSock::ErrorToSymbol(nWSACode);

	// Convert error to string.
	switch(eErrCode)
	{
		case E_CREATE_FAILED:
			m_strErrorText.Format(TXT("Failed to create socket: %s"), strSymbol);
			break;

		case E_RESOLVE_FAILED:
			m_strErrorText.Format(TXT("Failed to resolve hostname: %s"), strSymbol);
			break;

		case E_CONNECT_FAILED:
			m_strErrorText.Format(TXT("Failed to connect to host: %s"), strSymbol);
			break;

		case E_BIND_FAILED:
			m_strErrorText.Format(TXT("Failed to bind socket: %s"), strSymbol);
			break;

		case E_LISTEN_FAILED:
			m_strErrorText.Format(TXT("Failed to listen on socket: %s"), strSymbol);
			break;

		case E_ACCEPT_FAILED:
			m_strErrorText.Format(TXT("Failed to accept client connection: %s"), strSymbol);
			break;

		case E_SEND_FAILED:
			m_strErrorText.Format(TXT("Failed to send data: %s"), strSymbol);
			break;

		case E_RECV_FAILED:
			m_strErrorText.Format(TXT("Failed to receive data: %s"), strSymbol);
			break;

		case E_PEEK_FAILED:
			m_strErrorText.Format(TXT("Failed to peek incoming data: %s"), strSymbol);
			break;

		case E_SELECT_FAILED:
			m_strErrorText.Format(TXT("Failed to query socket state: %s"), strSymbol);
			break;

		case E_QUERY_FAILED:
			m_strErrorText.Format(TXT("Failed to query socket info: %s"), strSymbol);
			break;

		case E_DISCONNECTED:
			m_strErrorText.Format(TXT("Connection closed: %s"), strSymbol);
			break;

		case E_BAD_PROTOCOL:
			m_strErrorText.Format(TXT("Invalid socket protocol version"));
			break;

		case E_WAIT_FAILED:
			m_strErrorText.Format(TXT("Failed waiting for response: %s"), strSymbol);
			break;

		// Shouldn't happen!
		default:
			ASSERT_FALSE();
			break;
	}
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

CSocketException::~CSocketException()
{
}
