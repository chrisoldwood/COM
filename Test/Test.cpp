////////////////////////////////////////////////////////////////////////////////
//! \file   Test.cpp
//! \brief  The test harness entry point.
//! \author Chris Oldwood

#include "stdafx.h"
#include <tchar.h>
#include <Core/UnitTest.hpp>

////////////////////////////////////////////////////////////////////////////////
// The test group functions.

extern void TestComUtils();
extern void TestObjectBase();
extern void TestInprocServer();
extern void TestClassFactory();
extern void TestErrorInfo();

////////////////////////////////////////////////////////////////////////////////
//! The entry point for the test harness.

int _tmain(int argc, _TCHAR* argv[])
{
	TEST_SUITE_BEGIN(argc, argv)
	{
		TEST_CASE(TestComUtils);
		TEST_CASE(TestObjectBase);
		TEST_CASE(TestInprocServer);
		TEST_CASE(TestClassFactory);
		TEST_CASE(TestErrorInfo);

		Core::SetTestRunFinalStatus(true);
	}
	TEST_SUITE_END
}
