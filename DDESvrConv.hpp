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

#if _MSC_VER > 1000
#pragma once
#endif

#include <Legacy/TArray.hpp>
#include "DDEConv.hpp"

// Forward declarations.
class CDDELink;

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
	CDDELink* FindLink(const tchar* pszItem, uint nFormat) const;
	size_t    NumLinks() const;
	CDDELink* GetLink(size_t nIndex) const;
	size_t    GetAllLinks(CDDESvrLinks& aoLinks) const;
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
	CDDESvrConv(CDDEInst* pInst, HCONV hConv, const tchar* pszService, const tchar* pszTopic);
	CDDESvrConv(const CDDESvrConv&);
	virtual ~CDDESvrConv();

	//
	// Link methods.
	//
	CDDELink* CreateLink(const tchar* pszItem, uint nFormat);
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

inline size_t CDDESvrConv::NumLinks() const
{
	return m_aoLinks.Size();
}

inline CDDELink* CDDESvrConv::GetLink(size_t nIndex) const
{
	return m_aoLinks[nIndex];
}

inline size_t CDDESvrConv::GetAllLinks(CDDESvrLinks& aoLinks) const
{
	aoLinks.ShallowCopy(m_aoLinks);

	return aoLinks.Size();
}

#endif // DDESVRCONV_HPP
