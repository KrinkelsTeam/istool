#include "stdafx.h"
#include "ISTool.h"
#include "DlgCompile.h"

CDlgCompile::CDlgCompile(CMyDoc* pDoc, const bool bForceRun) :
	m_hCompiler(NULL), m_pDoc(pDoc), m_nCurrentLine(0),
	m_eAbort(FALSE, TRUE), m_eDone(FALSE, TRUE), m_bForceRun(bForceRun),
	m_hCompilerISPP(NULL)
{
	m_sec = CInnoScript::SEC_NONE;
	m_nErrorLine = 0;
	LPCTSTR pszLogFile = m_pDoc->GetScript().GetPropertyString(_T("LogFile"), CInnoScript::PRJ_ISTOOL);
	bool bAppend = m_pDoc->GetScript().GetPropertyBool(_T("LogFileAppend"), CInnoScript::PRJ_ISTOOL);
	if (pszLogFile && *pszLogFile) {
		if (!m_logWriter.Open(pszLogFile, TextEncoding::Auto, bAppend)) {
			CString txt = _L(_T("Failed to open log file '%1'."));
			txt.Replace(_T("%1"), pszLogFile);
			AtlMessageBox(m_hWnd, (LPCTSTR)txt, IDR_MAINFRAME, MB_OK | MB_ICONERROR);
		}
	}
}

CDlgCompile::~CDlgCompile() {
	if (m_hCompiler) FreeLibrary(m_hCompiler);
	if (m_hCompilerISPP) FreeLibrary(m_hCompilerISPP);
	if (m_logWriter.IsOpen()) m_logWriter.Close();
}

LRESULT CDlgCompile::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	_L(m_hWnd, _T("Compile"));
	CenterWindow(GetParent());

	m_wndList.Attach(GetDlgItem(IDC_COMPILE_LIST));
	m_wndList.SetHorizontalExtent(1024);
	m_wndProgress.Attach(GetDlgItem(IDC_PROGRESS));

	HMENU hMenu = GetSystemMenu(FALSE);
	EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);

	m_script = m_pDoc->GetScript();
	m_nCurrentLine = 0;

	if (!m_hCompiler) LoadCompiler();
	if (m_hCompiler) {
		DWORD dwThreadID;
		CreateThread(NULL, 0, ThreadEntry, this, 0, &dwThreadID);
	}

	return TRUE;
}

LRESULT CDlgCompile::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	if (wID == IDCANCEL) {
		if (WaitForSingleObject(m_eDone, 0) == WAIT_OBJECT_0) {
			EndDialog(wID);
		} else {
			m_eAbort.SetEvent();
			::EnableWindow(GetDlgItem(IDCANCEL), FALSE);
		}
	}
	return 0;
}

LRESULT CDlgCompile::OnGotoError(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	EndDialog(wID);
	return 0;
}

//-------------------------------------------------------------------------------------

bool CDlgCompile::LoadCompiler() {
	CString strLibrary;
	if (!m_pDoc->GetCompiler(strLibrary, true, false))
		return false;

	m_hCompiler = LoadLibrary(strLibrary);
	if (!m_hCompiler) {
		CString txt = _L(_T("Failed to open library '%1'."));
		txt.Replace(_T("%1"), strLibrary);
		AtlMessageBox(m_hWnd, (LPCTSTR)txt, IDR_MAINFRAME, MB_OK | MB_ICONERROR);
		return false;
	} else {
		m_fCompileScript = (ISDllCompileScriptProc)GetProcAddress(m_hCompiler, "ISDllCompileScriptW");
		m_fGetVersion = (ISDllGetVersionProc)GetProcAddress(m_hCompiler, "ISDllGetVersion");

		// ISPP is installed and ISTool is doing the
		// pre-processing, so bypass ISPP when compiling
		if (CMyApp::m_prefs.m_bPreProcess)
			if (m_pDoc->GetCompiler(strLibrary, true, true)) {
				if (m_hCompilerISPP = LoadLibrary(strLibrary)) {
					m_fCompileScriptISPP = (ISPreprocessScriptProc)GetProcAddress(m_hCompiler, "ISPreprocessScript");
				}
			}

		return true;
	}
}

long CDlgCompile::GetErrorLine() {
	return m_nErrorLine - 1;
}

