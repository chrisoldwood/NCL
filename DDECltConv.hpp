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

	//
	// Command methods.
	//
	CString  Request(const char* pszItem);
	CDDEData Request(const char* pszItem, uint nFormat);

	void Execute(const char* pszCommand);
	void Execute(const CDDEData& oCommand);

	void Poke(const char* pszItem, const char* pszValue);
	void Poke(const char* pszItem, uint nFormat, const byte* pValue, uint nSize);
	void Poke(const char* pszItem, uint nFormat, const CDDEData& oValue);

	//
	// Link methods.
	//
	CDDELink* CreateLink(const char* pszItem, uint nFormat = CF_TEXT);
	void      DestroyLink(CDDELink* pLink);
	void      DestroyAllLinks();
	CDDELink* FindLink(const char* pszItem, uint nFormat = CF_TEXT) const;
	int       NumLinks() const;
	CDDELink* GetLink(int nIndex) const;
	int       GetAllLinks(CDDECltLinks& aoLinks) const;

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
	CDDECltConv(CDDEInst* pInst, HCONV hConv, const char* pszService, const char* pszTopic);
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

inline int CDDECltConv::NumLinks() const
{
	return m_aoLinks.Size();
}

inline CDDELink* CDDECltConv::GetLink(int nIndex) const
{
	return m_aoLinks[nIndex];
}

inline int CDDECltConv::GetAllLinks(CDDECltLinks& aoLinks) const
{
	aoLinks.ShallowCopy(m_aoLinks);

	return aoLinks.Size();
}

#endif // DDECLTCONV_HPP
