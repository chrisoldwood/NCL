/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDESERVER.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CDDEServer class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef DDESERVER_HPP
#define DDESERVER_HPP

/******************************************************************************
** 
** This class provides DDE Server services.
**
*******************************************************************************
*/

class CDDEServer : public CDDEInst
{
public:
	virtual ~CDDEServer();

	//
	// Singleton methods.
	//
	static CDDEServer* Instance();

	//
	// Accessors.
	//

	//
	// Initialisation methods.
	//
	void Initialise(DWORD dwFlags = CBF_SKIP_REGISTRATIONS | CBF_SKIP_UNREGISTRATIONS);
	void Uninitialise();
	void Register(const char* pszService);
	void Unregister(const char* pszService);

	//
	// Conversation methods.
	//
	CDDESvrConv* FindConversation(const char* pszService, const char* pszTopic);
	CDDESvrConv* FindConversation(HCONV hConv);

	//
	// Event listener methods.
	//
	void AddListener(IDDEServerListener* pListener);
	void RemoveListener(IDDEServerListener* pListener);

protected:
	// Template shorthands.
	typedef TPtrArray<CDDESvrConv> CConvs;
	typedef TPtrArray<IDDEServerListener> CListeners;

	//
	// Members.
	//
	CConvs		m_aoConvs;		// The list of conversations.
	CListeners	m_aoListeners;	// The list of event listeners.

	//
	// Constructors/Destructor.
	//
	CDDEServer();
	CDDEServer(const CDDEServer&);

	//
	// DDECallback handlers.
	//
	bool OnConnect(const char* pszService, const char* pszTopic);
	void OnConnectConfirm(HCONV hConv, const char* pszService, const char* pszTopic);
	void OnDisconnect(HCONV hConv);
	bool OnRequest(HCONV hConv, const char* pszItem, uint nFormat, CDDEData& oData);
	bool OnAdviseStart(HCONV hConv, const char* pszItem, uint nFormat);
	void OnAdviseStop(HCONV hConv, const char* pszItem, uint nFormat);

	// The DDE Callback function.
	static HDDEDATA CALLBACK DDECallbackProc(UINT uType, UINT uFormat, HCONV hConv, HSZ hsz1, HSZ hsz2, HDDEDATA hData, DWORD dwData1, DWORD dwData2);

	// The single DDE Client object.
	static CDDEServer* g_pDDEServer;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // DDESERVER_HPP
