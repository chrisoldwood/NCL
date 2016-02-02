////////////////////////////////////////////////////////////////////////////////
//! \file   DDEClientTests.cpp
//! \brief  The integration tests for the CDDEClient class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <NCL/DDEClient.hpp>
#include <WCL/StrArray.hpp>
#include <NCL/DDECltConvPtr.hpp>
#include "DDEServerFake.hpp"

TEST_SET(DDEClient)
{
	// The order of construction is important as the
	// client dtor will hang if it is destroyed first.
	CDDEClient    client;
	DDEServerFake fakeServer;

	const tchar* SERVICE = DDEServerFake::SERVICE;
	const tchar* TOPIC = DDEServerFake::TOPIC;

TEST_CASE("querying for all servers returns the service names")
{
	CStrArray servers;

	client.QueryServers(servers);

	TEST_TRUE(servers.Find(SERVICE) != Core::npos);
}
TEST_CASE_END

TEST_CASE("querying a running server returns the topics it supports")
{
	CStrArray topics;

	client.QueryServerTopics(SERVICE, topics);

	TEST_TRUE(topics.Find(TOPIC) != Core::npos);
}
TEST_CASE_END

TEST_CASE("querying a non-running server throws an exception")
{
	CStrArray topics;

	TEST_THROWS(client.QueryServerTopics(TXT("InvalidServerName"), topics));
}
TEST_CASE_END

TEST_CASE("querying for all servers and topics returns two equal-sized arrays")
{
	CStrArray servers, topics;

	client.QueryAll(servers, topics);

	TEST_TRUE(servers.Size() == topics.Size());
	TEST_TRUE(servers.Size() != 0);
}
TEST_CASE_END

TEST_CASE("querying for all servers and topics returns the server and topic names")
{
	CStrArray servers, topics;

	client.QueryAll(servers, topics);

	TEST_TRUE(servers.Find(SERVICE) != Core::npos);
	TEST_TRUE(topics.Find(TOPIC) != Core::npos);
}
TEST_CASE_END

TEST_CASE("A conversation is created with a service and topic name")
{
	DDE::CltConvPtr conv(client.CreateConversation(SERVICE, TOPIC));

	TEST_FALSE(conv.empty());
	TEST_TRUE(conv->IsConnected());
}
TEST_CASE_END

TEST_CASE("The same conversation can be opened multiple times")
{
	DDE::CltConvPtr first(client.CreateConversation(SERVICE, TOPIC));

	DDE::CltConvPtr second(client.CreateConversation(SERVICE, TOPIC));

	TEST_FALSE(first.empty());
	TEST_FALSE(second.empty());
}
TEST_CASE_END

TEST_CASE("Trying to converse on an invalid service or topic throws an exception")
{
	TEST_THROWS(client.CreateConversation(TXT("InvalidServiceName"), TXT("InvalidTopicName")));
}
TEST_CASE_END

}
TEST_SET_END
