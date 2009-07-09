////////////////////////////////////////////////////////////////////////////////
//! \file   TestClasses.hpp
//! \brief  The classes used in various tests.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef TESTCLASSES_HPP
#define TESTCLASSES_HPP

#include <COM/ComTypes.hpp>
#include <COM/ObjectBase.hpp>
#include <COM/ServerRegInfo.hpp>
#include <COM/InprocServer.hpp>

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
// Test global variables.

static const IID IID_ITestInterface   = { 0x12345678, 0x1234, 0x1234, { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 } };
static const CLSID CLSID_TestClass    = { 0x12345678, 0x1234, 0x1234, { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 } };
static const GUID LIBID_TestServerLib = { 0x12345678, 0x1234, 0x1234, { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 } };

////////////////////////////////////////////////////////////////////////////////
//! The ObjectBase test class interface.

#ifdef _MSC_VER
[uuid ("{12345678-1234-1234-0102-030405060708}")]
#endif
struct ITestInterface : public IUnknown
{
};

////////////////////////////////////////////////////////////////////////////////
//! The ObjectBase test class.

class TestClass : public COM::ObjectBase<ITestInterface>
{
	DEFINE_INTERFACE_TABLE(ITestInterface)
		IMPLEMENT_INTERFACE(IID_ITestInterface, ITestInterface)
	END_INTERFACE_TABLE()
	IMPLEMENT_IUNKNOWN()
};

////////////////////////////////////////////////////////////////////////////////
//! The InprocServer test class.

class TestServer : public COM::InprocServer
{
	DEFINE_REGISTRATION_TABLE(TXT("TestServer"), LIBID_TestServerLib, 1, 0)
	END_REGISTRATION_TABLE()

	DEFINE_CLASS_FACTORY_TABLE()
		DEFINE_CLASS(CLSID_TestClass, TestClass, ITestInterface)
	END_CLASS_FACTORY_TABLE()
};

#endif // TESTCLASSES_HPP
