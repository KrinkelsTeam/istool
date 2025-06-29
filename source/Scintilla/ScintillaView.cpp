#include "stdafx.h"
#include "ISTool.h"
#include "MainFrm.h"
#include "DlgGoto.h"

LRESULT CScintillaView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	LRESULT res = DefWindowProc();
	SetCodePage(SC_CP_UTF8);

	static bool bLoaded = false;
	if (!bLoaded) {
		//LoadLexerLibrary("isslexer.dll");
		bLoaded = true;
	}
	return res;
}

LRESULT CScintillaView::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	if (AfxGetMainWnd().m_pFindReplaceDlg)
		AfxGetMainWnd().m_pFindReplaceDlg->EndDialog(IDCANCEL);
	bHandled = FALSE;
	return 0;
}

LRESULT CScintillaView::OnFindReplaceMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	if (AfxGetMainWnd().m_pFindReplaceDlg->IsTerminating()) {
		AfxGetMainWnd().m_pFindReplaceDlg = NULL;
	} else if (AfxGetMainWnd().m_pFindReplaceDlg->FindNext()) {
		m_strSearch = AfxGetMainWnd().m_pFindReplaceDlg->GetFindString();

		m_iSearchFlags = 0;
		if (AfxGetMainWnd().m_pFindReplaceDlg->MatchCase())
			m_iSearchFlags |= SCFIND_MATCHCASE;
		if (AfxGetMainWnd().m_pFindReplaceDlg->MatchWholeWord())
			m_iSearchFlags |= SCFIND_WHOLEWORD;

		CStringA strSearchUtf8 = CSTRING_TO_UTF8(m_strSearch);
		
		Sci_TextToFind ft = { 0 };
		ft.chrg.cpMin = GetCurrentPos() + 1;
		ft.chrg.cpMax = GetTextLength();
		ft.lpstrText = (LPSTR)(LPCSTR)strSearchUtf8;
		long pos = FindText(m_iSearchFlags, &ft);
		if (pos == -1) {
			MessageBeep(MB_ICONERROR);
		} else {
			SetCurrentPos(pos);
			SetSel(ft.chrgText.cpMin, ft.chrgText.cpMax);
		}
	} else if (AfxGetMainWnd().m_pFindReplaceDlg->ReplaceCurrent()) {
		if (GetSelectionStart() < GetSelectionEnd()) {
			m_strReplace = AfxGetMainWnd().m_pFindReplaceDlg->GetReplaceString();
			CStringA strReplaceUtf8 = CSTRING_TO_UTF8(m_strReplace);
			ReplaceSel(strReplaceUtf8);
		}
	} else if (AfxGetMainWnd().m_pFindReplaceDlg->ReplaceAll()) {
		m_strSearch = AfxGetMainWnd().m_pFindReplaceDlg->GetFindString();
		m_strReplace = AfxGetMainWnd().m_pFindReplaceDlg->GetReplaceString();

		m_iSearchFlags = 0;
		if (AfxGetMainWnd().m_pFindReplaceDlg->MatchCase())
			m_iSearchFlags |= SCFIND_MATCHCASE;
		if (AfxGetMainWnd().m_pFindReplaceDlg->MatchWholeWord())
			m_iSearchFlags |= SCFIND_WHOLEWORD;

		CStringA strSearchUtf8 = CSTRING_TO_UTF8(m_strSearch);
		CStringA strReplaceUtf8 = CSTRING_TO_UTF8(m_strReplace);
		
		long nStart = 0;
		do {
			Sci_TextToFind ft = { 0 };
			ft.chrg.cpMin = nStart;
			ft.chrg.cpMax = GetTextLength();
			ft.lpstrText = (LPSTR)(LPCSTR)strSearchUtf8;
			long pos = FindText(m_iSearchFlags, &ft);
			if (pos == -1) {
				//MessageBeep(MB_ICONERROR);
				break;
			} else {
				//SetCurrentPos(pos);
				SetSel(ft.chrgText.cpMin, ft.chrgText.cpMax);
				ReplaceSel(strReplaceUtf8);
				nStart = GetCurrentPos() + m_strReplace.GetLength();
			}
		} while (true);
#ifdef _DEBUG
	} else {
		AtlMessageBox(m_hWnd, _T("findreplace unhandled"));
#endif
	}
	return 0;
}

