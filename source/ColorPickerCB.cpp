// ColorPickerCB.cpp
//
// (c) 1998 James R. Twine
//
// Based On Code That Was Found On www.codeguru.com, And Was 
// � 1997 Baldvin Hansson
//
//	Bug Fixes And Additions:
//
//	Bug Fix From Marcel Galem:
//			Added Masking Of Inverted COLORREF Value.
//

#include "stdafx.h"
#include "ColorPickerCB.h"
#include "ISTool.h"
#include "MainFrm.h"

//
//	Load Standard Colors...
//	(If You Change This, Be Sure To Load CCB_MAX_COLORS Colors...)
//
SColorAndName	CColorPickerCB::ms_pColors[ CCB_MAX_COLORS ] =
{
	SColorAndName( RGB( 0x00, 0x00, 0x00 ),	"Black" ),
	SColorAndName( RGB( 0x80, 0x00, 0x00 ),	"Maroon" ),
	SColorAndName( RGB( 0x00, 0x80, 0x00 ),	"Green" ),
	SColorAndName( RGB( 0x80, 0x80, 0x00 ),	"Olive" ),
	SColorAndName( RGB( 0x00, 0x00, 0x80 ),	"Navy" ),
	SColorAndName( RGB( 0x80, 0x00, 0x80 ),	"Purple" ),
	SColorAndName( RGB( 0x00, 0x80, 0x80 ),	"Teal" ),
	SColorAndName( RGB( 0x80, 0x80, 0x80 ),	"Gray" ),
	SColorAndName( RGB( 0xC0, 0xC0, 0xC0 ),	"Silver" ),
	SColorAndName( RGB( 0xFF, 0x00, 0x00 ),	"Red" ),
	SColorAndName( RGB( 0x00, 0xFF, 0x00 ),	"Lime" ),
	SColorAndName( RGB( 0xFF, 0xFF, 0x00 ),	"Yellow" ),
	SColorAndName( RGB( 0x00, 0x00, 0xFF ),	"Blue" ),
	SColorAndName( RGB( 0xFF, 0x00, 0xFF ),	"Fuchsia" ),
	SColorAndName( RGB( 0x00, 0xFF, 0xFF ),	"Aqua" ),
	SColorAndName( RGB( 0xFF, 0xFF, 0xFF ),	"White" ),
	SColorAndName( RGB( 0x00, 0x00, 0x00 ), "Custom" ),
};

/////////////////////////////////////////////////////////////////////////////
// CColorPickerCB message handlers

LRESULT CColorPickerCB::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled) {
	Initialize();											// Initialize Contents
	SetCurSel(0);											// Select First Item By Default

	bHandled = FALSE;
	return( 0 );											// Done!
}


BOOL CColorPickerCB::SubclassWindow(HWND hWnd) {
	if(!CWindowImpl<CColorPickerCB,CComboBox>::SubclassWindow(hWnd))	// Subclass Control
		return FALSE;

	Initialize();											// Initialize Contents
	SetCurSel(0);											// Select First Item By Default

	return TRUE;											// Done!
}

void CColorPickerCB::Initialize( void )
{
	int		iAddedItem = -1;

	for( int iColor = 0; iColor < CCB_MAX_COLORS; iColor++ )// For All Colors
	{
		iAddedItem = AddString(	ms_pColors[
					iColor ].m_cColor );					// Set Color Name/Text
		if( iAddedItem == CB_ERRSPACE )						// If Not Added
			break;											// Stop
		else												// If Added Successfully
			SetItemData( iAddedItem, ms_pColors[
					iColor ].m_crColor );					// Set Color RGB Value
	}
}