DWORD WINAPI CDlgCompile::ThreadEntry(LPVOID lpParameter) {
	CDlgCompile* pDlg = (CDlgCompile*)lpParameter;
	DWORD dwRet = 0;

	pDlg->m_pDoc->SetCurrentDir();

	if (pDlg->m_fCompileScriptISPP && CMyApp::m_prefs.m_bPreProcess)
		dwRet = pDlg->PreProcess();

	if (dwRet == 0)
		dwRet = pDlg->DoCompile();

	if (!pDlg->m_strIncludeFile.IsEmpty())
		DeleteFile(pDlg->m_strIncludeFile);

	if (pDlg->m_logWriter.IsOpen()) {
		pDlg->m_logWriter.Close();
	}
	if (pDlg->m_pDoc->m_bCompileAndExit) {
		CMyApp::m_nExitCode = dwRet;
		pDlg->PostMessage(WM_CLOSE);
	}

	return dwRet;
}

UINT CDlgCompile::PreProcess() {
	return 0;
}

UINT CDlgCompile::DoCompile() {
	CString tmp;
	__time64_t time;
	_time64(&time);

	char timeStr[26];
	_ctime64_s(timeStr, sizeof(timeStr), &time);

	USES_CONVERSION;

	tmp.Format(_T("Compilation started: %s"), A2T(timeStr));
	tmp.TrimRight();
	AddListString(tmp);

	// Perform pre-compile steps
	AddListString(_T("Running pre-compile steps"));
	if (!RunCompileSteps(CInnoScript::PRJ_PRECOMPILESTEPS)) {
		SetFinished();
		return 1;
	}
	AddListString(_T(""));

	if (!AddDownloadSection())
		return 1;

	// Print compiler version information
	TCompilerVersionInfo* pVersionInfo = m_fGetVersion();
	CString strVersion;
	strVersion.Format(_T("Compiling script using %s %s"),
		CString(pVersionInfo->Title),
		CString(pVersionInfo->Version));
	AddListString(strVersion);

	// Find script folder
	CString strScriptPath(m_pDoc->GetPathName());
	int nPos = strScriptPath.ReverseFind(_T('\\'));
	if (nPos < 0) nPos = strScriptPath.ReverseFind(_T('/'));
	if (nPos >= 0) strScriptPath = strScriptPath.Left(nPos);

	// Compile script
	TCompileScriptParamsEx params;
	params.Size = sizeof params;
	params.CompilerPath = NULL;
	params.SourcePath = (LPTSTR)(LPCTSTR)strScriptPath;
	params.CallbackProc = CompilerCallback;
	params.AppData = (DWORD)this;
	params.Options = NULL;
	if (m_fCompileScript(&params) != isceNoError) {
		SetFinished();
		return 2;
	}

	// Empty string
	AddListString(_T(""));

	// Perform post-compile steps
	int nString = AddListString(_T("Running post-compile steps"));
	if (nString != LB_ERR) m_wndList.SetTopIndex(nString);
	if (!RunCompileSteps(CInnoScript::PRJ_POSTCOMPILESTEPS)) {
		SetFinished();
		return 3;
	}

	_time64(&time);
	_ctime64_s(timeStr, sizeof(timeStr), &time);
	tmp.Format(_T("Compilation ended: %s"), A2T(timeStr));
	tmp.TrimRight();
	AddListString(tmp);

	if (!m_pDoc->m_bCompileAndExit) {
		if (m_bForceRun || (CMyApp::m_prefs.m_bTestCompiledSetup && AtlMessageBox(m_hWnd, _L(_T("TestSetupNow"), _T("Setup compiled successfully. Test setup now?")), IDR_MAINFRAME, MB_YESNO | MB_ICONQUESTION) == IDYES)) {
			CMyApp::MyExec(m_strOutputExeFilename, NULL);
		}
		CString strOld = m_pDoc->GetScript().GetPropertyString(_T("OutputExeFilename"), CInnoScript::PRJ_ISTOOL);
		if (strOld != m_strOutputExeFilename && !CMyApp::m_prefs.m_bNoOutputExeFilename) {
			m_pDoc->GetScript().SetPropertyString(_T("OutputExeFilename"), m_strOutputExeFilename, CInnoScript::PRJ_ISTOOL);
			m_pDoc->SetModifiedFlag();
		}
	}

	// We're done
	SetFinished();

	return 0;
}

LONG __stdcall CDlgCompile::CompilerCallback(LONG Code, TCompilerCallbackData* Data, DWORD AppData) {
	return reinterpret_cast<CDlgCompile*>(AppData)->CompilerCallback(Code, Data);
}

