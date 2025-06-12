// Setup3.h

#pragma once

#include "MyDoc.h"
#include "Edit2.h"

/////////////////////////////////////////////////////////////////////////////
// CSetupUserInfo dialog

class CSetupUserInfo :
	public CPropertyPageImpl<CSetupUserInfo>,
	public CMyPropertyPageBase<CSetupUserInfo>,
	public CWinDataExchange<CSetupUserInfo>,
	public Henden::CToolTipDialog<CSetupUserInfo>
{
public:
	enum { IDD = IDD_SETUP_USERINFO };

	BEGIN_MSG_MAP(CSetupUserInfo)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CSetupUserInfo>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CSetupUserInfo>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CSetupUserInfo>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_USERINFOPAGE, BN_CLICKED, OnUserInfoPage)
		COMMAND_HANDLER(IDC_DEFAULTUSERINFONAME, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_DEFAULTUSERINFOORG, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_USEPREVIOUSUSERINFO, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_DEFAULTUSERINFOSERIAL, BN_CLICKED, OnModified)
		//	REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CSetupUserInfo)
		DDX_CHECK(IDC_USEPREVIOUSUSERINFO, m_bUsePreviousUserInfo)
		DDX_CHECK(IDC_USERINFOPAGE, m_bUserInfoPage)
		DDX_TEXT(IDC_DEFAULTUSERINFONAME, m_strDefaultUserInfoName)
		DDX_TEXT(IDC_DEFAULTUSERINFOORG, m_strDefaultUserInfoOrg)
		DDX_TEXT(IDC_DEFAULTUSERINFOSERIAL, m_strDefaultUserInfoSerial)
	END_DDX_MAP()

	BOOL	m_bUsePreviousUserInfo;
	BOOL	m_bUserInfoPage;
	CString	m_strDefaultUserInfoName;
	CString	m_strDefaultUserInfoOrg;
	CString	m_strDefaultUserInfoSerial;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd, _T("UserInfo"));

		GetDlgItem(IDC_STATIC1).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_STATIC2).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_STATIC3).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_DEFAULTUSERINFONAME).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_DEFAULTUSERINFOORG).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_USEPREVIOUSUSERINFO).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_DEFAULTUSERINFOSERIAL).EnableWindow(m_bUserInfoPage);

		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		CInnoScriptEx& script = m_pDoc->GetScript();

		// Inno Setup 3.0.0
		script.SetPropertyBool(_T("UsePreviousUserInfo"), m_bUsePreviousUserInfo ? true : false);
		script.SetPropertyBool(_T("UserInfoPage"), m_bUserInfoPage ? true : false);
		script.SetPropertyString(_T("DefaultUserInfoName"), m_strDefaultUserInfoName);
		script.SetPropertyString(_T("DefaultUserInfoOrg"), m_strDefaultUserInfoOrg);
		// My Inno Setup Extensions 3.0.6.1
		script.SetPropertyString(_T("DefaultUserInfoSerial"), m_strDefaultUserInfoSerial);

		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		SetModified();
		return 0;
	}

	LRESULT OnCreateAppDir(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if (!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);
		return 0;
	}

	LRESULT OnDisableProgramGroupPage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if (!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);

		return 0;
	}

	LRESULT OnUninstallable(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if (!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);
		return 0;
	}

	LRESULT OnUserInfoPage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if (!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);

		GetDlgItem(IDC_STATIC1).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_STATIC2).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_STATIC3).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_DEFAULTUSERINFONAME).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_DEFAULTUSERINFOORG).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_USEPREVIOUSUSERINFO).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_DEFAULTUSERINFOSERIAL).EnableWindow(m_bUserInfoPage);
		return 0;
	}

	CSetupUserInfo(CMyDoc* pDoc, const bool bNew, LPCTSTR pszTitle) :
		m_pDoc(pDoc),
		m_bNew(bNew),
		CPropertyPageImpl<CSetupUserInfo>(pszTitle)
	{
		CInnoScriptEx& script = m_pDoc->GetScript();

		// Inno Setup 3.0.0
		m_bUsePreviousUserInfo = script.GetPropertyBool(_T("UsePreviousUserInfo"));
		m_bUserInfoPage = script.GetPropertyBool(_T("UserInfoPage"));
		m_strDefaultUserInfoName = script.GetPropertyString(_T("DefaultUserInfoName"));
		m_strDefaultUserInfoOrg = script.GetPropertyString(_T("DefaultUserInfoOrg"));
		// My Inno Setup Extensions 3.0.6.1
		m_strDefaultUserInfoSerial = script.GetPropertyString(_T("DefaultUserInfoSerial"));
	}

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_USEPREVIOUSUSERINFO, _L(_T("Help|UserInfo|UsePreviousUserInfo"), _T("When this directive is yes, the default, at startup Setup will look in the registry to see if the same application is already installed, and if so, it will use the name, organization and serial number entered previously as the default settings presented to the user on the User Information wizard page.")))
		TOOLTIP_HANDLER(IDC_USERINFOPAGE, _L(_T("Help|UserInfo|UserInfoPage"), _T("If this is set to yes, Setup will show a User Information wizard page which asks for the user's name, organization and possibly a serial number.")))
		TOOLTIP_HANDLER(IDC_DEFAULTUSERINFONAME, _L(_T("Help|UserInfo|DefaultUserInfoName"), _T("Specifies the default name shown on the User Information wizard page. This can include constants.")))
		TOOLTIP_HANDLER(IDC_DEFAULTUSERINFOORG, _L(_T("Help|UserInfo|DefaultUserInfoOrg"), _T("Specifies the default organization shown on the User Information wizard page. This can include constants.")))
		TOOLTIP_HANDLER(IDC_DEFAULTUSERINFOSERIAL, _L(_T("Help|UserInfo|DefaultUserInfoSerial"), _T("Specifies the default serial number shown on the User Information wizard page. This can include constants.")))
	END_TOOLTIP_MAP()

protected:
	CMyDoc* m_pDoc;
	const bool	m_bNew;
};
