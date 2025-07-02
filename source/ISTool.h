// ISTool.h : main header file for the ISTOOL application

#pragma once

#include "resource.h"       // main symbols
#include "Update.h"
#include "mywtl.h"
#include "Translate.h"

class CMyApp;

extern CMyApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMyApp:
// See ISTool.cpp for the implementation of this class
//

int myFind(const CString& ref, TCHAR ch, int startPos = 0);
int myReverseFind(const CString& ref, TCHAR ch);

#include "Func.h"

typedef struct {
	UINT		uSchemeID;
	LPCTSTR		pszName;
} tagSCHEMA;

typedef struct {
	UINT		uSchemeID;
	LPCTSTR		pszName;
	UINT		uStyle;
	COLORREF	cFore;
	COLORREF	cBack;
	bool		bBold;
	bool		bItalic;
	bool		bUnderline;
} tagSTYLE;

enum class SaveEncoding {
	Auto = 0,
	UTF8WithBOM,
	UTF8WithoutBOM
};

class CMyPrefs {
public:
	CMyPrefs(LPCTSTR pszSubKey);
	bool LoadPrefs();
	bool SavePrefs();

	bool				m_bReplaceCopy;
	bool				m_bAutoComponentSelect;
	bool				m_bTestCompiledSetup;
	bool				m_bFilesList;
	bool				m_bIconsList;
	bool				m_bRegistryList;
	bool				m_bIgnoreDefaults;
	bool				m_bOverwriteMessages;
	CString				m_strInnoFolder;
	CString				m_strScriptFolder;
	bool				m_bToolBar;
	bool				m_bStatusBar;
	bool				m_bSectionPanel;
	bool				m_bOpenLastProject;
	bool				m_bShowNewWizard;
	UINT				m_uStartupSection;
	CString				m_strLanguageFile;
	int					m_nSplitterPos;
	CString				m_strFontName;
	long				m_nFontHeight;
	long				m_nTabStopValue;
	bool				m_bShowVerticalTabLines;
	bool				m_bAutoIndent;
	bool				m_bShowLineNumbers;
	CString				m_strAppDir;
	bool				m_bPreProcess;
	CAtlArray<CString>	m_languageDirs;
	bool				m_bNoOutputExeFilename;
protected:
	const CString	m_strSubKey;
};

typedef struct {
	CString	m_strName, m_strDescription;
} CallTipInfo;

template<class T>
class CWTLApp {
public:
	void SetHtmlHelpFile(LPCTSTR pszHtmlHelpFile) {
		m_strHtmlHelpFile = pszHtmlHelpFile;
	}

	LPCTSTR GetHtmlHelpFile() {
		ATLASSERT(!m_strHtmlHelpFile.IsEmpty());
		return m_strHtmlHelpFile;
	}

	bool WriteProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPVOID pData, ULONG nBytes) {
		return Henden::CAppReg(T::m_pszKeyApp).SetBinaryValue(lpszSection, lpszEntry, pData, nBytes);
	}

	bool WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue) {
		return Henden::CAppReg(T::m_pszKeyApp).SetDWORDValue(lpszSection, lpszEntry, nValue);
	}

	bool WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue) {
		return Henden::CAppReg(T::m_pszKeyApp).SetStringValue(lpszSection, lpszEntry, lpszValue);
	}

	bool GetProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPVOID pData, ULONG* pnBytes) {
		return Henden::CAppReg(T::m_pszKeyApp).QueryBinaryValue(lpszSection, lpszEntry, pData, *pnBytes);
	}

	UINT GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault) {
		DWORD dwRet = nDefault;
		if (!Henden::CAppReg(T::m_pszKeyApp).QueryDWORDValue(lpszSection, lpszEntry, dwRet))
			dwRet = nDefault;
		return dwRet;
	}

	CString GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault = NULL) {
		CString str;
		if (!Henden::CAppReg(T::m_pszKeyApp).QueryStringValue(lpszSection, lpszEntry, str))
			return lpszDefault;
		return str;
	}

protected:
	CString	m_strHtmlHelpFile;
	CString	m_strCallTipsFile;
};

class CMyApp : public CWTLApp<CMyApp> {
public:
	static const LPCTSTR m_pszKeyIS;
	static const LPCTSTR m_pszKeyApp;
	static CSimpleArray<CallTipInfo*>	m_functions;
	static CSimpleArray<CallTipInfo*>	m_constants;
	static CSimpleArray<CallTipInfo*>	m_calltips;
	static int							m_nExitCode;

	CMyApp();
	BOOL InitInstance();
	void ExitInstance();

	static CImageList	m_imageList;
	static HTREEITEM FindParentItem(CTreeViewCtrl&, LPCTSTR lpszFolder, bool bSystem = false);
	static void MyExpand(CTreeViewCtrl&, HTREEITEM);
	static DWORD MyExec(LPCTSTR pszFilename, LPCTSTR pszParams, LPCTSTR pszWorkingDir = NULL, bool bWaitUntilTerminated = true, bool bRunMinimized = false, bool bWaitForIdle = false);
	static CMyPrefs	m_prefs;
	void OpenHtmlHelp(UINT nCmd, DWORD dwData);
	static bool IsBooleanExp(LPCTSTR pszArg);

protected:
	CMutex m_mutex;

public:
	CString	m_strProgramPath;
	SaveEncoding m_saveEncoding;
};

#define WM_HIDEVIEW	(WM_USER + 1)
#define WM_SHOWVIEW	(WM_USER + 2)

/////////////////////////////////////////////////////////////////////////////