UINT CDlgCompile::CompilerCallback(LONG Code, TCompilerCallbackData* Data) {
	int nString = LB_ERR;
	CString str;

	if (WaitForSingleObject(m_eAbort, 0) == WAIT_OBJECT_0)
		return iscrRequestAbort;

	switch (Code) {
	case iscbNotifyPreproc:
		m_strTranslation = Data->NotifyStatus.StatusMsg;
		break;
	case iscbReadScript:
		if (Data->ReadScript.Reset) {
			m_sec = CInnoScript::SEC_NONE;
			//m_pCurrentLine = m_pScriptLines;
			m_nCurrentLine = 0;
		}

		if (m_nCurrentLine >= m_script.GetSize()) {
			Data->ReadScript.LineRead = NULL;
		} else if (m_script[m_nCurrentLine]->GetSection() != m_sec && m_script[m_nCurrentLine]->GetSection() != CInnoScript::SEC_NONE) {
			m_sec = m_script[m_nCurrentLine]->GetSection();
			m_strCurrentLine.Format(_T("[%s]"), CInnoScriptEx::GetSectionName(m_sec));
			Data->ReadScript.LineRead = (LPTSTR)(LPCTSTR)m_strCurrentLine;
		} else {
			m_script[m_nCurrentLine]->Write(m_strCurrentLine.GetBuffer(8192), 8192);
			m_strCurrentLine.ReleaseBuffer();
			Data->ReadScript.LineRead = (LPTSTR)(LPCTSTR)m_strCurrentLine;
			m_nCurrentLine++;
		}
		break;

	case iscbNotifyStatus:
		if (Data->NotifyStatus.StatusMsg)
			nString = AddListString(Data->NotifyStatus.StatusMsg);
		break;

	case iscbNotifyIdle:
		if (Data->NotifyIdle.CompressProgress > 0) {
			if (!m_wndProgress.IsWindowVisible())
				m_wndProgress.ShowWindow(SW_SHOW);

			m_wndProgress.SetRange32(0, Data->NotifyIdle.CompressProgressMax);
			m_wndProgress.SetPos(Data->NotifyIdle.CompressProgress);
		}
		break;

	case iscbNotifySuccess:
		if (Data->NotifySuccess.OutputExeFilename) {
			m_strOutputExeFilename = Data->NotifySuccess.OutputExeFilename;
			nString = AddListString(Data->NotifySuccess.OutputExeFilename);
		}
		break;

	case iscbNotifyError:
		if (Data->NotifyError.ErrorMsg) {
			CString str;
			if (Data->NotifyError.ErrorFilename)
				str.Format(_T("%s line %d:"), Data->NotifyError.ErrorFilename, Data->NotifyError.ErrorLine);
			else {
				str.Format(_T("Line %d:"), Data->NotifyError.ErrorLine);
				m_nErrorLine = Data->NotifyError.ErrorLine;
			}

			AddListString(str);
			nString = AddListString(Data->NotifyError.ErrorMsg);
		}
		break;
	}

	if (nString != LB_ERR) {
		//		m_wndList.SetTopIndex(nString);
		//		m_wndList.UpdateWindow();
				//Sleep(500);
	}

	return iscrSuccess;
}

