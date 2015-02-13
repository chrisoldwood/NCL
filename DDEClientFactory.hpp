////////////////////////////////////////////////////////////////////////////////
//! \file   DDEClientFactory.hpp
//! \brief  The DDEClientFactory class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef NCL_DDECLIENTFACTORY_HPP
#define NCL_DDECLIENTFACTORY_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "DDEFwd.hpp"
#include "DDEClient.hpp"
#include <Core/SharedPtr.hpp>

namespace DDE
{

////////////////////////////////////////////////////////////////////////////////
//! The factory used to create a DDE client.

class DDEClientFactory
{
public:
	//! The DDE client factory function type.
	typedef IDDEClientPtr (*FactoryFn)(DWORD flags);

public:
	//! Create a DDE client instance.
	static IDDEClientPtr create(DWORD flags = CDDEClient::DEFAULT_FLAGS);

	//! Register a custom factory function.
	static void registerFactory(FactoryFn factoryFn);

private:
	//! The factory function.
	static FactoryFn s_factoryFn;

	// Static class.
	DDEClientFactory();
	~DDEClientFactory();

	//! The default factory implementation.
	static IDDEClientPtr createClient(DWORD flags);
};

//namespace DDE
}

#endif // NCL_DDECLIENTFACTORY_HPP
