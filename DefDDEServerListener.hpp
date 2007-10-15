/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DEFDDESERVERLISTENER.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CDefDDEServerListener class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef DEFDDESERVERLISTENER_HPP
#define DEFDDESERVERLISTENER_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "IDDEServerListener.hpp"

/******************************************************************************
** 
** Default implementation of IDDEServerListener.
**
*******************************************************************************
*/

class CDefDDEServerListener : public IDDEServerListener
{
public:
	//
	// Methods.
	//
	virtual bool OnWildConnect(CStrArray& astrServices, CStrArray& astrTopics);
	virtual bool OnWildConnectService(const char* pszService, CStrArray& astrTopics);
	virtual bool OnWildConnectTopic(const char* pszTopic, CStrArray& astrServices);
	virtual bool OnConnect(const char* pszService, const char* pszTopic);
	virtual void OnConnectConfirm(CDDESvrConv* pConv);
	virtual void OnDisconnect(CDDESvrConv* pConv);
	virtual bool OnRequest(CDDESvrConv* pConv, const char* pszItem, uint nFormat, CDDEData& oData);
	virtual bool OnAdviseStart(CDDESvrConv* pConv, const char* pszItem, uint nFormat);
	virtual void OnAdviseConfirm(CDDESvrConv* pConv, CDDELink* pLink);
	virtual bool OnAdviseRequest(CDDESvrConv* pConv, CDDELink* pLink, CDDEData& oData);
	virtual void OnAdviseStop(CDDESvrConv* pConv, CDDELink* pLink);
	virtual bool OnExecute(CDDESvrConv* pConv, const CString& strCmd);
	virtual bool OnPoke(CDDESvrConv* pConv, const char* pszItem, uint nFormat, const CDDEData& oData);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline bool CDefDDEServerListener::OnWildConnect(CStrArray& /*astrServices*/, CStrArray& /*astrTopics*/)
{
	return false;
}

inline bool CDefDDEServerListener::OnWildConnectService(const char* /*pszService*/, CStrArray& /*astrTopics*/)
{
	return false;
}

inline bool CDefDDEServerListener::OnWildConnectTopic(const char* /*pszTopic*/, CStrArray& /*astrServices*/)
{
	return false;
}

inline bool CDefDDEServerListener::OnConnect(const char* /*pszService*/, const char* /*pszTopic*/)
{
	return false;
}

inline void CDefDDEServerListener::OnConnectConfirm(CDDESvrConv* /*pConv*/)
{
}

inline void CDefDDEServerListener::OnDisconnect(CDDESvrConv* /*pConv*/)
{
}

inline bool CDefDDEServerListener::OnRequest(CDDESvrConv* /*pConv*/, const char* /*pszItem*/, uint /*nFormat*/, CDDEData& /*oData*/)
{
	return false;
}

inline bool CDefDDEServerListener::OnAdviseStart(CDDESvrConv* /*pConv*/, const char* /*pszItem*/, uint /*nFormat*/)
{
	return false;
}

inline void CDefDDEServerListener::OnAdviseConfirm(CDDESvrConv* /*pConv*/, CDDELink* /*pLink*/)
{
}

inline bool CDefDDEServerListener::OnAdviseRequest(CDDESvrConv* pConv, CDDELink* pLink, CDDEData& oData)
{
	return OnRequest(pConv, pLink->Item(), pLink->Format(), oData);
}

inline void CDefDDEServerListener::OnAdviseStop(CDDESvrConv* /*pConv*/, CDDELink* /*pLink*/)
{
}

inline bool CDefDDEServerListener::OnExecute(CDDESvrConv* /*pConv*/, const CString& /*strCmd*/)
{
	return false;
}

inline bool CDefDDEServerListener::OnPoke(CDDESvrConv* /*pConv*/, const char* /*pszItem*/, uint /*nFormat*/, const CDDEData& /*oData*/)
{
	return false;
}

#endif // DEFDDESERVERLISTENER_HPP
