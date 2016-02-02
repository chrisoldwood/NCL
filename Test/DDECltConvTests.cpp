////////////////////////////////////////////////////////////////////////////////
//! \file   DDECltConvTests.cpp
//! \brief  The integration tests for the DDECltConv class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <NCL/DDEClient.hpp>
#include <NCL/DDECltConvPtr.hpp>
#include <NCL/DDEData.hpp>
#include "DDEServerFake.hpp"

TEST_SET(DDECltConv)
{
	// The order of construction is important as the
	// client dtor will hang if it is destroyed first.
	CDDEClient    client;
	DDEServerFake fakeServer;

	const tchar* SERVICE = DDEServerFake::SERVICE;
	const tchar* TOPIC = DDEServerFake::TOPIC;
	const tchar* ITEM = DDEServerFake::ITEM;
	const tchar* VALUE = DDEServerFake::VALUE;

TEST_CASE("A conversation's connection status can be queried")
{
	DDE::CltConvPtr conv(client.CreateConversation(SERVICE, TOPIC));

	TEST_TRUE(conv->IsConnected());
}
TEST_CASE_END

TEST_CASE("A conversation can be queried for its service and topic")
{
	DDE::CltConvPtr conv(client.CreateConversation(SERVICE, TOPIC));

	TEST_TRUE(conv->Service() == SERVICE);
	TEST_TRUE(conv->Topic() == TOPIC);
}
TEST_CASE_END

TEST_CASE("The value for an item can be requested in different formats")
{
	DDE::CltConvPtr conv(client.CreateConversation(SERVICE, TOPIC));

	CDDEData value = conv->Request(ITEM, CF_TEXT);

	TEST_TRUE(value.GetString(ANSI_TEXT) == VALUE);
}
TEST_CASE_END

TEST_CASE("The value for a string-based item can be requested")
{
	DDE::CltConvPtr conv(client.CreateConversation(SERVICE, TOPIC));

	CString value = conv->RequestString(ITEM, CF_TEXT);

	TEST_TRUE(value == VALUE);
}
TEST_CASE_END

TEST_CASE("A conversation can be copied")
{
	ASSERT(client.GetNumConversations() == 0);

	DDE::CltConvPtr conv(client.CreateConversation(SERVICE, TOPIC));
	DDE::CltConvPtr copy = conv;

	TEST_FALSE(copy.empty());
	TEST_TRUE(client.GetNumConversations() == 1);

	conv.Release();
	TEST_TRUE(client.GetNumConversations() == 1);

	copy.Release();
	TEST_TRUE(client.GetNumConversations() == 0);
}
TEST_CASE_END

}
TEST_SET_END
