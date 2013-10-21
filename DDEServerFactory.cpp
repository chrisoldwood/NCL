////////////////////////////////////////////////////////////////////////////////
//! \file   DDEServerFactory.cpp
//! \brief  The DDEServerFactory class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "DDEServerFactory.hpp"

namespace DDE
{

//! The factory function.
DDEServerFactory::FactoryFn DDEServerFactory::s_factoryFn = createServer;

////////////////////////////////////////////////////////////////////////////////
//! Create a DDE server instance.

IDDEServerPtr DDEServerFactory::create(DWORD flags)
{
	return s_factoryFn(flags);
}

////////////////////////////////////////////////////////////////////////////////
//! Register a custom factory function.

void DDEServerFactory::registerFactory(FactoryFn factoryFn)
{
	s_factoryFn = factoryFn;
}

////////////////////////////////////////////////////////////////////////////////
//! The default factory implementation.

IDDEServerPtr DDEServerFactory::createServer(DWORD flags)
{
	return IDDEServerPtr(new CDDEServer(flags));
}

//namespace 
}
