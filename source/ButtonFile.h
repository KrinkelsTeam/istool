/**
** @file
** @brief Source code for the CButtonFile class
*/

#pragma once

namespace Henden {

/**
** @brief Subclass a button and select file
**
** This class subclasses a button. When you click the button a file dialog will
** open, and the selected file will be inserted in the edit control before
** the subclassed button.
** @sa CButtonFolder
*/
class CButtonFile : public CWindowImpl<CButtonFile, CButton> {
public:
	BEGIN_MSG_MAP(CButtonFile)
		REFLECTED_COMMAND_CODE_HANDLER(BN_CLICKED, OnClicked)
	END_MSG_MAP()

	/// Constructor 
	CButtonFile(bool bOpen = true, LPCTSTR lpszFilter = NULL, LPCTSTR pszDefExt = NULL) : m_strFilter(lpszFilter), m_pszDefExt(pszDefExt) {
		m_bOpen = bOpen;
		if (m_strFilter.IsEmpty()) m_strFilter = _T("All Files (*.*)|*.*||");
	}

protected:
	/// The button was clicked - do our magic
	LRESULT OnClicked(WORD, WORD, HWND, BOOL&)
	{
		HWND hWndEdit = ::GetNextWindow(m_hWnd, GW_HWNDPREV);
		if (!::IsWindow(hWndEdit))
			return 0;

		CString strInitial;
		::GetWindowText(hWndEdit, strInitial.GetBuffer(MAX_PATH), MAX_PATH);
		strInitial.ReleaseBuffer();

		if (strInitial.Find(_T(':')) > 1)
			strInitial.Empty();

		// Convert to double-null filter string and parse it
		CString filterStr = MakeFilterDoubleNull(m_strFilter);
		CSimpleArray<CString> arrNames, arrSpecs;
		ParseFilterString(filterStr, arrNames, arrSpecs);

		if (arrNames.GetSize() != arrSpecs.GetSize() || arrNames.GetSize() == 0)
			return 0;

		CSimpleArray<COMDLG_FILTERSPEC> arrFilter;
		for (int i = 0; i < arrNames.GetSize(); ++i) {
			COMDLG_FILTERSPEC spec = { arrNames[i], arrSpecs[i] };
			arrFilter.Add(spec);
		}

		CString strResult;

		if (m_bOpen) {
			CShellFileOpenDialog dlg(
				strInitial,
				FOS_FILEMUSTEXIST | FOS_PATHMUSTEXIST | FOS_FORCEFILESYSTEM,
				m_pszDefExt,
				arrFilter.GetData(),
				arrFilter.GetSize()
			);

			if (dlg.DoModal(m_hWnd) == IDOK)
				dlg.GetFilePath(strResult);
		} else {
			CShellFileSaveDialog dlg(
				strInitial,
				FOS_OVERWRITEPROMPT | FOS_PATHMUSTEXIST | FOS_FORCEFILESYSTEM,
				m_pszDefExt,
				arrFilter.GetData(),
				arrFilter.GetSize()
			);

			if (dlg.DoModal(m_hWnd) == IDOK)
				dlg.GetFilePath(strResult);
		}

		if (!strResult.IsEmpty())
			::SetWindowText(hWndEdit, strResult);

		return 0;
	}

private:
	LPCTSTR	m_pszDefExt;
	CString	m_strFilter;
	BOOL	m_bOpen;

	/// Convert '|' delimited filter string to double-null-terminated format
	CString MakeFilterDoubleNull(const CString& strFilter)
	{
		CString result;
		for (int i = 0; i < strFilter.GetLength(); ++i) {
			TCHAR ch = strFilter[i];
			result += (ch == _T('|')) ? _T('\0') : ch;
		}
		result += _T('\0'); // Final null
		return result;
	}

	/// Parse double-null filter string into description/spec arrays
	void ParseFilterString(LPCTSTR psz, CSimpleArray<CString>& arrNames, CSimpleArray<CString>& arrSpecs)
	{
		arrNames.RemoveAll();
		arrSpecs.RemoveAll();

		while (*psz) {
			CString strName = psz;
			psz += strName.GetLength() + 1;
			if (!*psz)
				break;

			CString strSpec = psz;
			psz += strSpec.GetLength() + 1;

			if (!strName.IsEmpty() && !strSpec.IsEmpty()) {
				arrNames.Add(strName);
				arrSpecs.Add(strSpec);
			}
		}
	}
};

} // namespace Henden
