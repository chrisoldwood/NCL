/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDECLIENT.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CDDEClient class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "DDEClient.hpp"
#include <WCL/StrArray.hpp>
#include "DDEException.hpp"
#include "DDEString.hpp"
#include "DDECltConv.hpp"
#include "IDDEClientListener.hpp"
#include "DDEData.hpp"

////////////////////////////////////////////////////////////////////////////////
// Constants

//! The maximum length of the server name in chars.
static const size_t MAX_SERVER_LEN = 256;

//! The maximum length of the topic name in chars.
static const size_t MAX_TOPIC_LEN = 256;

/******************************************************************************
**
** Class members.
**
*******************************************************************************
*/

// The single DDE Client object.
CDDEClient* CDDEClient::g_pDDEClient = NULL;

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

CDDEClient::CDDEClient(DWORD dwFlags)
{
	m_eType = CLIENT;

	Initialise(dwFlags);

	ASSERT(g_pDDEClient == NULL);
	g_pDDEClient = this;
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

CDDEClient::~CDDEClient()
{
	ASSERT(g_pDDEClient == this);
	g_pDDEClient = NULL;

	Uninitialise();

	ASSERT(m_aoConvs.Size() == 0);
	ASSERT(m_aoListeners.Size() == 0);
}

/******************************************************************************
** Method:		Initialise()
**
** Description:	Initialise the DDE Client with the DDEML libray.
**
** Parameters:	dwFlags		The additional DDEML initialisation flags.
**
** Returns:		Nothing.
**
** Exceptions:	CDDEException.
**
*******************************************************************************
*/

void CDDEClient::Initialise(DWORD dwFlags)
{
	// Register with DDEML.
	UINT nResult = ::DdeInitialize(&m_dwInst, DDECallbackProc, APPCMD_CLIENTONLY | dwFlags, 0);

	// Failed?
	if (nResult != DMLERR_NO_ERROR)
		throw CDDEException(CDDEException::E_INIT_FAILED, ::DdeGetLastError(m_dwInst));
}

/******************************************************************************
** Method:		Uninitialise()
**
** Description:	Uninitialise the DDE Client with the DDEML libray.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEClient::Uninitialise()
{
	// Cleanup.
	if (m_dwInst != 0)
		::DdeUninitialize(m_dwInst);

	// Reset members.
	m_dwInst = 0;
	m_aoConvs.RemoveAll();
	m_aoListeners.RemoveAll();
}

/******************************************************************************
** Method:		CreateConversation()
**
** Description:	Create a conversation with a DDE service and topic.
**
** Parameters:	pszService	The service name.
**				pszTopic	The topic name.
**
** Returns:		The conversation.
**
** Exceptions:	CDDEException.
**
*******************************************************************************
*/

CDDECltConv* CDDEClient::CreateConversation(const tchar* pszService, const tchar* pszTopic)
{
	ASSERT(pszService != NULL);
	ASSERT(pszTopic   != NULL);
	ASSERT(m_dwInst   != NULL);

	// Already connected?
	CDDECltConv* pConv = FindConversation(pszService, pszTopic);

	if (pConv == NULL)
	{
		CDDEString strService(this, pszService);
		CDDEString strTopic(this, pszTopic);

		// Attempt to connect to the service/topic.
		HCONV hConv = ::DdeConnect(m_dwInst, strService, strTopic, NULL);

		// Connect failed?
		if (hConv == NULL)
			throw CDDEException(CDDEException::E_CONN_FAILED, ::DdeGetLastError(m_dwInst));

		// Create conversation.
		pConv = new CDDECltConv(this, hConv, pszService, pszTopic);

		// Add to collection.
		m_aoConvs.Add(pConv);
	}

	// New reference.
	++pConv->m_nRefCount;

	return pConv;
}

/******************************************************************************
** Method:		DestroyConversation()
**
** Description:	Destroy an existing conversation.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEClient::DestroyConversation(CDDECltConv* pConv)
{
	ASSERT(pConv != NULL);
	ASSERT(m_aoConvs.Find(pConv) != -1);

	// Last reference?
	if (--pConv->m_nRefCount == 0)
	{
		// Disconnect from service/topic.
		pConv->Disconnect();

		// Remove from collection.
		m_aoConvs.Remove(m_aoConvs.Find(pConv));

		// Delete conversation.
		delete pConv;
	}
}

/******************************************************************************
** Method:		FindConversation()
**
** Description:	Find an existing conversation.
**
** Parameters:	pszService	The service name.
**				pszTopic	The topic name.
**				hConv		The conversation handle.
**
** Returns:		The conversation or NULL if not found.
**
*******************************************************************************
*/

CDDECltConv* CDDEClient::FindConversation(const tchar* pszService, const tchar* pszTopic) const
{
	ASSERT(pszService != NULL);
	ASSERT(pszTopic   != NULL);
	ASSERT(m_dwInst   != NULL);

	// Search the conversation list.
	for (size_t i = 0; i < m_aoConvs.Size(); ++i)
	{
		CDDECltConv* pConv = m_aoConvs[i];

		if ( (pConv->Service() == pszService) && (pConv->Topic() == pszTopic) )
			return pConv;
	}

	return NULL;
}

CDDECltConv* CDDEClient::FindConversation(HCONV hConv) const
{
	// Search the conversation list.
	for (size_t i = 0; i < m_aoConvs.Size(); ++i)
	{
		CDDECltConv* pConv = m_aoConvs[i];

		if (pConv->Handle() == hConv)
			return pConv;
	}

	return NULL;
}

/******************************************************************************
** Method:		AddListener()
**
** Description:	Add a DDE Client event listener.
**
** Parameters:	pListener	The listener.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEClient::AddListener(IDDEClientListener* pListener)
{
	ASSERT(pListener != NULL);

	m_aoListeners.Add(pListener);
}

/******************************************************************************
** Method:		RemoveListener()
**
** Description:	Remove a DDE Client event listener.
**
** Parameters:	pListener	The listener.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEClient::RemoveListener(IDDEClientListener* pListener)
{
	ASSERT(pListener != NULL);

	int i = m_aoListeners.Find(pListener);

	if (i != -1)
		m_aoListeners.Remove(i);
}

/******************************************************************************
** Method:		OnRegister()
**
** Description:	A new DDE service has been registered.
**
** Parameters:	pszBaseName		The base service name.
**				pszInstName		The instance service name.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEClient::OnRegister(const tchar* pszBaseName, const tchar* pszInstName)
{
	for (size_t i = 0; i < m_aoListeners.Size(); ++i)
		m_aoListeners[i]->OnRegister(pszBaseName, pszInstName);
}

/******************************************************************************
** Method:		OnUnregister()
**
** Description:	A DDE service has been unregistered.
**
** Parameters:	pszBaseName		The base service name.
**				pszInstName		The instance service name.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEClient::OnUnregister(const tchar* pszBaseName, const tchar* pszInstName)
{
	for (size_t i = 0; i < m_aoListeners.Size(); ++i)
		m_aoListeners[i]->OnUnregister(pszBaseName, pszInstName);
}

/******************************************************************************
** Method:		OnDisconnect()
**
** Description:	A conversation was terminated by the server.
**
** Parameters:	hConv	The conversation.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEClient::OnDisconnect(HCONV hConv)
{
	// Find the conversation from the handle.
	CDDECltConv* pConv = FindConversation(hConv);

	ASSERT(pConv != NULL);

	for (size_t i = 0; i < m_aoListeners.Size(); ++i)
		m_aoListeners[i]->OnDisconnect(pConv);
}

/******************************************************************************
** Method:		OnAdvise()
**
** Description:	A linked has been updated by the server.
**
** Parameters:	hConv		The conversation.
**				pszTopic	The topic name.
**				pszItem		The item name.
**				nFormat		The clipboard format.
**				pData		The data.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEClient::OnAdvise(HCONV hConv, const tchar* /*pszTopic*/, const tchar* pszItem, uint nFormat, const CDDEData* pData)
{
	// Find the conversation for the handle.
	CDDECltConv* pConv = FindConversation(hConv);

	ASSERT(pConv != NULL);

	// Find the link.
	CDDELink* pLink = pConv->FindLink(pszItem, nFormat);

	// May still be in the Advise Start.
	if (pLink == NULL)
	{
//		TRACE3("OnAdvise('%s|%s', '%s') - Failed to find link\n", pConv->Service(), pConv->Topic(), pszItem);
		return;
	}

	for (size_t i = 0; i < m_aoListeners.Size(); ++i)
		m_aoListeners[i]->OnAdvise(pLink, pData);
}

