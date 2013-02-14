////////////////////////////////////////////////////////////////////////////////
//! \file   DDELinkTests.cpp
//! \brief  The unit tests for the DDELink class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <NCL/DDELink.hpp>

TEST_SET(DDELink)
{

TEST_CASE("A link matches the format service|topic!item")
{
	tstring service, topic, item;

	TEST_TRUE(CDDELink::ParseLink(TXT("service|topic!item"), service, topic, item));

	TEST_TRUE(service == TXT("service"));
	TEST_TRUE(topic   == TXT("topic"));
	TEST_TRUE(item    == TXT("item"));
}
TEST_CASE_END

TEST_CASE("Parsing a link with no service, topic or item should fail")
{
	tstring service, topic, item;

	TEST_FALSE(CDDELink::ParseLink(TXT(""), service, topic, item));

	TEST_FALSE(CDDELink::ParseLink(TXT("|!"), service, topic, item));

	TEST_FALSE(CDDELink::ParseLink(TXT("service"), service, topic, item));

	TEST_FALSE(CDDELink::ParseLink(TXT("service|"), service, topic, item));

	TEST_FALSE(CDDELink::ParseLink(TXT("service|topic"), service, topic, item));

	TEST_FALSE(CDDELink::ParseLink(TXT("service|topic!"), service, topic, item));

	TEST_FALSE(CDDELink::ParseLink(TXT("!item"), service, topic, item));

	TEST_FALSE(CDDELink::ParseLink(TXT("|topic!item"), service, topic, item));

	TEST_FALSE(CDDELink::ParseLink(TXT("service|!item"), service, topic, item));
}
TEST_CASE_END

}
TEST_SET_END
