/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		NAMEDPIPE.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CNamedPipe class definition.
**
*******************************************************************************
*/

#include "ncl.hpp"

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

CNamedPipe::CNamedPipe()
	: m_hPipe(INVALID_HANDLE_VALUE)
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

CNamedPipe::~CNamedPipe()
{
}

/******************************************************************************
** Method:		Available()
**
** Description:	Query the number of bytes available in the pipe for reading.
**
** Parameters:	None.
**
** Returns:		The number of bytes available.
**
** Exceptions:	CPipeException.
**
*******************************************************************************
*/

uint CNamedPipe::Available()
{
	DWORD dwAvail, dwMsgAvail;

	// Query the bytes available.
	if (!::PeekNamedPipe(m_hPipe, NULL, 0, NULL, &dwAvail, &dwMsgAvail))
		throw CPipeException(CPipeException::E_PEEK_FAILED, ::GetLastError());

	return dwAvail;
}

/******************************************************************************
** Method:		Peek()
**
** Description:	Peek at the data in the pipe.
**
** Parameters:	pBuffer		The buffer to store the data.
**				nBufSize	The number of bytes to peek at.
**
** Returns:		The number of bytes peeked.
**
** Exceptions:	CPipeException.
**
*******************************************************************************
*/

uint CNamedPipe::Peek(void* pBuffer, uint nBufSize)
{
	DWORD dwRead, dwAvail, dwMsgAvail;

	// Query the bytes available.
	if (!::PeekNamedPipe(m_hPipe, pBuffer, nBufSize, &dwRead, &dwAvail, &dwMsgAvail))
		throw CPipeException(CPipeException::E_PEEK_FAILED, ::GetLastError());

	return dwRead;
}

/******************************************************************************
** Method:		Read()
**
** Description:	Read data from the pipe.
**
** Parameters:	pBuffer		The buffer to store the data.
**				nBufSize	The number of bytes to read.
**
** Returns:		The number of bytes read.
**
** Exceptions:	CPipeException.
**
*******************************************************************************
*/

uint CNamedPipe::Read(void* pBuffer, uint nBufSize)
{
	DWORD dwRead;

	// Query the bytes available.
	if (!::ReadFile(m_hPipe, pBuffer, nBufSize, &dwRead, NULL))
		throw CPipeException(CPipeException::E_READ_FAILED, ::GetLastError());

	return dwRead;
}

/******************************************************************************
** Method:		Write()
**
** Description:	Write data to the pipe.
**
** Parameters:	pBuffer		The buffer to store the data.
**				nBufSize	The number of bytes to write.
**
** Returns:		The number of bytes written.
**
** Exceptions:	CPipeException.
**
*******************************************************************************
*/

uint CNamedPipe::Write(const void* pBuffer, uint nBufSize)
{
	DWORD dwWritten;

	// Query the bytes available.
	if (!::WriteFile(m_hPipe, pBuffer, nBufSize, &dwWritten, NULL))
		throw CPipeException(CPipeException::E_READ_FAILED, ::GetLastError());

	return dwWritten;
}
