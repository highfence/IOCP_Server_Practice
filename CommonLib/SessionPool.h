#pragma once

#include <vector>
#include "Common.h"
#include "SessionData.h"
#include "Lock.h"

namespace CommonLib
{
	using SessionId = DWORD;
	using SessionVector = std::vector<SessionData*>;

	// 템플릿으로 수정할 것
	class SessionPool
	{
	public :

		SessionPool();
		~SessionPool();

	public :

		SessionData*		CreateSession();
		SessionData*		FindSessionIndex(const int idx);
		SessionData*		FindSessionId(const int sessionId);
		const VOID			InsertSession(SessionData* pSession);
		const WORD			GetSessionSize();
		const VOID			DeleteSession(SessionData* pSession);
		// GETSESSIONID

	private :

		CriticalLock	_lock;
		SessionData*	_sessionBuffer[SESSION_NUM];
		SessionVector	_playerVec;
	};
}
