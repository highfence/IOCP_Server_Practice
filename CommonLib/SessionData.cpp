#include "SessionData.h"

namespace CommonLib
{
	SessionData::SessionData(WORD wSessionID)
	{
		_sessionId = wSessionID;
		_isReference = FALSE;
		_isSending = FALSE;

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

	const BOOL SessionData::PostSend(const char * data, const int dataSize)
	{
		_sendLock.Lock();
		{
			_sendQueue.Push(data, dataSize);
		}
		_sendLock.UnLock();

		return true;
	}

	const BOOL SessionData::FlushSend()
	{
		_sendLock.Lock();
		{
			if (_SocketContext.clntSocket == INVALID_SOCKET)
			{
				_sendLock.UnLock();
				return false;
			}

			if (_isSending == TRUE)
			{
				_sendLock.UnLock();
				return false;
			}

			auto sendData = _sendQueue.GetDataSize();

			if (sendData < 0)
			{
				_sendLock.UnLock();
				return false;
			}

			_isSending = TRUE;
			_sendQueue.Pop(_SocketContext.sendContext->Buffer, sendData);

			_SocketContext.sendContext->wsaBuf.buf = _SocketContext.sendContext->Buffer;
			_SocketContext.sendContext->wsaBuf.len = sendData;

			DWORD sendbytes = 0;
			DWORD flags = 0;

			int returnValue = WSASend(
				_SocketContext.clntSocket,
				&_SocketContext.sendContext->wsaBuf,
				1,
				&sendbytes,
				flags,
				&_SocketContext.sendContext->overlapped,
				NULL);

			if (returnValue == SOCKET_ERROR)
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					// 에러 처리.
				}
			}
		}
		_sendLock.UnLock();
		return true;
	}

	const BOOL SessionData::CompleteSend()
	{
		_sendLock.Lock();
		{
			if (_SocketContext.clntSocket == INVALID_SOCKET)
			{
				_sendLock.UnLock();
				return false;
			}

			_isSending = false;
		}
		_sendLock.UnLock();
		return true;
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