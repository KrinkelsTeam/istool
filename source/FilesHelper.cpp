// FilesHelper.cpp: implementation of the CFilesHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "istool.h"
#include "FilesHelper.h"
#include "MainFrm.h"
#include "MyDoc.h"
#include "DlgAddDirectory.h"
#include "FileInfo.h"
#include "SplitPath.h"
#include "StringToken.h"
#include "TextFileIO.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilesHelper::CFilesHelper(CMyDoc* pDoc) : m_pDoc(pDoc) {}

CFilesHelper::~CFilesHelper() {}

void CFilesHelper::OnDropFiles(HWND hWnd, HDROP hDropInfo, LPCTSTR lpszCurrentFolder) {
	UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);

	for (UINT iFile = 0; iFile < nFiles; iFile++) {
		TCHAR szFileName[_MAX_PATH];
		::DragQueryFile(hDropInfo, iFile, szFileName, _MAX_PATH);

		DWORD dwAttr = ::GetFileAttributes(szFileName);
		if (dwAttr == 0xFFFFFFFF) {
			CString txt = _L(_T("Error|GetFileAttributes"), _T("Error getting information about %1."));
			txt.Replace(_T("%1"), szFileName);
			AtlMessageBox(hWnd, (LPCTSTR)txt, IDR_MAINFRAME, MB_OK | MB_ICONERROR);
		} else if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) {
			// Add directory
			CDlgAddDirectory dlg(szFileName);
			if (dlg.DoModal(hWnd) == IDOK) {
				CString strWildCard;
				if (dlg.m_bAddFiles && !dlg.m_strWildCard.IsEmpty())
					strWildCard = dlg.m_strWildCard;
				else
					strWildCard = _T("*.*");
				CFileInfoArray fia;
				CWaitCursor wait;

				CString strAddedDir(szFileName);
				int pos = strAddedDir.ReverseFind(_T('\\'));
				if (pos < 0) pos = strAddedDir.ReverseFind(_T('/'));
				if (pos >= 0) strAddedDir = strAddedDir.Mid(pos + 1);

				CMyUtils::EndWith(szFileName, _T('\\'));
				fia.AddDir(
					CString(szFileName), strWildCard,
					dlg.m_bIncludeSubDirectories,
					dlg.m_bAddDirectories
				);
				if (fia.GetSize() > 0) {
					// Find the directory the files should be registered under
					CString strCurrentFolder(lpszCurrentFolder);
					CMyUtils::EndWith(strCurrentFolder, _T('\\'));
					strCurrentFolder += strAddedDir;

					{ // Add the main directory entry
						CScriptLine* pLine = new CScriptLine(CInnoScript::SEC_DIRS);
						pLine->SetParameter(_T("Name"), strCurrentFolder);
						m_pDoc->GetScript().AddLine(pLine);
					}
					CMyUtils::EndWith(strCurrentFolder, _T('\\'));

					// Loop through and add all files found
					for (int i = 0; i < fia.GetSize(); i++) {
						if (dlg.m_bAddFiles && !fia[i].IsDirectory()) {
							// Get path between szFileName and the files name
							CString strSubPath(fia[i].GetFilePath());
							strSubPath = strSubPath.Mid(_tcslen(szFileName)/*+1*/);
							pos = strSubPath.ReverseFind(_T('\\'));
							if (pos < 0) strSubPath.Empty();
							else strSubPath = /*"\\"+*/strSubPath.Left(pos);

							CScriptLine* pLine = new CScriptLine(CInnoScript::SEC_FILES);
							CString strSource(fia[i].GetFilePath());
							CString strSourceDir;
							if (!m_pDoc->GetUseAbsolutePaths() && m_pDoc->GetSourceDir(strSourceDir)) {
								strSource = CFunc::ExtractRelativePath(strSourceDir, strSource);
								if (strSource.IsEmpty())
									strSource = fia[i].GetFilePath();
							}
							if (dlg.m_bExternal && !dlg.m_strRoot.IsEmpty()) {
								if (!_tcsnicmp(dlg.m_strRoot, strSource, dlg.m_strRoot.GetLength()))
									strSource = _T("{src}\\") + strSource.Mid(dlg.m_strRoot.GetLength());
							}

							pLine->SetParameter(_T("Source"), strSource);
							pLine->SetParameter(_T("DestDir"), strCurrentFolder + strSubPath);

							if (fia[i].IsReadOnly()) pLine->SetParameterFlag(_T("Attribs"), _T("readonly"), true);
							if (fia[i].IsHidden()) pLine->SetParameterFlag(_T("Attribs"), _T("hidden"), true);
							if (fia[i].IsSystem()) pLine->SetParameterFlag(_T("Attribs"), _T("system"), true);
							if (dlg.m_bExternal) pLine->SetParameterFlag(_T("Flags"), _T("external"), true);
							m_pDoc->GetScript().AddLine(pLine);
						} else if (dlg.m_bAddDirectories && fia[i].IsDirectory()) {
							// Get path between szFileName and the files name
							CString strSubPath(fia[i].GetFilePath());
							strSubPath = strSubPath.Mid(_tcslen(szFileName)/*+1*/);

							CScriptLine* pLine = new CScriptLine(CInnoScript::SEC_DIRS);
							pLine->SetParameter(_T("Name"), strCurrentFolder + strSubPath);
							m_pDoc->GetScript().AddLine(pLine);
						}
					}
					m_pDoc->SetModifiedFlag();
				}
			}
		} else {
			// Add file
			InsertFileName(szFileName, lpszCurrentFolder);
			m_pDoc->SetModifiedFlag();
		}
	}
	::DragFinish(hDropInfo);
}

