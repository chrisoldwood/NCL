/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDECLTCONV.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CDDECltConv class definition.
**
*******************************************************************************
*/

#include "ncl.hpp"

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

	DestroyAllLinks();
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
		
	return CDDEData(m_pInst, hData);
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

CDDELink* CDDECltConv::FindLink(const char* pszItem, uint nFormat)
{
	ASSERT(pszItem != NULL);

	// Search the links list.
	for (int i = 0; i < m_aoLinks.Size(); ++i)
	{
		CDDELink* pLink = m_aoLinks[i];

		if ( (pLink->Item() == pszItem) && (pLink->Format() == nFormat) )
			return pLink;
	}

	return NULL;
}

/******************************************************************************
** Method:		GetAllLinks()
**
** Description:	Gets the collection of links.
**
** Parameters:	aoLinks		The return array for the collection.
**
** Returns:		The number of links.
**
*******************************************************************************
*/

uint CDDECltConv::GetAllLinks(CDDECltLinks& aoLinks)
{
	for (int i = 0; i < m_aoLinks.Size(); ++i)
		aoLinks.Add(m_aoLinks[i]);

	return aoLinks.Size();
}
