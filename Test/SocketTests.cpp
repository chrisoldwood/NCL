////////////////////////////////////////////////////////////////////////////////
//! \file   SocketTests.cpp
//! \brief  The unit tests for the CSocket class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <NCL/Socket.hpp>
#include <WCL/Module.hpp>
#include <NCL/AutoWinSock.hpp>

TEST_SET(Socket)
{
	CModule module;
	AutoWinSock autoWinSock;

TEST_CASE("a hostname can be resolved to an internet address")
{
	in_addr address = CSocket::Resolve(TXT("localhost"));

	TEST_TRUE(address.S_un.S_addr != 0);
}
TEST_CASE_END

TEST_CASE("failing to resolve a hostname throws")
{
	TEST_THROWS(CSocket::Resolve(TXT("invalid hostname")));
}
TEST_CASE_END

TEST_CASE("a hostname can be resolved to the IP address as a string")
{
	CString address = CSocket::ResolveStr(TXT("localhost"));

	TEST_TRUE(address == TXT("127.0.0.1"));
}
TEST_CASE_END

TEST_CASE("an ip address resolves to the same IP address as a string")
{
	CString address = CSocket::ResolveStr(TXT("127.0.0.1"));

	TEST_TRUE(address == TXT("127.0.0.1"));
}
TEST_CASE_END

TEST_CASE("failing to resolve a hostname to a string throws")
{
	TEST_THROWS(CSocket::ResolveStr(TXT("invalid hostname")));
}
TEST_CASE_END

TEST_CASE("trying to resolve an unknown hostname returns false")
{
	tstring address;

	TEST_FALSE(CSocket::tryResolveHostname(TXT("invalid hostname"), address));
}
TEST_CASE_END

TEST_CASE("trying to resolve a known hostname returns true and the host address")
{
	tstring address;
	bool    succeeded = CSocket::tryResolveHostname(TXT("localhost"), address);

	TEST_TRUE(succeeded);
	TEST_TRUE(address == TXT("127.0.0.1"));
}
TEST_CASE_END

TEST_CASE("trying to resolve a valid ip address returns true and the same host address")
{
	tstring address;
	bool    succeeded = CSocket::tryResolveHostname(TXT("127.0.0.1"), address);

	TEST_TRUE(succeeded);
	TEST_TRUE(address == TXT("127.0.0.1"));
}
TEST_CASE_END

TEST_CASE("testing if an unknown hostname can be resolved returns false")
{
	TEST_FALSE(CSocket::canResolveHostname(TXT("invalid hostname")));
}
TEST_CASE_END

TEST_CASE("testing if a known hostname can be resolved returns true")
{
	TEST_TRUE(CSocket::canResolveHostname(TXT("localhost")));
}
TEST_CASE_END

TEST_CASE("testing if a valid ip address can be resolved returns true")
{
	TEST_TRUE(CSocket::canResolveHostname(TXT("127.0.0.1")));
}
TEST_CASE_END

}
TEST_SET_END
