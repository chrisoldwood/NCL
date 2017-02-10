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

class XDDECltConv : virtual public XDDEConv
{
public:
	//! Destructor.
	virtual ~XDDECltConv() {}; 

	//
	// Properties.
	//

	//! The parent DDE client.
	virtual IDDEClient*	Client() const = 0;

	//! Set the timeout for a DDE transaction.
	virtual void SetTimeout(DWORD timeout) = 0;

	//
	// Methods.
	//

	//! Request a string based value.
	virtual CString RequestString(const tchar* pszItem, uint nFormat) const = 0;

	//! Request a value in a custom format.
	virtual CDDEData Request(const tchar* pszItem, uint nFormat) const = 0;

	//! Execute a string based command on the server.
	virtual void ExecuteString(const tchar* pszCommand) const = 0;

	//! Execute a command on the server.
	virtual void Execute(const void* pValue, size_t nSize) const = 0;

	//! Poke a sring based value.
	virtual void PokeString(const tchar* pszItem, const tchar* pszValue, uint nFormat) const = 0;

	//! Poke a sring based value.
	virtual void Poke(const tchar* pszItem, uint nFormat, const void* pValue, size_t nSize) const = 0;

	//! Start an advise loop on the server for an item.
	virtual CDDELink* CreateLink(const tchar* pszItem, uint nFormat = CF_TEXT) = 0;
};

//namespace DDE
}

#endif // NCL_IDDECLTCONV_HPP
