// ViewRegistry.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewRegistryL.h"
#include "MyDoc.h"
#include "FilesHelper.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewRegistryL

CViewRegistryL::CViewRegistryL() : CMyListView(CInnoScript::SEC_REGISTRY) {
	SetSubMenu(2);
	m_nItemImage = 6;
	m_dwFlags |= VFL_DRAGACCEPTFILES;
}

/////////////////////////////////////////////////////////////////////////////
// CViewRegistryL message handlers

const CListInfo CViewRegistryL::m_colInfo[] = {
	_T("Root"),				LVCFMT_LEFT,	140, 0,
	_T("Subkey"),			LVCFMT_LEFT,	200, 0,
	_T("ValueType"),		LVCFMT_LEFT,	 60, 0,
	_T("ValueName"),		LVCFMT_LEFT,	100, 0,
	_T("ValueData"),		LVCFMT_LEFT,	120, 0,
	_T("Flags"),			LVCFMT_LEFT,	150, 0,
	_T("MinVersion"),		LVCFMT_LEFT,	 80, CLI_HIDE,
	_T("OnlyBelowVersion"),	LVCFMT_LEFT,	 80, CLI_HIDE,
	_T("Components"),		LVCFMT_LEFT,	100, CLI_HIDE,
	_T("Tasks"),			LVCFMT_LEFT,	100, CLI_HIDE,
	_T("Languages"),		LVCFMT_LEFT,	 80, CLI_HIDE,
	NULL
};

LRESULT CViewRegistryL::OnDropFiles(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	HDROP hDropInfo = (HDROP)wParam;
	SetActiveWindow(); // activate us first !
	CFilesHelper helper(GetDocument());
	helper.OnDropFilesRegistry(m_hWnd, hDropInfo);
	Populate();
	return 0;
}
