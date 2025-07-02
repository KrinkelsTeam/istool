/*==============================================================================

  $Id: MenuProject.cpp,v 1.1 2002/03/21 17:55:38 net Exp $

  Implements the Project menu in the CMainFrame class

==============================================================================*/

#include "stdafx.h"
#include "ISTool.h"
#include "MainFrm.h"

#include "DlgSelectFonts.h"
#include "DlgCompile.h"
#include "DlgFileAssociation.h"
#include "FilesHelper.h"
#include "DlgIEShortcut.h"
#include "DlgLangOptions.h"
#include "DlgLogFile.h"
// Setup pages
#include "SetupCompiler.h"
#include "SetupDiskSpanning.h"
#include "SetupApplication.h"
#include "SetupUserInfo.h"
#include "SetupAppDir.h"
#include "SetupProgramGroup.h"
#include "SetupPages.h"
#include "SetupAppearance.h"
#include "SetupUninstall.h"
#include "Sheets.h"
#include "TextFileIO.h"

LRESULT CMainFrame::OnProjectUseAbsolutePaths(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CUpdate::UpdateAll(CUpdate::HINT_APPLYCHANGES);
	bool bAbs = true;
	try {
		bAbs = !m_document.GetScript().GetPropertyBool(_T("UseAbsolutePaths"), CInnoScript::PRJ_ISTOOL);
	} catch (...) {
		bAbs ^= 1;
	}
	m_document.GetScript().SetPropertyBool(_T("UseAbsolutePaths"), bAbs, CInnoScript::PRJ_ISTOOL);
	m_document.SetModifiedFlag(true);

	CScriptList files;
	m_document.GetScript().GetList(CInnoScript::SEC_FILES, files);

	CString strSourceDir;
	m_document.GetSourceDir(strSourceDir);

	if (files.GetSize() > 0) {
		if (bAbs) {
			int nResult = AtlMessageBox(m_hWnd, _L(_T("ConvertAbsPaths"), _T("Do you want to convert existing paths to absolute paths?")), IDR_MAINFRAME, MB_YESNO | MB_ICONQUESTION);
			if (nResult == IDYES) {
				for (int nPos = 0; nPos < files.GetSize(); nPos++) {
					CScriptLine* pLine = files[nPos];
					CString strSource = pLine->GetParameter(_T("Source"));
					if (CFunc::GetDriveLength(strSource) == 0 && (strSource.GetLength() == 0 || strSource[0] != _T('{'))) {
						// This is a relative path
						CString strNewSource(strSourceDir);
						if (strSource.GetLength() > 0) {
							if (strSource.Left(3) == _T("..\\") || strSource.Left(3) == _T("../")) {
								int nPos = strNewSource.ReverseFind(_T('\\'));
								if (nPos < 0)
									nPos = strNewSource.ReverseFind(_T('/'));
								if (nPos >= 0) {
									strNewSource = strNewSource.Left(nPos);
									strSource = strSource.Mid(3);
								}
							} else if (strSource.Left(2) == _T(".\\") || strSource.Left(2) == _T("./")) {
								strSource = strSource.Mid(2);
							}

							if (strSource[0] != _T('\\') && strSource[0] != _T('/'))
								CMyUtils::EndWith(strNewSource, _T('\\'));

							strNewSource += strSource;
							pLine->SetParameter(_T("Source"), strNewSource);
						}
					}
				}
			}
		} else {
			int nResult = AtlMessageBox(m_hWnd, _L(_T("ConvertRelPaths"), _T("Do you want to convert existing paths to relative paths?")), IDR_MAINFRAME, MB_YESNO | MB_ICONQUESTION);
			if (nResult == IDYES) {
				for (int nPos = 0; nPos < files.GetSize(); nPos++) {
					CScriptLine* pLine = files[nPos];
					CString strSource = pLine->GetParameter(_T("Source"));
					strSource = CFunc::ExtractRelativePath(strSourceDir, strSource);
					if (!strSource.IsEmpty() && strSource[0] != _T('{'))
						pLine->SetParameter(_T("Source"), strSource);
				}
			}
		}
	}

	CUpdate::UpdateAll();
	return 0;
}

LRESULT CMainFrame::OnProjectRunSetup(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CWaitCursor wait;
	CString strOutputExe;
	if (m_document.GetOutputExe(m_document.m_pScript, m_hWnd, strOutputExe))
		CMyApp::MyExec(strOutputExe, NULL);
	return 0;
}

