#pragma once

#include "Common.h"
#include "Lock.h"

namespace CommonLib
{
	enum ENUM_LOGSYSTEM
	{
		LOGSYSTEM_FILE		= 1,
		LOGSYSTEM_HANDLE	= 2,
	};

	/*
     *	Make a person	: 이재득
	 *	Make a date		: 2007.02.09
	 *	Class name		: LIB_LOGSYSTEM
	 *	Class describe	: 파일에 로그를 남긴다.
	*/

	class LogSystem
	{
	public :

		LogSystem();
		~LogSystem();

	public :

		const BOOL	InitialLogWrite(const BYTE bLogSystem, const LPCTSTR szFileName, const LPCTSTR szEventLogName);
		const BOOL	ReleaseLogWrite();
		const VOID	EventLog(int nLoop, LPCWSTR pFormat, ...);

	private :

		CriticalLock	_Lock;
		FILE*			_file;
		HANDLE			_fileHandle;
		BYTE			_logSystem;
		TCHAR			_eventLogName[MAX_STRING];

	private :

		const BOOL	initialLogFile(const LPCWSTR szFileName);
		const BOOL	initialLogHandle(const LPCTSTR szFileName);
		const BOOL	releaseLogFile();
		const BOOL	releaseLogHandle();
		const BOOL	writeLogFile(const LPCWSTR szWrite);
		const BOOL	writeLogHandle(const LPCWSTR szWrite);
		const BOOL	writeLog(const LPCTSTR szWrite);

	};
}
