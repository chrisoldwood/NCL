/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDEDATA.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CDDEData class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef DDEDATA_HPP
#define DDEDATA_HPP

/******************************************************************************
** 
** This is a helper class for dealing with DDE data.
**
*******************************************************************************
*/

class CDDEData
{
public:
	//
	// Constructors/Destructor.
	//
	CDDEData(CDDEInst* pInst, HDDEDATA hData, bool bOwn = true);
	CDDEData(CDDEInst* pInst, HSZ hItem, uint nFormat = CF_TEXT, uint nFlags = 0, bool bOwn = false);
	CDDEData(const CDDEData& oData);
	~CDDEData();

	//
	// Operators.
	//
	operator HDDEDATA() const;
	CDDEData& operator=(const CDDEData& oData);

	//
	// Accessors.
	//
	uint    Size() const;
	uint    GetData(byte* pBuffer, uint nSize, uint nOffset = 0) const;
	CString GetString() const;

	//
	// Mutators.
	//
	void SetData(byte* pBuffer, uint nSize, uint nOffset = 0);
	void SetString(const char* pszString);

protected:
	// Forward declarations.
	class CHandle;

	//
	// Members.
	//
	CHandle* m_pHandle;

	// Disallow copies.
};

/******************************************************************************
** 
** This is the reference counted body of CDDEData.
**
*******************************************************************************
*/

class CDDEData::CHandle
{
public:
	CHandle(CDDEInst* pInst, HDDEDATA hData, bool bOwn);
	~CHandle();

	//
	// Members.
	//
	uint		m_nRefCount;	// The reference count.
	CDDEInst*	m_pInst;		// The instance handle.
	HDDEDATA	m_hData;		// The data handle.
	bool		m_bOwn;			// Ownership flag.
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CDDEData::CHandle::CHandle(CDDEInst* pInst, HDDEDATA hData, bool bOwn)
	: m_nRefCount(1)
	, m_pInst(pInst)
	, m_hData(hData)
	, m_bOwn(bOwn)
{
	ASSERT(pInst != NULL);
}

inline CDDEData::CHandle::~CHandle()
{
	ASSERT(m_nRefCount == 0);

	// Free handle?
	if (m_bOwn)
		::DdeFreeDataHandle(m_hData);
}

inline CDDEData::CDDEData(CDDEInst* pInst, HDDEDATA hData, bool bOwn)
	: m_pHandle(new CHandle(pInst, hData, bOwn))
{
}

inline CDDEData::CDDEData(CDDEInst* pInst, HSZ hItem, uint nFormat, uint nFlags, bool bOwn)
{
	// Allocate data handle.
	HDDEDATA hData = ::DdeCreateDataHandle(pInst->Handle(), NULL, 0, 0, hItem, nFormat, nFlags);

	if (hData == NULL)
		throw CDDEException(CDDEException::E_ALLOC_FAILED, pInst->LastError());

	// Attach handle.
	m_pHandle = new CHandle(pInst, hData, bOwn);
}

inline CDDEData::CDDEData(const CDDEData& oData)
	: m_pHandle(oData.m_pHandle)
{
	++(m_pHandle->m_nRefCount);
}

inline CDDEData::~CDDEData()
{
	if (--(m_pHandle->m_nRefCount) == 0)
		delete m_pHandle;
}

inline CDDEData::operator HDDEDATA() const
{
	return m_pHandle->m_hData;
}

inline CDDEData& CDDEData::operator=(const CDDEData& oData)
{
	ASSERT(this != &oData);

	if (--(m_pHandle->m_nRefCount) == 0)
		delete m_pHandle;
	
	m_pHandle = oData.m_pHandle;

	++(m_pHandle->m_nRefCount);

	return *this;
}

inline uint CDDEData::Size() const
{
	return ::DdeGetData(m_pHandle->m_hData, NULL, 0, 0);
}

inline uint CDDEData::GetData(byte* pBuffer, uint nSize, uint nOffset) const
{
	return ::DdeGetData(m_pHandle->m_hData, pBuffer, nSize, nOffset);
}

inline CString CDDEData::GetString() const
{
	uint nSize = Size();

	// Allocate a temporary buffer.
	char* psz = (char*) _alloca(nSize+1);

	// Get the data contents.
	GetData((byte*)psz, nSize+1, 0);

	// Ensure its null terminated.
	psz[nSize] = '\0';

	return CString(psz);
}

inline void CDDEData::SetData(byte* pBuffer, uint nSize, uint nOffset)
{
	HDDEDATA hData = ::DdeAddData(m_pHandle->m_hData, pBuffer, nSize, nOffset);

	if (hData == NULL)
		throw CDDEException(CDDEException::E_ALLOC_FAILED, m_pHandle->m_pInst->LastError());

	m_pHandle->m_hData = hData;
}

inline void CDDEData::SetString(const char* pszString)
{
	SetData((byte*)pszString, strlen(pszString)+1);
}

#endif // DDEDATA_HPP
