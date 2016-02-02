////////////////////////////////////////////////////////////////////////////////
//! \file   InprocServerTests.cpp
//! \brief  The unit tests for the InprocServer class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include "TestClasses.hpp"

TEST_SET(InprocServer)
{

TEST_CASE("locking and unlocking the server modifies the lock count")
{
	TestServer server;

	long count = server.LockCount();

	server.Lock();

	TEST_TRUE(server.LockCount() == count+1);

	server.Unlock();

	TEST_TRUE(server.LockCount() == count);
}
TEST_CASE_END

TEST_CASE("this provides access to the current global instance")
{
	TestServer server;

	TEST_TRUE(&COM::InprocServer::This() == &server);
	TEST_TRUE(&COM::Server::This() == &server);
}
TEST_CASE_END

TEST_CASE("loading the type library returns a valid object")
{
	TestServer server;

	CModule oModule(::GetModuleHandle(NULL)); // Simulate DLL load/unload.

	TEST_TRUE(server.LoadTypeLibrary().get() != nullptr);
}
TEST_CASE_END

}
TEST_SET_END
