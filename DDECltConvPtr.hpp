////////////////////////////////////////////////////////////////////////////////
//! \file   CltConvPtr.hpp
//! \brief  The CltConvPtr class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef DDE_CLTCONVPTR_HPP
#define DDE_CLTCONVPTR_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <Core/SmartPtr.hpp>
#include "DDEClient.hpp"
#include "DDECltConv.hpp"

namespace DDE
{

////////////////////////////////////////////////////////////////////////////////
//! The smart-pointer type used to manage a DDE Client Conversation. These are
//! reference counted internally by the DDEClient.

class CltConvPtr : public Core::SmartPtr<CDDECltConv>
{
public:
	//! Default constructor.
	CltConvPtr();

	//! Attach an existing pointer.
	explicit CltConvPtr(CDDECltConv* pPointer, bool bAddRef = false);

	//! Copy constructor.
	CltConvPtr(const CltConvPtr& oPtr);

	//! Destructor.
	~CltConvPtr();
	
	//
	// Operators.
	//

	//! Assignment operator.
	CltConvPtr& operator=(const CltConvPtr& oPtr);

	//
	// Methods.
	//

	//! Release the pointer.
	void Release();
	
private:
	//
	// Members.
	//
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

inline CltConvPtr::CltConvPtr()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Attach an existing pointer. This either takes ownership of the pointer
//! or if bAddRef is true, shares ownership by requesting a copy.

inline CltConvPtr::CltConvPtr(CDDECltConv* pPointer, bool bAddRef)
{
	// Take ownership.
	if (!bAddRef)
	{
		m_pPointer = pPointer;
	}
	// Shared onership.
	else
	{
		ASSERT(pPointer != nullptr);

		m_pPointer = CDDEClient::Instance()->CreateConversation(pPointer->Service(), pPointer->Topic());
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Copy constructor. This creates a copy by opening the conversation again,
//! which only needs to increase the reference count.

inline CltConvPtr::CltConvPtr(const CltConvPtr& oPtr)
{
	if (oPtr.m_pPointer != nullptr)
		m_pPointer = CDDEClient::Instance()->CreateConversation(oPtr->Service(), oPtr->Topic());
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

inline CltConvPtr::~CltConvPtr()
{
	Release();
}

////////////////////////////////////////////////////////////////////////////////
//! Assignment operator.This creates a copy by opening the conversation again,
//! which only needs to increase the reference count.

inline CltConvPtr& CltConvPtr::operator=(const CltConvPtr& oPtr)
{
	// Check for self-assignment.
	if (this != &oPtr)
	{
		Release();

		// Create a copy of the conversation.
		if (oPtr.m_pPointer != nullptr)
			m_pPointer = CDDEClient::Instance()->CreateConversation(oPtr->Service(), oPtr->Topic());
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//! Release the pointer.

inline void CltConvPtr::Release()
{
	if (m_pPointer != nullptr)
	{
		CDDEClient::Instance()->DestroyConversation(m_pPointer);

		m_pPointer = nullptr;
	}
}

//namespace DDE
}

#endif // DDE_CLTCONVPTR_HPP
