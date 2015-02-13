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

#include "DDEFwd.hpp"
#include "IDDEClient.hpp"
#include "DDEInst.hpp"
#include <vector>

// Template shorthands.
typedef std::vector<CDDECltConv*> CDDECltConvs;

/******************************************************************************
** 
** This class provides DDE Client services.
**
*******************************************************************************
*/

class CDDEClient : public DDE::IDDEClient
                 , public CDDEInst
{
public:
	//! The default configuration flags.
	static const DWORD DEFAULT_FLAGS = CBF_FAIL_ALLSVRXACTIONS | CBF_SKIP_REGISTRATIONS | CBF_SKIP_UNREGISTRATIONS;

public:
	//! Constructor.
	CDDEClient(DWORD dwFlags = DEFAULT_FLAGS);

	//! Destructor.
	virtual ~CDDEClient();

	//
	// Properties.
	//

	//! Get the default transaction timeout (ms).
	DWORD DefaultTimeout() const;

	//! Set the default transaction timeout (ms).
	void SetDefaultTimeout(DWORD timeout);

	//
	// Conversation methods.
	//
	CDDECltConv* CreateConversation(const tchar* pszService, const tchar* pszTopic);
	void         DestroyConversation(CDDECltConv* pConv);
	CDDECltConv* FindConversation(const tchar* pszService, const tchar* pszTopic) const;
	CDDECltConv* FindConversation(HCONV hConv) const;
	size_t       GetNumConversations() const;
	size_t       GetAllConversations(CDDECltConvs& aoConvs) const;

	//
	// Event listener methods.
	//
	void AddListener(IDDEClientListener* pListener);
	void RemoveListener(IDDEClientListener* pListener);

	//
	// Server query methods.
	//
	void QueryServers(CStrArray& astrServers) const;
	void QueryServerTopics(const tchar* pszServer, CStrArray& astrTopics) const;

	//! Query for all running servers and topics.
	void QueryAll(CStrArray& astrServers, CStrArray& astrTopics) const;

protected:
	// Template shorthands.
	typedef std::vector<IDDEClientListener*> CListeners;

	//
	// Members.
	//
	DWORD			m_defaultTimeout;	//!< The default timeout for transactions (ms).
	CDDECltConvs	m_aoConvs;		// The list of conversations.
	CListeners		m_aoListeners;	// The list of event listeners.

	//
	// Initialisation methods.
	//
	void Initialise(DWORD dwFlags);
	void Uninitialise();

	//
	// DDECallback handlers.
	//
	void OnRegister(const tchar* pszBaseName, const tchar* pszInstName);
	void OnUnregister(const tchar* pszBaseName, const tchar* pszInstName);
	void OnDisconnect(HCONV hConv);
	void OnAdvise(HCONV hConv, const tchar* pszTopic, const tchar* pszItem, uint nFormat, const CDDEData* pData);

	// The DDE Callback function.
	static HDDEDATA CALLBACK DDECallbackProc(UINT uType, UINT uFormat, HCONV hConv, HSZ hsz1, HSZ hsz2, HDDEDATA hData, ULONG_PTR dwData1, ULONG_PTR dwData2);

	// The single DDE Client object.
	static CDDEClient* g_pDDEClient;
};

namespace DDE
{
//! The default CDDEClient smart-pointer type.
typedef Core::SharedPtr<CDDEClient> ClientPtr;
}

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

////////////////////////////////////////////////////////////////////////////////
//! Get the default transaction timeout (ms).

inline DWORD CDDEClient::DefaultTimeout() const
{
	return m_defaultTimeout;
}

////////////////////////////////////////////////////////////////////////////////
//! Set the default transaction timeout (ms).

inline void CDDEClient::SetDefaultTimeout(DWORD timeout)
{
	m_defaultTimeout = timeout;
}

inline size_t CDDEClient::GetNumConversations() const
{
	return m_aoConvs.size();
}

inline size_t CDDEClient::GetAllConversations(CDDECltConvs& aoConvs) const
{
	aoConvs = m_aoConvs;

	return aoConvs.size();
}

#endif // DDECLIENT_HPP
