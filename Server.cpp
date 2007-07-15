////////////////////////////////////////////////////////////////////////////////
//! \file   Server.cpp
//! \brief  The Server class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "Server.hpp"
#include <WCL/Path.hpp>
#include <WCL/Module.hpp>

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

namespace COM
{

//! The singleton COM server.
Server* Server::g_pThis = NULL;

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

Server::Server()
	: m_nLockCount(0)
{
	ASSERT(g_pThis == nullptr);

	g_pThis = this;
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

Server::~Server()
{
	ASSERT(m_nLockCount == 0);
	ASSERT(g_pThis == this);

	g_pThis = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
// Singleton accessor.

Server& Server::This()
{
	ASSERT(g_pThis != nullptr);

	return *g_pThis;
}

////////////////////////////////////////////////////////////////////////////////
//! Lock the server. This is used to ensure that the server is not unloaded
//! whilst it still has objects alive.

void Server::Lock()
{
	::InterlockedIncrement(&m_nLockCount);
}

////////////////////////////////////////////////////////////////////////////////
//! Unlock the server.

void Server::Unlock()
{
	ASSERT(m_nLockCount > 0);

	::InterlockedDecrement(&m_nLockCount);
}

////////////////////////////////////////////////////////////////////////////////
//! Load the type library.

ITypeLibPtr Server::LoadTypeLibrary() const
{
	USES_CONVERSION;

	CPath       strFile = CModule::This().Path();
	ITypeLibPtr pTypeLib;

	// Load the type library.
	HRESULT hr = ::LoadTypeLib(T2W(strFile), AttachTo(pTypeLib));

	if (FAILED(hr))
		throw WCL::ComException(hr, CString::Fmt("Failed to load the type library '%s'", strFile));

	return pTypeLib;
}

//namespace COM
}