void CFilesHelper::InsertFileName(LPCTSTR lpszFileName, LPCTSTR lpszCurrentFolder) {
	CString strCurrentFolder(lpszCurrentFolder);

	CScriptLine* pLine = new CScriptLine(CInnoScript::SEC_FILES);
	CString strSourceDir;
	if (!m_pDoc->GetUseAbsolutePaths() && m_pDoc->GetSourceDir(strSourceDir)) {
		CString strSource = CFunc::ExtractRelativePath(strSourceDir, lpszFileName);
		if (strSource.IsEmpty()) strSource = lpszFileName;
		pLine->SetParameter(_T("Source"), strSource);
	} else
		pLine->SetParameter(_T("Source"), lpszFileName);
	pLine->SetParameter(_T("DestDir"), strCurrentFolder);

	m_pDoc->GetScript().AddLine(pLine);
	AutoComponentSelect(pLine);
}

void CFilesHelper::CreateIcon(HWND hWnd, CScriptLine* pItem) {
	ATLASSERT(pItem);

	// Find sensible information for the icon
	CString strDestName, strName, strFilename;
	CSplitPath path;
	if (pItem->GetParameter(_T("DestName"))) {
		strDestName = pItem->GetParameter(_T("DestName"));
		path.Split(strDestName);
	} else {
		path.Split(pItem->GetParameter(_T("Source")));
	}
	strName = CString(_T("{group}\\")) + path.GetFilename();
	path.SetDrive(_T(""));
	path.SetDirectory(SAFESTR(pItem->GetParameter(_T("DestDir"))));
	path.Make(strFilename.GetBuffer(MAX_PATH));

	// Enumerate icons to see if this file got an icon already
	bool bAddIcon = true;
	CScriptList icons;
	m_pDoc->GetScript().GetList(CInnoScript::SEC_ICONS, icons);
	for (int nPos = 0; nPos < icons.GetSize(); nPos++) {
		CScriptLine* pIconCheck = icons[nPos];
		if (strFilename == SAFESTR(pIconCheck->GetParameter(_T("Filename")))) {
			CString txt = _L(_T("IconAlreadyExists"), _T("An icon already exists for %1.\n\nCreate icon anyway?"));
			txt.Replace(_T("%1"), strFilename);
			int nRet = AtlMessageBox(hWnd, (LPCTSTR)txt, MB_YESNOCANCEL | MB_DEFBUTTON2 | MB_ICONQUESTION);
			if (nRet == IDCANCEL)
				return;
			else if (nRet == IDNO)
				bAddIcon = false;
			break;
		}
	}

	// Create the icon
	if (bAddIcon) {
		CScriptLine* pIcon = new CScriptLine(CInnoScript::SEC_ICONS);
		pIcon->SetParameter(_T("Name"), strName);
		pIcon->SetParameter(_T("Filename"), strFilename);
		pIcon->SetParameter(_T("WorkingDir"), pItem->GetParameter(_T("DestDir")));
		pIcon->SetParameter(_T("Comment"), path.GetFilename());
		pIcon->SetParameterFlag(_T("Flags"), _T("createonlyiffileexists"), true);
		m_pDoc->GetScript().AddLine(pIcon);
		m_pDoc->SetModifiedFlag();
	}
}

