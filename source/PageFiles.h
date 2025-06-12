// WTL
#pragma once

#include "MyDoc.h"
#include "ComboBox2.h"

/////////////////////////////////////////////////////////////////////////////
// CPageFiles dialog

class CPageFiles :
	public CPropertyPageImpl<CPageFiles>,
	public CMyPropertyPageBase<CPageFiles>,
	public CWinDataExchange<CPageFiles>,
	public Henden::CToolTipDialog<CPageFiles>
{
public:
	enum { IDD = IDD_FILES };

	BEGIN_MSG_MAP(CPageFiles)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPageFiles>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageFiles>)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CPageFiles>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_FONTINSTALL, EN_CHANGE, OnChangeFontInstall)
		COMMAND_HANDLER(IDC_READONLY, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_EXTERNAL, BN_CLICKED, OnExternal)
		COMMAND_HANDLER(IDC_SOURCE, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_HIDDEN, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_SYSTEM, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_COMPARETIMESTAMP, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_CONFIRMOVERWRITE, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_DELETEAFTERINSTALL, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_FONTISNTTRUETYPE, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_ISREADME, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_REGSERVER, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_RESTARTREPLACE, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_SHAREDFILE, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_UNINSNEVERUNINSTALL, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_REGTYPELIB, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_SKIPIFSOURCEDOESNTEXIST, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_DESTNAME, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_OVERWRITEREADONLY, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_DESTDIR, CBN_EDITCHANGE, OnModified)
		COMMAND_HANDLER(IDC_DESTDIR, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_FILES_ONLYIFDESTFILEEXISTS, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_RECURSESUBDIRS, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_FILES_NOREGERROR, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_UNINSRESTARTDELETE, BN_CLICKED, OnModified)
		// IS 3.0.5
		COMMAND_HANDLER(IDC_PROMPTIFOLDER, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_ONLYIFDOESNTEXIST, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_IGNOREVERSION, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_DONTCOPY, BN_CLICKED, OnModified)
		// IS 4.0.5
		COMMAND_HANDLER(IDC_UNINSREMOVEREADONLY, BN_CLICKED, OnModified)
		// IS 4.1.0
		COMMAND_HANDLER(IDC_PERMISSIONS, EN_CHANGE, OnModified)
		// IS 4.1.6
		COMMAND_HANDLER(IDC_EXCLUDES, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_SORTFILESBYEXTENSION, BN_CLICKED, OnModified)
		// IS 4.2.0
		COMMAND_HANDLER(IDC_TOUCH, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_REPLACESAMEVERSION, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_NOENCRYPTION, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_NOCOMPRESSION, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_DONTVERIFYCHECKSUM, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_UNINSNOSHAREDFILEPROMPT, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_CREATEALLSUBDIRS, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_32BIT, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_64BIT, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_FILES_SOLIDBREAK, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_EXTERNALSIZE, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_SETNTFSCOMPRESSION, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_UNSETNTFSCOMPRESSION, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_SORTFILESBYNAME, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_GACINSTALL, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_STRONGASSEMBLYNAME, EN_CHANGE, OnModified)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageFiles)
		DDX_TEXT(IDC_SOURCE, m_strSource)
		DDX_TEXT(IDC_DESTDIR, m_strDestDir)
		DDX_TEXT(IDC_DESTNAME, m_strDestName)
		DDX_TEXT(IDC_FONTINSTALL, m_strFontInstall)
		DDX_CHECK(IDC_HIDDEN, m_bHidden)
		DDX_CHECK(IDC_READONLY, m_bReadOnly)
		DDX_CHECK(IDC_SYSTEM, m_bSystem)
		DDX_CHECK(IDC_COMPARETIMESTAMP, m_bCompareTimeStamp)
		DDX_CHECK(IDC_CONFIRMOVERWRITE, m_bConfirmOverwrite)
		DDX_CHECK(IDC_DELETEAFTERINSTALL, m_bDeleteAfterInstall)
		DDX_CHECK(IDC_FONTISNTTRUETYPE, m_bFontIsntTrueType)
		DDX_CHECK(IDC_ISREADME, m_bIsReadMe)
		DDX_CHECK(IDC_REGSERVER, m_bRegServer)
		DDX_CHECK(IDC_RESTARTREPLACE, m_bRestartReplace)
		DDX_CHECK(IDC_SHAREDFILE, m_bSharedFile)
		DDX_CHECK(IDC_UNINSNEVERUNINSTALL, m_bUninsNeverUninstall)
		DDX_CHECK(IDC_EXTERNAL, m_bExternal)
		DDX_CHECK(IDC_REGTYPELIB, m_bRegTypeLib)
		DDX_CHECK(IDC_SKIPIFSOURCEDOESNTEXIST, m_bSkipIfSourceDoesntExist)
		DDX_CHECK(IDC_OVERWRITEREADONLY, m_bOverwriteReadOnly)
		DDX_CHECK(IDC_FILES_ONLYIFDESTFILEEXISTS, m_nOnlyIfDestFileExists)
		DDX_CHECK(IDC_RECURSESUBDIRS, m_bRecurseSubDirs)
		DDX_CHECK(IDC_FILES_NOREGERROR, m_bNoRegError)
		DDX_CHECK(IDC_UNINSRESTARTDELETE, m_nUninsRestartDelete)
		// IS 3.0.5
		DDX_CHECK(IDC_PROMPTIFOLDER, m_nPromptIfOlder)
		DDX_CHECK(IDC_ONLYIFDOESNTEXIST, m_nOnlyIfDoesntExist)
		DDX_CHECK(IDC_IGNOREVERSION, m_nIgnoreVersion)
		DDX_CHECK(IDC_DONTCOPY, m_nDontCopy)
		// IS 4.0.5
		DDX_CHECK(IDC_UNINSREMOVEREADONLY, m_nUninsRemoveReadOnly)
		// IS 4.1.0
		DDX_TEXT(IDC_PERMISSIONS, m_strPermissions)
		// IS 4.1.6
		DDX_TEXT(IDC_EXCLUDES, m_strExcludes)
		DDX_CHECK(IDC_SORTFILESBYEXTENSION, m_nSortFilesByExtension)
		// IS 4.2.0
		DDX_CHECK(IDC_TOUCH, m_nTouch)
		DDX_CHECK(IDC_REPLACESAMEVERSION, m_nReplaceSameVersion)
		DDX_CHECK(IDC_NOENCRYPTION, m_nNoEncryption)
		DDX_CHECK(IDC_NOCOMPRESSION, m_nNoCompression)
		DDX_CHECK(IDC_DONTVERIFYCHECKSUM, m_nDontVerifyChecksum)
		DDX_CHECK(IDC_UNINSNOSHAREDFILEPROMPT, m_nUninsNoSharedFilePrompt)
		DDX_CHECK(IDC_CREATEALLSUBDIRS, m_nCreateAllSubdirs)
		DDX_CHECK(IDC_32BIT, m_n32Bit)
		DDX_CHECK(IDC_64BIT, m_n64Bit)
		DDX_CHECK(IDC_FILES_SOLIDBREAK, m_nSolidBreak)
		DDX_TEXT(IDC_EXTERNALSIZE, m_strExternalSize)
		DDX_CHECK(IDC_SETNTFSCOMPRESSION, m_nSetNTFSCompression)
		DDX_CHECK(IDC_UNSETNTFSCOMPRESSION, m_nUnsetNTFSCompression)
		DDX_CHECK(IDC_SORTFILESBYNAME, m_nSortFilesByName)
		DDX_CHECK(IDC_GACINSTALL, m_nGacInstall)
		DDX_TEXT(IDC_STRONGASSEMBLYNAME, m_strStrongAssemblyName)
	END_DDX_MAP()

	CEdit2		m_wndSource;
	CComboBox2	m_wndDestDir;
	Henden::CButtonFile	m_btnSource;
	CButton		m_wndFontIsntTrueType;
	CString		m_strSource;
	CString		m_strDestDir;
	CString		m_strDestName;
	CString		m_strFontInstall;
	BOOL		m_bHidden;
	BOOL		m_bReadOnly;
	BOOL		m_bSystem;
	BOOL		m_bCompareTimeStamp;
	BOOL		m_bConfirmOverwrite;
	BOOL		m_bDeleteAfterInstall;
	BOOL		m_bFontIsntTrueType;
	BOOL		m_bIsReadMe;
	BOOL		m_bRegServer;
	BOOL		m_bRestartReplace;
	BOOL		m_bSharedFile;
	BOOL		m_bUninsNeverUninstall;
	BOOL		m_bExternal;
	BOOL		m_bRegTypeLib;
	BOOL		m_bSkipIfSourceDoesntExist;
	BOOL		m_bOverwriteReadOnly;
	int			m_nOnlyIfDestFileExists;
	BOOL		m_bRecurseSubDirs;
	BOOL		m_bNoRegError;
	int			m_nUninsRestartDelete;
	int			m_nPromptIfOlder, m_nOnlyIfDoesntExist, m_nIgnoreVersion, m_nDontCopy;
	int			m_nUninsRemoveReadOnly;
	CString		m_strPermissions;
	CString		m_strExcludes, m_strExternalSize, m_strStrongAssemblyName;
	int			m_nSortFilesByExtension;
	int			m_nTouch, m_nReplaceSameVersion, m_nNoEncryption;
	int			m_nNoCompression, m_nDontVerifyChecksum;
	int			m_nUninsNoSharedFilePrompt;
	int			m_nCreateAllSubdirs, m_n32Bit, m_n64Bit, m_nSolidBreak, m_nSetNTFSCompression, m_nUnsetNTFSCompression;
	int			m_nSortFilesByName, m_nGacInstall;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
