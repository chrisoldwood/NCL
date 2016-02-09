/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDESERVER.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CDDEServer class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef DDESERVER_HPP
#define DDESERVER_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "DDEFwd.hpp"
#include "IDDEServer.hpp"
#include "DDEInst.hpp"
#include <vector>

// Forward declarations.
class CBuffer;

// Template shorthands.
typedef std::vector<CDDESvrConv*> CDDESvrConvs;

/******************************************************************************
**
** This class provides DDE Server services.
**
*******************************************************************************
*/

class CDDEServer : public DDE::IDDEServer
                 , public CDDEInst
{
public:
	//! The default configuration flags.
	static const DWORD DEFAULT_FLAGS = CBF_SKIP_REGISTRATIONS | CBF_SKIP_UNREGISTRATIONS;

public:
	//! Constructor.
	CDDEServer(DWORD dwFlags = DEFAULT_FLAGS);

	//! Destructor.
	virtual ~CDDEServer();

	//
	// Methods.
	//
	void Register(const tchar* pszService);
	void Unregister(const tchar* pszService);

	//
	// Conversation methods.
	//
	void         DestroyConversation(CDDESvrConv* pConv);
	CDDESvrConv* FindConversation(const tchar* pszService, const tchar* pszTopic) const;
	CDDESvrConv* FindConversation(HCONV hConv) const;
	size_t       GetNumConversations() const;
	size_t       GetAllConversations(CDDESvrConvs& aoConvs) const;

	//
	// Event listener methods.
	//
	void AddListener(IDDEServerListener* pListener);
	void RemoveListener(IDDEServerListener* pListener);

	//
	// Utility methods (initially public for testing)
	//
	static uint GuessTextFormat(const CBuffer& buffer);

protected:
	// Template shorthands.
	typedef std::vector<IDDEServerListener*> CListeners;

	//
	// Members.
	//
	CDDESvrConvs	m_aoConvs;		// The list of conversations.
	CListeners		m_aoListeners;	// The list of event listeners.

	//
	// Initialisation methods.
	//
	void Initialise(DWORD dwFlags);
	void Uninitialise();

	//
	// DDECallback handlers.
	//
	bool OnWildConnect(CStrArray& astrServices, CStrArray& astrTopics);
	bool OnWildConnectService(const tchar* pszService, CStrArray& astrTopics);
	bool OnWildConnectTopic(const tchar* pszTopic, CStrArray& astrServices);
	bool OnConnect(const tchar* pszService, const tchar* pszTopic);
	void OnConnectConfirm(HCONV hConv, const tchar* pszService, const tchar* pszTopic);
	void OnDisconnect(HCONV hConv);
	bool OnRequest(HCONV hConv, const tchar* pszItem, uint nFormat, CDDEData& oData);
	bool OnAdviseStart(HCONV hConv, const tchar* pszItem, uint nFormat);
	bool OnAdviseRequest(HCONV hConv, const tchar* pszItem, uint nFormat, CDDEData& oData);
	void OnAdviseStop(HCONV hConv, const tchar* pszItem, uint nFormat);
	bool OnExecute(HCONV hConv, const CDDEData& oData, uint nFormat);
	bool OnPoke(HCONV hConv, const tchar* pszItem, uint nFormat, const CDDEData& oData);

	//! The real DDE Callback function.
	static HDDEDATA CALLBACK DDECallbackProc(UINT uType, UINT uFormat, HCONV hConv, HSZ hsz1, HSZ hsz2, HDDEDATA hData, ULONG_PTR dwData1, ULONG_PTR dwData2);

	//! The DDE Callback function implementation.
	static HDDEDATA DDECallbackProcImpl(UINT uType, UINT uFormat, HCONV hConv, HSZ hsz1, HSZ hsz2, HDDEDATA hData);

	// The single DDE Client object.
	static CDDEServer* g_pDDEServer;
};

namespace DDE
{
//! The default CDDEServer smart-pointer type.
typedef Core::SharedPtr<CDDEServer> ServerPtr;
}

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline size_t CDDEServer::GetNumConversations() const
{
	return m_aoConvs.size();
}

inline size_t CDDEServer::GetAllConversations(CDDESvrConvs& aoConvs) const
{
	aoConvs = m_aoConvs;

	return aoConvs.size();
}

#endif // DDESERVER_HPP
