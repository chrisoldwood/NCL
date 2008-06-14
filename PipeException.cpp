/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PIPEEXCEPTION.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CPipeException class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "PipeException.hpp"
#include <WCL/StrCvt.hpp>
#include <Core/StringUtils.hpp>

/******************************************************************************
** Method:		Constructor.
**
** Description:	Formats an exception which includes GetLastError().
**
** Parameters:	eErrCode	The exception code
**				hResult		The result of GetLastError().
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CPipeException::CPipeException(int eErrCode, HRESULT hResult)
	: m_hResult(hResult)
{
	// Convert error to string.
	switch(eErrCode)
	{
		case E_CREATE_FAILED:
			m_strDetails = Core::Fmt(TXT("Failed to create pipe:\n\n%s"), CStrCvt::FormatError(hResult));
			break;

		case E_OPEN_FAILED:
			m_strDetails = Core::Fmt(TXT("Failed to open pipe:\n\n%s"), CStrCvt::FormatError(hResult));
			break;

		case E_ACCEPT_FAILED:
			m_strDetails = Core::Fmt(TXT("Failed to accept pipe connection:\n\n%s"), CStrCvt::FormatError(hResult));
			break;

		case E_PEEK_FAILED:
			m_strDetails = Core::Fmt(TXT("Failed to peek from pipe:\n\n%s"), CStrCvt::FormatError(hResult));
			break;

		case E_READ_FAILED:
			m_strDetails = Core::Fmt(TXT("Failed to read from pipe:\n\n%s"), CStrCvt::FormatError(hResult));
			break;

		case E_WRITE_FAILED:
			m_strDetails = Core::Fmt(TXT("Failed to write to pipe:\n\n%s"), CStrCvt::FormatError(hResult));
			break;

		case E_BAD_PROTOCOL:
			m_strDetails = Core::Fmt(TXT("Invalid pipe protocol version"));
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

CPipeException::~CPipeException()
{
}
