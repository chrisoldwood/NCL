/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		WINSOCK.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CWinSock class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef WINSOCK_HPP
#define WINSOCK_HPP

/******************************************************************************
** 
** Wrapper for the global WinSock functions.
**
*******************************************************************************
*/

class CWinSock
{
public:
	//
	// Methods.
	//
	static int Startup(uint nMajorVer, uint nMinorVer);
	static int Cleanup();

	static int     LastError();
	static CString ErrorToSymbol(int nError);

	static void BeginAsyncSelect(CSocket* pSocket, long lEventMask);
	static void EndAsyncSelect(CSocket* pSocket);

private:
	// Template shorthands.
	typedef TMap<SOCKET, CSocket*> CSocketMap;

	//
	// Class members.
	//
	static bool			g_bStarted;
	static WSADATA		g_oWSAData;
	static uint			g_nSockMsg;
	static HWND			g_hSockWnd;
	static CSocketMap*	g_pSockMap;

	// Socket window procedure.
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline int CWinSock::LastError()
{
	return ::WSAGetLastError();
}

#endif // WINSOCK_HPP
