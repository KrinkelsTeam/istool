// InnoScript.cpp: implementation of the CInnoScript class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ISTool.h"
#include "InnoScript.h"
#include "Lines.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInnoScript::~CInnoScript() {
	Clear();
}

static const struct {
	CInnoScript::SECTION	m_sec;
	LPCTSTR					m_pszName;
} m_sectionnames[] = {
	CInnoScript::SEC_SETUP,				_T("Setup"),
	CInnoScript::SEC_DIRS,				_T("Dirs"),
	CInnoScript::SEC_FILES,				_T("Files"),
	CInnoScript::SEC_ICONS,				_T("Icons"),
	CInnoScript::SEC_INI,				_T("INI"),
	CInnoScript::SEC_INSTALLDELETE,		_T("InstallDelete"),
	CInnoScript::SEC_MESSAGES,			_T("Messages"),
	CInnoScript::SEC_REGISTRY,			_T("Registry"),
	CInnoScript::SEC_RUN,				_T("Run"),
	CInnoScript::SEC_UNINSTALLDELETE,	_T("UninstallDelete"),
	CInnoScript::SEC_UNINSTALLRUN,		_T("UninstallRun"),
	CInnoScript::SEC_TYPES,				_T("Types"),
	CInnoScript::SEC_COMPONENTS,		_T("Components"),
	CInnoScript::SEC_TASKS,				_T("Tasks"),
	CInnoScript::SEC_LANGOPTIONS,		_T("LangOptions"),
	CInnoScript::SEC_CODE,				_T("Code"),
	CInnoScript::SEC_LANGUAGES,			_T("Languages"),
	CInnoScript::SEC_CUSTOMMESSAGES,	_T("CustomMessages"),
	CInnoScript::PRJ_PRECOMPILESTEPS,	_T("_ISToolPreCompile"),
	CInnoScript::PRJ_POSTCOMPILESTEPS,	_T("_ISToolPostCompile"),
	CInnoScript::PRJ_DOWNLOAD,			_T("_ISToolDownload"),
	CInnoScript::PRJ_ISTOOL,			_T("_ISTool"),
	CInnoScript::SEC_NONE,				NULL,
};

LPCTSTR CInnoScript::GetSectionName(SECTION sec) {
	for (UINT n = 0; m_sectionnames[n].m_pszName; n++) {
		if (m_sectionnames[n].m_sec == sec)
			return m_sectionnames[n].m_pszName;
	}
	return NULL;
}

CInnoScript::SECTION CInnoScript::GetSectionCode(LPCTSTR pszSection) {
	CString strSection(pszSection);
	int nLength = strSection.Trim().GetLength();
	if (nLength > 2 && strSection[0] == _T('[') && strSection[nLength - 1] == _T(']'))
		strSection = strSection.Mid(1, nLength - 2);
	for (UINT n = 0; m_sectionnames[n].m_pszName; n++) {
		if (!strSection.CompareNoCase(m_sectionnames[n].m_pszName))
			return m_sectionnames[n].m_sec;
	}
	return SEC_NONE;
}

void CInnoScript::Clear() {
	for (long i = 0; i < (long)m_lines.GetCount(); i++)
		delete m_lines[i];
	m_lines.RemoveAll();
}

bool CInnoScript::AddLine(SECTION& sec, CString& strLine) {
	CLine* pLine = NULL;
	DWORD dwUserFlags = 0;

	strLine.TrimRight();

	// Concatenation
	if (!strLine.Right(2).Compare(_T(" \\"))) {
		dwUserFlags |= CLine::FLG_CONCAT;
		strLine = strLine.Left(strLine.GetLength() - 2);
		strLine.ReleaseBuffer();
	}

	int nFirstNonWhite = 0;
	while (strLine[nFirstNonWhite] && _istspace(strLine[nFirstNonWhite])) nFirstNonWhite++;

	try {
		if (strLine[0] == _T('[')) {
			if (strLine.GetLength() > 1 && strLine[1] == _T('/')) {
				// End of section
				sec = SEC_NONE;
			} else {
				UINT nRow = 0;
				while (m_sectionnames[nRow].m_sec != SEC_NONE) {
					UINT nLength = _tcslen(m_sectionnames[nRow].m_pszName);
					if (!strLine.Mid(1, nLength).CompareNoCase(m_sectionnames[nRow].m_pszName) && strLine.GetAt(nLength + 1) == _T(']')) {
						sec = m_sectionnames[nRow].m_sec;
						break;
					}
					nRow++;
				}
				if (m_sectionnames[nRow].m_sec == SEC_NONE) {
					sec = SEC_NONE;
					pLine = new CLineComment(sec, strLine);
				}
			}
		} else if (strLine[0] == _T('#') || strLine[nFirstNonWhite] == _T('#')) {
			pLine = new CLineHash(sec, strLine);
		} else if (strLine[0] == _T(';') || !strLine[0] || sec == SEC_NONE || sec == SEC_CODE) {
			pLine = new CLineComment(sec, strLine);
		} else {
			if (sec == SEC_SETUP || sec == SEC_MESSAGES || sec == PRJ_ISTOOL || sec == SEC_LANGOPTIONS || sec == SEC_CUSTOMMESSAGES)
				pLine = new CLineSetup(sec, strLine);
			else
				pLine = new CLineParam(sec, strLine);
		}
	} catch (...) {
		if (pLine) delete pLine;
		pLine = new CLineComment(sec, strLine);
		nFirstNonWhite = 0;
	}
	// Insert the newly added line
	if (pLine) {
		if (nFirstNonWhite > 0)
			pLine->SetIndent(strLine.Left(nFirstNonWhite));
		pLine->m_dwUserFlags |= dwUserFlags;
		bool bFound = false;
		for (long i = 0; i < (long)m_lines.GetCount(); i++) {
			if (bFound && m_lines[i]->GetSection() != pLine->GetSection()) {
				m_lines.InsertAt(i, pLine);
				return true;
			}
			if (m_lines[i]->GetSection() == pLine->GetSection())
				bFound = true;
		}
		m_lines.Add(pLine);
	}
	return true;
}

