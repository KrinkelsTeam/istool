// ViewIni.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewIni.h"
#include "MyDoc.h"
#include "StringToken.h"
#include "Sheets.h"
#include "FilesHelper.h"

/////////////////////////////////////////////////////////////////////////////
// CViewIni

CViewIni::CViewIni() : CMyListView(CInnoScript::SEC_INI) {
	SetSubMenu(3);
	m_nItemImage = 4;
	m_dwFlags |= VFL_DRAGACCEPTFILES;
}

/////////////////////////////////////////////////////////////////////////////
// CViewIni message handlers

const CListInfo CViewIni::m_colInfo[] = {
	_T("Filename"),			LVCFMT_LEFT,	180, 0,
	_T("Section"),			LVCFMT_LEFT,	120, 0,
	_T("Key"),				LVCFMT_LEFT,	130, 0,
	_T("String"),			LVCFMT_LEFT,	160, 0,
	_T("Flags"),			LVCFMT_LEFT,	150, 0,
	_T("MinVersion"),		LVCFMT_LEFT,	 80, CLI_HIDE,
	_T("OnlyBelowVersion"),	LVCFMT_LEFT,	 80, CLI_HIDE,
	_T("Components"),		LVCFMT_LEFT,	100, CLI_HIDE,
	_T("Tasks"),			LVCFMT_LEFT,	100, CLI_HIDE,
	_T("Languages"),		LVCFMT_LEFT,	 80, CLI_HIDE,
	NULL
};

const CListInfo* CViewIni::GetListInfo() {
	return m_colInfo;
}

LRESULT CViewIni::OnDropFiles(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	HDROP hDropInfo = (HDROP)wParam;
	CFilesHelper helper(GetDocument());

	UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
	for (UINT iFile = 0; iFile < nFiles; iFile++) {
		TCHAR szFileName[_MAX_PATH];
		::DragQueryFile(hDropInfo, iFile, szFileName, _MAX_PATH);
		helper.ImportIni(m_hWnd, szFileName);
	}
	::DragFinish(hDropInfo);
	return 0;
}