LRESULT CColorPickerCB::OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	LPDRAWITEMSTRUCT pDIStruct = (LPDRAWITEMSTRUCT)lParam;
	static		CString	sColor;								// No Need To Reallocate Each Time

	CDC			dcContext;
	CRect		rItemRect( pDIStruct -> rcItem );
	CRect		rBlockRect( rItemRect );
	CRect		rTextRect( rBlockRect );
	CBrush		brFrameBrush;
	int			iFourthWidth = 0;
	int			iItem = pDIStruct -> itemID;
	int			iAction = pDIStruct -> itemAction;
	int			iState = pDIStruct -> itemState;
	COLORREF	crColor = 0;
	COLORREF	crNormal = GetSysColor( COLOR_WINDOW );
	COLORREF	crSelected = GetSysColor( COLOR_HIGHLIGHT );
	COLORREF	crText = GetSysColor( COLOR_WINDOWTEXT );

	dcContext.Attach(pDIStruct->hDC);

	iFourthWidth = ( rBlockRect.Width() / 4 );				// Get 1/4 Of Item Area
	brFrameBrush = AtlGetStockBrush(BLACK_BRUSH);			// Create Black Brush

	if( iState & ODS_SELECTED )								// If Selected
	{														// Set Selected Attributes
		dcContext.SetTextColor(
				( 0x00FFFFFF & ~( crText ) ) );				// Set Inverted Text Color (With Mask)
		dcContext.SetBkColor( crSelected );					// Set BG To Highlight Color
		dcContext.FillSolidRect( &rBlockRect, crSelected );	// Erase Item
	}
	else													// If Not Selected
	{														// Set Standard Attributes
		dcContext.SetTextColor( crText );					// Set Text Color
		dcContext.SetBkColor( crNormal );					// Set BG Color
		dcContext.FillSolidRect( &rBlockRect, crNormal );	// Erase Item
	}
	if( iState & ODS_FOCUS )								// If Item Has The Focus
		dcContext.DrawFocusRect( &rItemRect );				// Draw Focus Rect

	//
	//	Calculate Text Area
	//
	rTextRect.left += ( iFourthWidth + 2 );					// Set Start Of Text
	rTextRect.top += 2;										// Offset A Bit

	//
	//	Calculate Color Block Area
	//
	rBlockRect.DeflateRect( CSize( 2, 2 ) );				// Reduce Color Block Size
	rBlockRect.right = iFourthWidth;						// Set Width Of Color Block

	//
	//	Draw Color Text And Block
	//
	if( iItem != -1 )										// If Not An Empty Item
	{
		GetLBText( iItem, sColor );							// Get Color Text
		if( iState & ODS_DISABLED )	{						// If Disabled
			crColor = GetSysColor( COLOR_INACTIVECAPTIONTEXT );
			dcContext.SetTextColor( crColor );				// Set Text Color
		} else												// If Normal
			crColor = GetItemData( iItem );					// Get Color Value

		dcContext.FillSolidRect( &rBlockRect, crColor );	// Draw Color
		dcContext.FrameRect( &rBlockRect, brFrameBrush );	// Draw Frame

		dcContext.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));	// [BH]
		dcContext.SetBkMode( TRANSPARENT );					// Transparent Background
		dcContext.TextOut( rTextRect.left, rTextRect.top,
				sColor );									// Draw Color Name
	}
	dcContext.Detach();										// Detach DC From Object
	return TRUE;
}


COLORREF	CColorPickerCB::GetSelectedColorValue( void )
{
	int		iSelectedItem = GetCurSel();					// Get Selected Item

	if( iSelectedItem == CB_ERR )							// If Nothing Selected
		return( RGB( 0, 0, 0 ) );							// Return Black

	return( GetItemData( iSelectedItem ) );					// Return Selected Color
}


CString		CColorPickerCB::GetSelectedColorName( void )
{
	int		iSelectedItem = GetCurSel();					// Get Selected Item

	if( iSelectedItem == CB_ERR )							// If Nothing Selected
		return( m_sColorName = "" );			// Return Nothing (Not "Black!")

	GetLBText( iSelectedItem, m_sColorName );				// Store Name Of Color

	return( m_sColorName );									// Return Selected Color Name
}


