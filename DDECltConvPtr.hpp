////////////////////////////////////////////////////////////////////////////////
//! \file   DDECltConvPtr.hpp
//! \brief  The CltConvPtr class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef DDE_CLTCONVPTR_HPP
#define DDE_CLTCONVPTR_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <Core/SmartPtr.hpp>
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

//namespace DDE
}

#endif // DDE_CLTCONVPTR_HPP
