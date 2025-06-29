// ViewDelete.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewCompileStep.h"
#include "StringToken.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewCompileStep

CViewCompileStep::CViewCompileStep(CInnoScript::SECTION sec) : CMyListView(sec)
{
	m_nItemImage = 18;
}

/////////////////////////////////////////////////////////////////////////////
// CViewCompileStep message handlers

const CListInfo CViewCompileStep::m_colInfo[] = {
	_T("Name"),			LVCFMT_LEFT,	250,	0/*Flags*/,
	_T("Parameters"),	LVCFMT_LEFT,	180,	0,
	_T("Flags"),		LVCFMT_LEFT,	180,	0,
	NULL
};
