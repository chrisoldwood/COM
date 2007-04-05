////////////////////////////////////////////////////////////////////////////////
//! \author Chris Oldwood
//! \file   Com.hpp
//! \brief  Wrapper to include all the core COM library headers.

// Check for previous inclusion
#ifndef COM_HPP
#define COM_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
// Standard headers

#include <Core/Common.hpp>	// Core library headers.
#include "wcl.hpp"			// Windows C++ library.

////////////////////////////////////////////////////////////////////////////////
// Library headers

#include "ComUtils.hpp"
#include "ComMain.hpp"
#include "InprocServer.hpp"

#endif // COM_HPP