bool CDlgCompile::AddDownloadSection() {
	CScriptList listDownload;
	m_script.GetList(CInnoScript::PRJ_DOWNLOAD, listDownload);

	if (listDownload.GetSize() > 0) {
		CString strCode, tmp;
		AddListString(_T("Building download script"));

		strCode += _T("[Code]\r\n");	
		strCode += _T("procedure istool_AddFile(URL, Filename: String); external 'isxdl_AddFile@files:isxdl.dll stdcall';\r\n");
		strCode += _T("function istool_DownloadFiles(hWnd: Integer): Integer; external 'isxdl_DownloadFiles@files:isxdl.dll stdcall';\r\n");
		strCode += _T("procedure istool_ClearFiles; external 'isxdl_ClearFiles@files:isxdl.dll stdcall';\r\n");
		strCode += _T("\r\ntype\r\n");
		strCode += _T("  ISXDL = record\r\n");
		strCode += _T("    Source:     String;\r\n");
		strCode += _T("    DestDir:    String;\r\n");
		strCode += _T("    DestName:   String;\r\n");
		strCode += _T("    Tasks:      String;\r\n");
		strCode += _T("    Components: String;\r\n");
		strCode += _T("    Languages:  String;\r\n");
		strCode += _T("    Flags:      Cardinal;\r\n");
		strCode += _T("  end;\r\n");
		strCode += _T("\r\nvar\r\n");
		strCode += _T("  istool_files: array of ISXDL;\r\n");
		strCode += _T("\r\nprocedure istool_download_init();\r\n");
		strCode += _T("begin\r\n");
		
		tmp.Format(_T("  SetArrayLength(istool_files,%d);\r\n"), listDownload.GetSize()); strCode += tmp;
		for (int i = 0; i < listDownload.GetSize(); i++) {
			CScriptLine* pLine = listDownload[i];
			CString strSource, strDestDir, strDestName, strTasks, strComponents, strLanguages;
			strSource = pLine->GetParameter(_T("Source"));
			strDestDir = pLine->GetParameter(_T("DestDir"));
			strDestName = pLine->GetParameter(_T("DestName"));
			strTasks = pLine->GetParameter(_T("Tasks"));
			strComponents = pLine->GetParameter(_T("Components"));
			strLanguages = pLine->GetParameter(_T("Languages"));

			if (strSource.Find(_T('{')) < 0)
				tmp.Format(_T("  istool_files[%d].Source     := '%s';\r\n"), i, (LPCTSTR)strSource);
			else
				tmp.Format(_T("  istool_files[%d].Source     := ExpandConstant('%s');\r\n"), i, (LPCTSTR)strSource);
			strCode += tmp;
			if (strDestDir.Find(_T('{')) < 0)
				tmp.Format(_T("  istool_files[%d].DestDir    := '%s';\r\n"), i, (LPCTSTR)strDestDir);
			else
				tmp.Format(_T("  istool_files[%d].DestDir    := ExpandConstant('%s');\r\n"), i, (LPCTSTR)strDestDir);
			strCode += tmp;
			if (strDestName.Find(_T('{')) < 0)
				tmp.Format(_T("  istool_files[%d].DestName   := '%s';\r\n"), i, (LPCTSTR)strDestName);
			else
				tmp.Format(_T("  istool_files[%d].DestName   := ExpandConstant('%s');\r\n"), i, (LPCTSTR)strDestName);
			strCode += tmp;
			tmp.Format(_T("  istool_files[%d].Tasks      := '%s';\r\n"), i, (LPCTSTR)strTasks);
			strCode += tmp;
			tmp.Format(_T("  istool_files[%d].Components := '%s';\r\n"), i, (LPCTSTR)strComponents);
			strCode += tmp;
			tmp.Format(_T("  istool_files[%d].Languages  := '%s';\r\n"), i, (LPCTSTR)strLanguages);
			strCode += tmp;
			tmp.Format(_T("  istool_files[%d].Flags      := 0;\r\n"), i);
			strCode += tmp;
		}
		strCode += _T("end;\r\n");

		strCode += _T("\r\nfunction istool_checklanguages(Languages: String): Boolean;\r\n");
		strCode += _T("begin\r\n");
		strCode += _T("	Result := True;\r\n");
		strCode += _T("	if Languages = '' then exit;\r\n");
		strCode += _T("	if Pos(ActiveLanguage, Languages) = 0 then Result := False;\r\n");
		strCode += _T("end;\r\n");

		strCode += _T("\r\nfunction istool_download(CurPage: Integer): boolean;\r\n");
		strCode += _T("var\r\n");
		strCode += _T("  i, NumFiles: Integer;\r\n");
		strCode += _T("  DestFile, DestDir: String;\r\n");
		strCode += _T("begin\r\n");
		strCode += _T("  if CurPage <> wpReady then begin\r\n");
		strCode += _T("    Result := true;\r\n");
		strCode += _T("    Exit;\r\n");
		strCode += _T("  end;\r\n");
		strCode += _T("  istool_ClearFiles();\r\n");
		strCode += _T("  istool_download_init();\r\n");
		strCode += _T("  NumFiles := GetArrayLength(istool_files);\r\n");
		strCode += _T("  for i := 0 to NumFiles - 1 do begin\r\n");

		strCode += _T("	if (IsComponentSelected(istool_files[i].Components) = false) or (IsTaskSelected(istool_files[i].Tasks) = false) then continue;\r\n");
		strCode += _T("    if not istool_checklanguages(istool_files[i].Languages) then continue;\r\n");
		strCode += _T("    DestDir := AddBackslash(istool_files[i].DestDir);\r\n");
		strCode += _T("    if istool_files[i].DestName = '' then\r\n");
		strCode += _T("      DestFile := DestDir + 'dlfile.' + IntToStr(i)\r\n");
		strCode += _T("    else\r\n");
		strCode += _T("      DestFile := DestDir + istool_files[i].DestName;\r\n");
		strCode += _T("    istool_AddFile(istool_files[i].Source, DestFile);\r\n");
		strCode += _T("  end;\r\n");
		strCode += _T("  Result := 0 <> istool_DownloadFiles(StrToInt(ExpandConstant('{wizardhwnd}')));\r\n");
		strCode += _T("end;\r\n");
		strCode += _T("\r\n[Files]\r\n");

		tmp.Format(_T("Source: %sisxdl.dll; DestDir: {tmp}; Flags: dontcopy\r\n"), (LPCTSTR)theApp.m_strProgramPath);
		strCode += tmp;

		// Create a temporary file
		TCHAR szTempPath[MAX_PATH], szTempFile[MAX_PATH];
		GetTempPath(MAX_PATH, szTempPath);
		GetTempFileName(szTempPath, _T("ISXDL"), 0, szTempFile);

		// Save the script
		CTextFileWriter writer;
		if (!writer.Save(szTempFile, strCode, TextEncoding::Auto)) {
			AddListString(_T("Failed to write code file"));
			SetFinished();
			return false;
		}

		AddListString(szTempFile);
		AddListString(_T(""));

		if (m_logWriter.IsOpen())
			AppendLogFile(szTempFile);

		m_strIncludeFile = szTempFile;

		// AtlMessageBox(m_hWnd, (LPCTSTR)strCode);
		// AtlMessageBox(m_hWnd, (LPCTSTR)szTempFile);
		
		// Add include file to script
		tmp.Format(_T("#include \"%s\""), szTempFile);
		m_script.AddHead(new CInnoScript::CLineComment(CInnoScript::SEC_NONE, tmp));
	}
	return true;
}

