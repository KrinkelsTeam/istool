//-----------------------------------------------------------------------------
// Project:         ISTool
// Project author:  Bjørnar Henden (c) 1999 - 2009
// ----------------------------------------------------------------------------
// File:            TextFileIO.h
// Author:          YURSHAT <yurshat@gmail.com>
// Created:         2025-06-30
// Purpose:         Declaration of CTextFileReader / CTextFileWriter classes
//-----------------------------------------------------------------------------

#pragma once

// Enumeration of supported encodings
enum class TextEncoding {
	Unknown,
	Auto,
	Ansi,
	UTF8,
	UTF8_BOM,
	UTF16_LE
};

/**
 * @brief Reads a text file with automatic encoding detection.
 *
 * Supports detection of ANSI, UTF-8 (with or without BOM), and UTF-16 LE encodings.
 * Converts all content to UTF-16 internally and exposes it as a CString interface.
 */
class CTextFileReader
{
public:
	// Load and parse the specified file
	bool Load(LPCTSTR pszFileName);

	// Get the lines of the file as an array of strings
	void GetLines(CAtlArray<CString>& outLines) const;

	// Get the decoded content as a wide string
	const CString& GetContent() const;

	// Get the detected text encoding
	TextEncoding GetEncoding() const;

private:
	CString m_strContent;
	TextEncoding m_encoding = TextEncoding::Unknown;
};

/**
 * @brief Writes a text file in a specified encoding.
 *
 * Supports writing in UTF-8 (with or without BOM), UTF-16 LE, and ANSI.
 * Also provides automatic encoding selection based on content if TextEncoding::Auto is specified.
 */
class CTextFileWriter {
public:
	CTextFileWriter();
	~CTextFileWriter();

	// Open file for writing
	bool Open(LPCTSTR pszFileName, TextEncoding encoding = TextEncoding::Auto, bool bAppend = false);

	// Write a single line (with line break)
	bool WriteLine(const CString& strLine);

	// Finalize and close file
	void Close();

	// Save the entire content to file
	bool Save(LPCTSTR pszFileName, const CString& content, TextEncoding encoding = TextEncoding::Auto, bool bAppend = false);

	// Check if the file is currently open
	bool IsOpen() const { return m_bOpened; }

private:
	CAtlFile m_file;
	TextEncoding m_encoding;
	bool m_bOpened;

	bool WriteBOM();
	bool ConvertAndWrite(const CString& str);
};