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

/******************************************************************************
** 
** A UDP type socket.
**
*******************************************************************************
*/

class CUDPSocket : public CSocket
{
public:
	//
	// Constructors/Destructor.
	//
	CUDPSocket();
	virtual ~CUDPSocket();
	
protected:
	//
	// Members.
	//
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // UDPSOCKET_HPP
