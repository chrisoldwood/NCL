////////////////////////////////////////////////////////////////////////////////
//! \file   DDEClientFactory.cpp
//! \brief  The DDEClientFactory class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "DDEClientFactory.hpp"

namespace DDE
{

//! The factory function.
DDEClientFactory::FactoryFn DDEClientFactory::s_factoryFn = createClient;

////////////////////////////////////////////////////////////////////////////////
//! Create a DDE client instance.

IDDEClientPtr DDEClientFactory::create(DWORD flags)
{
	return s_factoryFn(flags);
}

////////////////////////////////////////////////////////////////////////////////
//! Register a custom factory function.

void DDEClientFactory::registerFactory(FactoryFn factoryFn)
{
	s_factoryFn = factoryFn;
}

////////////////////////////////////////////////////////////////////////////////
//! The default factory implementation.

IDDEClientPtr DDEClientFactory::createClient(DWORD flags)
{
	return IDDEClientPtr(new CDDEClient(flags));
}

//namespace DDE
}
