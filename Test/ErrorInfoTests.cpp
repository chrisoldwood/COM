////////////////////////////////////////////////////////////////////////////////
//! \file   ErrorInfoTests.cpp
//! \brief  The unit tests for the COM IErrorInfo error handling.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <WCL/ComPtr.hpp>
#include <WCL/ComStr.hpp>

TEST_SET(ErrorInfo)
{
	typedef WCL::ComPtr<IErrorInfo> IErrorInfoPtr;

TEST_CASE("throwing a COM error passes the function and error message")
{
	const char*  TEST_FUNCTION = __FUNCTION__;
	const tchar* TEST_DESCRIPTION = TXT("Description");

	bool thrown = COM::SetComErrorInfo(TEST_FUNCTION, TEST_DESCRIPTION);

	TEST_TRUE(thrown == true);

	IErrorInfoPtr pErrorInfo;

	HRESULT result = ::GetErrorInfo(0, AttachTo(pErrorInfo));

	TEST_TRUE(result == S_OK);

	GUID		oGUID = { 0 };
	WCL::ComStr bstrSource;
	WCL::ComStr bstrDescription;
	WCL::ComStr bstrHelpFile;
	DWORD		dwHelpContext = 0;

	TEST_TRUE(pErrorInfo->GetGUID(&oGUID) == S_OK);
	TEST_TRUE(oGUID == GUID_NULL);

	TEST_TRUE(pErrorInfo->GetSource(AttachTo(bstrSource)) == S_OK);
	TEST_TRUE(wcscmp(bstrSource.Get(), A2W(TEST_FUNCTION)) == 0);

	TEST_TRUE(pErrorInfo->GetDescription(AttachTo(bstrDescription)) == S_OK);
	TEST_TRUE(wcscmp(bstrDescription.Get(), T2W(TEST_DESCRIPTION)) == 0);

	const BSTR DEFAULT_HELP_FILE = nullptr;

	TEST_TRUE(pErrorInfo->GetHelpFile(AttachTo(bstrHelpFile)) == S_OK);
	TEST_TRUE(bstrHelpFile.Get() == DEFAULT_HELP_FILE);

	const DWORD DEFAULT_HELP_CONTEXT = 0;

	TEST_TRUE(pErrorInfo->GetHelpContext(&dwHelpContext) == S_OK);
	TEST_TRUE(dwHelpContext == DEFAULT_HELP_CONTEXT);
}
TEST_CASE_END

}
TEST_SET_END
