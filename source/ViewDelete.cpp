// ViewDelete.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewDelete.h"
#include "StringToken.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewDelete

CViewDelete::CViewDelete(CInnoScript::SECTION sec) : CMyListView(sec) {
	m_nItemImage = 17;
}

const CListInfo CViewDelete::m_colInfo[] = {
	_T("Name"),				LVCFMT_LEFT,	250, 0,
	_T("Type"),				LVCFMT_LEFT,	180, 0,
	_T("MinVersion"),		LVCFMT_LEFT,	 80, CLI_HIDE,
	_T("OnlyBelowVersion"),	LVCFMT_LEFT,	 80, CLI_HIDE,
	_T("Components"),		LVCFMT_LEFT,	100, CLI_HIDE,
	_T("Tasks"),			LVCFMT_LEFT,	100, CLI_HIDE,
	_T("Languages"),		LVCFMT_LEFT,	 80, CLI_HIDE,
	NULL
};
