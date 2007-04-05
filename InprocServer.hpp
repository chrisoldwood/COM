////////////////////////////////////////////////////////////////////////////////
//! \author Chris Oldwood
//! \file   InprocServer.hpp
//! \brief  The CInprocServer class declaration.

// Check for previous inclusion
#ifndef INPROCSERVER_HPP
#define INPROCSERVER_HPP

namespace COM
{

////////////////////////////////////////////////////////////////////////////////
//! The base class for In-proess (i.e. DLL based) COM servers.

class InprocServer : public CDll
{
public:
	//! Default constructor.
	InprocServer();

	//! Destructor.
	~InprocServer();
	
	// Singleton accessor.
	static InprocServer& This();

private:
	//
	// COM entry point methods.
	//

	//! Get the class factory.
	virtual HRESULT DllGetClassObject(REFCLSID roCLSID, REFIID roIID, LPVOID* ppFactory) = 0;

	//! Query if the server can be unloaded.
	virtual HRESULT DllCanUnloadNow() = 0;

	//! Register the server in the registry.
	virtual HRESULT DllRegisterServer() = 0;

	//! Unregister the server from the registry.
	virtual HRESULT DllUnregisterServer() = 0;

	//
	// Friends.
	//

	friend HRESULT STDAPICALLTYPE ::DllGetClassObject(REFCLSID oCLSID, REFIID oIID, LPVOID* ppFactory);
	friend HRESULT STDAPICALLTYPE ::DllCanUnloadNow(void);
	friend HRESULT STDAPICALLTYPE ::DllRegisterServer(void);
	friend HRESULT STDAPICALLTYPE ::DllUnregisterServer(void);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

//namespace COM
}

#endif // INPROCSERVER_HPP
