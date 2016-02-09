////////////////////////////////////////////////////////////////////////////////
//! \file   Server.hpp
//! \brief  The Server class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef COM_SERVER_HPP
#define COM_SERVER_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/IFacePtr.hpp>
#include <oaidl.h>

namespace COM
{

//! The Type Library smart-pointer type.
typedef WCL::IFacePtr<ITypeLib> ITypeLibPtr;

////////////////////////////////////////////////////////////////////////////////
//! The mix-in class used for the common (DLL/EXE) COM server behaviour.

class Server : private Core::NotCopyable
{
public:
	//
	// Properties.
	//

	//! Query the lock count.
	long LockCount() const;

	//
	// Methods.
	//

	//! Singleton accessor.
	static Server& This();

	//! Lock the server.
	virtual void Lock();

	//! Unlock the server.
	virtual void Unlock();

	//! Load the type library.
	ITypeLibPtr LoadTypeLibrary() const;	// throw(ComException)

protected:
	//! Default constructor.
	Server();

	//! Destructor.
	virtual ~Server();

private:
	//
	// Members.
	//
	long	m_nLockCount;	//!< The lock count.

	//
	// Class members.
	//

	//! The singleton COM server.
	static Server* g_pThis;
};

////////////////////////////////////////////////////////////////////////////////
//! Query the lock count.

inline long Server::LockCount() const
{
	return m_nLockCount;
}

//namespace COM
}

#endif // COM_SERVER_HPP
