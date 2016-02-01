////////////////////////////////////////////////////////////////////////////////
//! \file   RegUtils.hpp
//! \brief  Functions for use in registering and unregistering COM objects.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef COM_REGUTILS_HPP
#define COM_REGUTILS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace COM
{

// Forward declarations.
class ServerRegInfo;

////////////////////////////////////////////////////////////////////////////////
//! The scope used for registration.

enum Scope
{
	MACHINE = 1,	//!< Register in HKEY_LOCAL_MACHINE (HKEY_CLASSES_ROOT).
	USER = 2,		//!< Register in HKEY_CURRENT_USER\Software\Classes.
};

////////////////////////////////////////////////////////////////////////////////
// Register a CLSID.

void RegisterCLSID(Scope scope, const ServerRegInfo& rSvrInfo, const CLSID& rCLSID,
					const tstring& strClass, const tstring& strVersion,
					ThreadingModel eModel); // throw(RegistryException, ComException)

////////////////////////////////////////////////////////////////////////////////
// Unregister a CLSID.

void UnregisterCLSID(Scope scope, const ServerRegInfo& rSvrInfo, const CLSID& rCLSID,
					const tstring& strClass, const tstring& strVersion); // throw(RegistryException, ComException)

////////////////////////////////////////////////////////////////////////////////
// Register a type library.

void RegisterTypeLib(Scope scope, const tstring& strFile); // throw(ComException)

////////////////////////////////////////////////////////////////////////////////
// Unregister a type library.

void UnregisterTypeLib(Scope scope, const GUID& rLIBID, ushort nMajor, ushort nMinor); // throw(ComException)

////////////////////////////////////////////////////////////////////////////////
// Register a Moniker prefix.

void RegisterMonikerPrefix(Scope scope, const tstring& strPrefix, const tstring& strClass, const CLSID& rCLSID);

////////////////////////////////////////////////////////////////////////////////
// Unregister a Moniker prefix.

void UnregisterMonikerPrefix(Scope scope, const tstring& strPrefix);

//namespace COM
}

#endif // COM_REGUTILS_HPP