void CFilesHelper::OnFileAddFiles(LPCTSTR lpszCurrentFolder)
{
	CMyDoc* pDoc = m_pDoc; // optional shortcut

	// Create shell-style file open dialog with multi-select enabled
	CShellFileOpenDialog dlg(
		NULL,
		FOS_ALLOWMULTISELECT | FOS_FILEMUSTEXIST | FOS_PATHMUSTEXIST | FOS_FORCEFILESYSTEM,
		NULL, nullptr, 0
	);

	if (dlg.DoModal() != IDOK)
		return;

	// Get result array of selected files
	CComPtr<IFileOpenDialog> pFD = dlg.GetPtr();
	CComPtr<IShellItemArray> pItems;
	if (FAILED(pFD->GetResults(&pItems)))
		return;

	DWORD dwCount = 0;
	if (FAILED(pItems->GetCount(&dwCount)) || dwCount == 0)
		return;

	for (DWORD i = 0; i < dwCount; ++i) {
		CComPtr<IShellItem> pItem;
		if (SUCCEEDED(pItems->GetItemAt(i, &pItem))) {
			CString strPath;
			if (SUCCEEDED(CShellFileDialogImpl<CShellFileOpenDialog>::GetFileNameFromShellItem(pItem, SIGDN_FILESYSPATH, strPath)))
				InsertFileName(strPath, lpszCurrentFolder);
		}
	}

	pDoc->SetModifiedFlag();
}