LRESULT CMainFrame::OnProjectCompileAndRunSetup(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CDlgCompile dlg(&m_document, true);
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);
	if (dlg.LoadCompiler()) {
		if (dlg.DoModal(m_hWnd) == IDC_GOTOERROR) {
			SetSection(ID_VIEW_SCRIPT);
			m_wndView.SendMessage(SCI_GOTOLINE, (WPARAM)dlg.GetErrorLine(), (LPARAM)0);
			m_wndView.SetFocus();
		} else if (!CMyApp::m_prefs.m_bNoOutputExeFilename)
			m_document.UpdateAll();
	}

	return 0;
}

LRESULT CMainFrame::OnProjectOpenCompiler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	m_document.OpenInnoSetup(m_hWnd);
	return 0;
}

LRESULT CMainFrame::OnProjectCompileScript(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CDlgCompile dlg(&m_document, false);
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);
	if (dlg.LoadCompiler()) {
		if (dlg.DoModal(m_hWnd) == IDC_GOTOERROR) {
			SetSection(ID_VIEW_SCRIPT);
			m_wndView.SendMessage(SCI_GOTOLINE, (WPARAM)dlg.GetErrorLine(), (LPARAM)0);
			m_wndView.SetFocus();
			return 1;
		} else if (!CMyApp::m_prefs.m_bNoOutputExeFilename)
			m_document.UpdateAll();
	}
	return 0;
}

LRESULT CMainFrame::OnProjectCreateAssociation(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	CDlgFileAssociation	dlg(&m_document);
	if (dlg.DoModal(AfxGetMainWnd()) == IDOK) {
		m_document.UpdateAll();
	}
	return 0;
}

LRESULT CMainFrame::OnProjectInsertFiles(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	CFilesHelper helper(&m_document);
	CString strFolder;
	if (m_wndFilesT.GetView()) {
		m_wndFilesT.GetView()->GetCurrentFolder(strFolder);
	} else if (m_wndFilesL.GetView() && m_wndFilesL.GetView()->GetSelectedCount() == 1) {
		UINT nCount = m_wndFilesL.GetView()->GetItemCount();
		while (nCount--) {
			if (m_wndFilesL.GetView()->GetItemState(nCount, LVIS_SELECTED) == LVIS_SELECTED) {
				CScriptLine* pLine = (CScriptLine*)m_wndFilesL.GetView()->GetItemData(nCount);
				if (pLine && pLine->GetSection() == CInnoScript::SEC_DIRS) {
					strFolder = pLine->GetParameter(_T("Name"));
				}
				break;
			}
		}
	}
	if (strFolder.IsEmpty())
		strFolder = _T("{app}");

	helper.OnFileAddFiles(strFolder);
	m_document.UpdateAll();
	return 0;
}

LRESULT CMainFrame::OnProjectImportRegistry(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	const COMDLG_FILTERSPEC filters[] = {
		{ _T("Registry Files (*.reg)"), _T("*.reg") },
		{ _T("All Files (*.*)"), _T("*.*") }
	};

	CShellFileOpenDialog dlg(
		NULL, // lpszFileName
		FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST,
		_T("reg"), // default extension
		filters,
		_countof(filters)
	);

	if (dlg.DoModal(m_hWnd) != IDOK)
		return 0;

	CString strFile;
	if (FAILED(dlg.GetFilePath(strFile)))
		return 0;

	CFilesHelper helper(&m_document);
	if (helper.ImportRegistry(m_hWnd, strFile))
		m_document.UpdateAll();
	else
		m_document.UpdateAll();	// for reg on W2000?!?
	return 0;
}

LRESULT CMainFrame::OnProjectImportIni(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	const COMDLG_FILTERSPEC filters[] = {
		{ _T("INI Files (*.ini)"), _T("*.ini") },
		{ _T("All Files (*.*)"), _T("*.*") }
	};

	CShellFileOpenDialog dlg(
		NULL,
		FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST,
		_T("ini"),
		filters,
		_countof(filters)
	);

	if (dlg.DoModal(m_hWnd) != IDOK)
		return 0;

	CString strFile;
	if (FAILED(dlg.GetFilePath(strFile)))
		return 0;

	CFilesHelper helper(&m_document);
	helper.ImportIni(m_hWnd, strFile);

	return 0;
}

