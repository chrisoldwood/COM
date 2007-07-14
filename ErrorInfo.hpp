////////////////////////////////////////////////////////////////////////////////
//! \file   ErrorInfo.hpp
//! \brief  COM error handling macros and functions.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef COM_ERRORINFO_HPP
#define COM_ERRORINFO_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace COM
{

//! Set the COM ErrorInfo object for the logical thread.
bool SetComErrorInfo(const tchar* pszSource, const tchar* pszDescription); // throw()

////////////////////////////////////////////////////////////////////////////////
// Macro for catching and handling exceptions at module boundaries.

//! Catch any exceptions, write the message to the debugger and set the return value.
#define COM_CATCH(retval)																	\
									catch (const WCL::ComException& e)						\
									{														\
										TRACE1("WCL::ComException exception caught in " __FUNCTION__ " - %s\n", e.what());	\
																							\
										COM::SetComErrorInfo(__FUNCTION__, e.what());		\
																							\
										retval = e.m_hResult;								\
									}														\
									catch (const WCL::Win32Exception& e)					\
									{														\
										TRACE1("WCL::Win32Exception exception caught in " __FUNCTION__ " - %s\n", e.what());	\
																							\
										COM::SetComErrorInfo(__FUNCTION__, e.what());		\
																							\
										retval = HRESULT_FROM_WIN32(e.m_dwError);			\
									}														\
									catch (const std::exception& e)							\
									{														\
										TRACE1("std::exception caught in " __FUNCTION__ " - %s\n", e.what());	\
																							\
										COM::SetComErrorInfo(__FUNCTION__, e.what());		\
																							\
										retval = E_UNEXPECTED;								\
									}

//namespace COM
}

#endif // COM_ERRORINFO_HPP
