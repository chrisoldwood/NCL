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
	// Accessors.
	//
	HANDLE Handle() const;
	bool   IsOpen() const;

	//
	// Methods.
	//
	uint Available();
	uint Peek(void* pBuffer, uint nBufSize);
	uint Peek(CBuffer& oBuffer, uint nBufSize);
	uint Read(void* pBuffer, uint nBufSize);
	uint Read(CBuffer& oBuffer);
	uint Write(const void* pBuffer, uint nBufSize);
	uint Write(const CBuffer& oBuffer);

protected:
	//
	// Members.
	//
	HANDLE	m_hPipe;		// The pipe handle.
	CString	m_strName;		// The pipe name.

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

inline uint CNamedPipe::Peek(CBuffer& oBuffer, uint nBufSize)
{
	return Peek(oBuffer.Buffer(), nBufSize);
}

inline uint CNamedPipe::Read(CBuffer& oBuffer)
{
	return Read(oBuffer.Buffer(), oBuffer.Size());
}

inline uint CNamedPipe::Write(const CBuffer& oBuffer)
{
	return Write(oBuffer.Buffer(), oBuffer.Size());
}

#endif // NAMEDPIPE_HPP
