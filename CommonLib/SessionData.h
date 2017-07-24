#pragma once

#include "Common.h"

namespace CommonLib
{
	typedef struct tagPerIoContext
	{
		WSAOVERLAPPED	overlapped;
		char			Buffer[MAX_BUFFER];
		WSABUF			wsaBuf;
	}	PerIoContext, *PPerIoContext;

	typedef struct tagPerSocketContext
	{
		SOCKET			clntSocket;
		SOCKADDR_IN		clntAddrInfo;
		PPerIoContext	recvContext;
		PPerIoContext	sendContext;
	}	PerSocketContext, *PPerSocketContext;

	class SessionData
	{
	public :

		SessionData(WORD wSessionID);
		~SessionData();

	public :

		PerSocketContext	m_SocketCtx;
		BYTE				wCryptKey;

	public :

		const	WORD	GetSessionID();
		const	BOOL	GetReference();
		const	VOID	SetReference();
		const	VOID	SetClientAddr(SOCKADDR_IN* pClntAddr);
		const	VOID	ClearSession();
		const	VOID	ClearSocketContext();
		const	VOID	CreateCryptKey();

	private :

		WORD				_sessionId;
		BOOL				_isReference;
	};

	typedef std::map<DWORD, SessionData*> PLAYERMAP;
}
