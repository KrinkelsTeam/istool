// WTL
#pragma once

#include "CompInt.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogImpl<CAboutDlg>, public Henden::CToolTipDialog<CAboutDlg> {
public:
	enum { IDD = IDD_ABOUTBOX };

	BEGIN_MSG_MAP(CAboutDlg)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CAboutDlg>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	CHyperLink	m_wndEmail, m_wndISHomePage, m_wndHomePage;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		_L(m_hWnd, _T("About"));
		CenterWindow(GetParent());

		m_wndEmail.SubclassWindow(GetDlgItem(IDC_EMAIL));
		m_wndISHomePage.SubclassWindow(GetDlgItem(IDC_ISHOMEPAGE));
		m_wndHomePage.SubclassWindow(GetDlgItem(IDC_HOMEPAGE));

		SetDlgItemText(IDC_VERSION, m_strVersion);
		SetDlgItemText(IDC_ISVERSION, m_strISVersion);
		SetDlgItemText(IDC_ISVERSION_INSTALLED, m_strInstalledIS);
		SetDlgItemText(IDC_TRANSVER, m_strTransVer);

		m_wndEmail.SetHyperLink(MY_EMAIL);

		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}

	CString	m_strVersion, m_strISVersion, m_strInstalledIS, m_strTransVer;

	CAboutDlg() {
#ifdef _DEBUG
#ifdef VER_BUILD
		m_strVersion.Format(_T("%d.%d.%d.%d DEBUG"), VER_MAJOR, VER_MINOR, VER_REVISION, VER_BUILD);
#else
		m_strVersion.Format(_T("%d.%d.%d DEBUG"), VER_MAJOR, VER_MINOR, VER_REVISION);
#endif
#else
#ifdef VER_BUILD
		m_strVersion.Format(_T("%d.%d.%d.%d"), VER_MAJOR, VER_MINOR, VER_REVISION, VER_BUILD);
#else
		m_strVersion.Format(_T("%d.%d.%d"), VER_MAJOR, VER_MINOR, VER_REVISION);
#endif
#endif
		m_strISVersion.Format(_T("%s"), ISVERSION);

		m_strTransVer = _L("TransVersion", m_strVersion);

		GetCompilerVersion(CMyApp::m_prefs.m_strInnoFolder + _T("\\ISCmplr.dll"), m_strInstalledIS);
	}

	bool GetCompilerVersion(LPCTSTR pszFileName, CString& strVersion) {
		HINSTANCE hCompiler = LoadLibrary(pszFileName);
		if (!hCompiler) {
			strVersion = _T("Not available");
			return false;
		}

		ISDllGetVersionProc GetISVersion = (ISDllGetVersionProc)GetProcAddress(hCompiler, "ISDllGetVersion");
		if (!GetISVersion) {
			strVersion = _L(_T("Failed to query version"));
			FreeLibrary(hCompiler);
			return true;
		}

		TCompilerVersionInfo* pInfo = GetISVersion();
		if (!pInfo) {
			strVersion = _L(_T("Error|GetISVersion"), _T("Failed to get version information"));
			FreeLibrary(hCompiler);
			return true;
		}

		if (pInfo->BinVersion & 0xFF)
			strVersion.Format(_T("%d.%d.%d.%d"),
				(pInfo->BinVersion & 0xFF000000) >> 24,
				(pInfo->BinVersion & 0x00FF0000) >> 16,
				(pInfo->BinVersion & 0x0000FF00) >> 8,
				pInfo->BinVersion & 0x000000FF
			);
		else
			strVersion.Format(_T("%d.%d.%d"),
				(pInfo->BinVersion & 0xFF000000) >> 24,
				(pInfo->BinVersion & 0x00FF0000) >> 16,
				(pInfo->BinVersion & 0x0000FF00) >> 8
			);

		FreeLibrary(hCompiler);
		return true;
	}

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_HOMEPAGE, _L(_T("Help|About|HomePage"), _T("Click to open the ISTool home page")))
		TOOLTIP_HANDLER(IDC_EMAIL, _L(_T("Help|About|Author"), _T("Click here to email the author of ISTool")))
		TOOLTIP_HANDLER(IDC_ISHOMEPAGE, _L(_T("Help|About|ISHomePage"), _T("The Inno Setup home page")))
		TOOLTIP_HANDLER(IDOK, _L(_T("Help|About|OK"), _T("Closes the window")))
	END_TOOLTIP_MAP()
};

