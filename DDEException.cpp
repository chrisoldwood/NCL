/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDEEXCEPTION.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CDDEException class definition.
**
*******************************************************************************
*/

#include "ncl.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CDDEException::CDDEException(int eErrCode, uint nDDECode)
	: m_nDDECode(nDDECode)
{
	m_nErrorCode = eErrCode;

	// Convert error to string.
	switch(eErrCode)
	{
		case E_INIT_FAILED:
			m_strErrorText.Format("Failed to initialise DDE: 0x%08X", m_nDDECode);
			break;

		case E_CONN_FAILED:
			m_strErrorText.Format("Failed to connect to DDE server: 0x%08X", m_nDDECode);
			break;

		case E_STRING_FAILED:
			m_strErrorText.Format("Failed to create DDE string: 0x%08X", m_nDDECode);
			break;

		case E_REQUEST_FAILED:
			m_strErrorText.Format("Failed to request DDE item: 0x%08X", m_nDDECode);
			break;

		case E_LINK_FAILED:
			m_strErrorText.Format("Failed to create DDE advise loop: 0x%08X", m_nDDECode);
			break;

		case E_ALLOC_FAILED:
			m_strErrorText.Format("Failed to allocate DDE data handle: 0x%08X", m_nDDECode);
			break;

		// Shouldn't happen!
		default:
			ASSERT(false);
			break;
	}
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CDDEException::~CDDEException()
{
}
