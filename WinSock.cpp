/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		WINSOCK.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CWinSock class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "WinSock.hpp"
#include <WCL/Module.hpp>
#include "Socket.hpp"
#include "SocketException.hpp"
#include <tchar.h>
#include <limits>

// Linker directive.
#pragma comment(lib, "ws2_32")

/******************************************************************************
**
** Class members.
**
*******************************************************************************
*/

bool    CWinSock::g_bStarted = false;
WSADATA CWinSock::g_oWSAData = { 0 };
uint    CWinSock::g_nSockMsg = 0;
HWND    CWinSock::g_hSockWnd = NULL;
CWinSock::SocketMapPtr CWinSock::g_pSockMap;

/******************************************************************************
** Method:		Startup()
**
** Description:	Iniitalise the WinSock library.
**
** Parameters:	nMajorVer, nMinorVer	The expected version number.
**
** Returns:		See WSAStartup().
**
*******************************************************************************
*/

int CWinSock::Startup(uint nMajorVer, uint nMinorVer)
{
	g_bStarted = true;

	// Get an ID for the socket messages.
	g_nSockMsg = ::RegisterWindowMessage(TXT("WM_WSAASYNCSELECT"));

	ASSERT(g_nSockMsg != 0);

	WNDCLASS oWndClass = { 0 };

	oWndClass.lpfnWndProc   = WindowProc;
	oWndClass.hInstance     = CModule::This().Handle();
	oWndClass.lpszClassName = TXT("WinSockClass");

	// Register the socket window class.
	::RegisterClass(&oWndClass);

	// Create the socket window.
	g_hSockWnd = ::CreateWindowEx(0, TXT("WinSockClass"), TXT("WinSock Message Window"),
								WS_OVERLAPPED, 0, 0, 0, 0, HWND_DESKTOP, NULL,
								CModule::This().Handle(), NULL);

	ASSERT(g_hSockWnd != NULL);

	// Create the socket handle map.
	g_pSockMap = SocketMapPtr(new SocketMap);

	return ::WSAStartup(MAKEWORD(nMajorVer, nMinorVer), &g_oWSAData);
}

/******************************************************************************
** Method:		Cleanup()
**
** Description:	Shutdown the WinSock library.
**
** Parameters:	None.
**
** Returns:		See WSACleanup().
**
*******************************************************************************
*/

int CWinSock::Cleanup()
{
	ASSERT((g_pSockMap.Get() == nullptr) || (g_pSockMap->empty()));

	// Destroy the socket handle map.
	g_pSockMap.Reset();

	// Destroy the socket window.
	if (g_hSockWnd != NULL)
		::DestroyWindow(g_hSockWnd);

	return ::WSACleanup();
}

/******************************************************************************
** Method:		ErrorToSymbol()
**
** Description:	Convert the error code to its C++ #defined symbol.
**
** Parameters:	nError		The error.
**
** Returns:		The symbol or the error number, if unrecognised.
**
*******************************************************************************
*/

CString CWinSock::ErrorToSymbol(int nError)
{
	// Known value?
	switch (nError)
	{
		case WSABASEERR:			return TXT("WSABASEERR");
		case WSAEINTR:				return TXT("WSAEINTR");
		case WSAEBADF:				return TXT("WSAEBADF");
		case WSAEACCES:				return TXT("WSAEACCES");
		case WSAEFAULT:				return TXT("WSAEFAULT");
		case WSAEINVAL:				return TXT("WSAEINVAL");
		case WSAEMFILE:				return TXT("WSAEMFILE");
		case WSAEWOULDBLOCK:		return TXT("WSAEWOULDBLOCK");
		case WSAEINPROGRESS:		return TXT("WSAEINPROGRESS");
		case WSAEALREADY:			return TXT("WSAEALREADY");
		case WSAENOTSOCK:			return TXT("WSAENOTSOCK");
		case WSAEDESTADDRREQ:		return TXT("WSAEDESTADDRREQ");
		case WSAEMSGSIZE:			return TXT("WSAEMSGSIZE");
		case WSAEPROTOTYPE:			return TXT("WSAEPROTOTYPE");
		case WSAENOPROTOOPT:		return TXT("WSAENOPROTOOPT");
		case WSAEPROTONOSUPPORT:	return TXT("WSAEPROTONOSUPPORT");
		case WSAESOCKTNOSUPPORT:	return TXT("WSAESOCKTNOSUPPORT");
		case WSAEOPNOTSUPP:			return TXT("WSAEOPNOTSUPP");
		case WSAEPFNOSUPPORT:		return TXT("WSAEPFNOSUPPORT");
		case WSAEAFNOSUPPORT:		return TXT("WSAEAFNOSUPPORT");
		case WSAEADDRINUSE:			return TXT("WSAEADDRINUSE");
		case WSAEADDRNOTAVAIL:		return TXT("WSAEADDRNOTAVAIL");
		case WSAENETDOWN:			return TXT("WSAENETDOWN");
		case WSAENETUNREACH:		return TXT("WSAENETUNREACH");
		case WSAENETRESET:			return TXT("WSAENETRESET");
		case WSAECONNABORTED:		return TXT("WSAECONNABORTED");
		case WSAECONNRESET:			return TXT("WSAECONNRESET");
		case WSAENOBUFS:			return TXT("WSAENOBUFS");
		case WSAEISCONN:			return TXT("WSAEISCONN");
		case WSAENOTCONN:			return TXT("WSAENOTCONN");
		case WSAESHUTDOWN:			return TXT("WSAESHUTDOWN");
		case WSAETOOMANYREFS:		return TXT("WSAETOOMANYREFS");
		case WSAETIMEDOUT:			return TXT("WSAETIMEDOUT");
		case WSAECONNREFUSED:		return TXT("WSAECONNREFUSED");
		case WSAELOOP:				return TXT("WSAELOOP");
		case WSAENAMETOOLONG:		return TXT("WSAENAMETOOLONG");
		case WSAEHOSTDOWN:			return TXT("WSAEHOSTDOWN");
		case WSAEHOSTUNREACH:		return TXT("WSAEHOSTUNREACH");
		case WSAENOTEMPTY:			return TXT("WSAENOTEMPTY");
		case WSAEPROCLIM:			return TXT("WSAEPROCLIM");
		case WSAEUSERS:				return TXT("WSAEUSERS");
		case WSAEDQUOT:				return TXT("WSAEDQUOT");
		case WSAESTALE:				return TXT("WSAESTALE");
		case WSAEREMOTE:			return TXT("WSAEREMOTE");
		case WSAEDISCON:			return TXT("WSAEDISCON");
		case WSASYSNOTREADY:		return TXT("WSASYSNOTREADY");
		case WSAVERNOTSUPPORTED:	return TXT("WSAVERNOTSUPPORTED");
		case WSANOTINITIALISED:		return TXT("WSANOTINITIALISED");
		case WSAHOST_NOT_FOUND:		return TXT("WSAHOST_NOT_FOUND");
		case WSATRY_AGAIN:			return TXT("WSATRY_AGAIN");
		case WSANO_RECOVERY:		return TXT("WSANO_RECOVERY");
		case WSANO_DATA:			return TXT("WSANO_DATA");
	}

	tchar szError[std::numeric_limits<int>::digits10+3] = TXT("#");

	// Unrecognised, format as a number.
	_itot(nError, szError+1, 10);

	return szError;
}

