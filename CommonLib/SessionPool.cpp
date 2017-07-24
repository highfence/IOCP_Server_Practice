
#include "SessionPool.h"

namespace CommonLib
{
	SessionPool::SessionPool()
	{
		this->_lock.Lock();
		{
			for (WORD iter = 0; iter < SESSION_NUM; iter++)
				_sessionBuffer[iter] = new SessionData(iter);
		}
		this->_lock.UnLock();
	}

	SessionPool::~SessionPool()
	{
		this->_lock.Lock();
		{
			for (WORD iter = 0; iter < SESSION_NUM; iter++)
				SAFE_DELETE_POINT(_sessionBuffer[iter]);

			_mapPlayer.clear();
		}
		this->_lock.UnLock();
	}

	SessionData* SessionPool::CreateSession()
	{
		SessionData* pPlayer = NULL;

		this->_lock.Lock();
		{
			for (int iter = 0; iter < SESSION_NUM; iter++)
			{
				if (TRUE != _sessionBuffer[iter]->GetReference())	// empty session 이면
				{
					//				m_SessionBuffer[iter]->ClearSession();	// 혹시나 모르니까 한 번더 깨끗이 비운다.
					_sessionBuffer[iter]->SetReference();	// session 사용중으로 바꾼다.
					pPlayer = _sessionBuffer[iter];
					break;
				}
			}
		}
		this->_lock.UnLock();

		return pPlayer;
	}

	const SessionData* SessionPool::FindSession(WORD wSession)
	{
		SessionData* pFindSession = NULL;

		this->_lock.Lock();
		{

		}
		this->_lock.UnLock();

		return pFindSession;
	}

	const SessionData* SessionPool::FindSession(LPCTSTR szName)
	{
		SessionData* pFindSession = NULL;

		this->_lock.Lock();
		{

		}
		this->_lock.UnLock();

		return pFindSession;
	}

	const SessionData* SessionPool::FindSessionID(WORD wID)
	{
		SessionData* pFindSession = NULL;

		this->_lock.Lock();
		{

		}
		this->_lock.UnLock();

		return pFindSession;
	}

	const VOID SessionPool::InsertSession(SessionData* pSession)
	{
		this->_lock.Lock();
		{
			_mapPlayer.insert(PLAYERMAP::value_type(pSession->GetSessionID(), pSession));
		}
		this->_lock.UnLock();
	}

	const WORD SessionPool::GetSessionSize()
	{
		WORD	wSessionSize = 0;

		this->_lock.Lock();
		{
			wSessionSize = (WORD)_mapPlayer.size();
		}
		this->_lock.UnLock();

		return wSessionSize;
	}

	const VOID SessionPool::DeleteSession(SessionData* pSession)
	{
		PLAYERMAP::iterator iter;

		this->_lock.Lock();
		{
			for (iter = _mapPlayer.begin(); iter != _mapPlayer.end(); iter++)
			{
				SessionData* pFindSession = iter->second;
				if (pSession)
				{
					if ((pFindSession == pSession) && (pFindSession->GetSessionID() == pSession->GetSessionID()))
					{
						pFindSession->ClearSession();
						_mapPlayer.erase(iter);
						break;
					}
				}
			}
		}
		this->_lock.UnLock();
	}
}