/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDECLIENT.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CDDEClient class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef DDECLIENT_HPP
#define DDECLIENT_HPP

/******************************************************************************
** 
** This class provides DDE Client services.
**
*******************************************************************************
*/

class CDDEClient : public CDDEInst
{
public:
	virtual ~CDDEClient();

	//
	// Singleton methods.
	//
	static CDDEClient* Instance();

	//
	// Accessors.
	//

	//
	// Initialisation methods.
	//
	void Initialise(DWORD dwFlags = CBF_FAIL_ALLSVRXACTIONS | CBF_SKIP_REGISTRATIONS | CBF_SKIP_UNREGISTRATIONS);
	void Uninitialise();

	//
	// Conversation methods.
	//
	CDDECltConv* CreateConversation(const char* pszService, const char* pszTopic);
	void         DestroyConversation(CDDECltConv* pConv);
	CDDECltConv* FindConversation(const char* pszService, const char* pszTopic);
	CDDECltConv* FindConversation(HCONV hConv);

	//
	// Event listener methods.
	//
	void AddListener(IDDEClientListener* pListener);
	void RemoveListener(IDDEClientListener* pListener);

protected:
	// Template shorthands.
	typedef TPtrArray<CDDECltConv> CConvs;
	typedef TPtrArray<IDDEClientListener> CListeners;

	//
	// Members.
	//
	CConvs		m_aoConvs;		// The list of conversations.
	CListeners	m_aoListeners;	// The list of event listeners.

	//
	// Constructors/Destructor.
	//
	CDDEClient();
	CDDEClient(const CDDEClient&);

	//
	// DDECallback handlers.
	//
	void OnRegister(const char* pszBaseName, const char* pszInstName);
	void OnUnregister(const char* pszBaseName, const char* pszInstName);
	void OnDisconnect(HCONV hConv);
	void OnAdvise(HCONV hConv, const char* pszTopic, const char* pszItem, uint nFormat, const CDDEData* pData);

	// The DDE Callback function.
	static HDDEDATA CALLBACK DDECallbackProc(UINT uType, UINT uFormat, HCONV hConv, HSZ hsz1, HSZ hsz2, HDDEDATA hData, DWORD dwData1, DWORD dwData2);

	// The single DDE Client object.
	static CDDEClient* g_pDDEClient;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // DDECLIENT_HPP
