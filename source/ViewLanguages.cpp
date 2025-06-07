// ViewLanguages.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "ViewLanguages.h"
#include "MyDoc.h"
#include "StringToken.h"
#include "FilesHelper.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CViewLanguages

CViewLanguages::CViewLanguages() : CMyListView(CInnoScript::SEC_LANGUAGES)
{
	m_nItemImage = 14;
}

/////////////////////////////////////////////////////////////////////////////
// CViewLanguages message handlers

const CListInfo CViewLanguages::m_colInfo[] = {
	_T("Name"),				LVCFMT_LEFT,	100, 0,
	_T("MessagesFile"),		LVCFMT_LEFT,	120, 0,
	_T("LicenseFile"),		LVCFMT_LEFT,	120, 0,
	_T("InfoBeforeFile"),	LVCFMT_LEFT,	120, 0,
	_T("InfoAfterFile"),	LVCFMT_LEFT,	120, 0,
	NULL
};
