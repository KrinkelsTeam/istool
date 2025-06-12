// WTL
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDlgIEShortcut dialog

class CDlgIEShortcut :
	public CDialogImpl<CDlgIEShortcut>,
	public CWinDataExchange<CDlgIEShortcut>,
	public CMyDialogBase<CDlgIEShortcut>
{
public:
	enum { IDD = IDD_IESHORTCUT };

	BEGIN_MSG_MAP(CDlgIEShortcut)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_HANDLER(IDC_IESHORTCUT_FILENAME, EN_KILLFOCUS, OnKillFocus)
		CHAIN_MSG_MAP(CMyDialogBase<CDlgIEShortcut>)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CDlgIEShortcut)
		DDX_TEXT(IDC_IESHORTCUT_FILENAME, m_strFileName)
		DDX_TEXT(IDC_IESHORTCUT_URL, m_strURL)
	END_DDX_MAP()

	CString	m_strFileName;
	CString	m_strURL;

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		DoDataExchange(DDX_SAVE);
		EndDialog(wID);
		return 0;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd, _T("IEShortCut"));
		CenterWindow(GetParent());

		m_strFileName = _T("{app}\\ISTool.url");
		m_strURL = _T("https://istool.krinkels.org/");
		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnKillFocus(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		DoDataExchange(DDX_SAVE, IDC_IESHORTCUT_FILENAME);
		if (m_strFileName.GetLength() < 4 || m_strFileName.Right(4).CompareNoCase(_T(".url"))) {
			m_strFileName += _T(".url");
			DoDataExchange(DDX_LOAD, IDC_IESHORTCUT_FILENAME);
		}
		return 0;
	}
};
