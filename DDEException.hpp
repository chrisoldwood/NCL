/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDEEXCEPTION.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CDDEException class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef DDEEXCEPTION_HPP
#define DDEEXCEPTION_HPP

/******************************************************************************
** 
** The exception type thrown by the DDE classes.
**
*******************************************************************************
*/

class CDDEException : public CException
{
public:
	//
	// Exception codes.
	//
	enum ErrCode
	{
		E_INIT_FAILED		= 10,	// Initialisation failed.
		E_REG_FAILED		= 11,	// Server registration failed.
		E_CONN_FAILED		= 12,	// Connect failed.
		E_STRING_FAILED		= 13,	// Failed to create string handle.
		E_REQUEST_FAILED	= 14,	// Failed to request an item.
		E_LINK_FAILED		= 15,	// Failed to create advise loop.
		E_ALLOC_FAILED		= 16,	// Failed to allocate data handle.
	};

	//
	// Constructors/Destructor.
	//
	CDDEException(int eErrCode, uint nDDECode);
	~CDDEException();
	
	//
	// Members.
	//
	uint m_nDDECode;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // DDEEXCEPTION_HPP
