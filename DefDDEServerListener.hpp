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
	virtual bool OnConnect(const char* pszService, const char* pszTopic);
	virtual void OnConnectConfirm(CDDESvrConv* pConv);
	virtual void OnDisconnect(CDDESvrConv* pConv);
	virtual bool OnRequest(CDDESvrConv* pConv, const char* pszItem, uint nFormat, CDDEData& oData);
	virtual bool OnAdviseStart(CDDESvrConv* pConv, const char* pszItem, uint nFormat);
	virtual void OnAdviseConfirm(CDDESvrConv* pConv, CDDELink* pLink);
	virtual bool OnAdviseRequest(CDDESvrConv* pConv, CDDELink* pLink, CDDEData& oData);
	virtual void OnAdviseStop(CDDESvrConv* pConv, CDDELink* pLink);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

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

#endif // DEFDDESERVERLISTENER_HPP
