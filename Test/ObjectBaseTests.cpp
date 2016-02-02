////////////////////////////////////////////////////////////////////////////////
//! \file   ObjectBaseTests.cpp
//! \brief  The unit tests for the ObjectBase class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include "TestClasses.hpp"
#include <COM/ClassFactory.hpp>
#include <WCL/ComPtr.hpp>

#ifndef _MSC_VER
WCL_DECLARE_IFACETRAITS(IUnknown, IID_IUnknown);
WCL_DECLARE_IFACETRAITS(ITestInterface, IID_ITestInterface);
#endif

TEST_SET(ObjectBase)
{
	typedef WCL::ComPtr<IUnknown> IUnknownPtr;
	typedef WCL::ComPtr<ITestInterface> ITestInterfacePtr;
	typedef WCL::ComPtr<IClassFactory> IClassFactoryPtr;

TEST_CASE("new object begins with zero reference count")
{
	TestServer server;
	TestClass* object = new TestClass;

	TEST_TRUE(object->GetRefCount() == 0);

	delete object;
}
TEST_CASE_END

TEST_CASE("acquiring and releasing an interfaces modifies the reference count")
{
	TestServer server;
	TestClass* object = new TestClass;

	ulong count = object->GetRefCount();

	ITestInterfacePtr iface1(object, true);

	TEST_TRUE(object->GetRefCount() == count+1);

	ITestInterfacePtr iface2(iface1);

	TEST_TRUE(object->GetRefCount() == count+2);

	iface1.Release();

	TEST_TRUE(object->GetRefCount() == count+1);

	iface2.Release();
}
TEST_CASE_END

TEST_CASE("IUnknown can be acquired via any interface")
{
	TestServer        server;
	TestClass*        object(new TestClass);
	ITestInterfacePtr iface(object, true);

	IUnknownPtr unknown(iface);

	TEST_TRUE(unknown.get() != nullptr);
}
TEST_CASE_END

TEST_CASE("creating and destroying an object modifies the server lock count")
{
	TestServer        server;
	IClassFactoryPtr  factory(new COM::ClassFactory(CLSID_TestClass), true);	
	ITestInterfacePtr t;

	long count = server.LockCount();

	factory->CreateInstance(nullptr, IID_ITestInterface, reinterpret_cast<void**>(AttachTo(t)));

	TEST_TRUE(server.LockCount() == count+1);

	t.Release();

	TEST_TRUE(server.LockCount() == count);
}
TEST_CASE_END

}
TEST_SET_END