LRESULT CScintillaView::OnFind(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	if (!AfxGetMainWnd().m_pFindReplaceDlg) {
		long nStart = GetSelectionStart();
		long nEnd = GetSelectionEnd();
		if (nStart < nEnd) {
			CStringA strA;
			strA.GetBufferSetLength((nEnd - nStart) * 4); // reserve for UTF-8
			GetSelText(strA.GetBuffer());
			strA.ReleaseBuffer();

			m_strSearch = UTF8_TO_CSTRING(strA);
		}
		AfxGetMainWnd().m_pFindReplaceDlg = new CFindReplaceDialog();
		AfxGetMainWnd().m_pFindReplaceDlg->Create(TRUE, m_strSearch, NULL, FR_HIDEUPDOWN | FR_DOWN, m_hWnd);
		AfxGetMainWnd().m_pFindReplaceDlg->CenterWindow(m_hWnd);
		AfxGetMainWnd().m_pFindReplaceDlg->ShowWindow(SW_SHOW);
	}
	return 0;
}

LRESULT CScintillaView::OnRepeat(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	if (m_strSearch.IsEmpty()) {
		SendMessage(UWM_FIND);
		return 0;
	}

	Sci_TextToFind ft = { 0 };
	ft.chrg.cpMin = GetCurrentPos() + 1;
	ft.chrg.cpMax = GetTextLength();

	CStringA strUtf8 = CSTRING_TO_UTF8(m_strSearch);
	ft.lpstrText = (LPSTR)(LPCSTR)strUtf8;

	long pos = FindText(m_iSearchFlags, &ft);
	if (pos == -1) {
		MessageBeep(MB_ICONERROR);
	} else {
		SetCurrentPos(pos);
		SetSel(ft.chrgText.cpMin, ft.chrgText.cpMax);
	}

	return 0;
}

LRESULT CScintillaView::OnReplace(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	if (!AfxGetMainWnd().m_pFindReplaceDlg) {
		long nStart = GetSelectionStart();
		long nEnd = GetSelectionEnd();
		if (nStart < nEnd) {
			CStringA strA;
			strA.GetBufferSetLength((nEnd - nStart) * 4); // reserve for UTF-8
			GetSelText(strA.GetBuffer());
			strA.ReleaseBuffer();

			m_strSearch = UTF8_TO_CSTRING((LPCSTR)strA);
		}

		AfxGetMainWnd().m_pFindReplaceDlg = new CFindReplaceDialog();
		AfxGetMainWnd().m_pFindReplaceDlg->Create(FALSE, m_strSearch, m_strReplace, FR_HIDEUPDOWN | FR_DOWN, m_hWnd);
		AfxGetMainWnd().m_pFindReplaceDlg->CenterWindow(m_hWnd);
		AfxGetMainWnd().m_pFindReplaceDlg->ShowWindow(SW_SHOW);
	}
	return 0;
}

LRESULT CScintillaView::OnGoto(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	CDlgGoto dlg;
	dlg.m_strLineNumber.Format(_T("%d"), LineFromPosition(GetCurrentPos()) + 1);
	if (dlg.DoModal(m_hWnd) != IDOK)
		return 0;

	long nLine = _ttol(dlg.m_strLineNumber) - 1;
	if (nLine < 0)
		return 0;

	long nPos = PositionFromLine(nLine);
	SetCurrentPos(nPos);
	SetSel(nPos, nPos);
	CWindow::SetFocus();

	return 0;
}

LRESULT CScintillaView::OnSelectAll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	SelectAll();
	return 0;
}

LRESULT CScintillaView::OnDelete(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	Clear();
	return 0;
}

LRESULT CScintillaView::OnUndo(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	Undo();
	return 0;
}

LRESULT CScintillaView::OnRedo(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	Redo();
	return 0;
}

LRESULT CScintillaView::OnEditCommentSelection(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	return 0;
}

LRESULT CScintillaView::OnEditUncommentSelection(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	return 0;
}
