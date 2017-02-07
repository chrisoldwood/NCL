////////////////////////////////////////////////////////////////////////////////
//! \file   IDDECltConv.hpp
//! \brief  The IDDECltConv interface declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef NCL_IDDECLTCONV_HPP
#define NCL_IDDECLTCONV_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "IDDEConv.hpp"

namespace DDE
{

////////////////////////////////////////////////////////////////////////////////
//! A client DDE conversation.

class IDDECltConv : public IDDEConv
{
public:
	//! Destructor.
	virtual ~IDDECltConv() {}; 

	//
	// Properties.
	//

	//! The parent DDE client.
	virtual CDDEClient*	Client() const = 0;
};

//namespace DDE
}

#endif // NCL_IDDECLTCONV_HPP
