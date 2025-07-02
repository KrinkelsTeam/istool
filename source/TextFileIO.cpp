//-----------------------------------------------------------------------------
// Project:         ISTool
// Project author:  Bjørnar Henden (c) 1999 - 2009
// ----------------------------------------------------------------------------
// File:            TextFileIO.cpp
// Author:          YURSHAT <yurshat@gmail.com>
// Created:         2025-06-30
// Purpose:         Implementation of CTextFileReader / CTextFileWriter classes
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "TextFileIO.h"

bool CTextFileReader::Load(LPCTSTR pszFileName)
{
	m_strContent.Empty();
	m_encoding = TextEncoding::Unknown;

	CAtlFile file;
	if (FAILED(file.Create(pszFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING))) {
		return false;
	}

	ULONGLONG size = 0;
	if (FAILED(file.GetSize(size)) || size == 0 || size > ULONG_MAX) {
		return false;
	}

	DWORD len = static_cast<DWORD>(size);
	CHeapPtr<BYTE> buffer;
	if (!buffer.Allocate(len + 2)) { // +2 for null-termination safety
		return false;
	}

	DWORD read = 0;
	if (FAILED(file.Read(buffer, len, read)) || read != len) {
		return false;
	}

	buffer[len] = 0;
	buffer[len + 1] = 0;

	LPCBYTE data = buffer;
	DWORD offset = 0;

	// UTF-8 BOM detection
	if (len >= 3 && data[0] == 0xEF && data[1] == 0xBB && data[2] == 0xBF) {
		offset = 3;
		m_encoding = TextEncoding::UTF8_BOM;
	} else if (len >= 2 && data[0] == 0xFF && data[1] == 0xFE) {
		// UTF-16 LE BOM detection
		offset = 2;
		m_encoding = TextEncoding::UTF16_LE;
	} else {
		// Check if buffer looks like valid UTF-8
		auto isUtf8 = [&]() -> bool {
			for (DWORD i = 0; i < len; ++i) {
				if (data[i] >= 0x80) {
					return MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, reinterpret_cast<LPCSTR>(data), len, nullptr, 0) > 0;
				}
			}
			return false;
		};

		if (isUtf8()) {
			m_encoding = TextEncoding::UTF8;
		} else {
			m_encoding = TextEncoding::Ansi;
		}
	}

	if (m_encoding == TextEncoding::UTF16_LE) {
		m_strContent = CString(reinterpret_cast<LPCWSTR>(data + offset), (len - offset) / 2);
	} else if (m_encoding == TextEncoding::UTF8_BOM || m_encoding == TextEncoding::UTF8) {
		int wlen = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<LPCSTR>(data + offset), len - offset, nullptr, 0);
		if (wlen <= 0) return false;
		LPWSTR out = m_strContent.GetBuffer(wlen);
		MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<LPCSTR>(data + offset), len - offset, out, wlen);
		m_strContent.ReleaseBuffer(wlen);
	} else if (m_encoding == TextEncoding::Ansi) {
		m_strContent = CString(CStringA(reinterpret_cast<LPCSTR>(data)));
	} else {
		return false;
	}

	return true;
}

void CTextFileReader::GetLines(CAtlArray<CString>& outLines) const {
	outLines.RemoveAll();
	int pos = 0;
	while (pos >= 0) {
		int next = m_strContent.Find(_T('\n'), pos);
		CString line = (next >= 0) ? m_strContent.Mid(pos, next - pos) : m_strContent.Mid(pos);
		line.TrimRight(_T("\r\n"));
		outLines.Add(line);
		if (next >= 0) pos = next + 1; else break;
	}
}

const CString& CTextFileReader::GetContent() const
{
	return m_strContent;
}

TextEncoding CTextFileReader::GetEncoding() const
{
	return m_encoding;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

CTextFileWriter::CTextFileWriter()
	: m_encoding(TextEncoding::Auto), m_bOpened(false) {
}

CTextFileWriter::~CTextFileWriter() {
	Close();
}

bool CTextFileWriter::Open(LPCTSTR pszFileName, TextEncoding encoding, bool bAppend) {
	DWORD dwFlags = bAppend ? FILE_APPEND_DATA : GENERIC_WRITE;
	DWORD dwCreate = bAppend ? OPEN_ALWAYS : CREATE_ALWAYS;

	if (FAILED(m_file.Create(pszFileName, dwFlags, FILE_SHARE_READ, dwCreate))) {
		return false;
	}

	m_encoding = encoding;
	m_bOpened = true;

	if (!bAppend) {
		return WriteBOM();
	}

	m_file.Seek(0, FILE_END);
	return true;
}

bool CTextFileWriter::WriteLine(const CString& strLine) {
	if (!m_bOpened)
		return false;

	CString fullLine = strLine + _T("\r\n");
	return ConvertAndWrite(fullLine);
}

void CTextFileWriter::Close() {
	if (m_bOpened) {
		m_file.Close();
		m_bOpened = false;
	}
}

bool CTextFileWriter::Save(LPCTSTR pszFileName, const CString& content, TextEncoding encoding, bool bAppend) {
	if (!Open(pszFileName, encoding, bAppend))
		return false;

	bool result = ConvertAndWrite(content);
	Close();
	return result;
}

bool CTextFileWriter::WriteBOM() {
	if (m_encoding == TextEncoding::UTF8_BOM) {
		const BYTE bom[] = { 0xEF, 0xBB, 0xBF };
		return SUCCEEDED(m_file.Write(bom, sizeof(bom)));
	} else if (m_encoding == TextEncoding::UTF16_LE) {
		const BYTE bom[] = { 0xFF, 0xFE };
		return SUCCEEDED(m_file.Write(bom, sizeof(bom)));
	}
	return true;
}

bool CTextFileWriter::ConvertAndWrite(const CString& str) {
	TextEncoding finalEncoding = m_encoding;

	if (finalEncoding == TextEncoding::Auto) {
		CStringA ansi(str);
		CStringW roundTrip(ansi);
		finalEncoding = (str == roundTrip) ? TextEncoding::Ansi : TextEncoding::UTF8;
	}

	if (finalEncoding == TextEncoding::UTF16_LE) {
		return SUCCEEDED(m_file.Write(str.GetString(), str.GetLength() * sizeof(WCHAR)));
	} else if (finalEncoding == TextEncoding::Ansi) {
		CStringA ansi(str);
		return SUCCEEDED(m_file.Write((LPCSTR)ansi, ansi.GetLength()));
	} else {
		int len = WideCharToMultiByte(CP_UTF8, 0, str, -1, nullptr, 0, nullptr, nullptr);
		if (len <= 1)
			return false;

		CHeapPtr<char> utf8;
		if (!utf8.Allocate(len))
			return false;

		WideCharToMultiByte(CP_UTF8, 0, str, -1, utf8, len, nullptr, nullptr);
		return SUCCEEDED(m_file.Write(utf8, len - 1)); // exclude null terminator
	}
}