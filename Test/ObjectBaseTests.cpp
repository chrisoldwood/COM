////////////////////////////////////////////////////////////////////////////////
//! \file   TestObjectBase.cpp
//! \brief  The unit tests for the ObjectBase class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include "TestClasses.hpp"
#include <WCL/ComPtr.hpp>

#ifndef _MSC_VER
WCL_DECLARE_IFACETRAITS(IUnknown, IID_IUnknown);
WCL_DECLARE_IFACETRAITS(ITestInterface, IID_ITestInterface);
#endif

TEST_SET(ObjectBase)
{
	typedef WCL::ComPtr<IUnknown> IUnknownPtr;
	typedef WCL::ComPtr<ITestInterface> ITestInterfacePtr;

	TestServer oServer;
	TestClass* pTestClass = new TestClass;

	TEST_TRUE(pTestClass->GetRefCount() == 0);

	ITestInterfacePtr p(pTestClass, true);

	TEST_TRUE(pTestClass->GetRefCount() == 1);

	ITestInterfacePtr p2(p);

	TEST_TRUE(pTestClass->GetRefCount() == 2);

	p2.Release();

	TEST_TRUE(pTestClass->GetRefCount() == 1);

	p.Release();

	ASSERT(p.get() == nullptr);

	p = ITestInterfacePtr(new TestClass, true);

	IUnknownPtr u(p);

	TEST_TRUE(u.get() != nullptr);
	TEST_TRUE(pTestClass->GetRefCount() == 2);

	ITestInterfacePtr t(u);

	TEST_TRUE(t.get() != nullptr);
	TEST_TRUE(pTestClass->GetRefCount() == 3);
}
TEST_SET_END
