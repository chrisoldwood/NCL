/******************************************************************************
**
** MODULE:		NCL.HPP
** COMPONENT:	Network & Comms Library.
** DESCRIPTION:	Wrapper to include all the library headers.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef NCL_HPP
#define NCL_HPP

/******************************************************************************
**
** Standard headers.
**
*******************************************************************************
*/

#include <Core/Common.hpp>		// Core library common headers.
#include <WCL/Common.hpp>		// Windows C++ library common headers.
#include <ddeml.h>

/******************************************************************************
**
** Library specific headers.
**
*******************************************************************************
*/

// Utility Classes.
#include "NetBuffer.hpp"

// DDE Classes.
#include "DDEInst.hpp"
#include "DDEException.hpp"
#include "DDEString.hpp"
#include "DDEData.hpp"
#include "IDDELinkData.hpp"
#include "DDELink.hpp"
#include "IDDEConvData.hpp"
#include "DDEConv.hpp"
#include "DDECltConv.hpp"
#include "DDESvrConv.hpp"
#include "IDDEClientListener.hpp"
#include "DefDDEClientListener.hpp"
#include "DDEClient.hpp"
#include "IDDEServerListener.hpp"
#include "DefDDEServerListener.hpp"
#include "DDEServer.hpp"

// Named Pipe Classes.
#include "PipeException.hpp"
#include "NamedPipe.hpp"
#include "ClientPipe.hpp"
#include "ServerPipe.hpp"

// Socket Classes.
#include "SocketException.hpp"
#include "IClientSocketListener.hpp"
#include "IServerSocketListener.hpp"
#include "Socket.hpp"
#include "TCPSocket.hpp"
#include "TCPCltSocket.hpp"
#include "TCPSvrSocket.hpp"
#include "UDPSocket.hpp"
#include "UDPCltSocket.hpp"
#include "UDPSvrSocket.hpp"
#include "WinSock.hpp"

#endif //NCL_HPP
