// ButtonVer.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ButtonVer.h"
#include "DlgVersion.h"

namespace WTL {

LRESULT CButtonVer::OnClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HWND hWnd = ::GetNextWindow(m_hWnd, GW_HWNDPREV);
	if (hWnd) {
		CString strVer;
		::GetWindowText(hWnd, strVer.GetBuffer(100), 100);

		CDlgVersion dlg(m_type, strVer);
		if (dlg.DoModal(m_hWnd) == IDOK) {
			::SetWindowText(hWnd, strVer);
		}
	}
	return 0;
}

};
