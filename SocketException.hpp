/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SOCKETEXCEPTION.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CSocketException class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SOCKETEXCEPTION_HPP
#define SOCKETEXCEPTION_HPP

/******************************************************************************
** 
** The exception type thrown by the Socket classes.
**
*******************************************************************************
*/

class CSocketException : public CException
{
public:
	//
	// Exception codes.
	//
	enum ErrCode
	{
		E_CREATE_FAILED  = 10,	// Failed to create socket.
		E_RESOLVE_FAILED = 11,	// Failed to resolve host name.
		E_CONNECT_FAILED = 12,	// Failed to connect client to host.
		E_BIND_FAILED    = 13,	// Failed to bind server socket.
		E_LISTEN_FAILED  = 14,	// Failed to listen on server socket.
		E_ACCEPT_FAILED  = 15,	// Failed to accept client connection.
		E_SEND_FAILED    = 16,  // Failed to send data.
		E_RECV_FAILED    = 17,	// Failed to recieve data.
		E_PEEK_FAILED    = 18,	// Failed to peek at incoming data.
		E_SELECT_FAILED  = 19,	// Failed to query socket state.
		E_QUERY_FAILED   = 20,	// Failed to query socket info.
		E_DISCONNECTED   = 21,	// Connection was closed.
		E_BAD_PROTOCOL	 = 22,	// Incorrect protocol version.
	};

	//
	// Constructors/Destructor.
	//
	CSocketException(int eErrCode);
	CSocketException(int eErrCode, int nWSACode);
	~CSocketException();
	
	//
	// Members.
	//
	int m_nWSACode;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // SOCKETEXCEPTION_HPP
