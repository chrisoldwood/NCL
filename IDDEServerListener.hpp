/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		IDDESERVERLISTENER.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The IDDEServerListener interface declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef IDDESERVERLISTENER_HPP
#define IDDESERVERLISTENER_HPP

/******************************************************************************
** 
** The callback interface for DDE Server events.
**
*******************************************************************************
*/

class IDDEServerListener
{
public:
	//
	// Methods.
	//
	virtual bool OnWildConnect(CStrArray& astrServices, CStrArray& astrTopics) = 0;
	virtual bool OnWildConnectService(const char* pszService, CStrArray& astrTopics) = 0;
	virtual bool OnWildConnectTopic(const char* pszTopic, CStrArray& astrServices) = 0;
	virtual bool OnConnect(const char* pszService, const char* pszTopic) = 0;
	virtual void OnConnectConfirm(CDDESvrConv* pConv) = 0;
	virtual void OnDisconnect(CDDESvrConv* pConv) = 0;
	virtual bool OnRequest(CDDESvrConv* pConv, const char* pszItem, uint nFormat, CDDEData& oData) = 0;
	virtual bool OnAdviseStart(CDDESvrConv* pConv, const char* pszItem, uint nFormat) = 0;
	virtual void OnAdviseConfirm(CDDESvrConv* pConv, CDDELink* pLink) = 0;
	virtual bool OnAdviseRequest(CDDESvrConv* pConv, CDDELink* pLink, CDDEData& oData) = 0;
	virtual void OnAdviseStop(CDDESvrConv* pConv, CDDELink* pLink) = 0;
	virtual bool OnExecute(CDDESvrConv* pConv, const CString& strCmd) = 0;
	virtual bool OnPoke(CDDESvrConv* pConv, const char* pszItem, uint nFormat, const CDDEData& oData) = 0;

protected:
	// Make interface.
	virtual ~IDDEServerListener() = 0 {}; 
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // IDDESERVERLISTENER_HPP