/******************************************************************************
** Method:		QueryServers()
**
** Description:	Find all available DDE servers.
**
** Parameters:	astrServers		The list of returned server names.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEClient::QueryServers(CStrArray& astrServers) const
{
	// Query all servers.
	HCONVLIST hList = ::DdeConnectList(m_dwInst, NULL, NULL, NULL, NULL);

	// Failed?
	if (hList == NULL)
		throw CDDEException(CDDEException::E_QUERY_FAILED, LastError());

	HCONV hConv = NULL;

	// For all servers...
	while ((hConv = ::DdeQueryNextServer(hList, hConv)) != NULL)
	{
		CONVINFO oConvInfo = { sizeof(oConvInfo), 0 };

		// Query server details.
		if (!::DdeQueryConvInfo(hConv, QID_SYNC, &oConvInfo))
		{
			::DdeDisconnectList(hList);
			throw CDDEException(CDDEException::E_QUERY_FAILED, LastError());
		}

		const size_t MAX_LEN = 256;

		tchar szServer[MAX_LEN];

		// Get the server name.
		if (!::DdeQueryString(m_dwInst, oConvInfo.hszSvcPartner, szServer, MAX_LEN, CP_WIN_TCHAR))
		{
			::DdeDisconnectList(hList);
			throw CDDEException(CDDEException::E_QUERY_FAILED, LastError());
		}

		// Add, if not a duplicate.
		if (astrServers.Find(szServer, true) == -1)
			astrServers.Add(szServer);
	}

	// Free connection list.
	::DdeDisconnectList(hList);
}

/******************************************************************************
** Method:		QueryServerTopics()
**
** Description:	Find all topics available a DDE server.
**
** Parameters:	strServer		The DDE server to query.
**				astrTopics		The list of returned topics names.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEClient::QueryServerTopics(const tchar* pszServer, CStrArray& astrTopics) const
{
	ASSERT(pszServer != NULL);

	CDDEString strServer(g_pDDEClient, pszServer);

	// Query all servers.
	HCONVLIST hList = ::DdeConnectList(m_dwInst, strServer, NULL, NULL, NULL);

	// Failed?
	if (hList == NULL)
		throw CDDEException(CDDEException::E_QUERY_FAILED, LastError());

	HCONV hConv = NULL;

	// For all servers...
	while ((hConv = ::DdeQueryNextServer(hList, hConv)) != NULL)
	{
		CONVINFO oConvInfo = { sizeof(oConvInfo), 0 };

		// Query server details.
		if (!::DdeQueryConvInfo(hConv, QID_SYNC, &oConvInfo))
		{
			::DdeDisconnectList(hList);
			throw CDDEException(CDDEException::E_QUERY_FAILED, LastError());
		}

		tchar szServer[MAX_SERVER_LEN+1] = { 0 };

		// Get the server name.
		// NB: Some servers will return all service names regardless.
		if (!::DdeQueryString(m_dwInst, oConvInfo.hszSvcPartner, szServer, MAX_SERVER_LEN, CP_WIN_TCHAR))
		{
			::DdeDisconnectList(hList);
			throw CDDEException(CDDEException::E_QUERY_FAILED, LastError());
		}

		// Not the server we're after?
		if (tstricmp(szServer, pszServer) != 0)
			continue;

		tchar szTopic[MAX_TOPIC_LEN] = { 0 };

		// Get the topic name.
		if (!::DdeQueryString(m_dwInst, oConvInfo.hszTopic, szTopic, MAX_TOPIC_LEN, CP_WIN_TCHAR))
		{
			::DdeDisconnectList(hList);
			throw CDDEException(CDDEException::E_QUERY_FAILED, LastError());
		}

		astrTopics.Add(szTopic);
	}

	// Free connection list.
	::DdeDisconnectList(hList);
}

////////////////////////////////////////////////////////////////////////////////
//! Query for all running servers and topics.

void CDDEClient::QueryAll(CStrArray& astrServers, CStrArray& astrTopics) const
{
	// Query all servers.
	HCONVLIST hList = ::DdeConnectList(m_dwInst, NULL, NULL, NULL, NULL);

	// Failed?
	if (hList == NULL)
		throw CDDEException(CDDEException::E_QUERY_FAILED, LastError());

	HCONV hConv = NULL;

	// For all servers & topics...
	while ((hConv = ::DdeQueryNextServer(hList, hConv)) != NULL)
	{
		CONVINFO oConvInfo = { sizeof(oConvInfo), 0 };

		// Query server details.
		if (!::DdeQueryConvInfo(hConv, QID_SYNC, &oConvInfo))
		{
			::DdeDisconnectList(hList);
			throw CDDEException(CDDEException::E_QUERY_FAILED, LastError());
		}

		tchar szServer[MAX_SERVER_LEN+1] = { 0 };

		// Get the server name.
		if (!::DdeQueryString(m_dwInst, oConvInfo.hszSvcPartner, szServer, MAX_SERVER_LEN, CP_WIN_TCHAR))
		{
			::DdeDisconnectList(hList);
			throw CDDEException(CDDEException::E_QUERY_FAILED, LastError());
		}

		tchar szTopic[MAX_TOPIC_LEN+1] = { 0 };

		// Get the topic name.
		if (!::DdeQueryString(m_dwInst, oConvInfo.hszTopic, szTopic, MAX_TOPIC_LEN, CP_WIN_TCHAR))
		{
			::DdeDisconnectList(hList);
			throw CDDEException(CDDEException::E_QUERY_FAILED, LastError());
		}

		// Add to the collections.
		astrServers.Add(szServer);
		astrTopics.Add(szTopic);
	}

	// Free connection list.
	::DdeDisconnectList(hList);
}

/******************************************************************************
** Method:		DDECallbackProc()
**
** Description:	The callback function used by the DDEML library.
**
** Parameters:	See DdeCallback.
**
** Returns:		See DdeCallback.
**
*******************************************************************************
*/