LRESULT CMainFrame::OnProjectInstallFonts(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CWaitCursor wait;

	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	CDlgSelectFonts dlg(&m_document);
	if (dlg.DoModal(AfxGetMainWnd()) == IDOK) {
		wait.Restore();
		m_document.UpdateAll();
	}
	return 0;
}

static LPCTSTR GetFullRoot(LPCTSTR pszRoot) {
	if (!_tcsicmp(pszRoot, _T("HKCR")))
		return _T("HKEY_CLASSES_ROOT");
	if (!_tcsicmp(pszRoot, _T("HKCU")))
		return _T("HKEY_CURRENT_USER");
	if (!_tcsicmp(pszRoot, _T("HKLM")))
		return _T("HKEY_LOCAL_MACHINE");
	if (!_tcsicmp(pszRoot, _T("HKU")))
		return _T("HKEY_USERS");
	if (!_tcsicmp(pszRoot, _T("HKCC")))
		return _T("HKEY_CURRENT_CONFIG");

	return pszRoot;
}

LRESULT CMainFrame::OnProjectExportRegistry(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CScriptList	list;
	m_document.GetScript().GetList(CInnoScript::SEC_REGISTRY, list);
	if (list.GetSize() == 0) return 0;

	// Set up the shell save dialog with .reg filter
	static const COMDLG_FILTERSPEC filters[] = {
		{ _T("Registry Files (*.reg)"), _T("*.reg") },
		{ _T("All Files (*.*)"),        _T(L"*.*")  }
	};

	CString defaultName; // Optional: generate default name here if needed

	CShellFileSaveDialog dlg(
		defaultName,
		FOS_OVERWRITEPROMPT | FOS_PATHMUSTEXIST | FOS_FORCEFILESYSTEM,
		_T("reg"),
		filters,
		_countof(filters)
	);

	if (dlg.DoModal(m_hWnd) != IDOK)
		return 0;

	CString strFile;
	if (FAILED(dlg.GetFilePath(strFile)))
		return 0;

	CWaitCursor wait;

	// Function to format hex bytes from a string
	auto formatHexBytes = [](const CString& str) -> CString {
		CString result;
		for (int i = 0; i < str.GetLength(); ++i) {
			result.AppendFormat(_T("%02X,"), (BYTE)str[i]);
		}
		result += _T("00");
		return result;
	};

	// Function to format multi-string values as hex(7):...
	auto formatMultiSz = [](const CString& str) -> CString {
		CString result;
		CStringToken tokens(str, _T("|"));
		CString part;
		while (!(part = tokens.GetNext()).IsEmpty()) {
			for (int i = 0; i < part.GetLength(); ++i)
				result.AppendFormat(_T("%02X,"), (BYTE)part[i]);
			result.Append(_T("00,"));
		}
		result += _T("00");
		result.TrimRight(_T(","));
		return _T("hex(7):") + result;
	};

	CString out;
	out += _T("Windows Registry Editor Version 5.00\r\n\r\n");

	for (int i = 0; i < list.GetSize(); ++i) {
		CScriptLine* pLine = list[i];
		CString strRoot = GetFullRoot(pLine->GetParameter(_T("Root")));
		CString strSubKey = pLine->GetParameter(_T("SubKey"));
		CString section;
		section.Format(_T("[%s\\%s]\r\n"), (LPCWSTR)strRoot, (LPCWSTR)strSubKey);
		out += section;

		CString strValueName = pLine->GetParameter(_T("ValueName"));
		CString strValueData = pLine->GetParameter(_T("ValueData"));
		CString strValueType = pLine->GetParameter(_T("ValueType"));

		if (strValueName.IsEmpty()) strValueName = _T("@");

		CString line;

		if (strValueType.IsEmpty() || !_tcsicmp(strValueType, _T("none"))) {
			// Key-only (no value)
			line.Format(_T("%s=-\r\n"), (LPCTSTR)strValueName);
		} else if (!_tcsicmp(strValueType, _T("string"))) {
			strValueData.Replace(_T("{{"), _T("{"));
			strValueData.Replace(_T("\\"), _T("\\\\"));
			strValueData.Replace(_T("\""), _T("\\\""));
			line.Format(_T("\"%s\"=\"%s\"\r\n"), (LPCTSTR)strValueName, (LPCTSTR)strValueData);
		} else if (!_tcsicmp(strValueType, _T("expandsz"))) {
			strValueData.Replace(_T("{{"), _T("{"));
			strValueData.Replace(_T("\\"), _T("\\\\"));
			strValueData.Replace(_T("\""), _T("\\\""));
			line.Format(_T("\"%s\"=hex(2):%s\r\n"), (LPCTSTR)strValueName, (LPCTSTR)formatHexBytes(strValueData));
		} else if (!_tcsicmp(strValueType, _T("multisz"))) {
			CString formatted = formatMultiSz(strValueData); // Convert to hex(7):...
			line.Format(_T("\"%s\"=%s\r\n"), (LPCTSTR)strValueName, (LPCTSTR)formatted);
		} else if (!_tcsicmp(strValueType, _T("dword"))) {
			if (strValueData[0] == _T('$'))
				strValueData = strValueData.Mid(1);
			DWORD val = _tcstoul(strValueData, nullptr, 16);
			line.Format(_T("\"%s\"=dword:%08X\r\n"), (LPCTSTR)strValueName, val);
		} else if (!_tcsicmp(strValueType, _T("qword"))) {
			ULONGLONG val = _tcstoull(strValueData, nullptr, 16);
			CString tmp;
			for (int j = 0; j < 8; ++j) {
				tmp.AppendFormat(_T("%02X,"), (BYTE)((val >> (j * 8)) & 0xFF));
			}
			tmp.TrimRight(_T(","));
			line.Format(_T("\"%s\"=hex(b):%s\r\n"), (LPCTSTR)strValueName, (LPCTSTR)tmp);
		} else if (!_tcsicmp(strValueType, _T("binary"))) {
			strValueData.Replace(_T(" "), _T(","));
			line.Format(_T("\"%s\"=hex:%s\r\n"), (LPCTSTR)strValueName, (LPCTSTR)strValueData);
		}

		out += CString(line);
		out += _T("\r\n");
	}

	CTextFileWriter writer;
	if (!writer.Save(strFile, out, TextEncoding::UTF16_LE)) {
		CString txt = _L(_T("Error|CreateFile"), _T("Failed to save '%1'."));
		txt.Replace(_T("%1"), strFile);
		AtlMessageBox(m_hWnd, (LPCTSTR)txt, IDR_MAINFRAME, MB_OK | MB_ICONERROR);
	}

	return 0;
}

