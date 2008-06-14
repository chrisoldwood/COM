////////////////////////////////////////////////////////////////////////////////
//! \file   ErrorInfo.cpp
//! \brief  COM error handling macros and functions.
//! \author Chris Oldwood

#include "Common.hpp"
#include "ErrorInfo.hpp"
#include <WCL/ComPtr.hpp>

namespace COM
{

////////////////////////////////////////////////////////////////////////////////
//! Set the COM ErrorInfo object for the thread. The return value indicates if
//! the "throw" was succesful or not.

bool SetComErrorInfo(const char* pszSource, const tchar* pszDescription)
{
	// Type shorthands
	typedef WCL::ComPtr<ICreateErrorInfo> ICreateErrorInfoPtr;
	typedef WCL::ComPtr<IErrorInfo>       IErrorInfoPtr;

	ICreateErrorInfoPtr pCreateErrorInfo;

	// Allocate the exception object.
	HRESULT hr = ::CreateErrorInfo(AttachTo(pCreateErrorInfo));

	if (FAILED(hr))
		return false;

	// Fill in the exception details.
//	pCreateErrorInfo->SetGUID();
	pCreateErrorInfo->SetSource(const_cast<wchar_t*>(A2W(pszSource)));
	pCreateErrorInfo->SetDescription(const_cast<wchar_t*>(T2W(pszDescription)));
//	pCreateErrorInfo->SetHelpFile(); 
//	pCreateErrorInfo->SetHelpContext(); 
	
	IErrorInfoPtr pErrorInfo; 

	hr = pCreateErrorInfo->QueryInterface(AttachTo(pErrorInfo)); 

	if (FAILED(hr))
		return false;

	// Throw it.
	hr = ::SetErrorInfo(0, pErrorInfo.Get());

	if (FAILED(hr))
		return false;

	return true;
}

//namespace COM
}
