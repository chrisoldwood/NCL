////////////////////////////////////////////////////////////////////////////////
//! \file   DDECltConvTests.cpp
//! \brief  The integration tests for the DDECltConv class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <NCL/DDEClient.hpp>
#include <NCL/DDECltConvPtr.hpp>
#include <NCL/DDEData.hpp>

TEST_SET(DDECltConv)
{
	CDDEClient client;

	const tchar* SERVICE = TXT("PROGMAN");
	const tchar* TOPIC = TXT("PROGMAN");
	const tchar* ITEM = TXT("Accessories");

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

	TEST_TRUE(value.Size() != 0);
}
TEST_CASE_END

TEST_CASE("The value for a string-based item can be requested")
{
	DDE::CltConvPtr conv(client.CreateConversation(SERVICE, TOPIC));

	CString value = conv->RequestString(ITEM, CF_TEXT);

	TEST_FALSE(value.Empty());
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
