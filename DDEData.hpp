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
	CDDEData(CDDEInst* pInst, HDDEDATA hData, uint nFormat, bool bOwn);
	CDDEData(CDDEInst* pInst, HSZ hItem, uint nFormat, bool bOwn);
	CDDEData(CDDEInst* pInst, const void* pBuffer, uint nSize, uint nOffset, uint nFormat, bool bOwn);
	CDDEData(CDDEInst* pInst, const CBuffer& oBuffer, uint nFormat, bool bOwn);
	CDDEData(const CDDEData& oData);
	~CDDEData();

	//
	// Operators.
	//
	CDDEData& operator=(const CDDEData& oData);

	//
	// Accessors.
	//
	HDDEDATA Handle() const;
	uint     Size() const;
	uint     GetData(void* pBuffer, uint nSize, uint nOffset = 0) const;
	CBuffer  GetBuffer() const;
	CString  GetString() const;

	//
	// Mutators.
	//
	void SetData(const void* pBuffer, uint nSize, uint nOffset = 0);
	void SetBuffer(const CBuffer& oBuffer);
	void SetString(const char* pszString);

	void Free();

protected:
	// Forward declarations.
	class CHandle;

	//
	// Members.
	//
	CHandle* m_pHandle;
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
	CHandle(CDDEInst* pInst, HDDEDATA hData, uint nFormat, bool bOwn);
	~CHandle();

	//
	// Members.
	//
	uint		m_nRefCount;	// The reference count.
	CDDEInst*	m_pInst;		// The instance handle.
	HDDEDATA	m_hData;		// The data handle.
	uint		m_nFormat;		// The data format.
	bool		m_bOwn;			// Ownership flag.
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

inline CDDEData::CHandle::CHandle(CDDEInst* pInst, HDDEDATA hData, uint nFormat, bool bOwn)
	: m_nRefCount(1)
	, m_pInst(pInst)
	, m_hData(hData)
	, m_nFormat(nFormat)
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

inline CDDEData::CDDEData(CDDEInst* pInst, HDDEDATA hData, uint nFormat, bool bOwn)
	: m_pHandle(new CHandle(pInst, hData, nFormat, bOwn))
{
}

inline CDDEData::CDDEData(CDDEInst* pInst, HSZ hItem, uint nFormat, bool bOwn)
{
	// Allocate data handle.
	HDDEDATA hData = ::DdeCreateDataHandle(pInst->Handle(), NULL, 0, 0, hItem, nFormat, 0);

	if (hData == NULL)
		throw CDDEException(CDDEException::E_ALLOC_FAILED, pInst->LastError());

	// Attach handle.
	m_pHandle = new CHandle(pInst, hData, nFormat, bOwn);
}

inline CDDEData::CDDEData(CDDEInst* pInst, const void* pBuffer, uint nSize, uint nOffset, uint nFormat, bool bOwn)
{
	// Allocate data handle.
	HDDEDATA hData = ::DdeCreateDataHandle(pInst->Handle(), (byte*)pBuffer, nSize, nOffset, NULL, nFormat, 0);

	if (hData == NULL)
		throw CDDEException(CDDEException::E_ALLOC_FAILED, pInst->LastError());

	// Attach handle.
	m_pHandle = new CHandle(pInst, hData, nFormat, bOwn);
}

inline CDDEData::CDDEData(CDDEInst* pInst, const CBuffer& oBuffer, uint nFormat, bool bOwn)
{
	// Allocate data handle.
	HDDEDATA hData = ::DdeCreateDataHandle(pInst->Handle(), (byte*)oBuffer.Buffer(), oBuffer.Size(), 0, NULL, nFormat, 0);

	if (hData == NULL)
		throw CDDEException(CDDEException::E_ALLOC_FAILED, pInst->LastError());

	// Attach handle.
	m_pHandle = new CHandle(pInst, hData, nFormat, bOwn);
}

inline CDDEData::CDDEData(const CDDEData& oData)
	: m_pHandle(oData.m_pHandle)
{
	ASSERT(m_pHandle != NULL);

	++(m_pHandle->m_nRefCount);
}

inline CDDEData::~CDDEData()
{
	ASSERT(m_pHandle != NULL);

	if (--(m_pHandle->m_nRefCount) == 0)
		delete m_pHandle;
}

inline CDDEData& CDDEData::operator=(const CDDEData& oData)
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

inline HDDEDATA CDDEData::Handle() const
{
	ASSERT(m_pHandle != NULL);

	return m_pHandle->m_hData;
}

inline uint CDDEData::Size() const
{
	ASSERT(m_pHandle != NULL);

	if (m_pHandle->m_hData == NULL)
		return 0;

	return ::DdeGetData(m_pHandle->m_hData, NULL, 0, 0);
}

inline uint CDDEData::GetData(void* pBuffer, uint nSize, uint nOffset) const
{
	ASSERT(m_pHandle != NULL);
	ASSERT(m_pHandle->m_hData != NULL);
	ASSERT(pBuffer != NULL);
	ASSERT(nOffset+nSize <= Size());

	return ::DdeGetData(m_pHandle->m_hData, (byte*)pBuffer, nSize, nOffset);
}

inline CBuffer CDDEData::GetBuffer() const
{
	uint nSize = Size();

	CBuffer oBuffer(nSize);

	GetData(oBuffer.Buffer(), nSize, 0);

	return oBuffer;
}

inline CString CDDEData::GetString() const
{
	uint nSize = Size();

	// Allocate a temporary buffer.
	char* psz = (char*) _alloca(nSize+1);

	// Get the data contents.
	GetData((byte*)psz, nSize, 0);

	// Ensure its null terminated.
	psz[nSize] = '\0';

	return CString(psz);
}

inline void CDDEData::SetData(const void* pBuffer, uint nSize, uint nOffset)
{
	ASSERT(m_pHandle != NULL);
	ASSERT((pBuffer != NULL) || (nSize == 0));

	HDDEDATA hData = ::DdeAddData(m_pHandle->m_hData, (byte*)pBuffer, nSize, nOffset);

	if (hData == NULL)
		throw CDDEException(CDDEException::E_ALLOC_FAILED, m_pHandle->m_pInst->LastError());

	m_pHandle->m_hData = hData;
}

inline void CDDEData::SetBuffer(const CBuffer& oBuffer)
{
	SetData(oBuffer.Buffer(), oBuffer.Size());
}

inline void CDDEData::SetString(const char* pszString)
{
	// NB: Must include null terminator.
	SetData(pszString, strlen(pszString)+1);
}

inline void CDDEData::Free()
{
	ASSERT(m_pHandle != NULL);

	if (m_pHandle->m_hData != NULL)
		::DdeFreeDataHandle(m_pHandle->m_hData);

	m_pHandle->m_hData = NULL;
}

#ifdef _DEBUG
#undef new
#endif

#endif // DDEDATA_HPP
