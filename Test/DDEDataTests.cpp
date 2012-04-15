////////////////////////////////////////////////////////////////////////////////
//! \file   DDEDataTests.cpp
//! \brief  The unit tests for the CDDEData class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <NCL/DDEData.hpp>
#include <NCL/DDEClient.hpp>

TEST_SET(CDDEData)
{
	CDDEClient instance;

TEST_CASE("extracting CF_TEXT value should use text length not buffer size")
{
	const char value[] = "unit test\0garbage";
	const size_t bufsize = Core::numBytes<char>(ARRAY_SIZE(value));
	const size_t length = strlen(value);

	CDDEData data(&instance, value, bufsize, 0, CF_TEXT, true);

	ASSERT(data.Size() != length);

	CString string = data.GetString(ANSI_TEXT);
	size_t  actual = string.Length();

	TEST_TRUE(actual == length);
}
TEST_CASE_END

TEST_CASE("extracting CF_UNICODETEXT value should use text length not buffer size")
{
	const wchar_t value[] = L"unit test\0garbage";
	const size_t bufsize = Core::numBytes<wchar_t>(ARRAY_SIZE(value));
	const size_t length = wcslen(value);

	CDDEData data(&instance, value, bufsize, 0, CF_UNICODETEXT, true);

	ASSERT(data.Size() != length);

	CString string = data.GetString(UNICODE_TEXT);
	size_t  actual = string.Length();

	TEST_TRUE(actual == length);
}
TEST_CASE_END

}
TEST_SET_END
