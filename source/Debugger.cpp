#include "stdafx.h"
#include "ISTool.h"
#include "MainFrm.h"

static LRESULT SendCopyDataMessageStr(HWND DestWnd, HWND SourceWnd, DWORD CopyDataMsg, LPCTSTR Data) {
	COPYDATASTRUCT CopyDataStruct;

	CopyDataStruct.dwData = CopyDataMsg;
	CopyDataStruct.cbData = strlen(Data);
	CopyDataStruct.lpData = (PVOID)Data;
	
	return SendMessage(DestWnd, WM_COPYDATA, WPARAM(SourceWnd), LPARAM(&CopyDataStruct));
}

LRESULT CMainFrame::OnDebuggerCopyData(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	CString tmp;
	COPYDATASTRUCT* pcds = reinterpret_cast<COPYDATASTRUCT*>(lParam);

	switch (pcds->dwData) {
	case CD_Debugger_LogMessage:
		_tcsncpy_s(tmp.GetBufferSetLength(pcds->cbData), pcds->cbData + 1, (char*)pcds->lpData, pcds->cbData);
		tmp = LPCTSTR(pcds->lpData);
		//SetString(S, PChar(Message.CopyDataStruct.lpData),
		//Message.CopyDataStruct.cbData);
		//DebugLogMessage(S);
		return 1;
	default:
		_tcsncpy_s(tmp.GetBufferSetLength(pcds->cbData), pcds->cbData + 1, (char*)pcds->lpData, pcds->cbData);
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
		SendCopyDataMessageStr(hDebugClientWnd, m_hWnd, CD_DebugClient_CompiledCodeText, _T("FCompiledCodeText"));
	SendCopyDataMessageStr(hDebugClientWnd, m_hWnd, CD_DebugClient_CompiledCodeDebugInfo, _T("FCompiledCodeDebugInfo"));
	return 0;
}
