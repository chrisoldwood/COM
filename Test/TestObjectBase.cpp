////////////////////////////////////////////////////////////////////////////////
//! \file   TestObjectBase.cpp
//! \brief  The unit tests for the ObjectBase class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include "TestClasses.hpp"
#include <WCL/ComPtr.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the COM ObjectBase class.

void TestObjectBase()
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

	ASSERT(p.Get() == nullptr);

	p = ITestInterfacePtr(new TestClass, true);

	IUnknownPtr u(p);

	TEST_TRUE(u.Get() != nullptr);
	TEST_TRUE(pTestClass->GetRefCount() == 2);

	ITestInterfacePtr t(u);

	TEST_TRUE(t.Get() != nullptr);
	TEST_TRUE(pTestClass->GetRefCount() == 3);
}
