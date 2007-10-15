/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDECLIENT.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CDDEClient class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef DDECLIENT_HPP
#define DDECLIENT_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "DDEInst.hpp"
#include <Legacy/TArray.hpp>

// Forward declarations.
class CDDECltConv;
class IDDEClientListener;
class CDDEData;

// Template shorthands.
typedef TPtrArray<CDDECltConv> CDDECltConvs;

/******************************************************************************
** 
** This class provides DDE Client services.
**
*******************************************************************************
*/

class CDDEClient : public CDDEInst
{
public:
	virtual ~CDDEClient();

	//
	// Singleton methods.
	//
	static CDDEClient* Instance();

	//
	// Accessors.
	//

	//
	// Initialisation methods.
	//
	void Initialise(DWORD dwFlags = CBF_FAIL_ALLSVRXACTIONS | CBF_SKIP_REGISTRATIONS | CBF_SKIP_UNREGISTRATIONS);
	void Uninitialise();

	//
	// Conversation methods.
	//
	CDDECltConv* CreateConversation(const char* pszService, const char* pszTopic);
	void         DestroyConversation(CDDECltConv* pConv);
	CDDECltConv* FindConversation(const char* pszService, const char* pszTopic) const;
	CDDECltConv* FindConversation(HCONV hConv) const;
	int          GetNumConversations() const;
	int          GetAllConversations(CDDECltConvs& aoConvs) const;

	//
	// Event listener methods.
	//
	void AddListener(IDDEClientListener* pListener);
	void RemoveListener(IDDEClientListener* pListener);

	//
	// Server query methods.
	//
	void QueryServers(CStrArray& astrServers) const;
	void QueryServerTopics(const char* pszServer, CStrArray& astrTopics) const;

	//! Query for all running servers and topics.
	void QueryAll(CStrArray& astrServers, CStrArray& astrTopics) const;

protected:
	// Template shorthands.
	typedef TPtrArray<IDDEClientListener> CListeners;

	//
	// Members.
	//
	CDDECltConvs	m_aoConvs;		// The list of conversations.
	CListeners		m_aoListeners;	// The list of event listeners.

	//
	// Constructors/Destructor.
	//
	CDDEClient();
	CDDEClient(const CDDEClient&);

	//
	// DDECallback handlers.
	//
	void OnRegister(const char* pszBaseName, const char* pszInstName);
	void OnUnregister(const char* pszBaseName, const char* pszInstName);
	void OnDisconnect(HCONV hConv);
	void OnAdvise(HCONV hConv, const char* pszTopic, const char* pszItem, uint nFormat, const CDDEData* pData);

	// The DDE Callback function.
	static HDDEDATA CALLBACK DDECallbackProc(UINT uType, UINT uFormat, HCONV hConv, HSZ hsz1, HSZ hsz2, HDDEDATA hData, DWORD dwData1, DWORD dwData2);

	// The single DDE Client object.
	static CDDEClient* g_pDDEClient;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline int CDDEClient::GetNumConversations() const
{
	return m_aoConvs.Size();
}

inline int CDDEClient::GetAllConversations(CDDECltConvs& aoConvs) const
{
	aoConvs.ShallowCopy(m_aoConvs);

	return aoConvs.Size();
}

#endif // DDECLIENT_HPP
