////////////////////////////////////////////////////////////////////////////////
//! \file   IDDEServer.hpp
//! \brief  The IDDEServer interface declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef NCL_IDDESERVER_HPP
#define NCL_IDDESERVER_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <Core/SharedPtr.hpp>

namespace DDE
{

////////////////////////////////////////////////////////////////////////////////
//! A DDE server.

class IDDEServer
{
public:
	//! Destructor.
	virtual ~IDDEServer() = 0 {}; 
};

//! The default IDDEServer smart pointer type.
typedef Core::SharedPtr<IDDEServer> IDDEServerPtr;

//namespace DDE
}

#endif // NCL_IDDESERVER_HPP
