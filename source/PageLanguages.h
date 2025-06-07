#pragma once

#include "MyDoc.h"
#include "FilesHelper.h"
#include "StringToken.h"
#include "DlgSelectLanguageFiles.h"

/////////////////////////////////////////////////////////////////////////////
// CPageLanguages dialog

class CPageLanguages :
	public CPropertyPageImpl<CPageLanguages>,
	public CMyPropertyPageBase<CPageLanguages>,
	public CWinDataExchange<CPageLanguages>,
	public Henden::CToolTipDialog<CPageLanguages>
{
public:
	enum { IDD = IDD_LANGUAGES };

	BEGIN_MSG_MAP(CPageLanguages)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CPageLanguages>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPageLanguages>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageLanguages>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_LANGUAGES_NAME, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_LANGUAGES_MESSAGESFILE, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_LANGUAGES_LICENSEFILE, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_LANGUAGES_INFOBEFOREFILE, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_LANGUAGES_INFOAFTERFILE, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_LANGUAGES_MESSAGESFILE2, BN_CLICKED, OnSelectLanguageFiles)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageLanguages)
		DDX_CONTROL(IDC_LANGUAGES_LICENSEFILE2, m_btnLicenseFile)
		DDX_CONTROL(IDC_LANGUAGES_INFOBEFOREFILE2, m_btnInfoBeforeFile)
		DDX_CONTROL(IDC_LANGUAGES_INFOAFTERFILE2, m_btnInfoAfterFile)
		DDX_TEXT(IDC_LANGUAGES_NAME, m_strName)
		DDX_TEXT(IDC_LANGUAGES_MESSAGESFILE, m_strMessagesFile)
		DDX_TEXT(IDC_LANGUAGES_LICENSEFILE, m_strLicenseFile)
		DDX_TEXT(IDC_LANGUAGES_INFOBEFOREFILE, m_strInfoBeforeFile)
		DDX_TEXT(IDC_LANGUAGES_INFOAFTERFILE, m_strInfoAfterFile)
	END_DDX_MAP()

	CString		m_strName, m_strMessagesFile, m_strLicenseFile, m_strInfoBeforeFile, m_strInfoAfterFile;
	Henden::CButtonFile	m_btnLicenseFile, m_btnInfoBeforeFile, m_btnInfoAfterFile;
	CButton		m_btnMessagesFile;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		m_btnMessagesFile.Attach(GetDlgItem(IDC_LANGUAGES_MESSAGESFILE2));

		_L(m_hWnd, "Languages");
		for (int nPos = 0; nPos < m_list.GetSize(); nPos++) {
			CScriptLine* pLine = m_list[nPos];

			if (m_strName.CompareNoCase(SAFESTR(pLine->GetParameter(_T("Name"))))) m_strName.Empty();
			if (m_strMessagesFile.CompareNoCase(SAFESTR(pLine->GetParameter(_T("MessagesFile"))))) m_strMessagesFile.Empty();
			if (m_strLicenseFile.CompareNoCase(SAFESTR(pLine->GetParameter(_T("LicenseFile"))))) m_strLicenseFile.Empty();
			if (m_strInfoBeforeFile.CompareNoCase(SAFESTR(pLine->GetParameter(_T("InfoBeforeFile"))))) m_strInfoBeforeFile.Empty();
			if (m_strInfoAfterFile.CompareNoCase(SAFESTR(pLine->GetParameter(_T("InfoAfterFile"))))) m_strInfoAfterFile.Empty();
		}
		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		const bool bForce = m_list.GetSize() == 1;
		if (bForce && m_strName.IsEmpty()) {
			AtlMessageBox(m_hWnd, _L("You must enter a name."), IDR_MAINFRAME, MB_OK | MB_ICONERROR);
			return PSNRET_INVALID;
		}
		if (bForce && m_strMessagesFile.IsEmpty()) {
			AtlMessageBox(m_hWnd, _L("You must enter a messages file."), IDR_MAINFRAME, MB_OK | MB_ICONERROR);
			return PSNRET_INVALID;
		}

		for (int nPos = 0; nPos < m_list.GetSize(); nPos++) {
			CScriptLine* pLine = m_list[nPos];

			CInnoScriptEx::SetString(pLine, bForce, _T("Name"), m_strName);
			CInnoScriptEx::SetString(pLine, bForce, _T("MessagesFile"), m_strMessagesFile);
			CInnoScriptEx::SetString(pLine, bForce, _T("LicenseFile"), m_strLicenseFile);
			CInnoScriptEx::SetString(pLine, bForce, _T("InfoBeforeFile"), m_strInfoBeforeFile);
			CInnoScriptEx::SetString(pLine, bForce, _T("InfoAfterFile"), m_strInfoAfterFile);
		}

		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if (!m_bNew)
			SetModified();
		return 0;
	}

	LRESULT OnSelectLanguageFiles(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		DoDataExchange(DDX_SAVE, IDC_LANGUAGES_MESSAGESFILE);
		CDlgSelectLanguageFiles dlg(m_strMessagesFile);
		if (dlg.DoModal(m_hWnd) == IDOK) {
			m_strMessagesFile = dlg.m_strResult;
			DoDataExchange(DDX_LOAD, IDC_LANGUAGES_MESSAGESFILE);
			if (!m_bNew) SetModified();
		}
		return 0;
	}

	CString m_strTitle;
	CPageLanguages(CScriptList& list, CMyDoc* pDoc, const bool bNew) :
		m_pDoc(pDoc), m_bNew(bNew), m_list(list),
		//		m_btnMessagesFile(true,"Message Files (*.isl)|*.isl|All Files (*.*)|*.*||"),
		m_btnLicenseFile(true, _T("License Files (.txt;.rtf)|*.txt;*.rtf|All Files (*.*)|*.*||")),
		m_btnInfoBeforeFile(true, _T("Info Files (.txt;.rtf)|*.txt;*.rtf|All Files (*.*)|*.*||")),
		m_btnInfoAfterFile(true, _T("Info Files (.txt;.rtf)|*.txt;*.rtf|All Files (*.*)|*.*||"))
	{
		m_strTitle = _L("DialogTitles|Language", "Language");
		SetTitle((LPCTSTR)m_strTitle);

		for (int nPos = 0; nPos < m_list.GetSize(); nPos++) {
			CScriptLine* pLine = m_list[nPos];

			m_strName = pLine->GetParameter(_T("Name"));
			m_strMessagesFile = pLine->GetParameter(_T("MessagesFile"));
			m_strLicenseFile = pLine->GetParameter(_T("LicenseFile"));
			m_strInfoBeforeFile = pLine->GetParameter(_T("InfoBeforeFile"));
			m_strInfoAfterFile = pLine->GetParameter(_T("InfoAfterFile"));
			break;
		}
	}

	CScriptList& m_list;
	CMyDoc*      m_pDoc;
	const bool	 m_bNew;

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_LANGUAGES_LICENSEFILE2, _T("Specifies the name of an optional license agreement file, in .txt or .rtf (rich text) format, which is displayed before the user selects the destination directory for the program. This file must be located in your installation's source directory when running the Setup Compiler, unless a fully qualified pathname is specified or the pathname is prefixed by \"compiler:\", in which case it looks for the file in the Compiler directory."))
		TOOLTIP_HANDLER(IDC_LANGUAGES_INFOBEFOREFILE2, _T("Click this button to select a file."))
		TOOLTIP_HANDLER(IDC_LANGUAGES_INFOAFTERFILE2, _T("Click this button to select a file."))
		TOOLTIP_HANDLER(IDC_LANGUAGES_NAME, _T("The internal name of the language, which you can set to anything you like. This can used as a prefix on [LangOptions] or [Messages] section entries to have the entries apply to only one language. The {language} constant returns the internal name of the selected language."))
		TOOLTIP_HANDLER(IDC_LANGUAGES_MESSAGESFILE, _T("Specifies the name(s) of file(s) to read the default messages from. The file(s) must be located in your installation's source directory when running the Setup Compiler, unless a fully qualified pathname is specified or the pathname is prefixed by \"compiler:\", in which case it looks for the file in the Compiler directory"))
		TOOLTIP_HANDLER(IDC_LANGUAGES_LICENSEFILE, _T("Specifies the name of an optional license agreement file, in .txt or .rtf (rich text) format, which is displayed before the user selects the destination directory for the program. This file must be located in your installation's source directory when running the Setup Compiler, unless a fully qualified pathname is specified or the pathname is prefixed by \"compiler:\", in which case it looks for the file in the Compiler directory."))
		TOOLTIP_HANDLER(IDC_LANGUAGES_INFOBEFOREFILE, _T("Specifies the name of an optional \"readme\" file, in .txt or .rtf (rich text) format, which is displayed before the user selects the destination directory for the program. This file must be located in your installation's source directory when running the Setup Compiler, unless a fully qualified pathname is specified or the pathname is prefixed by \"compiler:\", in which case it looks for the file in the Compiler directory."))
		TOOLTIP_HANDLER(IDC_LANGUAGES_INFOAFTERFILE, _T("Specifies the name of an optional \"readme\" file, in .txt or .rtf (rich text) format, which is displayed after a successful install. This file must be located in your installation's source directory when running the Setup Compiler, unless a fully qualified pathname is specified or the pathname is prefixed by \"compiler:\", in which case it looks for the file in the Compiler directory.\r\n\r\nThis differs from isreadme files in that this text is displayed as a page of the wizard, instead of in a separate Notepad window."))
		TOOLTIP_HANDLER(IDC_LANGUAGES_MESSAGESFILE2, _T("Click this button to select one or more language files."))
	END_TOOLTIP_MAP()
};
