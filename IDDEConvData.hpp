/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		IDDECONVDATA.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The IDDEConvData interface declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef IDDECONVDATA_HPP
#define IDDECONVDATA_HPP

#if _MSC_VER > 1000
#pragma once
#endif

/******************************************************************************
** 
** Marker interface used for custom DDE conversation data.
**
*******************************************************************************
*/

class IDDEConvData
{
public:
	
protected:
	// Make interface.
	virtual ~IDDEConvData() = 0 {}; 
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // IDDECONVDATA_HPP
