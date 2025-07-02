/**
** @file
** @brief Miscellaneous classes not placed elsewhere
*/

#pragma once

/// My namespace
namespace Henden {

	/**
	** @brief A class for storing application information in registry.
	**
	** This class is used to read values from, and write values to registry.
	*/
	class CAppReg {
	public:
		/// Constructor
		CAppReg(LPCTSTR pszSubKey) {
			m_strRegPath = pszSubKey;
		}

		/// Saves a DWORD value to the registry.
		bool SetDWORDValue(LPCTSTR pszSection, LPCTSTR pszValueName, DWORD dwValue) {
			CRegKey regKey;
			if (regKey.Create(HKEY_CURRENT_USER, BuildSubKey(pszSection)) != ERROR_SUCCESS)
				return false;
			return regKey.SetDWORDValue(pszValueName, dwValue) == ERROR_SUCCESS;
		}

		/// Saves a STRING value to the registry.
		bool SetStringValue(LPCTSTR pszSection, LPCTSTR pszValueName, LPCTSTR pszValue) {
			CRegKey regKey;
			if (regKey.Create(HKEY_CURRENT_USER, BuildSubKey(pszSection)) != ERROR_SUCCESS)
				return false;
			return regKey.SetStringValue(pszValueName, pszValue) == ERROR_SUCCESS;
		}

		/// Deletes a value from the registry.
		bool DeleteValue(LPCTSTR pszSection, LPCTSTR pszValueName) {
			CRegKey regKey;
			if (regKey.Open(HKEY_CURRENT_USER, BuildSubKey(pszSection)) != ERROR_SUCCESS)
				return false;
			return regKey.DeleteValue(pszValueName) == ERROR_SUCCESS;
		}

		/// Queries a DWORD value from the registry.
		bool QueryDWORDValue(LPCTSTR pszSection, LPCTSTR pszValueName, DWORD& dwRet) {
			CRegKey regKey;
			if (regKey.Open(HKEY_CURRENT_USER, BuildSubKey(pszSection)) != ERROR_SUCCESS)
				return false;
			return regKey.QueryDWORDValue(pszValueName, dwRet) == ERROR_SUCCESS;
		}

		/// Queries a STRING value from the registry.
		bool QueryStringValue(LPCTSTR pszSection, LPCTSTR pszValueName, CString& strRet) {
			CRegKey regKey;
			ULONG len = 0;
			if (regKey.Open(HKEY_CURRENT_USER, BuildSubKey(pszSection), KEY_READ) != ERROR_SUCCESS)
				return false;
			if (regKey.QueryStringValue(pszValueName, nullptr, &len) != ERROR_SUCCESS || len == 0)
				return false;
			LPTSTR pszBuf = strRet.GetBuffer(len);
			if (regKey.QueryStringValue(pszValueName, pszBuf, &len) != ERROR_SUCCESS) {
				strRet.ReleaseBuffer(0); // prevent garbage
				return false;
			}
			strRet.ReleaseBuffer(len - 1); // remove terminating null
			return true;
		}

		/// Saves a BINARY value to the registry.
		bool SetBinaryValue(LPCTSTR pszSection, LPCTSTR pszValueName, const void* lpStruct, UINT uSize) {
			CRegKey regKey;
			if (regKey.Create(HKEY_CURRENT_USER, BuildSubKey(pszSection)) != ERROR_SUCCESS)
				return false;
			return ::RegSetValueEx(regKey, pszValueName, 0, REG_BINARY, reinterpret_cast<const BYTE*>(lpStruct), uSize) == ERROR_SUCCESS;
		}

		/// Queries a BINARY value from the registry.
		bool QueryBinaryValue(LPCTSTR pszSection, LPCTSTR pszValueName, void* lpStruct, UINT uSize) {
			CRegKey regKey;
			DWORD dwType = 0, dwSize = uSize;
			if (regKey.Open(HKEY_CURRENT_USER, BuildSubKey(pszSection)) != ERROR_SUCCESS)
				return false;
			return ::RegQueryValueEx(regKey, pszValueName, 0, &dwType, reinterpret_cast<BYTE*>(lpStruct), &dwSize) == ERROR_SUCCESS && dwType == REG_BINARY;
		}

	private:
		CString	m_strRegPath;	// Holds the name of the registry path.

		/// Builds a subkey path for the registry.
		CString BuildSubKey(LPCTSTR pszSection) const {
			CString path = m_strRegPath;
			if (pszSection && *pszSection) {
				path += _T('\\');
				path += pszSection;
			}
			return path;
		}
	};

} // namespace Henden
