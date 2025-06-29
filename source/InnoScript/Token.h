// Token.h: interface for the CToken class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "InnoScript.h"

class CInnoScript::CToken {
public:
	CToken(LPCTSTR pszLine, LPCTSTR pszDelim);
	virtual ~CToken();
	LPTSTR GetNext();
	LPTSTR GetRest();
protected:
	LPTSTR m_pszLine;
	LPTSTR m_pszDelim;
	LPTSTR m_pszCurrPos;
};
