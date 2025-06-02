// Param.h: interface for the CParam class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "InnoScript.h"

class CInnoScript::CParam {
	friend class CLine;
public:
	CParam(LPCTSTR pszName, LPCTSTR pszValue);
	virtual ~CParam();
	void AddTail(CParam* pParam);
	CParam* GetNext();
	LPCTSTR GetName();
	LPCTSTR GetValue();
	void SetValue(LPCTSTR pszValue);
	void SetName(LPCTSTR pszName);
protected:
	LPSTR	m_pszName;
	LPSTR	m_pszValue;
	CParam* m_pNext;
};
