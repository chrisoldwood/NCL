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
	// Accessors.
	//
	CDDEConv*	   Conversation() const;
	const CString& Item() const;
	uint           Format() const;
	
protected:
	//
	// Members.
	//
	uint		m_nRefCount;	// The reference count.
	CDDEConv*	m_pConv;		// The parent conversation.
	CString		m_strItem;		// The item name.
	uint		m_nFormat;		// The data format.

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
{
}

inline CDDELink::~CDDELink()
{
	ASSERT(m_nRefCount == 0);
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

#endif // DDELINK_HPP
