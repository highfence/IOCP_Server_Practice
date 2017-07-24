
#pragma once


#include "Common.h"

namespace COMMONLIB
{
	/*
	 *	Make a person	: �����
	 *	Make a date		: 2006.10.07
	 *	Class name		: LIB_PROFILE
	 *	Class describe	: INI ���Ϸκ��� �ؽ�Ʈ�� READ �ϰų� WRITE �� �����ϰ��ϴ� Ŭ����
	*/

	class LIB_PROFILE
	{
	public :

		LIB_PROFILE();
		~LIB_PROFILE();

	private :

		TCHAR	m_FilePath[MAX_STRING];

	public :

		const VOID	SetFilePath(LPCWSTR strPath);
		const VOID	WriteToINI(LPCWSTR strSection, LPCWSTR strKey, int nValue);
		const VOID	WriteToINI(LPCWSTR strSection, LPCWSTR strKey, LPCWSTR strValue);
		const VOID	GetStringFromINI(LPCWSTR strSection, LPCWSTR strKey, LPWSTR strResult);
		const DWORD	GetDWORDFromINI(LPCWSTR strSection, LPCWSTR strKey);
		const BOOL	DeleteINI();
	};
}
