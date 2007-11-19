////////////////////////////////////////////////////////////////////////////////
//! \file   InprocServer.cpp
//! \brief  The InprocServer class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "InprocServer.hpp"
#include "ServerRegInfo.hpp"
#include "RegUtils.hpp"
#include "ClassFactory.hpp"

// Linker directives.
#pragma comment(lib, "ole32")
#pragma comment(lib, "oleaut32")

namespace COM
{

//! The singleton inproc server.
InprocServer* InprocServer::g_pThis = NULL;

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

InprocServer::InprocServer()
{
	ASSERT(g_pThis == nullptr);

	g_pThis = this;
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

InprocServer::~InprocServer()
{
	ASSERT(g_pThis == this);

	g_pThis = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
// Singleton accessor.

InprocServer& InprocServer::This()
{
	ASSERT(g_pThis != nullptr);

	return *g_pThis;
}

////////////////////////////////////////////////////////////////////////////////
//! Get the class factory.

HRESULT InprocServer::DllGetClassObject(REFCLSID roCLSID, REFIID roIID, LPVOID* ppFactory)
{
	// Verify parameters.
	if (ppFactory == nullptr)
		return E_POINTER;

	// Reset output parameters.
	*ppFactory = nullptr;

	HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;

	try
	{
		// Create the class factory.
		IClassFactoryPtr pFactory = CreateClassFactory(roCLSID);

		if (pFactory.Get() != nullptr)
			hr = pFactory->QueryInterface(roIID, ppFactory);
	}
	COM_CATCH(hr)

	return hr;
}

////////////////////////////////////////////////////////////////////////////////
//! Query if the server can be unloaded.

HRESULT InprocServer::DllCanUnloadNow()
{
	return (LockCount() == 0) ? S_OK : S_FALSE;
}

////////////////////////////////////////////////////////////////////////////////
//! Register the server in the registry.

HRESULT InprocServer::DllRegisterServer()
{
	COM::ServerRegInfo oServerInfo;

	// Fill in the known server info.
	oServerInfo.m_eType   = INPROC_SERVER;
	oServerInfo.m_strFile = m_Module.Path();

	// Get the server and coclass details.
	GetServerRegInfo(oServerInfo);
	const ClassRegInfo* pClassInfo = GetClassRegInfo();

	// Register the coclasses.
	for (; pClassInfo->m_pCLSID != nullptr; ++pClassInfo)
		RegisterCLSID(oServerInfo, *pClassInfo->m_pCLSID, pClassInfo->m_pszName, pClassInfo->m_pszVersion, pClassInfo->m_eModel);

	// Register the type library.
	RegisterTypeLib(oServerInfo.m_strFile);

	return S_OK;
}

////////////////////////////////////////////////////////////////////////////////
//! Unregister the server from the registry.

HRESULT InprocServer::DllUnregisterServer()
{
	COM::ServerRegInfo oServerInfo;

	// Fill in the known server info.
	oServerInfo.m_eType   = INPROC_SERVER;
	oServerInfo.m_strFile = m_Module.Path();

	// Get the server and coclass details.
	GetServerRegInfo(oServerInfo);
	const ClassRegInfo* pClassInfo = GetClassRegInfo();

	// Unregister the coclasses.
	for (; pClassInfo->m_pCLSID != nullptr; ++pClassInfo)
		UnregisterCLSID(oServerInfo, *pClassInfo->m_pCLSID, pClassInfo->m_pszName, pClassInfo->m_pszVersion);

	// Unregister the type library.
	UnregisterTypeLib(oServerInfo.m_oLIBID, oServerInfo.m_nMajor, oServerInfo.m_nMinor);

	return S_OK;
}

////////////////////////////////////////////////////////////////////////////////
//! Template Method to get the servers class factory.

COM::IClassFactoryPtr InprocServer::CreateClassFactory(const CLSID& oCLSID)
{
	return IClassFactoryPtr(new ClassFactory(oCLSID), true);
}

//namespace COM
}
