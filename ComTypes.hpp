////////////////////////////////////////////////////////////////////////////////
//! \file   ComTypes.hpp
//! \brief  Common COM types and macros.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef COM_COMTYPES_HPP
#define COM_COMTYPES_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/IFacePtr.hpp>
#include <WCL/StrCvt.hpp>
#include <WCL/ComException.hpp>
#include <WCL/Win32Exception.hpp>

namespace COM
{

////////////////////////////////////////////////////////////////////////////////
// Useful COM macros.

//! The COM calling convention.
#define COMCALL		STDMETHODCALLTYPE

//! The standard COM return type and calling convention,
#define COMAPI		virtual HRESULT COMCALL

////////////////////////////////////////////////////////////////////////////////
//! COM server type.

enum ServerType
{
	INPROC_SERVER	= 1,		//!< In-process (DLL) based server (CLSCTX_INPROC_SERVER).
	LOCAL_SERVER	= 2,		//!< Out-of-process (EXE) based server (CLSCTX_LOCAL_SERVER).
};

////////////////////////////////////////////////////////////////////////////////
//! COM threading model.

enum ThreadingModel
{
	MAIN_THREAD_APT		= 1,	//!< ThreadingModel=[not set]
	SINGLE_THREAD_APT	= 2,	//!< ThreadingModel=Apartment
	FREE_THREAD_APT		= 3,	//!< ThreadingModel=Free
	ANY_APARTMENT		= 4,	//!< ThreadingModel=Both
	NEUTRAL_APARTMENT	= 5,	//!< ThreadingModel=Neutral
};

////////////////////////////////////////////////////////////////////////////////
// Macros for defining the interface table and IUnknown methods.

//! Implements IUnknown and ISupportErrorInfo.
#define IMPLEMENT_IUNKNOWN()											\
									virtual ULONG COMCALL AddRef()		\
									{	return AddRefImpl();	}		\
									virtual ULONG COMCALL Release()		\
									{	return ReleaseImpl();	}		\
									virtual HRESULT COMCALL QueryInterface(const IID& rIID, void** ppInterface)		\
									{	return QueryInterfaceImpl(rIID, ppInterface);	}							\
									virtual HRESULT COMCALL InterfaceSupportsErrorInfo(const IID& rIID)				\
									{	return InterfaceSupportsErrorInfoImpl(rIID);	}

//! Implements interface_cast to match and downcast to the rquired interface.
#define DEFINE_INTERFACE_TABLE(primary_iface)												\
									virtual void* interface_cast(const IID& rIID)			\
									{														\
										if (IsEqualIID(rIID, IID_IUnknown))					\
											return static_cast<primary_iface*>(this);		\
										if (IsEqualIID(rIID, IID_ISupportErrorInfo))		\
											return static_cast<ISupportErrorInfo*>(this);

//! Adds a match for an interface.
#define IMPLEMENT_INTERFACE(iid, iface_name)												\
										if (IsEqualIID(rIID, iid))							\
											return static_cast<iface_name*>(this);

//! End of CastInterfaceImpl implementation.
#define END_INTERFACE_TABLE()																\
										return nullptr;										\
									}								
									
//namespace COM
}

#endif // COM_COMTYPES_HPP
