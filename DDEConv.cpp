/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDECONV.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CDDEConv class definition.
**
*******************************************************************************
*/

#include "ncl.hpp"

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

CDDEConv::CDDEConv(CDDEInst* pInst, HCONV hConv, const char* pszService, const char* pszTopic)
	: m_pInst(pInst)
	, m_hConv(hConv)
	, m_strService(pszService)
	, m_strTopic(pszTopic)
{
	ASSERT(m_pInst      != NULL);
	ASSERT(m_hConv      != NULL);
	ASSERT(m_strService != "");
	ASSERT(m_strTopic   != "");
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

CDDEConv::~CDDEConv()
{
}
