/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDESVRCONV.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CDDESvrConv class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "DDESvrConv.hpp"
#include "DDEString.hpp"
#include "DDELink.hpp"

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

CDDESvrConv::CDDESvrConv(CDDEInst* pInst, HCONV hConv, const tchar* pszService, const tchar* pszTopic)
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

CDDELink* CDDESvrConv::CreateLink(const tchar* pszItem, uint nFormat)
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
	for (size_t i = 0; i < m_aoLinks.Size(); ++i)
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

CDDELink* CDDESvrConv::FindLink(const tchar* pszItem, uint nFormat) const
{
	ASSERT(pszItem != NULL);

	// Search the links list.
	for (size_t i = 0; i < m_aoLinks.Size(); ++i)
	{
		CDDELink* pLink = m_aoLinks[i];

		if ( (pLink->Item() == pszItem) && (pLink->Format() == nFormat) )
			return pLink;
	}

	return NULL;
}

/******************************************************************************
** Method:		PostLinkUpdate()
**
** Description:	Posts an update message to inform DDE clients that the link
**				has been updated.
**
** Parameters:	pLink	The updated link.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CDDESvrConv::PostLinkUpdate(const CDDELink* pLink)
{
	CDDEString strTopic(m_pInst, m_strTopic);
	CDDEString strItem(m_pInst, pLink->Item());

	return (::DdePostAdvise(m_pInst->Handle(), strTopic, strItem) > 0);
}