HDDEDATA CALLBACK CDDEClient::DDECallbackProc(UINT uType, UINT uFormat, HCONV hConv, HSZ hsz1, HSZ hsz2, HDDEDATA hData, DWORD /*dwData1*/, DWORD /*dwData2*/)
{
	ASSERT(g_pDDEClient != NULL);

	HDDEDATA hResult = NULL;

	// Forward to handler.
	switch (uType)
	{
		// Service registered?
		case XTYP_REGISTER:
		{
			CDDEString strBaseName(g_pDDEClient, hsz1);
			CDDEString strInstName(g_pDDEClient, hsz2);

			g_pDDEClient->OnRegister(strBaseName, strInstName);
		}
		break;

		// Service unregistered?
		case XTYP_UNREGISTER:
		{
			CDDEString strBaseName(g_pDDEClient, hsz1);
			CDDEString strInstName(g_pDDEClient, hsz2);

			g_pDDEClient->OnUnregister(strBaseName, strInstName);
		}
		break;

		// Conversation terminated?
		case XTYP_DISCONNECT:
		{
			g_pDDEClient->OnDisconnect(hConv);
		}
		break;

		// Link updated?
		case XTYP_ADVDATA:
		{
			CDDEString strTopic(g_pDDEClient, hsz1);
			CDDEString strItem(g_pDDEClient, hsz2);

			if (hData != NULL)
			{
				CDDEData oData(g_pDDEClient, hData, uFormat, false);

				g_pDDEClient->OnAdvise(hConv, strTopic, strItem, uFormat, &oData);
			}
			else
			{
				g_pDDEClient->OnAdvise(hConv, strTopic, strItem, uFormat, NULL);
			}

			hResult = (HDDEDATA)DDE_FACK;
		}
		break;

		// Unknown message.
		default:
		{
			TRACE1(TXT("DDECallbackProc(0x%08X)\n"), uType);
			ASSERT_FALSE();
		}
		break;
	}

	return hResult;
}
