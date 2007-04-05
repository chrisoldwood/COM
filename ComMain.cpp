////////////////////////////////////////////////////////////////////////////////
//! \author Chris Oldwood
//! \file   ComMain.cpp
//! \brief  The entry point methods for a COM inproc server.

#include "com.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

////////////////////////////////////////////////////////////////////////////////
//! Entry point for obtaining the class factory.

STDAPI DllGetClassObject(REFCLSID roCLSID, REFIID roIID, LPVOID* ppFactory)
{
	// Forward to inproc server singleton.
	return COM::InprocServer::This().DllGetClassObject(roCLSID, roIID, ppFactory);
}

////////////////////////////////////////////////////////////////////////////////
//! Entry point for querying if the server can be unloaded..

STDAPI DllCanUnloadNow(void)
{
	// Forward to inproc server singleton.
	return COM::InprocServer::This().DllCanUnloadNow();
}

////////////////////////////////////////////////////////////////////////////////
//! Entry point for registering the inproc server.

STDAPI DllRegisterServer(void)
{
	// Forward to inproc server singleton.
	return COM::InprocServer::This().DllRegisterServer();
}

////////////////////////////////////////////////////////////////////////////////
//! Entry point for unregistering the inproc server.

STDAPI DllUnregisterServer(void)
{
	// Forward to inproc server singleton.
	return COM::InprocServer::This().DllUnregisterServer();
}
