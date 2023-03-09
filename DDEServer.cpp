/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDESERVER.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CDDEServer class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "DDEServer.hpp"
#include <WCL/StrArray.hpp>
#include "DDEException.hpp"
#include "DDESvrConv.hpp"
#include "DDEString.hpp"
#include "IDDEServerListener.hpp"
#include "DDEData.hpp"
#include <algorithm>
#include <malloc.h>
#include <WCL/Exception.hpp>

/******************************************************************************
**
** Class members.
**
*******************************************************************************
*/

// The single DDE Client object.
CDDEServer* CDDEServer::g_pDDEServer = NULL;

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

CDDEServer::CDDEServer(DWORD dwFlags)
	: m_aoConvs()
	, m_aoListeners()
{
	m_eType = SERVER;

	Initialise(dwFlags);

	ASSERT(g_pDDEServer == nullptr);
	g_pDDEServer = this;
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

CDDEServer::~CDDEServer()
{
	ASSERT(g_pDDEServer == this);
	g_pDDEServer = nullptr;

	Uninitialise();

	ASSERT(m_aoConvs.empty());
	ASSERT(m_aoListeners.empty());
}

/******************************************************************************
** Method:		Initialise()
**
** Description:	Initialise the DDE Server with the DDEML library.
**
** Parameters:	dwFlags		The additional DDEML initialisation flags.
**
** Returns:		Nothing.
**
** Exceptions:	CDDEException.
**
*******************************************************************************
*/

void CDDEServer::Initialise(DWORD dwFlags)
{
	// Register with DDEML.
	UINT nResult = ::DdeInitialize(&m_dwInst, DDECallbackProc, APPCLASS_STANDARD | dwFlags, 0);

	// Failed?
	if (nResult != DMLERR_NO_ERROR)
		throw CDDEException(CDDEException::E_INIT_FAILED, nResult);

	ASSERT(m_dwInst != 0);	// Not guaranteed, but highly likely.
}

/******************************************************************************
** Method:		Uninitialise()
**
** Description:	Uninitialise the DDE Client with the DDEML library.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEServer::Uninitialise()
{
	// Clean-up.
	if (m_dwInst != 0)
	{
		BOOL okay = ::DdeUninitialize(m_dwInst);

		ASSERT_RESULT(okay, okay != FALSE);
	}

	// Delete all conversations.
	for (size_t i = 0, n = m_aoConvs.size(); i != n; ++i)
		delete m_aoConvs[i];

	// Reset members.
	m_dwInst = 0;
	m_aoConvs.clear();
	m_aoListeners.clear();
}

/******************************************************************************
** Method:		Register()
**
** Description:	Register the service name.
**
** Parameters:	pszService	The service name.
**
** Returns:		Nothing.
**
** Exceptions:	CDDEException.
**
*******************************************************************************
*/

void CDDEServer::Register(const tchar* pszService)
{
	ASSERT(pszService != nullptr);

	CDDEString strService(this, pszService);

	// Try and register the service name.
	HDDEDATA hResult = ::DdeNameService(m_dwInst, strService, NULL, DNS_REGISTER);

	if (hResult == NULL)
		throw CDDEException(CDDEException::E_REG_FAILED, ::DdeGetLastError(m_dwInst));
}

/******************************************************************************
** Method:		Unregister()
**
** Description:	Unregister the service name.
**
** Parameters:	pszService	The service name.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEServer::Unregister(const tchar* pszService)
{
	ASSERT(pszService != nullptr);

	CDDEString strService(this, pszService);

	HDDEDATA hResult = ::DdeNameService(m_dwInst, strService, NULL, DNS_UNREGISTER);

	ASSERT_RESULT(hResult, hResult != 0);
}

/******************************************************************************
** Method:		DestroyConversation()
**
** Description:	Disconnects a conversation.
**
** Parameters:	pConv	The conversation.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEServer::DestroyConversation(CDDESvrConv* pConv)
{
	ASSERT(pConv != nullptr);
	ASSERT(std::find(m_aoConvs.begin(), m_aoConvs.end(), pConv) != m_aoConvs.end());

	// Disconnect from service/topic.
	pConv->Disconnect();

	// Remove from collection.
	m_aoConvs.erase(std::find(m_aoConvs.begin(), m_aoConvs.end(), pConv));

	// Delete conversation.
	delete pConv;
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
** Returns:		The conversation or nullptr if not found.
**
*******************************************************************************
*/

CDDESvrConv* CDDEServer::FindConversation(const tchar* pszService, const tchar* pszTopic) const
{
	ASSERT(pszService != nullptr);
	ASSERT(pszTopic   != nullptr);

	// Search the conversation list.
	for (size_t i = 0, n = m_aoConvs.size(); i != n; ++i)
	{
		CDDESvrConv* pConv = m_aoConvs[i];

		if ( (pConv->Service() == pszService) && (pConv->Topic() == pszTopic) )
			return pConv;
	}

	return nullptr;
}

CDDESvrConv* CDDEServer::FindConversation(HCONV hConv) const
{
	ASSERT(hConv != NULL);

	// Search the conversation list.
	for (size_t i = 0, n = m_aoConvs.size(); i != n; ++i)
	{
		CDDESvrConv* pConv = m_aoConvs[i];

		if (pConv->Handle() == hConv)
			return pConv;
	}

	return nullptr;
}


/******************************************************************************
** Method:		AddListener()
**
** Description:	Add a DDE Server event listener.
**
** Parameters:	pListener	The listener.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEServer::AddListener(IDDEServerListener* pListener)
{
	ASSERT(pListener != nullptr);

	m_aoListeners.push_back(pListener);
}

/******************************************************************************
** Method:		RemoveListener()
**
** Description:	Remove a DDE Server event listener.
**
** Parameters:	pListener	The listener.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEServer::RemoveListener(IDDEServerListener* pListener)
{
	ASSERT(pListener != nullptr);

	CListeners::iterator it = std::find(m_aoListeners.begin(), m_aoListeners.end(), pListener);

	if (it != m_aoListeners.end())
		m_aoListeners.erase(it);
}

/******************************************************************************
** Methods:		OnWildConnect*()
**
** Description:	Multiple connection request from a DDE Client. The DDE Client
**				will create a connection for each returned service & topic.
**				NB: It is accepted if any listener accepts it.
**
** Parameters:	pszService		The service name.
**				pszTopic		The topic name.
**				astrServices	The returned list of service names.
**				astrTopics		The returned list of topic names.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CDDEServer::OnWildConnect(CStrArray& astrServices, CStrArray& astrTopics)
{
	// Query all listeners...
	for (size_t i = 0, n = m_aoListeners.size(); i != n; ++i)
	{
		if (m_aoListeners[i]->OnWildConnect(astrServices, astrTopics))
			return true;
	}

	return false;
}

bool CDDEServer::OnWildConnectService(const tchar* pszService, CStrArray& astrTopics)
{
	ASSERT(pszService != nullptr);

	// Query all listeners...
	for (size_t i = 0, n = m_aoListeners.size(); i != n; ++i)
	{
		if (m_aoListeners[i]->OnWildConnectService(pszService, astrTopics))
			return true;
	}

	return false;
}

bool CDDEServer::OnWildConnectTopic(const tchar* pszTopic, CStrArray& astrServices)
{
	ASSERT(pszTopic != nullptr);

	// Query all listeners...
	for (size_t i = 0, n = m_aoListeners.size(); i != n; ++i)
	{
		if (m_aoListeners[i]->OnWildConnectTopic(pszTopic, astrServices))
			return true;
	}

	return false;
}

/******************************************************************************
** Method:		OnConnect()
**
** Description:	Connection request from a DDE Client.
**				NB: It is accepted if any listener accepts it.
**
** Parameters:	pszService	The service name.
**				pszTopic	The topic name.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CDDEServer::OnConnect(const tchar* pszService, const tchar* pszTopic)
{
	ASSERT(pszService != nullptr);
	ASSERT(pszTopic   != nullptr);

	// Query all listeners...
	for (size_t i = 0, n = m_aoListeners.size(); i != n; ++i)
	{
		if (m_aoListeners[i]->OnConnect(pszService, pszTopic))
			return true;
	}

	return false;
}

/******************************************************************************
** Method:		OnConnectConfirm()
**
** Description:	Connection established with a DDE Client.
**
** Parameters:	hConv		The conversation handle.
**				pszService	The service name.
**				pszTopic	The topic name.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEServer::OnConnectConfirm(HCONV hConv, const tchar* pszService, const tchar* pszTopic)
{
	ASSERT(hConv      != NULL);
	ASSERT(pszService != nullptr);
	ASSERT(pszTopic   != nullptr);

	// Allocate a new conversation and add to the collection.
	CDDESvrConv* pConv = new CDDESvrConv(this, hConv, pszService, pszTopic);

	m_aoConvs.push_back(pConv);

	// Notify listeners.
	for (size_t i = 0, n = m_aoListeners.size(); i != n; ++i)
		m_aoListeners[i]->OnConnectConfirm(pConv);
}

/******************************************************************************
** Method:		OnDisconnect()
**
** Description:	Connection terminated with a DDE Client.
**
** Parameters:	hConv		The conversation handle.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEServer::OnDisconnect(HCONV hConv)
{
	ASSERT(hConv != NULL);

	// Find the conversation from the handle.
	CDDESvrConv* pConv = FindConversation(hConv);

	ASSERT(pConv != nullptr);

	// Notify listeners.
	for (size_t i = 0, n = m_aoListeners.size(); i != n; ++i)
		m_aoListeners[i]->OnDisconnect(pConv);

	// Remove from collection and delete.
	m_aoConvs.erase(std::find(m_aoConvs.begin(), m_aoConvs.end(), pConv));
	delete pConv;
}

/******************************************************************************
** Method:		OnRequest()
**
** Description:	DDE Client requesting an item.
**
** Parameters:	hConv		The conversation handle.
**				pszItem		The item being requested.
**				nFormat		The data format.
**				oData		The data to return.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CDDEServer::OnRequest(HCONV hConv, const tchar* pszItem, uint nFormat, CDDEData& oData)
{
	ASSERT(hConv   != NULL);
	ASSERT(pszItem != nullptr);

	// Find the conversation from the handle.
	CDDESvrConv* pConv = FindConversation(hConv);

	ASSERT(pConv != nullptr);

	// Query all listeners...
	for (size_t i = 0, n = m_aoListeners.size(); i != n; ++i)
	{
		if (m_aoListeners[i]->OnRequest(pConv, pszItem, nFormat, oData))
			return true;
	}

	return false;
}

/******************************************************************************
** Method:		OnAdviseStart()
**
** Description:	DDE Client requesting an advise on an item.
**
** Parameters:	hConv		The conversation handle.
**				pszItem		The item being requested.
**				nFormat		The data format.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CDDEServer::OnAdviseStart(HCONV hConv, const tchar* pszItem, uint nFormat)
{
	ASSERT(hConv   != NULL);
	ASSERT(pszItem != nullptr);

	bool bAccept = false;

	// Find the conversation from the handle.
	CDDESvrConv* pConv = FindConversation(hConv);

	ASSERT(pConv != nullptr);

	// Query all listeners...
	for (size_t i = 0, n = m_aoListeners.size(); i != n; ++i)
	{
		if (m_aoListeners[i]->OnAdviseStart(pConv, pszItem, nFormat))
		{
			bAccept = true;
			break;
		}
	}

	// Accepted?
	if (bAccept)
	{
		// Create a link and add to conversation collection.
		CDDELink* pLink = pConv->CreateLink(pszItem, nFormat);

		// Notify listeners.
		for (size_t i = 0, n = m_aoListeners.size(); i != n; ++i)
			m_aoListeners[i]->OnAdviseConfirm(pConv, pLink);
	}

	return bAccept;
}

/******************************************************************************
** Method:		OnAdviseRequest()
**
** Description:	DDEML requesting data for a link which has been updated.
**
** Parameters:	hConv		The conversation handle.
**				pszItem		The item being requested.
**				nFormat		The data format.
**				oData		The data to return.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CDDEServer::OnAdviseRequest(HCONV hConv, const tchar* pszItem, uint nFormat, CDDEData& oData)
{
	ASSERT(hConv   != NULL);
	ASSERT(pszItem != nullptr);

	// Find the conversation from the handle.
	CDDESvrConv* pConv = FindConversation(hConv);

	ASSERT(pConv != nullptr);

	// Find the conversations link.
	CDDELink* pLink = pConv->FindLink(pszItem, nFormat);

	ASSERT(pLink != nullptr);

	// Notify listeners.
	for (size_t i = 0, n = m_aoListeners.size(); i != n; ++i)
	{
		if (m_aoListeners[i]->OnAdviseRequest(pConv, pLink, oData))
			return true;
	}

	return false;
}

/******************************************************************************
** Method:		OnAdviseStop()
**
** Description:	Advise terminated on an item.
**
** Parameters:	hConv		The conversation handle.
**				pszItem		The item being requested.
**				nFormat		The data format.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDEServer::OnAdviseStop(HCONV hConv, const tchar* pszItem, uint nFormat)
{
	ASSERT(hConv   != NULL);
	ASSERT(pszItem != nullptr);

	// Find the conversation from the handle.
	CDDESvrConv* pConv = FindConversation(hConv);

	ASSERT(pConv != nullptr);

	// Find the conversations link.
	CDDELink* pLink = pConv->FindLink(pszItem, nFormat);

	ASSERT(pLink != nullptr);

	// Notify listeners.
	for (size_t i = 0, n = m_aoListeners.size(); i != n; ++i)
		m_aoListeners[i]->OnAdviseStop(pConv, pLink);

	// Remove from conversation.
	pConv->DestroyLink(pLink);
}

/******************************************************************************
** Method:		OnExecute()
**
** Description:	Execute a command.
**
** Parameters:	hConv		The conversation handle.
**				oData		The command.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CDDEServer::OnExecute(HCONV hConv, const CDDEData& oData, uint nFormat)
{
	ASSERT(hConv != NULL);
	ASSERT((nFormat == CF_TEXT) || (nFormat == CF_UNICODETEXT));

	// Find the conversation from the handle.
	CDDESvrConv* pConv = FindConversation(hConv);

	ASSERT(pConv != nullptr);

	CString strCmd;

	// Command data is assumed to be a CF_TEXT or CF_UNICODETEXT
	// string as XTYP_EXECUTE says format is unspecified.
	if (nFormat == CF_UNICODETEXT)
		strCmd = oData.GetString(UNICODE_TEXT);
	else
		strCmd = oData.GetString(ANSI_TEXT);

	// Notify listeners.
	for (size_t i = 0, n = m_aoListeners.size(); i != n; ++i)
	{
		if (m_aoListeners[i]->OnExecute(pConv, strCmd))
			return true;
	}

	return false;
}

/******************************************************************************
** Method:		OnPoke()
**
** Description:	Poke a value into an item.
**
** Parameters:	hConv		The conversation handle.
**				pszItem		The item being poked.
**				nFormat		The data format.
**				oData		The data.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CDDEServer::OnPoke(HCONV hConv, const tchar* pszItem, uint nFormat, const CDDEData& oData)
{
	ASSERT(hConv != NULL);

	// Find the conversation from the handle.
	CDDESvrConv* pConv = FindConversation(hConv);

	ASSERT(pConv != nullptr);

	// Notify listeners.
	for (size_t i = 0, n = m_aoListeners.size(); i != n; ++i)
	{
		if (m_aoListeners[i]->OnPoke(pConv, pszItem, nFormat, oData))
			return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////
//! Attempt to guess the format of the text data by inspecting it.

uint CDDEServer::GuessTextFormat(const CBuffer& buffer)
{
	const size_t size = buffer.Size();

	// Minimum unicode string is 1 character + EOL.
	// NB: Unicode EOL (\0\0) is equivalent to ANSI EOL (\0)
	if (size < Core::numBytes<wchar_t>(2))
		return CF_TEXT;

	// Unicode strings are an even number of bytes.
	if ((size % 2) != 0)
		return CF_TEXT;

	const byte* begin = static_cast<const byte*>(buffer.Buffer());
	const byte* end   = begin + size;

	ASSERT(begin != end);

	return (*(end-2) == '\0') ? CF_UNICODETEXT : CF_TEXT;
}

#if (__GNUC__ >= 8) // GCC 8+
// error: format '%hs' expects argument of type 'short int*', but argument 3 has type 'const char*' [-Werror=format=]
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
#endif

/******************************************************************************
** Method:		DDECallbackProc()
**
** Description:	The callback function invoked by the DDEML library.
**
** Parameters:	See DdeCallback.
**
** Returns:		See DdeCallback.
**
*******************************************************************************
*/

HDDEDATA CALLBACK CDDEServer::DDECallbackProc(UINT uType, UINT uFormat, HCONV hConv, HSZ hsz1, HSZ hsz2, HDDEDATA hData, ULONG_PTR /*dwData1*/, ULONG_PTR /*dwData2*/)
{
	HDDEDATA result = DDE_FNOTPROCESSED;

	try
	{
		result = DDECallbackProcImpl(uType, uFormat, hConv, hsz1, hsz2, hData);
	}
	catch (const Core::Exception& e)
	{
		WCL::ReportUnhandledException(	TXT("Unexpected exception caught in DDECallbackProc()\n\n")
										TXT("Args: Type=0x%08X Fmt=0x%08X C=0x%08X\n\n%s"),
										uType, uFormat, reinterpret_cast<INT_PTR>(hConv), e.twhat());
	}
	catch (const std::exception& e)
	{
		WCL::ReportUnhandledException(	TXT("Unexpected exception caught in DDECallbackProc()\n\n")
										TXT("Args: Type=0x%08X Fmt=0x%08X C=0x%08X\n\n%hs"),
										uType, uFormat, reinterpret_cast<INT_PTR>(hConv), e.what());
	}
	catch (...)
	{
		WCL::ReportUnhandledException(	TXT("Unexpected unknown exception caught in DDECallbackProc()\n\n")
										TXT("Args: Type=0x%08X Fmt=0x%08X C=0x%08X"),
										uType, uFormat, reinterpret_cast<INT_PTR>(hConv));
	}

	return result;
}

#if (__GNUC__ >= 8) // GCC 8+
#pragma GCC diagnostic pop
#endif

////////////////////////////////////////////////////////////////////////////////
//! The DDE Callback function implementation.

HDDEDATA CDDEServer::DDECallbackProcImpl(UINT uType, UINT uFormat, HCONV hConv, HSZ hsz1, HSZ hsz2, HDDEDATA hData)
{
	ASSERT(g_pDDEServer != nullptr);

	HDDEDATA hResult = NULL;

	// Forward to handler.
	switch (uType)
	{
		case XTYP_REGISTER:
		{
			// Not implemented, skipped by default.
		}
		break;

		case XTYP_UNREGISTER:
		{
			// Not implemented, skipped by default.
		}
		break;

		// Querying supported services/topics?
		case XTYP_WILDCONNECT:
		{
			// Querying all supported services & topics?
			if ((hsz2 == NULL) && (hsz1 == NULL))
			{
				CStrArray astrServices, astrTopics;

				// Get list of servers and topics.
				if (g_pDDEServer->OnWildConnect(astrServices, astrTopics))
				{
					ASSERT(astrServices.Size() > 0);
					ASSERT(astrServices.Size() == astrTopics.Size());

					size_t nEntries = (astrServices.Size() + 1);
					size_t nBufSize = sizeof(HSZPAIR) * nEntries;

					// Allocate temporary data buffer.
					HSZPAIR* aoPairs = static_cast<HSZPAIR*>(_alloca(nBufSize));

					// Set entries...
					for (size_t i = 0; i < astrServices.Size(); ++i)
					{
						aoPairs[i].hszSvc   = CDDEString(g_pDDEServer, astrServices[i], false);
						aoPairs[i].hszTopic = CDDEString(g_pDDEServer, astrTopics[i],   false);
					}

					// Last entry is NULL.
					aoPairs[nEntries-1].hszSvc   = NULL;
					aoPairs[nEntries-1].hszTopic = NULL;

					// Create result handle from temporary buffer.
					hResult = CDDEData(g_pDDEServer, aoPairs, nBufSize, 0, 0, false).Handle();
				}
			}
			// Querying all topics supported by a specific server?
			else if (hsz1 == NULL)
			{
				CDDEString strService(g_pDDEServer, hsz2);

				CStrArray astrTopics;

				// Get list of topics.
				if (g_pDDEServer->OnWildConnectService(strService, astrTopics))
				{
					ASSERT(astrTopics.Size() > 0);

					size_t nEntries = (astrTopics.Size() + 1);
					size_t nBufSize = sizeof(HSZPAIR) * nEntries;

					// Allocate temporary data buffer.
					HSZPAIR* aoPairs = static_cast<HSZPAIR*>(_alloca(nBufSize));

					// Set entries...
					for (size_t i = 0; i < astrTopics.Size(); ++i)
					{
						aoPairs[i].hszSvc   = hsz2;
						aoPairs[i].hszTopic = CDDEString(g_pDDEServer, astrTopics[i],   false);
					}

					// Last entry is NULL.
					aoPairs[nEntries-1].hszSvc   = NULL;
					aoPairs[nEntries-1].hszTopic = NULL;

					// Create result handle from temporary buffer.
					hResult = CDDEData(g_pDDEServer, aoPairs, nBufSize, 0, 0, false).Handle();
				}
			}
			// Querying all servers supporting a specific topic?
			else if (hsz2 == NULL)
			{
				CDDEString strTopic(g_pDDEServer, hsz1);

				CStrArray astrServices, astrTopics;

				// Get list of servers.
				if (g_pDDEServer->OnWildConnectTopic(strTopic, astrTopics))
				{
					ASSERT(astrServices.Size() > 0);

					size_t nEntries = (astrServices.Size() + 1);
					size_t nBufSize = sizeof(HSZPAIR) * nEntries;

					// Allocate temporary data buffer.
					HSZPAIR* aoPairs = static_cast<HSZPAIR*>(_alloca(nBufSize));

					// Set entries...
					for (size_t i = 0; i < astrTopics.Size(); ++i)
					{
						aoPairs[i].hszSvc   = CDDEString(g_pDDEServer, astrServices[i], false);
						aoPairs[i].hszTopic = hsz1;
					}

					// Last entry is NULL.
					aoPairs[nEntries-1].hszSvc   = NULL;
					aoPairs[nEntries-1].hszTopic = NULL;

					// Create result handle from temporary buffer.
					hResult = CDDEData(g_pDDEServer, aoPairs, nBufSize, 0, 0, false).Handle();
				}
			}
		}
		break;

		// Connection requested?
		case XTYP_CONNECT:
		{
			CDDEString strService(g_pDDEServer, hsz2);
			CDDEString strTopic(g_pDDEServer, hsz1);

			const bool result = g_pDDEServer->OnConnect(strService, strTopic);
			hResult = reinterpret_cast<HDDEDATA>(static_cast<INT_PTR>(result));
		}
		break;

		// Connection established?
		case XTYP_CONNECT_CONFIRM:
		{
			CDDEString strService(g_pDDEServer, hsz2);
			CDDEString strTopic(g_pDDEServer, hsz1);

			g_pDDEServer->OnConnectConfirm(hConv, strService, strTopic);
		}
		break;

		// Connection terminated?
		case XTYP_DISCONNECT:
		{
			g_pDDEServer->OnDisconnect(hConv);
		}
		break;

		// Item request?
		case XTYP_REQUEST:
		{
			CDDEString strItem(g_pDDEServer, hsz2);
			CDDEData   oData(g_pDDEServer, hsz2, uFormat, false);

			if (g_pDDEServer->OnRequest(hConv, strItem, uFormat, oData))
				hResult = oData.Handle();
			else
				oData.Free();
		}
		break;

		// Advise requested?
		case XTYP_ADVSTART:
		{
			CDDEString strItem(g_pDDEServer, hsz2);

			const bool result = g_pDDEServer->OnAdviseStart(hConv, strItem, uFormat);
			hResult = reinterpret_cast<HDDEDATA>(static_cast<INT_PTR>(result));
		}
		break;

		// Item updated?
		case XTYP_ADVREQ:
		{
			CDDEString strItem(g_pDDEServer, hsz2);
			CDDEData   oData(g_pDDEServer, hsz2, uFormat, false);

			if (g_pDDEServer->OnAdviseRequest(hConv, strItem, uFormat, oData))
				hResult = oData.Handle();
			else
				oData.Free();
		}
		break;

		// Advise terminated?
		case XTYP_ADVSTOP:
		{
			CDDEString strItem(g_pDDEServer, hsz2);

			g_pDDEServer->OnAdviseStop(hConv, strItem, uFormat);
		}
		break;

		// Execute command?
		case XTYP_EXECUTE:
		{
			CDDEData oData(g_pDDEServer, hData, CF_NONE, true);
			uint     nFormat = GuessTextFormat(oData.GetBuffer());

			hResult = DDE_FNOTPROCESSED;

			if (g_pDDEServer->OnExecute(hConv, oData, nFormat))
				hResult = reinterpret_cast<HDDEDATA>(DDE_FACK);
		}
		break;

		// Poke item?
		case XTYP_POKE:
		{
			CDDEString strItem(g_pDDEServer, hsz2);
			CDDEData   oData(g_pDDEServer, hData, uFormat, false);

			hResult = DDE_FNOTPROCESSED;

			if (g_pDDEServer->OnPoke(hConv, strItem, uFormat, oData))
				hResult = reinterpret_cast<HDDEDATA>(DDE_FACK);
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
