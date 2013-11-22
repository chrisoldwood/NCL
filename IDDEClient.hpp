////////////////////////////////////////////////////////////////////////////////
//! \file   IDDEClient.hpp
//! \brief  The IDDEClient interface declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef NCL_IDDECLIENT_HPP
#define NCL_IDDECLIENT_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <Core/SharedPtr.hpp>

namespace DDE
{

////////////////////////////////////////////////////////////////////////////////
//! A DDE client.

class IDDEClient
{
public:
	//! Destructor.
	virtual ~IDDEClient() {};
};

//! The default IDDEClient smart pointer type.
typedef Core::SharedPtr<IDDEClient> IDDEClientPtr;

//namespace DDE
}

#endif // NCL_IDDECLIENT_HPP
