/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDESVRCONV.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CDDESvrConv class definition.
**
*******************************************************************************
*/

#include "ncl.hpp"

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

CDDESvrConv::CDDESvrConv(CDDEInst* pInst, HCONV hConv, const char* pszService, const char* pszTopic)
	: CDDEConv(pInst, hConv, pszService, pszTopic)
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

CDDESvrConv::~CDDESvrConv()
{
	DestroyAllLinks();
}

/******************************************************************************
** Method:		CreateLink()
**
** Description:	Creates a link.
**
** Parameters:	pszItem		The item to link to.
**				nFormat		The item data format.
**
** Returns:		The link.
**
*******************************************************************************
*/

CDDELink* CDDESvrConv::CreateLink(const char* pszItem, uint nFormat)
{
	CDDELink* pLink = new CDDELink(this, pszItem, nFormat);

	m_aoLinks.Add(pLink);

	return pLink;
}

/******************************************************************************
** Method:		DestroyLink()
**
** Description:	Destroys a link.
**
** Parameters:	pLink		The link to destroy.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDESvrConv::DestroyLink(CDDELink* pLink)
{
	m_aoLinks.Remove(m_aoLinks.Find(pLink));
	delete pLink;
}

/******************************************************************************
** Method:		DestroyAllLinks()
**
** Description:	Destroys all links.
**
** Parameters:	pLink		The link to destroy.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CDDESvrConv::DestroyAllLinks()
{
	for (int i = 0; i < m_aoLinks.Size(); ++i)
		delete m_aoLinks[i];

	m_aoLinks.RemoveAll();
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

CDDELink* CDDESvrConv::FindLink(const char* pszItem, uint nFormat)
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

uint CDDESvrConv::GetAllLinks(CDDESvrLinks& aoLinks)
{
	for (int i = 0; i < m_aoLinks.Size(); ++i)
		aoLinks.Add(m_aoLinks[i]);

	return aoLinks.Size();
}

/******************************************************************************
** Method:		PostLinkUpdate()
**
** Description:	Posts an update message to inform DDE clients that the link
**				has been updated.
**
** Parameters:	pLink	The updated link.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

bool CDDESvrConv::PostLinkUpdate(const CDDELink* pLink)
{
	CDDEString strTopic(m_pInst, m_strTopic);
	CDDEString strItem(m_pInst, pLink->Item());

	return (::DdePostAdvise(m_pInst->Handle(), strTopic, strItem) > 0);
}
