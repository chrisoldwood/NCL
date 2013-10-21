////////////////////////////////////////////////////////////////////////////////
//! \file   DDEServerFactoryTests.cpp
//! \brief  The unit tests for the DDEServerFactory class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <NCL/DDEServerFactory.hpp>

bool testServerCreated = false;

class TestServer : public DDE::IDDEServer
{
public:
	static DDE::IDDEServerPtr create(DWORD flags)
	{
		testServerCreated = true;

		return DDE::IDDEServerPtr(new CDDEServer(flags));
	}
};

TEST_SET(DDEServerFactory)
{

TEST_CASE("factory creates a real DDE server by default")
{
	DDE::IDDEServerPtr server = DDE::DDEServerFactory::create();

	TEST_TRUE(server.get() != nullptr);
}
TEST_CASE_END

TEST_CASE("factory invokes custom creater when overridden")
{
	DDE::DDEServerFactory::registerFactory(TestServer::create);

	DDE::IDDEServerPtr server = DDE::DDEServerFactory::create();

	TEST_TRUE(testServerCreated == true);
}
TEST_CASE_END

}
TEST_SET_END
