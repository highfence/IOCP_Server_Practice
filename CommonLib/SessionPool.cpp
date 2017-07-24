#include <algorithm>
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

			_playerVec.clear();
		}
		this->_lock.UnLock();
	}

	SessionData* SessionPool::CreateSession()
	{
		SessionData* pPlayer = nullptr;

		this->_lock.Lock();
		{
			// ERROR :: iter가 명확하지 않아 SessionData의 포인터로 받음. 확인 필요.
			for (SessionData* iter = 0; reinterpret_cast<int>(iter) < SESSION_NUM; iter++)
			{
				int idx = reinterpret_cast<int>(iter);

				// empty session 이면
				if (_sessionBuffer[idx]->GetReference() == FALSE)	
				{
					_sessionBuffer[idx]->SetReference();	// session 사용중으로 바꾼다.
					pPlayer = _sessionBuffer[idx];
					break;
				}
			}
		}
		this->_lock.UnLock();

		return pPlayer;
	}

	// TODO :: wSession은 어떤 기준이지?
	const SessionData* SessionPool::FindSession(WORD wSession)
	{
		SessionData* pFindSession = nullptr;

		this->_lock.Lock();
		{

		}
		this->_lock.UnLock();

		return pFindSession;
	}

	// TODO :: szName은 어떤 기준으로 찾은거지?
	const SessionData* SessionPool::FindSession(LPCTSTR szName)
	{
		SessionData* pFindSession = nullptr;

		this->_lock.Lock();
		{
			
		}
		this->_lock.UnLock();

		return pFindSession;
	}

	const SessionData* SessionPool::FindSessionID(WORD wID)
	{
		SessionData* pFindSession = nullptr;

		this->_lock.Lock();
		{
			// SessionId가 일치하는 player가 있다면 반환한다.
			auto player = std::find(_playerVec.begin(), _playerVec.end(), [&](SessionData* data) { return data->GetSessionID() == wID; });

			if (player != _playerVec.end())
			{
				pFindSession = *player;
			}
		}
		this->_lock.UnLock();

		return pFindSession;
	}

	const VOID SessionPool::InsertSession(SessionData* pSession)
	{
		if (pSession == nullptr)
		{
			return;
		}

		this->_lock.Lock();
		{
			_playerVec.emplace_back(std::move(pSession));
		}
		this->_lock.UnLock();
	}

	const WORD SessionPool::GetSessionSize()
	{
		WORD	wSessionSize = 0;

		this->_lock.Lock();
		{
			wSessionSize = (WORD)_playerVec.size();
		}
		this->_lock.UnLock();

		return wSessionSize;
	}

	const VOID SessionPool::DeleteSession(SessionData* pSession)
	{
		if (pSession == nullptr)
		{
			return;
		}

		this->_lock.Lock();
		{
			for (SessionVector::size_type i = 0; i < _playerVec.size();)
			{
				if ((_playerVec[i] == pSession) && (_playerVec[i]->GetSessionID() == pSession->GetSessionID()))
				{
					_playerVec.erase(_playerVec.begin() + i);
				}
				else
				{
					++i;
				}
			}
		}
		this->_lock.UnLock();
	}
}