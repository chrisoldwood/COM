////////////////////////////////////////////////////////////////////////////////
//! \file   ClassFactory.cpp
//! \brief  The ClassFactory class definition.
//! \author Chris Oldwood

#include "com.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

namespace COM
{

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

ClassFactory::ClassFactory(const CLSID& rCLSID)
	: m_oCLSID(rCLSID)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

ClassFactory::~ClassFactory()
{
}


////////////////////////////////////////////////////////////////////////////////
//! Create an instance object of the class.

HRESULT COMCALL ClassFactory::CreateInstance(IUnknown* pOuter, const IID& rIID, void** ppInterface)
{
	// Check parameters.
	if (ppInterface == nullptr)
		return E_POINTER;

	// Reset output parameters.
	*ppInterface = nullptr;

	// Validate input parameters.
	if (pOuter != nullptr)
		return CLASS_E_NOAGGREGATION;

	HRESULT hr = E_FAIL;

	try
	{
		// Create the object.
		IUnknownPtr pUnknown = InprocServer::This().CreateObject(m_oCLSID);

		if (pUnknown.Get() != nullptr)
			hr = pUnknown->QueryInterface(rIID, ppInterface);
	}
	COM_CATCH_TRACE_AND_SET("ClassFactory::CreateInstance()", hr)
	
	return hr;
}

////////////////////////////////////////////////////////////////////////////////
//! Lock the COM server.

HRESULT COMCALL ClassFactory::LockServer(BOOL fLock)
{
	HRESULT hr = S_OK;

	InprocServer& oServer = InprocServer::This();

	(fLock) ? oServer.Lock() : oServer.Unlock();

	return hr;
}

//namespace COM
}
