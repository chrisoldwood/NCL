////////////////////////////////////////////////////////////////////////////////
//! \file   DDEServerTests.cpp
//! \brief  The unit tests for the CDDEServer class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <NCL/DDEServer.hpp>
#include <WCL/Buffer.hpp>

TEST_SET(CDDEServer)
{

TEST_CASE("detected data format will be CF_TEXT when less than 4 bytes long")
{
	const char* buffer = "1234";

	TEST_TRUE(CDDEServer::GuessTextFormat(CBuffer(buffer, 0)) == CF_TEXT);
	TEST_TRUE(CDDEServer::GuessTextFormat(CBuffer(buffer, 1)) == CF_TEXT);
	TEST_TRUE(CDDEServer::GuessTextFormat(CBuffer(buffer, 2)) == CF_TEXT);
	TEST_TRUE(CDDEServer::GuessTextFormat(CBuffer(buffer, 3)) == CF_TEXT);
}
TEST_CASE_END

TEST_CASE("detected data format will be CF_TEXT when data length is odd number of bytes")
{
	const char* buffer = "1234"; // 4 bytes + EOL

	TEST_TRUE((Core::numBytes(buffer)+Core::numBytes<char>(1)) % 2 == 1);
	TEST_TRUE(CDDEServer::GuessTextFormat(CBuffer(buffer, 5)) == CF_TEXT);
}
TEST_CASE_END

TEST_CASE("detected data format will be CF_TEXT when data length is even and last but one char is not zero")
{
	const char* buffer = "12345"; // 5 bytes + EOL

	TEST_TRUE((Core::numBytes(buffer)+Core::numBytes<char>(1)) % 2 == 0);
	TEST_TRUE(CDDEServer::GuessTextFormat(CBuffer(buffer, 6)) == CF_TEXT);
}
TEST_CASE_END

TEST_CASE("detected data format will be CF_UNICODE text when data length is even and last two chars are zero")
{
	const wchar_t* buffer = L"1"; // 2 bytes + 2 EOL

	TEST_TRUE((Core::numBytes(buffer)+Core::numBytes<wchar_t>(1)) % 2 == 0);
	TEST_TRUE(CDDEServer::GuessTextFormat(CBuffer(buffer, 4)) == CF_UNICODETEXT);
}
TEST_CASE_END

}
TEST_SET_END
