/**
** @file
** @brief Source code for the CButtonFolder class
*/

#pragma once

namespace Henden {

/**
** @brief Subclass a button and select folder
**
** This class subclasses a button. When you click the button a folder dialog will
** open, and the selected folder will be inserted in the edit control before
** the subclassed button.
** @sa CButtonFile
*/
class CButtonFolder : public CWindowImpl<CButtonFolder, CButton> {
public:
	BEGIN_MSG_MAP(CButtonFolder)
		COMMAND_CODE_HANDLER(BN_CLICKED, OnClicked)
		REFLECTED_COMMAND_CODE_HANDLER(BN_CLICKED, OnClicked)
	END_MSG_MAP()

	/// Constructor
	CButtonFolder(LPCTSTR lpszTitle = NULL) : m_lpszTitle(lpszTitle) {}

protected:
	/// The button was clicked - open a browse for folder dialog
	LRESULT OnClicked(WORD, WORD, HWND, BOOL&) {
		HWND hWndEdit = ::GetNextWindow(m_hWnd, GW_HWNDPREV);
		if (!hWndEdit)
			return 0;

		CString strInitial;
		::GetWindowText(hWndEdit, strInitial.GetBuffer(MAX_PATH), MAX_PATH);

		CShellFileOpenDialog dlg;
		CComPtr<IFileOpenDialog> pDlg = dlg.GetPtr();

		DWORD dwOptions = 0;
		pDlg->GetOptions(&dwOptions);
		pDlg->SetOptions(dwOptions | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);

		if (m_lpszTitle != NULL)
			pDlg->SetTitle(m_lpszTitle);

		if (!strInitial.IsEmpty()) {
			CComPtr<IShellItem> psiFolder;
			if (SUCCEEDED(SHCreateItemFromParsingName(strInitial, NULL, IID_PPV_ARGS(&psiFolder)))) {
				pDlg->SetFolder(psiFolder);
			}
		}

		if (dlg.DoModal(m_hWnd) == IDOK) {
			CComPtr<IShellItem> pItem;
			if (SUCCEEDED(pDlg->GetResult(&pItem))) {
				CString strFolder = GetShellItemPath(pItem);
				::SetWindowText(hWndEdit, strFolder);
			}
		}

		return 0;
	}

private:
	CString GetShellItemPath(IShellItem* pItem) {
		CString strPath;
		if (pItem) {
			PWSTR pszPath = nullptr;
			if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszPath))) {
				strPath = pszPath;
				CoTaskMemFree(pszPath);
			}
		}
		return strPath;
	}

	LPCTSTR m_lpszTitle;
};

} // namespace Henden
