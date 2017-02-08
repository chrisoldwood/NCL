////////////////////////////////////////////////////////////////////////////////
//! \file   DDEFwd.hpp
//! \brief  Forward declarations for the DDE library types.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef NCL_DDEFWD_HPP
#define NCL_DDEFWD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

class IDDEClientListener;
class IDDEConvData;
class IDDELinkData;
class IDDEServerListener;

class CDDEClient;
class CDDECltConv;
class CDDEConv;
class CDDEData;
class CDDEInst;
class CDDELink;
class CDDESvrConv;

//#define USE_DDE_INTERFACES
#ifdef USE_DDE_INTERFACES

namespace DDE
{
class XDDEConv;
class XDDECltConv;
class XDDEClient;

typedef XDDEConv IDDEConv;
typedef XDDECltConv IDDECltConv;
typedef XDDEClient IDDEClient;
}

#else

namespace DDE
{
typedef CDDEConv IDDEConv;
typedef CDDECltConv IDDECltConv;
typedef CDDEClient IDDEClient;
}

#endif

#endif // NCL_DDEFWD_HPP
