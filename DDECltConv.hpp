/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDECLTCONV.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CDDECltConv class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef DDECLTCONV_HPP
#define DDECLTCONV_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "DDEConv.hpp"
#include <Legacy/TArray.hpp>

// Forward declarations.
class CDDELink;
class CDDEData;

// Template shorthands.
typedef TPtrArray<CDDELink> CDDECltLinks;

/******************************************************************************
** 
** The type on conversation used by a DDE Server.
**
*******************************************************************************
*/

class CDDECltConv : public CDDEConv
{
public:
	//
	// Properties.
	//
	uint RefCount() const;

	DWORD  TimeOut() const;
	void   SetTimeOut(DWORD dwTimeOut);

	//
	// Command methods.
	//
	CString  RequestString(const tchar* pszItem, uint nFormat);
	CDDEData Request(const tchar* pszItem, uint nFormat);

	void Execute(const tchar* pszCommand);

	void PokeString(const tchar* pszItem, const tchar* pszValue, uint nFormat);
	void Poke(const tchar* pszItem, uint nFormat, const void* pValue, size_t nSize);

	//
	// Link methods.
	//
	CDDELink* CreateLink(const tchar* pszItem, uint nFormat = CF_TEXT);
	void      DestroyLink(CDDELink* pLink);
	void      DestroyAllLinks();
	CDDELink* FindLink(const tchar* pszItem, uint nFormat = CF_TEXT) const;
	size_t    NumLinks() const;
	CDDELink* GetLink(size_t nIndex) const;
	size_t    GetAllLinks(CDDECltLinks& aoLinks) const;

protected:
	//
	// Members.
	//
	uint			m_nRefCount;	// The reference count.
	DWORD			m_dwTimeout;	// The timeout value for transactions.
	CDDECltLinks	m_aoLinks;		// The list of links.

	//
	// Constants.
	//
	static DWORD DEF_TIMEOUT;

	//
	// Constructors/Destructor.
	// NB: Only available to CDDEClient.
	//
	CDDECltConv(CDDEInst* pInst, HCONV hConv, const tchar* pszService, const tchar* pszTopic);
	CDDECltConv(const CDDECltConv&);
	virtual ~CDDECltConv();

	// Friends.
	friend class CDDEClient;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline uint CDDECltConv::RefCount() const
{
	return m_nRefCount;
}

inline DWORD CDDECltConv::TimeOut() const
{
	return m_dwTimeout;
}

inline void CDDECltConv::SetTimeOut(DWORD dwTimeOut)
{
	m_dwTimeout = dwTimeOut;
}

inline size_t CDDECltConv::NumLinks() const
{
	return m_aoLinks.Size();
}

inline CDDELink* CDDECltConv::GetLink(size_t nIndex) const
{
	return m_aoLinks[nIndex];
}

inline size_t CDDECltConv::GetAllLinks(CDDECltLinks& aoLinks) const
{
	aoLinks.ShallowCopy(m_aoLinks);

	return aoLinks.Size();
}

#endif // DDECLTCONV_HPP
