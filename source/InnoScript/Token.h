// Token.h: interface for the CToken class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "InnoScript.h"

class CInnoScript::CToken {
public:
	CToken(LPCTSTR pszLine, LPCTSTR pszDelim);
	virtual ~CToken();
	LPSTR GetNext();
	LPSTR GetRest();
protected:
	LPSTR m_pszLine;
	LPSTR m_pszDelim;
	LPSTR m_pszCurrPos;
};
