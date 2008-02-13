////////////////////////////////////////////////////////////////////////////////
//! \file   IDispatchImpl.hpp
//! \brief  The IDispatchImpl class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef COM_IDISPATCHIMPL_HPP
#define COM_IDISPATCHIMPL_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "ComUtils.hpp"

namespace COM
{

////////////////////////////////////////////////////////////////////////////////
//! The implementaion of IDispatch.

template<typename T>
class IDispatchImpl
{
public:
	//! Full constructor.
	IDispatchImpl(const IID& oDIID);

	//! Destructor.
	virtual ~IDispatchImpl();
	
	//
	// IDispatch methods.
	//

	//! Get the number of type information interfaces supported.
	virtual HRESULT COMCALL GetTypeInfoCount(UINT* pnInfo);

	//! Get the type information for an object.
	virtual HRESULT COMCALL GetTypeInfo(UINT nInfo, LCID dwLCID, ITypeInfo** ppTypeInfo);

	//! Map a number of names to their dispatch IDs.
	virtual HRESULT COMCALL GetIDsOfNames(REFIID rIID, LPOLESTR* aszNames, UINT nNames, LCID dwLCID, DISPID* alMemberIDs);

	//! Invoke a method or access a property.
	virtual HRESULT COMCALL Invoke(DISPID lMemberID, REFIID rIID, LCID dwLCID, WORD wFlags, DISPPARAMS* pParams, VARIANT* pResult, EXCEPINFO* pExcepInfo, UINT* pnArgError);

private:
	// Type shorthands.
	typedef Core::IFacePtr<ITypeLib>  ITypeLibPtr;
	typedef Core::IFacePtr<ITypeInfo> ITypeInfoPtr;

	//
	// Members.
	//
	IID				m_oDIID;		//!< The dual interface ID.
	ITypeLibPtr		m_pTypeLib;		//!< The type library.
	ITypeInfoPtr	m_pTypeInfo;	//!< The interface type information.

	//
	// Internal methods.
	//

