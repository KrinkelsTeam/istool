// Lines.h: interface for the CLine class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "InnoScript.h"

class CInnoScript::CLine {
	friend class CInnoScript;
public:
	CLine(SECTION sec);
	virtual ~CLine();
	virtual CLine* Copy();

	virtual void	Write(LPTSTR pszOut, UINT nLength);
	SECTION			GetSection();
	virtual LPCTSTR	GetComment();
	virtual LPCTSTR	GetParameter(LPCTSTR pszName);
	void			AddParameter(LPCTSTR pszName, LPCTSTR pszValue);
	void			SetParameter(LPCTSTR pszName, LPCTSTR pszValue);
	void			DeleteParameter(LPCTSTR pszName);
	bool			GetParameterFlag(LPCTSTR pszName, LPCTSTR pszFlag);
	void			SetParameterFlag(LPCTSTR pszName, LPCTSTR pszFlag, bool bSet);
	virtual bool	IsHash();

	// For use in [Setup] and [Messages]
	virtual LPCTSTR	GetKey();
	virtual LPCTSTR	GetValue();

	enum { FLG_CONCAT = 0x400 };

	DWORD			m_dwUserFlags;

	void SetIndent(const CString& strIndent);
	LPCTSTR GetIndent() const;

protected:
	LPTSTR			UnQuote(LPTSTR pszLine);

	const SECTION	m_sec;
	CParam* m_pParams;
	CString			m_strIndent;
};

class CInnoScript::CLineComment : public CLine {
public:
	CLineComment(SECTION sec, LPCTSTR pszLine);
	virtual ~CLineComment();
	virtual CLine* Copy();

	virtual void	Write(LPTSTR pszOut, UINT nLength);
	virtual LPCTSTR	GetComment();
protected:
	LPTSTR			m_pszLine;
};

class CInnoScript::CLineHash : public CLine {
public:
	CLineHash(SECTION sec, LPCTSTR pszLine);
	virtual ~CLineHash();
	virtual CLine* Copy();

	virtual void	Write(LPTSTR pszOut, UINT nLength);
	virtual LPCTSTR	GetComment();
	virtual bool	IsHash();
protected:
	LPTSTR			m_pszLine;
};

class CInnoScript::CLineSetup : public CLine {
public:
	CLineSetup(SECTION sec, LPCTSTR pszLine);
	CLineSetup(SECTION sec);
	virtual CLine* Copy();
	virtual ~CLineSetup();
	virtual void Write(LPTSTR pszOut, UINT nLength);
	virtual LPCTSTR	GetKey();
	virtual LPCTSTR	GetValue();
	virtual void SetKey(LPCTSTR pszKey);
	virtual void SetValue(LPCTSTR pszValue);
};

class CInnoScript::CLineParam : public CLine {
public:
	CLineParam(SECTION sec, LPCTSTR pszLine);
	virtual ~CLineParam();
	virtual CLine* Copy();
};
