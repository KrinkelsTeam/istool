#pragma once

// Debug client -> debugger messages
#define WM_Debugger_Hello (WM_USER + 0x700)
#define WM_Debugger_Goodbye (WM_USER + 0x701)
#define WM_Debugger_Stepped (WM_USER + 0x702)
#define WM_Debugger_SteppedIntermediate (WM_USER + 0x703)
#define WM_Debugger_Exception (WM_USER + 0x704)
#define WM_Debugger_SetForegroundWindow (WM_USER + 0x705)
#define WM_Debugger_QueryVersion (WM_USER + 0x706)
#define WM_Debugger_CallStackCount (WM_USER + 0x707)

// Debug client -> debugger WM_COPYDATA messages
#define CD_Debugger_ReplyW 0x700
#define CD_Debugger_ExceptionW 0x701
#define CD_Debugger_UninstExeW 0x702
#define CD_Debugger_LogMessageW 0x703
#define CD_Debugger_TempDirW 0x704
#define CD_Debugger_CallStackW 0x705

// Debugger -> debug client messages
#define WM_DebugClient_Detach (WM_USER + 0x800)
#define WM_DebugClient_Continue (WM_USER + 0x801)
#define WM_DebugClient_SetForegroundWindow (WM_USER + 0x803)

// Debugger -> debug client WM_COPYDATA messages
#define CD_DebugClient_EvaluateConstantW 0x800
#define CD_DebugClient_EvaluateVariableEntry 0x801
#define CD_DebugClient_CompiledCodeTextA 0x802
#define CD_DebugClient_CompiledCodeDebugInfoA 0x803
