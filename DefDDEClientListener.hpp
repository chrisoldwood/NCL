/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DEFDDECLIENTLISTENER.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CDefDDEClientListener class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef DEFDDECLIENTLISTENER_HPP
#define DEFDDECLIENTLISTENER_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "IDDEClientListener.hpp"

/******************************************************************************
** 
** Default implementation of IDDEClientListener.
**
*******************************************************************************
*/

class CDefDDEClientListener : public IDDEClientListener
{
public:
	//
	// Methods.
	//
	virtual void OnRegister(const tchar* pszBaseName, const tchar* pszInstName);
	virtual void OnUnregister(const tchar* pszBaseName, const tchar* pszInstName);
	virtual void OnDisconnect(CDDECltConv* pConv);
	virtual void OnAdvise(CDDELink* pLink, const CDDEData* pData);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline void CDefDDEClientListener::OnRegister(const tchar* /*pszBaseName*/, const tchar* /*pszInstName*/)
{
}

inline void CDefDDEClientListener::OnUnregister(const tchar* /*pszBaseName*/, const tchar* /*pszInstName*/)
{
}

inline void CDefDDEClientListener::OnDisconnect(CDDECltConv* /*pConv*/)
{
}

inline void CDefDDEClientListener::OnAdvise(CDDELink* /*pLink*/, const CDDEData* /*pData*/)
{
}

#endif // DEFDDECLIENTLISTENER_HPP
