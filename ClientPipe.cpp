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

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

/******************************************************************************
**
** Constants.
**
*******************************************************************************
*/

const DWORD CClientPipe::DEF_OPEN_MODE = /*FILE_ATTRIBUTE_NORMAL |*/ FILE_FLAG_OVERLAPPED;
const DWORD CClientPipe::DEF_PIPE_MODE = PIPE_READMODE_MESSAGE | PIPE_WAIT;
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
	DWORD dwMaxTime = ::GetTickCount() + m_dwTimeOut;

	while (::GetTickCount() < dwMaxTime)
	{
		::WaitNamedPipe(pszName, DEF_INTERVAL);

		// Try and open the pipe.
		m_hPipe = ::CreateFile(pszName, GENERIC_READWRITE, 0, NULL, OPEN_EXISTING, DEF_OPEN_MODE, NULL);

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
	CNamedPipe::Close();
}
