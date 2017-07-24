#include "LIB_PROFILE.h"

namespace COMMONLIB
{
	LIB_PROFILE::LIB_PROFILE()
	{
		ZeroMemory(m_FilePath, MAX_STRING);
	}

	LIB_PROFILE::~LIB_PROFILE()
	{
	}

	const VOID LIB_PROFILE::SetFilePath(LPCWSTR strPath)
	{
		wcscpy(m_FilePath, strPath);
	}

	const VOID LIB_PROFILE::WriteToINI(LPCWSTR strSection, LPCWSTR strKey, int nValue)
	{
		TCHAR	strValue[MAX_STRING] = L"";
		wsprintf(strValue, L"%d", nValue);
		WritePrivateProfileString(strSection, strKey, strValue, m_FilePath);
	}

	const VOID LIB_PROFILE::WriteToINI(LPCWSTR strSection, LPCWSTR strKey, LPCWSTR strValue)
	{
		WritePrivateProfileString(strSection, strKey, strValue, m_FilePath);
	}

	const VOID LIB_PROFILE::GetStringFromINI(LPCWSTR strSection, LPCWSTR strKey, LPWSTR strResult)
	{
		GetPrivateProfileString(strSection, strKey, L"", strResult, MAX_STRING, m_FilePath);
	}

	const DWORD LIB_PROFILE::GetDWORDFromINI(LPCWSTR strSection, LPCWSTR strKey)
	{
		return GetPrivateProfileInt(strSection, strKey, 0, m_FilePath);
	}

	const BOOL LIB_PROFILE::DeleteINI()
	{
		return ::DeleteFile(m_FilePath);
	}
}