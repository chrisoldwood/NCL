/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		NAMEDPIPE.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CNamedPipe class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef NAMEDPIPE_HPP
#define NAMEDPIPE_HPP

/******************************************************************************
** 
** The base class for Named Pipes.
**
*******************************************************************************
*/

class CNamedPipe
{
public:
	//
	// Properties.
	//
	HANDLE Handle() const;
	bool   IsOpen() const;

	DWORD  TimeOut() const;
	void   SetTimeOut(DWORD dwTimeOut);

	//
	// Methods.
	//
	uint Available();
	uint Peek(void* pBuffer, uint nBufSize);
	uint Peek(CBuffer& oBuffer, uint nBufSize);
	void Read(void* pBuffer, uint nBufSize);
	void Read(CBuffer& oBuffer);
	void Write(const void* pBuffer, uint nBufSize);
	void Write(const CBuffer& oBuffer);

	virtual void Close();

protected:
	//
	// Members.
	//
	HANDLE		m_hPipe;		// The pipe handle.
	CString		m_strName;		// The pipe name.
	CEvent		m_oReadEvent;	// Read Overlapped I/O event.
	OVERLAPPED	m_oReadIO;		// Read Overlapped I/O data.
	CEvent		m_oWriteEvent;	// Write Overlapped I/O event.
	OVERLAPPED	m_oWriteIO;		// Write Overlapped I/O data.
	bool		m_bPrevWrite;	// Write outstanding?
	uint		m_nPrevBytes;	// Previous write size.
	DWORD		m_dwTimeOut;	// Connect/Read/Write timeout.

	//
	// Constants.
	//
	static const DWORD DEF_TIMEOUT;

	//
	// Constructors/Destructor.
	// NB: Abstract class.
	//
	CNamedPipe();
	CNamedPipe(const CNamedPipe&);
	virtual ~CNamedPipe();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline HANDLE CNamedPipe::Handle() const
{
	return m_hPipe;
}

inline bool CNamedPipe::IsOpen() const
{
	return (m_hPipe != INVALID_HANDLE_VALUE);
}

inline DWORD CNamedPipe::TimeOut() const
{
	return m_dwTimeOut;
}

inline void CNamedPipe::SetTimeOut(DWORD dwTimeOut)
{
	m_dwTimeOut = dwTimeOut;
}

inline uint CNamedPipe::Peek(CBuffer& oBuffer, uint nBufSize)
{
	return Peek(oBuffer.Buffer(), nBufSize);
}

inline void CNamedPipe::Read(CBuffer& oBuffer)
{
	Read(oBuffer.Buffer(), oBuffer.Size());
}

inline void CNamedPipe::Write(const CBuffer& oBuffer)
{
	Write(oBuffer.Buffer(), oBuffer.Size());
}

#endif // NAMEDPIPE_HPP
