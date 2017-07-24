
#include "LogSystem.h"

namespace CommonLib
{
	LogSystem::LogSystem()
	{
		_logSystem = 0;
		_file = NULL;
		_fileHandle = INVALID_HANDLE_VALUE;
		ZeroMemory(&_eventLogName, MAX_STRING);
	}

	LogSystem::~LogSystem()
	{	// ��� ���� Release�� �̹� ReleaseLog ���� ���ְ� �ִ�
	}

	const BOOL LogSystem::InitialLogWrite(const BYTE bLogSystem, const LPCTSTR szFileName, const LPCTSTR szEventLogName)
	{
		BOOL	bReturn = FALSE;
		WCHAR	szCreateFile[MAX_STRING] = _T("");

		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY) != TRUE)
			return bReturn;

		auto errorLogDirPath = _T(STR_FOLDER_ERR);
		CreateDirectory(errorLogDirPath, NULL);
		wsprintf(szCreateFile, L"%s\\%s", errorLogDirPath, szFileName);

		_Lock.Lock();
		{
			if (wcslen(szEventLogName) <= 1)
				return bReturn;
			else
				wcscpy(_eventLogName, szEventLogName);

			_logSystem = bLogSystem;

			switch (_logSystem)
			{
			case LOGSYSTEM_FILE:	bReturn = initialLogFile(szCreateFile);		break;
			case LOGSYSTEM_HANDLE:	bReturn = initialLogHandle(szCreateFile);	break;
			default:	break;
			}
		}
		_Lock.UnLock();

		return bReturn;
	}

	const BOOL LogSystem::initialLogFile(const LPCWSTR szFileName)
	{
		_Lock.Lock();
		{
			// WARNING :: char*�� ������� ĳ������.
			_file = fopen((char*)szFileName, "a");

			if (_file == NULL)
			{
				SAFE_CLOSE_FILE(_file);
				return FALSE;
			}
		}
		_Lock.UnLock();

		return TRUE;
	}

	const BOOL LogSystem::initialLogHandle(const LPCTSTR szFileName)
	{
		_Lock.Lock();
		{
			_fileHandle = CreateFile(szFileName, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			SetFilePointer(_fileHandle, 0, NULL, FILE_END);

			if (_fileHandle == INVALID_HANDLE_VALUE)
			{
				SAFE_CLOSE_HANDLE(_fileHandle);
				return FALSE;
			}
		}
		_Lock.UnLock();

		return TRUE;
	}

	const BOOL LogSystem::releaseLogFile()
	{
		_Lock.Lock();
		{
			if (_file != NULL)
			{
				SAFE_DELETE_POINT(_file);
			}
		}
		_Lock.UnLock();

		return TRUE;
	}

	const BOOL LogSystem::releaseLogHandle()
	{
		_Lock.Lock();
		{
			if (_fileHandle != INVALID_HANDLE_VALUE)
			{
				SAFE_CLOSE_HANDLE(_fileHandle);
			}
		}
		_Lock.UnLock();

		return TRUE;
	}

	const BOOL LogSystem::ReleaseLogWrite()
	{
		BOOL bReturn = 0;

		switch (_logSystem)
		{
		case LOGSYSTEM_FILE:	bReturn = releaseLogFile();		break;
		case LOGSYSTEM_HANDLE:	bReturn = releaseLogHandle();	break;
		default:	break;
		}

		return bReturn;
	}

	const BOOL LogSystem::writeLogFile(const LPCWSTR szWrite)
	{
		_Lock.Lock();
		{
			fwrite(szWrite, sizeof(TCHAR), wcslen(szWrite), _file);
			fflush(_file);	// fclose �ϱ����� ���ۿ� �ִ� ������ write �Ѵ�. ������ �� �ڵ带 �����ϴ� ������� ��� ����� �Ѥ�^ ���� ������.
		}
		_Lock.UnLock();

		return TRUE;
	}

	const BOOL LogSystem::writeLogHandle(const LPCWSTR szWrite)
	{
		DWORD dwWritten = 0;

		if (WriteFile(_fileHandle, szWrite, (DWORD)wcslen(szWrite), &dwWritten, NULL) != TRUE)
		{
			return FALSE;
		}

		return TRUE;
	}

	const BOOL LogSystem::writeLog(const LPCTSTR szWrite)
	{
		BOOL bReturn = FALSE;

		TCHAR	szFileWrite[MAX_STRING] = _T("");

		_Lock.Lock();
		{
			switch (_logSystem)
			{
			case LOGSYSTEM_FILE:	wsprintf(szFileWrite, L"%s\n", szWrite);
				bReturn = writeLogFile(szFileWrite);	break;
			case LOGSYSTEM_HANDLE:	wsprintf(szFileWrite, L"%s\r\n", szWrite);
				bReturn = writeLogHandle(szFileWrite);	break;
			}
		}
		_Lock.UnLock();

		return bReturn;
	}

	const VOID LogSystem::EventLog(int nLoop, LPCWSTR pFormat, ...)
	{
		WCHAR szDate[MAX_PATH] = _T("");
		WCHAR szError[MAX_PATH] = _T("");

		LPWSTR  lpszStrings[1];
		lpszStrings[0] = szError;

		SYSTEMTIME st;
		::GetLocalTime(&st);
		wsprintf(szDate, L"[%0.2d.%0.2d %0.2d:%0.2d:%0.2d] ", st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

		va_list pArg;
		va_start(pArg, pFormat);
		_vstprintf(szError, pFormat, pArg);
		wcscat(szDate, szError);

		if (nLoop >= 2)
		{
			for (int iter = 0; iter < nLoop - 1; iter++)
			{
				LPCTSTR lpText = va_arg(pArg, LPCTSTR);
				wcscat(szDate, L" / ");
				wcscat(szDate, lpText);
			}
		}

		va_end(pArg);

#ifdef _DEBUG			// ����� ��� : �ܼ� ȭ�鿡 ���, ���� �α� �ۼ�
		_putws(szDate);
		writeLog(szDate);
#else if _NDEBUG		// ������ ��� : �̺�Ʈ �α� �ۼ�, ���� �α� �ۼ�
		HANDLE hEventSource = RegisterEventSource(NULL, m_szEventLogName);
		if (hEventSource)
		{
			ReportEvent(hEventSource, EVENTLOG_WARNING_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*)&lpszStrings[0], NULL);
			DeregisterEventSource(hEventSource);
		}
		WriteLog(szDate);
#endif	// _DEBUG & _NDEBUG
	}
}