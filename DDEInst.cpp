/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDEINST.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CDDEInst class definition.
**
*******************************************************************************
*/

#include "ncl.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

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
		case DMLERR_NO_ERROR:				return "DMLERR_NO_ERROR";
		case DMLERR_ADVACKTIMEOUT:			return "DMLERR_ADVACKTIMEOUT";
		case DMLERR_BUSY:					return "DMLERR_BUSY";
		case DMLERR_DATAACKTIMEOUT:			return "DMLERR_DATAACKTIMEOUT";
		case DMLERR_DLL_NOT_INITIALIZED:	return "DMLERR_DLL_NOT_INITIALIZED";
		case DMLERR_DLL_USAGE:				return "DMLERR_DLL_USAGE";
		case DMLERR_EXECACKTIMEOUT:			return "DMLERR_EXECACKTIMEOUT";
		case DMLERR_INVALIDPARAMETER:		return "DMLERR_INVALIDPARAMETER";
		case DMLERR_LOW_MEMORY:				return "DMLERR_LOW_MEMORY";
		case DMLERR_MEMORY_ERROR:			return "DMLERR_MEMORY_ERROR";
		case DMLERR_NOTPROCESSED:			return "DMLERR_NOTPROCESSED";
		case DMLERR_NO_CONV_ESTABLISHED:	return "DMLERR_NO_CONV_ESTABLISHED";
		case DMLERR_POKEACKTIMEOUT:			return "DMLERR_POKEACKTIMEOUT";
		case DMLERR_POSTMSG_FAILED:			return "DMLERR_POSTMSG_FAILED";
		case DMLERR_REENTRANCY:				return "DMLERR_REENTRANCY";
		case DMLERR_SERVER_DIED:			return "DMLERR_SERVER_DIED";
		case DMLERR_SYS_ERROR:				return "DMLERR_SYS_ERROR";
		case DMLERR_UNADVACKTIMEOUT:		return "DMLERR_UNADVACKTIMEOUT";
		case DMLERR_UNFOUND_QUEUE_ID:		return "DMLERR_UNFOUND_QUEUE_ID";
	}

	// Unknown error code.
	return CString::Fmt("0x%08X", nError);
}
