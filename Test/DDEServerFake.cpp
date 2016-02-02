////////////////////////////////////////////////////////////////////////////////
//! \file   DDEServerFake.cpp
//! \brief  The DDEServerFake class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "DDEServerFake.hpp"
#include <NCL/DDEData.hpp>
#include <WCL/StrArray.hpp>

////////////////////////////////////////////////////////////////////////////////
// Constants.

//! The service name.
const tchar* DDEServerFake::SERVICE = TXT("FAKE_DDE_SERVER");
//! The topic name.
const tchar* DDEServerFake::TOPIC = TXT("FAKE_TOPIC");
//! An item name.
const tchar* DDEServerFake::ITEM = TXT("FAKE_ITEM");
//! The value for the item.
const tchar* DDEServerFake::VALUE = TXT("FAKE_VALUE");

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

DDEServerFake::DDEServerFake()
	: m_server()
{
	m_server.AddListener(this);
	m_server.Register(SERVICE);
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

DDEServerFake::~DDEServerFake()
{
	m_server.Unregister(SERVICE);
}

////////////////////////////////////////////////////////////////////////////////
//! Handle a request for the supported services and topics.

bool DDEServerFake::OnWildConnect(CStrArray& services, CStrArray& topics)
{
	services.Add(SERVICE);
	topics.Add(TOPIC);

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Handle a request for the topics supported by a given service.

bool DDEServerFake::OnWildConnectService(const tchar* service, CStrArray& topics)
{
	if (tstricmp(service, SERVICE) == 0)
	{
		topics.Add(TOPIC);
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////
//! Handle a request for the services supporting a given topic.

bool DDEServerFake::OnWildConnectTopic(const tchar* topic, CStrArray& services)
{
	if (tstricmp(topic, TOPIC) == 0)
	{
		services.Add(SERVICE);
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////
//! Handle a connection request.

bool DDEServerFake::OnConnect(const tchar* service, const tchar* topic)
{
	return ( (tstricmp(service, SERVICE) == 0)
		  && (tstricmp(topic, TOPIC) == 0) );
}

////////////////////////////////////////////////////////////////////////////////
//! Handle a request for an item.

bool DDEServerFake::OnRequest(CDDESvrConv* /*conversation*/, const tchar* item, uint format, CDDEData& data)
{
	if ( (tstricmp(item, ITEM) == 0) && (format == CF_TEXT) )
	{
		data.SetAnsiString(VALUE, ANSI_TEXT);
		return true;
	}

	return false;
}
