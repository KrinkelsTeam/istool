// WTL complete
#pragma once

#include "MyDoc.h"
#include <fcntl.h>
#include <io.h>

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectFonts dialog

// Macros for TrueType portability
#define FS_2BYTE(p)  (((unsigned short)((p)[0]) << 8) | (p)[1])
#define FS_4BYTE(p)  (FS_2BYTE((p)+2) | ((FS_2BYTE(p) + 0L) << 16))
#define SWAPW(a)     ((short) FS_2BYTE((unsigned char FAR*)(&a)))
#define SWAPL(a)     ((long) FS_4BYTE((unsigned char FAR*)(&a)))

#define tag_NamingTable  0x656d616e  /* 'name' */

class CDlgSelectFonts :
	public CDialogImpl<CDlgSelectFonts>,
	public CMyDialogBase<CDlgSelectFonts>
{
public:
	enum { IDD = IDD_SELECTFONTS };

	BEGIN_MSG_MAP(CDlgSelectFonts)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		NOTIFY_HANDLER(IDC_FONTLIST, LVN_DELETEITEM, OnDeleteItem)
		CHAIN_MSG_MAP(CMyDialogBase<CDlgSelectFonts>)
	END_MSG_MAP()

	CDlgSelectFonts(CMyDoc* pDoc) : m_pDoc(pDoc) {}

	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&) {
		_L(m_hWnd, _T("SelectFonts"));
		CenterWindow(GetParent());

		CWaitCursor wait;
		m_wndFontList.Attach(GetDlgItem(IDC_FONTLIST));
		m_wndFontList.InsertColumn(0, _T("Font Name"), LVCFMT_LEFT, 10, 0);
		m_wndFontList.InsertColumn(1, _T("File Name"), LVCFMT_LEFT, 120, 0);

		EnumTTF();
		//EnumFON();

		CRect rc;
		m_wndFontList.GetClientRect(rc);
		m_wndFontList.SetColumnWidth(0, rc.Width() - 120);
		m_wndFontList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		return TRUE;
	}

	LRESULT OnCloseCmd(WORD, WORD wID, HWND, BOOL&) {
		if (wID == IDOK) {
			if (m_wndFontList.GetSelectedCount() == 0) {
				EndDialog(IDCANCEL);
				return 0;
			}

			UINT nCount = m_wndFontList.GetItemCount();
			for (UINT nItem = 0; nItem < nCount; nItem++) {
				if (m_wndFontList.GetItemState(nItem, LVIS_SELECTED) == LVIS_SELECTED) {
					CData* pData = (CData*)m_wndFontList.GetItemData(nItem);

					CString strLine;
					strLine.Format(_T("Source: %s; DestDir: {fonts}; ")
						_T("Flags: onlyifdoesntexist uninsneveruninstall%s; FontInstall: %s"),
						pData->m_szFilePath,
						pData->m_bTrueType ? _T("") : _T(" fontisnttruetype"),
						pData->m_szFaceName);

					m_pDoc->GetScript().AddLine(new CInnoScript::CLineParam(CInnoScript::SEC_FILES, strLine));
				}
			}

			m_pDoc->SetModifiedFlag(TRUE);
		}
		EndDialog(wID);
		return 0;
	}

	LRESULT OnDeleteItem(int, NMHDR* pNMHDR, BOOL&) {
		NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
		CData* pData = (CData*)m_wndFontList.GetItemData(pNMListView->iItem);
		delete pData;
		return 0;
	}