bool CFilesHelper::ImportRegistry(HWND hWnd, LPCTSTR lpszRegFile) {
	CWaitCursor wait;
	bool bRet = false;

	CTextFileReader reader;
	if (!reader.Load(lpszRegFile)) {
		CString txt = _L(_T("Failed to open '%1'."));
		txt.Replace(_T("%1"), lpszRegFile);
		AtlMessageBox(hWnd, (LPCTSTR)txt, IDR_MAINFRAME, MB_OK | MB_ICONERROR);
		return false;
	}

	CAtlArray<CString> lines;
	reader.GetLines(lines);

	if (lines.IsEmpty()) return false;

	LPCTSTR pszRoot = NULL;
	CString strSubkey;
	CString strFormat = lines[0];

	if (!_tcsicmp(strFormat, _T("REGEDIT4")) || !_tcsicmp(strFormat, _T("Windows Registry Editor Version 5.00"))) {
		for (size_t i = 1; i < lines.GetCount(); ++i) {
			CString str = lines[i].Trim();
			CScriptLine* p = NULL;

			if (str.IsEmpty() || str[0] == _T(';')) continue;

			if (str[0] == _T('[') && str[str.GetLength() - 1] == _T(']')) {
				CStringToken token(str.Mid(1, str.GetLength() - 2), _T("\\"));
				LPCTSTR lpszRoot = token.GetNext();
				strSubkey = token.GetRest();
				strSubkey.Replace(_T("{"), _T("{{"));

				bool bDeleteKey = false;
				if (*lpszRoot == _T('-')) {
					bDeleteKey = true;
					++lpszRoot;
				}

				if (!_tcsicmp(lpszRoot, _T("HKEY_CLASSES_ROOT"))) pszRoot = _T("HKCR");
				else if (!_tcsicmp(lpszRoot, _T("HKEY_CURRENT_USER"))) pszRoot = _T("HKCU");
				else if (!_tcsicmp(lpszRoot, _T("HKEY_LOCAL_MACHINE"))) pszRoot = _T("HKLM");
				else if (!_tcsicmp(lpszRoot, _T("HKEY_USERS"))) pszRoot = _T("HKU");
				else if (!_tcsicmp(lpszRoot, _T("HKEY_CURRENT_CONFIG"))) pszRoot = _T("HKCC");
				else {
					CString txt = _L(_T("Unknown registry root %1."));
					txt.Replace(_T("%1"), lpszRoot);
					AtlMessageBox(hWnd, (LPCTSTR)txt, IDR_MAINFRAME, MB_OK | MB_ICONERROR);
					pszRoot = NULL;
				}

				if (bDeleteKey) {
					p = new CScriptLine(CInnoScript::SEC_REGISTRY);
					p->SetParameter(_T("Root"), pszRoot);
					p->SetParameter(_T("SubKey"), strSubkey);
					p->SetParameterFlag(_T("Flags"), _T("deletekey"), true);
				}
			} else {
				while (str.Right(1) == _T("\\")) {
					if (++i >= lines.GetCount()) break;
					CString tmp = lines[i].Trim();
					if (!tmp.IsEmpty()) {
						str = str.Left(str.GetLength() - 1) + tmp;
					}
				}

				CStringToken token(str, _T("="));
				CString strValueName(token.GetNext());
				CString strValueData(token.GetRest());
				bool bDeleteValue = strValueData == _T("-");

				if (strValueName[0] == _T('"') && strValueName[strValueName.GetLength() - 1] == _T('"'))
					strValueName = strValueName.Mid(1, strValueName.GetLength() - 2);
				if (strValueData[0] == _T('"') && strValueData[strValueData.GetLength() - 1] == _T('"'))
					strValueData = strValueData.Mid(1, strValueData.GetLength() - 2);

				p = new CScriptLine(CInnoScript::SEC_REGISTRY);
				p->SetParameter(_T("Root"), pszRoot);
				p->SetParameter(_T("SubKey"), strSubkey);

				int nPos = strValueData.Find(_T(':'));
				if (nPos >= 0) {
					CStringToken token(strValueData, _T(":"));
					LPCTSTR lpszValueType = token.GetNext();
					CString strNewValueData = token.GetRest();

					if (!_tcsicmp(lpszValueType, _T("dword"))) {
						p->SetParameter(_T("ValueType"), _T("dword"));
						strValueData = (strNewValueData[0] != _T('$')) ? (_T("$") + strNewValueData) : strNewValueData;

					} else if (!_tcsicmp(lpszValueType, _T("qword"))) {
						p->SetParameter(_T("ValueType"), _T("qword"));
						strValueData = (strNewValueData[0] != _T('$')) ? (_T("$") + strNewValueData) : strNewValueData;

					} else if (!_tcsicmp(lpszValueType, _T("hex"))) {
						p->SetParameter(_T("ValueType"), _T("binary"));
						strValueData = strNewValueData;
						strValueData.Replace(_T(","), _T(" "));

					} else if (!_tcsnicmp(lpszValueType, _T("hex("), 4)) {
						if (!_tcsnicmp(lpszValueType, _T("hex(2)"), 7)) {
							p->SetParameter(_T("ValueType"), _T("expandsz"));
						} else if (!_tcsnicmp(lpszValueType, _T("hex(7)"), 7)) {
							p->SetParameter(_T("ValueType"), _T("multisz"));
						} else {
							p->SetParameter(_T("ValueType"), _T("binary"));
						}
						strValueData = strNewValueData;
						strValueData.Replace(_T(","), _T(" "));

					} else {
						p->SetParameter(_T("ValueType"), _T("string"));
					}
				} else {
					p->SetParameter(_T("ValueType"), _T("string"));
				}

				if (strValueName == _T("@")) strValueName.Empty();
				p->SetParameter(_T("ValueName"), strValueName);

				strValueData.Replace(_T("{"), _T("{{"));
				strValueData.Replace(_T("\\\\"), _T("\\"));

				p->SetParameter(_T("ValueData"), strValueData);
				if (bDeleteValue) {
					p->DeleteParameter(_T("ValueData"));
					p->SetParameterFlag(_T("Flags"), _T("deletevalue"), true);
					p->SetParameter(_T("ValueType"), _T("none"));
				}
			}

			if (p) {
				m_pDoc->GetScript().AddLine(p);
				m_pDoc->SetModifiedFlag();
				bRet = true;
			}
		}
	} else {
		CString txt = _L(_T("Error|UnknownRegistryFormat"), _T("Unknown or unimplemented registry format '%1'."));
		txt.Replace(_T("%1"), strFormat);
		AtlMessageBox(hWnd, (LPCTSTR)txt, IDR_MAINFRAME, MB_OK | MB_ICONERROR);
	}

	return bRet;
}

