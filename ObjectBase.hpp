////////////////////////////////////////////////////////////////////////////////
//! \file   ObjectBase.hpp
//! \brief  The ObjectBase class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef COM_OBJECTBASE_HPP
#define COM_OBJECTBASE_HPP

#include <COM/Server.hpp>
#include <unknwn.h>

#if _MSC_VER > 1000
#pragma once
#endif

namespace COM
{

////////////////////////////////////////////////////////////////////////////////
//! The base class for all COM objects. This provides an implementation of
//! IUnknown for dynamically allocated objects. It also marks the interface as
//! supporting COM exceptions which are handled automatically by the macro
//! COM_CATCH_* in ErrorInfo.hpp.

template<typename Base = IUnknown>
class ObjectBase : public Base, public ISupportErrorInfo
{
public:
	//! Default constructor.
	ObjectBase();

	//! Destructor.
	virtual ~ObjectBase();

	//
	// Properties.
	//

	//! Get the object reference count.
	ULONG GetRefCount() const;

	//
	// IUnknown methods.
	//

	//! Query the object for a particular interface.
	virtual HRESULT QueryInterfaceImpl(const IID& rIID, void** ppInterface);

	//! Increment the objects reference count.
	virtual ULONG AddRefImpl();

	//! Decrement the objects reference count.
	virtual ULONG ReleaseImpl();

	//
	// ISupportErrorInfo methods.
	//

	//! Queries if the interface supports COM exceptions.
	virtual HRESULT InterfaceSupportsErrorInfoImpl(const IID& rIID);

protected:
	//
	// Internal methods.
	//

	//! Template Method used to obtain the requested interface, if supported.
	virtual void* interface_cast(const IID& rIID) = 0;

private:
	//
	// Members.
	//
	LONG	m_nRefCount;	//!< The object reference count.
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

template<typename Base>
inline ObjectBase<Base>::ObjectBase()
	: m_nRefCount(0)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

template<typename Base>
inline ObjectBase<Base>::~ObjectBase()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Get the object reference count.

template<typename Base>
inline ULONG ObjectBase<Base>::GetRefCount() const
{
	return m_nRefCount;
}

////////////////////////////////////////////////////////////////////////////////
//! Query the object for a particular interface.

template<typename Base>
inline HRESULT ObjectBase<Base>::QueryInterfaceImpl(const IID& rIID, void** ppInterface)
{
	// Check parameters.
	if (ppInterface == nullptr)
		return E_POINTER;

	// Acquire interface pointer.
	*ppInterface = interface_cast(rIID);

	if (*ppInterface != nullptr)
		AddRefImpl();

	return (*ppInterface != nullptr) ? S_OK : E_NOINTERFACE;
}

////////////////////////////////////////////////////////////////////////////////
//! Increment the objects reference count.

template<typename Base>
inline ULONG ObjectBase<Base>::AddRefImpl()
{
	LONG nRefCount = ::InterlockedIncrement(&m_nRefCount);

	if (nRefCount == 1)
		Server::This().Lock();

	return m_nRefCount;
}

////////////////////////////////////////////////////////////////////////////////
//! Decrement the objects reference count.

template<typename Base>
inline ULONG ObjectBase<Base>::ReleaseImpl()
{
	ASSERT(m_nRefCount > 0);

	LONG nRefCount = ::InterlockedDecrement(&m_nRefCount);

	if (m_nRefCount == 0)
		Server::This().Unlock();

	if (m_nRefCount == 0)
		delete this;

	return nRefCount;
}

////////////////////////////////////////////////////////////////////////////////
//! Queries if the interface supports COM exceptions.

template<typename Base>
inline HRESULT ObjectBase<Base>::InterfaceSupportsErrorInfoImpl(const IID& /*rIID*/)
{
	return S_OK;
}

//namespace COM
}

#endif // COM_OBJECTBASE_HPP
