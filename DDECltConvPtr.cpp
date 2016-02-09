////////////////////////////////////////////////////////////////////////////////
//! \file   DDECltConvPtr.cpp
//! \brief  The CltConvPtr class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "DDECltConvPtr.hpp"
#include "DDEClient.hpp"

namespace DDE
{

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

CltConvPtr::CltConvPtr()
	: Core::SmartPtr<CDDECltConv>()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Attach an existing pointer. This either takes ownership of the pointer
//! or if bAddRef is true, shares ownership by requesting a copy.

CltConvPtr::CltConvPtr(CDDECltConv* pPointer, bool bAddRef)
	: Core::SmartPtr<CDDECltConv>()
{
	// Take ownership.
	if (!bAddRef)
	{
		m_ptr = pPointer;
	}
	// Shared ownership.
	else
	{
		ASSERT(pPointer != nullptr);

		m_ptr = pPointer->Client()->CreateConversation(pPointer->Service(), pPointer->Topic());
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Copy constructor. This creates a copy by opening the conversation again,
//! which only needs to increase the reference count.

CltConvPtr::CltConvPtr(const CltConvPtr& oPtr)
	: Core::SmartPtr<CDDECltConv>()
{
	if (oPtr.m_ptr != nullptr)
		m_ptr = oPtr->Client()->CreateConversation(oPtr->Service(), oPtr->Topic());
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

CltConvPtr::~CltConvPtr()
{
	Release();
}

////////////////////////////////////////////////////////////////////////////////
//! Assignment operator.This creates a copy by opening the conversation again,
//! which only needs to increase the reference count.

CltConvPtr& CltConvPtr::operator=(const CltConvPtr& oPtr)
{
	// Check for self-assignment.
	if (this != &oPtr)
	{
		Release();

		// Create a copy of the conversation.
		if (oPtr.m_ptr != nullptr)
			m_ptr = oPtr->Client()->CreateConversation(oPtr->Service(), oPtr->Topic());
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//! Release the pointer.

void CltConvPtr::Release()
{
	if (m_ptr != nullptr)
	{
		m_ptr->Client()->DestroyConversation(m_ptr);
		m_ptr = nullptr;
	}
}

//namespace DDE
}
