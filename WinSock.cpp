/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		WINSOCK.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CWinSock class definition.
**
*******************************************************************************
*/

#include "ncl.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

// Directive to link to WinSock library.
#pragma comment(lib, "ws2_32")
//#pragma comment(lib, "wsock32")

// For message only windows.
#ifndef HWND_MESSAGE
#define HWND_MESSAGE     ((HWND)-3)
#endif

/******************************************************************************
**
** Class members.
**
*******************************************************************************
*/

bool    CWinSock::g_bStarted = false;
WSADATA CWinSock::g_oWSAData = { 0 };
uint	CWinSock::g_nSockMsg = 0;
HWND	CWinSock::g_hSockWnd = NULL;

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
	g_nSockMsg = ::RegisterWindowMessage("WM_WSAASYNCSELECT");

	ASSERT(g_nSockMsg != 0);

	WNDCLASS oWndClass = { 0 };

	oWndClass.lpfnWndProc   = WindowProc;
	oWndClass.hInstance     = CModule::This().Handle();
	oWndClass.lpszClassName = "WinSockClass";

	// Register the socket window class.
	::RegisterClass(&oWndClass);

	// Create the socket window.
	g_hSockWnd = ::CreateWindowEx(0, "WinSockClass", "WinSock Message Window",
								WS_OVERLAPPED, 0, 0, 0, 0, HWND_MESSAGE, NULL,
								CModule::This().Handle(), NULL);

	ASSERT(g_hSockWnd != NULL);

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
		case WSABASEERR:			return "WSABASEERR";
		case WSAEINTR:				return "WSAEINTR";
		case WSAEBADF:				return "WSAEBADF";
		case WSAEACCES:				return "WSAEACCES";
		case WSAEFAULT:				return "WSAEFAULT";
		case WSAEINVAL:				return "WSAEINVAL";
		case WSAEMFILE:				return "WSAEMFILE";
		case WSAEWOULDBLOCK:		return "WSAEWOULDBLOCK";
		case WSAEINPROGRESS:		return "WSAEINPROGRESS";
		case WSAEALREADY:			return "WSAEALREADY";
		case WSAENOTSOCK:			return "WSAENOTSOCK";
		case WSAEDESTADDRREQ:		return "WSAEDESTADDRREQ";
		case WSAEMSGSIZE:			return "WSAEMSGSIZE";
		case WSAEPROTOTYPE:			return "WSAEPROTOTYPE";
		case WSAENOPROTOOPT:		return "WSAENOPROTOOPT";
		case WSAEPROTONOSUPPORT:	return "WSAEPROTONOSUPPORT";
		case WSAESOCKTNOSUPPORT:	return "WSAESOCKTNOSUPPORT";
		case WSAEOPNOTSUPP:			return "WSAEOPNOTSUPP";
		case WSAEPFNOSUPPORT:		return "WSAEPFNOSUPPORT";
		case WSAEAFNOSUPPORT:		return "WSAEAFNOSUPPORT";
		case WSAEADDRINUSE:			return "WSAEADDRINUSE";
		case WSAEADDRNOTAVAIL:		return "WSAEADDRNOTAVAIL";
		case WSAENETDOWN:			return "WSAENETDOWN";
		case WSAENETUNREACH:		return "WSAENETUNREACH";
		case WSAENETRESET:			return "WSAENETRESET";
		case WSAECONNABORTED:		return "WSAECONNABORTED";
		case WSAECONNRESET:			return "WSAECONNRESET";
		case WSAENOBUFS:			return "WSAENOBUFS";
		case WSAEISCONN:			return "WSAEISCONN";
		case WSAENOTCONN:			return "WSAENOTCONN";
		case WSAESHUTDOWN:			return "WSAESHUTDOWN";
		case WSAETOOMANYREFS:		return "WSAETOOMANYREFS";
		case WSAETIMEDOUT:			return "WSAETIMEDOUT";
		case WSAECONNREFUSED:		return "WSAECONNREFUSED";
		case WSAELOOP:				return "WSAELOOP";
		case WSAENAMETOOLONG:		return "WSAENAMETOOLONG";
		case WSAEHOSTDOWN:			return "WSAEHOSTDOWN";
		case WSAEHOSTUNREACH:		return "WSAEHOSTUNREACH";
		case WSAENOTEMPTY:			return "WSAENOTEMPTY";
		case WSAEPROCLIM:			return "WSAEPROCLIM";
		case WSAEUSERS:				return "WSAEUSERS";
		case WSAEDQUOT:				return "WSAEDQUOT";
		case WSAESTALE:				return "WSAESTALE";
		case WSAEREMOTE:			return "WSAEREMOTE";
		case WSAEDISCON:			return "WSAEDISCON";
		case WSASYSNOTREADY:		return "WSASYSNOTREADY";
		case WSAVERNOTSUPPORTED:	return "WSAVERNOTSUPPORTED";
		case WSANOTINITIALISED:		return "WSANOTINITIALISED";
		case WSAHOST_NOT_FOUND:		return "WSAHOST_NOT_FOUND";
		case WSATRY_AGAIN:			return "WSATRY_AGAIN";
		case WSANO_RECOVERY:		return "WSANO_RECOVERY";
		case WSANO_DATA:			return "WSANO_DATA";
	}

	char szError[50] = "#";

	// Unrecognised, format as a number.
	itoa(nError, szError+1, 10);

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
#ifdef _DEBUG
		SOCKET hSocket = wParam;
		int    nEvent  = WSAGETSELECTEVENT(lParam);
		int    nError  = WSAGETSELECTERROR(lParam);

		TRACE3("Socket message: 0x%08X %u %u\n", hSocket, nEvent, nError);
#endif

		return 0;
	}

	// Do default processing.
	return ::DefWindowProc(hWnd, nMsg, wParam, lParam);
}
