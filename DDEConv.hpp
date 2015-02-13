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

#include "DDEFwd.hpp"

/******************************************************************************
**
** This class is used to encapsulate a DDE conversation.
**
*******************************************************************************
*/

class CDDEConv /*: private Core::NotCopyable*/
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
	CDDEConv(CDDEInst* pInst, HCONV hConv, const tchar* pszService, const tchar* pszTopic);
	virtual ~CDDEConv();

	//
	// Methods.
	//
	void Disconnect();

private:
	// NotCopyable.
	CDDEConv(const CDDEConv&);
	CDDEConv& operator=(const CDDEConv&);
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

#endif // DDECONV_HPP
