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

// Set the COM ErrorInfo object for the logical thread.
bool SetComErrorInfo(const char* pszSource, const tchar* pszDescription); // throw()

////////////////////////////////////////////////////////////////////////////////
// Macro for catching and handling exceptions at module boundaries.

//! Catch any exceptions, write the message to the debugger and set the return value.
#define COM_CATCH(retval)																	\
									catch (const WCL::ComException& e)						\
									{														\
										TRACE2(TXT("WCL::ComException exception caught in '%hs' - %s\n"), __FUNCTION__, e.twhat());	\
																							\
										COM::SetComErrorInfo(__FUNCTION__, e.twhat());		\
																							\
										retval = e.m_result;								\
									}														\
									catch (const WCL::Win32Exception& e)					\
									{														\
										TRACE2(TXT("WCL::Win32Exception exception caught in '%hs' - %s\n"), __FUNCTION__, e.twhat());	\
																							\
										COM::SetComErrorInfo(__FUNCTION__, e.twhat());		\
																							\
										retval = HRESULT_FROM_WIN32(e.m_dwError);			\
									}														\
									catch (const Core::Exception& e)						\
									{														\
										TRACE2(TXT("Core::Exception exception caught in '%hs' - %s\n"), __FUNCTION__, e.twhat());	\
																							\
										COM::SetComErrorInfo(__FUNCTION__, e.twhat());		\
																							\
										retval = E_UNEXPECTED;								\
									}														\
									catch (const std::exception& e)							\
									{														\
										TRACE2(TXT("std::exception caught in '%hs' - %hs\n"), __FUNCTION__, e.what());	\
																							\
										COM::SetComErrorInfo(__FUNCTION__, A2T(e.what()));	\
																							\
										retval = E_UNEXPECTED;								\
									}														\
									catch (...)												\
									{														\
										const tchar* what = TXT("Unknown exception");		\
										TRACE1(TXT("Unknown exception caught in '%hs'\n"), __FUNCTION__);	\
																							\
										COM::SetComErrorInfo(__FUNCTION__, what);			\
																							\
										retval = E_UNEXPECTED;								\
									}

//namespace COM
}

#endif // COM_ERRORINFO_HPP
