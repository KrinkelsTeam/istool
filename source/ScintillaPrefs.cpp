#include "stdafx.h"
#include "ScintillaPrefs.h"
#include "ISTool.h"

CScintillaPrefs::Scheme CScintillaPrefs::m_schemes[] = {
	1,	_T("Inno Setup Script"),
	2,	_T("Code Section")
};

CScintillaPrefs::Style CScintillaPrefs::m_styles[] = {
	1,	_T("Default"),		STYLE_DEFAULT,		RGB(0,0,0),		RGB(255,255,255),false,false,false,
	1,	_T("Comment"),		SCE_ISS_COMMENT,	RGB(0,128,0),	CLR_DEFAULT,false,false,false,
	1,	_T("Directive"),	SCE_ISS_DIRECTIVE,	RGB(0,0,255),	CLR_DEFAULT,false,false,false,
	1,	_T("Section"),		SCE_ISS_SECTION,	RGB(0,0,0),		CLR_DEFAULT,true,false,false,
	1,	_T("Constant"),		SCE_ISS_CONSTANT,	RGB(255,64,64), CLR_DEFAULT,false,false,false,
	1,	_T("String"),		SCE_ISS_STRING,		RGB(96,96,96),	CLR_DEFAULT,false,false,false,
	1,	_T("Flag"),			SCE_ISS_FLAG,		RGB(128,32,32),	CLR_DEFAULT,false,false,false,
	1,	_T("PreProcessor"),	SCE_ISS_PREPROC,	RGB(0,128,255),	CLR_DEFAULT,false,false,false,

	2,	_T("Default"),		STYLE_DEFAULT,		RGB(0,0,0),		RGB(255,255,255),false,false,false,
	2,	_T("Comment"),		STYLE_DEFAULT,		RGB(0,128,0),	CLR_DEFAULT,false,false,false,
	2,	_T("Line Comment"),	STYLE_DEFAULT,		RGB(0,128,0),	CLR_DEFAULT,false,false,false,
	2,	_T("Word"),			STYLE_DEFAULT,		RGB(0,0,255),	CLR_DEFAULT,false,false,false
};

CScintillaPrefs::KeyWords CScintillaPrefs::m_keywords[] = {
	// Inno Setup Script
	1,	0,	_T("setup files dirs registry"),
	1,	1,	_T("ignoreversion comparetimestamp"),
	1,	2,	_T("flags source destdir destname"),
	// Code Section
	2,	0,	_T("program const type var begin continue end array set packed record string if then else ")
			_T("while for to downto do with repeat until case of goto exit label procedure ")
			_T("function nil file and or not xor div mod unit uses implementation interface ")
			_T("external asm inline object constructor destructor virtual far assembler near ")
			_T("inherited stdcall cdecl library export exports end. ")
			_T("class ansistring raise try except on index name finally ")
			_T("resourcestring false true initialization finalization ")
			_T("override overload"),
	2,	1,	_T("write read default public protected private property published stored")
};

long CScintillaPrefs::GetKeyWordsCount() {
	return sizeof(m_keywords) / sizeof(m_keywords[0]);
}

long CScintillaPrefs::GetStyleCount() {
	return sizeof(m_styles) / sizeof(m_styles[0]);
}

void CScintillaPrefs::SavePrefs(CMyApp& app) {
	for (int i = 0; i < NUMSTYLES; i++) {
		CString strEntry;
		strEntry.Format(_T("Style%X"), i);

		Style s = m_styles[i];
		s.pszName = 0;
		app.WriteProfileBinary(_T("Editor"), strEntry, &s, sizeof s);
	}
}

void CScintillaPrefs::LoadPrefs(CMyApp& app) {
	for (int i = 0; i < NUMSTYLES; i++) {
		CString strEntry;
		strEntry.Format(_T("Style%X"), i);

		Style s;
		ULONG nSize = sizeof s;
		if (!app.GetProfileBinary(_T("Editor"), strEntry, &s, &nSize) || nSize != sizeof s)
			break;

		s.pszName = m_styles[i].pszName;
		m_styles[i] = s;
	}
}
