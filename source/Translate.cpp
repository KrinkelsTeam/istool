#include "stdafx.h"
#include "resource.h"
#include "translate.h"

CAtlMap<CString, CString>	CTranslate::m_map;
#ifdef _DEBUG
const CString				CTranslate::m_strOrgFile = _T("D:\\svnroot\\istool\\istool\\trunk\\languages\\istool-en.lng");
#endif

void CTranslate::Warning(const CString& strKey, const CString& strDefault) {
#ifdef _DEBUG
	if (!m_strOrgFile.IsEmpty()) {
		CString tmp(strDefault);
		tmp.Replace(_T("\r\n"), _T("\\r\\n"));
		tmp.Replace(_T("\r"), _T("\\r"));
		tmp.Replace(_T("\n"), _T("\\n"));
		tmp.Replace(_T("\t"), _T("\\t"));
		::WritePrivateProfileString(_T("Original Translation"), strKey, tmp, m_strOrgFile);
	} else {
		AtlMessageBox(::GetActiveWindow(), (LPCTSTR)strKey, IDR_MAINFRAME, MB_OK | MB_ICONWARNING);
	}
#endif
}

HMENU CTranslate::Translate(HMENU hMenu, CString strParent/*=CString()*/) {
	CMenuHandle menu(hMenu);
	FixMenuTitle(strParent, CString());
	int iCount = menu.GetMenuItemCount();
	for (int i = 0; i < iCount; i++) {
		CString			strName, strAcc;
		MENUITEMINFO	info = { sizeof MENUITEMINFO };
		info.fMask = MIIM_ID | MIIM_SUBMENU | MIIM_STRING /*| MIIM_FTYPE*/;
		info.cch = 1024;
		info.dwTypeData = strName.GetBuffer(info.cch);
		if (!menu.GetMenuItemInfo(i, TRUE, &info) || !info.cch) continue;
		strName.GetBufferSetLength(info.cch);
		CString strOrgText(strName);
		FixMenuTitle(strName, strAcc);

		CString strKey;
		if (strParent.IsEmpty())
			strKey = strName;
		else
			strKey.Format(_T("%s|%s"), (LPCTSTR)strParent, (LPCTSTR)strName);

		// Translate submenu
		if (info.hSubMenu) Translate(info.hSubMenu, strKey);

		if (!strAcc.IsEmpty()) {
			CString strAccKey;
			strAccKey.Format(_T("ShortCut|%s"), (LPCTSTR)strAcc);

			Warning(strAccKey, strAcc);

			CString strAccTrans;
			if (m_map.Lookup(strAccKey, strAccTrans))
				strAcc = strAccTrans;
		}

		int pos = strOrgText.Find(_T('\t'));
		if (pos > 0) strOrgText.ReleaseBufferSetLength(pos);
		Warning(strKey, strOrgText);

		CString strTrans;
		if (!m_map.Lookup(strKey, strTrans))
			continue;

		strTrans.Replace(_T("\\t"), _T("\t"));

		if (!strAcc.IsEmpty()) {
			strTrans += _T("\t");
			strTrans += strAcc;
		}

		info.fMask = MIIM_STRING;
		info.cch = strTrans.GetLength();
		info.dwTypeData = strTrans.GetBuffer();
		menu.SetMenuItemInfo(i, TRUE, &info);
	}
	return hMenu;
}

void CTranslate::AddFile(const CString& strFileName)
{
	if (strFileName.IsEmpty())
		return;

	CAtlFile file;
	if (FAILED(file.Create(strFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING))) {
		return;
	}

	ULONGLONG size = 0;
	if (FAILED(file.GetSize(size)) || size == 0 || size > ULONG_MAX) {
		return;
	}

	DWORD len = static_cast<DWORD>(size);
	CHeapPtr<BYTE> buf;
	if (!buf.Allocate(len + 1)) {
		return;
	}

	DWORD read = 0;
	if (FAILED(file.Read(buf, len, read)) || read != len) {
		return;
	}

	buf[len] = 0;
	LPCSTR data = reinterpret_cast<LPCSTR>(static_cast<BYTE*>(buf));
	DWORD offset = 0;

	// Lambda to check whether the buffer contains valid UTF-8 characters
	auto isUtf8 = [=]() -> bool {
		for (DWORD i = 0; i < len; ++i) {
			if ((BYTE)data[i] >= 0x80) {
				return MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, data, len, nullptr, 0) > 0;
			}
		}
		return false;
	};

	CStringW content;

	// UTF-8 with BOM
	if (len >= 3 && (BYTE)data[0] == 0xEF && (BYTE)data[1] == 0xBB && (BYTE)data[2] == 0xBF) {
		offset = 3;
	} else if (isUtf8()) {
		offset = 0;
	} else {
		content = CStringW(CStringA(data));
	}

	// Convert UTF-8 content if needed
	if (content.IsEmpty() && len > offset) {
		int wlen = MultiByteToWideChar(CP_UTF8, 0, data + offset, len - offset, nullptr, 0);
		if (wlen <= 0) return;

		LPWSTR p = content.GetBuffer(wlen);
		MultiByteToWideChar(CP_UTF8, 0, data + offset, len - offset, p, wlen);
		content.ReleaseBuffer(wlen);
	}

	// Parse key=value lines
	int pos = 0;
	while (pos >= 0) {
		int next = content.Find(L'\n', pos);
		CStringW line = (next >= 0) ? content.Mid(pos, next - pos) : content.Mid(pos);
		line.TrimRight(L"\r\n");

		int eq = line.Find(L'=');
		if (line.IsEmpty() || line[0] == L';' || line[0] == L'[' || eq < 0) {
			if (next < 0) break;
			pos = next + 1;
			continue;
		}

		CStringW value = line.Mid(eq + 1).Trim();
		if (!value.IsEmpty()) {
			// Replace escaped sequences
			value.Replace(L"\\r", L"\r");
			value.Replace(L"\\n", L"\n");
			value.Replace(L"\\t", L"\t");

			CStringW key = line.Left(eq).Trim();
			m_map[CString(key)] = CString(value);
		}

		if (next < 0) break;
		pos = next + 1;
	}
}

