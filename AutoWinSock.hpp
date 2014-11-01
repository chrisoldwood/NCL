////////////////////////////////////////////////////////////////////////////////
//! \file   AutoWinSock.hpp
//! \brief  The AutoWinSock class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef NCL_AUTOWINSOCK_HPP
#define NCL_AUTOWINSOCK_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "WinSock.hpp"
#include "SocketException.hpp"

////////////////////////////////////////////////////////////////////////////////
//! An RAII style class for initialising and uninitialising the WinSock library.

class AutoWinSock
{
public:
	//! Default constructor.
	AutoWinSock();

	//! Destructor.
	~AutoWinSock();
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

inline AutoWinSock::AutoWinSock()
{
	int result = CWinSock::Startup(1, 1);

	if (result != 0)
	{
		ASSERT_FALSE();
		throw CSocketException(CSocketException::E_INIT_FAILED, CWinSock::LastError());
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

inline AutoWinSock::~AutoWinSock()
{
	int result = CWinSock::Cleanup();

	if (result != 0)
	{
		ASSERT_FALSE();
		Core::debugWrite(TXT("WinSock cleanup failed with result: %d [%s]\n"), result, CWinSock::LastErrorToSymbol().c_str());
	}
}

#endif // NCL_AUTOWINSOCK_HPP
