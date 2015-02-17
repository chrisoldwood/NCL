////////////////////////////////////////////////////////////////////////////////
//! \file   DDEServerFake.hpp
//! \brief  The DDEServerFake class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef NCL_DDESERVERFAKE_HPP
#define NCL_DDESERVERFAKE_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <NCL/DefDDEServerListener.hpp>
#include <NCL/DDEServer.hpp>

////////////////////////////////////////////////////////////////////////////////
//! A mock DDE server for use when testing the client.

class DDEServerFake : CDefDDEServerListener
{
public:
	//! Default constructor.
	DDEServerFake();

	//! Destructor.
	~DDEServerFake();

	//
	// Constants.
	//

	//! The service name.
	static const tchar* SERVICE;
	//! The topic name.
	static const tchar* TOPIC;
	//! An item name.
	static const tchar* ITEM;
	//! The value for the item.
	static const tchar* VALUE;
	
private:
	//
	// Members.
	//
	CDDEServer	m_server;	//!< The DDE server.

	//
	// IDDEServerListener methods.
	//

	//! Handle a request for the supported services and topics.
	virtual bool OnWildConnect(CStrArray& services, CStrArray& topics);

	//! Handle a request for the topics supported by a given service.
	virtual bool OnWildConnectService(const tchar* service, CStrArray& topics);

	//! Handle a request for the services supporting a given topic.
	virtual bool OnWildConnectTopic(const tchar* topic, CStrArray& services);

	//! Handle a connection request.
	virtual bool OnConnect(const tchar* service, const tchar* topic);

	//! Handle a request for an item.
	virtual bool OnRequest(CDDESvrConv* conversation, const tchar* item, uint format, CDDEData& data);
};

#endif // NCL_DDESERVERFAKE_HPP
