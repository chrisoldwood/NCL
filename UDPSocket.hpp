/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		UDPSOCKET.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CUDPSocket class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef UDPSOCKET_HPP
#define UDPSOCKET_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "Socket.hpp"

/******************************************************************************
** 
** A UDP type socket.
**
*******************************************************************************
*/

class CUDPSocket : public CSocket
{
public:
	virtual ~CUDPSocket();

	//
	// Properties.
	//
	virtual int Type()     const;
	virtual int Protocol() const;

	//
	// Methods.
	//
	size_t SendTo(const void* pBuffer, size_t nBufSize, const in_addr& oAddr, uint nPort);
	size_t SendTo(const CBuffer& oBuffer, const in_addr& oAddr, uint nPort);

	size_t RecvFrom(void* pBuffer, size_t nBufSize, in_addr& oAddr, uint& nPort);
	size_t RecvFrom(CBuffer& oBuffer, in_addr& oAddr, uint& nPort);

protected:
	//
	// Members.
	//

	// Protect creation etc.
	CUDPSocket(Mode eMode);
	CUDPSocket(const CUDPSocket&);
	void operator=(const CUDPSocket&);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline size_t CUDPSocket::SendTo(const CBuffer& oBuffer, const in_addr& oAddr, uint nPort)
{
	return SendTo(oBuffer.Buffer(), oBuffer.Size(), oAddr, nPort);
}

inline size_t CUDPSocket::RecvFrom(CBuffer& oBuffer, in_addr& oAddr, uint& nPort)
{
	return RecvFrom(oBuffer.Buffer(), oBuffer.Size(), oAddr, nPort);
}

#endif // UDPSOCKET_HPP
