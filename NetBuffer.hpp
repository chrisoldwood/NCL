/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		NETBUFFER.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CNetBuffer class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef NETBUFFER_HPP
#define NETBUFFER_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/Buffer.hpp>

/******************************************************************************
** 
** A variable-sized data buffer.
**
*******************************************************************************
*/

class CNetBuffer
{
public:
	//
	// Constructors/Destructor.
	//
	CNetBuffer();
	~CNetBuffer();
	
	//
	// Properties.
	//
	bool		Empty() const;
	uint        Size() const;
	uint        Capacity() const;
	const void* Ptr() const;
	
	//
	// Methods.
	//
	uint Append(const void* pBuffer, uint nBufSize);
	uint Discard(uint nCount);
	void Clear();

protected:
	//
	// Members.
	//
	CBuffer		m_oBuffer;			// The underlying buffer.
	uint		m_nDataSize;		// The used buffer space.
	uint		m_nMinCapacity;		// Minimum capacity.

	//
	// Constants.
	//
	static const uint DEF_MIN_CAPACITY = 4096;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline bool CNetBuffer::Empty() const
{
	return (Size() == 0);
}

inline uint CNetBuffer::Size() const
{
	return m_nDataSize;
}

inline uint CNetBuffer::Capacity() const
{
	return m_oBuffer.Size();
}

inline const void* CNetBuffer::Ptr() const
{
	return m_oBuffer.Buffer();
}

#endif // NETBUFFER_HPP
