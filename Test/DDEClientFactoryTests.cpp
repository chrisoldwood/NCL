////////////////////////////////////////////////////////////////////////////////
//! \file   DDEClientFactoryTests.cpp
//! \brief  The unit tests for the DDEClientFactory class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <NCL/DDEClientFactory.hpp>

bool testClientCreated = false;

class TestClient : DDE::IDDEClient
{
public:
	static DDE::IDDEClientPtr create(DWORD flags)
	{
		testClientCreated = true;

		return DDE::IDDEClientPtr(new CDDEClient(flags));
	}
};

TEST_SET(DDEClientFactory)
{

TEST_CASE("factory creates a real DDE client by default")
{
	DDE::IDDEClientPtr client = DDE::DDEClientFactory::create();

	TEST_TRUE(client.get() != nullptr);
}
TEST_CASE_END

TEST_CASE("factory invokes custom creator when overridden")
{
	DDE::DDEClientFactory::registerFactory(TestClient::create);

	DDE::IDDEClientPtr client = DDE::DDEClientFactory::create();

	TEST_TRUE(testClientCreated == true);
}
TEST_CASE_END

}
TEST_SET_END