void CDlgCompile::ParseDir(LPCTSTR pszFilter, CAtlTemporaryFile& file, const CString strDestDir, const CString& strRoot) {
	CString tmp;
	CFindFile find;

	if (find.FindFile(pszFilter)) {
		do {
			if (find.IsDots()) {
			} else if (find.IsDirectory()) {
				tmp.Format(_T("%s\\*.*"), (LPCTSTR)find.GetFilePath());
				ParseDir(tmp, file, strDestDir, strRoot);
			} else {
				CString strSource = find.GetFilePath();
				strSource = strSource.Mid(strRoot.GetLength());
				CString strSourcePath = find.GetRoot();
				strSourcePath = strSourcePath.Mid(strRoot.GetLength());
				tmp.Format(_T("Source: {src}\\%s; Flags: external; DestDir: %s\r\n"),
					find.GetFilePath().Mid(strRoot.GetLength()),
					strDestDir + strSourcePath
				);
				file.Write((LPCTSTR)tmp, tmp.GetLength());
			}
		} while (find.FindNextFile());
	}
}

void CDlgCompile::AppendLogFile(LPCTSTR pszFileName) {
	if (!m_logWriter.IsOpen()) return;
	
	CTextFileReader reader;
	if (reader.Load(pszFileName)) {
		m_logWriter.WriteLine(_T("=============================================================================="));
		CString tmp;
		tmp.Format(_T("Contents of \"%s\""), pszFileName);
		m_logWriter.WriteLine(tmp);
		m_logWriter.WriteLine(_T("=============================================================================="));

		CAtlArray<CString> lines;
		for (size_t i = 0; i < lines.GetCount(); i++) {
			m_logWriter.WriteLine(lines[i]);
		}

		m_logWriter.WriteLine(_T("=============================================================================="));
	}
}

void CDlgCompile::SetFinished() {
	m_eDone.SetEvent();
	::EnableWindow(GetDlgItem(IDCANCEL), TRUE);
	GetDlgItem(IDC_GOTOERROR).EnableWindow(m_nErrorLine > 0);
	SetDlgItemText(IDCANCEL, _T("Close"));

	HMENU hMenu = GetSystemMenu(FALSE);
	EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);
}

int CDlgCompile::AddListString(LPCTSTR pszString) {
	if (m_logWriter.IsOpen()) {
		m_logWriter.WriteLine(pszString);
	}

	m_wndList.SetRedraw(FALSE);
	int nString = m_wndList.AddString(pszString);
	m_wndList.SetTopIndex(nString);
	m_wndList.SetRedraw(TRUE);
	m_wndList.UpdateWindow();
	return nString;
}

bool CDlgCompile::RunCompileSteps(CInnoScript::SECTION sec) {
	CString strSection;
	if (sec == CInnoScript::PRJ_PRECOMPILESTEPS)
		strSection = _T("[PRE-COMPILE] ");
	else
		strSection = _T("[POST-COMPILE] ");
	CScriptList	steps;
	m_script.GetList(sec, steps);

	CString strOutputDir;
	try {
		m_pDoc->GetOutputFolder(strOutputDir);
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

		strParam.Replace(_T("{script}"), m_pDoc->GetPathName());
		strParam.Replace(_T("{outputdir}"), strOutputDir);

		AddListString(strSection + strName + _T(" ") + strParam);

		if (CMyApp::MyExec(strName, strParam, NULL, true, bMinimized) && bAbortOnError)
			return false;
	}
	return true;
}
