/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		IDDELINKDATA.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The IDDELinkData interface declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef IDDELINKDATA_HPP
#define IDDELINKDATA_HPP

/******************************************************************************
** 
** Marker interface used for custom DDE link data.
**
*******************************************************************************
*/

class IDDELinkData
{
public:
	
protected:
	// Make interface.
	virtual ~IDDELinkData() = 0 {}; 
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // IDDELINKDATA_HPP
