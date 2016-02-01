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

static void SetRegistryValue(Scope scope, const tstring& strSubKey, const tstring& strValue)
{
	ASSERT((scope == MACHINE) || (scope == USER));

	if (scope == MACHINE)
	{
		WCL::RegKey::WriteKeyDefaultValue(HKEY_CLASSES_ROOT, strSubKey.c_str(), strValue.c_str());
	}
	else // (scope == USER)
	{
		WCL::RegKey root;

		root.Open(HKEY_CURRENT_USER, TXT("Software\\Classes"), KEY_WRITE);

		WCL::RegKey::WriteKeyDefaultValue(root.Handle(), strSubKey.c_str(), strValue.c_str());
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Creates or updates a named value for a registry key under the HKCR tree.

static void SetRegistryValue(Scope scope, const tstring& strSubKey, const tstring& strName, const tstring& strValue)
{
	ASSERT((scope == MACHINE) || (scope == USER));

	if (scope == MACHINE)
	{
		WCL::RegKey::WriteKeyStringValue(HKEY_CLASSES_ROOT, strSubKey.c_str(), strName.c_str(), strValue.c_str());
	}
	else // (scope == USER)
	{
		WCL::RegKey root;

		root.Open(HKEY_CURRENT_USER, TXT("Software\\Classes"), KEY_WRITE);

		WCL::RegKey::WriteKeyStringValue(root.Handle(), strSubKey.c_str(), strName.c_str(), strValue.c_str());
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Deletes the key from under the HKCR tree.

static void DeleteKey(Scope scope, const tstring& strSubKey)
{
	ASSERT((scope == MACHINE) || (scope == USER));

	bool bDeleted = false;

	if (scope == MACHINE)
	{
		bDeleted = WCL::RegKey::Delete(HKEY_CLASSES_ROOT, strSubKey.c_str());
	}
	else // (scope == USER)
	{
		WCL::RegKey root;

		root.Open(HKEY_CURRENT_USER, TXT("Software\\Classes"), KEY_WRITE);

		bDeleted = WCL::RegKey::Delete(root.Handle(), strSubKey.c_str());
	}

#ifdef _DEBUG
	if (!bDeleted)
	{
		DWORD dwError = ::GetLastError();
		TRACE3(TXT("Failed to delete the key '<root>\\%s' [0x%08X - %s]\n"), strSubKey.c_str(), dwError, CStrCvt::FormatError(dwError).c_str());
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////
//! Get the registry key name used for configuring the server type.

static const tchar* GetServerTypeKey(ServerType eType)
{
	const tchar* pszType = TXT("");

	switch (eType)
	{
		case INPROC_SERVER:	pszType = TXT("InprocServer32");	break;
		case LOCAL_SERVER:	pszType = TXT("LocalServer32");		break;
		default:			ASSERT_FALSE();						break;
	}

	return pszType;
}

////////////////////////////////////////////////////////////////////////////////
//! Get the registry key name used for configuring the threading model.

static const tchar* GetThreadModelKey(ThreadingModel eModel)
{
	const tchar* pszModel = TXT("");

	switch (eModel)
	{
		case MAIN_THREAD_APT:	pszModel = TXT("");				break;
		case SINGLE_THREAD_APT:	pszModel = TXT("Apartment");	break;
		case FREE_THREAD_APT:	pszModel = TXT("Free");			break;
		case ANY_APARTMENT:		pszModel = TXT("Both");			break;
		case NEUTRAL_APARTMENT:	pszModel = TXT("Neutral");		break;
		default:				ASSERT_FALSE();					break;
	}

	return pszModel;
}

////////////////////////////////////////////////////////////////////////////////
//! Register a CLSID.

void RegisterCLSID(Scope scope, const ServerRegInfo& rSvrInfo, const CLSID& rCLSID,
					const tstring& strClass, const tstring& strVersion,
					ThreadingModel eModel)
{
	// Create key names.
	tstring strCLSID       = FormatGUID(rCLSID);
	tstring strLIBID       = FormatGUID(rSvrInfo.m_oLIBID);
	tstring strProgID      = rSvrInfo.m_strLibrary + TXT(".") + strClass;
	tstring strVerProgID   = strProgID + TXT(".") + strVersion;
	tstring strDescription = strClass + TXT(" Class");
	tstring strCLSIDKey    = TXT("CLSID\\") + strCLSID;
	tstring strServerType  = GetServerTypeKey(rSvrInfo.m_eType);
	tstring strThreadModel = GetThreadModelKey(eModel);

	// Create the version independent prog ID section.
	SetRegistryValue(scope, strProgID,                   strDescription);
	SetRegistryValue(scope, strProgID + TXT("\\CLSID"),  strCLSID);
	SetRegistryValue(scope, strProgID + TXT("\\CurVer"), strVerProgID);

	// Create the version dependent prog ID section.
	SetRegistryValue(scope, strVerProgID,                  strDescription);
	SetRegistryValue(scope, strVerProgID + TXT("\\CLSID"), strCLSID);

	// Create the CLSID section.
	SetRegistryValue(scope, strCLSIDKey,                                     strClass);
	SetRegistryValue(scope, strCLSIDKey + TXT("\\") + strServerType,         rSvrInfo.m_strFile);
	SetRegistryValue(scope, strCLSIDKey + TXT("\\") + strServerType,         TXT("ThreadingModel"), strThreadModel);
	SetRegistryValue(scope, strCLSIDKey + TXT("\\ProgID"),                   strVerProgID);
	SetRegistryValue(scope, strCLSIDKey + TXT("\\VersionIndependentProgID"), strProgID);
	SetRegistryValue(scope, strCLSIDKey + TXT("\\TypeLib"),                  strLIBID);
}

////////////////////////////////////////////////////////////////////////////////
//! Unregister a CLSID.

void UnregisterCLSID(Scope scope, const ServerRegInfo& rSvrInfo, const CLSID& rCLSID,
					const tstring& strClass, const tstring& strVersion)
{
	// Create key names.
	tstring strCLSID     = FormatGUID(rCLSID);
	tstring strProgID    = rSvrInfo.m_strLibrary + TXT(".") + strClass;
	tstring strVerProgID = strProgID + TXT(".") + strVersion;
	tstring strCLSIDKey  = TXT("CLSID\\") + strCLSID;
	tstring strServerType  = GetServerTypeKey(rSvrInfo.m_eType);

	// Delete the version independent prog ID section.
	DeleteKey(scope, strProgID + TXT("\\CLSID"));
	DeleteKey(scope, strProgID + TXT("\\CurVer"));
	DeleteKey(scope, strProgID);

	// Delete the version dependent prog ID section.
	DeleteKey(scope, strVerProgID + TXT("\\CLSID"));
	DeleteKey(scope, strVerProgID);

	// Delete the CLSID section.
	DeleteKey(scope, strCLSIDKey + TXT("\\") + strServerType);
	DeleteKey(scope, strCLSIDKey + TXT("\\ProgID"));
	DeleteKey(scope, strCLSIDKey + TXT("\\VersionIndependentProgID"));
	DeleteKey(scope, strCLSIDKey + TXT("\\TypeLib"));
	DeleteKey(scope, strCLSIDKey);
}

////////////////////////////////////////////////////////////////////////////////
//! Register a type library.

void RegisterTypeLib(Scope scope, const tstring& strFile)
{
	ASSERT((scope == MACHINE) || (scope == USER));

	wchar_t     szFile[MAX_PATH+1] = {0};
	ITypeLibPtr pTypeLib;

	// Need non-const string for ::RegisterTypeLib().
	wcsncpy(szFile, T2W(strFile.c_str()), MAX_PATH);

	// Load the type library.
	HRESULT hr = ::LoadTypeLib(szFile, AttachTo(pTypeLib));

	if (FAILED(hr))
		throw WCL::ComException(hr, CString::Fmt(TXT("Failed to load the type library '%s'"), strFile.c_str()));

	// Register it.
	if (scope == MACHINE)
		hr = ::RegisterTypeLib(pTypeLib.get(), szFile, nullptr);
	else
		hr = ::RegisterTypeLibForUser(pTypeLib.get(), szFile, nullptr);

	if (FAILED(hr))
		throw WCL::ComException(hr, CString::Fmt(TXT("Failed to register the type library '%s'"), strFile.c_str()));
}

////////////////////////////////////////////////////////////////////////////////
//! Unregister a type library.

void UnregisterTypeLib(Scope scope, const GUID& rLIBID, ushort nMajor, ushort nMinor)
{
	ASSERT((scope == MACHINE) || (scope == USER));

	HRESULT hr = S_OK;

	// Unregister it.
	if (scope == MACHINE)
		hr = ::UnRegisterTypeLib(rLIBID, nMajor, nMinor, 0, SYS_WIN32);
	else
		hr = ::UnRegisterTypeLibForUser(rLIBID, nMajor, nMinor, 0, SYS_WIN32);

#ifdef _DEBUG
	if (FAILED(hr))
	{
		TRACE2(TXT("Failed to unregister the type library [0x%08X - %s]"), hr, CStrCvt::FormatError(hr).c_str());
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////
// Register a Moniker prefix. This registers the prefix for a custom moniker
// and the CLSID to associate with it.

void RegisterMonikerPrefix(Scope scope, const tstring& strPrefix, const tstring& strClass, const CLSID& rCLSID)
{
	// Create key names.
	tstring strCLSID       = FormatGUID(rCLSID);
	tstring strDescription = strClass + TXT(" Class");

	SetRegistryValue(scope, strPrefix,                  strDescription);
	SetRegistryValue(scope, strPrefix + TXT("\\CLSID"), strCLSID);
}

////////////////////////////////////////////////////////////////////////////////
// Unregister a Moniker prefix.

void UnregisterMonikerPrefix(Scope scope, const tstring& strPrefix)
{
	DeleteKey(scope, strPrefix + TXT("\\CLSID"));
	DeleteKey(scope, strPrefix);
}

//namespace COM
}
