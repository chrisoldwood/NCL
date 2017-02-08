////////////////////////////////////////////////////////////////////////////////
//! \file   IDDEConv.hpp
//! \brief  The IDDEConv interface declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef NCL_IDDECONV_HPP
#define NCL_IDDECONV_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "DDEFwd.hpp"

namespace DDE
{

////////////////////////////////////////////////////////////////////////////////
//! A DDE conversation.

class XDDEConv
{
public:
	//! Destructor.
	virtual ~XDDEConv() {}; 

	//
	// Properties.
	//

	//! The name of the DDE service.
	virtual const CString& Service() const = 0;

	//! The name of the DDE topic.
	virtual const CString& Topic() const = 0;
};

//namespace DDE
}

#endif // NCL_IDDECONV_HPP
