// MyDoc.cpp : implementation of the CMyDoc class
//

#include "stdafx.h"
#include "ISTool.h"
#include "MyDoc.h"
#include "SplitPath.h"
#include "StringToken.h"
#include "MainFrm.h"
#include "DlgCompile.h"

/////////////////////////////////////////////////////////////////////////////
// CMyDoc construction/destruction

CMyDoc::CMyDoc() : m_pScript(NULL), m_bModified(false), m_pMessages(NULL) {
	m_pScript = new CInnoScriptEx();
	m_pScript->Clear();

	m_nScriptLine = -1;
	m_bCompileAndExit = false;
}

CMyDoc::~CMyDoc() {
	if (m_pMessages) delete m_pMessages;
}

void CMyDoc::SetModifiedFlag(bool bModified/*=true*/) {
	m_bModified = bModified;
	SetPathName(GetPathName());
}

const CString& CMyDoc::GetPathName() {
	return m_strPathName;
}

void CMyDoc::ClearScript() {
	if (m_pScript) delete m_pScript;
	m_pScript = new CInnoScriptEx();
	m_pScript->Clear();
}

BOOL CMyDoc::OnNewDocument(HWND hWnd, bool bHideWizard) {
	if (!SaveModified(hWnd)) return FALSE;
	SetModifiedFlag(false);
	SetPathName(_T(""));
	ClearScript();

	bool bShowWizard = !bHideWizard && AfxGetMainWnd() && CMyApp::m_prefs.m_bShowNewWizard;
	if (bShowWizard && RunISWizard()) SetModifiedFlag(true);

	UpdateAll();	// Empty current view
	UpdateOther(HINT_INITIALIZE);

	return TRUE;
}

void CMyDoc::OnCloseDocument() {
	if (m_pScript) {
		delete m_pScript;
		m_pScript = NULL;
	}
}

BOOL CMyDoc::OnMergeDocument(HWND hWnd, LPCTSTR lpszPathName) {
	UpdateAll(HINT_APPLYCHANGES);
	if (VBIsVBFile(lpszPathName))
		VBLoadFile(lpszPathName);
	else
		GetScript().LoadScript(lpszPathName);
	UpdateAll();
	SetModifiedFlag(TRUE);
	return TRUE;
}

