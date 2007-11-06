////////////////////////////////////////////////////////////////////////////////
//! \file   RegUtils.cpp
//! \brief  Functions for use in registering and unregistering COM objects.
//! \author Chris Oldwood

#include "Common.hpp"
#include "RegUtils.hpp"
#include "ServerRegInfo.hpp"
#include "ComUtils.hpp"
#include <WCL/RegKey.hpp>

namespace COM
{

////////////////////////////////////////////////////////////////////////////////
//! Creates or updates the default value for a registry key under the HKCR tree.

void SetRegistryValue(const std::tstring& strSubKey, const std::tstring& strValue)
{
	WCL::RegKey::WriteKeyDefaultValue(HKEY_CLASSES_ROOT, strSubKey.c_str(), strValue.c_str());
}

////////////////////////////////////////////////////////////////////////////////
//! Creates or updates a named value for a registry key under the HKCR tree.

void SetRegistryValue(const std::tstring& strSubKey, const std::tstring& strName, const std::tstring& strValue)
{
	WCL::RegKey::WriteKeyStringValue(HKEY_CLASSES_ROOT, strSubKey.c_str(), strName.c_str(), strValue.c_str());
}

////////////////////////////////////////////////////////////////////////////////
//! Deletes the key from under the HKCR tree.

void DeleteKey(const std::tstring& strSubKey)
{
	bool bDeleted = false;

	bDeleted = WCL::RegKey::Delete(HKEY_CLASSES_ROOT, strSubKey.c_str());

#ifdef _DEBUG
	if (!bDeleted)
	{
		DWORD dwError = ::GetLastError();
		TRACE3("Failed to delete the key 'HKCR\\%s' [0x%08X - %s]\n", strSubKey.c_str(), dwError, CStrCvt::FormatError(dwError));
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////
//! Get the registry key name used for configuring the server type.

const char* GetServerTypeKey(ServerType eType)
{
	const char* pszType = "";

	switch (eType)
	{
		case INPROC_SERVER:	pszType = "InprocServer32";	break;
		case LOCAL_SERVER:	pszType = "LocalServer32";	break;
		default:			ASSERT_FALSE();				break;
	}

	return pszType;
}

////////////////////////////////////////////////////////////////////////////////
//! Get the registry key name used for configuring the threading model.

const char* GetThreadModelKey(ThreadingModel eModel)
{
	const char* pszModel = "";

	switch (eModel)
	{
		case MAIN_THREAD_APT:	pszModel = "";			break;
		case SINGLE_THREAD_APT:	pszModel = "Apartment";	break;
		case FREE_THREAD_APT:	pszModel = "Free";		break;
		case ANY_APARTMENT:		pszModel = "Both";		break;
		case NEUTRAL_APARTMENT:	pszModel = "Neutral";	break;
		default:				ASSERT_FALSE();			break;
	}

	return pszModel;
}

////////////////////////////////////////////////////////////////////////////////
//! Register a CLSID.

void RegisterCLSID(const ServerRegInfo& rSvrInfo, const CLSID& rCLSID,
					const std::tstring& strClass, const std::tstring& strVersion,
					ThreadingModel eModel)
{
	// Create key names.
	std::tstring strCLSID       = FormatGUID(rCLSID);
	std::tstring strLIBID       = FormatGUID(rSvrInfo.m_oLIBID);
	std::tstring strProgID      = rSvrInfo.m_strLibrary + "." + strClass;
	std::tstring strVerProgID   = strProgID + "." + strVersion;
	std::tstring strDescription = strClass + " Class";
	std::tstring strCLSIDKey    = "CLSID\\" + strCLSID;
	std::tstring strServerType  = GetServerTypeKey(rSvrInfo.m_eType);
	std::tstring strThreadModel = GetThreadModelKey(eModel);

	// Create the version independent prog ID section.
	SetRegistryValue(strProgID,              strDescription);
	SetRegistryValue(strProgID + "\\CLSID",  strCLSID);
	SetRegistryValue(strProgID + "\\CurVer", strVerProgID);

	// Create the version dependent prog ID section.
	SetRegistryValue(strVerProgID,             strDescription);
	SetRegistryValue(strVerProgID + "\\CLSID", strCLSID);

	// Create the CLSID section.
	SetRegistryValue(strCLSIDKey,                                strClass);
	SetRegistryValue(strCLSIDKey + "\\" + strServerType,         rSvrInfo.m_strFile);
	SetRegistryValue(strCLSIDKey + "\\" + strServerType,         "ThreadingModel", strThreadModel);
	SetRegistryValue(strCLSIDKey + "\\ProgID",                   strVerProgID);
	SetRegistryValue(strCLSIDKey + "\\VersionIndependentProgID", strProgID);
	SetRegistryValue(strCLSIDKey + "\\TypeLib",                  strLIBID);
}

////////////////////////////////////////////////////////////////////////////////
//! Unregister a CLSID.

void UnregisterCLSID(const ServerRegInfo& rSvrInfo, const CLSID& rCLSID,
					const std::tstring& strClass, const std::tstring& strVersion)
{
	// Create key names.
	std::tstring strCLSID     = FormatGUID(rCLSID);
	std::tstring strProgID    = rSvrInfo.m_strLibrary + "." + strClass;
	std::tstring strVerProgID = strProgID + "." + strVersion;
	std::tstring strCLSIDKey  = "CLSID\\" + strCLSID;
	std::tstring strServerType  = GetServerTypeKey(rSvrInfo.m_eType);

	// Delete the version independent prog ID section.
	DeleteKey(strProgID + "\\CLSID");
	DeleteKey(strProgID + "\\CurVer");
	DeleteKey(strProgID);

	// Delete the version dependent prog ID section.
	DeleteKey(strVerProgID + "\\CLSID");
	DeleteKey(strVerProgID);

	// Delete the CLSID section.
	DeleteKey(strCLSIDKey + "\\" + strServerType);
	DeleteKey(strCLSIDKey + "\\ProgID");
	DeleteKey(strCLSIDKey + "\\VersionIndependentProgID");
	DeleteKey(strCLSIDKey + "\\TypeLib");
	DeleteKey(strCLSIDKey);
}

////////////////////////////////////////////////////////////////////////////////
//! Register a type library.

void RegisterTypeLib(const std::tstring& strFile)
{
	// Type shorthands.
	typedef Core::IFacePtr<ITypeLib> ITypeLibPtr;

	wchar_t     szFile[MAX_PATH+1] = {0};
	ITypeLibPtr pTypeLib;

	// Need non-const string for ::RegisterTypeLib().
	wcsncpy(szFile, T2W(strFile.c_str()), MAX_PATH);

	// Load the type library.
	HRESULT hr = ::LoadTypeLib(szFile, AttachTo(pTypeLib));

	if (FAILED(hr))
		throw WCL::ComException(hr, CString::Fmt("Failed to load the type library '%s'", strFile.c_str()));

	// Register it.
	hr = ::RegisterTypeLib(pTypeLib.Get(), szFile, nullptr);

	if (FAILED(hr))
		throw WCL::ComException(hr, CString::Fmt("Failed to register the type library '%s'", strFile.c_str()));
}

////////////////////////////////////////////////////////////////////////////////
//! Unregister a type library.

void UnregisterTypeLib(const GUID& rLIBID, ushort nMajor, ushort nMinor)
{
	HRESULT hr = S_OK;

	// Unregister it.
	hr = ::UnRegisterTypeLib(rLIBID, nMajor, nMinor, 0, SYS_WIN32);

#ifdef _DEBUG
	if (FAILED(hr))
	{
		TRACE2("Failed to unregister the type library [0x%08X - %s]", hr, CStrCvt::FormatError(hr));
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////
// Register a Moniker prefix. This registers the prefix for a custom moniker
// and the CLSID to associate with it.

void RegisterMonikerPrefix(const std::tstring& strPrefix, const std::tstring& strClass, const CLSID& rCLSID)
{
	// Create key names.
	std::tstring strCLSID       = FormatGUID(rCLSID);
	std::tstring strDescription = strClass + " Class";

	SetRegistryValue(strPrefix,              strDescription);
	SetRegistryValue(strPrefix + "\\CLSID",  strCLSID);
}

////////////////////////////////////////////////////////////////////////////////
// Unregister a Moniker prefix.

void UnregisterMonikerPrefix(const std::tstring& strPrefix)
{
	DeleteKey(strPrefix + "\\CLSID");
	DeleteKey(strPrefix);
}

//namespace COM
}
