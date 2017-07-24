
#pragma once


#include "Common.h"
#include "LIB_SESSIONDATA.h"
#include "Lock.h"

namespace COMMONLIB
{
	// ���ø����� ������ ��
	class LIB_SESSIONPOOL
	{
	public :
		LIB_SESSIONPOOL();
		~LIB_SESSIONPOOL();
	private :
		CriticalLock	m_Lock;
		LIB_SESSIONDATA*	m_SessionBuffer[SESSION_NUM];
		PLAYERMAP		m_mapPlayer;
	public :
		LIB_SESSIONDATA*	CreateSession();
		const LIB_SESSIONDATA*	FindSession(WORD wSession);
		const LIB_SESSIONDATA*	FindSession(LPCTSTR szName);
		const LIB_SESSIONDATA*	FindSessionID(WORD wID);
		const VOID			InsertSession(LIB_SESSIONDATA* pSession);
		const WORD			GetSessionSize();
		const VOID			DeleteSession(LIB_SESSIONDATA* pSession);
		// GETSESSIONID
	};
}