BOOL CMyDoc::OnOpenDocument(HWND hWnd, LPCTSTR lpszPathName) {
	if (!SaveModified(hWnd)) return FALSE;
	SetModifiedFlag(false);
	ClearScript();
	AfxGetMainWnd().m_wndView.SendMessage(WM_SHOWVIEW);
	UpdateAll();	// Empty current view

	if (VBIsVBFile(lpszPathName)) {
		VBLoadFile(lpszPathName);
	} else {
		if (!GetScript().LoadScript(lpszPathName))
			return FALSE;

		//CheckSourcePaths();

		/*
		** Scan through the script for obsolete features.
		** Current obsolete features it scans for is:
		**   isreadme flag: Creates entry in run section
		**   showcheckbox flag: Replaced with postinstall flag
		**   compresslevel is replaced with compression
		*/
		CScriptList	list;
		CString		strMessage;

		list.RemoveAll();
		GetScript().GetList(CInnoScript::SEC_RUN, list);
		for (int nPos = 0; nPos < list.GetSize(); nPos++) {
			CScriptLine* pLine = list[nPos];
			if (pLine->GetParameterFlag(_T("Flags"), _T("showcheckbox"))) {
				CString txt = _L(_T("Obsolete|ShowCheckbox"), _T("Replaced showcheckbox flag with postinstall flag for %1 inside the run section."));
				txt.Replace(_T("%1"), pLine->GetParameter(_T("FileName")));

				if (strMessage.GetLength() > 0) strMessage += _T("\n");
				strMessage += (LPCTSTR)txt;

				pLine->SetParameterFlag(_T("Flags"), _T("showcheckbox"), false);
				pLine->SetParameterFlag(_T("Flags"), _T("postinstall"), true);
			}
		}
		// CompressLevel, AlwaysCreateUninstallIcon, UninstallIconName
		list.RemoveAll();
		GetScript().GetList(CInnoScript::SEC_SETUP, list);
		for (int nPos = 0; nPos < list.GetSize(); nPos++) {
			CScriptLine* pLine = list[nPos];
			if (pLine->GetSection() == CInnoScript::SEC_SETUP) {
				if (!_tcsicmp(pLine->GetKey(), _T("CompressLevel"))) {
					if (_ttol(pLine->GetValue()) != 7) {
						CString str;
						str.Format(_T("zip/%d"), _ttol(pLine->GetValue()));
						GetScript().SetPropertyString(_T("Compression"), str);
					}
					GetScript().DeleteLine(pLine);

					if (strMessage.GetLength() > 0) strMessage += _T("\n");
					strMessage += _L(_T("Obsolete|CompressLevel"), _T("CompressLevel directive replaced with new Compression directive."));
				} else if (!_tcsicmp(pLine->GetKey(), _T("AlwaysCreateUninstallIcon"))) {
					GetScript().DeleteLine(pLine);

					if (strMessage.GetLength() > 0) strMessage += _T("\n");
					strMessage += _L(_T("Obsolete|AlwaysCreateUninstallIcon"), _T("AlwaysCreateUninstallIcon directive removed."));
				} else if (!_tcsicmp(pLine->GetKey(), _T("UninstallIconName"))) {
					GetScript().DeleteLine(pLine);

					if (strMessage.GetLength() > 0) strMessage += _T("\n");
					strMessage += _L(_T("Obsolete|UninstallIconName"), _T("UninstallIconName directive removed."));
				} else if (!_tcsicmp(pLine->GetKey(), _T("MessagesFile"))) {
					CScriptLine* pNewLine = new CScriptLine(CInnoScript::SEC_LANGUAGES);
					pNewLine->SetParameter(_T("Name"), _T("default"));
					pNewLine->SetParameter(_T("MessagesFile"), pLine->GetValue());
					GetScript().AddLine(pNewLine);

					GetScript().DeleteLine(pLine);

					if (strMessage.GetLength() > 0) strMessage += _T("\n");
					strMessage += _L(_T("Obsolete|MessagesFile"), _T("MessagesFile directive replaced with [Languages] section."));
				}
			}
		}

		// CopyMode
		list.RemoveAll();
		GetScript().GetList(CInnoScript::SEC_FILES, list);
		bool bCopyMode = false;
		bool bCompareTimeStampAlso = false;
		for (int nPos = 0; nPos < list.GetSize(); nPos++) {
			CScriptLine* pLine = list[nPos];
			CString tmp = pLine->GetParameter(_T("CopyMode"));
			if (!tmp.CompareNoCase(_T("alwaysskipifsameorolder"))) {
				// New default
				bCopyMode = true;
			} else if (!tmp.CompareNoCase(_T("onlyifdoesntexist"))) {
				pLine->SetParameterFlag(_T("Flags"), _T("onlyifdoesntexist"), true);
				bCopyMode = true;
			} else if (!tmp.CompareNoCase(_T("alwaysoverwrite"))) {
				pLine->SetParameterFlag(_T("Flags"), _T("ignoreversion"), true);
				bCopyMode = true;
			} else if (!tmp.CompareNoCase(_T("dontcopy"))) {
				pLine->SetParameterFlag(_T("Flags"), _T("dontcopy"), true);
				bCopyMode = true;
			} else if (!tmp.CompareNoCase(_T("normal"))) {
				// normal, old default
				pLine->SetParameterFlag(_T("Flags"), _T("promptifolder"), true);
				bCopyMode = true;
			}
			pLine->DeleteParameter(_T("CopyMode"));
			if (pLine->GetParameterFlag(_T("Flags"), _T("comparetimestampalso"))) {
				bCompareTimeStampAlso = true;
				pLine->SetParameterFlag(_T("Flags"), _T("comparetimestampalso"), false);
				pLine->SetParameterFlag(_T("Flags"), _T("comparetimestamp"), true);
			}
		}
		if (bCopyMode) {
			if (strMessage.GetLength() > 0) strMessage += _T("\n");
			strMessage += _L(_T("Obsolete|CopyMode"), _T("Replaced deprecated parameter CopyMode with corresponding flags [IS 3.0.5]."));
		}
		if (bCompareTimeStampAlso) {
			if (strMessage.GetLength() > 0) strMessage += _T("\n");
			strMessage += _L(_T("Obsolete|CompareTimeStampAlso"), _T("Replaced old flag comparetimestampalso with the new comparetimestamp [IS 3.0.5]."));
		}

		// Tell the user what has been altered in the script
		if (strMessage.GetLength() > 0) AtlMessageBox(hWnd, (LPCTSTR)strMessage, IDR_MAINFRAME, MB_OK | MB_ICONWARNING);

		SetPathName(lpszPathName);
	}

	// More initializations
	UpdateAll();
	AfxGetMainWnd().m_wndView.SendMessage(WM_SHOWVIEW);
	UpdateOther(HINT_INITIALIZE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMyDoc commands

BOOL CMyDoc::GetCompiler(CString& str, bool bDLL/*=false*/, bool bISPP/*=false*/) {
	CString strFolder;
	strFolder = CMyApp::m_prefs.m_strInnoFolder;
	if (strFolder.IsEmpty()) {
		CString txt = _L(_T("IS4NotFound"), _T("This project uses Inno Setup, but\nthe Inno Setup Folder can't be found.\n\nCheck the preferences."));
		AtlMessageBox(AfxGetMainWnd(), (LPCTSTR)txt, IDR_MAINFRAME, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	CString strTemp(strFolder);
	CMyUtils::EndWith(strTemp, __T('\\'));
	if (bDLL) {
		if (bISPP)
			strTemp += _T("ISPP.dll");
		else
			strTemp += _T("ISCmplr.dll");
	} else
		strTemp += _T("compil32.exe");

	if (strFolder.IsEmpty() || !CMyUtils::IsFile(strTemp)) {
		CString txt = _L(_T("IS4NotFound"), _T("This project uses Inno Setup, but\nthe Inno Setup Folder can't be found.\n\nCheck the preferences."));
		AtlMessageBox(AfxGetMainWnd(), (LPCTSTR)txt, IDR_MAINFRAME, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	str = strTemp;
	return TRUE;
}

void GenerateTempFileName(CString& str) {
	TCHAR szPath[MAX_PATH + 1];

	GetTempPath(MAX_PATH, szPath);
	GetTempFileName(szPath, _T("x"), 0, str.GetBuffer(MAX_PATH + 1));
	str.ReleaseBuffer();
}

bool CMyDoc::RunISWizard() {
	CString strFolder;
	strFolder = CMyApp::m_prefs.m_strInnoFolder;

	if (strFolder.IsEmpty()) return false;

	CString strTempFile;
	GenerateTempFileName(strTempFile);

	CString strTemp, strParam;
	strTemp.Format(_T("%s\\compil32.exe"), (LPCTSTR)strFolder);
	strParam.Format(_T("/wizard \"Inno Setup Script Wizard\" \"%s\""), (LPCTSTR)strTempFile);
	ShowWindow(AfxGetMainWnd(), SW_HIDE);
	DWORD dwResult = CMyApp::MyExec(strTemp, strParam);
	ShowWindow(AfxGetMainWnd(), SW_SHOW);

	if (dwResult == 0) {
		GetScript().Clear();
		GetScript().LoadScript(strTempFile);
	}

	DeleteFile(strTempFile);
	return dwResult == 0;
}

bool CMyDoc::GetCompilerPath(CString& strFolder) {
	strFolder = CMyApp::m_prefs.m_strInnoFolder;

	if (strFolder.IsEmpty()) return false;

	if (!CMyUtils::IsDirectory(strFolder)) return false;
	return true;
}

BOOL CMyDoc::GetMessageFile(CString& str) {
	if (CMyApp::m_prefs.m_strInnoFolder.IsEmpty()) return FALSE;

	CString strTemp;
	strTemp.Format(_T("%s\\Default.isl"), (LPCTSTR)CMyApp::m_prefs.m_strInnoFolder);

	if (!CMyUtils::IsFile(strTemp)) return FALSE;
	str = strTemp;
	return TRUE;
}

void CMyDoc::AddDirs(CComboBox& ref) {
	CScriptList	dirs;
	GetScript().GetList(CInnoScript::SEC_DIRS, dirs);

	for (int nPos = 0; nPos < dirs.GetSize(); nPos++) {
		CScriptLine* pItem = dirs[nPos];
		CString strName(pItem->GetParameter(_T("Name")));
		if (ref.FindString(-1, strName) == CB_ERR)
			ref.AddString(strName);
	}
}

void CMyDoc::AddDirConstants(CComboBox& ref) {
	UINT nPos = 0;
	while (CInnoScriptEx::m_constants[nPos].m_pszConstant) {
		if (CInnoScriptEx::m_constants[nPos].m_bFolder)
			ref.AddString(CInnoScriptEx::m_constants[nPos].m_pszConstant);
		nPos++;
	}
}

void CMyDoc::AddFiles(CComboBox& ref) {
	CScriptList	files;
	CInnoScriptEx& script = GetScript();
	script.GetList(CInnoScript::SEC_FILES, files);

	// Init storage if many items
	if (files.GetSize() > 100) {
		ref.InitStorage(files.GetSize(), MAX_PATH);
	}

	for (int nPos = 0; nPos < files.GetSize(); nPos++) {
		CScriptLine* pItem = files[nPos];
		CString str;
		script.GetDestName(pItem, str);
		// Next line commented out to speed up large scripts
//		if(/*!str.Right(4).CompareNoCase(".exe") &&*/ ref.FindString(-1,str)==CB_ERR)
		ref.AddString(str);
	}
}

void CMyDoc::AddIniFilenames(CComboBox& ref) {
	CScriptList	ini;
	GetScript().GetList(CInnoScript::SEC_INI, ini);

	for (int nPos = 0; nPos < ini.GetSize(); nPos++) {
		CScriptLine* pItem = ini[nPos];
		CString strFilename(pItem->GetParameter(_T("Filename")));
		if (ref.FindString(-1, strFilename) == CB_ERR)
			ref.AddString(strFilename);
	}
}

void CMyDoc::AddIniSections(CComboBox& ref) {
	CScriptList	ini;
	GetScript().GetList(CInnoScript::SEC_INI, ini);

	for (int nPos = 0; nPos < ini.GetSize(); nPos++) {
		CScriptLine* pItem = ini[nPos];
		CString strSection(pItem->GetParameter(_T("Section")));
		if (ref.FindString(-1, strSection) == CB_ERR)
			ref.AddString(strSection);
	}
}

void CMyDoc::AddRegistrySubkeys(CComboBox& ref) {
	CScriptList	registry;
	GetScript().GetList(CInnoScript::SEC_REGISTRY, registry);

	for (int nPos = 0; nPos < registry.GetSize(); nPos++) {
		CScriptLine* pItem = registry[nPos];
		CString strSubkey(pItem->GetParameter(_T("Subkey")));
		if (ref.FindString(-1, strSubkey) == CB_ERR)
			ref.AddString(strSubkey);
	}
}

void CMyDoc::AddFilesExeFiles(CComboBox& ref) {
	CScriptList	files;
	GetScript().GetList(CInnoScript::SEC_FILES, files);

	for (int nPos = 0; nPos < files.GetSize(); nPos++) {
		CScriptLine* pItem = files[nPos];
		CString str;
		GetScript().GetDestName(pItem, str);

		if (str.Right(4).CompareNoCase(_T(".exe"))) continue;
		if (ref.FindString(-1, str) == CB_ERR)
			ref.AddString(str);
	}
}

BOOL CMyDoc::OnSaveDocument(HWND hWnd, LPCTSTR lpszPathName) {
	UpdateAll(HINT_APPLYCHANGES);
	BOOL bRet = GetScript().WriteScript(lpszPathName);
	if (bRet) {
		SetModifiedFlag(false);
		AfxGetMainWnd().AddToRecentFileList(lpszPathName);
	} else {
		CMyUtils::ShowSysMsg(GetLastError());
		bRet = TRUE;
	}
	return bRet;
}

void CMyDoc::RenameFile(HWND hWnd, LPCTSTR lpszFrom, LPCTSTR lpszTo) {
	// Icons, Delete*2, Run*2
	UINT nCount = 0;

	CScriptList	list;
	int nPos;

	GetScript().GetList(CInnoScript::SEC_ICONS, list);
	for (nPos = 0; nPos < list.GetSize(); nPos++) {
		CScriptLine* pItem = list[nPos];
		LPCTSTR psz = pItem->GetParameter(_T("Filename"));
		if (psz && !_tcsicmp(psz, lpszFrom))
			nCount++;
	}

	list.RemoveAll();
	GetScript().GetList(CInnoScript::SEC_INSTALLDELETE, list);
	for (nPos = 0; nPos < list.GetSize(); nPos++) {
		CScriptLine* pItem = list[nPos];
		LPCTSTR psz = pItem->GetParameter(_T("Name"));
		if (psz && !_tcsicmp(psz, lpszFrom))
			nCount++;
	}

	list.RemoveAll();
	GetScript().GetList(CInnoScript::SEC_UNINSTALLDELETE, list);
	for (nPos = 0; nPos < list.GetSize(); nPos++) {
		CScriptLine* pItem = list[nPos];
		LPCTSTR psz = pItem->GetParameter(_T("Name"));
		if (psz && !_tcsicmp(psz, lpszFrom))
			nCount++;
	}

	list.RemoveAll();
	GetScript().GetList(CInnoScript::SEC_RUN, list);
	for (nPos = 0; nPos < list.GetSize(); nPos++) {
		CScriptLine* pItem = list[nPos];
		LPCTSTR psz = pItem->GetParameter(_T("Filename"));
		if (psz && !_tcsicmp(psz, lpszFrom))
			nCount++;
	}

	list.RemoveAll();
	GetScript().GetList(CInnoScript::SEC_UNINSTALLRUN, list);
	for (nPos = 0; nPos < list.GetSize(); nPos++) {
		CScriptLine* pItem = list[nPos];
		LPCTSTR psz = pItem->GetParameter(_T("Filename"));
		if (psz && !_tcsicmp(psz, lpszFrom))
			nCount++;
	}

	if (nCount > 0) {
		CString txt = _L(_T("FileIsReferenced"), _T("The file '%1' is referenced %2 times.\nChange these references to '%3'?"));
		CString str;
		str.Format(_T("%d"), nCount);
		txt.Replace(_T("%1"), lpszFrom);
		txt.Replace(_T("%2"), str);
		txt.Replace(_T("%3"), lpszTo);
		if (AtlMessageBox(hWnd, (LPCTSTR)txt, IDR_MAINFRAME, MB_YESNO | MB_ICONQUESTION) != IDYES)
			return;
	}

	list.RemoveAll();
	GetScript().GetList(CInnoScript::SEC_ICONS, list);
	for (nPos = 0; nPos < list.GetSize(); nPos++) {
		CScriptLine* pItem = list[nPos];
		LPCTSTR psz = pItem->GetParameter(_T("Filename"));
		if (psz && !_tcsicmp(psz, lpszFrom))
			pItem->SetParameter(_T("Filename"), lpszTo);
	}

	list.RemoveAll();
	GetScript().GetList(CInnoScript::SEC_INSTALLDELETE, list);
	for (nPos = 0; nPos < list.GetSize(); nPos++) {
		CScriptLine* pItem = list[nPos];
		LPCTSTR psz = pItem->GetParameter(_T("Name"));
		if (psz && !_tcsicmp(psz, lpszFrom))
			pItem->SetParameter(_T("Name"), lpszTo);
	}

	list.RemoveAll();
	GetScript().GetList(CInnoScript::SEC_UNINSTALLDELETE, list);
	for (nPos = 0; nPos < list.GetSize(); nPos++) {
		CScriptLine* pItem = list[nPos];
		LPCTSTR psz = pItem->GetParameter(_T("Name"));
		if (psz && !_tcsicmp(psz, lpszFrom))
			pItem->SetParameter(_T("Name"), lpszTo);
	}

	list.RemoveAll();
	GetScript().GetList(CInnoScript::SEC_RUN, list);
	for (nPos = 0; nPos < list.GetSize(); nPos++) {
		CScriptLine* pItem = list[nPos];
		LPCTSTR psz = pItem->GetParameter(_T("Filename"));
		if (psz && !_tcsicmp(psz, lpszFrom))
			pItem->SetParameter(_T("Filename"), lpszTo);
	}

	list.RemoveAll();
	GetScript().GetList(CInnoScript::SEC_UNINSTALLRUN, list);
	for (nPos = 0; nPos < list.GetSize(); nPos++) {
		CScriptLine* pItem = list[nPos];
		LPCTSTR psz = pItem->GetParameter(_T("Filename"));
		if (psz && !_tcsicmp(psz, lpszFrom))
			pItem->SetParameter(_T("Filename"), lpszTo);
	}
}

void CMyDoc::RenameDir(LPCTSTR lpszFrom, LPCTSTR lpszTo) {
#ifdef TODO
	POSITION pos;
	UINT lenFrom = _tcslen(lpszFrom);
	UINT nCount = 0;
	CScriptFileList	listFiles;	// Store referencing files here
	CScriptDirList	listDirs;	// Store referencing dirs here
	CScriptIconList	listIcons;	// Store referencing icons here
	CScriptDeleteList	listDelete;	// Store referencing delete entries here
	CScriptRunList	listRun;	// Store referencing run entries here

	// Files (m_strDestDir)
	for (pos = GetScript().m_files.GetHeadPosition(); pos;) {
		CScriptFile* pItem = GetScript().m_files.GetNext(pos);
		if (!pItem->m_strDestDir.Left(lenFrom).CompareNoCase(lpszFrom)) {
			nCount++;
			listFiles.Add(pItem);
		}
	}
	// Dirs (m_strName)
	for (pos = GetScript().m_dirs.GetHeadPosition(); pos;) {
		CScriptDir* pItem = GetScript().m_dirs.GetNext(pos);
		if (!pItem->m_strName.Left(lenFrom).CompareNoCase(lpszFrom)) {
			if (!pItem->m_strName.CompareNoCase(lpszTo)) continue;
			nCount++;
			listDirs.Add(pItem);
		}
	}
	// Icons (m_strWorkingDir)
	for (pos = GetScript().m_icons.GetHeadPosition(); pos;) {
		CScriptIcon* pItem = GetScript().m_icons.GetNext(pos);
		if (!pItem->m_strWorkingDir.Left(lenFrom).CompareNoCase(lpszFrom)) {
			nCount++;
			listIcons.Add(pItem);
		}
	}
	// InstallDelete (m_strName)
	for (pos = GetScript().m_installdelete.GetHeadPosition(); pos;) {
		CScriptDelete* pItem = GetScript().m_installdelete.GetNext(pos);
		if (!pItem->m_strName.Left(lenFrom).CompareNoCase(lpszFrom)) {
			nCount++;
			listDelete.Add(pItem);
		}
	}
	// UninstallDelete (m_strName)
	for (pos = GetScript().m_uninstalldelete.GetHeadPosition(); pos;) {
		CScriptDelete* pItem = GetScript().m_uninstalldelete.GetNext(pos);
		if (!pItem->m_strName.Left(lenFrom).CompareNoCase(lpszFrom)) {
			nCount++;
			listDelete.Add(pItem);
		}
	}
	// InstallRun (m_strFilename,m_strWorkingDir)
	for (pos = GetScript().m_run.GetHeadPosition(); pos;) {
		CScriptRun* pItem = GetScript().m_run.GetNext(pos);
		if (!pItem->m_strFilename.Left(lenFrom).CompareNoCase(lpszFrom)
			|| !pItem->m_strWorkingDir.Left(lenFrom).CompareNoCase(lpszFrom)) {
			nCount++;
			listRun.Add(pItem);
		}
	}
	// UninstallRun (m_strFilename,m_strWorkingDir)
	for (pos = GetScript().m_uninstallrun.GetHeadPosition(); pos;) {
		CScriptRun* pItem = GetScript().m_uninstallrun.GetNext(pos);
		if (!pItem->m_strFilename.Left(lenFrom).CompareNoCase(lpszFrom)
			|| !pItem->m_strWorkingDir.Left(lenFrom).CompareNoCase(lpszFrom)) {
			nCount++;
			listRun.Add(pItem);
		}
	}

	if (!nCount) return;

	if (nCount > 0) {
		CString str;
		str.Format(_T("The directory %s is referenced %d times.\n")
			_T("Change these references to %s?"), lpszFrom, nCount, lpszTo);
		int nRet = AfxMessageBox(str, MB_YESNO);
		if (nRet != IDYES) return;
	}

	// Change files
	for (pos = listFiles.GetHeadPosition(); pos;) {
		CScriptFile* pItem = listFiles.GetNext(pos);
		pItem->m_strDestDir.Replace(lpszFrom, lpszTo);
	}
	// Change dirs
	for (pos = listDirs.GetHeadPosition(); pos;) {
		CScriptDir* pItem = listDirs.GetNext(pos);
		pItem->m_strName.Replace(lpszFrom, lpszTo);
	}
	// Change icons
	for (pos = listIcons.GetHeadPosition(); pos;) {
		CScriptIcon* pItem = listIcons.GetNext(pos);
		pItem->m_strWorkingDir.Replace(lpszFrom, lpszTo);
	}
	// Change delete
	for (pos = listDelete.GetHeadPosition(); pos;) {
		CScriptDelete* pItem = listDelete.GetNext(pos);
		pItem->m_strName.Replace(lpszFrom, lpszTo);
	}
	// Change run
	for (pos = listRun.GetHeadPosition(); pos;) {
		CScriptRun* pItem = listRun.GetNext(pos);
		pItem->m_strFilename.Replace(lpszFrom, lpszTo);
		pItem->m_strWorkingDir.Replace(lpszFrom, lpszTo);
	}
	UpdateAllViews(NULL, HINT_REFRESHTEXT);
#endif
}

void CMyDoc::AddValidMessages(HWND hWnd, CComboBox& wndCombo) {
	if (CMyApp::m_prefs.m_strInnoFolder.IsEmpty())
		return;

	CString strFolder(CMyApp::m_prefs.m_strInnoFolder);
	CMyUtils::EndWith(strFolder, _T('\\'));

	wndCombo.ResetContent();

	LPCTSTR pszMessagesFile = GetScript().GetPropertyString(_T("MessagesFile"));

	if (m_pMessages) delete m_pMessages;
	m_pMessages = new CInnoScriptEx;

	CStringToken tok(pszMessagesFile, _T(","));
	while (pszMessagesFile = tok.GetNext()) {
		CString strFile(pszMessagesFile);
		strFile.Replace(_T("compiler:"), strFolder);

#if 1
		if (!CMyUtils::IsFile(strFile)) {
			CString strAlt;
			GetSourceDir(strAlt);
			CMyUtils::EndWith(strAlt, _T('\\'));
			strAlt += strFile;

			if (!CMyUtils::IsFile(strAlt)) {
				CString txt = _L(_T("Failed to open '%1'."));
				txt.Replace(_T("%1"), strFile);
				AtlMessageBox(hWnd, (LPCTSTR)txt, IDR_MAINFRAME, MB_OK | MB_ICONERROR);
				continue;
				//AfxThrowUserException();
			}
			strFile = strAlt;
		}
		m_pMessages->LoadScript(strFile);
#else
		CStdioFile file;
		if (!file.Open(strFile, CFile::modeRead | CFile::typeText)) {
			CString strAlt;
			GetSourceDir(strAlt);
			CMyUtils::EndWith(strAlt, _T('\\'));
			strAlt += strFile;
			if (!file.Open(strAlt, CFile::modeRead | CFile::typeText)) {
				AfxMessageBox(_T("Failed to open ") + strFile);
				AfxThrowUserException();
			}
		}

		CString strLine;
		UINT nLine = 0;
		try {
			bool bInSection = false;
			while (file.ReadString(strLine)) {
				nLine++;
				strLine.TrimLeft(); strLine.TrimRight();
				if (strLine.IsEmpty() || strLine[0] == _T(';')) continue;

				if (strLine[0] == _T('[')) {
					if (!strLine.CompareNoCase(_T("[Messages]"))) bInSection = true;
					else bInSection = false;
					continue;
				}

				if (!bInSection) continue;
				CInnoScript::CLineSetup msg(CInnoScript::SEC_MESSAGES, strLine);
				if (wndCombo.FindString(-1, msg.GetKey()) == CB_ERR)
					wndCombo.AddString(msg.GetKey());
			}
		} catch (LPCTSTR ptr) {
			CString err;
			err.Format(_T("Error parsing script at line #%d:\n\nFile: %s\n\n%s"), nLine, strFile, ptr);
			AfxMessageBox(err);
			AfxThrowUserException();
		}
#endif
	}
	// Insert the messages
	if (m_pMessages) {
		CScriptList	list;
		m_pMessages->GetList(CInnoScript::SEC_MESSAGES, list);
		for (int nPos = 0; nPos < list.GetSize(); nPos++) {
			CScriptLine* pLine = list[nPos];
			wndCombo.AddString(pLine->GetKey());
		}
		//wndCombo.AddString(msg.GetKey());
	}
}

void CMyDoc::OpenInnoSetup(HWND hWnd) {
	if (GetPathName().IsEmpty()) {
		AtlMessageBox(hWnd, _L(_T("The script must be saved first.")), IDR_MAINFRAME, MB_OK | MB_ICONERROR);
		return;
	}

	CString strCompiler;
	if (!GetCompiler(strCompiler))
		return;

	if (IsModified()) {
		int nRet = AtlMessageBox(hWnd, _L(_T("ScriptModifiedSaveChanges"), _T("The script is modified.\nSave script before opening compiler?")), IDR_MAINFRAME, MB_YESNOCANCEL | MB_ICONQUESTION);
		if (nRet == IDCANCEL) return;
		if (nRet == IDYES) {
			if (!OnSaveDocument(hWnd, GetPathName()))
				return;
		}
	}
	CString strParam;
	strParam.Format(_T("\"%s\""), (LPCTSTR)GetPathName());

	CWaitCursor wait;
	/*DWORD dwCode =*/ CMyApp::MyExec(strCompiler, strParam, NULL, false);
}

bool CMyDoc::RunCompileSteps(CInnoScript::SECTION sec) {
	CScriptList	steps;
	GetScript().GetList(sec, steps);

	CString strOutputDir;
	try {
		GetOutputFolder(strOutputDir);
	} catch (...) {
		return false;
	}

	for (int nPos = 0; nPos < steps.GetSize(); nPos++) {
		CScriptLine* pLine = steps[nPos];
		if (pLine->GetComment()) continue;

		CString strName = pLine->GetParameter(_T("Name"));
		CString strParam = pLine->GetParameter(_T("Parameters"));

		bool bMinimized = pLine->GetParameterFlag(_T("Flags"), _T("runminimized"));
		bool bAbortOnError = pLine->GetParameterFlag(_T("Flags"), _T("abortonerror"));

		strParam.Replace(_T("{script}"), GetPathName());
		strParam.Replace(_T("{outputdir}"), strOutputDir);
		if (CMyApp::MyExec(strName, strParam, NULL, true, bMinimized) && bAbortOnError)
			return false;
	}
	return true;
}

void CMyDoc::GetOutputFolder(CString& rStr) {
	// First check the script
	rStr = m_pScript->GetPropertyString(_T("OutputDir"));
	// Next, check source file directory
	if (rStr.IsEmpty()) {
		rStr = GetPathName();
		int nPos = rStr.ReverseFind(_T('\\'));
		if (nPos < 0) nPos = rStr.ReverseFind(_T('/'));
		if (nPos >= 0)
			rStr = rStr.Left(nPos);
		CMyUtils::EndWith(rStr, _T('\\'));
		rStr += _T("Output");
	} else {
		CString strTmp;
		GetScriptFileName(strTmp, rStr);
		rStr = strTmp;
	}
}

bool CMyDoc::GetOutputExe(CInnoScriptEx* m_pScript, HWND hWnd, CString& rStr, bool bNoWarning/*=false*/) {
	CString strFile(m_pScript->GetPropertyString(_T("OutputExeFilename"), CInnoScript::PRJ_ISTOOL));
	if (!strFile.IsEmpty()) {
		rStr = strFile;
		return true;
	}

	strFile = m_pScript->GetPropertyString(_T("OutputBaseFilename"));
	if (strFile.IsEmpty()) strFile = _T("setup");

	GetOutputFolder(rStr);
	CMyUtils::EndWith(rStr, _T('\\'));
	rStr += strFile + _T(".exe");
	if (!bNoWarning && !CMyUtils::IsFile(rStr)) {
		CString txt = _L(_T("Output file '%1' doesn't exist."));
		txt.Replace(_T("%1"), rStr);
		ShowWindow(AfxGetMainWnd(), SW_RESTORE);
		AtlMessageBox(hWnd, (LPCTSTR)txt, IDR_MAINFRAME, MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

void CMyDoc::AddFileIfDoesntExist(CScriptLine* pLine) {
	// Scan files and don't add if source and destdir is similar
	CScriptList files;
	GetScript().GetList(CInnoScript::SEC_FILES, files);

	for (int nPos = 0; nPos < files.GetSize(); nPos++) {
		CScriptLine* p2 = files[nPos];

		if (!_tcsicmp(SAFESTR(pLine->GetParameter(_T("Source"))), SAFESTR(p2->GetParameter(_T("Source"))))
			&& !_tcsicmp(SAFESTR(pLine->GetParameter(_T("DestDir"))), SAFESTR(p2->GetParameter(_T("DestDir"))))) {

			delete pLine;
			return;
		}
	}

	GetScript().AddLine(pLine);
	SetModifiedFlag();
}

void CMyDoc::OnUpdate(LONG lHint, void* pParam) {}

void CMyDoc::SetPathName(const CString& strPathName) {
	m_strPathName = strPathName;

	CString strTitle, strTmp;
	VERIFY(strTitle.LoadString(WTL_IDS_APP_TITLE));
	if (m_strPathName.IsEmpty())
		strTmp = _T("Untitled");
	else
		strTmp += m_strPathName;

	if (IsModified())
		strTmp += _T("*");

	SetWindowText(AfxGetMainWnd(), strTmp + _T(" - ") + strTitle);
}

bool CMyDoc::SaveModified(HWND hWnd) {
	if (!IsModified())
		return true;        // ok to continue

	// get name/title of document
	CString name;
	if (m_strPathName.IsEmpty()) {
		// get name based on caption
		//name = m_strTitle;
		//if (name.IsEmpty())
		name = _L(_T("System|Untitled"), _T("Untitled"));
	} else {
		// get name based on file title of path name
		TCHAR szTitle[_MAX_PATH];
		AfxGetFileTitle(m_strPathName, szTitle, _countof(szTitle));
		name = szTitle;
	}

	CString txt = _L(_T("Save changes to '%1'?"));
	txt.Replace(_T("%1"), name);

	switch (AtlMessageBox(hWnd, (LPCTSTR)txt, IDR_MAINFRAME, MB_YESNOCANCEL | MB_ICONQUESTION)) {
	case IDCANCEL:
		return false; // don't continue

	case IDYES:
		// If so, either Save or Update, as appropriate
		if (!DoFileSave(hWnd))
			return false; // don't continue
		break;

	case IDNO:
		// If not saving changes, revert the document
		break;

	default:
		ATLASSERT(FALSE);
		break;
	}
	return true; // keep going
}

bool CMyDoc::DoFileSave(HWND hWnd) {
	DWORD dwAttrib = GetFileAttributes(m_strPathName);
	if (dwAttrib & FILE_ATTRIBUTE_READONLY) {
		// we do not have read-write access or the file does not (now) exist
		if (!DoSave(hWnd, NULL)) {
			ATLTRACE(_T("Warning: File save with new name failed.\n"));
			return false;
		}
	} else {
		if (!DoSave(hWnd, m_strPathName)) {
			ATLTRACE(_T("Warning: File save failed.\n"));
			return false;
		}
	}
	return true;
}

bool CMyDoc::DoSave(HWND hWnd, LPCTSTR lpszPathName, bool bReplace/*=true*/)
// Save the document data to a file
// lpszPathName = path name where to save document file
// if lpszPathName is NULL then the user will be prompted (SaveAs)
// note: lpszPathName can be different than 'm_strPathName'
// if 'bReplace' is TRUE will change file name if successful (SaveAs)
// if 'bReplace' is FALSE will not change path name (SaveCopyAs)
{
	CString newName = lpszPathName;
	if (newName.IsEmpty()) {
		newName = GetPathName();
		if (!DoPrompt(hWnd, newName, false, bReplace ? WTL_IDS_SAVEFILE : WTL_IDS_SAVEFILECOPY))
			return false;
	}

	CWaitCursor wait;

	if (!OnSaveDocument(hWnd, newName)) {
		if (lpszPathName == NULL)
			// be sure to delete the file
			DeleteFile(newName);

		return false;
	}

	// reset the title and change the document name
	if (bReplace)
		SetPathName(newName);

	return true;        // success
}

bool CMyDoc::DoPrompt(HWND hWnd, CString& newName, bool bOpen, UINT nID)
{
	const COMDLG_FILTERSPEC g_filterOpen[] = {
		{ _T("Supported Files"),      _T("*.iss;*.lst") },
		{ _T("Inno Setup Scripts"),   _T("*.iss") },
		{ _T("VB Setup Files"),       _T("*.lst") },
		{ _T("All Files"),            _T("*.*") }
	};

	const COMDLG_FILTERSPEC g_filterSave[] = {
		{ _T("Inno Setup Scripts (*.iss)"), _T("*.iss") },
		{ _T("All Files (*.*)"),            _T("*.*") }
	};

	// Load dialog title from resources
	CString title;
	if (!title.LoadString(nID))
		return false;

	// Retrieve initial folder path
	CString initialDir = CMyApp::m_prefs.m_strScriptFolder;

	int nResult = IDCANCEL;

	if (bOpen) {
		// Construct shell file open dialog
		CShellFileOpenDialog dlg(
			newName,
			FOS_FILEMUSTEXIST | FOS_PATHMUSTEXIST | FOS_FORCEFILESYSTEM,
			_T("iss"),
			g_filterOpen,
			_countof(g_filterOpen)
		);

		// Set title manually via COM pointer
		if (IFileDialog* pFD = dlg.GetPtr()) {
			pFD->SetTitle(title);

			if (!initialDir.IsEmpty()) {
				CComPtr<IShellItem> psiFolder;
				if (SUCCEEDED(SHCreateItemFromParsingName(initialDir, NULL, IID_PPV_ARGS(&psiFolder))))
					pFD->SetDefaultFolder(psiFolder);
			}
		}

		nResult = dlg.DoModal(hWnd);
		if (nResult == IDOK && FAILED(dlg.GetFilePath(newName)))
			return false;
	} else {
		// Construct shell file save dialog
		CShellFileSaveDialog dlg(
			newName,
			FOS_OVERWRITEPROMPT | FOS_PATHMUSTEXIST | FOS_FORCEFILESYSTEM,
			_T("iss"),
			g_filterSave,
			_countof(g_filterSave)
		);

		// Set title and initial folder
		if (IFileDialog* pFD = dlg.GetPtr()) {
			pFD->SetTitle(title);

			if (!initialDir.IsEmpty()) {
				CComPtr<IShellItem> psiFolder;
				if (SUCCEEDED(SHCreateItemFromParsingName(initialDir, NULL, IID_PPV_ARGS(&psiFolder))))
					pFD->SetDefaultFolder(psiFolder);
			}
		}

		nResult = dlg.DoModal(hWnd);
		if (nResult == IDOK && FAILED(dlg.GetFilePath(newName)))
			return false;
	}

	if (nResult != IDOK)
		return false;

	// Update stored script directory path
	int nPos = newName.ReverseFind(L'\\');
	if (nPos > 0)
		CMyApp::m_prefs.m_strScriptFolder = newName.Left(nPos);

	return true;
}

bool CMyDoc::GetSourceDir(CString& rStr) {
	CString str(GetScript().GetPropertyString(_T("SourceDir")));
	if (str.IsEmpty() && GetPathName().IsEmpty())
		return false;

	if (str.IsEmpty()) {
		str = GetPathName();
		int nPos = str.ReverseFind(_T('\\'));
		if (nPos < 0)
			nPos = str.ReverseFind(_T('/'));
		if (nPos < 0) return false;

		rStr = str.Left(nPos);
	} else if (CMyUtils::IsRelativePath(str)) {
		rStr = GetPathName();
		int nPos = rStr.ReverseFind(_T('\\'));
		if (nPos < 0)
			nPos = rStr.ReverseFind(_T('/'));
		if (nPos < 0) return false;

		rStr = rStr.Left(nPos);
		CMyUtils::EndWith(rStr, _T('\\'));
		if (str[0] == _T('\\') && rStr.GetLength() >= 2) {
			if (rStr[1] == _T(':'))
				rStr.ReleaseBuffer(2);
			else if (rStr.GetLength() > 2)
				rStr.ReleaseBuffer(rStr.Find(_T('\\'), 2));
		}
		rStr += str;
	} else
		rStr = str;

	return true;
}

/*
** Finds a file hopefully going through all possibilities
*/
void CMyDoc::GetScriptFileName(CString& ref, LPCTSTR pszFileName) {
	ref.Empty();

	if (!pszFileName || !*pszFileName) return;

	if (CMyUtils::IsRelativePath(pszFileName)) {
		GetSourceDir(ref);
		CMyUtils::EndWith(ref, _T('\\'));
		if (pszFileName[0] == _T('\\') && pszFileName[1] != _T('\\') && ref.GetLength() >= 2 && ref[1] == _T(':')) {
			ref.ReleaseBuffer(2);
		}
		ref += pszFileName;
	} else
		ref = pszFileName;

	CString strTmp;
	// Thanks to Silvio Iaccarino <Silvio.Iaccarino@de.adp.com> for this one
	if (PathCanonicalize(strTmp.GetBuffer(MAX_PATH), ref)) {
		strTmp.ReleaseBuffer();
		ref = strTmp;
	}
}

bool CMyDoc::GetUseAbsolutePaths() {
	bool bAbs = true;
	try {
		bAbs = GetScript().GetPropertyBool(_T("UseAbsolutePaths"), CInnoScript::PRJ_ISTOOL);
	} catch (...) {
		GetScript().SetPropertyBool(_T("UseAbsolutePaths"), bAbs, CInnoScript::PRJ_ISTOOL);
	}
	return bAbs;
}

void CMyDoc::SetCurrentDir() {
	if (!m_strPathName.IsEmpty()) {
		int pos = m_strPathName.ReverseFind(_T('\\'));
		if (pos > 0) {
			SetCurrentDirectory(m_strPathName.Left(pos));
		}
	}
}