protected:
	CListViewCtrl m_wndFontList;
	CMyDoc* m_pDoc;

	typedef struct {
		TCHAR m_szFaceName[MAX_PATH];
		TCHAR m_szFilePath[MAX_PATH];
		bool  m_bTrueType;
	} CData;

	typedef short int16;
	typedef unsigned short uint16;
	typedef long int32;
	typedef unsigned long uint32;
	typedef long sfnt_TableTag;

	typedef struct {
		uint16 platformID;
		uint16 specificID;
		uint16 languageID;
		uint16 nameID;
		uint16 length;
		uint16 offset;
	} sfnt_NameRecord;

	typedef struct {
		uint16 format;
		uint16 count;
		uint16 stringOffset;
	} sfnt_NamingTable;

	typedef struct {
		sfnt_TableTag tag;
		uint32 checkSum;
		uint32 offset;
		uint32 length;
	} sfnt_DirectoryEntry;

	typedef struct {
		int32 version;                  /* 0x10000 (1.0) */
		uint16 numOffsets;              /* number of tables */
		uint16 searchRange;             /* (max2 <= numOffsets)*16 */
		uint16 entrySelector;           /* log2 (max2 <= numOffsets) */
		uint16 rangeShift;              /* numOffsets*16-searchRange*/
		sfnt_DirectoryEntry table[1];   /* table[numOffsets] */
	} sfnt_OffsetTable;

	// Read long font name from TTF file using Windows Unicode (UTF-16 BE)
	CString ReadLongName(LPCTSTR pszFile) {
		int fp;
		if (_tsopen_s(&fp, pszFile, O_RDONLY | O_BINARY, _SH_DENYNO, 0) != 0)
			return _T("");

		sfnt_OffsetTable offsetTable;
		if (_read(fp, &offsetTable, sizeof(offsetTable) - sizeof(sfnt_DirectoryEntry)) != sizeof(offsetTable) - sizeof(sfnt_DirectoryEntry)) {
			_close(fp);
			return _T("");
		}

		UINT numTables = SWAPW(offsetTable.numOffsets);
		sfnt_DirectoryEntry table;

		for (UINT i = 0; i < numTables; i++) {
			if (_read(fp, &table, sizeof(table)) != sizeof(table)) {
				_close(fp);
				return _T("");
			}
			if (table.tag == tag_NamingTable) {
				DWORD nameTableOffset = SWAPL(table.offset);
				_lseek(fp, nameTableOffset, SEEK_SET);

				sfnt_NamingTable namingTable;
				if (_read(fp, &namingTable, sizeof(namingTable)) != sizeof(namingTable)) {
					_close(fp);
					return _T("");
				}

				UINT nameCount = SWAPW(namingTable.count);
				UINT stringOffset = SWAPW(namingTable.stringOffset);

				for (UINT j = 0; j < nameCount; j++) {
					sfnt_NameRecord record;
					if (_read(fp, &record, sizeof(record)) != sizeof(record)) break;

					if (SWAPW(record.nameID) != 4) continue; // Full font name
					if (SWAPW(record.platformID) != 3 || SWAPW(record.specificID) != 1) continue; // Windows Unicode

					UINT len = SWAPW(record.length);
					UINT ofs = SWAPW(record.offset);
					LONG cur = _tell(fp);

					_lseek(fp, nameTableOffset + stringOffset + ofs, SEEK_SET);

					// Read UTF-16BE string and convert to native UTF-16LE
					CAtlArray<BYTE> buf;
					buf.SetCount(len);
					if (_read(fp, buf.GetData(), len) != (int)len) break;

					CAtlArray<WCHAR> out;
					out.SetCount(len / 2 + 1);
					for (UINT k = 0; k < len; k += 2)
						out[k / 2] = (buf[k] << 8) | buf[k + 1];
					out[len / 2] = 0;

					_close(fp);
					return CString(out.GetData());
				}
				break;
			}
		}
		_close(fp);
		return _T("");
	}

	// Get Fonts folder path using shell API
	bool GetShellFolderPath(int nFolder, CString& strOut) {
		LPMALLOC pMalloc;
		LPITEMIDLIST pidl;
		bool bResult = false;

		if (FAILED(SHGetMalloc(&pMalloc))) pMalloc = NULL;
		if (SUCCEEDED(SHGetSpecialFolderLocation(0, nFolder, &pidl))) {
			strOut.Empty();
			if (SHGetPathFromIDList(pidl, strOut.GetBuffer(MAX_PATH + 1)))
				bResult = true;
			if (pMalloc)
				pMalloc->Free(pidl);
		}
		strOut.ReleaseBuffer();
		return bResult;
	}

	// Enumerate .TTF files from Fonts folder
	void EnumTTF() {
		WIN32_FIND_DATA wfd;
		CString strFonts, strWild;

		if (!GetShellFolderPath(CSIDL_FONTS, strFonts)) return;
		CMyUtils::EndWith(strFonts, _T('\\'));
		strWild.Format(_T("%s*.ttf"), strFonts);

		HANDLE hFind = FindFirstFile(strWild, &wfd);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				CString strFilePath(strFonts + wfd.cFileName);
				CString strFaceName = ReadLongName(strFilePath);
				if (strFaceName.IsEmpty()) continue;

				CData* pData = new CData;
				_tcscpy_s(pData->m_szFaceName, _countof(pData->m_szFaceName), strFaceName);
				_tcscpy_s(pData->m_szFilePath, _countof(pData->m_szFilePath), strFilePath);
				pData->m_bTrueType = true;

				strFaceName += _T(" (TrueType)");
				int nItem = m_wndFontList.InsertItem(0, strFaceName);
				m_wndFontList.SetItemText(nItem, 1, wfd.cFileName);
				m_wndFontList.SetItemData(nItem, (DWORD_PTR)pData);
			} while (FindNextFile(hFind, &wfd));
			FindClose(hFind);
		}
	}
};
