#pragma once

namespace NetworkLib
{
	struct WinSockErrorCodeTable
	{
		int code;
		const WCHAR* str;
	};

	WinSockErrorCodeTable g_ErrTable[] = 
	{
		{ WSAEINTR,					L"Interrupted function call" },
		{ WSAEACCES,				L"Permission denied" },
		{ WSAEFAULT,				L"Bad address" },
		{ WSAEINVAL,				L"Invalid argument" },
		{ WSAEMFILE,				L"Too many open files" },
		{ WSAEWOULDBLOCK,			L"Socket would block" },
		{ WSAEINPROGRESS,			L"Operation now in progress" },
		{ WSAEALREADY,				L"Operation already in progress" },
		{ WSAENOTSOCK,				L"Socket operation on nonsocket" },
		{ WSAEDESTADDRREQ,			L"Destination address required" },
		{ WSAEMSGSIZE,				L"Message too long" },
		{ WSAEPROTOTYPE,			L"Protocol wrong type for socket" },
		{ WSAENOPROTOOPT,			L"Bad protocol option" },
		{ WSAEPROTONOSUPPORT,		L"Protocol not supported" },
		{ WSAESOCKTNOSUPPORT,		L"Socket type not supported" },
		{ WSAEOPNOTSUPP,			L"Operation not supported" },
		{ WSAEPFNOSUPPORT,			L"Protocol family not supported" },
		{ WSAEAFNOSUPPORT,			L"Address family not supported by protocol family" },
		{ WSAEADDRINUSE,			L"Address already in use" },
		{ WSAEADDRNOTAVAIL,			L"Cannot assign requested address" },
		{ WSAENETDOWN,				L"Network is down" },
		{ WSAENETUNREACH,			L"Network is unreachable" },
		{ WSAENETRESET,				L"Network dropped connection on reset" },
		{ WSAECONNABORTED,			L"Software caused connection abort" },
		{ WSAECONNRESET,			L"Connection reset by peer" },
		{ WSAENOBUFS,				L"No buffer space available" },
		{ WSAEISCONN,				L"Socket is already connected" },
		{ WSAENOTCONN,				L"Socket is not connected" },
		{ WSAESHUTDOWN,				L"Cannot send after socket shutdown" },
		{ WSAETIMEDOUT,				L"Connection timed out" },
		{ WSAECONNREFUSED,			L"Connection refused" },
		{ WSAEHOSTDOWN,				L"Host is down" },
		{ WSAEHOSTUNREACH,			L"No route to host" },
		{ WSAEPROCLIM,				L"Too many processes" },
		{ WSASYSNOTREADY,			L"Network subsystem is unavailable" },
		{ WSAVERNOTSUPPORTED,		L"Winsock.dll version out of range" },
		{ WSANOTINITIALISED,		L"Successful WSAStartup not yet performed" },
		{ WSAEDISCON,				L"Graceful shutdown in progress" },
		{ WSATYPE_NOT_FOUND,		L"Class type not found" },
		{ WSAHOST_NOT_FOUND,		L"Host not found" },
		{ WSATRY_AGAIN,				L"Nonauthoritative host not found" },
		{ WSANO_RECOVERY,			L"This is a nonrecoverable error" },
		{ WSANO_DATA,				L"Valid name, no data record of requested type" },
		{ WSA_INVALID_HANDLE,		L"Specified event object handle is invalid" },
		{ WSA_INVALID_PARAMETER,	L"One or more parameters are invalid" },
		{ WSA_IO_INCOMPLETE,		L"Overlapped I/O event object not in signaled state" },
		{ WSA_IO_PENDING,			L"Overlapped operations will complete later" },
		{ WSA_NOT_ENOUGH_MEMORY,	L"Insufficient memory available" },
		{ WSA_OPERATION_ABORTED,	L"Overlapped operation aborted" },
		//	{ WSAINVALIDPROCTABLE,		"Invalid procedure table from service provider" },
		//	{ WSAINVALIDPROVIDER,		"Invalid service provider version number" },
		//	{ WSAPROVIDERFAILEDINIT,	"Unable to initialize a service provider" },
		{ WSASYSCALLFAILURE,		L"System call failure" }
	};

	/*
	 *	Example for use :	const WCHAR* pp = GetStringErrCode(WSAEOPNOTSUPP);
	*/
	const WCHAR* GetStringErrCode(const int nCode)
	{
		for ( int iter = 0; iter < 50; iter++ )
		{
			if ( g_ErrTable[iter].code == nCode )
				return g_ErrTable[iter].str;
		}

		return NULL;
	}
}