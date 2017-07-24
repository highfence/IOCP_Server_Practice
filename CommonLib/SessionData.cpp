
#include "SessionData.h"

namespace CommonLib
{

	SessionData::SessionData(WORD wSessionID)
	{
		_sessionId = wSessionID;
		_isReference = FALSE;

		_SocketContext.recvContext = new PerIoContext;
		_SocketContext.sendContext = new PerIoContext;

		_SocketContext.clntSocket = INVALID_SOCKET;
		ClearSocketContext();
	}

	SessionData::~SessionData()
	{
		SAFE_DELETE_POINT(_SocketContext.recvContext);
		SAFE_DELETE_POINT(_SocketContext.sendContext);
	}

	const WORD SessionData::GetSessionID()
	{
		return _sessionId;
	}

	const VOID SessionData::SetReference()
	{
		_isReference = TRUE;
	}

	const BOOL SessionData::GetReference()
	{
		return _isReference;
	}

	const VOID SessionData::ClearSession()
	{
		ClearSocketContext();

		_isReference = FALSE;
	}

	const VOID SessionData::SetClientAddr(SOCKADDR_IN* pClntAddr)
	{
		ZeroMemory(&_SocketContext.clntAddrInfo, sizeof(SOCKADDR_IN));

		_SocketContext.clntAddrInfo.sin_family = pClntAddr->sin_family;
		_SocketContext.clntAddrInfo.sin_port = pClntAddr->sin_port;
		_SocketContext.clntAddrInfo.sin_addr.s_addr = pClntAddr->sin_addr.s_addr;
	}

	const VOID SessionData::CreateCryptKey()
	{
		DWORD	dwTickCount = GetTickCount();
		srand(dwTickCount);
		wCryptKey = rand() & 255;
	}

	const VOID SessionData::ClearSocketContext()
	{
		ZeroMemory(&_SocketContext.clntAddrInfo, sizeof(SOCKADDR_IN));

		ZeroMemory(&_SocketContext.recvContext->Buffer, MAX_BUFFER);
		ZeroMemory(&_SocketContext.recvContext->wsaBuf, sizeof(WSABUF));
		ZeroMemory(&_SocketContext.recvContext->overlapped, sizeof(WSAOVERLAPPED));

		ZeroMemory(&_SocketContext.sendContext->Buffer, MAX_BUFFER);
		ZeroMemory(&_SocketContext.sendContext->wsaBuf, sizeof(WSABUF));
		ZeroMemory(&_SocketContext.sendContext->overlapped, sizeof(WSAOVERLAPPED));

		SAFE_CLOSE_SOCKET(_SocketContext.clntSocket);

		_SocketContext.recvContext->wsaBuf.buf = _SocketContext.recvContext->Buffer;
		_SocketContext.recvContext->wsaBuf.len = MAX_BUFFER;

		_SocketContext.sendContext->wsaBuf.buf = _SocketContext.sendContext->Buffer;
		_SocketContext.sendContext->wsaBuf.len = MAX_BUFFER;
	}
}