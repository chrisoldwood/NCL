////////////////////////////////////////////////////////////////////////////////
//! \file   DDEServerFactory.hpp
//! \brief  The DDEServerFactory class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef NCL_DDESERVERFACTORY_HPP
#define NCL_DDESERVERFACTORY_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "DDEServer.hpp"
#include <Core/SharedPtr.hpp>

namespace DDE
{

////////////////////////////////////////////////////////////////////////////////
//! The factory used to create a DDE server.

class DDEServerFactory
{
public:
	//! The DDE server factory function type.
	typedef IDDEServerPtr (*FactoryFn)(DWORD flags);

public:
	//! Create a DDE server instance.
	static IDDEServerPtr create(DWORD flags = CDDEServer::DEFAULT_FLAGS);

	//! Register a custom factory function.
	static void registerFactory(FactoryFn factoryFn);

private:
	//! The factory function.
	static FactoryFn s_factoryFn;

	// Static class.
	DDEServerFactory();
	~DDEServerFactory();

	//! The default factory implementation.
	static IDDEServerPtr createServer(DWORD flags);
};

//namespace 
}

#endif // NCL_DDESERVERFACTORY_HPP
