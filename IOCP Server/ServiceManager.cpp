
#include <WinSock2.h>
#include "ServiceManager.h"
#include "../NetworkLib/InitialNetwork.h"
#include "../NetworkLib/LIB_WINSOCKERRCODE.h"
#include "../CommonLib/SessionData.h"
#include "../CommonLib/Protocol.h"
#include "../CommonLib/Profile.h"

ServiceManager::ServiceManager()
{
	_stopEvent = INVALID_HANDLE_VALUE;
}

ServiceManager::~ServiceManager()
{
	if ( StopServer() != TRUE )
		assert("ERROR : [return FALSE] ServiceMain::~ServiceMain");

	SAFE_CLOSE_HANDLE(_stopEvent);
}

const BOOL ServiceManager::initialAllData()
{
	_log.EventLog(2, L"InitialAllData", L"Start...");

	if (this->initialProfile() != TRUE) {
		_log.EventLog(2, L"InitialAllData", L"Initialize Fail : InitialProfile");
		return FALSE;
	}

	if ( this->initialLog() != TRUE )
	{
		_log.EventLog(2, L"InitialAllData", L"Initialize Fail : Log System");
		return FALSE;
	}

	_log.EventLog(2, L"InitialAllData", L"Initialize OK : Log System");

	if ( this->initialNetwork() != TRUE )
	{
		_log.EventLog(2, L"InitialAllData", L"Initialize Fail : Network");
		return FALSE;
	}

	_log.EventLog(2, L"InitialAllData", L"Initialize OK : Network");

	if ( this->initialThread() != TRUE )
	{
		_log.EventLog(2, L"InitialAllData", L"Initialize Fail : Thread");
		return FALSE;
	}

	_log.EventLog(2, L"InitialAllData", L"Initialize OK : Thread");

	return TRUE;
}

const BOOL ServiceManager::releaseAllData()
{
	_log.EventLog(1, L"Server Program Release");

	if ( this->releaseLog() != TRUE )
		return FALSE;

	return TRUE;
}

const BOOL ServiceManager::initialProfile()
{
	WCHAR	szBuffer[MAX_STRING]	= _T("");
	WCHAR	szDrive[MAX_STRING]		= _T("");
	WCHAR	szDir[MAX_STRING]		= _T("");
	WCHAR	szFileName[MAX_STRING]	= _T("");
	WCHAR	szFileExt[MAX_STRING]	= _T("");
	WCHAR	szIniPath[MAX_STRING]	= _T("");

	::GetModuleFileName(GetModuleHandle(NULL), szBuffer, MAX_STRING);
	::_wsplitpath_s(szBuffer, szDrive, szDir, szFileName, szFileExt);
	wsprintf(szIniPath, L"%s%s%s.ini", szDrive, szDir, szFileName);

	Profile	Profile;
	Profile.SetFilePath(szIniPath);

	/*
	m_ServerInfo.dwWorldID		= Profile.GetDWORDFromINI(L"SERVER", L"WORLD_ID");
	m_ServerInfo.dwServerID		= Profile.GetDWORDFromINI(L"SERVER", L"SERVER_ID");
	m_ServerInfo.dwServerType	= Profile.GetDWORDFromINI(L"SERVER", L"SERVER_TYPE");
	m_ServerInfo.dwTCPPort		= Profile.GetDWORDFromINI(L"SERVER", L"TCP_PORT");
	m_ServerInfo.dwUDPPort		= Profile.GetDWORDFromINI(L"SERVER", L"UDP_PORT");
	*/
	
	_serverInfo.dwWorldID = 1;
	_serverInfo.dwServerID = 10;
	_serverInfo.dwServerType = 100;
	_serverInfo.dwTCPPort = 14400;
	_serverInfo.dwUDPPort = 0;

	/*
	Profile.GetStringFromINI(L"SERVER", L"SERVER_NAME", m_ServerInfo.szServerName);
	Profile.GetStringFromINI(L"SERVER", L"SERVER_IP", m_ServerInfo.szServerIP);
	*/

	wcscpy_s(_serverInfo.szServerName, L"GameServer");
	wcscpy_s(_serverInfo.szServerIP, L"127.0.0.1");

	if ( _serverInfo.dwWorldID == 0 )
		return FALSE;

	return TRUE;
}