void CFilesHelper::OnDropFilesRegistry(HWND hWnd, HDROP hDropInfo) {
	UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);

	for (UINT iFile = 0; iFile < nFiles; iFile++) {
		TCHAR szFileName[_MAX_PATH];
		::DragQueryFile(hDropInfo, iFile, szFileName, _MAX_PATH);
		ImportRegistry(hWnd, szFileName);
	}
	::DragFinish(hDropInfo);
}

bool CFilesHelper::ImportIni(HWND hWnd, LPCTSTR pszPathName)
{
	CWaitCursor wait;

	CTextFileReader reader;
	if (!reader.Load(pszPathName)) {
		CString txt = _L(_T("Failed to open '%1'."));
		txt.Replace(_T("%1"), pszPathName);
		AtlMessageBox(hWnd, (LPCTSTR)txt, IDR_MAINFRAME, MB_OK | MB_ICONERROR);
		return false;
	}

	CString strFilename(pszPathName);
	CString strSection;
	bool bAdded = false;

	CAtlArray<CString> lines;
	reader.GetLines(lines);

	for (size_t i = 0; i < lines.GetCount(); ++i) {
		CString str = lines[i].Trim();

		if (str.IsEmpty()) {
			continue;
		} else if (str[0] == _T('[') && str[str.GetLength() - 1] == _T(']')) {
			strSection = str.Mid(1, str.GetLength() - 2);
		} else if (str[0] != _T(';')) {
			CStringToken token(str, _T("="));
			CString strKey(token.GetNext());
			CString strString(token.GetRest());

			CScriptLine* pLine = new CScriptLine(CInnoScript::SEC_INI);
			pLine->SetParameter(_T("Filename"), strFilename);
			pLine->SetParameter(_T("Section"), strSection);
			pLine->SetParameter(_T("Key"), strKey);
			pLine->SetParameter(_T("String"), strString);
			m_pDoc->GetScript().AddLine(pLine);
			m_pDoc->SetModifiedFlag();
			bAdded = true;
		}
	}

	if (bAdded)
		m_pDoc->UpdateAll();

	return true;
}

void CFilesHelper::AutoComponentSelect(CScriptList& list) {
	// Automatically selects a component if destdir matches a component
	if (CMyApp::m_prefs.m_bAutoComponentSelect) {
		for (int nPos0 = 0; nPos0 < list.GetSize(); nPos0++) {
			CScriptLine* pFile = list[nPos0];
			CString str(pFile->GetParameter(_T("DestDir")));

			if (str.IsEmpty()) continue;
			if (str[str.GetLength() - 1] == _T('\\') || str[str.GetLength() - 1] == _T('/'))
				str = str.Left(str.GetLength() - 1);
			int nPos = str.ReverseFind(_T('\\'));
			if (nPos < 0) nPos = str.ReverseFind(_T('/'));
			if (nPos >= 0) {
				str = str.Mid(nPos + 1);
				CScriptList list;
				m_pDoc->GetScript().GetList(CInnoScript::SEC_COMPONENTS, list);
				for (int nPos = 0; nPos < list.GetSize(); nPos++) {
					if (!str.CompareNoCase(list[nPos]->GetParameter(_T("Name")))) {
						pFile->SetParameterFlag(_T("Components"), str, true);
						break;
					}
				}
			}
		}
	}
}

