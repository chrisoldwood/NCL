/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDESVRCONV.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CDDESvrConv class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef DDESVRCONV_HPP
#define DDESVRCONV_HPP

// Template shorthands.
typedef TPtrArray<CDDELink> CDDESvrLinks;

/******************************************************************************
** 
** The type on conversation used by a DDE Server.
**
*******************************************************************************
*/

class CDDESvrConv : public CDDEConv
{
public:
	//
	// Link methods.
	//
	CDDELink* FindLink(const char* pszItem, uint nFormat);
	uint      GetAllLinks(CDDESvrLinks& aoLinks);
	bool      PostLinkUpdate(const CDDELink* pLink);

protected:
	//
	// Members.
	//
	CDDESvrLinks	m_aoLinks;		// The list of links.

	//
	// Constructors/Destructor.
	// NB: Only available to CDDEServer.
	//
	CDDESvrConv(CDDEInst* pInst, HCONV hConv, const char* pszService, const char* pszTopic);
	CDDESvrConv(const CDDESvrConv&);
	virtual ~CDDESvrConv();

	//
	// Link methods.
	//
	CDDELink* CreateLink(const char* pszItem, uint nFormat);
	void      DestroyLink(CDDELink* pLink);
	void      DestroyAllLinks();

	// Friends.
	friend class CDDEServer;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // DDESVRCONV_HPP
