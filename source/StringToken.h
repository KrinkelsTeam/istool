// StringToken.h

#pragma once

class CStringToken {
public:
	CStringToken(LPCTSTR lpszLine, LPCTSTR lpszDel) : m_ptr(0) {
		m_ptr = new TCHAR[_tcslen(lpszLine) + 1];
		_tcscpy_s(m_ptr, _tcslen(lpszLine) + 1, lpszLine);
		m_del = new TCHAR[_tcslen(lpszDel) + 1];
		_tcscpy_s(m_del, _tcslen(lpszDel) + 1, lpszDel);
		m_next = m_ptr;
	}

	~CStringToken() {
		delete[]m_ptr;
		delete[]m_del;
	}

	LPCTSTR GetNext() {
		while (_istspace(*m_next)) m_next++;
		if (!*m_next) return nullptr;
		LPTSTR retptr = m_next;
		bool bQuote = false;
		while (*m_next) {
			if (*m_next == _T('"')) {
				bQuote = !bQuote;
			}
			if (!bQuote && _tcschr(m_del, *m_next)) {
				LPTSTR sp = m_next - 1;
				*m_next++ = 0;
				while (_istspace(*sp) && sp > retptr) *sp-- = 0;
				break;
			}
			m_next++;
		}
		return retptr;
	}

	LPCTSTR GetRest() {
		while (_istspace(*m_next)) m_next++;
		LPTSTR ptr = m_next;
		while (*ptr && *(ptr + 1)) ptr++;
		while (_istspace(*ptr)) *ptr-- = 0;
		ptr = m_next;
		m_next = nullptr;
		return ptr;
	}

private:
	LPTSTR m_ptr;
	LPTSTR m_del;
	LPTSTR m_next;
};
