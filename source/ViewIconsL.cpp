// ViewIconsB.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewIconsL.h"
#include "MyDoc.h"
#include "StringToken.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewIconsL

CViewIconsL::CViewIconsL() : CMyListView(CInnoScript::SEC_ICONS) {
	m_nItemImage = 5;
}

/////////////////////////////////////////////////////////////////////////////
// CViewIconsL message handlers

const CListInfo CViewIconsL::m_colInfo[] = {
	_T("Name"),				LVCFMT_LEFT,	140, 0,
	_T("Filename"),			LVCFMT_LEFT,	200, 0,
	_T("Parameters"),		LVCFMT_LEFT,	150, CLI_HIDE,
	_T("WorkingDir"),		LVCFMT_LEFT,	160, 0,
	_T("IconFilename"),		LVCFMT_LEFT,	140, CLI_HIDE,
	_T("IconIndex"),		LVCFMT_RIGHT,	 60, CLI_HIDE,
	_T("Comment"),			LVCFMT_LEFT,	120, CLI_HIDE,
	_T("Flags"),			LVCFMT_LEFT,	150, 0,
	_T("MinVersion"),		LVCFMT_LEFT,	 80, CLI_HIDE,
	_T("OnlyBelowVersion"),	LVCFMT_LEFT,	 80, CLI_HIDE,
	_T("Components"),		LVCFMT_LEFT,	100, CLI_HIDE,
	_T("Tasks"),			LVCFMT_LEFT,	100, CLI_HIDE,
	_T("Languages"),		LVCFMT_LEFT,	 80, CLI_HIDE,
	NULL
};
