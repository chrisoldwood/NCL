/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CLIENTPIPE.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CClientPipe class definition.
**
*******************************************************************************
*/

#include "ncl.hpp"

/******************************************************************************
**
** Constants.
**
*******************************************************************************
*/

const DWORD CClientPipe::DEF_PIPE_MODE = PIPE_READMODE_MESSAGE | PIPE_NOWAIT;
const DWORD CClientPipe::DEF_TIMEOUT   = 30000;
const DWORD CClientPipe::DEF_INTERVAL  = 1000;

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

CClientPipe::CClientPipe()
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

CClientPipe::~CClientPipe()
{
	Close();
}

/******************************************************************************
** Method:		Open()
**
** Description:	Open an exisitng named pipe.
**
** Parameters:	pszName		The pipes' name.
**
** Returns:		Nothing.
**
** Exceptions:	CPipeException.
**
*******************************************************************************
*/

void CClientPipe::Open(const char* pszName)
{
	// Calculate max wait time.
	DWORD dwMaxTime = ::GetTickCount() + DEF_TIMEOUT;

	while (::GetTickCount() < dwMaxTime)
	{
		::WaitNamedPipe(pszName, DEF_INTERVAL);

		// Try and open the pipe.
		m_hPipe = ::CreateFile(pszName, GENERIC_READWRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		// Success OR NOT server busy?
		if ( (m_hPipe != INVALID_HANDLE_VALUE) || (::GetLastError() != ERROR_PIPE_BUSY) )
			break;
	}

	// Create failed?
	if (m_hPipe == INVALID_HANDLE_VALUE)
		throw CPipeException(CPipeException::E_OPEN_FAILED, ::GetLastError());

	DWORD dwPipeMode = DEF_PIPE_MODE;

	// Switch pipe to message mode.
	if (::SetNamedPipeHandleState(m_hPipe, &dwPipeMode, NULL, NULL) == 0)
		throw CPipeException(CPipeException::E_OPEN_FAILED, ::GetLastError());
}

/******************************************************************************
** Method:		Close()
**
** Description:	Close the named pipe.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CClientPipe::Close()
{
	// Close, if handle was allocated.
	if (m_hPipe != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_hPipe);
	}

	// Reset members.
	m_hPipe = INVALID_HANDLE_VALUE;
}