bool CInnoScript::LoadScriptBuffer(LPTSTR pszBuffer) {
	SECTION sec = SEC_NONE;
	LPCTSTR pszLineStart = pszBuffer;
	while (*pszBuffer) {
		if (*pszBuffer == _T('\n')) {
			*pszBuffer++ = 0;
			AddLine(sec, CString(pszLineStart));
			pszLineStart = pszBuffer;
		} else
			pszBuffer++;
	}
	if (pszBuffer != pszLineStart)
		AddLine(sec, CString(pszLineStart));

	return true;
}

bool CInnoScript::LoadScript(LPCTSTR pszFileName) {
	CAtlFile file;
	if (FAILED(file.Create(pszFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING)))
		return false;

	ULONGLONG size = 0;
	if (FAILED(file.GetSize(size)) || size == 0 || size > ULONG_MAX)
		return false;

	DWORD len = static_cast<DWORD>(size);
	CHeapPtr<BYTE> buf;
	if (!buf.Allocate(len + 1))
		return false;

	DWORD read = 0;
	if (FAILED(file.Read(buf, len, read)) || read != len)
		return false;

	buf[len] = 0;
	LPCSTR data = reinterpret_cast<LPCSTR>(static_cast<BYTE*>(buf));
	DWORD offset = 0;

	CStringW content;

	bool looksLikeUtf8 = false;
	for (DWORD i = 0; i < len; ++i) {
		if ((BYTE)data[i] >= 0x80) {
			looksLikeUtf8 = true;
			break;
		}
	}

	if (len >= 3 && (BYTE)data[0] == 0xEF && (BYTE)data[1] == 0xBB && (BYTE)data[2] == 0xBF) {
		offset = 3;
		theApp.m_saveEncoding = SaveEncoding::UTF8WithBOM;
	} else if (looksLikeUtf8 && MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, data, len, nullptr, 0) > 0) {
		offset = 0;
		theApp.m_saveEncoding = SaveEncoding::UTF8WithoutBOM;
	} else {
		content = CStringW(CStringA(data));
		theApp.m_saveEncoding = SaveEncoding::Auto;
	}

	if (content.IsEmpty() && len > offset) {
		int wlen = MultiByteToWideChar(CP_UTF8, 0, data + offset, len - offset, nullptr, 0);
		if (wlen <= 0) return false;
		LPWSTR p = content.GetBuffer(wlen);
		MultiByteToWideChar(CP_UTF8, 0, data + offset, len - offset, p, wlen);
		content.ReleaseBuffer(wlen);
	}

	SECTION sec = SEC_NONE;
	int pos = 0;

	while (pos >= 0) {
		int next = content.Find(L'\n', pos);
		CStringW line = (next >= 0) ? content.Mid(pos, next - pos) : content.Mid(pos);
		line.TrimRight(L"\r\n");
		AddLine(sec, CString(line));
		if (next >= 0) pos = next + 1; else break;
	}

	return true;
}

