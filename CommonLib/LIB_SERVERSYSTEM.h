#pragma once

#include "Common.h"

namespace COMMONLIB
{
	const wchar_t NAME_TESTSERVER[MAX_STRING]	= _T("TestServer");

	class LIB_SERVERINFO
	{
	public :
		LIB_SERVERINFO()
		{
			dwServerID		= 0;
			dwWorldID		= 0;
			dwSession		= 0;
			dwTCPPort		= 0;
			dwUDPPort		= 0;
			dwServerType	= 0;
            ZeroMemory(&szServerIP, MIN_STRING);
			ZeroMemory(&szServerName, MIN_STRING);
		}
		~LIB_SERVERINFO()	{}
	public :
		DWORD	dwServerID;
		DWORD	dwWorldID;
		DWORD	dwSession;
		DWORD	dwTCPPort;
		DWORD	dwUDPPort;
		DWORD	dwServerType;
		wchar_t szServerIP[MIN_STRING];
		wchar_t szServerName[MIN_STRING];
	};
}
