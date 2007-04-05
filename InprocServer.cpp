////////////////////////////////////////////////////////////////////////////////
//! \author Chris Oldwood
//! \file   InprocServer.cpp
//! \brief  The InprocServer class definition.

#include "com.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

namespace COM
{

//! The singleton inproc server..
static InprocServer* pThis = NULL;

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

InprocServer::InprocServer()
{
	pThis = this;
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

InprocServer::~InprocServer()
{
	pThis = NULL;
}

////////////////////////////////////////////////////////////////////////////////
// Singleton accessor.

InprocServer& InprocServer::This()
{
	ASSERT(pThis != NULL);

	return *pThis;
}

//namespace COM
}
