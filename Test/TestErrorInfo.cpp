////////////////////////////////////////////////////////////////////////////////
//! \file   TestErrorInfo.cpp
//! \brief  The unit tests for the COM IErrorInfo error handling.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <WCL/ComPtr.hpp>
#include <WCL/ComStr.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the SetComErrorInfo function.

void TestErrorInfo()
{
	typedef WCL::ComPtr<IErrorInfo> IErrorInfoPtr;

	TEST_TRUE(COM::SetComErrorInfo(__FUNCTION__, TXT("Description")));

	IErrorInfoPtr pErrorInfo;

	TEST_TRUE(::GetErrorInfo(0, AttachTo(pErrorInfo)) == S_OK);

	GUID		oGUID = { 0 };
	WCL::ComStr bstrSource;
	WCL::ComStr bstrDescription;
	WCL::ComStr bstrHelpFile;
	DWORD		dwHelpContext = 0;

	TEST_TRUE(pErrorInfo->GetGUID(&oGUID) == S_OK);
	TEST_TRUE(oGUID == GUID_NULL);

	TEST_TRUE(pErrorInfo->GetSource(AttachTo(bstrSource)) == S_OK);
	TEST_TRUE(wcscmp(bstrSource.Get(), L"TestErrorInfo") == 0);

	TEST_TRUE(pErrorInfo->GetDescription(AttachTo(bstrDescription)) == S_OK);
	TEST_TRUE(wcscmp(bstrDescription.Get(), L"Description") == 0);

	TEST_TRUE(pErrorInfo->GetHelpFile(AttachTo(bstrHelpFile)) == S_OK);
	TEST_TRUE(bstrHelpFile.Get() == nullptr);

	TEST_TRUE(pErrorInfo->GetHelpContext(&dwHelpContext) == S_OK);
	TEST_TRUE(dwHelpContext == 0);
}
