/*----------------------------------------------------------------------.
Copyright (c) 1998,1999 Gipsysoft. All Rights Reserved.
File:	SplitPath.h
Owner:	russf@gipsysoft.com
Purpose:	Convientient wrapper for the split/make path functions.
					Also adds some checking when setting the path components
					
					Rewrited by YURSHAT
					Added Unicode support
----------------------------------------------------------------------*/

#pragma once

class CSplitPath
{
public:
	CSplitPath(LPCTSTR pcszPath = nullptr)
	{
		if (pcszPath)
			Split(pcszPath);
	}

	void Split(LPCTSTR pcszPath)
	{
		ATLASSERT(pcszPath != nullptr);
		_tsplitpath_s(pcszPath,
			m_szDrive, _countof(m_szDrive),
			m_szDirectory, _countof(m_szDirectory),
			m_szFilename, _countof(m_szFilename),
			m_szExtension, _countof(m_szExtension));
	}

	void Make(LPTSTR szPathBuffer)
	{
		ATLASSERT(szPathBuffer != nullptr);
		_tmakepath_s(szPathBuffer, _MAX_PATH,
			m_szDrive, m_szDirectory, m_szFilename, m_szExtension);
	}

	CString GetDrive() const { return m_szDrive; }
	CString GetDirectory() const { return m_szDirectory; }
	CString GetFilename() const { return m_szFilename; }
	CString GetExtension() const { return m_szExtension; }

	void SetDrive(LPCTSTR pcszDrive)
	{
		ATLASSERT(pcszDrive != nullptr);
		_tcsncpy_s(m_szDrive, pcszDrive, _TRUNCATE);
	}

	void SetDirectory(LPCTSTR pcszDirectory)
	{
		ATLASSERT(pcszDirectory != nullptr);
		_tcsncpy_s(m_szDirectory, pcszDirectory, _TRUNCATE);
	}

	void SetFilename(LPCTSTR pcszFilename)
	{
		ATLASSERT(pcszFilename != nullptr);
		_tcsncpy_s(m_szFilename, pcszFilename, _TRUNCATE);
	}

	void SetExtension(LPCTSTR pcszExtension)
	{
		ATLASSERT(pcszExtension != nullptr);
		_tcsncpy_s(m_szExtension, pcszExtension, _TRUNCATE);
	}

private:
	TCHAR m_szDrive[_MAX_DRIVE] = {};
	TCHAR m_szDirectory[_MAX_DIR] = {};
	TCHAR m_szFilename[_MAX_FNAME] = {};
	TCHAR m_szExtension[_MAX_EXT] = {};
};
