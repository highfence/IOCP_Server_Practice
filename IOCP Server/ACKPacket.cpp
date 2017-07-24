
#include "ServiceManager.h"
#include "../CommonLib/Protocol.h"

using namespace NetworkLib;

const BOOL ServiceManager::recvCS_AUTH_LOGIN_ACK(SessionData* pSession)
{
	ZeroMemory(pSession->m_SocketCtx.sendContext, MAX_BUFFER);

	SYSTEMTIME st;
	GetSystemTime(&st);

	WCHAR	szTime[MAX_STRING] = _T("");

	wsprintf(szTime, L"%d/%d/%ds/%dms", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	memcpy(&pSession->m_SocketCtx.sendContext->Buffer, szTime, wcslen(szTime));

	_lock.Lock();
	{
		_sendCtx = pSession;
		SetEvent(_sendEvent);
	}
	_lock.UnLock();

	return 1;
}