LRESULT CMainFrame::OnProjectImportMessages(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	// File type filters
	static const COMDLG_FILTERSPEC filters[] = {
		{ _T("Message Files (*.isl;*.iss)"), _T("*.isl;*.iss") },
		{ _T("All Files (*.*)"),             _T("*.*") }
	};

	// Create shell-style open dialog
	CShellFileOpenDialog dlg(
		NULL,
		FOS_FILEMUSTEXIST | FOS_PATHMUSTEXIST | FOS_FORCEFILESYSTEM,
		NULL,
		filters,
		_countof(filters)
	);

	if (dlg.DoModal(m_hWnd) != IDOK)
		return 0;

	CString strFilename;
	if (FAILED(dlg.GetFilePath(strFilename)))
		return 0;

	CWaitCursor wait;

	CTextFileReader reader;
	if (!reader.Load(strFilename)) {
		CString txt = _L(_T("Failed to open '%1'."));
		txt.Replace(_T("%1"), strFilename);
		AtlMessageBox(m_hWnd, (LPCTSTR)txt, IDR_MAINFRAME, MB_OK | MB_ICONERROR);
		return 0;
	}

	CAtlArray<CString> lines;
	reader.GetLines(lines);
	
	bool bInSection = false;
	bool bAdded = false;
	for (size_t i = 0; i < lines.GetCount(); ++i) {
		CString str = lines[i].Trim();
		if (str.IsEmpty() || str[0] == _T(';')) continue;

		if (str[0] == _T('[')) {
			if (!str.CompareNoCase(_T("[Messages]"))) {
				bInSection = true;
			} else {
				bInSection = false;
			}
		} else if (bInSection) {
			CScriptLine* pLine = NULL;
			try {
				pLine = new CInnoScript::CLineSetup(CInnoScript::SEC_MESSAGES, str);
				bAdded = true;
				if (CMyApp::m_prefs.m_bOverwriteMessages)
					m_document.GetScript().SetPropertyString(pLine->GetKey(), pLine->GetValue(), pLine->GetSection());
				else
					m_document.GetScript().AddLine(pLine);
				m_document.SetModifiedFlag();
			} catch (LPCTSTR ptr) {
				if (pLine) delete pLine;
				CString txt = _L(_T("Error|ParseMessages"), _T("Error parsing messages.\n\n%1"));
				txt.Replace(_T("%1"), ptr);
				AtlMessageBox(m_hWnd, (LPCTSTR)txt, IDR_MAINFRAME, MB_OK | MB_ICONERROR);
			}
		}
	}

	if (bAdded)
		m_document.UpdateAll();

	return 0;
}

