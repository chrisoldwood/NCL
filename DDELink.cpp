/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDELINK.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CDDELink class definition.
**
*******************************************************************************
*/

#include "ncl.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

/******************************************************************************
**
** Local variables.
**
*******************************************************************************
*/

static uint s_nLinkFormat = CClipboard::RegisterFormat("Link");

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

CDDELink::CDDELink(CDDEConv* pConv, const char* pszItem, uint nFormat)
	: m_nRefCount(0)
	, m_pConv(pConv)
	, m_strItem(pszItem)
	, m_nFormat(nFormat)
	, m_pAppData(NULL)
{
}

/******************************************************************************
** Method:		CopyLink()
**
** Description:	Copy the link to the clipboard.
**
** Parameters:	hOwner			The owner window.
**				pLink			The link to copy.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CDDELink::CopyLink(HWND hOwner, const CDDELink* pLink)
{
	ASSERT(::IsWindow(hOwner));
	ASSERT(pLink != NULL);
	ASSERT(pLink->Conversation() != NULL);

	CDDEConv* pConv = pLink->Conversation();

	return CopyLink(hOwner, pConv->Service(), pConv->Topic(), pLink->Item());
}

/******************************************************************************
** Method:		CopyLink()
**
** Description:	Copy the link to the clipboard.
**
** Parameters:	hOwner			The owner window.
**				pszService		The service name.
**				pszTopic		The topic name.
**				pszItem			The item name.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CDDELink::CopyLink(HWND hOwner, const char* pszService, const char* pszTopic, const char* pszItem)
{
	ASSERT(::IsWindow(hOwner));
	ASSERT(pszService != NULL);
	ASSERT(pszTopic   != NULL);
	ASSERT(pszItem    != NULL);

	bool bCopied = false;

	// Open the clipboard.
	if (::OpenClipboard(hOwner))
	{
		// Clear existing contents.
		if (::EmptyClipboard())
		{
			// Format is "SERVICE\0TOPIC\0ITEM\0\0".
			int     nLen      = strlen(pszService) + strlen(pszTopic) + strlen(pszItem) + 4;
			HGLOBAL hLinkData = ::GlobalAlloc(GMEM_MOVEABLE, nLen);
			HGLOBAL hTextData = ::GlobalAlloc(GMEM_MOVEABLE, nLen);

			// Allocated blocks?
			if ( (hLinkData != NULL) && (hTextData != NULL) )
			{
				char* pszLinkData = (char*) ::GlobalLock(hLinkData);

				// Locked "Link" block?
				if (pszLinkData != NULL)
				{
					char* pszData = pszLinkData;

					// Copy to clipboard buffer.
					pszData = strcpy(pszData,                       pszService);
					pszData = strcpy(pszData + strlen(pszData) + 1, pszTopic);
					pszData = strcpy(pszData + strlen(pszData) + 1, pszItem);
					pszData = strcpy(pszData + strlen(pszData) + 1, "");

					::GlobalUnlock(hLinkData);

					// Copy to clipbaord.
					if (::SetClipboardData(s_nLinkFormat, hLinkData) != NULL)
						bCopied = true;
				}

				char* pszTextData = (char*) ::GlobalLock(hTextData);

				// Locked "Text" block?
				if (pszLinkData != NULL)
				{
					char* pszData = pszTextData;

					// Copy to clipboard buffer.
					strcpy(pszData, pszService);
					strcat(pszData, "|");
					strcat(pszData, pszTopic);
					strcat(pszData, "!");
					strcat(pszData, pszItem);

					::GlobalUnlock(hTextData);

					// Copy to clipbaord.
					if (::SetClipboardData(CF_TEXT, hTextData) != NULL)
						bCopied = true;
				}
			}
		}

		// Close it.
		::CloseClipboard();
	}

	return bCopied;
}

/******************************************************************************
** Method:		CanPasteLink()
**
** Description:	Queries if a link is on the clipboard.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CDDELink::CanPasteLink()
{
	return CClipboard::IsFormatAvail(s_nLinkFormat);
}

/******************************************************************************
** Method:		PasteLink()
**
** Description:	Get a DDE "link" from the clipboard as a single string.
**				NB: Returns the link in "SERVICE|TOPIC!ITEM" format.
**
** Parameters:	strLink		The returned link.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CDDELink::PasteLink(CString& strLink)
{
	CString strService, strTopic, strItem;

	// Get raw link.
	if (!PasteLink(strService, strTopic, strItem))
		return false;

	// Format.
	strLink.Format("%s|%s!%s", strService, strTopic, strItem);

	return true;
}

/******************************************************************************
** Method:		PasteLink()
**
** Description:	Get a DDE "link" from the clipboard.
**
** Parameters:	strService		The service name.
**				strTopic		The topic name.
**				strItem			The item name.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CDDELink::PasteLink(CString& strService, CString& strTopic, CString& strItem)
{
	bool bPasted = false;

	// Open the clipboard.
	if (::OpenClipboard(NULL))
	{
		HGLOBAL hData = ::GetClipboardData(s_nLinkFormat);

		// Got data?
		if (hData != NULL)
		{
			const char* psz = (const char*) ::GlobalLock(hData);

			// Locked block?
			if (psz != NULL)
			{
				// Format is "SERVICE\0TOPIC\0ITEM\0\0".
				const char* pszService = psz;
				const char* pszTopic   = pszService + strlen(pszService) + 1;
				const char* pszItem    = pszTopic   + strlen(pszTopic)   + 1;

				// Copy to return buffers.
				strService = pszService;
				strTopic   = pszTopic;
				strItem    = pszItem;
			
				::GlobalUnlock(hData); 

				bPasted = true;
			} 
		}

		// Close it.
		::CloseClipboard();
	}

	return bPasted;
}

/******************************************************************************
** Method:		ParseLink()
**
** Description:	.
**
** Parameters:	.
**
** Returns:		.
**
*******************************************************************************
*/

bool CDDELink::ParseLink(const CString& /*strLink*/, CString& /*strService*/, CString& /*strTopic*/, CString& /*strItem*/)
{
	return false;
}
