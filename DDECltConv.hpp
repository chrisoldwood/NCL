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
	// Methods.
	//

	//
	// Request methods.
	//
	CString  Request(const char* pszItem);
	CDDEData Request(const char* pszItem, uint nFormat);

	//
	// Link methods.
	//
	CDDELink* CreateLink(const char* pszItem, uint nFormat = CF_TEXT);
	void      DestroyLink(CDDELink* pLink);
	void      DestroyAllLinks();
	CDDELink* FindLink(const char* pszItem, uint nFormat = CF_TEXT);
	uint      GetAllLinks(CDDECltLinks& aoLinks);

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

#endif // DDECLTCONV_HPP