#define GetCurrentFolder(a) a = _T("{app}")

LRESULT CMainFrame::OnProjectCreateDirectory(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	CString strFolder;
	GetCurrentFolder(strFolder);
	strFolder += _T("\\");

	CScriptList	list;

	CScriptLine* pLine = new CScriptLine(CInnoScript::SEC_DIRS);
	pLine->SetParameter(_T("Name"), strFolder);
	list.Add(pLine);

	if (!CSheets::ShowSheet(m_hWnd, list, true)) {
		delete pLine;
		return 0;
	}

	m_document.SetModifiedFlag();
	m_document.GetScript().AddLine(pLine);
	m_document.UpdateAll();

	return 0;
}

LRESULT CMainFrame::OnProjectCreateInternetShortcut(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	CDlgIEShortcut dlg;
	dlg.m_strFileName = _T("{app}\\");
	dlg.m_strFileName += m_document.GetScript().GetPropertyString(_T("AppName"));
	dlg.m_strFileName += _T(".url");

	dlg.m_strURL = _T("https://");
	dlg.m_strURL += m_document.GetScript().GetPropertyString(_T("AppName"));
	dlg.m_strURL += _T(".com/");
	dlg.m_strURL.MakeLower();

	if (dlg.DoModal(AfxGetMainWnd()) == IDOK) {
		CScriptLine* pLine = new CScriptLine(CInnoScript::SEC_INI);
		pLine->SetParameter(_T("Filename"), dlg.m_strFileName);
		pLine->SetParameter(_T("Section"), _T("InternetShortcut"));
		pLine->SetParameter(_T("Key"), _T("URL"));
		pLine->SetParameter(_T("String"), dlg.m_strURL);
		m_document.GetScript().AddLine(pLine);

		pLine = new CScriptLine(CInnoScript::SEC_UNINSTALLDELETE);
		pLine->SetParameter(_T("Type"), _T("files"));
		pLine->SetParameter(_T("Name"), dlg.m_strFileName);
		m_document.GetScript().AddLine(pLine);

		m_document.UpdateAll();
		m_document.SetModifiedFlag();
	}
	return 0;
}

/*
** Verifies that all files exists on the local computer.
*/
LRESULT CMainFrame::OnProjectVerifyFiles(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CWaitCursor		wait;
	CScriptList		list;
	CString			strSourceDir;

	bool bValidSourceDir = m_document.GetSourceDir(strSourceDir);
	if (bValidSourceDir) CMyUtils::EndWith(strSourceDir, _T('\\'));

	// Unselect all files if files section
	bool bFilesList = m_uCurrentView == ID_VIEW_FILES && CMyApp::m_prefs.m_bFilesList;
	CListViewCtrl m_wndFilesL(m_wndView);
	if (bFilesList) {
		UINT nCount = m_wndFilesL.GetItemCount();
		while (nCount--) m_wndFilesL.SetItemState(nCount, 0, LVIS_SELECTED);
	}

	m_document.GetScript().GetList(CInnoScript::SEC_FILES, list);
	CString strMessage;
	UINT nCount = 0;
	for (int nPos = 0; nPos < list.GetSize(); nPos++) {
		CScriptLine* pLine = list[nPos];
		CString strSource = pLine->GetParameter(_T("Source"));

		// Don't check empty entries and anything that uses constants
		if (!strSource.GetLength() || strSource[0] == _T('{')) continue;

		CString strFileName;
		m_document.GetScriptFileName(strFileName, strSource);

		if (!CMyUtils::FileExists(strFileName)) {
			//if(!CMyUtils::FileExists(strSource) && !CMyUtils::FileExists(strSourceDir+strSource)) {
			if (bFilesList) {
				UINT nCount = m_wndFilesL.GetItemCount();
				while (nCount--) {
					CScriptLine* pData = (CScriptLine*)m_wndFilesL.GetItemData(nCount);
					if (pData == pLine) {
						m_wndFilesL.SetItemState(nCount, LVIS_SELECTED, LVIS_SELECTED);
						break;
					}
				}
			}
			if (++nCount == 11) {
				strMessage += _T("\n\t...");
			} else if (nCount < 11) {
				if (strMessage.GetLength() > 0) strMessage += _T("\n");
				strMessage += _T("\t") + strSource;
			}
		}
	}
	if (strMessage.GetLength() > 0) {
		strMessage = _L(_T("FilesNotFound"), _T("The following files could not be found:\n")) + strMessage;
		if (bFilesList) strMessage += _L(_T("FilesSelected"), _T("\n\nThese files have been selected."));
		AtlMessageBox(m_hWnd, (LPCTSTR)strMessage, IDR_MAINFRAME, MB_OK | MB_ICONWARNING);
	} else {
		AtlMessageBox(m_hWnd, _L(_T("VerifiedOk"), _T("All checked files seem to exist.")), IDR_MAINFRAME);
	}
	return 0;
}

