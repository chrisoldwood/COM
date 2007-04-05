////////////////////////////////////////////////////////////////////////////////
//! \author Chris Oldwood
//! \file   ComUtils.hpp
//! \brief  COM utility functions.

// Check for previous inclusion
#ifndef COMUTILS_HPP
#define COMUTILS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace COM
{

////////////////////////////////////////////////////////////////////////////////
//! Format the GUID as a string in Registry format.

std::tstring FormatGUID(const GUID& rGUID);

//namespace COM
}

#endif // COMUTILS_HPP
