////////////////////////////////////////////////////////////////////////////////
//! \file   DDEDataTests.cpp
//! \brief  The unit tests for the CDDEData class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <NCL/DDEData.hpp>
#include <NCL/DDEClient.hpp>
#include <Core/AnsiWide.hpp>

TEST_SET(DDEData)
{
	CDDEClient instance;
	const CBuffer empty;

TEST_CASE("extracting CF_TEXT value should use text length not buffer size")
{
	const char value[] = "unit test\0garbage";
	const size_t numChars = ARRAY_SIZE(value);
	const size_t bufsize = Core::numBytes<char>(numChars);
	const size_t length = strlen(value);

	ASSERT(length < numChars);

	CDDEData data(&instance, value, bufsize, 0, CF_TEXT, true);

	ASSERT(data.Size() > length);

	CString string = data.GetString(ANSI_TEXT);
	size_t  actual = string.Length();

	TEST_TRUE(actual != numChars);
	TEST_TRUE(actual == length);
}
TEST_CASE_END

TEST_CASE("extracting CF_TEXT value with no string terminator should equal buffer size")
{
	const char value[] = "unit test";
	const size_t numChars = strlen("unit");
	const size_t bufsize = Core::numBytes<char>(numChars);
	const size_t length = strlen(value);

	ASSERT(numChars < length);

	CDDEData data(&instance, value, bufsize, 0, CF_TEXT, true);

	ASSERT(data.Size() < Core::numBytes<char>(length));

	CString string = data.GetString(ANSI_TEXT);
	size_t  actual = string.Length();

	TEST_TRUE(actual == numChars);

	DEBUG_USE_ONLY(length);
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

TEST_CASE("extracting CF_UNICODETEXT value with no string terminator should equal buffer size")
{
	const wchar_t value[] = L"unit test";
	const size_t numChars = wcslen(L"unit");
	const size_t bufsize = Core::numBytes<wchar_t>(numChars);
	const size_t length = wcslen(value);

	ASSERT(numChars < length);

	CDDEData data(&instance, value, bufsize, 0, CF_UNICODETEXT, true);

	ASSERT(data.Size() < Core::numBytes<wchar_t>(length));

	CString string = data.GetString(UNICODE_TEXT);
	size_t  actual = string.Length();

	TEST_TRUE(actual == numChars);

	DEBUG_USE_ONLY(length);
}
TEST_CASE_END

TEST_CASE("Ansi data buffer can be set from an Ansi string")
{
	const std::string value = "unit test";

	CDDEData data(&instance, empty, CF_TEXT, true);

	data.SetString(A2T(value), ANSI_TEXT);

	CBuffer buffer = data.GetBuffer();

	TEST_TRUE(memcmp(buffer.Buffer(), value.data(), Core::numBytes<char>(value.length()+1)) == 0);
}
TEST_CASE_END

TEST_CASE("Unicode data buffer can be set from an Ansi string")
{
	const std::string ansiValue = "unit test";
	const std::wstring unicodeValue = L"unit test";

	CDDEData data(&instance, empty, CF_UNICODETEXT, true);

	data.SetString(A2T(ansiValue), UNICODE_TEXT);

	CBuffer buffer = data.GetBuffer();

	TEST_TRUE(memcmp(buffer.Buffer(), unicodeValue.data(), Core::numBytes<wchar_t>(unicodeValue.length()+1)) == 0);
}
TEST_CASE_END

TEST_CASE("Unicode data buffer can be set from a Unicode string")
{
	const std::wstring value = L"unit test";

	CDDEData data(&instance, empty, CF_UNICODETEXT, true);

	data.SetString(W2T(value), UNICODE_TEXT);

	CBuffer buffer = data.GetBuffer();

	TEST_TRUE(memcmp(buffer.Buffer(), value.data(), Core::numBytes<wchar_t>(value.length()+1)) == 0);
}
TEST_CASE_END

TEST_CASE("Ansi data buffer can be set from a Unicode string")
{
	const std::string ansiValue = "unit test";
	const std::wstring unicodeValue = L"unit test";

	CDDEData data(&instance, empty, CF_TEXT, true);

	data.SetString(W2T(unicodeValue), ANSI_TEXT);

	CBuffer buffer = data.GetBuffer();

	TEST_TRUE(memcmp(buffer.Buffer(), ansiValue.data(), Core::numBytes<char>(ansiValue.length()+1)) == 0);
}
TEST_CASE_END

}
TEST_SET_END
