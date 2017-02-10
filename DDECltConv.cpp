/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDECLTCONV.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CDDECltConv class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "DDECltConv.hpp"
#include "DDEClient.hpp"
#include "DDELink.hpp"
#include "DDEData.hpp"
#include "DDEString.hpp"
#include "DDEException.hpp"
#include <Core/AnsiWide.hpp>
#include <algorithm>

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CDDECltConv::CDDECltConv(CDDEClient* client, HCONV hConv, const tchar* pszService, const tchar* pszTopic, DWORD timeout)
	: CDDEConv(client, hConv, pszService, pszTopic)
	, m_nRefCount(0)
	, m_client(client)
	, m_timeout(timeout)
	, m_aoLinks()
{
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CDDECltConv::~CDDECltConv()
{
	ASSERT(m_nRefCount == 0);

	// Delete all links.
	for (size_t i = 0, n = m_aoLinks.size(); i != n; ++i)
	{
		CDDELink* pLink = m_aoLinks[i];

		pLink->m_nRefCount = 0;

		delete pLink;
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Get the DDE Client that this conversation belongs to.

DDE::IDDEClient* CDDECltConv::Client() const
{
	return m_client;
}

/******************************************************************************
** Method:		Request()
**
** Description:	Request an item from the server.
**
** Parameters:	pszItem		The item to fetch.
**				nformat		The string format (ANSI or Unicode).
**
** Returns:		The item.
**
*******************************************************************************
*/

CString CDDECltConv::RequestString(const tchar* pszItem, uint nFormat) const
{
	ASSERT((nFormat == CF_TEXT) || (nFormat == CF_UNICODETEXT));

	if (nFormat == CF_TEXT)
		return Request(pszItem, CF_TEXT).GetString(ANSI_TEXT);
	else
		return Request(pszItem, CF_UNICODETEXT).GetString(UNICODE_TEXT);
}

/******************************************************************************
** Method:		Request()
**
** Description:	Request an item from the server.
**
** Parameters:	pszItem		The item to fetch.
**				nFormat		The item data format.
**
** Returns:		The item.
**
*******************************************************************************
*/

CDDEData CDDECltConv::Request(const tchar* pszItem, uint nFormat) const
{
	ASSERT(pszItem != nullptr);

	CDDEString strItem(m_pInst, pszItem);
	DWORD      dwResult;

	// Make the request.
	HDDEDATA hData = ::DdeClientTransaction(nullptr, 0, m_hConv, strItem, nFormat, XTYP_REQUEST, m_timeout, &dwResult);

	// Request failed?
	if (hData == NULL)
		throw CDDEException(CDDEException::E_REQUEST_FAILED, m_pInst->LastError());

	return CDDEData(m_pInst, hData, nFormat, true);
}

/******************************************************************************
** Methods:		Execute()
**
** Description:	Execute a command on the DDE server.
**
** Parameters:	Command		The command to execute.
**
** Returns:		Nothing.
**
** Notes:		The XTYP_EXECUTE transaction type does not support passing of
**				the data format. There also appears to be bug in DDEML that
**				means passing CF_TEXT from a Unicode DDE client to an ANSI
**				DDE server fails. DDEML seems to try and convert the payload to
**				match the target build type, irrespective of what is sent by
**				the client.
**
*******************************************************************************
*/

void CDDECltConv::ExecuteString(const tchar* pszCommand) const
{
#ifdef ANSI_BUILD
	Execute(T2A(pszCommand), Core::numBytes<char>(tstrlen(pszCommand)+1));
#else
	Execute(T2W(pszCommand), Core::numBytes<wchar_t>(tstrlen(pszCommand)+1));
#endif
}

void CDDECltConv::Execute(const void* pValue, size_t nSize) const
{
	ASSERT(pValue != nullptr);

	LPBYTE lpData = static_cast<byte*>(const_cast<void*>(pValue));

	// Execute it.
	HDDEDATA hResult = ::DdeClientTransaction(lpData, static_cast<DWORD>(nSize), m_hConv,
												NULL, 0, XTYP_EXECUTE, m_timeout, nullptr);

	// Execute failed?
	if (hResult == NULL)
		throw CDDEException(CDDEException::E_EXECUTE_FAILED, m_pInst->LastError());
}

/******************************************************************************
** Methods:		Poke()
**
** Description:	Poke a value into an item on the server.
**
** Parameters:	pszItem		The item to poke into.
**				nFormat		The format of the data.
**				Value		The value.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDECltConv::PokeString(const tchar* pszItem, const tchar* pszValue, uint nFormat) const
{
	ASSERT((nFormat == CF_TEXT) || (nFormat == CF_UNICODETEXT));

	if (nFormat == CF_TEXT)
		Poke(pszItem, CF_TEXT, T2A(pszValue), Core::numBytes<char>(tstrlen(pszValue)+1));
	else
		Poke(pszItem, CF_UNICODETEXT, T2W(pszValue), Core::numBytes<wchar_t>(tstrlen(pszValue)+1));
}

void CDDECltConv::Poke(const tchar* pszItem, uint nFormat, const void* pValue, size_t nSize) const
{
	ASSERT(pszItem != nullptr);
	ASSERT(pValue  != nullptr);

	CDDEString strItem(m_pInst, pszItem);

	LPBYTE lpData = static_cast<byte*>(const_cast<void*>(pValue));

	// Do the poke.
	HDDEDATA hResult = ::DdeClientTransaction(lpData, static_cast<DWORD>(nSize), m_hConv,
												strItem, nFormat, XTYP_POKE, m_timeout, nullptr);

	// Poke failed?
	if (hResult == NULL)
		throw CDDEException(CDDEException::E_POKE_FAILED, m_pInst->LastError());
}

/******************************************************************************
** Method:		CreateLink()
**
** Description:	Creates an advise loop for an item.
**
** Parameters:	pszItem		The item to link to.
**				nFormat		The item data format.
**
** Returns:		The link.
**
*******************************************************************************
*/

CDDELink* CDDECltConv::CreateLink(const tchar* pszItem, uint nFormat)
{
	ASSERT(pszItem != nullptr);

	// Already linked with format?
	CDDELink* pLink = FindLink(pszItem, nFormat);

	if (pLink == nullptr)
	{
		CDDEString strItem(m_pInst, pszItem);
		DWORD      dwResult;

		// Attempt to start the advise loop.
		HDDEDATA hData = ::DdeClientTransaction(nullptr, 0, m_hConv, strItem, nFormat, XTYP_ADVSTART, m_timeout, &dwResult);

		// Advise failed?
		if (hData == NULL)
			throw CDDEException(CDDEException::E_LINK_FAILED, m_pInst->LastError());

		// Create link.
		pLink = new CDDELink(this, pszItem, nFormat);

		// Add to collection.
		m_aoLinks.push_back(pLink);
	}

	// New reference.
	++pLink->m_nRefCount;

	return pLink;
}

/******************************************************************************
** Method:		DestroyLink()
**
** Description:	Ends an advise loop for an item.
**
** Parameters:	pLink		The link to end.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDECltConv::DestroyLink(CDDELink* pLink)
{
	ASSERT(pLink != nullptr);

	// Last reference?
	if (--pLink->m_nRefCount == 0)
	{
		CDDEString strItem(m_pInst, pLink->Item());
		DWORD      dwResult;

		// End advise.
		::DdeClientTransaction(nullptr, 0, m_hConv, strItem, pLink->Format(), XTYP_ADVSTOP, m_timeout, &dwResult);

		// Delete link.
		delete pLink;

		// Remove from collection.
		m_aoLinks.erase(std::find(m_aoLinks.begin(), m_aoLinks.end(), pLink));
	}
}

/******************************************************************************
** Method:		DestroyAllLinks()
**
** Description:	Ends an advise loop for all items.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDECltConv::DestroyAllLinks()
{
	while (!m_aoLinks.empty())
		DestroyLink(m_aoLinks[0]);
}

/******************************************************************************
** Method:		FindLink()
**
** Description:	Finds an existing link.
**
** Parameters:	pszItem		The item to link to.
**				nFormat		The item data format.
**
** Returns:		The link or nullptr if not found.
**
*******************************************************************************
*/

CDDELink* CDDECltConv::FindLink(const tchar* pszItem, uint nFormat) const
{
	ASSERT(pszItem != nullptr);

	// Search the links list.
	for (size_t i = 0, n = m_aoLinks.size(); i != n; ++i)
	{
		CDDELink* pLink = m_aoLinks[i];

		if ( (tstricmp(pLink->Item(), pszItem) == 0) && (pLink->Format() == nFormat) )
			return pLink;
	}

	return nullptr;
}
