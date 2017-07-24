#pragma once

#include "Common.h"
#include "SessionData.h"
#include "Lock.h"

namespace CommonLib
{
	// ���ø����� ������ ��
	class SessionPool
	{
	public :

		SessionPool();
		~SessionPool();

	public :

		SessionData*	CreateSession();
		const SessionData*	FindSession(WORD wSession);
		const SessionData*	FindSession(LPCTSTR szName);
		const SessionData*	FindSessionID(WORD wID);
		const VOID			InsertSession(SessionData* pSession);
		const WORD			GetSessionSize();
		const VOID			DeleteSession(SessionData* pSession);
		// GETSESSIONID

	private :

		CriticalLock	_lock;
		SessionData*	_sessionBuffer[SESSION_NUM];
		PLAYERMAP		_mapPlayer;
	};
}
