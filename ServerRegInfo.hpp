////////////////////////////////////////////////////////////////////////////////
//! \file   ServerRegInfo.hpp
//! \brief  The ServerRegInfo class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef COM_SERVERREGINFO_HPP
#define COM_SERVERREGINFO_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace COM
{

////////////////////////////////////////////////////////////////////////////////
//! The data class used to store the library registry details.

class ServerRegInfo
{
public:
	//! Default constructor.
	ServerRegInfo();

	//
	// Members.
	//
	ServerType		m_eType;		//!< The server type.
	tstring			m_strFile;		//!< The full path to the file.
	tstring			m_strLibrary;	//!< The library name.
	GUID			m_oLIBID;		//!< The type library GUID.
	ushort			m_nMajor;		//!< The type library major version.
	ushort			m_nMinor;		//!< The type library minor version.
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

inline ServerRegInfo::ServerRegInfo()
	: m_eType(INPROC_SERVER)
	, m_strFile()
	, m_strLibrary()
	, m_oLIBID(IID_NULL)
	, m_nMajor(0)
	, m_nMinor(0)
{
}

////////////////////////////////////////////////////////////////////////////////
//! The data class used to store the coclass registry details.

struct ClassRegInfo
{
	const CLSID*	m_pCLSID;		//!< The coclass GUID.
	const tchar*	m_pszName;		//!< The coclass name.
	const tchar*	m_pszVersion;	//!< The coclass version.
	ThreadingModel	m_eModel;		//!< The threading model.
};

////////////////////////////////////////////////////////////////////////////////
// Macros for [un]registering the COM classes and type library.

#define	DEFINE_REGISTRATION_TABLE(libname, libuuid, major, minor)			\
		virtual void GetServerRegInfo(COM::ServerRegInfo& oInfo) const		\
		{																	\
			oInfo.m_strLibrary = libname;									\
			oInfo.m_oLIBID     = libuuid;									\
			oInfo.m_nMajor     = major;										\
			oInfo.m_nMinor     = minor;										\
		}																	\
		virtual const COM::ClassRegInfo* GetClassRegInfo() const			\
		{																	\
			static COM::ClassRegInfo aoClasses[] =							\
			{														

#define DEFINE_CLASS_REG_INFO(clsid, name, version, model)					\
				{ &clsid, name, version, model },					

#define END_REGISTRATION_TABLE()											\
				{ NULL, NULL, NULL, static_cast<COM::ThreadingModel>(0) }	\
			};																\
			return aoClasses;												\
		}																

//namespace COM
}

#endif // COM_SERVERREGINFO_HPP
