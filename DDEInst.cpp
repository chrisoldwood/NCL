/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDEINST.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CDDEInst class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "DDEInst.hpp"

/******************************************************************************
** Method:		GetErrorCode()
**
** Description:	Convert the error code to a string.
**
** Parameters:	nError		The error code.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CString CDDEInst::GetErrorCode(uint nError)
{
	// Known error codes.
	switch (nError)
	{
		case DMLERR_NO_ERROR:				return TXT("DMLERR_NO_ERROR");
		case DMLERR_ADVACKTIMEOUT:			return TXT("DMLERR_ADVACKTIMEOUT");
		case DMLERR_BUSY:					return TXT("DMLERR_BUSY");
		case DMLERR_DATAACKTIMEOUT:			return TXT("DMLERR_DATAACKTIMEOUT");
		case DMLERR_DLL_NOT_INITIALIZED:	return TXT("DMLERR_DLL_NOT_INITIALIZED");
		case DMLERR_DLL_USAGE:				return TXT("DMLERR_DLL_USAGE");
		case DMLERR_EXECACKTIMEOUT:			return TXT("DMLERR_EXECACKTIMEOUT");
		case DMLERR_INVALIDPARAMETER:		return TXT("DMLERR_INVALIDPARAMETER");
		case DMLERR_LOW_MEMORY:				return TXT("DMLERR_LOW_MEMORY");
		case DMLERR_MEMORY_ERROR:			return TXT("DMLERR_MEMORY_ERROR");
		case DMLERR_NOTPROCESSED:			return TXT("DMLERR_NOTPROCESSED");
		case DMLERR_NO_CONV_ESTABLISHED:	return TXT("DMLERR_NO_CONV_ESTABLISHED");
		case DMLERR_POKEACKTIMEOUT:			return TXT("DMLERR_POKEACKTIMEOUT");
		case DMLERR_POSTMSG_FAILED:			return TXT("DMLERR_POSTMSG_FAILED");
		case DMLERR_REENTRANCY:				return TXT("DMLERR_REENTRANCY");
		case DMLERR_SERVER_DIED:			return TXT("DMLERR_SERVER_DIED");
		case DMLERR_SYS_ERROR:				return TXT("DMLERR_SYS_ERROR");
		case DMLERR_UNADVACKTIMEOUT:		return TXT("DMLERR_UNADVACKTIMEOUT");
		case DMLERR_UNFOUND_QUEUE_ID:		return TXT("DMLERR_UNFOUND_QUEUE_ID");
	}

	// Unknown error code.
	return CString::Fmt(TXT("0x%08X"), nError);
}
