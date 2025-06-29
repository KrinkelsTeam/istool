#include "stdafx.h"
#include "ISTool.h"
#include "MainFrm.h"

static LRESULT SendCopyDataMessageStr(HWND DestWnd, HWND SourceWnd, DWORD CopyDataMsg, LPCTSTR Data) {
	COPYDATASTRUCT CopyDataStruct;

	CopyDataStruct.dwData = CopyDataMsg;
	CopyDataStruct.cbData = (_tcslen(Data) + 1) * sizeof(TCHAR);
	CopyDataStruct.lpData = (PVOID)Data;
	
	return SendMessage(DestWnd, WM_COPYDATA, WPARAM(SourceWnd), LPARAM(&CopyDataStruct));
}

LRESULT CMainFrame::OnDebuggerCopyData(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	CString tmp;
	COPYDATASTRUCT* pcds = reinterpret_cast<COPYDATASTRUCT*>(lParam);

	switch (pcds->dwData) {
	case CD_Debugger_LogMessageW:
		tmp = CString((LPCTSTR)pcds->lpData);
		// DebugLogMessage(tmp);
		return 1;
	default:
		tmp = CString((LPCTSTR)pcds->lpData);
		return 1;
	}

	return 0;
}

LRESULT CMainFrame::OnDebuggerHello(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	HWND hDebugClientWnd = HWND(wParam);
	BOOL bWantCodeText = BOOL(lParam);
	HANDLE hProcess = 0;

	DWORD dwPID = 0;
	::GetWindowThreadProcessId(hDebugClientWnd, &dwPID);
	if (dwPID != 0)
		hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwPID);

	if (bWantCodeText)
		SendCopyDataMessageStr(hDebugClientWnd, m_hWnd, CD_DebugClient_CompiledCodeTextA, _T("FCompiledCodeText"));
	SendCopyDataMessageStr(hDebugClientWnd, m_hWnd, CD_DebugClient_CompiledCodeDebugInfoA, _T("FCompiledCodeDebugInfo"));
	return 0;
}
