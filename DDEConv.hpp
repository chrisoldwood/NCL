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

#if _MSC_VER > 1000
#pragma once
#endif

// Forward declarations.
class IDDEConvData;
class CDDEInst;

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

	bool           IsConnected() const;

	IDDEConvData*  AppData() const;
	void           SetAppData(IDDEConvData* pAppData);

protected:
	//
	// Members.
	//
	CDDEInst*		m_pInst;		// The instance handle.
	HCONV			m_hConv;		// The conversation handle.
	CString			m_strService;	// The service name.
	CString			m_strTopic;		// The topic name.
	IDDEConvData*	m_pAppData;		// Custom data.

	//
	// Constructors/Destructor.
	//
	CDDEConv(CDDEInst* pInst, HCONV hConv, const char* pszService, const char* pszTopic);
	CDDEConv(const CDDEConv&);
	virtual ~CDDEConv();

	//
	// Methods.
	//
	void Disconnect();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CDDEConv::CDDEConv(CDDEInst* pInst, HCONV hConv, const char* pszService, const char* pszTopic)
	: m_pInst(pInst)
	, m_hConv(hConv)
	, m_strService(pszService)
	, m_strTopic(pszTopic)
	, m_pAppData(NULL)
{
	ASSERT(m_pInst      != NULL);
	ASSERT(m_hConv      != NULL);
	ASSERT(m_strService != "");
	ASSERT(m_strTopic   != "");
}

inline CDDEConv::~CDDEConv()
{
}

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

inline bool CDDEConv::IsConnected() const
{
	return (m_hConv != NULL);
}

inline IDDEConvData* CDDEConv::AppData() const
{
	return m_pAppData;
}

inline void CDDEConv::SetAppData(IDDEConvData* pAppData)
{
	m_pAppData = pAppData;
}

inline void CDDEConv::Disconnect()
{
	if (m_hConv != NULL)
		::DdeDisconnect(m_hConv);

	m_hConv = NULL;
}

#endif // DDECONV_HPP
