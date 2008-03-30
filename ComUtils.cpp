////////////////////////////////////////////////////////////////////////////////
//! \file   ComUtils.cpp
//! \brief  COM utility functions.
//! \author Chris Oldwood

#include "Common.hpp"
#include <WCL/RegKey.hpp>
#include <Core/AnsiWide.hpp>
#include <Core/BadLogicException.hpp>

namespace COM
{

//! The maximum length of a string GUID.
const size_t MAX_GUID_CHARS = 38;

////////////////////////////////////////////////////////////////////////////////
//! Format the GUID as a string in Registry format.

std::tstring FormatGUID(const GUID& rGUID)
{
	wchar_t szBuffer[MAX_GUID_CHARS+1];

	if (::StringFromGUID2(rGUID, szBuffer, MAX_GUID_CHARS+1) == 0)
		throw Core::BadLogicException(TXT("Invalid buffer size passed to StringFromGUID2()"));

	return std::tstring(W2T(szBuffer));
}

////////////////////////////////////////////////////////////////////////////////
//! Find the human readable name for the class ID.

std::tstring LookupCLSID(const CLSID& rCLSID)
{
	// Format the key name.
	std::tstring strKey = TXT("CLSID\\") + FormatGUID(rCLSID);

	if (!WCL::RegKey::Exists(HKEY_CLASSES_ROOT, strKey.c_str()))
		return TXT("");

	// The key default value is the class name.
	return std::tstring(WCL::RegKey::ReadKeyDefaultValue(HKEY_CLASSES_ROOT, strKey.c_str()));
}

////////////////////////////////////////////////////////////////////////////////
//! Find the human readable name for the interface ID.

std::tstring LookupIID(const IID& rIID)
{
	// Format the key name.
	std::tstring strKey = TXT("Interface\\") + FormatGUID(rIID);

	if (!WCL::RegKey::Exists(HKEY_CLASSES_ROOT, strKey.c_str()))
		return TXT("");

	// The key default value is the interface name.
	return std::tstring(WCL::RegKey::ReadKeyDefaultValue(HKEY_CLASSES_ROOT, strKey.c_str()));
}

//namespace COM
}
