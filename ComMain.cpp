////////////////////////////////////////////////////////////////////////////////
//! \file   ComMain.cpp
//! \brief  The entry point methods for a COM inproc server.
//! \author Chris Oldwood

#include "Common.hpp"
#include "InprocServer.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Entry point for obtaining the class factory.

STDAPI DllGetClassObject(REFCLSID roCLSID, REFIID roIID, LPVOID* ppFactory)
{
	HRESULT hr = E_FAIL;

	try
	{
		// Forward to inproc server singleton.
		hr = COM::InprocServer::This().DllGetClassObject(roCLSID, roIID, ppFactory);
	}
	COM_CATCH(hr)

	return hr;
}

////////////////////////////////////////////////////////////////////////////////
//! Entry point for querying if the server can be unloaded.

STDAPI DllCanUnloadNow(void)
{
	HRESULT hr = E_FAIL;

	try
	{
		// Forward to inproc server singleton.
		hr = COM::InprocServer::This().DllCanUnloadNow();
	}
	COM_CATCH(hr)

	return hr;
}

////////////////////////////////////////////////////////////////////////////////
//! Entry point for registering the inproc server.

STDAPI DllRegisterServer(void)
{
	HRESULT hr = E_FAIL;

	try
	{
		// Forward to inproc server singleton.
		hr = COM::InprocServer::This().DllRegisterServer();
	}
	COM_CATCH(hr)

	return hr;
}

////////////////////////////////////////////////////////////////////////////////
//! Entry point for unregistering the inproc server.

STDAPI DllUnregisterServer(void)
{
	HRESULT hr = E_FAIL;

	try
	{
		// Forward to inproc server singleton.
		hr = COM::InprocServer::This().DllUnregisterServer();
	}
	COM_CATCH(hr)

	return hr;
}
