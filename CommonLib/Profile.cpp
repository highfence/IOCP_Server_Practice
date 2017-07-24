#include "Profile.h"

namespace CommonLib
{
	Profile::Profile()
	{
		ZeroMemory(_filePath, MAX_STRING);
	}

	Profile::~Profile()
	{
	}

	const VOID Profile::SetFilePath(LPCWSTR strPath)
	{
		wcscpy(_filePath, strPath);
	}

	const VOID Profile::WriteToINI(LPCWSTR strSection, LPCWSTR strKey, int nValue)
	{
		TCHAR	strValue[MAX_STRING] = L"";
		wsprintf(strValue, L"%d", nValue);
		WritePrivateProfileString(strSection, strKey, strValue, _filePath);
	}

	const VOID Profile::WriteToINI(LPCWSTR strSection, LPCWSTR strKey, LPCWSTR strValue)
	{
		WritePrivateProfileString(strSection, strKey, strValue, _filePath);
	}

	const VOID Profile::GetStringFromINI(LPCWSTR strSection, LPCWSTR strKey, LPWSTR strResult)
	{
		GetPrivateProfileString(strSection, strKey, L"", strResult, MAX_STRING, _filePath);
	}

	const DWORD Profile::GetDWORDFromINI(LPCWSTR strSection, LPCWSTR strKey)
	{
		return GetPrivateProfileInt(strSection, strKey, 0, _filePath);
	}

	const BOOL Profile::DeleteINI()
	{
		return ::DeleteFile(_filePath);
	}
}