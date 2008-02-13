/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		NETBUFFER.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CNetBuffer class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "NetBuffer.hpp"

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

CNetBuffer::CNetBuffer()
	: m_oBuffer(DEF_MIN_CAPACITY)
	, m_nDataSize(0)
	, m_nMinCapacity(DEF_MIN_CAPACITY)
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

CNetBuffer::~CNetBuffer()
{
}

/******************************************************************************
** Method:		Append()
**
** Description:	Appends new data to the end of the buffer.
**
** Parameters:	pBuffer		The buffer of data.
**				nBufSize	The buffer size.
**
** Returns:		The new buffer size.
**
*******************************************************************************
*/

size_t CNetBuffer::Append(const void* pBuffer, size_t nBufSize)
{
	ASSERT(pBuffer != NULL);

	// Anything to append?
	if (nBufSize > 0)
	{
		// Increase capacity?
		while ((Size() + nBufSize) > Capacity())
			m_oBuffer.Size(m_oBuffer.Size() * 2);

		byte* pBufEnd = static_cast<byte*>(m_oBuffer.Buffer()) + Size();

		// Copy data in.
		memcpy(pBufEnd, pBuffer, nBufSize);

		// Update state.
		m_nDataSize += nBufSize;
	}

	return Size();
}

/******************************************************************************
** Method:		Discard()
**
** Description:	Discard a number of bytes from the front of the buffer.
**
** Parameters:	nCount		The number of bytes.
**
** Returns:		The new buffer size.
**
*******************************************************************************
*/

size_t CNetBuffer::Discard(size_t nCount)
{
	// Discarding the entire buffer?
	if (nCount == m_nDataSize)
	{
		Clear();
	}
	// Discarding part only.
	else if (nCount > 0)
	{
		byte* pBuffer = static_cast<byte*>(m_oBuffer.Buffer());

		// Shuffle data down.
		memmove(pBuffer, pBuffer+nCount, m_nDataSize-nCount);

		// Update state.
		m_nDataSize -= nCount;
	}

	return Size();
}

/******************************************************************************
** Method:		Clear()
**
** Description:	Discard the buffer contents and reset its capacity.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CNetBuffer::Clear()
{
	// Reset buffer.
	m_oBuffer.Size(m_nMinCapacity);
	m_nDataSize = 0;
}
