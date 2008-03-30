////////////////////////////////////////////////////////////////////////////////
//! \file   TestComUtils.cpp
//! \brief  The unit tests for the COM util functions.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <COM/ComUtils.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the COM util functions.

void TestComUtils()
{
	GUID  oGUID  = { 0x12345678, 0x1234, 0x1234, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
	CLSID oCLSID = { 0x0000031A, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 };
	IID   oIID   = { 0x00000000, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 };

	TEST_TRUE(COM::FormatGUID(oGUID) == TXT("{12345678-1234-1234-0102-030405060708}"));
	TEST_TRUE(COM::LookupCLSID(oCLSID) == TXT("ClassMoniker"));
	TEST_TRUE(COM::LookupIID(oIID) == TXT("IUnknown"));
}
