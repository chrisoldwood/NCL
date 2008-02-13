/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		NAMEDPIPE.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CNamedPipe class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "NamedPipe.hpp"
#include "PipeException.hpp"

/******************************************************************************
**
** Constants.
**
*******************************************************************************
*/

const DWORD CNamedPipe::DEF_TIMEOUT = 30000;

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
	, m_dwTimeOut(DEF_TIMEOUT)
{
	// Clear Overlapped I/O structs.
	memset(&m_oReadIO,  0, sizeof(m_oReadIO));
	memset(&m_oWriteIO, 0, sizeof(m_oWriteIO));

	// Attach event handles to Overlapped I/O structs.
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
	ASSERT(m_hPipe == INVALID_HANDLE_VALUE);
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

size_t CNamedPipe::Available()
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

size_t CNamedPipe::Peek(void* pBuffer, size_t nBufSize)
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

void CNamedPipe::Read(void* pBuffer, size_t nBufSize)
{
	ASSERT(pBuffer  != NULL);
	ASSERT(nBufSize != 0 );

	DWORD dwRead;

	// Start the read.
	BOOL bResult = ::ReadFile(m_hPipe, pBuffer, nBufSize, &dwRead, &m_oReadIO);

	// Read failed?
	if ( (bResult == FALSE) && (::GetLastError() != ERROR_IO_PENDING) )
		throw CPipeException(CPipeException::E_READ_FAILED, ::GetLastError());

	// Wait for I/O to finish OR time out.
	m_oReadEvent.Wait(m_dwTimeOut);

	bResult = ::GetOverlappedResult(m_hPipe, &m_oReadIO, &dwRead, FALSE);

	// Read failed?
	if (bResult == FALSE)
	{
		DWORD dwResult = ::GetLastError();

		::CancelIo(m_hPipe);

		// Remap error code if it timed-out.
		if (dwResult == ERROR_IO_INCOMPLETE)
			dwResult = WAIT_TIMEOUT;

		throw CPipeException(CPipeException::E_READ_FAILED, dwResult);
	}

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

void CNamedPipe::Write(const void* pBuffer, size_t nBufSize)
{
	ASSERT(pBuffer  != NULL);
	ASSERT(nBufSize != 0 );

	DWORD dwWritten;

	if (m_bPrevWrite)
	{
		// Wait for I/O to finish OR time out.
		m_oWriteEvent.Wait(m_dwTimeOut);

		BOOL bResult = ::GetOverlappedResult(m_hPipe, &m_oWriteIO, &dwWritten, FALSE);

		// Write failed?
		if (bResult == FALSE)
		{
			DWORD dwResult = ::GetLastError();

			::CancelIo(m_hPipe);

			// Remap error code if it timed-out.
			if (dwResult == ERROR_IO_INCOMPLETE)
				dwResult = WAIT_TIMEOUT;

			throw CPipeException(CPipeException::E_WRITE_FAILED, dwResult);
		}

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

void CNamedPipe::Close()
{
	// Not already closed?.
	if (m_hPipe != INVALID_HANDLE_VALUE)
		::CloseHandle(m_hPipe);

	// Reset members.
	m_hPipe      = INVALID_HANDLE_VALUE;
	m_bPrevWrite = false;
	m_nPrevBytes = 0;

	// Reset events to signalled.
	m_oReadEvent.Signal();
	m_oWriteEvent.Signal();
}
