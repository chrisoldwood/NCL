/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDESTRING.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CDDEString class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "DDEString.hpp"
#include "DDEInst.hpp"
#include "DDEException.hpp"

CDDEString::CDDEString(CDDEInst* pInst, const tchar* pszString, bool bOwn)
	: m_pInst(pInst)
	, m_hsz()
	, m_bOwn(bOwn)
{
	ASSERT(pInst     != nullptr);
	ASSERT(pszString != nullptr);

	// Create the string handle.
	m_hsz = ::DdeCreateStringHandle(m_pInst->Handle(), pszString, CP_WIN_TCHAR);

	if (m_hsz == NULL)
		throw CDDEException(CDDEException::E_STRING_FAILED, m_pInst->LastError());

	// Save original string.
	tstrncpy(m_sz, pszString, MAX_LENGTH);

	m_sz[MAX_LENGTH] = '\0';
}

CDDEString::CDDEString(CDDEInst* pInst, HSZ hsz, bool bOwn)
	: m_pInst(pInst)
	, m_hsz(hsz)
	, m_bOwn(bOwn)
{
	ASSERT(pInst != nullptr);

	// Extract original string.
	DWORD result = ::DdeQueryString(m_pInst->Handle(), m_hsz, m_sz, MAX_LENGTH+1, CP_WIN_TCHAR);

	if (result == 0)
		throw CDDEException(CDDEException::E_STRCOPY_FAILED, m_pInst->LastError());
}

CDDEString::~CDDEString()
{
	// Free the string, if owned.
	if (m_bOwn)
	{
		BOOL okay = ::DdeFreeStringHandle(m_pInst->Handle(), m_hsz);

		ASSERT_RESULT(okay, okay != FALSE);
	}
}