/******************************************************************************
** Method:		WindowProc()
**
** Description:	The socket window callback function.
**
** Parameters:	See WindowProc().
**
** Returns:		See WindowProc().
**
*******************************************************************************
*/

LRESULT CALLBACK CWinSock::WindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	// Is a socket message?
	if (nMsg == g_nSockMsg)
	{
		ASSERT(g_pSockMap.Get() != nullptr);

		// Extract message details.
		SOCKET hSocket = wParam;
		int    nEvent  = WSAGETSELECTEVENT(lParam);
		int    nError  = WSAGETSELECTERROR(lParam);

		try
		{
//			TRACE3("Socket message: 0x%08X %u %u\n", hSocket, nEvent, nError);

			SocketMap::const_iterator it = g_pSockMap->find(hSocket);

			// Socket mapped?
			if (it != g_pSockMap->end())
			{
				CSocket* pSocket = it->second;

				ASSERT(pSocket != NULL);

				// Forward event.
				pSocket->OnAsyncSelect(nEvent, nError);

			}

			return 0;
		}
		catch (const Core::Exception& e)
		{
			WCL::ReportUnhandledException(	TXT("Unexpected exception caught in CWinSock::WindowProc()\n\n")
											TXT("Message: Event=0x%08X, Error=0x%08X\n\n%s"),
											nEvent, nError, e.What());
		}
		catch (const std::exception& e)
		{
			WCL::ReportUnhandledException(	TXT("Unexpected exception caught in CWinSock::WindowProc()\n\n")
											TXT("Message: Event=0x%08X, Error=0x%08X\n\n%hs"),
											nEvent, nError, e.what());
		}
		catch (...)
		{
			WCL::ReportUnhandledException(	TXT("Unexpected unknown exception caught in DlgProc()\n\n")
											TXT("Message: Event=0x%08X, Error=0x%08X"),
											nEvent, nError);
		}
	}

	// Do default processing.
	return ::DefWindowProc(hWnd, nMsg, wParam, lParam);
}

/******************************************************************************
** Method:		BeginAsyncSelect()
**
** Description:	Switch a socket to non-blocking mode and add it to the socket
**				handle map.
**
** Parameters:	pSocket		The socket.
**				lEventMask	The events to notify.
**
** Returns:		Nothing.
**
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

void CWinSock::BeginAsyncSelect(CSocket* pSocket, long lEventMask)
{
	ASSERT(pSocket != nullptr);
	ASSERT(g_pSockMap.Get() != nullptr);

	// Get the socket handle.
	SOCKET hSocket = pSocket->Handle();

	ASSERT(hSocket != INVALID_SOCKET);

	// Add handle<->socket mapping.
	g_pSockMap->insert(std::make_pair(hSocket, pSocket));

	// Start events...
	if (::WSAAsyncSelect(hSocket, g_hSockWnd, g_nSockMsg, lEventMask) == SOCKET_ERROR)
		throw CSocketException(CSocketException::E_SELECT_FAILED, CWinSock::LastError());
}

/******************************************************************************
** Method:		EndAsyncSelect()
**
** Description:	The socket window callback function.
**
** Parameters:	pSocket		The socket.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CWinSock::EndAsyncSelect(CSocket* pSocket)
{
	ASSERT(pSocket != NULL);

	// Get the socket handle.
	SOCKET hSocket = pSocket->Handle();

	ASSERT(hSocket != INVALID_SOCKET);

	// Stop events.
	::WSAAsyncSelect(hSocket, g_hSockWnd, g_nSockMsg, 0);

	// Remove handle<->socket mapping.
	SocketMap::iterator it = g_pSockMap->find(hSocket);

	if (it != g_pSockMap->end())
		g_pSockMap->erase(it);
}

/******************************************************************************
** Method:		ProcessSocketMsgs()
**
** Description:	Process any socket messages in the message queue.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CWinSock::ProcessSocketMsgs()
{
	MSG oMsg;

	while (::PeekMessage(&oMsg, g_hSockWnd, g_nSockMsg, g_nSockMsg, PM_REMOVE))
		::DispatchMessage(&oMsg);
}
