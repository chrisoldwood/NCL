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
#include "IDDECltConv.hpp"

namespace DDE
{

////////////////////////////////////////////////////////////////////////////////
//! A DDE client.

class IDDEClient
{
public:
	//! Destructor.
	virtual ~IDDEClient() {};

	//! Create a conversation for the service and topic.
	virtual CDDECltConv* CreateConversation(const tchar* service, const tchar* topic) = 0;

	//! Query for all running servers and topics.
	virtual void QueryAll(CStrArray& astrServers, CStrArray& astrTopics) const = 0;
};

//! The default IDDEClient smart pointer type.
typedef Core::SharedPtr<IDDEClient> IDDEClientPtr;

//namespace DDE
}

#endif // NCL_IDDECLIENT_HPP
