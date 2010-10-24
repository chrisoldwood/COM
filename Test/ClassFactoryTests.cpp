////////////////////////////////////////////////////////////////////////////////
//! \file   ClassFactoryTests.cpp
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

TEST_CASE("creating and destroying class factory modifies the server lock count")
{
	TestServer server;

	long count = server.LockCount();

	IClassFactoryPtr factory(new COM::ClassFactory(CLSID_TestClass), true);	

	TEST_TRUE(server.LockCount() == count+1);

	factory.Release();

	TEST_TRUE(server.LockCount() == count);
}
TEST_CASE_END

TEST_CASE("locking the server modifies the server lock count")
{
	TestServer       server;
	IClassFactoryPtr factory(new COM::ClassFactory(CLSID_TestClass), true);	

	long count = server.LockCount();

	factory->LockServer(TRUE);

	TEST_TRUE(server.LockCount() == count+1);

	factory->LockServer(FALSE);

	TEST_TRUE(server.LockCount() == count);
}
TEST_CASE_END

TEST_CASE("creating an instance returns S_OK and a new object")
{
	TestServer        server;
	IClassFactoryPtr  factory(new COM::ClassFactory(CLSID_TestClass), true);	
	ITestInterfacePtr t;

	HRESULT result = factory->CreateInstance(nullptr, IID_ITestInterface, reinterpret_cast<void**>(AttachTo(t)));

	TEST_TRUE(result == S_OK);
	TEST_TRUE(t.get() != nullptr);
}
TEST_CASE_END

}
TEST_SET_END