const BOOL ServiceManager::initialNetwork()
{
	if ( StartNetwork() != TRUE )
		return FALSE;

	if ( _iocp.StartIOCP() != TRUE )
		return FALSE;

	return TRUE;
}

const BOOL ServiceManager::initialThread()
{
	_acceptThread.Initialize(this, _AcceptThread);
	_acceptThread.StartThread();

	_controlThread.Initialize(this, _ControlThread);
	_controlThread.StartThread();

	_workerThread.Initialize(VALUE_THREAD, this, _WorkerThread);
	_workerThread.StartMultiThread();

	_sendThread.Initialize(this, _SendThread);
	_sendThread.StartThread();

	_logicThread.Initialize(this, _LogicThread);
	_logicThread.StartThread();

	return TRUE;
}

const BOOL ServiceManager::initialLog()
{
	WCHAR	szFileName[MAX_STRING] = _T("");

	SYSTEMTIME st;
	::GetLocalTime(&st);

	wsprintf(szFileName, L"[%s] %0.2d-%0.2d-%0.2d.txt", _serverInfo.szServerName, st.wYear, st.wMonth, st.wDay);

	if ( _log.InitialLogWrite(LOGSYSTEM_HANDLE, szFileName, NAME_TESTSERVER) != TRUE )
		return FALSE;

	return TRUE;
}

const BOOL ServiceManager::releaseLog()
{
	if ( _log.ReleaseLogWrite() != TRUE )
		return FALSE;

	return TRUE;
}

const BOOL ServiceManager::StartServer(LPTSTR* argv)
{
	_log.EventLog(2, L"StartServer", L"Start...");

	if (this->initialAllData() != TRUE) {
		return FALSE;
	}

	_log.EventLog(2, L"StartServer", L"Initialize : Success");


	_stopEvent = CreateEvent(NULL, TRUE, FALSE, L"Server Stop Event");

	if ( WaitForSingleObject(_stopEvent, INFINITE) != WAIT_TIMEOUT )
	{
		SAFE_CLOSE_HANDLE(_stopEvent);
	}
	else
		return FALSE;

	return TRUE;
}

const BOOL ServiceManager::StopServer()
{
	if ( this->_acceptThread.StopThread() != TRUE )
	{
		_log.EventLog(2, L"StopServer", L"Release Fail : AcceptThread");
		return FALSE;
	}

	_log.EventLog(2, L"StopServer", L"Release OK : AcceptThread");

	if ( this->_controlThread.StopThread() != TRUE )
	{
		_log.EventLog(2, L"StopServer", L"Release Fail : ControlThread");
		return FALSE;
	}

	_log.EventLog(2, L"StopServer", L"Release OK : ControlThread");

	if ( this->_workerThread.StopMultiThread() != TRUE )
	{
		_log.EventLog(2, L"StopServer", L"Release Fail : WorkerThread");
		return FALSE;
	}

	_log.EventLog(2, L"StopServer", L"Release OK : WorkerThread");

	if ( EndNetwork() != TRUE )
	{
		_log.EventLog(2, L"StopServer", L"Release Fail : END_NETWORK");
		return FALSE;
	}

	_log.EventLog(2, L"StopServer", L"Release OK : END_NETWORK");

	if (this->releaseAllData() != TRUE)
	{
		return FALSE;
	}

	SetEvent(_stopEvent);

	return TRUE;
}

const UINT ServiceManager::_AcceptThread(LPVOID lpParam)
{
	ServiceManager* pThis = (ServiceManager*)lpParam;
	if (!pThis)
	{
		return FALSE;
	}

	pThis->acceptThread();

	return TRUE;
}

const UINT ServiceManager::_ControlThread(LPVOID lpParam)
{
	ServiceManager* pThis = (ServiceManager*)lpParam;
	if (!pThis)
	{
		return FALSE;
	}

	pThis->controlThread();

	return TRUE;
}

const UINT ServiceManager::_WorkerThread(LPVOID lpParam)
{
	ServiceManager* pThis = (ServiceManager*)lpParam;
	if (!pThis)
	{
		return FALSE;
	}

	pThis->workerThread();

	return TRUE;
}

