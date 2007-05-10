////////////////////////////////////////////////////////////////////////////////
//! \file   ClassFactory.hpp
//! \brief  The ClassFactory class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef COM_CLASSFACTORY_HPP
#define COM_CLASSFACTORY_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace COM
{

////////////////////////////////////////////////////////////////////////////////
//! The singleton coclass factory.

class ClassFactory : public ObjectBase<IClassFactory>
{
public:
	//! Default constructor.
	ClassFactory(const CLSID& rCLSID);

	//! Destructor.
	~ClassFactory();
	
	DEFINE_INTERFACE_TABLE(IClassFactory)
		IMPLEMENT_INTERFACE(IID_IClassFactory, IClassFactory)
	END_INTERFACE_TABLE()
	IMPLEMENT_IUNKNOWN()

	//
	// IClassFactory methods.
	//

	//! Create an instance object of the class.
	virtual HRESULT COMCALL CreateInstance(IUnknown* pOuter, const IID& rIID, void** ppInterface);

	//! Lock the COM server.
	virtual HRESULT COMCALL LockServer(BOOL fLock);
	
private:
	//
	// Members.
	//
	CLSID	m_oCLSID;		//!< The CLSID to manufacture objects of.
};

//namespace COM
}

#endif // COM_CLASSFACTORY_HPP
