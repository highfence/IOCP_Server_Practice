
#pragma once


#include "Common.h"
#include "SessionData.h"
#include "Lock.h"

namespace CommonLib
{
	// 템플릿으로 수정할 것
	class LIB_SESSIONPOOL
	{
	public :
		LIB_SESSIONPOOL();
		~LIB_SESSIONPOOL();
	private :
		CriticalLock	m_Lock;
		SessionData*	m_SessionBuffer[SESSION_NUM];
		PLAYERMAP		m_mapPlayer;
	public :
		SessionData*	CreateSession();
		const SessionData*	FindSession(WORD wSession);
		const SessionData*	FindSession(LPCTSTR szName);
		const SessionData*	FindSessionID(WORD wID);
		const VOID			InsertSession(SessionData* pSession);
		const WORD			GetSessionSize();
		const VOID			DeleteSession(SessionData* pSession);
		// GETSESSIONID
	};
}
