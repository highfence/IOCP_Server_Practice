
#include "SessionData.h"

namespace CommonLib
{

	SessionData::SessionData(WORD wSessionID)
	{
		_sessionId = wSessionID;
		_isReference = FALSE;

		m_SocketCtx.recvContext = new PerIoContext;
		m_SocketCtx.sendContext = new PerIoContext;

		m_SocketCtx.clntSocket = INVALID_SOCKET;
		ClearSocketContext();
	}

	SessionData::~SessionData()
	{
		SAFE_DELETE_POINT(m_SocketCtx.recvContext);
		SAFE_DELETE_POINT(m_SocketCtx.sendContext);
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
		ZeroMemory(&m_SocketCtx.clntAddrInfo, sizeof(SOCKADDR_IN));

		m_SocketCtx.clntAddrInfo.sin_family = pClntAddr->sin_family;
		m_SocketCtx.clntAddrInfo.sin_port = pClntAddr->sin_port;
		m_SocketCtx.clntAddrInfo.sin_addr.s_addr = pClntAddr->sin_addr.s_addr;
	}

	const VOID SessionData::CreateCryptKey()
	{
		DWORD	dwTickCount = GetTickCount();
		srand(dwTickCount);
		wCryptKey = rand() & 255;
	}

	const VOID SessionData::ClearSocketContext()
	{
		ZeroMemory(&m_SocketCtx.clntAddrInfo, sizeof(SOCKADDR_IN));

		ZeroMemory(&m_SocketCtx.recvContext->Buffer, MAX_BUFFER);
		ZeroMemory(&m_SocketCtx.recvContext->wsaBuf, sizeof(WSABUF));
		ZeroMemory(&m_SocketCtx.recvContext->overlapped, sizeof(WSAOVERLAPPED));

		ZeroMemory(&m_SocketCtx.sendContext->Buffer, MAX_BUFFER);
		ZeroMemory(&m_SocketCtx.sendContext->wsaBuf, sizeof(WSABUF));
		ZeroMemory(&m_SocketCtx.sendContext->overlapped, sizeof(WSAOVERLAPPED));

		SAFE_CLOSE_SOCKET(m_SocketCtx.clntSocket);

		m_SocketCtx.recvContext->wsaBuf.buf = m_SocketCtx.recvContext->Buffer;
		m_SocketCtx.recvContext->wsaBuf.len = MAX_BUFFER;

		m_SocketCtx.sendContext->wsaBuf.buf = m_SocketCtx.sendContext->Buffer;
		m_SocketCtx.sendContext->wsaBuf.len = MAX_BUFFER;
	}
}