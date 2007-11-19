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
#include "DDELink.hpp"
#include "DDEData.hpp"
#include "DDEString.hpp"

/******************************************************************************
**
** Class members.
**
*******************************************************************************
*/

// The default timeout (30 secs).
DWORD CDDECltConv::DEF_TIMEOUT = 30000;

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

CDDECltConv::CDDECltConv(CDDEInst* pInst, HCONV hConv, const char* pszService, const char* pszTopic)
	: CDDEConv(pInst, hConv, pszService, pszTopic)
	, m_nRefCount(0)
	, m_dwTimeout(DEF_TIMEOUT)
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
	for (int i = 0; i < m_aoLinks.Size(); ++i)
	{
		CDDELink* pLink = m_aoLinks[i];

		pLink->m_nRefCount = 0;

		delete pLink;
	}
}

/******************************************************************************
** Method:		Request()
**
** Description:	Request an item from the server.
**				NB: Assumes that the item is of the format CF_TEXT.
**
** Parameters:	pszItem		The item to fetch.
**
** Returns:		The item.
**
*******************************************************************************
*/

CString CDDECltConv::Request(const char* pszItem)
{
	return Request(pszItem, CF_TEXT).GetString();
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

CDDEData CDDECltConv::Request(const char* pszItem, uint nFormat)
{
	ASSERT(pszItem != NULL);

	CDDEString strItem(m_pInst, pszItem);
	DWORD      dwResult;

	// Make the request.
	HDDEDATA hData = ::DdeClientTransaction(NULL, 0, m_hConv, strItem, nFormat, XTYP_REQUEST, m_dwTimeout, &dwResult);

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
*******************************************************************************
*/

void CDDECltConv::Execute(const char* pszCommand)
{
	ASSERT(pszCommand != NULL);

	// Execute it.
	HDDEDATA hResult = ::DdeClientTransaction((byte*)pszCommand, strlen(pszCommand)+1, m_hConv, NULL, NULL, XTYP_EXECUTE, m_dwTimeout, NULL);

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

void CDDECltConv::Poke(const char* pszItem, const char* pszValue)
{
	Poke(pszItem, CF_TEXT, (const byte*)pszValue, strlen(pszValue)+1);
}

void CDDECltConv::Poke(const char* pszItem, uint nFormat, const void* pValue, uint nSize)
{
	ASSERT(pszItem != NULL);
	ASSERT(pValue  != NULL);

	CDDEString strItem(m_pInst, pszItem);

	// Do the poke.
	HDDEDATA hResult = ::DdeClientTransaction((byte*)pValue, nSize, m_hConv, strItem, nFormat, XTYP_POKE, m_dwTimeout, NULL);

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

CDDELink* CDDECltConv::CreateLink(const char* pszItem, uint nFormat)
{
	ASSERT(pszItem != NULL);

	// Already linked with format?
	CDDELink* pLink = FindLink(pszItem, nFormat);

	if (pLink == NULL)
	{
		CDDEString strItem(m_pInst, pszItem);
		DWORD      dwResult;

		// Attempt to start the advise loop.
		HDDEDATA hData = ::DdeClientTransaction(NULL, 0, m_hConv, strItem, nFormat, XTYP_ADVSTART, m_dwTimeout, &dwResult);

		// Advise failed?
		if (hData == NULL)
			throw CDDEException(CDDEException::E_LINK_FAILED, m_pInst->LastError());

		// Create link.
		pLink = new CDDELink(this, pszItem, nFormat);

		// Add to collection.
		m_aoLinks.Add(pLink);
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
	ASSERT(pLink != NULL);

	// Last reference?
	if (--pLink->m_nRefCount == 0)
	{
		CDDEString strItem(m_pInst, pLink->Item());
		DWORD      dwResult;

		// End advise.
		::DdeClientTransaction(NULL, 0, m_hConv, strItem, pLink->Format(), XTYP_ADVSTOP, m_dwTimeout, &dwResult);

		// Delete link.
		delete pLink;

		// Remove from collection.
		m_aoLinks.Remove(m_aoLinks.Find(pLink));
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
	while (m_aoLinks.Size() > 0)
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
** Returns:		The link or NULL if not found.
**
*******************************************************************************
*/

CDDELink* CDDECltConv::FindLink(const char* pszItem, uint nFormat) const
{
	ASSERT(pszItem != NULL);

	// Search the links list.
	for (int i = 0; i < m_aoLinks.Size(); ++i)
	{
		CDDELink* pLink = m_aoLinks[i];

		if ( (_stricmp(pLink->Item(), pszItem) == 0) && (pLink->Format() == nFormat) )
			return pLink;
	}

	return NULL;
}
