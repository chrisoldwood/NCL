/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SOCKET.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CSocket class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SOCKET_HPP
#define SOCKET_HPP

/******************************************************************************
** 
** The base class for all sockets.
**
*******************************************************************************
*/

class CSocket
{
public:
	virtual ~CSocket();

	//
	// Properties.
	//
	SOCKET Handle() const;
	bool   IsOpen() const;
	
	//
	// Methods.
	//
	virtual void Close();

protected:
	//
	// Members.
	//
	SOCKET	m_hSocket;

	// Protect creation etc.
	CSocket();
	CSocket(const CSocket&);
	void operator=(const CSocket&);

	//
	// Internal methods.
	//
	void Create(int nAF, int nType, int nProtocol);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline SOCKET CSocket::Handle() const
{
	return m_hSocket;
}

inline bool CSocket::IsOpen() const
{
	return (m_hSocket != INVALID_SOCKET);
}

#endif // SOCKET_HPP
