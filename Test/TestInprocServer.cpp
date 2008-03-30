////////////////////////////////////////////////////////////////////////////////
//! \file   TestInprocServer.cpp
//! \brief  The unit tests for the InprocServer class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include "TestClasses.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the COM ObjectBase class.

void TestInprocServer()
{
	typedef WCL::IFacePtr<ITestInterface> ITestInterfacePtr;

	TestServer oServer;

	TEST_TRUE(oServer.LockCount() == 0);

	TestClass* pTestClass = new TestClass;
	ITestInterfacePtr p(pTestClass, true);

	TEST_TRUE(oServer.LockCount() == 1);

	ITestInterfacePtr p2(p);
	ASSERT(pTestClass->GetRefCount() == 2);

	TEST_TRUE(oServer.LockCount() == 1);

	p2.Release();

	TEST_TRUE(oServer.LockCount() == 1);

	p.Release();

	TEST_TRUE(oServer.LockCount() == 0);

	CModule oModule(::GetModuleHandle(NULL));

	TEST_TRUE(oServer.LoadTypeLibrary().Get() != nullptr);
}