LRESULT CMainFrame::OnProjectLangOptions(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);
	CDlgLangOptions dlg(&m_document);
	if (dlg.DoModal(AfxGetMainWnd()) == IDOK) {
		m_document.UpdateAll();
		m_document.SetModifiedFlag();
	}
	return 0;
}

LRESULT CMainFrame::OnProjectExportMessages(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CScriptList	list;
	m_document.GetScript().GetList(CInnoScript::SEC_MESSAGES, list);
	if (list.GetSize() == 0) return 0;

	static const COMDLG_FILTERSPEC filters[] = {
		{ _T("Message Files (*.isl)"), _T("*.isl") },
		{ _T("All Files (*.*)"),       _T("*.*") }
	};

	CShellFileSaveDialog dlg(
		NULL, // optional default file name
		FOS_OVERWRITEPROMPT | FOS_PATHMUSTEXIST | FOS_FORCEFILESYSTEM,
		_T("isl"),
		filters,
		_countof(filters)
	);

	if (dlg.DoModal(m_hWnd) != IDOK)
		return 0;

	CString strFilename;
	if (FAILED(dlg.GetFilePath(strFilename)))
		return 0;

	CWaitCursor wait;

	CString content;
	content += _T("[Messages]\r\n");

	for (int nPos = 0; nPos < list.GetSize(); nPos++) {
		CScriptLine* pLine = list[nPos];
		CString strLine;
		pLine->Write(strLine.GetBuffer(8000), 8000);
		strLine.ReleaseBuffer();
		content += strLine;
		content += _T("\r\n");
	}

	CTextFileWriter writer;
	if (!writer.Save(strFilename, CString(content), TextEncoding::Auto)) {
		CString txt = _L(_T("Error|CreateFile"), _T("Failed to create '%1'."));
		txt.Replace(_T("%1"), strFilename);
		AtlMessageBox(m_hWnd, (LPCTSTR)txt, IDR_MAINFRAME, MB_OK | MB_ICONERROR);
		return 0;
	}

	return 0;
}

LRESULT CMainFrame::OnProjectCreateUninstallIcon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	// TODO: Sjekk at et slikt icon ikke finnes fra fшr
	CString strAppName = m_document.GetScript().GetPropertyString(_T("AppName"));
	if (strAppName.IsEmpty()) {
		AtlMessageBox(m_hWnd, _L(_T("NeedAppName"), _T("Please enter a value for application name in options first.")), IDR_MAINFRAME, MB_OK | MB_ICONWARNING);
		return 0;
	}

	CScriptLine* pLine = new CScriptLine(CInnoScript::SEC_ICONS);
	pLine->SetParameter(_T("Name"), _T("{group}\\{cm:UninstallProgram, ") + strAppName + _T("}"));
	pLine->SetParameter(_T("Filename"), _T("{uninstallexe}"));
	m_document.GetScript().AddLine(pLine);

	m_document.UpdateAll();
	m_document.SetModifiedFlag();
	AtlMessageBox(m_hWnd, _L(_T("Icon added.")), IDR_MAINFRAME);
	return 0;
}

