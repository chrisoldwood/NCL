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

	// Get error code as #defined symbol.
	CString strErrDef = CDDEInst::GetErrorCode(m_nDDECode);

	// Convert error to string.
	switch(eErrCode)
	{
		case E_INIT_FAILED:
			m_strErrorText.Format("Failed to initialise DDE: %s", strErrDef);
			break;

		case E_CONN_FAILED:
			m_strErrorText.Format("Failed to connect to DDE server: %s", strErrDef);
			break;

		case E_STRING_FAILED:
			m_strErrorText.Format("Failed to create DDE string: %s", strErrDef);
			break;

		case E_REQUEST_FAILED:
			m_strErrorText.Format("Failed to request DDE item: %s", strErrDef);
			break;

		case E_LINK_FAILED:
			m_strErrorText.Format("Failed to create DDE advise loop: %s", strErrDef);
			break;

		case E_ALLOC_FAILED:
			m_strErrorText.Format("Failed to allocate DDE data handle: %s", strErrDef);
			break;

		case E_QUERY_FAILED:
			m_strErrorText.Format("Failed to query DDE servers: %s", strErrDef);
			break;

		case E_EXECUTE_FAILED:
			m_strErrorText.Format("Failed to execute a DDE command: %s", strErrDef);
			break;

		case E_POKE_FAILED:
			m_strErrorText.Format("Failed to poke an item: %s", strErrDef);
			break;

		// Shouldn't happen!
		default:
			ASSERT_FALSE();
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
