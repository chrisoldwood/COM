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

////////////////////////////////////////////////////////////////////////////////
// Register a CLSID.

void RegisterCLSID(const ServerRegInfo& rSvrInfo, const CLSID& rCLSID,
					const std::tstring& strClass, const std::tstring& strVersion,
					ThreadingModel eModel); // throw(RegistryException, ComException)

////////////////////////////////////////////////////////////////////////////////
// Unregister a CLSID.

void UnregisterCLSID(const ServerRegInfo& rSvrInfo, const CLSID& rCLSID,
					const std::tstring& strClass, const std::tstring& strVersion); // throw(RegistryException, ComException)

////////////////////////////////////////////////////////////////////////////////
// Register a type library.

void RegisterTypeLib(const std::tstring& strFile); // throw(ComException)

////////////////////////////////////////////////////////////////////////////////
// Unregister a type library.

void UnregisterTypeLib(const GUID& rLIBID, ushort nMajor, ushort nMinor); // throw(ComException)

//namespace COM
}

#endif // COM_REGUTILS_HPP
