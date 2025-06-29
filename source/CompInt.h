// CompInt.h : Compiler Interface

#pragma once

/*
	Inno Setup
	Copyright (C) 1997-2024 Jordan Russell
	Portions by Martijn Laan
	For conditions of distribution and use, see LICENSE.TXT.

	Compiler interface records and other types
*/

/* Constants passed in Code parameter of callback function */
enum {
	iscbReadScript = 1,		/* Sent when compiler needs the next script line */
	iscbNotifyStatus = 2,	/* Sent to notify the application of compiler status */
	iscbNotifyIdle = 3,		/* Sent at various intervals during the compilation */
	iscbNotifySuccess = 4,	/* Sent when compilation succeeds */
	iscbNotifyError = 5,	/* Sent when compilation fails or is aborted by the
							   application */
	iscbNotifyPreproc = 6	/* Sent to notify the application of preprocessor results */
};

/* Return values for callback function */
enum {
	iscrSuccess = 0,		/* Return this for compiler to continue */
	iscrRequestAbort = 1	/* Return this to abort compilation immediately. 
							   (When this value is returned, it is not necessary 
							   to set any of the "out" fields in the
							   TCompilerCallbackData; the compiler will ignore them.) */
};

/* Return values for ISDllCompileScript */
enum {
	isceNoError = 0,		/* Successful */
	isceInvalidParam = 1,	/* Bad parameters passed to function */
	isceCompileFailure = 2	/* There was an error compiling or it was aborted
							   by the application */
};

/* TCompilerCallbackData is a record passed to the callback function. The
  fields which you may access vary depending on what Code was passed to the
  callback function. */
typedef struct {
	union {
		struct {
			bool Reset;			/* [in] This field can be ignored in compiler
								   versions 3.0.1 and later. (Previous versions
								   of the compiler made multiple passes over the
								   script, and set Reset to True when it needed
								   to return to the beginning.) */
			LPTSTR LineRead;	/* [out] Application returns pointer to the next
								   line it reads, or a NULL pointer if the end of
								   file is reached. Application is responsible for
								   allocating a buffer to hold the line; LineRead
								   is initially NULL when the callback function
								   is called. The pointer only needs to remain
								   valid until the next time the callback function
								   is called (i.e. the application may return the
								   same pointer each time). */
		} ReadScript;

		struct {
			LPTSTR StatusMsg;	/* [in] Contents of status message. */
			BOOL Warning;		/* [in] Warning indicator (new in 6.0.0) */
		} NotifyStatus;

		struct {
			DWORD CompressProgress;			/* [in] Amount compressed so far (new in 4.1.6) */
			DWORD CompressProgressMax;		/* [in] Maximum value of CompressProgress (new in 4.1.6) */
			LONG SecondsRemaining;			/* [in] Estimated time remaining, or -1
										       if not known (new in 5.1.13) */
			DWORD BytesCompressedPerSecond; /* [in] Average bytes compressed per second (new in 5.1.13) */
		} NotifyIdle;

		struct {
			LPTSTR PreprocessedScript;	/* [in] Preprocessed script (new in 6.1.0) */
			LPTSTR IncludedFilenames;	/* [in] Names of #included files. Each name is
										   a null-terminated string, and the final
										   name is followed by an additional null
										   character (new in 6.1.0) */
		} NotifyPreproc;

		struct {
			LPTSTR OutputExeFilename;	/* [in] The name of the resulting setup.exe,
										   or empty if output was disabled
										   (latter new in 5.5.5) */
			void* DebugInfo;			/* [in] Debug info (new in 3.0.0.1) */
			DWORD DebugInfoSize;		/* [in] Size of debug info (new in 3.0.0.1) */
		} NotifySuccess;

		struct {
			LPTSTR ErrorMsg;		/* [in] The error message, or NULL if compilation
									   was aborted by the application. */
			LPTSTR ErrorFilename;	/* [in] Filename in which the error occured. This
									   is NULL if the file is the main script. */
			LONG ErrorLine;			/* [in] The line number the error occured on.
									   Zero if the error doesn't apply to any
									   particular line. */
		} NotifyError;
	};
} TCompilerCallbackData;

typedef LONG(__stdcall* TCompilerCallbackProc)(LONG Code, TCompilerCallbackData* Data, DWORD AppData);

typedef struct {
	DWORD Size;				/* [in] Set to SizeOf(TCompileScriptParamsEx). */
	LPTSTR CompilerPath;	/* [in] The "compiler:" directory. This is the
							   directory which contains the *.e32 files. If this
							   is set to NULL, the compiler will use the directory
							   containing the compiler DLL/EXE. */
	LPTSTR SourcePath;		/* [in] The default source directory, and directory to
							   look in for #include files. Normally, this is
							   the directory containing the script file. This
							   cannot be NULL. */
	TCompilerCallbackProc CallbackProc;
	/* [in] The callback procedure which the compiler calls
	   to read the script and for status notification. */
	DWORD AppData;			/* [in] Application-defined. AppData is passed to the
							   callback function. */
	LPTSTR Options;			/* [in] Additional options. Each option is a
							   null-terminated string, and the final option is
							   followed by an additional null character.
							   If you do not wish to specify any options, set this
							   field to NULL or to point to a single null
							   character.

							   Currently supported options:

							   Output=(0|no|false|1|yes|true)
							      Enables or disables output.
							   OutputBaseFilename=[filename]
							      Overrides any OutputBaseFilename setting in the
								  script; causes the compiler to use [filename]
								  instead.
							   OutputDir=[path]
							      Overrides any output directory in the script;
								  causes the compiler to use [path] instead.
							   SignTool-[name]=[command]
							      Configures a SignTool with name [name] and command
								  [command].
							   ISPP:[isppoption]
							      Configures an ISPP option. */
} TCompileScriptParamsEx;

typedef struct {
	LPCSTR Title;		/* Name of compiler engine - 'Inno Setup' */
	LPCSTR Version;		/* Version number text */
	DWORD BinVersion;	/* Version number as an integer */
} TCompilerVersionInfo, * PCompilerVersionInfo;

/* The ISDllCompileScript function begins compilation of a script. See the above
   description of the TCompileScriptParamsEx struct. Return value is one of the
   isce* constants. */
typedef LONG(__stdcall* ISDllCompileScriptProc)(TCompileScriptParamsEx* Params);

/* The ISDllGetVersion returns a pointer to a TCompilerVersionInfo record which
   contains information about the compiler version. */
typedef PCompilerVersionInfo(__stdcall* ISDllGetVersionProc)();


typedef LONG(__stdcall* ISPreprocessScriptProc)(TCompileScriptParamsEx* Params);
