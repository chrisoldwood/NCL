/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDELINK.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CDDELink class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef DDELINK_HPP
#define DDELINK_HPP

// Forward declarations.
class CDDEConv;

/******************************************************************************
** 
** This is the data class for a DDE link.
**
*******************************************************************************
*/

class CDDELink
{
public:
	//
	// Properties.
	//
	uint           RefCount() const;
	CDDEConv*	   Conversation() const;
	const CString& Item() const;
	uint           Format() const;

	IDDELinkData*  AppData() const;
	void           SetAppData(IDDELinkData* pAppData);
	
protected:
	//
	// Members.
	//
	uint			m_nRefCount;	// The reference count.
	CDDEConv*		m_pConv;		// The parent conversation.
	CString			m_strItem;		// The item name.
	uint			m_nFormat;		// The data format.
	IDDELinkData*	m_pAppData;		// Custom data.

	//
	// Constructors/Destructor.
	//
	CDDELink(CDDEConv* pConv, const char* pszItem, uint nFormat);
	CDDELink(const CDDELink&);
	~CDDELink();

	// Friends.
	friend class CDDECltConv;
	friend class CDDESvrConv;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CDDELink::CDDELink(CDDEConv* pConv, const char* pszItem, uint nFormat)
	: m_nRefCount(0)
	, m_pConv(pConv)
	, m_strItem(pszItem)
	, m_nFormat(nFormat)
	, m_pAppData(NULL)
{
}

inline CDDELink::~CDDELink()
{
	ASSERT(m_nRefCount == 0);
}

inline uint CDDELink::RefCount() const
{
	return m_nRefCount;
}

inline CDDEConv* CDDELink::Conversation() const
{
	return m_pConv;
}

inline const CString& CDDELink::Item() const
{
	return m_strItem;
}

inline uint CDDELink::Format() const
{
	return m_nFormat;
}

inline IDDELinkData* CDDELink::AppData() const
{
	return m_pAppData;
}

inline void CDDELink::SetAppData(IDDELinkData* pAppData)
{
	m_pAppData = pAppData;
}

#endif // DDELINK_HPP
