/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SERVERPIPE.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CServerPipe class definition.
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

const DWORD CServerPipe::DEF_OPEN_MODE = PIPE_ACCESS_DUPLEX;
const DWORD CServerPipe::DEF_PIPE_MODE = PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT;
const DWORD CServerPipe::DEF_BUF_SIZE  = 4096;
const DWORD CServerPipe::DEF_TIMEOUT   = 30000;

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

CServerPipe::CServerPipe()
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

CServerPipe::~CServerPipe()
{
	Close();
}

/******************************************************************************
** Method:		Create()
**
** Description:	Create an instance of the named pipe.
**
** Parameters:	pszName		The pipes' name.
**
** Returns:		Nothing.
**
** Exceptions:	CPipeException.
**
*******************************************************************************
*/

void CServerPipe::Create(const char* pszName)
{
	// Reset error flag, to detect ERROR_ALREADY_EXISTS.
	::SetLastError(NO_ERROR);

	// Attempt to create the pipe.
	m_hPipe = ::CreateNamedPipe(pszName, DEF_OPEN_MODE, DEF_PIPE_MODE,
									PIPE_UNLIMITED_INSTANCES, DEF_BUF_SIZE,
									DEF_BUF_SIZE, DEF_TIMEOUT, NULL);

	// Create failed?
	if (m_hPipe == INVALID_HANDLE_VALUE)
		throw CPipeException(CPipeException::E_CREATE_FAILED, ::GetLastError());

	// Already exists?
	if (::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		Close();
		throw CPipeException(CPipeException::E_CREATE_FAILED, ERROR_ALREADY_EXISTS);
	}
}

/******************************************************************************
** Method:		Accept()
**
** Description:	Accept a connection if one is outstanding.
**
** Parameters:	None.
**
** Returns:		true or false.
**
** Exceptions:	CPipeException.
**
*******************************************************************************
*/

bool CServerPipe::Accept()
{
	ASSERT(m_hPipe != INVALID_HANDLE_VALUE);

	// Connection waiting?
	// NB: Non-blocking by default.
	BOOL bOK = ::ConnectNamedPipe(m_hPipe, NULL);

	// Connection already established?
	if ( (bOK == FALSE) && (::GetLastError() == ERROR_PIPE_CONNECTED) )
		bOK = TRUE;

	// Error occurred?
	if ( (bOK == FALSE) && (::GetLastError() != ERROR_PIPE_LISTENING) )
		throw CPipeException(CPipeException::E_ACCEPT_FAILED, ::GetLastError());

	return bOK;
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

void CServerPipe::Close()
{
	// Close, if handle was allocated.
	if (m_hPipe != INVALID_HANDLE_VALUE)
	{
		::DisconnectNamedPipe(m_hPipe);
		::CloseHandle(m_hPipe);
	}

	// Reset members.
	m_hPipe = INVALID_HANDLE_VALUE;
}
