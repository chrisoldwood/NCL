/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDECONV.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CDDEConv class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef DDECONV_HPP
#define DDECONV_HPP

/******************************************************************************
** 
** This class is used to encapsulate a DDE conversation.
**
*******************************************************************************
*/

class CDDEConv
{
public:
	//
	// Accessors.
	//
	HCONV          Handle() const;
	const CString& Service() const;
	const CString& Topic() const;

protected:
	//
	// Members.
	//
	CDDEInst*	m_pInst;		// The instance handle.
	HCONV		m_hConv;		// The conversation handle.
	CString		m_strService;	// The service name.
	CString		m_strTopic;		// The topic name.

	//
	// Constructors/Destructor.
	//
	CDDEConv(CDDEInst* pInst, HCONV hConv, const char* pszService, const char* pszTopic);
	CDDEConv(const CDDEConv&);
	virtual ~CDDEConv();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline HCONV CDDEConv::Handle() const
{
	return m_hConv;
}

inline const CString& CDDEConv::Service() const
{
	return m_strService;
}

inline const CString& CDDEConv::Topic() const
{
	return m_strTopic;
}

#endif // DDECONV_HPP
