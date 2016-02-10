/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDECONV.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CDDEConv class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "DDEConv.hpp"
#include "DDEInst.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

CDDEConv::CDDEConv(CDDEInst* pInst, HCONV hConv, const tchar* pszService, const tchar* pszTopic)
	: m_pInst(pInst)
	, m_hConv(hConv)
	, m_strService(pszService)
	, m_strTopic(pszTopic)
	, m_pAppData(nullptr)
{
	ASSERT(m_pInst != nullptr);
	ASSERT(m_hConv != NULL);
	ASSERT(!m_strService.Empty());
	ASSERT(!m_strTopic.Empty());
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

CDDEConv::~CDDEConv()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Disconnect the conversation.

void CDDEConv::Disconnect()
{
	if (m_hConv != NULL)
	{
		BOOL okay = ::DdeDisconnect(m_hConv);

		if (!okay)
		{
			uint error = m_pInst->LastError();

			TRACE1(TXT("WARNING: DdeDisconnect() returned error code 0x%08X\n"), error);
			DEBUG_USE_ONLY(error);
		}
	}

	m_hConv = NULL;
}
