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

const DWORD CServerPipe::DEF_OPEN_MODE = PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED;
const DWORD CServerPipe::DEF_PIPE_MODE = PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT;
const DWORD CServerPipe::DEF_BUF_SIZE  = 4096;

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
	SECURITY_DESCRIPTOR* pSecDescriptor = (SECURITY_DESCRIPTOR*) alloca(SECURITY_DESCRIPTOR_MIN_LENGTH);

	// Create a security descriptor to allow everyone access.
	if (::InitializeSecurityDescriptor(pSecDescriptor, SECURITY_DESCRIPTOR_REVISION) == 0)
		throw CPipeException(CPipeException::E_CREATE_FAILED, ::GetLastError());

	if (::SetSecurityDescriptorDacl(pSecDescriptor, TRUE, NULL, FALSE) == 0)
		throw CPipeException(CPipeException::E_CREATE_FAILED, ::GetLastError());

	SECURITY_ATTRIBUTES oSecAttributes = { 0 };

	// Bind descriptor to attributes handle.
	oSecAttributes.nLength              = sizeof(SECURITY_ATTRIBUTES);
	oSecAttributes.bInheritHandle       = TRUE;
	oSecAttributes.lpSecurityDescriptor = pSecDescriptor;

	// Reset error flag, to detect ERROR_ALREADY_EXISTS.
	::SetLastError(NO_ERROR);

	// Attempt to create the pipe.
	m_hPipe = ::CreateNamedPipe(pszName, DEF_OPEN_MODE, DEF_PIPE_MODE,
									PIPE_UNLIMITED_INSTANCES, DEF_BUF_SIZE,
									DEF_BUF_SIZE, m_dwTimeOut, &oSecAttributes);

	// Create failed?
	if (m_hPipe == INVALID_HANDLE_VALUE)
		throw CPipeException(CPipeException::E_CREATE_FAILED, ::GetLastError());

	// Already exists?
	if (::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		Close();
		throw CPipeException(CPipeException::E_CREATE_FAILED, ERROR_ALREADY_EXISTS);
	}

	// Connection waiting?
	BOOL bResult = ::ConnectNamedPipe(m_hPipe, &m_oReadIO);

	// Connection already established?
	if ( (bResult == FALSE) && (::GetLastError() == ERROR_PIPE_CONNECTED) )
		bResult = TRUE;

	// Error occurred?
	if ( (bResult == FALSE) && (::GetLastError() != ERROR_IO_PENDING) )
		throw CPipeException(CPipeException::E_ACCEPT_FAILED, ::GetLastError());
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

	DWORD dwRead;

	// Connection accepted?
	BOOL bResult = ::GetOverlappedResult(m_hPipe, &m_oReadIO, &dwRead, FALSE);

	// Error occurred?
	if ( (bResult == FALSE) && (::GetLastError() != ERROR_IO_INCOMPLETE) )
		throw CPipeException(CPipeException::E_ACCEPT_FAILED, ::GetLastError());

	return bResult;
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
	if (m_hPipe != INVALID_HANDLE_VALUE)
		::DisconnectNamedPipe(m_hPipe);

	CNamedPipe::Close();
}
