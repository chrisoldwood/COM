////////////////////////////////////////////////////////////////////////////////
//! \file   TestClassFactory.cpp
//! \brief  The unit tests for the ClassFactory class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include "TestClasses.hpp"
#include <WCL/ComPtr.hpp>
#include <COM/ClassFactory.hpp>

TEST_SET(ClassFactory)
{
	typedef WCL::ComPtr<IClassFactory> IClassFactoryPtr;
	typedef WCL::ComPtr<ITestInterface> ITestInterfacePtr;

	TestServer oServer;

	IClassFactoryPtr p(new COM::ClassFactory(CLSID_TestClass), true);	

	TEST_TRUE(oServer.LockCount() == 1);

	p->LockServer(TRUE);

	TEST_TRUE(oServer.LockCount() == 2);

	ITestInterfacePtr t;

	TEST_TRUE(p->CreateInstance(nullptr, IID_ITestInterface, reinterpret_cast<void**>(AttachTo(t))) == S_OK);
	TEST_TRUE(oServer.LockCount() == 3);

	t.Release();
	p->LockServer(FALSE);
	p.Release();

	TEST_TRUE(oServer.LockCount() == 0);
}
TEST_SET_END
