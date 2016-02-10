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
#include "DDEInst.hpp"
#include "DDEException.hpp"
#include <Core/AnsiWide.hpp>
#include <malloc.h>
#include <Core/StringUtils.hpp>

CDDEData::CHandle::CHandle(CDDEInst* pInst, HDDEDATA hData, uint nFormat, bool bOwn)
	: m_nRefCount(1)
	, m_pInst(pInst)
	, m_hData(hData)
	, m_nFormat(nFormat)
	, m_bOwn(bOwn)
{
	ASSERT(pInst != nullptr);
}

CDDEData::CHandle::~CHandle()
{
	ASSERT(m_nRefCount == 0);

	// Free handle?
	if (m_bOwn)
	{
		BOOL okay = ::DdeFreeDataHandle(m_hData);

		ASSERT_RESULT(okay, okay != FALSE);
	}
}

CDDEData::CDDEData(CDDEInst* pInst, HDDEDATA hData, uint nFormat, bool bOwn)
	: m_pHandle(new CHandle(pInst, hData, nFormat, bOwn))
{
}

CDDEData::CDDEData(CDDEInst* pInst, HSZ hItem, uint nFormat, bool bOwn)
	: m_pHandle()
{
	// Allocate data handle.
	HDDEDATA hData = ::DdeCreateDataHandle(pInst->Handle(), nullptr, 0, 0, hItem, nFormat, 0);

	if (hData == NULL)
		throw CDDEException(CDDEException::E_ALLOC_FAILED, pInst->LastError());

	// Attach handle.
	m_pHandle = new CHandle(pInst, hData, nFormat, bOwn);
}

CDDEData::CDDEData(CDDEInst* pInst, const void* pBuffer, size_t nSize, size_t nOffset, uint nFormat, bool bOwn)
	: m_pHandle()
{
	LPBYTE lpData = static_cast<byte*>(const_cast<void*>(pBuffer));

	// Allocate data handle.
	HDDEDATA hData = ::DdeCreateDataHandle(pInst->Handle(), lpData, static_cast<DWORD>(nSize), static_cast<DWORD>(nOffset), NULL, nFormat, 0);

	if (hData == NULL)
		throw CDDEException(CDDEException::E_ALLOC_FAILED, pInst->LastError());

	// Attach handle.
	m_pHandle = new CHandle(pInst, hData, nFormat, bOwn);
}

CDDEData::CDDEData(CDDEInst* pInst, const CBuffer& oBuffer, uint nFormat, bool bOwn)
	: m_pHandle()
{
	LPBYTE lpData = static_cast<byte*>(const_cast<void*>(oBuffer.Buffer()));

	// Allocate data handle.
	HDDEDATA hData = ::DdeCreateDataHandle(pInst->Handle(), lpData, static_cast<DWORD>(oBuffer.Size()), 0, NULL, nFormat, 0);

	if (hData == NULL)
		throw CDDEException(CDDEException::E_ALLOC_FAILED, pInst->LastError());

	// Attach handle.
	m_pHandle = new CHandle(pInst, hData, nFormat, bOwn);
}

CDDEData::CDDEData(const CDDEData& oData)
	: m_pHandle(oData.m_pHandle)
{
	ASSERT(m_pHandle != nullptr);

	++(m_pHandle->m_nRefCount);
}

CDDEData::~CDDEData()
{
	ASSERT(m_pHandle != nullptr);

	if (--(m_pHandle->m_nRefCount) == 0)
		delete m_pHandle;
}

CDDEData& CDDEData::operator=(const CDDEData& oData)
{
	ASSERT(this != &oData);
	ASSERT(m_pHandle != nullptr);
	ASSERT(oData.m_pHandle != nullptr);

	if (--(m_pHandle->m_nRefCount) == 0)
		delete m_pHandle;

	m_pHandle = oData.m_pHandle;

	++(m_pHandle->m_nRefCount);

	return *this;
}

