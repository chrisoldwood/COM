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

void SetRegistryValue(const tstring& strSubKey, const tstring& strValue)
{
	WCL::RegKey::WriteKeyDefaultValue(HKEY_CLASSES_ROOT, strSubKey.c_str(), strValue.c_str());
}

////////////////////////////////////////////////////////////////////////////////
//! Creates or updates a named value for a registry key under the HKCR tree.

void SetRegistryValue(const tstring& strSubKey, const tstring& strName, const tstring& strValue)
{
	WCL::RegKey::WriteKeyStringValue(HKEY_CLASSES_ROOT, strSubKey.c_str(), strName.c_str(), strValue.c_str());
}

////////////////////////////////////////////////////////////////////////////////
//! Deletes the key from under the HKCR tree.

void DeleteKey(const tstring& strSubKey)
{
	bool bDeleted = false;

	bDeleted = WCL::RegKey::Delete(HKEY_CLASSES_ROOT, strSubKey.c_str());

#ifdef _DEBUG
	if (!bDeleted)
	{
		DWORD dwError = ::GetLastError();
		TRACE3(TXT("Failed to delete the key 'HKCR\\%s' [0x%08X - %s]\n"), strSubKey.c_str(), dwError, CStrCvt::FormatError(dwError).c_str());
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////
//! Get the registry key name used for configuring the server type.

const tchar* GetServerTypeKey(ServerType eType)
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

const tchar* GetThreadModelKey(ThreadingModel eModel)
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

void RegisterCLSID(const ServerRegInfo& rSvrInfo, const CLSID& rCLSID,
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
	SetRegistryValue(strProgID,                   strDescription);
	SetRegistryValue(strProgID + TXT("\\CLSID"),  strCLSID);
	SetRegistryValue(strProgID + TXT("\\CurVer"), strVerProgID);

	// Create the version dependent prog ID section.
	SetRegistryValue(strVerProgID,                  strDescription);
	SetRegistryValue(strVerProgID + TXT("\\CLSID"), strCLSID);

	// Create the CLSID section.
	SetRegistryValue(strCLSIDKey,                                     strClass);
	SetRegistryValue(strCLSIDKey + TXT("\\") + strServerType,         rSvrInfo.m_strFile);
	SetRegistryValue(strCLSIDKey + TXT("\\") + strServerType,         TXT("ThreadingModel"), strThreadModel);
	SetRegistryValue(strCLSIDKey + TXT("\\ProgID"),                   strVerProgID);
	SetRegistryValue(strCLSIDKey + TXT("\\VersionIndependentProgID"), strProgID);
	SetRegistryValue(strCLSIDKey + TXT("\\TypeLib"),                  strLIBID);
}

////////////////////////////////////////////////////////////////////////////////
//! Unregister a CLSID.

void UnregisterCLSID(const ServerRegInfo& rSvrInfo, const CLSID& rCLSID,
					const tstring& strClass, const tstring& strVersion)
{
	// Create key names.
	tstring strCLSID     = FormatGUID(rCLSID);
	tstring strProgID    = rSvrInfo.m_strLibrary + TXT(".") + strClass;
	tstring strVerProgID = strProgID + TXT(".") + strVersion;
	tstring strCLSIDKey  = TXT("CLSID\\") + strCLSID;
	tstring strServerType  = GetServerTypeKey(rSvrInfo.m_eType);

	// Delete the version independent prog ID section.
	DeleteKey(strProgID + TXT("\\CLSID"));
	DeleteKey(strProgID + TXT("\\CurVer"));
	DeleteKey(strProgID);

	// Delete the version dependent prog ID section.
	DeleteKey(strVerProgID + TXT("\\CLSID"));
	DeleteKey(strVerProgID);

	// Delete the CLSID section.
	DeleteKey(strCLSIDKey + TXT("\\") + strServerType);
	DeleteKey(strCLSIDKey + TXT("\\ProgID"));
	DeleteKey(strCLSIDKey + TXT("\\VersionIndependentProgID"));
	DeleteKey(strCLSIDKey + TXT("\\TypeLib"));
	DeleteKey(strCLSIDKey);
}

////////////////////////////////////////////////////////////////////////////////
//! Register a type library.

void RegisterTypeLib(const tstring& strFile)
{
	// Type shorthands.
	typedef WCL::IFacePtr<ITypeLib> ITypeLibPtr;

	wchar_t     szFile[MAX_PATH+1] = {0};
	ITypeLibPtr pTypeLib;

	// Need non-const string for ::RegisterTypeLib().
	wcsncpy(szFile, T2W(strFile.c_str()), MAX_PATH);

	// Load the type library.
	HRESULT hr = ::LoadTypeLib(szFile, AttachTo(pTypeLib));

	if (FAILED(hr))
		throw WCL::ComException(hr, CString::Fmt(TXT("Failed to load the type library '%s'"), strFile.c_str()));

	// Register it.
	hr = ::RegisterTypeLib(pTypeLib.get(), szFile, nullptr);

	if (FAILED(hr))
		throw WCL::ComException(hr, CString::Fmt(TXT("Failed to register the type library '%s'"), strFile.c_str()));
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
		TRACE2(TXT("Failed to unregister the type library [0x%08X - %s]"), hr, CStrCvt::FormatError(hr).c_str());
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////
// Register a Moniker prefix. This registers the prefix for a custom moniker
// and the CLSID to associate with it.

void RegisterMonikerPrefix(const tstring& strPrefix, const tstring& strClass, const CLSID& rCLSID)
{
	// Create key names.
	tstring strCLSID       = FormatGUID(rCLSID);
	tstring strDescription = strClass + TXT(" Class");

	SetRegistryValue(strPrefix,                  strDescription);
	SetRegistryValue(strPrefix + TXT("\\CLSID"), strCLSID);
}

////////////////////////////////////////////////////////////////////////////////
// Unregister a Moniker prefix.

void UnregisterMonikerPrefix(const tstring& strPrefix)
{
	DeleteKey(strPrefix + TXT("\\CLSID"));
	DeleteKey(strPrefix);
}

//namespace COM
}
