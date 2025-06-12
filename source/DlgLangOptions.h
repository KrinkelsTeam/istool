// WTL - remember OnwerDraw FIXED in dialog font combo boxes
#pragma once

#include "MyDoc.h"
#include "FontCombo.h"

static const struct {
	UINT	m_nLanguageID;
	LPCTSTR	m_pszLanguageName;
} m_langInfo[] = {
	0x0436, _T("Afrikaans"),
	0x041c, _T("Albanian"),
	0x0401, _T("Arabic (Saudi Arabia)"),
	0x0801, _T("Arabic (Iraq)"),
	0x0c01, _T("Arabic (Egypt)"),
	0x1001, _T("Arabic (Libya)"),
	0x1401, _T("Arabic (Algeria)"),
	0x1801, _T("Arabic (Morocco)"),
	0x1c01, _T("Arabic (Tunisia)"),
	0x2001, _T("Arabic (Oman)"),
	0x2401, _T("Arabic (Yemen)"),
	0x2801, _T("Arabic (Syria)"),
	0x2c01, _T("Arabic (Jordan)"),
	0x3001, _T("Arabic (Lebanon)"),
	0x3401, _T("Arabic (Kuwait)"),
	0x3801, _T("Arabic (U.A.E.)"),
	0x3c01, _T("Arabic (Bahrain)"),
	0x4001, _T("Arabic (Qatar)"),
	0x042b, _T("Armenian"),
	0x044d, _T("Assamese"),
	0x042c, _T("Azeri (Latin)"),
	0x082c, _T("Azeri (Cyrillic)"),
	0x042d, _T("Basque"),
	0x0423, _T("Belarussian"),
	0x0445, _T("Bengali"),
	0x0402, _T("Bulgarian"),
	0x0455, _T("Burmese"),
	0x0403, _T("Catalan"),
	0x0404, _T("Chinese (Taiwan)"),
	0x0804, _T("Chinese (PRC)"),
	0x0c04, _T("Chinese (Hong Kong SAR, PRC)"),
	0x1004, _T("Chinese (Singapore)"),
	0x1404, _T("Chinese (Macau SAR)"),
	0x041a, _T("Croatian"),
	0x0405, _T("Czech"),
	0x0406, _T("Danish"),
	0x0413, _T("Dutch (Netherlands)"),
	0x0813, _T("Dutch (Belgium)"),
	0x0409, _T("English (United States)"),
	0x0809, _T("English (United Kingdom)"),
	0x0c09, _T("English (Australian)"),
	0x1009, _T("English (Canadian)"),
	0x1409, _T("English (New Zealand)"),
	0x1809, _T("English (Ireland)"),
	0x1c09, _T("English (South Africa)"),
	0x2009, _T("English (Jamaica)"),
	0x2409, _T("English (Caribbean)"),
	0x2809, _T("English (Belize)"),
	0x2c09, _T("English (Trinidad)"),
	0x3009, _T("English (Zimbabwe)"),
	0x3409, _T("English (Philippines)"),
	0x0425, _T("Estonian"),
	0x0438, _T("Faeroese"),
	0x0429, _T("Farsi"),
	0x040b, _T("Finnish"),
	0x040c, _T("French (Standard)"),
	0x080c, _T("French (Belgian)"),
	0x0c0c, _T("French (Canadian)"),
	0x100c, _T("French (Switzerland)"),
	0x140c, _T("French (Luxembourg)"),
	0x180c, _T("French (Monaco)"),
	0x0437, _T("Georgian"),
	0x0407, _T("German (Standard)"),
	0x0807, _T("German (Switzerland)"),
	0x0c07, _T("German (Austria)"),
	0x1007, _T("German (Luxembourg)"),
	0x1407, _T("German (Liechtenstein)"),
	0x0408, _T("Greek"),
	0x0447, _T("Gujarati"),
	0x040d, _T("Hebrew"),
	0x0439, _T("Hindi"),
	0x040e, _T("Hungarian"),
	0x040f, _T("Icelandic"),
	0x0421, _T("Indonesian"),
	0x0410, _T("Italian (Standard)"),
	0x0810, _T("Italian (Switzerland)"),
	0x0411, _T("Japanese"),
	0x044b, _T("Kannada"),
	0x0860, _T("Kashmiri (India)"),
	0x043f, _T("Kazakh"),
	0x0457, _T("Konkani"),
	0x0412, _T("Korean"),
	0x0812, _T("Korean (Johab)"),
	0x0426, _T("Latvian"),
	0x0427, _T("Lithuanian"),
	0x0827, _T("Lithuanian (Classic)"),
	0x042f, _T("Macedonian"),
	0x043e, _T("Malay (Malaysian)"),
	0x083e, _T("Malay (Brunei Darussalam)"),
	0x044c, _T("Malayalam"),
	0x0458, _T("Manipuri"),
	0x044e, _T("Marathi"),
	0x0861, _T("Nepali (India)"),
	0x0414, _T("Norwegian (Bokmal)"),
	0x0814, _T("Norwegian (Nynorsk)"),
	0x0448, _T("Oriya"),
	0x0415, _T("Polish"),
	0x0416, _T("Portuguese (Brazil)"),
	0x0816, _T("Portuguese (Standard)"),
	0x0446, _T("Punjabi"),
	0x0418, _T("Romanian"),
	0x0419, _T("Russian"),
	0x044f, _T("Sanskrit"),
	0x0c1a, _T("Serbian (Cyrillic)"),
	0x081a, _T("Serbian (Latin)"),
	0x0459, _T("Sindhi"),
	0x041b, _T("Slovak"),
	0x0424, _T("Slovenian"),
	0x040a, _T("Spanish (Traditional Sort)"),
	0x080a, _T("Spanish (Mexican)"),
	0x0c0a, _T("Spanish (Modern Sort)"),
	0x100a, _T("Spanish (Guatemala)"),
	0x140a, _T("Spanish (Costa Rica)"),
	0x180a, _T("Spanish (Panama)"),
	0x1c0a, _T("Spanish (Dominican Republic)"),
	0x200a, _T("Spanish (Venezuela)"),
	0x240a, _T("Spanish (Colombia)"),
	0x280a, _T("Spanish (Peru)"),
	0x2c0a, _T("Spanish (Argentina)"),
	0x300a, _T("Spanish (Ecuador)"),
	0x340a, _T("Spanish (Chile)"),
	0x380a, _T("Spanish (Uruguay)"),
	0x3c0a, _T("Spanish (Paraguay)"),
	0x400a, _T("Spanish (Bolivia)"),
	0x440a, _T("Spanish (El Salvador)"),
	0x480a, _T("Spanish (Honduras)"),
	0x4c0a, _T("Spanish (Nicaragua)"),
	0x500a, _T("Spanish (Puerto Rico)"),
	0x0430, _T("Sutu"),
	0x0441, _T("Swahili (Kenya)"),
	0x041d, _T("Swedish"),
	0x081d, _T("Swedish (Finland)"),
	0x0449, _T("Tamil"),
	0x0444, _T("Tatar (Tatarstan)"),
	0x044a, _T("Telugu"),
	0x041e, _T("Thai"),
	0x041f, _T("Turkish"),
	0x0422, _T("Ukrainian"),
	0x0420, _T("Urdu (Pakistan)"),
	0x0820, _T("Urdu (India)"),
	0x0443, _T("Uzbek (Latin)"),
	0x0843, _T("Uzbek (Cyrillic)"),
	0x042a, _T("Vietnamese"),
	0x0000,	NULL
};