size_t CDDEData::Size() const
{
	ASSERT(m_pHandle != nullptr);

	if (m_pHandle->m_hData == NULL)
		return 0;

	DWORD dwResult = ::DdeGetData(m_pHandle->m_hData, nullptr, 0, 0);

	// The documentation does not say how an error is detected, only that
	// DdeGetLastError can be queried. But DdeGetData does reset it on success,
	// so it might be a previous call that has failed instead.
	if (dwResult == 0)
	{
		UINT uLastError = m_pHandle->m_pInst->LastError();

		ASSERT_RESULT(uLastError, uLastError == DMLERR_NO_ERROR);
	}

	return dwResult;
}

size_t CDDEData::GetData(void* pBuffer, size_t nSize, size_t nOffset) const
{
	ASSERT(m_pHandle != nullptr);
	ASSERT(m_pHandle->m_hData != NULL);
	ASSERT(pBuffer != nullptr);
	ASSERT(nOffset+nSize <= Size());

	DWORD dwResult = ::DdeGetData(m_pHandle->m_hData, static_cast<byte*>(pBuffer),
						static_cast<DWORD>(nSize), static_cast<DWORD>(nOffset));

	// The documentation does not say how an error is detected, only that
	// DdeGetLastError can be queried. But DdeGetData does reset it on success,
	// so it might be a previous call that has failed instead.
	if (dwResult == 0)
	{
		UINT uLastError = m_pHandle->m_pInst->LastError();

		ASSERT_RESULT(uLastError, uLastError == DMLERR_NO_ERROR);
	}

	return dwResult;
}

CBuffer CDDEData::GetBuffer() const
{
	size_t nSize = Size();

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
		GetData(reinterpret_cast<byte*>(str.Buffer()), nBytes, 0);
#else
		// Copy the ANSI string to a temporary buffer.
		char* psz = static_cast<char*>(_alloca(nChars));

		GetData(reinterpret_cast<byte*>(psz), nBytes, 0);

		nChars = strnlen(psz, nChars);

		// Convert to Unicode.
		Core::ansiToWide(psz, psz+nChars, str.Buffer());
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

		nChars = wcsnlen(psz, nChars);

		// Convert to ANSI.
		Core::wideToAnsi(psz, psz+nChars, str.Buffer());
#else
		// Copy the data contents directly into the string.
		GetData(reinterpret_cast<byte*>(str.Buffer()), nBytes, 0);
#endif
		// Ensure its null terminated.
		str[nChars] = TXT('\0');
	}

	return str;
}

void CDDEData::SetData(const void* pBuffer, size_t nSize, size_t nOffset)
{
	ASSERT(m_pHandle != nullptr);
	ASSERT((pBuffer != nullptr) || (nSize == 0));

	LPBYTE lpData = static_cast<byte*>(const_cast<void*>(pBuffer));

	HDDEDATA hData = ::DdeAddData(m_pHandle->m_hData, lpData, static_cast<DWORD>(nSize), static_cast<DWORD>(nOffset));

	if (hData == NULL)
		throw CDDEException(CDDEException::E_ALLOC_FAILED, m_pHandle->m_pInst->LastError());

	m_pHandle->m_hData = hData;
}

void CDDEData::SetAnsiString(const CString& str, TextFormat eFormat)
{
	size_t nChars = str.Length();

	if (eFormat == ANSI_TEXT)
	{
		size_t nBytes = Core::numBytes<char>(nChars+1);

#ifdef ANSI_BUILD
		SetData(str.Buffer(), nBytes);
#else
		SetData(T2A(str.Buffer()), nBytes);
#endif
	}
	else // (eFormat == UNICODE_TEXT)
	{
		size_t nBytes = Core::numBytes<wchar_t>(nChars+1);

#ifdef ANSI_BUILD
		SetData(T2W(str.Buffer()), nBytes);
#else
		SetData(str.Buffer(), nBytes);
#endif
	}
}

void CDDEData::Free()
{
	ASSERT(m_pHandle != nullptr);

	if (m_pHandle->m_hData != NULL)
	{
		BOOL okay = ::DdeFreeDataHandle(m_pHandle->m_hData);

		ASSERT_RESULT(okay, okay != FALSE);
	}

	m_pHandle->m_hData = NULL;
}
