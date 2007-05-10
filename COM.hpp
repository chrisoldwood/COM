////////////////////////////////////////////////////////////////////////////////
//! \file   Com.hpp
//! \brief  Wrapper to include all the COM library headers.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef COM_COM_HPP
#define COM_COM_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
// Library documentation
//
//! \namespace COM
//! \brief     The COM Library namespace.

////////////////////////////////////////////////////////////////////////////////
// Standard headers.

#include <Core/Common.hpp>		// Core library common headers.
#include <WCL/wcl.hpp>			// Windows C++ library.

////////////////////////////////////////////////////////////////////////////////
// Other commonly used headers.

#include <Core/IFacePtr.hpp>	// Interface smart-pointer class.

////////////////////////////////////////////////////////////////////////////////
// Library headers.

#include <COM/ComTypes.hpp>
#include <COM/ComUtils.hpp>
#include <COM/ServerRegInfo.hpp>
#include <COM/RegUtils.hpp>
#include <COM/ComMain.hpp>
#include <COM/ObjectBase.hpp>
#include <COM/Server.hpp>
#include <COM/InprocServer.hpp>
#include <COM/ClassFactory.hpp>
#include <COM/IDispatchImpl.hpp>

#endif // COM_COM_HPP