/////////////////////////////////////////////////////////////////////////////
// CDlgLangOptions dialog

class CDlgLangOptions :
	public CDialogImpl<CDlgLangOptions>,
	public CWinDataExchange<CDlgLangOptions>,
	public CMyDialogBase<CDlgLangOptions>
{
public:
	enum { IDD = IDD_LANGOPTIONS };

	BEGIN_MSG_MAP(CDlgLangOptions)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_HANDLER(IDC_LANGOPTIONS_LANGUAGENAME, CBN_SELCHANGE, OnSelChangeLanguageName)
		COMMAND_HANDLER(IDC_LANGOPTIONS_LANGUAGEID, CBN_SELCHANGE, OnSelChangeLanguageID)
		CHAIN_MSG_MAP(CMyDialogBase<CDlgLangOptions>)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CDlgLangOptions)
		DDX_CBINDEX(IDC_LANGOPTIONS_LANGUAGENAME, m_nLanguageName)
		DDX_CBINDEX(IDC_LANGOPTIONS_LANGUAGEID, m_nLanguageID)
		DDX_CBINDEX(IDC_LANGOPTIONS_TITLEFONTNAME, m_nTitleFontName)
		DDX_CBINDEX(IDC_LANGOPTIONS_WELCOMEFONTNAME, m_nWelcomeFontName)
		DDX_CBINDEX(IDC_LANGOPTIONS_COPYRIGHTFONTNAME, m_nCopyrightFontName)
		DDX_CBINDEX(IDC_LANGOPTIONS_DIALOGFONTNAME, m_nDialogFontName)
		DDX_INT(IDC_LANGOPTIONS_TITLEFONTSIZE, m_nTitleFontSize)
		DDX_INT(IDC_LANGOPTIONS_DIALOGFONTSIZE, m_nDialogFontSize)
		DDX_INT(IDC_LANGOPTIONS_WELCOMEFONTSIZE, m_nWelcomeFontSize)
		DDX_INT(IDC_LANGOPTIONS_COPYRIGHTFONTSIZE, m_nCopyrightFontSize)
		DDX_INT(IDC_LANGOPTIONS_DIALOGFONTSTANDARDHEIGHT, m_nDialogFontStandardHeight)
		DDX_CHECK(IDC_RIGHTTOLEFT, m_bRightToLeft)
	END_DDX_MAP()

	CUpDownCtrl	m_wndSpin5;
	CUpDownCtrl	m_wndSpin4;
	CUpDownCtrl	m_wndSpin3;
	CUpDownCtrl	m_wndSpin2;
	CUpDownCtrl	m_wndSpin1;
	CFontCombo	m_wndCopyrightFontName;
	CFontCombo	m_wndWelcomeFontName;
	CFontCombo	m_wndTitleFontName;
	CFontCombo	m_wndDialogFontName;
	CComboBox	m_wndLanguageID;
	CComboBox	m_wndLanguageName;
	int		m_nLanguageName;
	int		m_nLanguageID;
	int		m_nTitleFontName;
	int		m_nWelcomeFontName;
	int		m_nCopyrightFontName;
	int		m_nDialogFontName;
	UINT	m_nTitleFontSize;
	UINT	m_nDialogFontSize;
	UINT	m_nWelcomeFontSize;
	UINT	m_nCopyrightFontSize;
	UINT	m_nDialogFontStandardHeight;
	BOOL	m_bRightToLeft;

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if (wID == IDOK) {
			DoDataExchange(DDX_SAVE);
			CInnoScriptEx& script = m_pDoc->GetScript();
			CString		strLanguageName = m_langInfo[m_nLanguageName].m_pszLanguageName;
			CString		strLanguageID; strLanguageID.Format(_T("$%04X"), m_langInfo[m_nLanguageName].m_nLanguageID);

			int n = strLanguageName.Find('(');
			if (n >= 0) {
				strLanguageName.GetBufferSetLength(n);
				strLanguageName.TrimRight();
			}

			CString str;

			script.SetPropertyString(_T("LanguageName"), strLanguageName, CInnoScript::SEC_LANGOPTIONS);
			script.SetPropertyString(_T("LanguageID"), strLanguageID, CInnoScript::SEC_LANGOPTIONS);
			m_wndDialogFontName.GetWindowText(str.GetBuffer(255), 256); str.ReleaseBuffer();
			script.SetPropertyString(_T("DialogFontName"), str, CInnoScript::SEC_LANGOPTIONS);
			script.SetPropertyNumber(_T("DialogFontSize"), m_nDialogFontSize, CInnoScript::SEC_LANGOPTIONS);
			script.SetPropertyNumber(_T("DialogFontStandardHeight"), m_nDialogFontStandardHeight, CInnoScript::SEC_LANGOPTIONS);
			m_wndTitleFontName.GetWindowText(str.GetBuffer(255), 256); str.ReleaseBuffer();
			script.SetPropertyString(_T("TitleFontName"), str, CInnoScript::SEC_LANGOPTIONS);
			script.SetPropertyNumber(_T("TitleFontSize"), m_nTitleFontSize, CInnoScript::SEC_LANGOPTIONS);
			m_wndWelcomeFontName.GetWindowText(str.GetBuffer(255), 256); str.ReleaseBuffer();
			script.SetPropertyString(_T("WelcomeFontName"), str, CInnoScript::SEC_LANGOPTIONS);
			script.SetPropertyNumber(_T("WelcomeFontSize"), m_nWelcomeFontSize, CInnoScript::SEC_LANGOPTIONS);
			m_wndCopyrightFontName.GetWindowText(str.GetBuffer(255), 256); str.ReleaseBuffer();
			script.SetPropertyString(_T("CopyrightFontName"), str, CInnoScript::SEC_LANGOPTIONS);
			script.SetPropertyNumber(_T("CopyrightFontSize"), m_nCopyrightFontSize, CInnoScript::SEC_LANGOPTIONS);
			script.SetPropertyBool(_T("RightToLeft"), m_bRightToLeft ? true : false, CInnoScript::SEC_LANGOPTIONS);
		}
		EndDialog(wID);
		return 0;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd, _T("LangOptions"));
		CenterWindow(GetParent());

		m_wndSpin1.Attach(GetDlgItem(IDC_SPIN1));
		m_wndSpin2.Attach(GetDlgItem(IDC_SPIN2));
		m_wndSpin3.Attach(GetDlgItem(IDC_SPIN3));
		m_wndSpin4.Attach(GetDlgItem(IDC_SPIN4));
		m_wndSpin5.Attach(GetDlgItem(IDC_SPIN5));
		m_wndCopyrightFontName.SubclassWindow(GetDlgItem(IDC_LANGOPTIONS_COPYRIGHTFONTNAME));
		m_wndWelcomeFontName.SubclassWindow(GetDlgItem(IDC_LANGOPTIONS_WELCOMEFONTNAME));
		m_wndTitleFontName.SubclassWindow(GetDlgItem(IDC_LANGOPTIONS_TITLEFONTNAME));
		m_wndDialogFontName.SubclassWindow(GetDlgItem(IDC_LANGOPTIONS_DIALOGFONTNAME));
		m_wndLanguageID.Attach(GetDlgItem(IDC_LANGOPTIONS_LANGUAGEID));
		m_wndLanguageName.Attach(GetDlgItem(IDC_LANGOPTIONS_LANGUAGENAME));

		m_nLanguageName = -1;
		m_nLanguageID = -1;
		m_nTitleFontName = -1;
		m_nWelcomeFontName = -1;
		m_nCopyrightFontName = -1;
		m_nDialogFontName = -1;

		CInnoScriptEx& script = m_pDoc->GetScript();
		m_nDialogFontSize = script.GetPropertyNumber(_T("DialogFontSize"), CInnoScript::SEC_LANGOPTIONS);
		m_nTitleFontSize = script.GetPropertyNumber(_T("TitleFontSize"), CInnoScript::SEC_LANGOPTIONS);
		m_nWelcomeFontSize = script.GetPropertyNumber(_T("WelcomeFontSize"), CInnoScript::SEC_LANGOPTIONS);
		m_nCopyrightFontSize = script.GetPropertyNumber(_T("CopyrightFontSize"), CInnoScript::SEC_LANGOPTIONS);
		m_nDialogFontStandardHeight = script.GetPropertyNumber(_T("DialogFontStandardHeight"), CInnoScript::SEC_LANGOPTIONS);

		m_wndDialogFontName.Initialize();
		m_wndTitleFontName.Initialize();
		m_wndWelcomeFontName.Initialize();
		m_wndCopyrightFontName.Initialize();
		m_wndSpin1.SetRange32(6, 128);
		m_wndSpin2.SetRange32(6, 128);
		m_wndSpin3.SetRange32(6, 128);
		m_wndSpin4.SetRange32(6, 128);
		m_wndSpin5.SetRange32(6, 128);

		CString strLanguageID = script.GetPropertyString(_T("LanguageID"), CInnoScript::SEC_LANGOPTIONS);
		UINT nLanguage = 0;
		while (m_langInfo[nLanguage].m_nLanguageID && m_langInfo[nLanguage].m_pszLanguageName) {
			m_wndLanguageName.AddString(m_langInfo[nLanguage].m_pszLanguageName);
			CString strID;
			strID.Format(_T("$%04X"), m_langInfo[nLanguage].m_nLanguageID);
			int nIndex = m_wndLanguageID.AddString(strID);
			if (!_stricmp(strLanguageID, strID)) {
				m_nLanguageName = nIndex;
				m_nLanguageID = nIndex;
			}
			nLanguage++;
		}

		m_nDialogFontName = m_wndDialogFontName.FindStringExact(0, script.GetPropertyString(_T("DialogFontName"), CInnoScript::SEC_LANGOPTIONS));
		m_wndDialogFontName.SetCurSel(m_nDialogFontName);
		m_nTitleFontName = m_wndTitleFontName.FindStringExact(0, script.GetPropertyString(_T("TitleFontName"), CInnoScript::SEC_LANGOPTIONS));
		m_wndTitleFontName.SetCurSel(m_nTitleFontName);
		m_nWelcomeFontName = m_wndWelcomeFontName.FindStringExact(0, script.GetPropertyString(_T("WelcomeFontName"), CInnoScript::SEC_LANGOPTIONS));
		m_wndWelcomeFontName.SetCurSel(m_nWelcomeFontName);
		m_nCopyrightFontName = m_wndCopyrightFontName.FindStringExact(0, script.GetPropertyString(_T("CopyrightFontName"), CInnoScript::SEC_LANGOPTIONS));
		m_wndCopyrightFontName.SetCurSel(m_nCopyrightFontName);

		m_nTitleFontSize = script.GetPropertyNumber(_T("TitleFontSize"), CInnoScript::SEC_LANGOPTIONS);
		m_bRightToLeft = script.GetPropertyBool(_T("RightToLeft"), CInnoScript::SEC_LANGOPTIONS);

		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnSelChangeLanguageName(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		DoDataExchange(DDX_SAVE);
		m_nLanguageID = m_nLanguageName;
		DoDataExchange(DDX_LOAD);
		return 0;
	}

	LRESULT OnSelChangeLanguageID(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		DoDataExchange(DDX_SAVE);
		m_nLanguageName = m_nLanguageID;
		DoDataExchange(DDX_LOAD);
		return 0;
	}

	CDlgLangOptions(CMyDoc* pDoc) : m_pDoc(pDoc) {}
protected:
	CMyDoc* m_pDoc;
};
