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
		E_CONNECT_FAILED = 11,	// Failed to connect to host.
		E_RESOLVE_FAILED = 12,	// Failed to resolve host name.
		E_SEND_FAILED    = 13,  // Failed to send data.
		E_RECV_FAILED    = 14,	// Failed to recieve data.
		E_PEEK_FAILED    = 15,	// Failed to peek at incoming data.
	};

	//
	// Constructors/Destructor.
	//
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
