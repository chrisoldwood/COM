////////////////////////////////////////////////////////////////////////////////
//! \file   ComUtilsTests.cpp
//! \brief  The unit tests for the COM utility functions.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <COM/ComUtils.hpp>

TEST_SET(ComUtils)
{

TEST_CASE("conversion of a GUID to string returns a 5-part number enclosed in braces")
{
	GUID  oGUID  = { 0x12345678, 0x1234, 0x1234, { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 } };

	TEST_TRUE(COM::FormatGUID(oGUID) == TXT("{12345678-1234-1234-0102-030405060708}"));
}
TEST_CASE_END

TEST_CASE("looking up a CLSID returns the default value associated with the CLSID registry key")
{
	CLSID oCLSID = { 0x0000031A, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };

	TEST_TRUE(COM::LookupCLSID(oCLSID) == TXT("ClassMoniker"));
}
TEST_CASE_END

TEST_CASE("looking up an IID returns the default value associated with the IID registry key")
{
	IID   oIID   = { 0x00000000, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };

	TEST_TRUE(COM::LookupIID(oIID) == TXT("IUnknown"));
}
TEST_CASE_END

}
TEST_SET_END
