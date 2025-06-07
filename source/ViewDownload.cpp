// ViewDelete.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewDownload.h"
#include "StringToken.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewCompileStep

CViewDownload::CViewDownload() : CMyListView(CInnoScript::PRJ_DOWNLOAD)
{
	m_nItemImage = 15;
}

/////////////////////////////////////////////////////////////////////////////
// CViewCompileStep message handlers

const CListInfo CViewDownload::m_colInfo[] = {
	_T("Source"),		LVCFMT_LEFT,	250,	0,
	_T("DestDir"),		LVCFMT_LEFT,	180,	0,
	_T("DestName"),		LVCFMT_LEFT,	180,	0,
	NULL
};
