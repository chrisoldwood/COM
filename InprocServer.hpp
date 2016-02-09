////////////////////////////////////////////////////////////////////////////////
//! \file   InprocServer.hpp
//! \brief  The InprocServer class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef COM_INPROCSERVER_HPP
#define COM_INPROCSERVER_HPP

#include <WCL/Dll.hpp>
#include "Server.hpp"
#include "ComMain.hpp"

namespace COM
{

// Forward declarations.
class ServerRegInfo;
struct ClassRegInfo;

//! The class factory smart-pointer type.
typedef WCL::IFacePtr<IClassFactory> IClassFactoryPtr;

//! The IUnknown smart-pointer type.
typedef WCL::IFacePtr<IUnknown> IUnknownPtr;

////////////////////////////////////////////////////////////////////////////////
//! The base class for In-process (DLL based) servers.

class InprocServer : public CDll, public Server
{
public:
	//! Default constructor.
	InprocServer();

	//! Destructor.
	virtual ~InprocServer();

	//! Singleton accessors.
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

	//! Register or unregister the server to/from the registry.
	virtual HRESULT DllInstall(bool install, const tchar* cmdLine);

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
	friend HRESULT STDAPICALLTYPE ::DllInstall(BOOL, const tchar*);
	friend class ClassFactory;

	//! Register the server in the registry.
	HRESULT RegisterServer(bool perUser);

	//! Unregister the server from the registry.
	HRESULT UnregisterServer(bool perUser);
};

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
//namespace COM
}

#endif // COM_INPROCSERVER_HPP
