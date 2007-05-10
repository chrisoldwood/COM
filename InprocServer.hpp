////////////////////////////////////////////////////////////////////////////////
//! \file   InprocServer.hpp
//! \brief  The InprocServer class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef COM_INPROCSERVER_HPP
#define COM_INPROCSERVER_HPP

namespace COM
{

////////////////////////////////////////////////////////////////////////////////
//! The base class for In-prcoess (DLL based) servers.

class InprocServer : public CDll, public Server
{
public:
	//! Default constructor.
	InprocServer();

	//! Destructor.
	virtual ~InprocServer();
	
	//! Singleton accessor.
	static InprocServer& This();

protected:
	//
	// COM entry point methods.
	//

	//! Get the class factory.
	virtual HRESULT DllGetClassObject(REFCLSID roCLSID, REFIID roIID, LPVOID* ppFactory);

	//! Query if the server can be unloaded.
	virtual HRESULT DllCanUnloadNow();

	//! Register the server in the registry.
	virtual HRESULT DllRegisterServer();

	//! Unregister the server from the registry.
	virtual HRESULT DllUnregisterServer();

	//
	// Internal methods.
	//

	//! Template Method used to obtain the server type information.
	virtual void GetServerRegInfo(ServerRegInfo& oInfo) const = 0;

	//! Template Method used to obtain the server coclasses information.
	virtual const ClassRegInfo* GetClassRegInfo() const = 0;

	//! Template Method to get the servers class factory.
	virtual COM::IClassFactoryPtr CreateClassFactory(const CLSID& oCLSID);

	//! Template Method to allocate an object for the class factory.
	virtual COM::IUnknownPtr CreateObject(const CLSID& oCLSID) = 0;

private:
	//
	// Class members.
	//

	//! The singleton inproc server.
	static InprocServer* g_pThis;

	//
	// Friends.
	//

	friend HRESULT STDAPICALLTYPE ::DllGetClassObject(REFCLSID oCLSID, REFIID oIID, LPVOID* ppFactory);
	friend HRESULT STDAPICALLTYPE ::DllCanUnloadNow(void);
	friend HRESULT STDAPICALLTYPE ::DllRegisterServer(void);
	friend HRESULT STDAPICALLTYPE ::DllUnregisterServer(void);
	friend class ClassFactory;
};

//namespace COM
}

#endif // COM_INPROCSERVER_HPP
