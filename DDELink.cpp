/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDELINK.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CDDELink class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "DDELink.hpp"
#include "DDEConv.hpp"
#include <WCL/Clipboard.hpp>
#include <Core/AnsiWide.hpp>

/******************************************************************************
**
** Local variables.
**
*******************************************************************************
*/

static uint s_nLinkFormat = CClipboard::RegisterFormat(TXT("Link"));

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

CDDELink::CDDELink(CDDEConv* pConv, const tchar* pszItem, uint nFormat)
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

bool CDDELink::CopyLink(HWND hOwner, const tchar* pszService, const tchar* pszTopic, const tchar* pszItem)
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
			// Strings must be ANSI.
			std::string strService = T2A(pszService);
			std::string strTopic   = T2A(pszTopic);
			std::string strItem    = T2A(pszItem);

			// Format is "SERVICE\0TOPIC\0ITEM\0\0".
			size_t  nChars    = strService.length() + strTopic.length() + strItem.length() + 4;
			size_t  nBytes    = Core::NumBytes<char>(nChars);
			HGLOBAL hLinkData = ::GlobalAlloc(GMEM_MOVEABLE, nBytes);
			HGLOBAL hTextData = ::GlobalAlloc(GMEM_MOVEABLE, nBytes);

			// Allocated blocks?
			if ( (hLinkData != NULL) && (hTextData != NULL) )
			{
				char* pszLinkData = static_cast<char*>(::GlobalLock(hLinkData));

				// Locked "Link" block?
				if (pszLinkData != NULL)
				{
					char* pszData = pszLinkData;

					// Copy to clipboard buffer.
					pszData = strcpy(pszData,                       strService.c_str());
					pszData = strcpy(pszData + strlen(pszData) + 1, strTopic.c_str());
					pszData = strcpy(pszData + strlen(pszData) + 1, strItem.c_str());
					pszData = strcpy(pszData + strlen(pszData) + 1, "");

					::GlobalUnlock(hLinkData);

					// Copy to clipbaord.
					if (::SetClipboardData(s_nLinkFormat, hLinkData) != NULL)
						bCopied = true;
				}

				char* pszTextData = static_cast<char*>(::GlobalLock(hTextData));

				// Locked "Text" block?
				if (pszLinkData != NULL)
				{
					char* pszData = pszTextData;

					// Copy to clipboard buffer.
					strcpy(pszData, strService.c_str());
					strcat(pszData, "|");
					strcat(pszData, strTopic.c_str());
					strcat(pszData, "!");
					strcat(pszData, strItem.c_str());

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
	strLink.Format(TXT("%s|%s!%s"), strService, strTopic, strItem);

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
			const char* psz = static_cast<const char*>(::GlobalLock(hData));

			// Locked block?
			if (psz != NULL)
			{
				// Format is "SERVICE\0TOPIC\0ITEM\0\0" [ANSI].
				const char* pszService = psz;
				const char* pszTopic   = pszService + strlen(pszService) + 1;
				const char* pszItem    = pszTopic   + strlen(pszTopic)   + 1;

				// Copy to return buffers.
				strService = A2T(pszService);
				strTopic   = A2T(pszTopic);
				strItem    = A2T(pszItem);
			
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