LRESULT CMainFrame::OnProjectOptions(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// Allow all views to apply any changes not applied yet
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	int nPage = 0;

	if (wID != ID_PROJECT_OPTIONS) nPage = wID - ID_PROJECT_OPTIONS_COMPILER;

	{
		CMyPropertySheet	sheet(_L(_T("DialogTitles|SetupOptions"), _T("Setup Options")), nPage);

		CSetupCompiler		pageCompiler(&m_document, false, _L(_T("DialogTitles|Compiler"), _T("Compiler")));
		CSetupSpanning		pageSpanning(&m_document, false, _L(_T("DialogTitles|DiskSpanning"), _T("Disk Spanning")));
		CSetupApplication	pageApplication(&m_document, false, _L(_T("DialogTitles|Application"), _T("Application")));
		CSetupUserInfo		pageUserInfo(&m_document, false, _L(_T("DialogTitles|UserInfo"), _T("User Info")));
		CSetupAppDir		pageAppDir(&m_document, false, _L(_T("DialogTitles|AppDirectory"), _T("App Directory")));
		CSetupProgramGroup	pageGroup(&m_document, false, _L(_T("DialogTitles|ProgramGroup"), _T("Program Group")));
		CSetupPages			pagePages(&m_document, false, _L(_T("DialogTitles|Pages"), _T("Pages")));
		CSetupAppearance	pageAppearance(&m_document, false, _L(_T("DialogTitles|Appearance"), _T("Appearance")));
		CSetupUninstall		pageUninstall(&m_document, false, _L(_T("DialogTitles|Uninstall"), _T("Uninstall")));

		sheet.AddPage(pageCompiler);
		sheet.AddPage(pageSpanning);
		sheet.AddPage(pageApplication);
		sheet.AddPage(pageUserInfo);
		sheet.AddPage(pageAppDir);
		sheet.AddPage(pageGroup);
		sheet.AddPage(pagePages);
		sheet.AddPage(pageAppearance);
		sheet.AddPage(pageUninstall);

		int nRet = sheet.DoModal(AfxGetMainWnd());
		if (nRet == IDOK) m_document.SetModifiedFlag(TRUE);
	}

	m_document.UpdateAll();
	return 0;
}

LRESULT CMainFrame::OnProjectLogFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	m_document.UpdateAll(m_document.HINT_APPLYCHANGES);

	CDlgLogFile dlg(m_document.GetScript());
	if (dlg.DoModal(m_hWnd) == IDOK) m_document.SetModifiedFlag();

	m_document.UpdateAll();
	return 0;
}

LRESULT CMainFrame::OnProjectUninstall(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CWaitCursor wait;
	CInnoScriptEx& script = m_document.GetScript();

	if (!script.GetPropertyBool(_T("Uninstallable")))
		return 0;

	LPCTSTR pszKey = script.GetPropertyString(_T("AppID"));
	if (!pszKey || !*pszKey)
		return 0;

	const LPCTSTR pszPath = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%s_is1");
	CString strKey;
	strKey.Format(pszPath, pszKey);

	CRegKey reg;
	if (reg.Open(HKEY_CURRENT_USER, strKey, KEY_READ) != ERROR_SUCCESS)
		if (reg.Open(HKEY_LOCAL_MACHINE, strKey, KEY_READ) != ERROR_SUCCESS)
			return 0;

	ULONG nChars = MAX_PATH;
	CString strValue;
	if (reg.QueryStringValue(_T("UninstallString"), strValue.GetBuffer(nChars), &nChars) != ERROR_SUCCESS)
		return 0;

	strValue.ReleaseBuffer(nChars - 1);


	STARTUPINFO			si;
	PROCESS_INFORMATION	pi;
	DWORD				dwResult = -1;

	memset(&si, 0, sizeof si);
	si.cb = sizeof si;

	if (!CreateProcess(NULL, strValue.GetBuffer(MAX_PATH), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		return 0;
	}

	wait.Restore();
	CloseHandle(pi.hThread);
	WaitForInputIdle(pi.hProcess, INFINITE);
	WaitForSingleObject(pi.hProcess, INFINITE);
	GetExitCodeProcess(pi.hProcess, &dwResult);
	CloseHandle(pi.hProcess);

	return 0;
}