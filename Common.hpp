////////////////////////////////////////////////////////////////////////////////
//! \file   Common.hpp
//! \brief  Wrapper to include the most common COM library headers.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef COM_COMMON_HPP
#define COM_COMMON_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
// Library documentation
//
//! \namespace COM
//! \brief     The COM Library namespace.
//
// Inproc Server CheckList:-
//
// - MIDL|Output|Type Library = $(ProjectName).tlb
// - Linker|General|Register Output = Yes
// - Linker|Input|Module Definition File = ..\Lib\COM\InprocServer.def
// - Add resource = IDR_TYPELIB TYPELIB "ProjectName.tlb"
// - Ensure IDR_TYPELIB = 1

////////////////////////////////////////////////////////////////////////////////
// Standard headers.

#include <Core/Common.hpp>		// Core library common headers.
#include <WCL/Common.hpp>		// Windows C++ library common headers.
#include <WCL/StrConvs.hpp>		// String conversions.

////////////////////////////////////////////////////////////////////////////////
// Library headers.

#include <COM/ComTypes.hpp>		// Core types and macros.
#include <COM/ObjectBase.hpp>	// Default IUnknown implementation.
#include <COM/ErrorInfo.hpp>	// COM error handling macros and functions.

#endif // COM_COMMON_HPP
