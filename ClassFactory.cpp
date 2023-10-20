////////////////////////////////////////////////////////////////////////////////
//! \file   ClassFactory.cpp
//! \brief  The ClassFactory class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "ClassFactory.hpp"
#include "InprocServer.hpp"

#if (__GNUC__ >= 8) // GCC 8+
// error: format '%hs' expects argument of type 'short int*', but argument 3 has type 'const char*' [-Werror=format=]
#pragma GCC diagnostic ignored "-Wformat"
#endif

namespace COM
{

////////////////////////////////////////////////////////////////////////////////
//! Construction from a CLSID.

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

		if (pUnknown.get() != nullptr)
			hr = pUnknown->QueryInterface(rIID, ppInterface);
	}
	COM_CATCH(hr)

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
