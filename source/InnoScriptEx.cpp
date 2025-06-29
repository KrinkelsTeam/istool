// InnoScriptEx.cpp: implementation of the CInnoScriptEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InnoScriptEx.h"
#include "mywtl.h"
#include "resource.h"

struct {
	LPCTSTR	m_pszName;
	LPCTSTR	m_pszValue;
} m_defaults2[] = {
	// [LangOptions]

	_T("LanguageName"),					_T("English"),
	_T("LanguageID"),					_T("$0409"),
	_T("DialogFontName"),				_T("MS Shell Dlg"),
	_T("DialogFontSize"),				_T("8"),
	_T("DialogFontStandardHeight"),		_T("13"),
	_T("TitleFontName"),				_T("Arial"),
	_T("TitleFontSize"),				_T("29"),
	_T("WelcomeFontName"),				_T("Arial"),
	_T("WelcomeFontSize"),				_T("12"),
	_T("CopyrightFontName"),			_T("Arial"),
	_T("CopyrightFontSize"),			_T("8"),
	_T("RightToLeft"),					_T("false"),

	// [Setup]
	// Inno Setup 5.2.4
	_T("SignTool"),						_T(""),
	// Inno Setup 5.2.3
	_T("VersionInfoProductName"),		_T(""),
	_T("VersionInfoproductVersion"),	_T(""),
	// Inno Setup 5.2.1
	_T("SignedUninstaller"),			_T("false"),
	_T("SignedUninstallerDir"),			_T(""),
	// Inno Setup 5.1.14
	_T("CompressionThreads"),			_T("auto"),
	// Inno Setup 5.1.13
	_T("SetupLogging"),					_T("false"),
	_T("AppSupportPhone"),				_T(""),
	// Inno Setup 5.1.7
	_T("TerminalServicesAware"),		_T("true"),
	_T("ShowUndisplayableLanguages"),	_T("false"),
	// Inno Setup 5.1.5
	_T("VersionInfoCopyright"),			_T(""),
	// Inno Setup 5.1.0
	_T("ArchitecturesInstallIn64BitMode"),	_T(""),
	_T("ArchitecturesAllowed"),			_T(""),
	// Inno Setup 5.0.5
	_T("OutputManifestFile"),			_T(""),
	// Inno Setup 5.0.4
	_T("ChangesEnvironment"),			_T("false"),
	// Inno Setup 4.2.4
	_T("AppModifyPath"),				_T(""),
	_T("AppComments"),					_T(""),
	_T("AppContact"),					_T(""),
	_T("AppReadmeFile"),				_T(""),
	// Inno Setup 4.2.2
	_T("Encryption"),					_T("false"),
	// Inno Setup 4.2.0
	_T("TouchDate"),					_T("current"),
	_T("TouchTime"),					_T("current"),
	// Inno Setup 4.1.8
	_T("AppendDefaultDirName"),			_T("true"),
	_T("AppendDefaultGroupName"),		_T("true"),
	// Inno Setup 4.1.3
	_T("WizardImageStretch"),			_T("true"),
	// Inno Setup 4.1.0
	_T("SetupIconFile"),				_T(""),
	_T("VersionInfoVersion"),			_T(""),
	_T("VersionInfoCompany"),			_T(""),
	_T("VersionInfoDescription"),		_T(""),
	_T("VersionInfoTextVersion"),		_T(""),
	_T("TimeStampRounding"),			_T("2"),
	// Inno Setup 4.0.10
	_T("TimeStampsInUTC"),				_T("false"),
	_T("LanguageDetectionMethod"),		_T("uilanguage"),
	// Inno Setup 4.0.9
	_T("AllowCancelDuringInstall"),		_T("true"),
	// Inno Setup 4.0.1
	_T("SolidCompression"),				_T("false"),
	_T("DetectLanguageUsingLocale"),	_T("false"),
	// Inno Setup 4.0.0
	_T("ShowLanguageDialog"),			_T("yes"),
	_T("DiskSliceSize"),				_T("1457664"),
	_T("SlicesPerDisk"),				_T("1"),

	// My Inno Setup Extensions 3.0.6.1
	_T("DefaultUserInfoSerial"),		_T(""),

	// My Inno Setup Extensions 3.0.3
	_T("ShowTasksTreeLines"),			_T("false"),

	// Inno Setup 3.0.4, Inno Setup 5.1.9
	_T("PrivilegesRequired"),			_T("admin"),

	// Inno Setup 3.0.3
	_T("AlwaysRestart"),				_T("false"),
	_T("RestartIfNeededByRun"),			_T("true"),

	// Inno Setup 3.0.1
	_T("UninstallRestartComputer"),		_T("false"),

	// Inno Setup 3.0.0
	_T("UserInfoPage"),					_T("false"),
	_T("DefaultUserInfoName"),			_T("{sysuserinfoname}"),
	_T("DefaultUserInfoOrg"),			_T("{sysuserinfoorg}"),
	_T("UsePreviousUserInfo"),			_T("true"),

	// Inno Setup 2.0.18
	_T("AllowUNCPath"),					_T("true"),

	// Inno Seup 2.0.17
	_T("Compression"),					_T("lzma"),
	_T("InternalCompressLevel"),		_T("normal"),

	// Inno Setup 2.0.7
	_T("AlwaysShowDirOnReadyPage"),		_T("false"),
	_T("AlwaysShowGroupOnReadyPage"),	_T("false"),

	// Inno Setup 2.0.0
	_T("DisableReadyMemo"),				_T("false"),
	_T("AlwaysShowComponentsList"),		_T("true"),
	_T("ShowComponentSizes"),			_T("true"),
	_T("FlatComponentsList"),			_T("true"),
	_T("DisableReadyPage"),				_T("false"),
	_T("UsePreviousSetupType"),			_T("true"),
	_T("UsePreviousTasks"),				_T("true"),
	_T("WindowVisible"),				_T("false"),
	_T("WizardImageBackColor"),			_T("$400000"),
	_T("WizardImageFile"),				_T("compiler:wizmodernimage.bmp"),
	_T("WizardSmallImageFile"),			_T("compiler:wizmodernsmallimage.bmp"),

	// Inno Setup
	_T("UpdateUninstallLogAppName"),	_T("true"),		// IS 1.3.20
	_T("AdminPrivilegesRequired"),		_T("false"),
	_T("AllowNoIcons"),					_T("false"),
	_T("AllowRootDirectory"),			_T("false"),
	_T("AlwaysUsePersonalGroup"),		_T("false"),
	_T("AppCopyright"),					_T(""),
	_T("AppName"),						_T(""),
	_T("AppVerName"),					_T(""),
	_T("BackColor"),					_T("clBlue"),
	_T("BackSolid"),					_T("false"),
	_T("ChangesAssociations"),			_T("false"),
	_T("CreateAppDir"),					_T("true"),
	_T("DefaultDirName"),				_T("{pf}\\"),
	_T("DefaultGroupName"),				_T(""),
	_T("DisableDirPage"),				_T("false"),
	_T("DisableFinishedPage"),			_T("false"),
	_T("DisableProgramGroupPage"),		_T("false"),
	_T("DisableStartupPrompt"),			_T("true"),
	_T("DiskClusterSize"),				_T("512"),
	_T("DiskSpanning"),					_T("false"),
	_T("MergeDuplicateFiles"),			_T("true"),
	_T("EnableDirDoesntExistWarning"),	_T("false"),
	_T("ExtraDiskSpaceRequired"),		_T("0"),
	_T("InfoAfterFile"),				_T(""),
	_T("InfoBeforeFile"),				_T(""),
	_T("LicenseFile"),					_T(""),
	_T("MessagesFile"),					_T("compiler:default.isl"),
	_T("MinVersion"),					_T(""),
	_T("OnlyBelowVersion"),				_T(""),
	_T("OutputBaseFilename"),			_T("setup"),
	_T("OutputDir"),					_T(""),
	_T("OverwriteUninstRegEntries"),	_T("true"),
	_T("Password"),						_T(""),
	_T("ReserveBytes"),					_T("0"),
	_T("SourceDir"),					_T(""),
	_T("Uninstallable"),				_T("true"),
	_T("UseSetupLdr"),					_T("true"),
	_T("WindowShowCaption"),			_T("true"),
	_T("WindowStartMaximized"),			_T("true"),
	_T("WindowResizable"),				_T("true"),

	// Inno Setup 1.3.0
	_T("AppID"),						_T(""),
	_T("AppPublisher"),					_T(""),
	_T("AppPublisherURL"),				_T(""),
	_T("AppSupportURL"),				_T(""),
	_T("AppUpdatesURL"),				_T(""),
	_T("AppVersion"),					_T(""),
	_T("CreateUninstallRegKey"),		_T("true"),
	_T("UninstallLogMode"),				_T("append"),

	// Inno Setup 1.3.1
	_T("UsePreviousAppDir"),			_T("true"),

	// Inno Setup 1.3.3
	_T("BackColor2"),					_T("clBlack"),
	_T("BackColorDirection"),			_T("toptobottom"),
	_T("UninstallFilesDir"),			_T("{app}"),

	// Inno Setup 1.3.6
	_T("DirExistsWarning"),				_T("auto"),
	_T("UninstallDisplayIcon"),			_T(""),
	_T("UninstallDisplayName"),			_T(""),

	// Inno Setup 1.3.10 (My suggestion:)
	_T("UsePreviousGroup"),				_T("true"),

	// Inno Setup 1.3.14
	_T("AppMutex"),						_T(""),
	NULL
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInnoScriptEx::CInnoScriptEx() {}

CInnoScriptEx::~CInnoScriptEx() {}

void CInnoScriptEx::GetList(SECTION sec, CScriptList& list) {
	for (long i = 0; i < GetSize(); i++) {
		CLine* pLine = m_lines[i];
		if ((sec == SEC_CODE || !pLine->GetComment()) && pLine->GetSection() == sec) {
			list.Add(pLine);
		}
	}
}

void CInnoScriptEx::GetCompleteList(CScriptList& list) {
	for (long i = 0; i < GetSize(); i++)
		list.Add(m_lines[i]);
}

void CInnoScriptEx::AddLine(CLine* pLine) {
	bool bFound = false;
	for (long i = 0; i < GetSize(); i++) {
		if (bFound && m_lines[i]->GetSection() != pLine->GetSection()) {
			// Rewind
			while (i > 0 && m_lines[i - 1]->GetSection() == pLine->GetSection()) {
				CString strLine;
				m_lines[i - 1]->Write(strLine.GetBufferSetLength(10000), 10000);
				strLine.ReleaseBuffer();
				if (strLine.Trim().IsEmpty())
					i--;
				else
					break;
			}
			m_lines.InsertAt(i, pLine);
			return;
		}
		if (m_lines[i]->GetSection() == pLine->GetSection())
			bFound = true;
	}
	m_lines.Add(pLine);
}

void CInnoScriptEx::DeleteLine(CLine* pLine) {
	for (long i = 0; i < GetSize(); i++) {
		if (m_lines[i] == pLine) {
			m_lines.RemoveAt(i);
			delete pLine;
			break;
		}
	}
}

UINT CInnoScriptEx::CountLines(SECTION sec) {
	UINT nCount = 0;
	for (long i = 0; i < GetSize(); i++) {
		CLine* pLine = m_lines[i];
		if (pLine->GetSection() == sec && !pLine->GetComment())
			nCount++;
	}
	return nCount;
}

bool CInnoScriptEx::GetBoolean(LPCTSTR pszValue) {
	if (!_tcsicmp(pszValue, _T("yes")))
		return true;
	if (!_tcsicmp(pszValue, _T("true")))
		return true;
	if (_ttol(pszValue))
		return true;
	return false;
}

int CInnoScriptEx::Convert(const CConverter* pData, LPCTSTR pszValue) {
	if (!pszValue) return -1;
	for (UINT n = 0; pData[n].m_pszString; n++) {
		if (!_tcsicmp(pszValue, pData[n].m_pszString))
			return pData[n].m_nValue;
	}
	return -1;
}

LPCTSTR CInnoScriptEx::Convert(const CConverter* pData, int nValue) {
	for (UINT n = 0; pData[n].m_pszString; n++) {
		if (nValue == pData[n].m_nValue)
			return pData[n].m_pszString;
	}
	return NULL;
}

void CInnoScriptEx::GetDestDir(CLine* pLine, CString& ref) {
	ref.Empty();

	if (pLine->GetSection() != SEC_DIRS) return;

	CString str(pLine->GetParameter(_T("Name")));
	int pos = str.ReverseFind(_T('\\'));
	if (pos >= 0) ref = str.Left(pos);
	else ref.Empty();
}

void CInnoScriptEx::GetDestName(CLine* pLine, CString& ref) {
	ref.Empty();
	if (pLine->GetSection() != SEC_FILES) return;

	ref = pLine->GetParameter(_T("DestDir"));
	CMyUtils::EndWith(ref, _T('\\'));
	LPCTSTR pszDestName = pLine->GetParameter(_T("DestName"));
	if (!pszDestName) {
		CString tmp(pLine->GetParameter(_T("Source")));
		int pos = tmp.ReverseFind(_T('\\'));
		if (pos <= 0) pos = tmp.ReverseFind(_T(':'));
		if (pos >= 0) ref += tmp.Mid(pos + 1);
		else ref += pLine->GetParameter(_T("Source"));
	} else
		ref += pszDestName;
}

void CInnoScriptEx::GetDestTitle(CLine* pLine, CString& ref) {
	ref.Empty();
	if (pLine->GetSection() == SEC_FILES) {
		LPCTSTR pszDestName = pLine->GetParameter(_T("DestName"));
		if (!pszDestName) {
			CString tmp(pLine->GetParameter(_T("Source")));
			int pos = tmp.ReverseFind(_T('\\'));
			if (pos <= 0) pos = tmp.ReverseFind(_T(':'));
			if (pos >= 0) ref = tmp.Mid(pos + 1);
			else ref = pLine->GetParameter(_T("Source"));
		} else {
			ref = pszDestName;
		}
	} else if (pLine->GetSection() == SEC_DIRS) {
		CString str(pLine->GetParameter(_T("Name")));
		int pos = str.ReverseFind(_T('\\'));
		if (pos >= 0) ref = str.Mid(pos + 1);
		else ref = pLine->GetParameter(_T("Name"));
	}
}

void CInnoScriptEx::SetString(CLine* pLine, bool bForce, LPCTSTR pszName, LPCTSTR pszValue) {
	if (bForce || (pszValue && *pszValue)) {
		if (pszValue && *pszValue)
			pLine->SetParameter(pszName, pszValue);
		else
			pLine->DeleteParameter(pszName);
	}
}

void CInnoScriptEx::SetLong(CLine* pLine, bool bForce, LPCTSTR pszName, LONG nDefault, LONG nValue) {
	if (bForce || nValue != nDefault) {
		if (nValue != nDefault) {
			CString strValue;
			strValue.Format(_T("%d"), nValue);
			pLine->SetParameter(pszName, strValue);
		} else
			pLine->DeleteParameter(pszName);
	}
}

void CInnoScriptEx::SetFlag(CLine* pLine, LPCTSTR pszName, LPCTSTR pszFlag, int nFlag) {
	if (nFlag == 1)
		pLine->SetParameterFlag(pszName, pszFlag, true);
	else if (nFlag == 0)
		pLine->SetParameterFlag(pszName, pszFlag, false);
}

void CInnoScriptEx::AddComment(SECTION sec, LPCTSTR pszComment) {
	AddLine(new CLineComment(sec, pszComment));
}

void CInnoScriptEx::SetPropertyBool(LPCTSTR pszName, bool bValue, SECTION sec/*=SEC_SETUP*/) {
	SetPropertyString(pszName, bValue ? _T("true") : _T("false"), sec);
}

void CInnoScriptEx::SetPropertyNumber(LPCTSTR pszName, LONG nValue, SECTION sec/*=SEC_SETUP*/) {
	CString str;
	str.Format(_T("%d"), nValue);
	SetPropertyString(pszName, str, sec);
}

LPCTSTR CInnoScriptEx::GetDefault(LPCTSTR pszName, SECTION sec/*=SEC_SETUP*/) {
	if (sec != SEC_SETUP && sec != SEC_LANGOPTIONS) return NULL;

	// If it equals default don't add it
	for (UINT n = 0; m_defaults2[n].m_pszName; n++) {
		if (!_tcsicmp(pszName, m_defaults2[n].m_pszName))
			return m_defaults2[n].m_pszValue;
	}
#ifdef _DEBUG
	CString strError;
	strError.Format(_T("Failed to find default value for setup directive %s!"), pszName);
	AtlMessageBox(AfxGetMainHWnd(), (LPCTSTR)strError, IDR_MAINFRAME, MB_OK | MB_ICONERROR);
#endif
	return NULL;
}


void CInnoScriptEx::SetPropertyString(LPCTSTR pszName, LPCTSTR pszValue, SECTION sec/*=SEC_SETUP*/) {
	for (long i = 0; i < GetSize(); i++) {
		CLine* pLine = m_lines[i];
		if (pLine->GetSection() == sec && !pLine->GetComment() && pLine->GetKey() && !_tcsicmp(pLine->GetKey(), pszName)) {
			if (pszValue)
				pLine->SetParameter(pszName, pszValue);
			else
				pLine->DeleteParameter(pszName);
			return;
		}
	}
	// Not found so far
	if (!pszValue) return;

	// If it equals default don't add it
	LPCTSTR pszDefault = GetDefault(pszName, sec);
	if ((sec == SEC_SETUP || sec == SEC_LANGOPTIONS) && pszDefault && !_tcsicmp(pszDefault, pszValue))
		return;

	CString str;
	str.Format(_T("%s=%s"), pszName, pszValue);
	CLine* pLine = new CLineSetup(sec, str);
	pLine->SetParameter(pszName, pszValue);
	AddLine(pLine);
}

bool CInnoScriptEx::GetPropertyBool(LPCTSTR pszName, SECTION sec/*=SEC_SETUP*/) {
	LPCTSTR pszValue = GetPropertyString(pszName, sec);
	return pszValue && GetBoolean(pszValue);
}

LONG CInnoScriptEx::GetPropertyNumber(LPCTSTR pszName, SECTION sec/*=SEC_SETUP*/) {
	return _ttol(GetPropertyString(pszName, sec));
}

LPCTSTR CInnoScriptEx::GetPropertyString(LPCTSTR pszName, SECTION sec/*=SEC_SETUP*/) {
	for (long i = 0; i < GetSize(); i++) {
		CLine* pLine = m_lines[i];
		if (pLine->GetSection() == sec && !pLine->GetComment() && pLine->GetKey() && !_tcsicmp(pLine->GetKey(), pszName)) {
			return pLine->GetValue();
		}
	}
	return GetDefault(pszName, sec);
}

static const CInnoScriptEx::CConverter m_valueType[] = {
	_T("none"),		0,
	_T("string"),	1,
	_T("expandsz"),	2,
	_T("multisz"),	3,
	_T("dword"),	4,
	_T("binary"),	5,
	_T("qword"),	6,
	NULL
};

LPCTSTR CInnoScriptEx::GetRegistryValueType(int nRoot) {
	return Convert(m_valueType, nRoot);
}

int CInnoScriptEx::GetRegistryValueType(LPCTSTR pszValue) {
	return Convert(m_valueType, pszValue);
}

static const CInnoScriptEx::CConverter m_dirExistsWarning[] = {
	_T("auto"),		0,
	_T("yes"),		1,
	_T("true"),		1,
	_T("1"),		1,
	_T("no"),		2,
	_T("false"),	2,
	_T("0"),		2,
	NULL
};

int CInnoScriptEx::GetDirExistsWarning(LPCTSTR pszValue) {
	return Convert(m_dirExistsWarning, pszValue);
}

LPCTSTR CInnoScriptEx::GetDirExistsWarning(int nMode) {
	return Convert(m_dirExistsWarning, nMode);
}

static const CInnoScriptEx::CConverter m_showLanguageDialog[] = {
	_T("auto"),		0,
	_T("yes"),		1,
	_T("true"),		1,
	_T("1"),		1,
	_T("no"),		2,
	_T("false"),	2,
	_T("0"),		2,
	NULL
};

int CInnoScriptEx::GetShowLanguageDialog(LPCTSTR pszValue) {
	return Convert(m_showLanguageDialog, pszValue);
}

LPCTSTR CInnoScriptEx::GetShowLanguageDialog(int nMode) {
	return Convert(m_showLanguageDialog, nMode);
}

static const CInnoScriptEx::CConverter m_privilegesRequired[] = {
	_T("none"),			0,
	_T("poweruser"),	1,
	_T("admin"),		2,
	NULL
};

LPCTSTR CInnoScriptEx::GetPrivilegesRequired(int nMode) {
	return Convert(m_privilegesRequired, nMode);
}

int CInnoScriptEx::GetPrivilegesRequired(LPCTSTR pszValue) {
	return Convert(m_privilegesRequired, pszValue);
}

static const CInnoScriptEx::CConverter m_restartComputer[] = {
	_T("auto"),		0,
	_T("yes"),		1,
	_T("true"),		1,
	_T("1"),		1,
	_T("no"),		2,
	_T("false"),	2,
	_T("0"),		2,
	NULL
};

int CInnoScriptEx::GetRestartComputer(LPCTSTR pszValue) {
	return Convert(m_restartComputer, pszValue);
}

LPCTSTR CInnoScriptEx::GetRestartComputer(int nMode) {
	return Convert(m_restartComputer, nMode);
}

static const CInnoScriptEx::CConverter m_deleteType[] = {
	_T("files"),			0,
	_T("filesandordirs"),	1,
	_T("dirifempty"),		2,
	NULL
};

int CInnoScriptEx::GetDeleteType(LPCTSTR pszType) {
	return Convert(m_deleteType, pszType);
}

LPCTSTR CInnoScriptEx::GetDeleteType(int nType) {
	return Convert(m_deleteType, nType);
}

static const CInnoScriptEx::CConverter m_backColorDirection[] = {
	_T("toptobottom"),	0,
	_T("lefttoright"),	1,
	NULL
};

int CInnoScriptEx::GetBackColorDirection(LPCTSTR pszValue) {
	return Convert(m_backColorDirection, pszValue);
}

LPCTSTR CInnoScriptEx::GetBackColorDirection(int nMode) {
	return Convert(m_backColorDirection, nMode);
}

static const CInnoScriptEx::CConverter m_langaugeDetectionMethod[] = {
	_T("uilanguage"),	0,
	_T("locale"),		1,
	_T("none"),			2,
	NULL
};

int CInnoScriptEx::GetLanguageDetectionMethod(LPCTSTR pszValue) {
	return Convert(m_langaugeDetectionMethod, pszValue);
}

LPCTSTR CInnoScriptEx::GetLanguageDetectionMethod(int nMode) {
	return Convert(m_langaugeDetectionMethod, nMode);
}

static const CInnoScriptEx::CConverter m_internalCompressLevel[] = {
	_T("none"),		0,
	_T("fast"),		1,
	_T("normal"),	2,
	_T("max"),		3,
	_T("ultra"),	4,
	_T("ultra64"),	5,
	NULL
};

int CInnoScriptEx::GetInternalCompressLevel(LPCTSTR pszValue) {
	return Convert(m_internalCompressLevel, pszValue);
}

LPCTSTR CInnoScriptEx::GetInternalCompressLevel(int nMode) {
	return Convert(m_internalCompressLevel, nMode);
}

static const CInnoScriptEx::CConverter m_uninstallLogMode[] = {
	_T("append"),		0,
	_T("new"),			1,
	_T("overwrite"),	2,
	NULL
};

int CInnoScriptEx::GetUninstallLogMode(LPCTSTR pszValue) {
	return Convert(m_uninstallLogMode, pszValue);
}

LPCTSTR CInnoScriptEx::GetUninstallLogMode(int nMode) {
	return Convert(m_uninstallLogMode, nMode);
}

static const CInnoScriptEx::CConverter m_root[] = {
	_T("HKCR"),	0,
	_T("HKCU"),	1,
	_T("HKLM"),	2,
	_T("HKU"),	3,
	_T("HKCC"),	4,
	NULL
};

int CInnoScriptEx::GetRegistryRoot(LPCTSTR pszValue) {
	return Convert(m_root, pszValue);
}

LPCTSTR CInnoScriptEx::GetRegistryRoot(int nRoot) {
	return Convert(m_root, nRoot);
}



// File system additions
bool CInnoScriptEx::GetFolderName(CString& ref, CScriptLine* p) {
	ref.Empty();
	if (p->GetSection() == CInnoScript::SEC_ICONS) {
		CString strDir(p->GetParameter(_T("Name")));
		int nPos = strDir.ReverseFind(_T('\\'));
		if (nPos >= 0)
			ref = strDir.Left(nPos);
	} else if (p->GetSection() == CInnoScript::SEC_FILES) {
		ref = p->GetParameter(_T("DestDir"));
	} else if (p->GetSection() == CInnoScript::SEC_DIRS) {
		ref = p->GetParameter(_T("Name"));
	} else if (p->GetSection() == CInnoScript::SEC_REGISTRY) {
		LPCTSTR pszRoot = p->GetParameter(_T("Root"));
		LPCTSTR pszSubkey = p->GetParameter(_T("Subkey"));
		if (pszRoot) pszRoot = GetRootName(pszRoot);
		if (!pszRoot) pszRoot = _T("");
		if (!pszSubkey) pszSubkey = _T("");
		ref.Format(_T("%s\\%s"), pszRoot, pszSubkey);
	} else {
#ifdef _DEBUG
		AtlMessageBox(AfxGetMainHWnd(), _T("Unhandled script section specified in GetFolderName()."), IDR_MAINFRAME, MB_OK | MB_ICONERROR);
		ATLASSERT(FALSE);
#endif
	}

	if (!ref.IsEmpty() && ref[ref.GetLength() - 1] == _T('\\'))
		ref.ReleaseBuffer(ref.GetLength() - 1);

	return ref.IsEmpty() ? false : true;
}

const CInnoScriptEx::CConstantInfo CInnoScriptEx::m_constants[] = {
	// Directory Constants 
	_T("{app}"),			_T("The application directory"), true,
	_T("{win}"),			_T("The system's Windows directory"), true,
	_T("{sys}"),			_T("The system's Windows System directory"), true,
	_T("{src}"),			_T("The directory in which the Setup files are located"), true,
	_T("{sd}"),				_T("System Drive"), true,
	_T("{pf}"),				_T("Program Files"), true,
	_T("{cf}"),				_T("Common Files"), true,
	_T("{tmp}"),			_T("Temporary directory"), true,
	_T("{fonts}"),			_T("Fonts directory"), true,
	_T("{dao}"),			_T("DAO directory"), true,
	_T("{pf32}"),			_T("Program Files"), true,
	_T("{pf64}"),			_T("Program Files"), true,
	_T("{cf32}"),			_T("Common Files"), true,
	_T("{cf64}"),			_T("Common Files"), true,

	// Shell Folder Constants 
	_T("{group}"),			_T("The path to the program group"), true,
	_T("{sendto}"),			_T("The path to the current user's Send To folder"), true,

	_T("{userappdata}"),	_T("The path to the Application Data folder"), true,
	_T("{commonappdata}"),	_T("The path to the Application Data folder"), true,
	_T("{userdesktop}"),	_T("The path to the desktop folder"), true,
	_T("{commondesktop}"),	_T("The path to the desktop folder"), true,
	_T("{userdocs}"),		_T("The path to the My Documents folder"), true,
	_T("{commondocs}"),		_T("The path to the My Documents folder"), true,
	_T("{userfavorites}"),	_T("The path to the Favorites folder"), true,
	_T("{commonfavorites}"),_T("The path to the Favorites folder"), true,
	_T("{localappdata}"),	_T("The path to the Application Data folder"), true,

	_T("{userprograms}"),	_T("The path to the Programs folder on the Start Menu"), true,
	_T("{commonprograms}"), _T("The path to the Programs folder on the Start Menu"), true,
	_T("{userstartmenu}"),	_T("The path to the top level of the Start Menu"), true,
	_T("{commonstartmenu}"),_T("The path to the top level of the Start Menu"), true,
	_T("{userstartup}"),	_T("The path to the Startup folder on the Start Menu"), true,
	_T("{commonstartup}"),	_T("The path to the Startup folder on the Start Menu"), true,
	_T("{usertemplates}"),	_T("The path to the Templates folder"), true,
	_T("{commontemplates}"),_T("The path to the Templates folder"), true,

	// Other Constants 
	_T("{\\}"),												_T("A backslash character"), false,
	_T("{%NAME,DefaultValue}"),								_T("Embeds an environment variable"), false,
	_T("{computername}"),									_T("The name of the computer"), false,
	_T("{groupname}"),										_T("Group selected on Select Program Group wizard page"), false,
	_T("{hwnd}"),											_T("Handle of the Setup program's background window"), false,
	_T("{reg:HKxx\\SubkeyName,ValueName|DefaultValue}"),	_T("Embeds a registry value"), false,
	_T("{ini:filename,section,key|defaultvalue}"),			_T("Embeds a ini value"), false,
	_T("{param:parametername|defaultvalue}"),				_T("Embeds a command line parameter value"), false,
	_T("{code:FunctionName|DefaultValue}"),					_T("Embeds a code value"), false,
	_T("{srcexe}"),											_T("Full pathname of the Setup program file"), false,
	_T("{username}"),										_T("Name of user who is running Setup program"), false,
	_T("{wizardhwnd}"),										_T("Handle to wizard window"), false,
	_T("{userinfoname}"),									_T("Name entered by the user"), false,
	_T("{userinfoorg}"),									_T("Organization entered by the user"), false,
	_T("{sysuserinfoname}"),								_T("Name that Windows is registered to"), false,
	_T("{sysuserinfoorg}"),									_T("Organization that Windows is registered to"), false,
	_T("{uninstallexe}"),									_T("Full path and filename of the uninstall program"), false,
	_T("{cmd}"),											_T("System's command interpreter"), false,
	_T("{language}"),										_T("The current setup language"), false,
	_T("{drive:...}"),										_T("Extracts drive from path"), false,
	_T("{syswow64}"),										_T("?"), false,
	_T("{log}"),											_T("The log file name, or an empty string if logging is not enabled."), false,
	_T("{regasmexe}"),										_T("?"), false,
	_T("{regasmexe32}"),									_T("?"), false,
	_T("{regasmexe64}"),									_T("?"), false,

	NULL,NULL,false
};

bool CInnoScriptEx::IsFolderConstant(LPCTSTR pszConstant) {
	UINT nPos = 0;
	while (m_constants[nPos].m_pszConstant) {
		if (!_tcsicmp(pszConstant, m_constants[nPos].m_pszConstant))
			return true;
		nPos++;
	}
	return false;
}

LPCTSTR CInnoScriptEx::GetConstantName(LPCTSTR pszConstant) {
	UINT nPos = 0;
	while (m_constants[nPos].m_pszConstant) {
		if (!_tcsicmp(pszConstant, m_constants[nPos].m_pszConstant))
			return m_constants[nPos].m_pszName;
		nPos++;
	}
	return pszConstant;
}

LPCTSTR CInnoScriptEx::GetRootName(LPCTSTR pszRoot) {
	if (!pszRoot) return NULL;
	if (!_tcsicmp(pszRoot, _T("HKCR")))
		return _T("HKEY_CLASSES_ROOT");
	else if (!_tcsicmp(pszRoot, _T("HKCU")))
		return _T("HKEY_CURRENT_USER");
	else if (!_tcsicmp(pszRoot, _T("HKLM")))
		return _T("HKEY_LOCAL_MACHINE");
	else if (!_tcsicmp(pszRoot, _T("HKU")))
		return _T("HKEY_USERS");
	else if (!_tcsicmp(pszRoot, _T("HKCC")))
		return _T("HKEY_CURRENT_CONFIG");
	return NULL;
}

LPCTSTR CInnoScriptEx::GetDisplayName(CScriptLine* p) {
	if (p->GetSection() == CInnoScript::SEC_DIRS) {
		LPCTSTR pszName = p->GetParameter(_T("Name"));
		LPCTSTR psz = _tcsrchr(pszName, _T('\\'));
		if (psz)
			return psz + 1;
		return pszName;
	} else if (p->GetSection() == CInnoScript::SEC_FILES) {
		LPCTSTR pszName = p->GetParameter(_T("DestName"));
		if (pszName) return pszName;

		pszName = p->GetParameter(_T("Source"));
		LPCTSTR psz = _tcsrchr(pszName, _T('\\'));
		if (psz)
			return psz + 1;
		return pszName;
	} else if (p->GetSection() == CInnoScript::SEC_ICONS) {
		LPCTSTR pszName = p->GetParameter(_T("Name"));
		LPCTSTR psz = _tcsrchr(pszName, _T('\\'));
		if (psz)
			return psz + 1;
		return pszName;
	} else if (p->GetSection() == CInnoScript::SEC_REGISTRY) {
		return p->GetParameter(_T("ValueName"));
	} else {
#ifdef _DEBUG
		AtlMessageBox(AfxGetMainHWnd(), _T("Unhandled script section specified in GetDisplayName()."), IDR_MAINFRAME, MB_OK | MB_ICONERROR);
		ATLASSERT(FALSE);
#endif
	}

	return NULL;
}

void CInnoScriptEx::Split(const CString& Str, TSetupVersionDataVersion& Ver, WORD& ServicePack) {
	//void Split(const CString& Str,DWORD& ver,WORD& ServicePack) {
	int I, J;
	CString Z, B;
	bool HasBuild;

	//Ver = 0;
	memset(&Ver, 0, sizeof Ver);
	ServicePack = 0;
	Z = Str;
	Z.MakeLower();

	I = Z.Find(_T("sp"));
	if (I >= 0) {
		J = _ttol(Z.Mid(I + 2));
		if (J < 0 || J>255) throw 0;
		ServicePack = J;
		Z.SetAt(I, 0);
		Z.ReleaseBuffer();
	}

	I = Z.Find(_T("."));
	if (I == Z.GetLength() - 1) throw 0;
	if (I >= 0) {
		J = _ttol(Z.Left(I));
		if (J < 0 || J>255) throw 0;
		Ver.Major = J;
		Z = Z.Mid(I + 1);

		I = Z.Find(_T("."));
		HasBuild = I >= 0;
		if (!HasBuild) {
			I = Z.GetLength();
			B.Empty();
		} else {
			B = Z.Mid(I + 1);
			Z = Z.Left(I);
		}
		J = _ttol(Z);
		if (J < 0 || J>99) throw 0;
		if (J < 10 && Z[0] != _T('0')) J *= 10;
		Ver.Minor = J;
		if (HasBuild) {
			J = _ttol(B);
			if (J < 0 || J>65535) throw 0;
			Ver.Build = J;
		}
	} else { // no minor version specified }
		J = _ttol(Str);
		if (J < 0 || J>255) throw 0;
		Ver.Major = J;
	}
}

bool CInnoScriptEx::StrToVersionNumbers(const CString& S, TSetupVersionData& VerData) {
	int I;
	WORD SP;

	try {
		I = S.Find(_T(","));
		if (I < 0) throw 0;

		CString tmp;
		tmp = S.Left(I);
		tmp.TrimLeft(), tmp.TrimRight();
		Split(tmp, VerData.WinVersion, SP);
		if (SP) throw 0; // only NT has service packs
		tmp = S.Mid(I + 1);
		tmp.TrimLeft(), tmp.TrimRight();
		Split(tmp, VerData.NTVersion, VerData.NTServicePack);
		return true;
	} catch (...) {
		return false;
	}
}

CString CInnoScriptEx::GetMessage(const CString& strName, CString strHomeDir) {
	CMyUtils::EndWith(strHomeDir, _T('\\'));
	CString strMessagesFile;
	CScriptList lines;
	GetList(CInnoScript::SEC_LANGUAGES, lines);
	if (lines.GetSize() == 0)
		strMessagesFile = _T("compiler:default.isl");
	else
		strMessagesFile = lines[0]->GetParameter(_T("MessagesFile"));

	long nPos = strMessagesFile.Trim().ReverseFind(_T(','));
	do {
		CString strFile;
		if (nPos < 0) {
			strFile = strMessagesFile;
			strMessagesFile.Empty();
		} else {
			strFile = strMessagesFile.Mid(nPos + 1);
			strMessagesFile.ReleaseBufferSetLength(nPos);
		}

		strFile.Trim().Replace(_T("compiler:"), strHomeDir);

		CString strMessage;
		DWORD dwChars = GetPrivateProfileString(_T("Messages"), strName, NULL, strMessage.GetBuffer(1024), 1024, strFile);
		strMessage.ReleaseBufferSetLength(dwChars);
		if (!strMessage.IsEmpty())
			return strMessage;

		nPos = strMessagesFile.Trim().ReverseFind(_T(','));
	} while (!strMessagesFile.IsEmpty());


	return _T("strMessagesFile");
}