bool CTranslate::Lookup(LPCTSTR pszKey, CString& strTrans) {
	CString strKey(pszKey);

	FixTextTitle(strKey);

	Warning(strKey, pszKey);
	if (!m_map.Lookup(strKey, strTrans))
		return false;

	return true;
}

void CTranslate::FixMenuTitle(CString& str, CString& strAcc) {
	strAcc.Empty();
	int len = str.GetLength();
	for (int i = 0; i < len; i++) {
		if (str[i] == _T('\t')) {
			strAcc = str.Mid(i + 1);
			str.ReleaseBufferSetLength(len = i);
			break;
		} else if (str[i] == _T(' ') || str[i] == _T('-') || str[i] == _T('=')) {
			str.Delete(i);
			str.SetAt(i, toupper(str[i]));
			i--;
			len--;
		} else if (str[i] >= 128 || str[i] == _T('&') || str[i] == _T(':') || str[i] == _T('.') || str[i] == _T('\'') || str[i] == _T('(') || str[i] == _T(')') || str[i] == _T(',') || str[i] == _T('/') || str[i] == _T('%') || str[i] == _T('!') || str[i] == _T('?') || str[i] == _T('\n') || str[i] == _T('\r') || str[i] == _T('\t')) {
			str.Delete(i--);
			len--;
		}
	}
	if (len > 3 && str[len - 3] == _T('.') && str[len - 2] == _T('.') && str[len - 1] == _T('.'))
		str.ReleaseBufferSetLength(len - 3);
}

void CTranslate::FixTextTitle(CString& str) {
	int len = str.GetLength();
	for (int i = 0; i < len; i++) {
		if (str[i] == _T(' ') || str[i] == _T('-') || str[i] == _T('=')) {
			str.Delete(i);
			str.SetAt(i, toupper(str[i]));
			i--;
			len--;
		} else if (str[i] >= 128 || str[i] == _T('&') || str[i] == _T(' ') || str[i] == _T(':') || str[i] == _T('.') || str[i] == _T('\'') || str[i] == _T('(') || str[i] == _T(')') || str[i] == _T(',') || str[i] == _T('/') || str[i] == _T('%') || str[i] == _T('!') || str[i] == _T('?') || str[i] == _T('\n') || str[i] == _T('\r') || str[i] == _T('\t')) {
			str.Delete(i--);
			len--;
		}
	}
}

void CTranslate::Translate(HWND hWnd, const CString& strTitle) {
	EnumChildProc(hWnd, (LPARAM)(LPCTSTR)strTitle);
	::EnumChildWindows(hWnd, EnumChildProc, (LPARAM)(LPCTSTR)strTitle);
}

BOOL CALLBACK CTranslate::EnumChildProc(HWND hWnd, LPARAM lParam) {
	LPCTSTR pszTitle = (LPCTSTR)lParam;

	CString str;
	CWindow wnd(hWnd);
	wnd.GetWindowText(str);

	if (str.IsEmpty() || IgnoreWord(str))
		return TRUE;

	CString strKey, strTrans;
	strKey.Format(_T("%s|%s"), pszTitle, (LPCTSTR)str);

	FixTextTitle(strKey);

	Warning(strKey, str);
	if (!m_map.Lookup(strKey, strTrans))
		return TRUE;

	strTrans.Replace(_T("\\n"), _T("\n"));
	strTrans.Replace(_T("\\r"), _T("\r"));
	strTrans.Replace(_T("\\t"), _T("\t"));
	wnd.SetWindowText(strTrans);
	return TRUE;
}

bool CTranslate::IgnoreWord(LPCTSTR pszWord) {
	static LPCTSTR pszIgnoreList[] = {
		_T("..."),
		_T("0"),
		_T("List1"),
		_T("Spin1"),
		_T("X.X.X"),
		_T("https://istool.krinkels.org/"),
		_T("ISTool"),
		_T("Bjørnar Henden"),
		_T("Copyright © 1999 - 2009 Bjørnar Henden."),
		_T("https://www.innosetup.com/"),
		_T("Inno Setup"),
		_T("KrinkelsTeam"),
		NULL
	};
	LPCTSTR* pszWordList = pszIgnoreList;
	while (*pszWordList) {
		if (!_tcscmp(pszWord, *pszWordList))
			return true;
		pszWordList++;
	}

	return false;
}

LPCTSTR CTranslate::Translate(LPCTSTR pszDefault) {
	CString strKey(pszDefault);
	FixTextTitle(strKey);

	Warning(strKey, pszDefault);
	if (m_map.Lookup(strKey, CString()))
		return m_map[strKey];

	return pszDefault;
}

LPCTSTR CTranslate::Translate(LPCTSTR pszKey, LPCTSTR pszDefault) {
	CString strKey(pszKey);
	FixTextTitle(strKey);

	Warning(strKey, pszDefault);
	if (m_map.Lookup(strKey, CString()))
		return m_map[strKey];

	return pszDefault;
}