void CFilesHelper::AutoComponentSelect(CScriptLine* pLine) {
	CScriptList list;
	list.Add(pLine);
	AutoComponentSelect(list);
}

void CFilesHelper::RenameComponent(LPCTSTR pszFrom, LPCTSTR pszTo) {
	CWaitCursor wait;
	CScriptList	list;
	CInnoScriptEx& script = m_pDoc->GetScript();

	script.GetList(CInnoScript::SEC_FILES, list);
	script.GetList(CInnoScript::SEC_DIRS, list);
	script.GetList(CInnoScript::SEC_ICONS, list);
	script.GetList(CInnoScript::SEC_REGISTRY, list);
	script.GetList(CInnoScript::SEC_INSTALLDELETE, list);
	script.GetList(CInnoScript::SEC_RUN, list);
	script.GetList(CInnoScript::SEC_UNINSTALLDELETE, list);
	script.GetList(CInnoScript::SEC_UNINSTALLRUN, list);
	script.GetList(CInnoScript::SEC_TASKS, list);
	const LPCTSTR pszSection = _T("Components");

	for (int nPos = 0; nPos < list.GetSize(); nPos++) {
		CScriptLine* pLine = list[nPos];
		if (pLine->GetParameterFlag(pszSection, pszFrom)) {
			pLine->SetParameterFlag(pszSection, pszFrom, false);
			pLine->SetParameterFlag(pszSection, pszTo, true);
		}
	}
}

void CFilesHelper::RenameType(LPCTSTR pszFrom, LPCTSTR pszTo) {
	CWaitCursor wait;
	CScriptList	list;
	CInnoScriptEx& script = m_pDoc->GetScript();

	script.GetList(CInnoScript::SEC_COMPONENTS, list);
	const LPCTSTR pszSection = _T("Types");

	for (int nPos = 0; nPos < list.GetSize(); nPos++) {
		CScriptLine* pLine = list[nPos];
		if (pLine->GetParameterFlag(pszSection, pszFrom)) {
			pLine->SetParameterFlag(pszSection, pszFrom, false);
			pLine->SetParameterFlag(pszSection, pszTo, true);
		}
	}
}

void CFilesHelper::RenameTask(LPCTSTR pszFrom, LPCTSTR pszTo) {
	CWaitCursor wait;
	CScriptList	list;
	CInnoScriptEx& script = m_pDoc->GetScript();

	script.GetList(CInnoScript::SEC_FILES, list);
	script.GetList(CInnoScript::SEC_DIRS, list);
	script.GetList(CInnoScript::SEC_ICONS, list);
	script.GetList(CInnoScript::SEC_REGISTRY, list);
	script.GetList(CInnoScript::SEC_INSTALLDELETE, list);
	script.GetList(CInnoScript::SEC_RUN, list);
	script.GetList(CInnoScript::SEC_UNINSTALLDELETE, list);
	script.GetList(CInnoScript::SEC_UNINSTALLRUN, list);
	const LPCTSTR pszSection = _T("Tasks");

	for (int nPos = 0; nPos < list.GetSize(); nPos++) {
		CScriptLine* pLine = list[nPos];
		if (pLine->GetParameterFlag(pszSection, pszFrom)) {
			pLine->SetParameterFlag(pszSection, pszFrom, false);
			pLine->SetParameterFlag(pszSection, pszTo, true);
		}
	}
}
