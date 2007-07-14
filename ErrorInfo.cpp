////////////////////////////////////////////////////////////////////////////////
//! \file   ErrorInfo.cpp
//! \brief  .
//! \author Chris Oldwood

#include "Common.hpp"
#include "ErrorInfo.hpp"
#include <WCL/ComPtr.hpp>
#include <atlconv.h>

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

namespace COM
{

////////////////////////////////////////////////////////////////////////////////
//! Set the COM ErrorInfo object for the thread. The return value indicates if
//! the "throw" was succesful or not.

bool SetComErrorInfo(const tchar* pszSource, const tchar* pszDescription)
{
	USES_CONVERSION;

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
	pCreateErrorInfo->SetSource(T2OLE(pszSource));
	pCreateErrorInfo->SetDescription(T2OLE(pszDescription));
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
