/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDEINST.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CDDEInst class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef DDEINST_HPP
#define DDEINST_HPP

#if _MSC_VER > 1000
#pragma once
#endif

/******************************************************************************
** 
** The base class for all DDE services.
**
*******************************************************************************
*/

class CDDEInst
{
public:
	// Service types.
	enum InstType
	{
		CLIENT,
		SERVER,
	};

	//
	// Accessors.
	//
	DWORD    Handle() const;
	InstType Type() const;

	//
	// Methods.
	//
	uint    LastError() const;
	CString LastErrorCode() const;

	static CString GetErrorCode(uint nError);

protected:
	//
	// Members.
	//
	DWORD		m_dwInst;	// The instance handle.
	InstType	m_eType;	// The instance type.

	//
	// Constructors/Destructor.
	// NB: Make abstract.
	//
	CDDEInst();
	virtual ~CDDEInst();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CDDEInst::CDDEInst()
	: m_dwInst(0)
	, m_eType(CLIENT)
{
}

inline CDDEInst::~CDDEInst()
{
}

inline DWORD CDDEInst::Handle() const
{
	return m_dwInst;
}

inline CDDEInst::InstType CDDEInst::Type() const
{
	return m_eType;
}

inline uint CDDEInst::LastError() const
{
	return ::DdeGetLastError(m_dwInst);
}

inline CString CDDEInst::LastErrorCode() const
{
	return GetErrorCode(::DdeGetLastError(m_dwInst));
}

#endif // DDEINST_HPP
