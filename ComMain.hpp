////////////////////////////////////////////////////////////////////////////////
//! \file   ComMain.hpp
//! \brief  Function prototypes for the COM entry points.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef COM_COMMAIN_HPP
#define COM_COMMAIN_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
// COM inproc server function entry points.

//! Entry point for obtaining the class factory.
STDAPI DllGetClassObject(REFCLSID oCLSID, REFIID oIID, LPVOID* ppFactory);

//! Entry point for querying if the server can be unloaded..
STDAPI DllCanUnloadNow(void);

//! Entry point for registering the inproc server.
STDAPI DllRegisterServer(void);

//! Entry point for unregistering the inproc server.
STDAPI DllUnregisterServer(void);

#endif // COM_COMMAIN_HPP