#if 1
		_L(m_hWnd, _T("Files"));
#else
		CTransDialog trans(m_hWnd);
		trans.Translate();
#endif
		m_wndSource.SubclassWindow(GetDlgItem(IDC_SOURCE));
		m_wndDestDir.SubclassWindow(GetDlgItem(IDC_DESTDIR));
		m_btnSource.SubclassWindow(GetDlgItem(IDC_BTN_SOURCE));
		m_wndFontIsntTrueType.Attach(GetDlgItem(IDC_FONTISNTTRUETYPE));

		m_wndFontIsntTrueType.EnableWindow(!m_strFontInstall.IsEmpty());
		m_pDoc->AddDirConstants(m_wndDestDir);
		m_pDoc->AddDirs(m_wndDestDir);

		for (int nPos = 0; nPos < m_listFiles.GetSize(); nPos++) {
			CScriptLine* pFile = m_listFiles[nPos];

			if (m_strSource.CompareNoCase(SAFESTR(pFile->GetParameter(_T("Source")))))
				m_strSource.Empty();

			if (m_strDestDir.CompareNoCase(SAFESTR(pFile->GetParameter(_T("DestDir")))))
				m_strDestDir.Empty();

			if (m_strDestName.CompareNoCase(SAFESTR(pFile->GetParameter(_T("DestName")))))
				m_strDestName.Empty();

			if (m_strFontInstall.CompareNoCase(SAFESTR(pFile->GetParameter(_T("FontInstall")))))
				m_strFontInstall.Empty();

			if (m_strPermissions.CompareNoCase(SAFESTR(pFile->GetParameter(_T("Permissions")))))
				m_strPermissions.Empty();

			if (m_strExcludes.CompareNoCase(SAFESTR(pFile->GetParameter(_T("Excludes")))))
				m_strExcludes.Empty();

			if (m_strExternalSize.CompareNoCase(SAFESTR(pFile->GetParameter(_T("ExternalSize")))))
				m_strExternalSize.Empty();

			if (m_strStrongAssemblyName.CompareNoCase(SAFESTR(pFile->GetParameter(_T("StrongAssemblyName")))))
				m_strStrongAssemblyName.Empty();

			// Attributes
			if (m_bReadOnly != (pFile->GetParameterFlag(_T("Attribs"), _T("readonly")) ? TRUE : FALSE)) {
				MyModifyStyle(IDC_READONLY, BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_bReadOnly = 2;
			}

			if (m_bHidden != (pFile->GetParameterFlag(_T("Attribs"), _T("hidden")) ? TRUE : FALSE)) {
				MyModifyStyle(IDC_HIDDEN, BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_bHidden = 2;
			}

			if (m_bSystem != (pFile->GetParameterFlag(_T("Attribs"), _T("system")) ? TRUE : FALSE)) {
				MyModifyStyle(IDC_SYSTEM, BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_bSystem = 2;
			}

			// Flags
			if (m_bCompareTimeStamp != (pFile->GetParameterFlag(_T("Flags"), _T("comparetimestamp")) ? TRUE : FALSE)) {
				MyModifyStyle(IDC_COMPARETIMESTAMP, BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_bCompareTimeStamp = 2;
			}

			if (m_bConfirmOverwrite != (pFile->GetParameterFlag(_T("Flags"), _T("confirmoverwrite")) ? TRUE : FALSE)) {
				MyModifyStyle(IDC_CONFIRMOVERWRITE, BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_bConfirmOverwrite = 2;
			}

			if (m_bDeleteAfterInstall != (pFile->GetParameterFlag(_T("Flags"), _T("deleteafterinstall")) ? TRUE : FALSE)) {
				MyModifyStyle(IDC_DELETEAFTERINSTALL, BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_bDeleteAfterInstall = 2;
			}

			if (m_bFontIsntTrueType != (pFile->GetParameterFlag(_T("Flags"), _T("fontisnttruetype")) ? TRUE : FALSE)) {
				MyModifyStyle(IDC_FONTISNTTRUETYPE, BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_bFontIsntTrueType = 2;
			}

			if (m_bIsReadMe != (pFile->GetParameterFlag(_T("Flags"), _T("isreadme")) ? TRUE : FALSE)) {
				MyModifyStyle(IDC_ISREADME, BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_bIsReadMe = 2;
			}

			if (m_bRegServer != (pFile->GetParameterFlag(_T("Flags"), _T("regserver")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_REGSERVER).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_bRegServer = 2;
			}

			if (m_bRestartReplace != (pFile->GetParameterFlag(_T("Flags"), _T("restartreplace")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_RESTARTREPLACE).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_bRestartReplace = 2;
			}

			if (m_bSharedFile != (pFile->GetParameterFlag(_T("Flags"), _T("sharedfile")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_SHAREDFILE).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_bSharedFile = 2;
			}

			if (m_bUninsNeverUninstall != (pFile->GetParameterFlag(_T("Flags"), _T("uninsneveruninstall")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_UNINSNEVERUNINSTALL).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_bUninsNeverUninstall = 2;
			}

			if (m_bExternal != (pFile->GetParameterFlag(_T("Flags"), _T("external")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_EXTERNAL).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_bExternal = 2;
			}

			if (m_bRegTypeLib != (pFile->GetParameterFlag(_T("Flags"), _T("regtypelib")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_REGTYPELIB).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_bRegTypeLib = 2;
			}

			if (m_bSkipIfSourceDoesntExist != (pFile->GetParameterFlag(_T("Flags"), _T("skipifsourcedoesntexist")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_SKIPIFSOURCEDOESNTEXIST).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_bSkipIfSourceDoesntExist = 2;
			}

			if (m_bOverwriteReadOnly != (pFile->GetParameterFlag(_T("Flags"), _T("overwritereadonly")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_OVERWRITEREADONLY).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_bOverwriteReadOnly = 2;
			}

			if (m_nOnlyIfDestFileExists != (pFile->GetParameterFlag(_T("Flags"), _T("onlyifdestfileexists")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_FILES_ONLYIFDESTFILEEXISTS).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_nOnlyIfDestFileExists = 2;
			}

			if (m_bRecurseSubDirs != (pFile->GetParameterFlag(_T("Flags"), _T("recursesubdirs")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_RECURSESUBDIRS).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_bRecurseSubDirs = 2;
			}

			if (m_bNoRegError != (pFile->GetParameterFlag(_T("Flags"), _T("noregerror")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_FILES_NOREGERROR).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_bNoRegError = 2;
			}

			if (m_nUninsRestartDelete != (pFile->GetParameterFlag(_T("Flags"), _T("uninsrestartdelete")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_UNINSRESTARTDELETE).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_nUninsRestartDelete = 2;
			}
			// IS 3.0.5
			if (m_nPromptIfOlder != (pFile->GetParameterFlag(_T("Flags"), _T("promptifolder")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_PROMPTIFOLDER).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_nPromptIfOlder = 2;
			}
			if (m_nOnlyIfDoesntExist != (pFile->GetParameterFlag(_T("Flags"), _T("onlyifdoesntexist")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_ONLYIFDOESNTEXIST).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_nOnlyIfDoesntExist = 2;
			}
			if (m_nIgnoreVersion != (pFile->GetParameterFlag(_T("Flags"), _T("ignoreversion")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_IGNOREVERSION).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_nIgnoreVersion = 2;
			}
			if (m_nDontCopy != (pFile->GetParameterFlag(_T("Flags"), _T("dontcopy")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_DONTCOPY).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_nDontCopy = 2;
			}
			// IS 4.0.5
			if (m_nUninsRemoveReadOnly != (pFile->GetParameterFlag(_T("Flags"), _T("uninsremovereadonly")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_UNINSREMOVEREADONLY).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_nUninsRemoveReadOnly = 2;
			}
			// IS 4.1.6
			if (m_nSortFilesByExtension != (pFile->GetParameterFlag(_T("Flags"), _T("sortfilesbyextension")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_SORTFILESBYEXTENSION).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_nSortFilesByExtension = 2;
			}
			// IS 4.2.0
			if (m_nTouch != (pFile->GetParameterFlag(_T("Flags"), _T("touch")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_TOUCH).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_nTouch = 2;
			}
			if (m_nReplaceSameVersion != (pFile->GetParameterFlag(_T("Flags"), _T("replacesameversion")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_REPLACESAMEVERSION).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_nReplaceSameVersion = 2;
			}
			if (m_nNoEncryption != (pFile->GetParameterFlag(_T("Flags"), _T("noencryption")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_NOENCRYPTION).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_nNoEncryption = 2;
			}
			if (m_nNoCompression != (pFile->GetParameterFlag(_T("Flags"), _T("nocompression")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_NOCOMPRESSION).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_nNoCompression = 2;
			}
			if (m_nDontVerifyChecksum != (pFile->GetParameterFlag(_T("Flags"), _T("dontverifychecksum")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_DONTVERIFYCHECKSUM).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_nDontVerifyChecksum = 2;
			}
			if (m_nUninsNoSharedFilePrompt != (pFile->GetParameterFlag(_T("Flags"), _T("uninsnosharedfileprompt")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_UNINSNOSHAREDFILEPROMPT).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_nUninsNoSharedFilePrompt = 2;
			}
			if (m_nCreateAllSubdirs != (pFile->GetParameterFlag(_T("Flags"), _T("createallsubdirs")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_CREATEALLSUBDIRS).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_nCreateAllSubdirs = 2;
			}
			if (m_n32Bit != (pFile->GetParameterFlag(_T("Flags"), _T("32bit")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_32BIT).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_n32Bit = 2;
			}
			if (m_n64Bit != (pFile->GetParameterFlag(_T("Flags"), _T("64bit")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_64BIT).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_n64Bit = 2;
			}
			if (m_nSolidBreak != (pFile->GetParameterFlag(_T("Flags"), _T("solidbreak")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_FILES_SOLIDBREAK).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_nSolidBreak = 2;
			}
			if (m_nSetNTFSCompression != (pFile->GetParameterFlag(_T("Flags"), _T("setntfscompression")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_SETNTFSCOMPRESSION).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_nSetNTFSCompression = 2;
			}
			if (m_nUnsetNTFSCompression != (pFile->GetParameterFlag(_T("Flags"), _T("unsetntfscompression")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_UNSETNTFSCOMPRESSION).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_nUnsetNTFSCompression = 2;
			}
			if (m_nSortFilesByName != (pFile->GetParameterFlag(_T("Flags"), _T("sortfilesbyname")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_SORTFILESBYNAME).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_nSortFilesByName = 2;
			}
			if (m_nGacInstall != (pFile->GetParameterFlag(_T("Flags"), _T("gacinstall")) ? TRUE : FALSE)) {
				GetDlgItem(IDC_GACINSTALL).ModifyStyle(BS_CHECKBOX | BS_3STATE | BS_AUTO3STATE, BS_AUTO3STATE);
				m_nGacInstall = 2;
			}
		}
		GetDlgItem(IDC_EXTERNALSIZE).EnableWindow(m_bExternal);
		GetDlgItem(IDC_STATIC_EXTERNALSIZE).EnableWindow(m_bExternal);
		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		const bool bForce = m_listFiles.GetSize() == 1;

		if (bForce) {
			if (m_strSource.IsEmpty()) {
				AtlMessageBox(m_hWnd, _L(_T("You must enter a source.")), IDR_MAINFRAME, MB_OK | MB_ICONERROR);
				GetDlgItem(IDC_SOURCE).SetFocus();
				return PSNRET_INVALID;
			}
			if (m_strDestDir.IsEmpty() && !m_nDontCopy) {
				AtlMessageBox(m_hWnd, _L(_T("You must enter a destination directory.")), IDR_MAINFRAME, MB_OK | MB_ICONERROR);
				GetDlgItem(IDC_DESTDIR).SetFocus();
				return PSNRET_INVALID;
			}

			CString strSourceDir;
			if (!m_pDoc->GetUseAbsolutePaths() && m_pDoc->GetSourceDir(strSourceDir)) {
				CString strSource = CFunc::ExtractRelativePath(strSourceDir, m_strSource);
				if (!strSource.IsEmpty()) {
					m_strSource = strSource;
					DoDataExchange(DDX_LOAD, IDC_SOURCE);
				}
			}
		}

#ifdef TODO
		if ((bForce || !m_strSource.IsEmpty()) && m_bExternal != 1) {
			if (!CMyUtils::IsFile(m_strSource))
				if (AfxMessageBox(m_strSource + _T(" doesn't exist. Continue?"), MB_YESNO | MB_ICONQUESTION) != IDYES)
					return PSNRET_INVALID;
		}
#endif

		for (int nPos = 0; nPos < m_listFiles.GetSize(); nPos++) {
			CScriptLine* pFile = m_listFiles[nPos];

			CString strOld, strNew;
			CInnoScriptEx::GetDestName(pFile, strOld);

			CInnoScriptEx::SetString(pFile, bForce, _T("Source"), m_strSource);
			CInnoScriptEx::SetString(pFile, bForce, _T("DestDir"), m_strDestDir);
			CInnoScriptEx::SetString(pFile, bForce, _T("DestName"), m_strDestName);

			CInnoScriptEx::GetDestName(pFile, strNew);
			// If destination file has changed, check if references should be changed also
			if (strNew.CompareNoCase(strOld)) m_pDoc->RenameFile(m_hWnd, strOld, strNew);

			CInnoScriptEx::SetString(pFile, bForce, _T("FontInstall"), m_strFontInstall);
			CInnoScriptEx::SetString(pFile, bForce, _T("Permissions"), m_strPermissions);
			CInnoScriptEx::SetString(pFile, bForce, _T("Excludes"), m_strExcludes);
			CInnoScriptEx::SetString(pFile, bForce, _T("ExternalSize"), m_strExternalSize);
			CInnoScriptEx::SetString(pFile, bForce, _T("StrongAssemblyName"), m_strStrongAssemblyName);

			// Attribs
			CInnoScriptEx::SetFlag(pFile, _T("Attribs"), _T("readonly"), m_bReadOnly);
			CInnoScriptEx::SetFlag(pFile, _T("Attribs"), _T("hidden"), m_bHidden);
			CInnoScriptEx::SetFlag(pFile, _T("Attribs"), _T("system"), m_bSystem);

			// Flags
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("comparetimestamp"), m_bCompareTimeStamp);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("confirmoverwrite"), m_bConfirmOverwrite);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("deleteafterinstall"), m_bDeleteAfterInstall);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("fontisnttruetype"), m_bFontIsntTrueType);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("isreadme"), m_bIsReadMe);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("regserver"), m_bRegServer);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("restartreplace"), m_bRestartReplace);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("sharedfile"), m_bSharedFile);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("uninsneveruninstall"), m_bUninsNeverUninstall);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("external"), m_bExternal);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("regtypelib"), m_bRegTypeLib);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("skipifsourcedoesntexist"), m_bSkipIfSourceDoesntExist);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("overwritereadonly"), m_bOverwriteReadOnly);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("onlyifdestfileexists"), m_nOnlyIfDestFileExists);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("recursesubdirs"), m_bRecurseSubDirs);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("noregerror"), m_bNoRegError);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("uninsrestartdelete"), m_nUninsRestartDelete);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("promptifolder"), m_nPromptIfOlder);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("onlyifdoesntexist"), m_nOnlyIfDoesntExist);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("ignoreversion"), m_nIgnoreVersion);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("dontcopy"), m_nDontCopy);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("uninsremovereadonly"), m_nUninsRemoveReadOnly);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("sortfilesbyextension"), m_nSortFilesByExtension);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("touch"), m_nTouch);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("replacesameversion"), m_nReplaceSameVersion);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("noencryption"), m_nNoEncryption);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("nocompression"), m_nNoCompression);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("dontverifychecksum"), m_nDontVerifyChecksum);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("uninsnosharedfileprompt"), m_nUninsNoSharedFilePrompt);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("createallsubdirs"), m_nCreateAllSubdirs);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("32bit"), m_n32Bit);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("64bit"), m_n64Bit);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("solidbreak"), m_nSolidBreak);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("setntfscompression"), m_nSetNTFSCompression);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("unsetntfscompression"), m_nUnsetNTFSCompression);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("sortfilesbyname"), m_nSortFilesByName);
			CInnoScriptEx::SetFlag(pFile, _T("Flags"), _T("gacinstall"), m_nGacInstall);
		}

		if (m_bNew)
			CFilesHelper(m_pDoc).AutoComponentSelect(m_listFiles);
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if (!m_bNew) SetModified();
		return 0;
	}

	LRESULT OnChangeFontInstall(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if (!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE, IDC_FONTINSTALL);
		m_wndFontIsntTrueType.EnableWindow(!m_strFontInstall.IsEmpty());
		return 0;
	}

	LRESULT OnExternal(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if (!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);
		GetDlgItem(IDC_EXTERNALSIZE).EnableWindow(m_bExternal);
		GetDlgItem(IDC_STATIC_EXTERNALSIZE).EnableWindow(m_bExternal);
		return 0;
	}

	CString m_strTitle;
	CPageFiles::CPageFiles(CScriptList& listFiles, CMyDoc* pDoc, const bool bNew) :
		m_listFiles(listFiles), m_pDoc(pDoc), m_bNew(bNew), m_wndDestDir(pDoc), m_wndSource(pDoc)
	{
		m_strTitle = _L(_T("DialogTitles|File"), _T("File"));
		SetTitle((LPCTSTR)m_strTitle);

		m_bHidden = FALSE;
		m_bReadOnly = FALSE;
		m_bSystem = FALSE;
		m_bCompareTimeStamp = FALSE;
		m_bConfirmOverwrite = FALSE;
		m_bDeleteAfterInstall = FALSE;
		m_bFontIsntTrueType = FALSE;
		m_bIsReadMe = FALSE;
		m_bRegServer = FALSE;
		m_bRestartReplace = FALSE;
		m_bSharedFile = FALSE;
		m_bUninsNeverUninstall = FALSE;
		m_bExternal = FALSE;
		m_bRegTypeLib = FALSE;
		m_bSkipIfSourceDoesntExist = FALSE;
		m_bOverwriteReadOnly = FALSE;
		m_nOnlyIfDestFileExists = 0;
		m_bRecurseSubDirs = FALSE;
		m_bNoRegError = FALSE;
		m_nUninsRestartDelete = FALSE;
		m_nPromptIfOlder = 0;
		m_nOnlyIfDoesntExist = 0;
		m_nIgnoreVersion = 0;
		m_nDontCopy = 0;
		m_nUninsRemoveReadOnly = 0;
		m_nSortFilesByExtension = 0;
		m_nTouch = 0;
		m_nReplaceSameVersion = 0;
		m_nNoEncryption = 0;
		m_nNoCompression = 0;
		m_nDontVerifyChecksum = 0;
		m_nUninsNoSharedFilePrompt = 0;
		m_nCreateAllSubdirs = 0;
		m_n32Bit = 0;
		m_n64Bit = 0;
		m_nSolidBreak = 0;
		m_nSetNTFSCompression = 0;
		m_nUnsetNTFSCompression = 0;
		m_nSortFilesByName = 0;
		m_nGacInstall = 0;

		for (int nPos = 0; nPos < m_listFiles.GetSize(); nPos++) {
			CScriptLine* pFile = m_listFiles[nPos];
			m_strSource = pFile->GetParameter(_T("Source"));
			m_strDestDir = pFile->GetParameter(_T("DestDir"));
			m_strDestName = pFile->GetParameter(_T("DestName"));
			m_strFontInstall = pFile->GetParameter(_T("FontInstall"));
			m_strPermissions = pFile->GetParameter(_T("Permissions"));
			m_strExcludes = pFile->GetParameter(_T("Excludes"));
			m_strExternalSize = pFile->GetParameter(_T("ExternalSize"));
			m_strStrongAssemblyName = pFile->GetParameter(_T("StrongAssemblyName"));

			if (pFile->GetParameterFlag(_T("Attribs"), _T("readonly"))) m_bReadOnly = TRUE;
			if (pFile->GetParameterFlag(_T("Attribs"), _T("hidden"))) m_bHidden = TRUE;
			if (pFile->GetParameterFlag(_T("Attribs"), _T("system"))) m_bSystem = TRUE;

			if (pFile->GetParameterFlag(_T("Flags"), _T("comparetimestamp"))) m_bCompareTimeStamp = TRUE;
			if (pFile->GetParameterFlag(_T("Flags"), _T("confirmoverwrite"))) m_bConfirmOverwrite = TRUE;
			if (pFile->GetParameterFlag(_T("Flags"), _T("deleteafterinstall"))) m_bDeleteAfterInstall = TRUE;
			if (pFile->GetParameterFlag(_T("Flags"), _T("fontisnttruetype"))) m_bFontIsntTrueType = TRUE;
			if (pFile->GetParameterFlag(_T("Flags"), _T("isreadme"))) m_bIsReadMe = TRUE;
			if (pFile->GetParameterFlag(_T("Flags"), _T("regserver"))) m_bRegServer = TRUE;
			if (pFile->GetParameterFlag(_T("Flags"), _T("restartreplace"))) m_bRestartReplace = TRUE;
			if (pFile->GetParameterFlag(_T("Flags"), _T("sharedfile"))) m_bSharedFile = TRUE;
			if (pFile->GetParameterFlag(_T("Flags"), _T("uninsneveruninstall"))) m_bUninsNeverUninstall = TRUE;
			if (pFile->GetParameterFlag(_T("Flags"), _T("external"))) m_bExternal = TRUE;
			if (pFile->GetParameterFlag(_T("Flags"), _T("regtypelib"))) m_bRegTypeLib = TRUE;
			if (pFile->GetParameterFlag(_T("Flags"), _T("skipifsourcedoesntexist"))) m_bSkipIfSourceDoesntExist = TRUE;
			if (pFile->GetParameterFlag(_T("Flags"), _T("overwritereadonly"))) m_bOverwriteReadOnly = TRUE;
			if (pFile->GetParameterFlag(_T("Flags"), _T("onlyifdestfileexists"))) m_nOnlyIfDestFileExists = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("recursesubdirs"))) m_bRecurseSubDirs = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("noregerror"))) m_bNoRegError = TRUE;
			if (pFile->GetParameterFlag(_T("Flags"), _T("uninsrestartdelete"))) m_nUninsRestartDelete = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("promptifolder"))) m_nPromptIfOlder = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("onlyifdoesntexist"))) m_nOnlyIfDoesntExist = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("ignoreversion"))) m_nIgnoreVersion = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("dontcopy"))) m_nDontCopy = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("uninsremovereadonly"))) m_nUninsRemoveReadOnly = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("sortfilesbyextension"))) m_nSortFilesByExtension = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("touch"))) m_nTouch = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("replacesameversion"))) m_nReplaceSameVersion = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("noencryption"))) m_nNoEncryption = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("nocompression"))) m_nNoCompression = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("dontverifychecksum"))) m_nDontVerifyChecksum = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("uninsnosharedfileprompt"))) m_nUninsNoSharedFilePrompt = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("createallsubdirs"))) m_nCreateAllSubdirs = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("32bit"))) m_n32Bit = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("64bit"))) m_n64Bit = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("solidbreak"))) m_nSolidBreak = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("setntfscompression"))) m_nSetNTFSCompression = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("unsetntfscompression"))) m_nUnsetNTFSCompression = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("sortfilesbyname"))) m_nSortFilesByName = 1;
			if (pFile->GetParameterFlag(_T("Flags"), _T("gacinstall"))) m_nGacInstall = 1;
			break;
		}
	}

	CScriptList& m_listFiles;
	CMyDoc* m_pDoc;
	const bool	m_bNew;

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_SOURCE, _L(_T("Help|Files|Source"), _T("The name of the source file. The compiler will prepend the path of your installation's source directory if you do not specify a fully qualified pathname.\r\n\r\nThis can be a wildcard to specify a group of files in a single entry. When a wildcard is used, all files matching it use the same options.\r\n\r\nWhen the flag external is specified, Source must be the full pathname of an existing file (or wildcard) on the distribution media or the user's system (e.g. \"{src}\\license.ini\").\r\n\r\nConstants may only be used when the external flag is specified, because the compiler does not do any constant translating itself.")))
		TOOLTIP_HANDLER(IDC_BTN_SOURCE, _L(_T("Help|Files|Source"), _T("Click this button to select source file.")))
		TOOLTIP_HANDLER(IDC_DESTDIR, _L(_T("Help|Files|DestDir"), _T("The directory where the file is to be installed on the user's system. The will almost always begin with one of the directory constants. If the specified path does not already exist on the user's system, it will be created automatically, and removed automatically during uninstallation if empty.")))
		TOOLTIP_HANDLER(IDC_FONTINSTALL, _L(_T("Help|Files|FontInstall"), _T("Tells Setup the file is a font that needs to be installed. The value of this parameter is the name of the font as stored in the registry or WIN.INI. This must be exactly the same name as you see when you double-click the font file in Explorer. Note that Setup will automatically append \" (TrueType)\" to the end of the name.\r\n\r\nIf the file is not a TrueType font, you must specify the flag fontisnttruetype in the Flags parameter.\r\n\r\nIt's recommended that you use the flags onlyifdoesntexist and uninsneveruninstall when installing fonts to the {fonts} directory.\r\n\r\nTo successfully install a font on Windows 2000/XP, the user must be a member of the Power Users or Administrators groups. On Windows NT 4.0 and earlier, anyone can install a font.")))
		TOOLTIP_HANDLER(IDC_READONLY, _L(_T("Help|Files|ReadOnly"), _T("Specifies additional attributes for the file.")))
		TOOLTIP_HANDLER(IDC_EXTERNAL, _L(_T("Help|Files|External"), _T("This flag instructs Inno Setup not to statically compile the file specified by the Source parameter into the installation files, but instead copy from an existing file on the distribution media or the user's system. See the Source parameter description for more information.")))
		TOOLTIP_HANDLER(IDC_HIDDEN, _L(_T("Help|Files|Hidden"), _T("Specifies additional attributes for the file.")))
		TOOLTIP_HANDLER(IDC_SYSTEM, _L(_T("Help|Files|System"), _T("Specifies additional attributes for the file.")))
		TOOLTIP_HANDLER(IDC_DESTNAME, _L(_T("Help|Files|DestName"), _T("This parameter specifies a new name for the file when it is installed on the user's system. By default, Setup uses the name from the Source parameter, so in most cases it's not necessary to specify this parameter.")))
		TOOLTIP_HANDLER(IDC_FONTISNTTRUETYPE, _L(_T("Help|Files|FontIsntTrueType"), _T("Specify this flag if the entry is installing a non-TrueType font with the FontInstall parameter.")))
		TOOLTIP_HANDLER(IDC_IGNOREVERSION, _L(_T("Help|Files|IgnoreVersion"), _T("Don't compare version info at all; replace existing files regardless of their version number.\r\n\r\nThis flag should only be used on files private to your application, never on shared system files.")))
		TOOLTIP_HANDLER(IDC_DONTCOPY, _L(_T("Help|Files|DontCopy"), _T("Don't copy the file to the user's system. This flag is useful if the file is handled by the [Code] section exclusively.")))
		TOOLTIP_HANDLER(IDC_UNINSREMOVEREADONLY, _L(_T("Help|Files|UninsRemoveReadOnly"), _T("When uninstalling the file, remove any read-only attribute from the file before attempting to delete it.")))
		TOOLTIP_HANDLER(IDC_TOUCH, _L(_T("Help|Files|Touch"), _T("This flag causes Setup to set the time/date stamp of the installed file(s) to that which is specified by the TouchDate and TouchTime [Setup] section directives.\r\n\r\nThis flag has no effect if combined with the external flag.")))
		TOOLTIP_HANDLER(IDC_REPLACESAMEVERSION, _L(_T("Help|Files|ReplaceSameVersion"), _T("When this flag is used and the file already exists on the user's system and it has the same version number as the file being installed, Setup will compare the files and replace the existing file if their contents differ.\r\n\r\nThe default behavior (i.e. when this flag isn't used) is to not replace an existing file with the same version number.")))
		TOOLTIP_HANDLER(IDC_PERMISSIONS, _L(_T("Help|Files|Permissions"), _T("Specifies additional permissions to grant in the file's ACL (access control list). It is not recommended that you use this parameter if you aren't familiar with ACLs or why you would need to change them, because misusing it could negatively impact system security.")))
		TOOLTIP_HANDLER(IDC_EXCLUDES, _L(_T("Help|Files|Excludes"), _T("Specifies a list of patterns to exclude, separated by commas. This parameter cannot be combined with the external flag.")))
		TOOLTIP_HANDLER(IDC_SORTFILESBYEXTENSION, _L(_T("Help|Files|SortFilesByExtension"), _T("This flag instructs the compiler to compress the found files sorted by extension befores it sorts by path name. This potentially decreases the size of Setup if SolidCompression is also used.")))
		TOOLTIP_HANDLER(IDC_COMPARETIMESTAMP, _L(_T("Help|Files|CompareTimeStamp"), _T("(Not recommended, see Inno Setup help file)\r\nInstructs Setup to proceed to comparing time stamps if the file being installed already exists on the user's system.")))
		TOOLTIP_HANDLER(IDC_CONFIRMOVERWRITE, _L(_T("Help|Files|ConfirmOverwrite"), _T("Always ask the user to confirm before replacing an existing file.")))
		TOOLTIP_HANDLER(IDC_DELETEAFTERINSTALL, _L(_T("Help|Files|DeleteAfterInstall"), _T("Instructs Setup to install the file as usual, but then delete it once the installation is completed (or aborted). This can be useful for extracting temporary data needed by a program executed in the script's [Run] section.")))
		TOOLTIP_HANDLER(IDC_ISREADME, _L(_T("Help|Files|IsReadme"), _T("File is the \"README\" file. Only one file in an installation can have this flag. When a file has this flag, the user will asked if he/she would like to view the README file after the installation has completed. If Yes is chosen, Setup will open the file, using the default program for the file type. For this reason, the README file should always end with an extension like .txt, .wri, or .doc.")))
		TOOLTIP_HANDLER(IDC_REGSERVER, _L(_T("Help|Files|RegServer"), _T("Register the OLE server (a.k.a. ActiveX control). With this flag set, Setup will locate and execute the DLL/OCX's DllRegisterServer export. The uninstaller calls DllUnregisterServer. When used in combination with sharedfile, the DLL/OCX will only be unregistered when the reference count reaches zero.")))
		TOOLTIP_HANDLER(IDC_RESTARTREPLACE, _L(_T("Help|Files|RestartReplace"), _T("This flag is generally useful when replacing core system files. If the file existed beforehand and was found to be locked resulting in Setup being unable to replace it, Setup will register the file (either in WININIT.INI or by using MoveFileEx, for Windows and Windows NT respectively) to be replaced the next time the system is restarted. When this happens, the user will be prompted to restart the computer at the end of the installation process.")))
		TOOLTIP_HANDLER(IDC_REGTYPELIB, _L(_T("Help|Files|RegTypeLib"), _T("Register the type library (.tlb). The uninstaller will unregister the type library (unless the flag uninsneveruninstall is specified). As with the regserver flag, when used in combination with sharedfile, the file will only be unregistered by the uninstaller when the reference count reaches zero.")))
		TOOLTIP_HANDLER(IDC_SKIPIFSOURCEDOESNTEXIST, _L(_T("Help|Files|SkipIfSourceDoesntExist"), _T("This flag instructs the compiler -- or Setup, if the external flag is also used -- to silently skip over the entry if the source file does not exist, instead of displaying an error message.")))
		TOOLTIP_HANDLER(IDC_FILES_NOREGERROR, _L(_T("Help|Files|NoRegError"), _T("When combined with either the regserver or regtypelib flags, Setup will not display any error message if the registration fails.")))
		TOOLTIP_HANDLER(IDC_UNINSRESTARTDELETE, _L(_T("Help|Files|UninsRestartDelete"), _T("When this flag is used and the file is in use at uninstall time, the uninstaller will queue the file to be deleted when the system is restarted, and at the end of the uninstallation process ask the user if he/she wants to restart. This flag can be useful when uninstalling things like shell extensions which cannot be programmatically stopped. Note that administrative privileges are required on Windows NT/2000/XP for this flag to have an effect.")))
		TOOLTIP_HANDLER(IDC_SHAREDFILE, _L(_T("Help|Files|SharedFile"), _T("Uses Windows' shared file counting feature (located in the registry at HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\SharedDLLs). This enables a file to be shared between applications, without worrying about it being inadvertently removed. Each time the file is installed, the reference count for the file is incremented. When an application using the file is uninstalled, the reference count is decremented. If the count reaches zero, the file is deleted (with the user's confirmation).")))
		TOOLTIP_HANDLER(IDC_UNINSNEVERUNINSTALL, _L(_T("Help|Files|UninsNeverUninstall"), _T("Never uninstall this file. This flag should be used sparingly, and is usually used in combination with the onlyifdoesntexist flag. It is meant to be used when installing a very common shared file, such as CTL3DV2.DLL or an MFC DLL, because you wouldn't want the uninstaller to delete the file since other applications make use of it also.")))
		TOOLTIP_HANDLER(IDC_OVERWRITEREADONLY, _L(_T("Help|Files|OverwriteReadOnly"), _T("Always overwrite a read-only file. Without this flag, Setup will ask the user if an existing read-only file should be overwritten.")))
		TOOLTIP_HANDLER(IDC_FILES_ONLYIFDESTFILEEXISTS, _L(_T("Help|Files|OnlyIfDestFileExists"), _T("Only install the file if a file of the same name already exists on the user's system. This flag may be useful if your installation is a patch to an existing installation, and you don't want files to be installed that the user didn't already have.")))
		TOOLTIP_HANDLER(IDC_RECURSESUBDIRS, _L(_T("Help|Files|RecurseSubDirs"), _T("Instructs the compiler or Setup to also search for the Source filename/wildcard in subdirectories under the Source directory.")))
		TOOLTIP_HANDLER(IDC_PROMPTIFOLDER, _L(_T("Help|Files|PromptIfOlder"), _T("By default, when a file being installed has an older version number (or older time stamp, when the comparetimestamp flag is used) than an existing file, Setup will not replace the existing file. (See the Remarks section at the bottom of this topic for more details.) When this flag is used, Setup will ask the user whether the file should be replaced, with the default answer being to keep the existing file.")))
		TOOLTIP_HANDLER(IDC_ONLYIFDOESNTEXIST, _L(_T("Help|Files|OnlyIfDoesntExist"), _T("Only install the file if it doesn't already exist on the user's system.")))
		TOOLTIP_HANDLER(IDC_NOENCRYPTION, _L(_T("Help|Files|NoEncryption"), _T("Prevents the file from being stored encrypted. Use this flag if you have enabled encryption (using the [Setup] section directive Encryption) but want to be able to extract the file using the [Code] section support function ExtractTemporaryFile before the user has entered the correct password.")))
		TOOLTIP_HANDLER(IDC_NOCOMPRESSION, _L(_T("Help|Files|NoCompression"), _T("Prevents the compiler from attempting to compress the file. Use this flag on file types that you know can't benefit from compression (for example, JPEG images) to speed up the compilation process and save a few bytes in the resulting installation.")))
		TOOLTIP_HANDLER(IDC_DONTVERIFYCHECKSUM, _L(_T("Help|Files|DontVerifyChecksum"), _T("Prevents Setup from verifying the file checksum after extraction. Use this flag on files you wish to modify while already compiled into Setup.\r\n\r\nMust be combined with nocompression.")))
		TOOLTIP_HANDLER(IDC_UNINSNOSHAREDFILEPROMPT, _L(_T("Help|Files|UninsNoSharedFilePrompt"), _T("Tells the uninstaller to automatically remove the shared file when its reference count reaches zero instead of asking the user. Must be combined with the sharedfile flag to have an effect.")))
		TOOLTIP_HANDLER(IDC_CREATEALLSUBDIRS, _L(_T("Help|Files|CreateAllSubdirs"), _T("By default the compiler skips empty directories when it recurses subdirectories searching for the Source filename/wildcard. The createallsubdirs flag causes these directories to be created at install time (just like if you created [Dirs] entries for them). Must be combined with recursesubdirs.")))
		TOOLTIP_HANDLER(IDC_32BIT, _L(_T("Help|Files|32Bit"), _T("Causes the {sys} constant to map to the 32-bit System directory when used in the Source and DestDir parameters, the regserver and regtypelib flags to treat the file as 32-bit, and the sharedfile flag to update the 32-bit SharedDLLs registry key. This is the default behavior in a 32-bit mode install.")))
		TOOLTIP_HANDLER(IDC_64BIT, _L(_T("Help|Files|64Bit"), _T("Causes the {sys} constant to map to the 64-bit System directory when used in the Source and DestDir parameters, the regserver and regtypelib flags to treat the file as 64-bit, and the sharedfile flag to update the 64-bit SharedDLLs registry key. This is the default behavior in a 64-bit mode install.")))
		TOOLTIP_HANDLER(IDC_FILES_SOLIDBREAK, _L(_T("Help|Files|SolidBreak"), _T("When solid compression is enabled, this flag instructs the compiler to finalize the current compression stream and begin a new one before compressing the file(s) matched by Source. This allows Setup to seek to the file instantly without having to decompress any preceding files first. May be useful in a large, multi-component installation if you find too much time is being spent decompressing files belonging to components that weren't selected.")))
		TOOLTIP_HANDLER(IDC_EXTERNALSIZE, _L(_T("Help|Files|ExternalSize"), _T("This parameter must be combined with the external flag and specifies the size of the external file in bytes. If this parameter is not specified, Setup retrieves the file size at startup. Primarily useful for files that aren't available at startup, for example files located on a second disk when disk spanning is being used.")))
		TOOLTIP_HANDLER(IDC_SETNTFSCOMPRESSION, _L(_T("Help|Files|SetNTFSCompression"), _T("Instructs Setup to enable NTFS compression on the file (even if it didn't replace the file). If it fails to set the compression state for any reason (for example, if compression is not supported by the file system), no error message will be displayed.")))
		TOOLTIP_HANDLER(IDC_UNSETNTFSCOMPRESSION, _L(_T("Help|Files|UnsetNTFSCompression"), _T("Instructs Setup to disable NTFS compression on the file (even if it didn't replace the file). If it fails to set the compression state for any reason (for example, if compression is not supported by the file system), no error message will be displayed.")))
		TOOLTIP_HANDLER(IDC_SORTFILESBYNAME, _L(_T("Help|Files|SortFilesByName"), _T("?")))
		TOOLTIP_HANDLER(IDC_GACINSTALL, _L(_T("Help|Files|GacInstall"), _T("?")))
		TOOLTIP_HANDLER(IDC_STRONGASSEMBLYNAME, _L(_T("Help|Files|StrongAssemblyName"), _T("?")))
	END_TOOLTIP_MAP()
};