void		CColorPickerCB::SetSelectedColorValue( COLORREF crClr )
{
	int		iItems = GetCount();
	
	for( int iItem = 0; iItem < iItems; iItem++ )
	{
		if( crClr == GetItemData( iItem ) )					// If Match Found
			SetCurSel( iItem );								// Select It
	}
	return;													// Done!
}


void		CColorPickerCB::SetSelectedColorName( PCSTR cpColor )
{
	int		iItems = GetCount();
	CString	sColorName;

	for( int iItem = 0; iItem < iItems; iItem++ )
	{
		GetLBText( iItem, sColorName );						// Get Color Name

		if( !sColorName.CompareNoCase( cpColor ) )			// If Match Found
			SetCurSel( iItem );								// Select It
	}
	return;													// Done!
}


bool		CColorPickerCB::RemoveColor( PCSTR cpColor )
{
	int		iItems = GetCount();
	CString	sColor;
	bool	bRemoved = false;

	for( int iItem = 0; iItem < iItems; iItem++ )
	{
		GetLBText( iItem, sColor );							// Get Color Name
		if( !sColor.CompareNoCase( cpColor ) )				// If Match Found
		{
			DeleteString( iItem );							// Remove It
			bRemoved = true;								// Set Flag
			break;											// Stop Checking
		}
	}
	return( bRemoved );										// Done!
}


bool		CColorPickerCB::RemoveColor( COLORREF crClr )
{
	int		iItems = GetCount();
	bool	bRemoved = false;

	for( int iItem = 0; iItem < iItems; iItem++ )
	{
		if( crClr == GetItemData( iItem ) )					// If Desired Color Found
		{
			DeleteString( iItem );							// Remove It
			bRemoved = true;								// Set Flag
			break;											// Stop Checking
		}
	}
	return( bRemoved );										// Done!
}


int			CColorPickerCB::AddColor( PCSTR cpName, COLORREF crColor )
{
	int		iItem = -1;

	iItem = InsertString( -1, cpName );						// Insert String
	if( iItem != LB_ERR )									// If Insert Good
		SetItemData( iItem, crColor );						// Set Color Value 

	return( iItem );										// Done! Return Location
}

LRESULT CColorPickerCB::OnSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled) {
	CString strSelColor = GetSelectedColorName();
	if(!strSelColor.CompareNoCase("Custom")) {
		COLORREF cr = GetSelectedColorValue();
		CColorDialog dlg(cr,CC_FULLOPEN,AfxGetMainWnd());
		if(dlg.DoModal()==IDOK) {
			cr = dlg.GetColor();
			int iSelectedItem = GetCurSel();					// Get Selected Item
			if(iSelectedItem != CB_ERR) {
				SetItemData(iSelectedItem,cr);
			}
		}
	} else
		bHandled = FALSE;

	return 0;
}

void CColorPickerCB::SetInnoColor(LPCTSTR lpszColor) {
	ATLASSERT(lpszColor && _tcslen(lpszColor));
	CString str(lpszColor);
	if(!str.Left(2).CompareNoCase("cl")) {
		str = str.Mid(2);
		SetSelectedColorName(str);
	} else if(str[0]=='$') {
		COLORREF cr;
		sscanf(str,"$%x",&cr);
//		cr = RGB(GetBValue(cr),GetGValue(cr),GetRValue(cr));	// Get correct byte order
		SetSelectedColorName("Custom");
		SetItemData(GetCurSel(),cr);
	} else {
		AtlMessageBox(m_hWnd,_L("Invalid color value."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
	}
}

void CColorPickerCB::GetInnoColor(CString& refStr) {
	CString str = GetSelectedColorName();
	if(!str.CompareNoCase("Custom")) {
		COLORREF cr = GetSelectedColorValue();
		refStr.Format("$%02x%02x%02x",
			GetBValue(cr),
			GetGValue(cr),
			GetRValue(cr)
		);
	} else {
		refStr.Format("cl%s",str);
	}
}
