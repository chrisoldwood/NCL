/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PIPEEXCEPTION.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CPipeException class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef PIPEEXCEPTION_HPP
#define PIPEEXCEPTION_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <Core/Exception.hpp>

/******************************************************************************
** 
** The exception type thrown by the Named Pipe classes.
**
*******************************************************************************
*/

class CPipeException : public Core::Exception
{
public:
	//
	// Exception codes.
	//
	enum ErrCode
	{
		E_CREATE_FAILED	= 10,	// Failed to create pipe.
		E_OPEN_FAILED	= 11,	// Failed to open pipe.
		E_ACCEPT_FAILED	= 12,	// Failed to accept connection.
		E_PEEK_FAILED	= 13,	// Failed to read from pipe.
		E_READ_FAILED	= 14,	// Failed to read from pipe.
		E_WRITE_FAILED	= 15,	// Failed to write to pipe.
		E_BAD_PROTOCOL	= 16,	// Incorrect protocol version.
	};

	//
	// Constructors/Destructor.
	//
	CPipeException(int eErrCode, HRESULT hResult);
	~CPipeException();

	//
	// Members.
	//
	HRESULT m_hResult;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // PIPEEXCEPTION_HPP
