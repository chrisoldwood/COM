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
// Typedefs for some common interfaces.

//! The class factory smart-pointer type.
typedef Core::IFacePtr<IClassFactory> IClassFactoryPtr;

//! The IUnknown smart-pointer type.
typedef Core::IFacePtr<IUnknown> IUnknownPtr;

////////////////////////////////////////////////////////////////////////////////
// Macros for defining the interface table and IUnknown methods.

//! Implements IUnknown.
#define IMPLEMENT_IUNKNOWN()											\
									virtual ULONG COMCALL AddRef()		\
									{	return AddRefImpl();	}		\
									virtual ULONG COMCALL Release()		\
									{	return ReleaseImpl();	}		\
									virtual HRESULT COMCALL QueryInterface(const IID& rIID, void** ppInterface)		\
									{	return QueryInterfaceImpl(rIID, ppInterface);	}

//! Implements interface_cast to match and downcast to the rquired interface.
#define DEFINE_INTERFACE_TABLE(primary_iface)												\
									virtual void* interface_cast(const IID& rIID)			\
									{														\
										if (IsEqualIID(rIID, IID_IUnknown))					\
											return static_cast<primary_iface*>(this);

//! Adds a match for an interface.
#define IMPLEMENT_INTERFACE(iid, iface_name)												\
										if (IsEqualIID(rIID, iid))							\
											return static_cast<iface_name*>(this);

//! End of CastInterfaceImpl implementation.
#define END_INTERFACE_TABLE()																\
										return nullptr;										\
									}								
									
////////////////////////////////////////////////////////////////////////////////
// Macros for defining the class factory table.

#define DEFINE_CLASS_FACTORY_TABLE()																\
									virtual COM::IUnknownPtr CreateObject(const CLSID& oCLSID)		\
									{																\
										COM::IUnknownPtr pUnknown;									

#define DEFINE_CLASS(clsid, type, primary_iface)													\
										if (oCLSID == clsid)										\
											pUnknown = COM::IUnknownPtr(static_cast<primary_iface*>(new type), true);

#define END_CLASS_FACTORY_TABLE()																	\
										return pUnknown;											\
									}

////////////////////////////////////////////////////////////////////////////////
// Macros for catching and handling exceptions at module boundaries.

//! Catch any exceptions, write the message to the debugger and set the return value.
#define COM_CATCH_TRACE_AND_SET(func, retval)												\
									catch (const HRESULT& e)								\
									{														\
										TRACE2("Unhandled exception caught in " func " - %s [0x%08X]\n", CStrCvt::FormatError(e), e);	\
																							\
										retval = e;											\
									}														\
									catch (const WCL::ComException& e)						\
									{														\
										TRACE1("Unhandled exception caught in " func " - %s\n", e.what());	\
																							\
										retval = e.m_hResult;								\
									}														\
									catch (const WCL::Win32Exception& e)					\
									{														\
										TRACE1("Unhandled exception caught in " func " - %s\n", e.what());	\
																							\
										retval = HRESULT_FROM_WIN32(e.m_dwError);			\
									}														\
									catch (const std::exception& e)							\
									{														\
										TRACE1("Unhandled exception caught in " func " - %s\n", e.what());	\
																							\
										retval = E_UNEXPECTED;								\
																							\
										DEBUG_USE_ONLY(e);									\
									}

//namespace COM
}

#endif // COM_COMTYPES_HPP
