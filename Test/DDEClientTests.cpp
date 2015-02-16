////////////////////////////////////////////////////////////////////////////////
//! \file   DDEClientTests.cpp
//! \brief  The integration tests for the CDDEClient class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <NCL/DDEClient.hpp>
#include <WCL/StrArray.hpp>
#include <NCL/DDECltConvPtr.hpp>

TEST_SET(CDDEClient)
{
	CDDEClient client;

TEST_CASE("querying for all servers should at least find the old shell")
{
	CStrArray servers;

	client.QueryServers(servers);

	TEST_TRUE(servers.Find(TXT("PROGMAN")) != Core::npos);
}
TEST_CASE_END

TEST_CASE("querying a running server should return its topics")
{
	CStrArray topics;

	client.QueryServerTopics(TXT("Shell"), topics);

	TEST_TRUE(topics.Find(TXT("AppProperties")) != Core::npos);
}
TEST_CASE_END

TEST_CASE("querying a non-running server should throw an exception")
{
	CStrArray topics;

	TEST_THROWS(client.QueryServerTopics(TXT("InvalidServerName"), topics));
}
TEST_CASE_END

TEST_CASE("querying for all servers and topics should return two equal-sized arrays")
{
	CStrArray servers, topics;

	client.QueryAll(servers, topics);

	TEST_TRUE(servers.Size() == topics.Size());
	TEST_TRUE(servers.Size() != 0);
}
TEST_CASE_END

TEST_CASE("querying for all servers and topics should return the old shell")
{
	CStrArray servers, topics;

	client.QueryAll(servers, topics);

	TEST_TRUE(servers.Find(TXT("PROGMAN")) != Core::npos);
	TEST_TRUE(topics.Find(TXT("PROGMAN")) != Core::npos);
}
TEST_CASE_END

	const tchar* SERVICE = TXT("PROGMAN");
	const tchar* TOPIC = TXT("PROGMAN");

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
