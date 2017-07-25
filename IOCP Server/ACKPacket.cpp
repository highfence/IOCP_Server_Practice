
#include <algorithm>
#include "ServiceManager.h"
#include "../CommonLib/Protocol.h"

using namespace NetworkLib;

const BOOL ServiceManager::recvCS_AUTH_LOGIN_ACK(SessionData* pSession)
{
	ZeroMemory(pSession->_SocketContext.sendContext, MAX_BUFFER);

	SYSTEMTIME st;
	GetSystemTime(&st);

	WCHAR	szTime[MAX_STRING] = _T("");

	wsprintf(szTime, L"%d/%d/%ds/%dms", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	pSession->PostSend((char*)szTime, wcslen(szTime));

	return 1;
}