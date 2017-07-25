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
			for (WORD iter = 0; iter < SESSION_NUM; iter++)
			{
				// empty session 이면
				if (_sessionBuffer[iter]->GetReference() == FALSE)	
				{
					_sessionBuffer[iter]->SetReference();	// session 사용중으로 바꾼다.
					pPlayer = _sessionBuffer[iter];
					break;
				}
			}
		}
		this->_lock.UnLock();

		return pPlayer;
	}

	//SessionData* SessionPool::FindSessionID(WORD wID)
	//{
	//	SessionData* pFindSession = nullptr;

	//	this->_lock.Lock();
	//	{
	//		// SessionId가 일치하는 player가 있다면 반환한다.
	//		auto player = std::find(_playerVec.begin(), _playerVec.end(), [&](SessionData* data) { return data->GetSessionID() == wID; });

	//		if (player != _playerVec.end())
	//		{
	//			pFindSession = *player;
	//		}
	//	}
	//	this->_lock.UnLock();

	//	return pFindSession;
	//}

	SessionData * SessionPool::FindSessionIndex(const int idx)
	{
		if (idx < 0 || idx > SESSION_NUM)
		{
			return nullptr;
		}

		return _sessionBuffer[idx];
	}

	SessionData * SessionPool::FindSessionId(const int sessionId)
	{
		_lock.Lock();
		{
			for (SessionVector::size_type i = 0; i < _playerVec.size(); ++i)
			{
				if (_playerVec[i]->GetSessionID() == sessionId)
				{
					_lock.UnLock();
					return _playerVec[i];
				}
			}
		}
		_lock.UnLock();

		return nullptr;
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