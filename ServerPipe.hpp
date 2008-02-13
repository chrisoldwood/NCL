/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SERVERPIPE.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CServerPipe class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SERVERPIPE_HPP
#define SERVERPIPE_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "NamedPipe.hpp"

/******************************************************************************
** 
** The server end of a Named Pipe.
**
*******************************************************************************
*/

class CServerPipe : public CNamedPipe
{
public:
	//
	// Constructors/Destructor.
	//
	CServerPipe();
	virtual ~CServerPipe();

	//
	// Methods.
	//
	void Create(const tchar* pszName);
	bool Accept();

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
	static const DWORD DEF_BUF_SIZE;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // SERVERPIPE_HPP