const UINT ServiceManager::_SendThread(LPVOID lpParam)
{
	ServiceManager* pThis = (ServiceManager*)lpParam;
	if (!pThis)
	{
		return FALSE;
	}

	pThis->sendThread();

	return TRUE;
}

const UINT ServiceManager::_LogicThread(LPVOID lpParam)
{
	ServiceManager* pThis = (ServiceManager*)lpParam;
	if (!pThis)
	{
		return FALSE;
	}

	pThis->logicThread();

	return TRUE;
}

const BOOL ServiceManager::acceptThread()
{
	SOCKET		hServSock = INVALID_SOCKET;
	SOCKET		hClntSock = INVALID_SOCKET;
	SOCKADDR_IN	servAddr;
	SOCKADDR_IN	clntAddr;

	ZeroMemory(&servAddr, sizeof(servAddr));
	ZeroMemory(&clntAddr, sizeof(clntAddr));

	hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	if ( hServSock == INVALID_SOCKET )
		return FALSE;

	servAddr.sin_family			= AF_INET;
	servAddr.sin_port			= htons((u_short)_serverInfo.dwTCPPort);
	servAddr.sin_addr.s_addr	= htonl(INADDR_ANY);

	if ( bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR )
		return FALSE;

	if ( listen(hServSock, SOMAXCONN) == SOCKET_ERROR )
		return FALSE;

	int nSizeClntAddr = sizeof(clntAddr);

	while ( _acceptThread.IsRun() )
	{
		DWORD dwRecvByte = 0;
		DWORD dwFlags = 0;

		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &nSizeClntAddr);

		if ( hClntSock == INVALID_SOCKET )
		{
			if ( WSAEINTR == WSAGetLastError() )
			{
				_log.EventLog(2, L"AcceptThread", GetStringErrCode(WSAEINTR));
				return 0;
			}
		}

		DWORD nonblock = 1;
		if ( ioctlsocket(hClntSock, FIONBIO, &nonblock) == SOCKET_ERROR )
			continue;

		int	nZero = 0;
		if ( SOCKET_ERROR == setsockopt(hClntSock, SOL_SOCKET, SO_RCVBUF, (const char*)&nZero, sizeof(nZero)) )
			continue;

		if ( SOCKET_ERROR == setsockopt(hClntSock, SOL_SOCKET, SO_SNDBUF, (const char*)&nZero, sizeof(nZero)) )
			continue;

		SessionData* pSession = _sessionPool.CreateSession();
		pSession->_SocketContext.clntSocket = hClntSock;
		pSession->SetClientAddr(&clntAddr);

		_sessionPool.InsertSession(pSession);

		if ( _iocp.RegisterCilent(hClntSock, pSession) != TRUE )
		{
//			m_Log.EventLog();
			continue;
		}

		int nRet = WSARecv(hClntSock, &(pSession->_SocketContext.recvContext->wsaBuf), 1, &dwRecvByte, &dwFlags, &(pSession->_SocketContext.recvContext->overlapped), NULL);

		if ( SOCKET_ERROR == nRet )
		{
			int ErrCode = WSAGetLastError();
			if ( ErrCode != WSA_IO_PENDING )
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

const BOOL ServiceManager::controlThread()
{
	WCHAR	szTitle[MAX_STRING] = _T("");
	WCHAR	szWatch[MAX_STRING]	= _T("");
	DWORD	dwHour = 0, dwMinute = 0, dwSecond = 0;

	HANDLE hTimeEvent = WSACreateEvent();

	while ( _controlThread.IsRun() )
	{
		// 999 ms �� 1�ʷ� ��������. �ֳ��ϸ� if �� ���� ����ó�� �ð��� ����ߴ�.
		if ( WaitForSingleObject(hTimeEvent, 999) == WAIT_TIMEOUT )	
		{
			if ( dwSecond >= 60 )
			{
				dwMinute++;
				dwSecond = 0;
			}
			if ( dwMinute >= 60 )
			{
				dwHour++;
				dwMinute = 0;
			}

			wsprintf(szTitle, L"Time(%d/%d/%d) | Player(%d)", dwHour, dwMinute, dwSecond++, _sessionPool.GetSessionSize());
			SetConsoleTitle(szTitle);
		}
	}

	SAFE_CLOSE_HANDLE(hTimeEvent);

	return TRUE;
}

/*
 *	��Ƽ �����带 ����Ѵ�. ���� ��� ������ �����ϸ� �� Lock �� �ɾ�д�
*/
const BOOL ServiceManager::workerThread()
{
	DWORD				dwSizeInOutData = 0;
	DWORD				dwResult = 0;
	SessionData*		pSession = NULL;
	PPerIoContext		pPerIoCtx = NULL;

	while (_workerThread.IsRun())
	{
		dwSizeInOutData = 0;
		dwResult = 0;
		pSession = NULL;
		pPerIoCtx = NULL;

		if (FALSE == GetQueuedCompletionStatus(
				_iocp.GetCompletionPort(),
				&dwSizeInOutData,
				(PULONG_PTR)&pSession,
				(LPOVERLAPPED*)&pPerIoCtx,
				INFINITE))

		dwResult = WSAGetLastError();

		if (dwResult == 0)
		{
			if (pSession == NULL)
				continue;

			// Ŭ���̾�Ʈ�� ������ ������
			if (dwSizeInOutData == 0)	
			{
				static int snCount = 0;
				WCHAR szTemp[MAX_STRING] = _T("");
				wsprintf(szTemp, L"Ŭ���̾�Ʈ close Ƚ�� : %d", ++snCount);
				// ���� 3�� �ڵ��� �����ϼ���. �׽�Ʈ�� ����ſ���.

				_log.EventLog(1, szTemp);
				_sessionPool.DeleteSession(pSession);
				continue;
			}

			if (pPerIoCtx == pSession->_SocketContext.recvContext)
			{
				// ���� ��Ŷ�� �����ŭ �޾Ƽ� ó���� ��
				//packetProcess(pSession);
				_packetLock.Lock();
				{
					_packetProcessQueue.push_back(pSession->GetSessionID());
				}
				_packetLock.UnLock();

				DWORD dwFlags = 0;	// out parameter, and I/O ó���� ������ �� �ش� ���Ͽ� recv �� �ɾ�д�.
				WSARecv(pSession->_SocketContext.clntSocket, &(pSession->_SocketContext.recvContext->wsaBuf), 1, NULL, &dwFlags, &(pSession->_SocketContext.recvContext->overlapped), NULL);
			}
			else if (pPerIoCtx == pSession->_SocketContext.sendContext)
			{
				pSession->CompleteSend();
			}
		}
		else
		{
			if (WAIT_TIMEOUT == dwResult)
			{
			}
			else if (ERROR_NETNAME_DELETED == dwResult)
			{
				_log.EventLog(2, L"WorkerThread", L"ERROR CODE : ERROR_NETNAME_DELETED");
				_sessionPool.DeleteSession(pSession);
			}
			else
			{
				_log.EventLog(2, L"GetQueuedCompletionStatus unknown Error [%d]", dwResult);
				_sessionPool.DeleteSession(pSession);
			}

			continue;
		}
	}

	return TRUE;
}

const BOOL ServiceManager::sendThread()
{
	while (_sendThread.IsRun())
	{
		for (int i = 0; i < SESSION_NUM; ++i)
		{
			auto session = _sessionPool.FindSessionIndex(i);

			if (session == nullptr)
			{
				continue;
			}
			
			session->FlushSend();
		}

		Sleep(10);
	}
	return TRUE;
}

const BOOL ServiceManager::logicThread()
{
	while (_logicThread.IsRun())
	{
		_packetLock.Lock();
		while (!_packetProcessQueue.empty())
		{
			int sessionId = _packetProcessQueue.front();
			packetProcess(sessionId);
			_packetProcessQueue.pop_front();
		}
		_packetLock.UnLock();

		Sleep(10);
	}
	return TRUE;
}

const BOOL ServiceManager::packetProcess(const int sessionId)
{
	BOOL bReturn = TRUE;
	auto pSession = _sessionPool.FindSessionId(sessionId);

	if (pSession == nullptr)
	{
		return FALSE;
	}

	header_special Header;
	memcpy(&Header, pSession->_SocketContext.recvContext->Buffer, SIZE_HEADER);

	switch (Header.command)
	{
	case CS_AUTH_LOGIN_REQ:	this->recvCS_AUTH_LOGIN_REQ(pSession);		break;
	default:	bReturn = FALSE;							break;
	}

	return bReturn;
}