#pragma once

namespace NetworkLib
{
	static BOOL StartNetwork()
	{
		WSADATA	wsaData;
		if ( WSAStartup(MAKEWORD(2, 2), &wsaData) != 0 )
			return FALSE;

		return TRUE;
	}

	static BOOL EndNetwork()
	{
		if ( WSACleanup() == SOCKET_ERROR )
			return FALSE;

		return TRUE;
	}
}