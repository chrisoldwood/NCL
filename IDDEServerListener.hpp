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
	virtual bool OnConnect(const char* pszService, const char* pszTopic) = 0;
	virtual void OnConnectConfirm(CDDESvrConv* pConv) = 0;
	virtual void OnDisconnect(CDDESvrConv* pConv) = 0;
	virtual bool OnRequest(CDDEConv* pConv, const char* pszItem, uint nFormat, CDDEData& oData) = 0;
	virtual bool OnAdviseStart(CDDEConv* pConv, const char* pszItem, uint nFormat) = 0;
	virtual void OnAdviseConfirm(CDDEConv* pConv, CDDELink* pLink) = 0;
	virtual void OnAdviseStop(CDDEConv* pConv, CDDELink* pLink) = 0;

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
