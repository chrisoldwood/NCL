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

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

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
	, m_oReadEvent(true, true)
	, m_oWriteEvent(true, true)
	, m_bPrevWrite(false)
	, m_nPrevBytes(0)
{
	// Clear Overlapped I/O structs.
	memset(&m_oReadIO,  0, sizeof(m_oReadIO));
	memset(&m_oWriteIO, 0, sizeof(m_oWriteIO));

	// Attach event handles Overlapped I/O structs.
	m_oReadIO.hEvent  = m_oReadEvent.Handle();
	m_oWriteIO.hEvent = m_oWriteEvent.Handle();
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
** Returns:		Nothing.
**
** Exceptions:	CPipeException.
**
*******************************************************************************
*/

void CNamedPipe::Read(void* pBuffer, uint nBufSize)
{
	DWORD dwRead;

	// Start the read.
	BOOL bResult = ::ReadFile(m_hPipe, pBuffer, nBufSize, &dwRead, &m_oReadIO);

	// Read failed?
	if ( (bResult == FALSE) && (::GetLastError() != ERROR_IO_PENDING) )
		throw CPipeException(CPipeException::E_READ_FAILED, ::GetLastError());

	// Wait for I/O to finish.
	bResult = ::GetOverlappedResult(m_hPipe, &m_oReadIO, &dwRead, TRUE);

	// Read failed?
	if (bResult == FALSE)
		throw CPipeException(CPipeException::E_READ_FAILED, ::GetLastError());

	ASSERT(dwRead == nBufSize);
}

/******************************************************************************
** Method:		Write()
**
** Description:	Write data to the pipe.
**
** Parameters:	pBuffer		The buffer to store the data.
**				nBufSize	The number of bytes to write.
**
** Returns:		Nothing.
**
** Exceptions:	CPipeException.
**
*******************************************************************************
*/

void CNamedPipe::Write(const void* pBuffer, uint nBufSize)
{
	DWORD dwWritten;

	if (m_bPrevWrite)
	{
		// Wait for previous write to finish.
		BOOL bResult = ::GetOverlappedResult(m_hPipe, &m_oWriteIO, &dwWritten, TRUE);

		// Write failed?
		if (bResult == FALSE)
			throw CPipeException(CPipeException::E_WRITE_FAILED, ::GetLastError());

		ASSERT(dwWritten == m_nPrevBytes);
	}

	// Start the write.
	BOOL bResult = ::WriteFile(m_hPipe, pBuffer, nBufSize, &dwWritten, &m_oWriteIO);

	// Write failed?
	if ( (bResult == FALSE) && (::GetLastError() != ERROR_IO_PENDING) )
		throw CPipeException(CPipeException::E_WRITE_FAILED, ::GetLastError());

	m_bPrevWrite = true;
	m_nPrevBytes = nBufSize;
}
