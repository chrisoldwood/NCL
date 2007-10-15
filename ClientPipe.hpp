/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CLIENTPIPE.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CClientPipe class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef CLIENTPIPE_HPP
#define CLIENTPIPE_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "NamedPipe.hpp"

/******************************************************************************
** 
** The client end of a Named Pipe.
**
*******************************************************************************
*/

class CClientPipe : public CNamedPipe
{
public:
	//
	// Constructors/Destructor.
	//
	CClientPipe();
	virtual ~CClientPipe();
	
	//
	// Methods.
	//
	void Open(const char* pszName);

	virtual void Close();

protected:
	//
	// Members.
	//

	//
	// Constants.
	//
	static const DWORD DEF_OPEN_MODE;
	static const DWORD DEF_PIPE_MODE;
	static const DWORD DEF_INTERVAL;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // CLIENTPIPE_HPP
