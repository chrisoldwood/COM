////////////////////////////////////////////////////////////////////////////////
//! \author Chris Oldwood
//! \file   ComUtils.cpp
//! \brief  COM utility functions.

#include "com.hpp"
#include <atlconv.h>

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

namespace COM
{

//! The maximum length of a string GUID.
const size_t MAX_GUID_CHARS = 38;

////////////////////////////////////////////////////////////////////////////////
//! Format the GUID as a string in Registry format.

std::tstring FormatGUID(const GUID& rGUID)
{
	USES_CONVERSION;

	wchar_t szBuffer[MAX_GUID_CHARS+1];

	if (::StringFromGUID2(rGUID, szBuffer, MAX_GUID_CHARS) == 0)
		throw std::logic_error("Invalid buffer size passed to StringFromGUID2()");

	return W2T(szBuffer);
}

//namespace COM
}
