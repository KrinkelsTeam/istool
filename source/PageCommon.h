// WTL
#pragma once

#include "ButtonVer.h"
#include "MyDoc.h"
#include "StringToken.h"

/////////////////////////////////////////////////////////////////////////////
// CPageCommon dialog

class CPageCommon :
	public CPropertyPageImpl<CPageCommon>,
	public CMyPropertyPageBase<CPageCommon>,
	public CWinDataExchange<CPageCommon>,
	public CDialogResize<CPageCommon>,
	public Henden::CToolTipDialog<CPageCommon>
{
public:
	enum { IDD = IDD_COMMON_NEW };
	enum {
		NONE             = 0x00,
		HASCHECK         = 0x01,
		HASCOMPONENTS    = 0x02,
		HASTASKS         = 0x04,
		HASLANGUAGES     = 0x08,
		HASBEFOREINSTALL = 0x10,
		HASAFTERINSTALL  = 0x20
	};

	BEGIN_DLGRESIZE_MAP(CPageCommon)
		DLGRESIZE_CONTROL(IDC_MINVERSION, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_MINIMUM_VERSION_9X, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_BTN_MINVERSIONWIN, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_MINIMUM_VERSION_NT, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_BTN_MINVERSIONNT, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_ONLYBELOWVERSION, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_ONLY_BELOW_VERSION_9X, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_BTN_BELOWVERSIONWIN, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_ONLY_BELOW_VERSION_NT, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_BTN_BELOWVERSIONNT, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_SCRIPTING, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_SCRIPTING_CHECK, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_BEFOREINSTALL, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_AFTERINSTALL, DLSZ_SIZE_X)
	END_DLGRESIZE_MAP()

	BEGIN_MSG_MAP(CPageCommon)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CPageCommon>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPageCommon>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageCommon>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_MINIMUM_VERSION_9X, CBN_SELCHANGE, OnModified)
		//ON_WM_HELPINFO()
		COMMAND_HANDLER(IDC_MINIMUM_VERSION_NT, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_ONLY_BELOW_VERSION_9X, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_ONLY_BELOW_VERSION_NT, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_SCRIPTING_CHECK, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_BEFOREINSTALL, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_AFTERINSTALL, EN_CHANGE, OnModified)
		REFLECT_NOTIFICATIONS()
		CHAIN_MSG_MAP(CDialogResize<CPageCommon>)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageCommon)
		DDX_TEXT(IDC_MINIMUM_VERSION_9X, m_strMinimumVersion9X)
		DDX_TEXT(IDC_MINIMUM_VERSION_NT, m_strMinimumVersionNT)
		DDX_TEXT(IDC_ONLY_BELOW_VERSION_9X, m_strOnlyBelowVersion9X)
		DDX_TEXT(IDC_ONLY_BELOW_VERSION_NT, m_strOnlyBelowVersionNT)
		DDX_TEXT(IDC_SCRIPTING_CHECK, m_strCheck)
		DDX_TEXT(IDC_BEFOREINSTALL, m_strBeforeInstall)
		DDX_TEXT(IDC_AFTERINSTALL, m_strAfterInstall)
	END_DDX_MAP()

	CComboBox	m_wndCheck;
	CButtonVer	m_wndBelowVersionNT;
	CButtonVer	m_wndBelowVersionWin;
	CButtonVer	m_wndMinVersionNT;
	CButtonVer	m_wndMinVersionWin;
	CString	m_strMinimumVersion9X;
	CString	m_strMinimumVersionNT;
	CString	m_strOnlyBelowVersion9X;
	CString	m_strOnlyBelowVersionNT;
	CString	m_strCheck, m_strBeforeInstall, m_strAfterInstall;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		DlgResize_Init(false, false, 0);
		_L(m_hWnd, "Common");
		m_wndCheck.Attach(GetDlgItem(IDC_SCRIPTING_CHECK));
		m_wndBelowVersionNT.SubclassWindow(GetDlgItem(IDC_BTN_BELOWVERSIONNT));
		m_wndBelowVersionWin.SubclassWindow(GetDlgItem(IDC_BTN_BELOWVERSIONWIN));
		m_wndMinVersionNT.SubclassWindow(GetDlgItem(IDC_BTN_MINVERSIONNT));
		m_wndMinVersionWin.SubclassWindow(GetDlgItem(IDC_BTN_MINVERSIONWIN));

		ParseCode();

		for (int nPos = 0; nPos < m_listBase.GetSize(); nPos++) {
			CInnoScript::CLine* pBase = m_listBase[nPos];

			GetDlgItem(IDC_BEFOREINSTALL_T).EnableWindow(m_dwFlags & HASBEFOREINSTALL);
			GetDlgItem(IDC_BEFOREINSTALL).EnableWindow(m_dwFlags & HASBEFOREINSTALL);
			GetDlgItem(IDC_AFTERINSTALL_T).EnableWindow(m_dwFlags & HASAFTERINSTALL);
			GetDlgItem(IDC_AFTERINSTALL).EnableWindow(m_dwFlags & HASAFTERINSTALL);

			LPCTSTR pszMinVersion = pBase->GetParameter(_T("MinVersion"));
			if (!pszMinVersion) {
				m_strMinimumVersion9X.Empty();
				m_strMinimumVersionNT.Empty();
			} else {
				CStringToken tok(pszMinVersion, _T(","));
				CString tmp1 = tok.GetNext();
				CString tmp2 = tok.GetNext();
				if (m_strMinimumVersion9X.CompareNoCase(tmp1) || m_strMinimumVersionNT.CompareNoCase(tmp2)) {
					m_strMinimumVersion9X.Empty();
					m_strMinimumVersionNT.Empty();
				} else {
					m_strMinimumVersion9X = tmp1;
					m_strMinimumVersionNT = tmp2;
				}
			}

			LPCTSTR pszOnlyBelowVersion = pBase->GetParameter(_T("OnlyBelowVersion"));
			if (!pszOnlyBelowVersion) {
				m_strOnlyBelowVersion9X.Empty();
				m_strOnlyBelowVersionNT.Empty();
			} else {
				CStringToken tok(pszOnlyBelowVersion, _T(","));
				CString tmp1 = tok.GetNext();
				CString tmp2 = tok.GetNext();
				if (m_strOnlyBelowVersion9X.CompareNoCase(tmp1) || m_strOnlyBelowVersionNT.CompareNoCase(tmp2)) {
					m_strOnlyBelowVersion9X.Empty();
					m_strOnlyBelowVersionNT.Empty();
				} else {
					m_strOnlyBelowVersion9X = tmp1;
					m_strOnlyBelowVersionNT = tmp2;
				}
			}

			if (m_strCheck.CompareNoCase(SAFESTR(pBase->GetParameter(_T("Check")))))
				m_strCheck.Empty();
			if (m_strBeforeInstall.CompareNoCase(SAFESTR(pBase->GetParameter(_T("BeforeInstall")))))
				m_strBeforeInstall.Empty();
			if (m_strAfterInstall.CompareNoCase(SAFESTR(pBase->GetParameter(_T("AfterInstall")))))
				m_strAfterInstall.Empty();
		}
		DoDataExchange(DDX_LOAD);

		m_wndCheck.EnableWindow(m_dwFlags & HASCHECK);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		const bool bForce = m_listBase.GetSize() == 1;

		CInnoScriptEx& script = m_pDoc->GetScript();
		for (int nPos = 0; nPos < m_listBase.GetSize(); nPos++) {
			CInnoScript::CLine* pBase = m_listBase[nPos];
			if (m_strMinimumVersion9X.IsEmpty() && !m_strMinimumVersionNT.IsEmpty())
				m_strMinimumVersion9X = _T("0");
			else if (!m_strMinimumVersion9X.IsEmpty() && m_strMinimumVersionNT.IsEmpty())
				m_strMinimumVersionNT = _T("0");
			if (m_strOnlyBelowVersion9X.IsEmpty() && !m_strOnlyBelowVersionNT.IsEmpty())
				m_strOnlyBelowVersion9X = _T("0");
			else if (!m_strOnlyBelowVersion9X.IsEmpty() && m_strOnlyBelowVersionNT.IsEmpty())
				m_strOnlyBelowVersionNT = _T("0");

			if (!m_strMinimumVersion9X.IsEmpty() || !m_strMinimumVersionNT.IsEmpty()) {
				CString strMinVersion;
				strMinVersion.Format(_T("%s,%s"), m_strMinimumVersion9X, m_strMinimumVersionNT);
				pBase->SetParameter(_T("MinVersion"), strMinVersion);
			} else if (bForce)
				pBase->DeleteParameter(_T("MinVersion"));

			if (!m_strOnlyBelowVersion9X.IsEmpty() || !m_strOnlyBelowVersionNT.IsEmpty()) {
				CString strOnlyBelowVersion;
				strOnlyBelowVersion.Format(_T("%s,%s"), m_strOnlyBelowVersion9X, m_strOnlyBelowVersionNT);
				pBase->SetParameter(_T("OnlyBelowVersion"), strOnlyBelowVersion);
			} else if (bForce)
				pBase->DeleteParameter(_T("OnlyBelowVersion"));

			CInnoScriptEx::SetString(pBase, bForce, _T("Check"), m_strCheck);
			CInnoScriptEx::SetString(pBase, bForce, _T("BeforeInstall"), m_strBeforeInstall);
			CInnoScriptEx::SetString(pBase, bForce, _T("AfterInstall"), m_strAfterInstall);
		}
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if (!m_bNew) SetModified();
		return 0;
	}

	static void SplitVersion(LPCTSTR lpszVersion, int& v1, int& v2) {
		CStringToken tok(lpszVersion, _T(","));
		CString str9X(tok.GetNext());
		CString strNT(tok.GetNext());
		float f1 = 0, f2 = 0;
		sscanf_s(str9X, _T("%f"), &f1);
		sscanf_s(strNT, _T("%f"), &f2);
		if (f1 < 3.09) v1 = 1;
		else if (f1 < 3.99) v1 = 2;
		else if (f1 < 4.09) v1 = 3;
		else v1 = 4;

		if (f2 < 3.505) v2 = 1;
		else if (f2 < 3.99) v2 = 2;
		else if (f2 < 4.99) v2 = 3;
		else v2 = 4;
	}

	void ParseCode() {
		CScriptList list;
		m_pDoc->GetScript().GetList(CInnoScript::SEC_CODE, list);

		for (int nPos = 0; nPos < list.GetSize(); nPos++) {
			CScriptLine* pLine = list[nPos];
			CString strLine;
			pLine->Write(strLine.GetBuffer(1024), 1024);
			LPCTSTR pszLine = strLine;


			char szName[256] = _T("");

			while (iswspace(*pszLine)) pszLine++;
			if (!_tcsnicmp(pszLine, _T("function"), 8)) {
				pszLine += 8;
				while (*pszLine && iswspace(*pszLine)) pszLine++;

				bool bInParm = false;
				while (*pszLine && (*pszLine != ':' || bInParm)) {
					if (!bInParm && *pszLine == '(') bInParm = true;
					else if (!bInParm && !iswspace(*pszLine)) _tcsncat_s(szName, sizeof(szName) / sizeof(TCHAR), pszLine, 1);
					else if (bInParm && *pszLine == ')') bInParm = false;
					pszLine++;
				}

				while (*pszLine && (iswspace(*pszLine) || *pszLine == ':')) pszLine++;

				if (!_tcsnicmp(pszLine, _T("Boolean"), 7)) m_wndCheck.AddString(szName);
			}
		}
	}

	CString m_strTitle;
	CPageCommon(DWORD dwFlags, CScriptList& listBase, CMyDoc* pDoc, const bool bNew) :
		m_listBase(listBase),
		m_pDoc(pDoc),
		m_bNew(bNew),
		m_wndMinVersionWin(VER_WIN),
		m_wndMinVersionNT(VER_NT),
		m_wndBelowVersionWin(VER_WIN),
		m_wndBelowVersionNT(VER_NT),
		m_dwFlags(dwFlags)
	{
		m_strTitle = _L("DialogTitles|Common", "Common");
		SetTitle((LPCTSTR)m_strTitle);

		for (int nPos = 0; nPos < m_listBase.GetSize(); nPos++) {
			CInnoScript::CLine* pBase = m_listBase[nPos];

			LPCTSTR pszMinVersion = pBase->GetParameter(_T("MinVersion"));
			if (!pszMinVersion) {
				m_strMinimumVersion9X.Empty();
				m_strMinimumVersionNT.Empty();
			} else {
				CStringToken tok(pszMinVersion, _T(","));
				m_strMinimumVersion9X = tok.GetNext();
				m_strMinimumVersionNT = tok.GetNext();
			}

			LPCTSTR pszOnlyBelowVersion = pBase->GetParameter(_T("OnlyBelowVersion"));
			if (!pszOnlyBelowVersion) {
				m_strOnlyBelowVersion9X.Empty();
				m_strOnlyBelowVersionNT.Empty();
			} else {
				CStringToken tok(pszOnlyBelowVersion, _T(","));
				m_strOnlyBelowVersion9X = tok.GetNext();
				m_strOnlyBelowVersionNT = tok.GetNext();
			}

			m_strCheck = pBase->GetParameter(_T("Check"));
			m_strBeforeInstall = pBase->GetParameter(_T("BeforeInstall"));
			m_strAfterInstall = pBase->GetParameter(_T("AfterInstall"));
			break;
		}
	}

	CScriptList&  m_listBase;
	CMyDoc*       m_pDoc;
	const bool    m_bNew;
	DWORD         m_dwFlags;

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_MINIMUM_VERSION_9X, _L("Help|Common|MinVerWin", "A minimum Windows version for the entry to be processed."))
		TOOLTIP_HANDLER(IDC_BTN_MINVERSIONWIN, _L("Help|Common|MinVerWinButton", "Click this button to select Windows version."))
		TOOLTIP_HANDLER(IDC_MINIMUM_VERSION_NT, _L("Help|Common|MinVerNT", "A minimum Windows NT version for the entry to be processed."))
		TOOLTIP_HANDLER(IDC_BTN_MINVERSIONNT, _L("Help|Common|MinVerNTButton", "Click this button to select Windows NT version."))
		TOOLTIP_HANDLER(IDC_ONLY_BELOW_VERSION_9X, _L("Help|Common|BelowVerWin", "Basically the opposite of MinVersion. Specifies the minimum Windows version for the entry not to be processed."))
		TOOLTIP_HANDLER(IDC_BTN_BELOWVERSIONWIN, _L("Help|Common|BelowVerWinButton", "Click this button to select Windows version."))
		TOOLTIP_HANDLER(IDC_ONLY_BELOW_VERSION_NT, _L("Help|Common|BelowVerNT", "Basically the opposite of MinVersion. Specifies the minimum Windows NT version for the entry not to be processed."))
		TOOLTIP_HANDLER(IDC_BTN_BELOWVERSIONNT, _L("Help|Common|BelowVerNTButton", "Click this button to select Windows NT version."))
		TOOLTIP_HANDLER(IDC_SCRIPTING_CHECK, _L("Help|Common|Check", "The name of the check function in the [Code] section that determines whether an entry has to be processed or not. May include one parameter that Setup should pass to the check function. This parameter may include constants."))
		TOOLTIP_HANDLER(IDC_BEFOREINSTALL, _L("Help|Common|BeforeInstall", "The name of the function in the [Code] section that is to be called once just before an entry is installed. May include one parameter that Setup should pass to the function. This parameter may include constants."))
		TOOLTIP_HANDLER(IDC_AFTERINSTALL, _L("Help|Common|AfterInstall", "The name of the function in the [Code] section that is to be called once just after an entry is installed. May include one parameter that Setup should pass to the function. This parameter may include constants."))
	END_TOOLTIP_MAP()
};
