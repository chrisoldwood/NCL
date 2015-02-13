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

#include "DDEFwd.hpp"
#include "DDEConv.hpp"
#include <vector>

// Template shorthands.
typedef std::vector<CDDELink*> CDDECltLinks;

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
	CDDEClient*	Client() const;

	DWORD  Timeout() const;
	void   SetTimeout(DWORD timeout);

	//
	// Command methods.
	//
	CString  RequestString(const tchar* pszItem, uint nFormat);
	CDDEData Request(const tchar* pszItem, uint nFormat);

	void ExecuteString(const tchar* pszCommand);
	void Execute(const void* pValue, size_t nSize);

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
	CDDEClient*		m_client;		//!< The owning DDE client.
	DWORD			m_timeout;		//!< The timeout value for transactions.
	CDDECltLinks	m_aoLinks;		// The list of links.

	//
	// Constructors/Destructor.
	// NB: Only available to CDDEClient.
	//
	CDDECltConv(CDDEClient* client, HCONV hConv, const tchar* pszService, const tchar* pszTopic, DWORD timeout);
	CDDECltConv(const CDDECltConv&);
	virtual ~CDDECltConv();

	// Friends.
	friend class CDDEClient;

private:
	// NotCopyable.
	CDDECltConv& operator=(const CDDECltConv&);
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

////////////////////////////////////////////////////////////////////////////////
//! Get the DDE Client that this conversation belongs to.

inline CDDEClient* CDDECltConv::Client() const
{
	return m_client;
}

inline DWORD CDDECltConv::Timeout() const
{
	return m_timeout;
}

inline void CDDECltConv::SetTimeout(DWORD timeout)
{
	m_timeout = timeout;
}

inline size_t CDDECltConv::NumLinks() const
{
	return m_aoLinks.size();
}

inline CDDELink* CDDECltConv::GetLink(size_t nIndex) const
{
	return m_aoLinks[nIndex];
}

inline size_t CDDECltConv::GetAllLinks(CDDECltLinks& aoLinks) const
{
	aoLinks = m_aoLinks;

	return aoLinks.size();
}

#endif // DDECLTCONV_HPP