	//! Load the type information.
	void LoadTypeInfo();	// throw(ComException)
};

////////////////////////////////////////////////////////////////////////////////
//! Full constructor.

template<typename T>
IDispatchImpl<T>::IDispatchImpl(const IID& oDIID)
	: m_oDIID(oDIID)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

template<typename T>
IDispatchImpl<T>::~IDispatchImpl()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Get the number of type information interfaces supported.

template<typename T>
HRESULT COMCALL IDispatchImpl<T>::GetTypeInfoCount(UINT* pnInfo)
{
	HRESULT hr = S_OK;

	try
	{
		// Check output parameters.
		if (pnInfo == nullptr)
			throw WCL::ComException(E_POINTER, TXT("pnInfo is NULL"));

		*pnInfo = 1;
	}
	COM_CATCH(hr)

	return hr;
}

////////////////////////////////////////////////////////////////////////////////
//! Get the type information for an object.

template<typename T>
HRESULT COMCALL IDispatchImpl<T>::GetTypeInfo(UINT nInfo, LCID /*dwLCID*/, ITypeInfo** ppTypeInfo)
{
	HRESULT hr = S_OK;

	try
	{
		// Check output parameters.
		if (ppTypeInfo == nullptr)
			throw WCL::ComException(E_POINTER, TXT("ppTypeInfo is NULL"));

		// Reset output parameters.
		*ppTypeInfo = nullptr;

		// Validate parameters.
		if (nInfo != 0)
			throw WCL::ComException(DISP_E_BADINDEX, TXT("nInfo must be 0"));

		// Load on first request.
		if (m_pTypeLib.Get() ==  nullptr || m_pTypeInfo.Get() == nullptr)
			LoadTypeInfo();

		m_pTypeInfo->AddRef();

		*ppTypeInfo = m_pTypeInfo.Get();
	}
	COM_CATCH(hr)

	return hr;
}

////////////////////////////////////////////////////////////////////////////////
//! Map a number of names to their dispatch IDs.

template<typename T>
HRESULT COMCALL IDispatchImpl<T>::GetIDsOfNames(REFIID /*rIID*/, LPOLESTR* aszNames, UINT nNames, LCID /*dwLCID*/, DISPID* alMemberIDs)
{
	HRESULT hr = S_OK;

	try
	{
		// Load on first request.
		if (m_pTypeLib.Get() ==  nullptr || m_pTypeInfo.Get() == nullptr)
			LoadTypeInfo();

		hr = m_pTypeInfo->GetIDsOfNames(aszNames, nNames, alMemberIDs);
	}
	COM_CATCH(hr)

	return hr;
}

////////////////////////////////////////////////////////////////////////////////
//! Invoke a method or access a property.

template<typename T>
HRESULT COMCALL IDispatchImpl<T>::Invoke(DISPID lMemberID, REFIID /*rIID*/, LCID /*dwLCID*/, WORD wFlags, DISPPARAMS* pParams, VARIANT* pResult, EXCEPINFO* pExcepInfo, UINT* pnArgError)
{
	HRESULT hr = S_OK;

	try
	{
		// Load on first request.
		if (m_pTypeLib.Get() ==  nullptr || m_pTypeInfo.Get() == nullptr)
			LoadTypeInfo();

		// Clear the last exception.
		::SetErrorInfo(0, nullptr);

		hr = m_pTypeInfo->Invoke(static_cast<T*>(this), lMemberID, wFlags, pParams, pResult, pExcepInfo, pnArgError);
	}
	COM_CATCH(hr)

	return hr;
}

////////////////////////////////////////////////////////////////////////////////
//! Load the type information.

template<typename T>
void IDispatchImpl<T>::LoadTypeInfo()
{
	// Load the type library..
	if (m_pTypeLib.Get() == nullptr)
		m_pTypeLib = COM::Server::This().LoadTypeLibrary();

	// Retrieve the type info for the interface.
	if (m_pTypeInfo.Get() == nullptr)
	{
		HRESULT hr = m_pTypeLib->GetTypeInfoOfGuid(m_oDIID, AttachTo(m_pTypeInfo));

		if (FAILED(hr))
		{
			std::tstring strGUID = FormatGUID(m_oDIID);
			std::tstring strName = LookupIID(m_oDIID);

			throw WCL::ComException(hr, CString::Fmt(TXT("Failed to get the type information for %s [%s]"), strGUID.c_str(), strName.c_str()));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Macros for defining the IDispatch methods.

//! Implements IUnknown.
#define IMPLEMENT_IDISPATCH(T)																	\
	virtual HRESULT COMCALL GetTypeInfoCount(UINT* pnInfo)										\
	{ return COM::IDispatchImpl<T>::GetTypeInfoCount(pnInfo); }									\
	virtual HRESULT COMCALL GetTypeInfo(UINT nInfo, LCID dwLCID, ITypeInfo** ppTypeInfo)		\
	{ return COM::IDispatchImpl<T>::GetTypeInfo(nInfo, dwLCID, ppTypeInfo); }					\
	virtual HRESULT COMCALL GetIDsOfNames(REFIID rIID, LPOLESTR* aszNames, UINT nNames, LCID dwLCID, DISPID* alMemberIDs)														\
	{ return COM::IDispatchImpl<T>::GetIDsOfNames(rIID, aszNames, nNames, dwLCID, alMemberIDs); }																				\
	virtual HRESULT COMCALL Invoke(DISPID lMemberID, REFIID rIID, LCID dwLCID, WORD wFlags, DISPPARAMS* pParams, VARIANT* pResult, EXCEPINFO* pExcepInfo, UINT* pnArgError)		\
	{ return COM::IDispatchImpl<T>::Invoke(lMemberID, rIID, dwLCID, wFlags, pParams, pResult, pExcepInfo, pnArgError); }																

//namespace COM
}

#endif // COM_IDISPATCHIMPL_HPP
