/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DEFDDECLIENTLISTENER.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CDefDDEClientListener class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef DEFDDECLIENTLISTENER_HPP
#define DEFDDECLIENTLISTENER_HPP

/******************************************************************************
** 
** Default implementation of IDDEClientListener.
**
*******************************************************************************
*/

class CDefDDEClientListener : public IDDEClientListener
{
public:
	//
	// Methods.
	//
	virtual void OnRegister(const char* pszBaseName, const char* pszInstName);
	virtual void OnUnregister(const char* pszBaseName, const char* pszInstName);
	virtual void OnDisconnect(CDDECltConv* pConv);
	virtual void OnAdvise(CDDELink* pLink, const CDDEData* pData);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline void CDefDDEClientListener::OnRegister(const char* pszBaseName, const char* pszInstName)
{
}

inline void CDefDDEClientListener::OnUnregister(const char* pszBaseName, const char* pszInstName)
{
}

inline void CDefDDEClientListener::OnDisconnect(CDDECltConv* pConv)
{
}

inline void CDefDDEClientListener::OnAdvise(CDDELink* pLink, const CDDEData* pData)
{
}

#endif // DEFDDECLIENTLISTENER_HPP
