/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDEDATA.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CDDEData class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "DDEData.hpp"
#include <Core/AnsiWide.hpp>

CDDEData::CHandle::CHandle(CDDEInst* pInst, HDDEDATA hData, uint nFormat, bool bOwn)
	: m_nRefCount(1)
	, m_pInst(pInst)
	, m_hData(hData)
	, m_nFormat(nFormat)
	, m_bOwn(bOwn)
{
	ASSERT(pInst != NULL);
}

CDDEData::CHandle::~CHandle()
{
	ASSERT(m_nRefCount == 0);

	// Free handle?
	if (m_bOwn)
		::DdeFreeDataHandle(m_hData);
}

CDDEData::CDDEData(CDDEInst* pInst, HDDEDATA hData, uint nFormat, bool bOwn)
	: m_pHandle(new CHandle(pInst, hData, nFormat, bOwn))
{
}

CDDEData::CDDEData(CDDEInst* pInst, HSZ hItem, uint nFormat, bool bOwn)
{
	// Allocate data handle.
	HDDEDATA hData = ::DdeCreateDataHandle(pInst->Handle(), NULL, 0, 0, hItem, nFormat, 0);

	if (hData == NULL)
		throw CDDEException(CDDEException::E_ALLOC_FAILED, pInst->LastError());

	// Attach handle.
	m_pHandle = new CHandle(pInst, hData, nFormat, bOwn);
}

CDDEData::CDDEData(CDDEInst* pInst, const void* pBuffer, uint nSize, uint nOffset, uint nFormat, bool bOwn)
{
	LPBYTE lpData = static_cast<byte*>(const_cast<void*>(pBuffer));

	// Allocate data handle.
	HDDEDATA hData = ::DdeCreateDataHandle(pInst->Handle(), lpData, nSize, nOffset, NULL, nFormat, 0);

	if (hData == NULL)
		throw CDDEException(CDDEException::E_ALLOC_FAILED, pInst->LastError());

	// Attach handle.
	m_pHandle = new CHandle(pInst, hData, nFormat, bOwn);
}

CDDEData::CDDEData(CDDEInst* pInst, const CBuffer& oBuffer, uint nFormat, bool bOwn)
{
	LPBYTE lpData = static_cast<byte*>(const_cast<void*>(oBuffer.Buffer()));

	// Allocate data handle.
	HDDEDATA hData = ::DdeCreateDataHandle(pInst->Handle(), lpData, oBuffer.Size(), 0, NULL, nFormat, 0);

	if (hData == NULL)
		throw CDDEException(CDDEException::E_ALLOC_FAILED, pInst->LastError());

	// Attach handle.
	m_pHandle = new CHandle(pInst, hData, nFormat, bOwn);
}

CDDEData::CDDEData(const CDDEData& oData)
	: m_pHandle(oData.m_pHandle)
{
	ASSERT(m_pHandle != NULL);

	++(m_pHandle->m_nRefCount);
}

CDDEData::~CDDEData()
{
	ASSERT(m_pHandle != NULL);

	if (--(m_pHandle->m_nRefCount) == 0)
		delete m_pHandle;
}

CDDEData& CDDEData::operator=(const CDDEData& oData)
{
	ASSERT(this != &oData);
	ASSERT(m_pHandle != NULL);
	ASSERT(oData.m_pHandle != NULL);

	if (--(m_pHandle->m_nRefCount) == 0)
		delete m_pHandle;
	
	m_pHandle = oData.m_pHandle;

	++(m_pHandle->m_nRefCount);

	return *this;
}

uint CDDEData::Size() const
{
	ASSERT(m_pHandle != NULL);

	if (m_pHandle->m_hData == NULL)
		return 0;

	return ::DdeGetData(m_pHandle->m_hData, NULL, 0, 0);
}

CBuffer CDDEData::GetBuffer() const
{
	uint nSize = Size();

	CBuffer oBuffer(nSize);

	GetData(oBuffer.Buffer(), nSize, 0);

	return oBuffer;
}

CString CDDEData::GetString(TextFormat eFormat) const
{
	CString str;
	size_t  nBytes = Size();

	if (eFormat == ANSI_TEXT)
	{
		size_t nChars = nBytes;

		// Allocate the string buffer.
		str.BufferSize(nChars+1);

#ifdef ANSI_BUILD
		// Copy the data contents directly into the string.
		GetData(reinterpret_cast<byte*>(str.Buffer()), nSize, 0);
#else
		// Copy the ANSI string to a temporary buffer.
		char* psz = static_cast<char*>(_alloca(nChars));

		GetData(reinterpret_cast<byte*>(psz), nBytes, 0);

		// Convert to Unicode.
		Core::AnsiToWide(psz, psz+nChars, str.Buffer());
#endif
		// Ensure its null terminated.
		str[nChars] = TXT('\0');
	}
	else // (eFormat == UNICODE_TEXT)
	{
		ASSERT((nBytes % 2) == 0);

		size_t nChars = nBytes / 2;

		// Allocate the string buffer.
		str.BufferSize(nChars+1);

#ifdef ANSI_BUILD
		// Copy the Unicode string to a temporary buffer.
		wchar_t* psz = static_cast<wchar_t*>(_alloca(nChars));

		GetData(reinterpret_cast<byte*>(psz), nBytes, 0);

		// Convert to ANSI.
		Core::WideToAnsi(psz, psz+nChars, str.Buffer());
#else
		// Copy the data contents directly into the string.
		GetData(reinterpret_cast<byte*>(str.Buffer()), nBytes, 0);
#endif
		// Ensure its null terminated.
		str[nChars] = TXT('\0');
	}

	return str;
}

void CDDEData::SetData(const void* pBuffer, uint nSize, uint nOffset)
{
	ASSERT(m_pHandle != NULL);
	ASSERT((pBuffer != NULL) || (nSize == 0));

	LPBYTE lpData = static_cast<byte*>(const_cast<void*>(pBuffer));

	HDDEDATA hData = ::DdeAddData(m_pHandle->m_hData, lpData, nSize, nOffset);

	if (hData == NULL)
		throw CDDEException(CDDEException::E_ALLOC_FAILED, m_pHandle->m_pInst->LastError());

	m_pHandle->m_hData = hData;
}

void CDDEData::SetAnsiString(const CString& str, TextFormat eFormat)
{
	size_t nChars = str.Length();

	if (eFormat == ANSI_TEXT)
	{
		size_t nBytes = Core::NumBytes<char>(nChars+1);

#ifdef ANSI_BUILD
		SetData(str.Buffer(), nBytes);
#else
		SetData(T2A(str.Buffer()), nBytes);
#endif
	}
	else // (eFormat == UNICODE_TEXT)
	{
		size_t nBytes = Core::NumBytes<wchar_t>(nChars+1);

#ifdef ANSI_BUILD
		SetData(T2W(str.Buffer()), nBytes);
#else
		SetData(str.Buffer(), nBytes);
#endif
	}
}

void CDDEData::Free()
{
	ASSERT(m_pHandle != NULL);

	if (m_pHandle->m_hData != NULL)
		::DdeFreeDataHandle(m_pHandle->m_hData);

	m_pHandle->m_hData = NULL;
}
