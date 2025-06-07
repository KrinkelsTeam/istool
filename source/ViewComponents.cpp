// ViewComponents.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewComponents.h"
#include "MyDoc.h"
#include "StringToken.h"
#include "FilesHelper.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewComponents

CViewComponents::CViewComponents() : CMyListView(CInnoScript::SEC_COMPONENTS)
{
	m_nItemImage = 13;
}


/////////////////////////////////////////////////////////////////////////////
// CViewComponents message handlers

const CListInfo CViewComponents::m_colInfo[] = {
	_T("Name"),						LVCFMT_LEFT,	100, 0,
	_T("Description"),				LVCFMT_LEFT,	120, 0,
	_T("ExtraDiskSpaceRequired"),	LVCFMT_RIGHT,	120, 0,
	_T("Types"),					LVCFMT_LEFT,	150, 0,
	_T("Flags"),					LVCFMT_LEFT,	150, 0,
	_T("MinVersion"),				LVCFMT_LEFT,	 80, CLI_HIDE,
	_T("OnlyBelowVersion"),			LVCFMT_LEFT,	 80, CLI_HIDE,
	_T("Languages"),				LVCFMT_LEFT,	 80, CLI_HIDE,
	NULL
};

void CViewComponents::OnDeleteItem(CScriptLine* pLine) {
	LPCTSTR pszComponent = pLine->GetParameter(_T("Name"));
	if (!pszComponent || !*pszComponent) return;

	CScriptList list;
	UINT nCount = 0;
	GetDocument()->GetScript().GetCompleteList(list);
	for (int i = 0; i < list.GetSize(); i++) {
		if (list[i]->GetParameterFlag(_T("Components"), pszComponent)) {
			list[i]->SetParameterFlag(_T("Components"), pszComponent, false);
			nCount++;
		}
	}
#ifdef _DEBUG
	CString tmp;
	tmp.Format(_T("Removed %d references to the %s component."), nCount, pszComponent);
	AtlMessageBox(m_hWnd, (LPCTSTR)tmp, IDR_MAINFRAME);
#endif
}
