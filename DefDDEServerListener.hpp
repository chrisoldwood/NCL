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
	virtual bool OnRequest(CDDEConv* pConv, const char* pszItem, uint nFormat, CDDEData& oData);
	virtual bool OnAdviseStart(CDDEConv* pConv, const char* pszItem, uint nFormat);
	virtual void OnAdviseConfirm(CDDEConv* pConv, CDDELink* pLink);
	virtual void OnAdviseStop(CDDEConv* pConv, CDDELink* pLink);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline bool CDefDDEServerListener::OnConnect(const char* pszService, const char* pszTopic)
{
	return false;
}

inline void CDefDDEServerListener::OnConnectConfirm(CDDESvrConv* pConv)
{
}

inline void CDefDDEServerListener::OnDisconnect(CDDESvrConv* pConv)
{
}

inline bool CDefDDEServerListener::OnRequest(CDDEConv* pConv, const char* pszItem, uint nFormat, CDDEData& oData)
{
	return false;
}

inline bool CDefDDEServerListener::OnAdviseStart(CDDEConv* pConv, const char* pszItem, uint nFormat)
{
	return false;
}

inline void CDefDDEServerListener::OnAdviseConfirm(CDDEConv* pConv, CDDELink* pLink)
{
}

inline void CDefDDEServerListener::OnAdviseStop(CDDEConv* pConv, CDDELink* pLink)
{
}

#endif // DEFDDESERVERLISTENER_HPP