bool CInnoScript::WriteScript(LPCTSTR pszFileName)
{
	CAtlFile file;
	HRESULT hr = file.Create(pszFileName, GENERIC_WRITE, FILE_SHARE_READ, CREATE_ALWAYS);
	if (FAILED(hr))
		return false;

	// Write UTF-8 BOM if required
	if (theApp.m_saveEncoding == SaveEncoding::UTF8WithBOM) {
		static const BYTE bom[] = { 0xEF, 0xBB, 0xBF };
		file.Write(bom, sizeof(bom));
	}

	// Inline lambda for ANSI compatibility check
	auto isAnsi = [](const CString& str) -> bool {
		CStringA ansi(str);
		CStringW roundTrip(ansi);
		return str == roundTrip;
	};

	SECTION sec = SEC_NONE;
	CString line;
	bool requiresUtf8 = false;

	for (long i = 0; i < GetCount(); i++) {
		CLine* pLine = m_lines[i];

		// Insert section header if changed
		if (pLine->GetSection() != sec) {
			sec = pLine->GetSection();
			if (sec != SEC_NONE) {
				line.Format(_T("[%s]\r\n"), m_sectionnames[sec].m_pszName);
				if (theApp.m_saveEncoding == SaveEncoding::Auto && !isAnsi(line))
					requiresUtf8 = true;

				WriteLineToFile(file, line);
			}
		}

		// Write line content
		TCHAR szLine[5000] = {};
		pLine->Write(szLine, _countof(szLine));
		line.Format(_T("%s\r\n"), szLine);
		if (theApp.m_saveEncoding == SaveEncoding::Auto && !isAnsi(line))
			requiresUtf8 = true;

		WriteLineToFile(file, line);
	}

	// Adjust encoding if Auto mode was used
	if (theApp.m_saveEncoding == SaveEncoding::Auto) {
		theApp.m_saveEncoding = requiresUtf8
			? SaveEncoding::UTF8WithoutBOM
			: SaveEncoding::Auto; // ANSI-compatible
	}

	return true;
}

void CInnoScript::WriteLineToFile(CAtlFile& file, const CString& line)
{
	LPCWSTR wsz = line;

	if (theApp.m_saveEncoding == SaveEncoding::UTF8WithBOM ||
		theApp.m_saveEncoding == SaveEncoding::UTF8WithoutBOM) {
		int len = WideCharToMultiByte(CP_UTF8, 0, wsz, -1, nullptr, 0, nullptr, nullptr);
		if (len <= 0) return;

		CHeapPtr<char> utf8;
		if (!utf8.Allocate(len)) return;

		WideCharToMultiByte(CP_UTF8, 0, wsz, -1, utf8, len, nullptr, nullptr);
		file.Write(utf8, len - 1); // exclude null terminator
	} else {
		CStringA ansi(wsz);
		file.Write(ansi, ansi.GetLength());
	}
}

void CInnoScript::MoveUp(CLine* pLine) {
	for (long i = 1; i < GetCount(); i++) {
		if (m_lines[i] == pLine) {
			m_lines.RemoveAt(i);
			while (i > 1 && m_lines[i - 1]->GetComment()) i--;
			m_lines.InsertAt(i - 1, pLine);
			break;
		}
	}
}

void CInnoScript::MoveDown(CLine* pLine) {
	for (long i = 0; i < GetCount() - 1; i++) {
		if (m_lines[i] == pLine) {
			m_lines.RemoveAt(i);
			while (i < GetCount() && m_lines[i]->GetComment()) i++;
			if (i + 2 >= GetCount())
				m_lines.Add(pLine);
			else
				m_lines.InsertAt(i + 1, pLine);
			break;
		}
	}
}

long CInnoScript::GetLineNumber(CLine* pLineFind) {
	SECTION sec = SEC_NONE;
	long nLine = -1;
	for (long i = 0; i < GetCount(); i++) {
		CLine* pLine = m_lines[i];
		nLine++;
		// New section?
		if (pLine->GetSection() != sec) {
			sec = pLine->GetSection();
			if (sec != SEC_NONE) nLine++;
		}
		if (pLine == pLineFind)
			return nLine;
	}
	return -1;
}

long CInnoScript::GetLineNumber(SECTION secFind) {
	SECTION sec = SEC_NONE;
	long nLine = -1;
	for (long i = 0; i < GetCount(); i++) {
		CLine* pLine = m_lines[i];
		nLine++;
		// New section?
		if (pLine->GetSection() != sec) {
			sec = pLine->GetSection();
			if (sec != SEC_NONE) nLine++;
		}
		if (sec == secFind)
			return nLine;
	}
	return -1;
}

bool CInnoScript::AddHead(CLine* pLine) {
	m_lines.InsertAt(0, pLine);
	return true;
}

bool CInnoScript::AddTail(CLine* pLine) {
	m_lines.Add(pLine);
	return true;
}

long CInnoScript::GetSize() {
	return m_lines.GetCount();
}

long CInnoScript::GetCount() {
	return GetSize();
}

CInnoScript::CLine* CInnoScript::operator[](long iItem) {
	ATLASSERT(iItem < (long)m_lines.GetCount());
	return m_lines[iItem];
}

const CInnoScript& CInnoScript::operator=(CInnoScript& in) {
	Clear();
	for (long i = 0; i < in.GetSize(); i++)
		m_lines.Add(in.m_lines[i]->Copy());
	return *this;
}
