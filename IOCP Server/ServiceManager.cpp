
#include <WinSock2.h>
#include "ServiceManager.h"
#include "../NetworkLib/InitialNetwork.h"
#include "../NetworkLib/LIB_WINSOCKERRCODE.h"
#include "../CommonLib/SessionData.h"
#include "../CommonLib/Protocol.h"
#include "../CommonLib/Profile.h"

ServiceManager::ServiceManager()
{
	m_hSendEvent = INVALID_HANDLE_VALUE;
	m_hStopEvent = INVALID_HANDLE_VALUE;
}

ServiceManager::~ServiceManager()
{
	if ( StopServer() != TRUE )
		assert("ERROR : [return FALSE] ServiceMain::~ServiceMain");

	SAFE_CLOSE_HANDLE(m_hSendEvent);
	SAFE_CLOSE_HANDLE(m_hStopEvent);
}

const BOOL ServiceManager::InitialAllData()
{
	m_Log.EventLog(2, L"InitialAllData", L"Start...");

	if (this->InitialProfile() != TRUE) {
		m_Log.EventLog(2, L"InitialAllData", L"Initialize Fail : InitialProfile");
		return FALSE;
	}

	if ( this->InitialLog() != TRUE )
	{
		m_Log.EventLog(2, L"InitialAllData", L"Initialize Fail : Log System");
		return FALSE;
	}

	m_Log.EventLog(2, L"InitialAllData", L"Initialize OK : Log System");

	if ( this->InitialNetwork() != TRUE )
	{
		m_Log.EventLog(2, L"InitialAllData", L"Initialize Fail : Network");
		return FALSE;
	}

	m_Log.EventLog(2, L"InitialAllData", L"Initialize OK : Network");

	if ( this->InitialThread() != TRUE )
	{
		m_Log.EventLog(2, L"InitialAllData", L"Initialize Fail : Thread");
		return FALSE;
	}

	m_Log.EventLog(2, L"InitialAllData", L"Initialize OK : Thread");

	return TRUE;
}

const BOOL ServiceManager::ReleaseAllData()
{
	m_Log.EventLog(1, L"Server Program Release");

	if ( this->ReleaseLog() != TRUE )
		return FALSE;

	return TRUE;
}

const BOOL ServiceManager::InitialProfile()
{
	WCHAR	szBuffer[MAX_STRING]	= _T("");
	WCHAR	szDrive[MAX_STRING]		= _T("");
	WCHAR	szDir[MAX_STRING]		= _T("");
	WCHAR	szFileName[MAX_STRING]	= _T("");
	WCHAR	szFileExt[MAX_STRING]	= _T("");
	WCHAR	szIniPath[MAX_STRING]	= _T("");

	::GetModuleFileName(GetModuleHandle(NULL), szBuffer, MAX_STRING);
	::_wsplitpath(szBuffer, szDrive, szDir, szFileName, szFileExt);
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
	
	m_ServerInfo.dwWorldID = 1;
	m_ServerInfo.dwServerID = 10;
	m_ServerInfo.dwServerType = 100;
	m_ServerInfo.dwTCPPort = 14400;
	m_ServerInfo.dwUDPPort = 0;

	/*
	Profile.GetStringFromINI(L"SERVER", L"SERVER_NAME", m_ServerInfo.szServerName);
	Profile.GetStringFromINI(L"SERVER", L"SERVER_IP", m_ServerInfo.szServerIP);
	*/

	wcscpy(m_ServerInfo.szServerName, L"GameServer");
	wcscpy(m_ServerInfo.szServerIP, L"127.0.0.1");

	if ( m_ServerInfo.dwWorldID == 0 )
		return FALSE;

	return TRUE;
}

const BOOL ServiceManager::InitialNetwork()
{
	if ( StartNetwork() != TRUE )
		return FALSE;

	if ( m_IOCP.StartIOCP() != TRUE )
		return FALSE;

	return TRUE;
}

const BOOL ServiceManager::InitialThread()
{
	m_AcceptThread.Initialize(this, _AcceptThread);
	m_AcceptThread.StartThread();

	m_ControlThread.Initialize(this, _ControlThread);
	m_ControlThread.StartThread();

	m_WorkerThread.Initialize(VALUE_THREAD, this, _WorkerThread);
	m_WorkerThread.StartMultiThread();

	m_SendThread.Initialize(this, _SendThread);
	m_SendThread.StartThread();

	return TRUE;
}

const BOOL ServiceManager::InitialLog()
{
	WCHAR	szFileName[MAX_STRING] = _T("");

	SYSTEMTIME st;
	::GetLocalTime(&st);

	wsprintf(szFileName, L"[%s] %0.2d-%0.2d-%0.2d.txt", m_ServerInfo.szServerName, st.wYear, st.wMonth, st.wDay);

	if ( m_Log.InitialLogWrite(LOGSYSTEM_HANDLE, szFileName, NAME_TESTSERVER) != TRUE )
		return FALSE;

	return TRUE;
}

const BOOL ServiceManager::ReleaseLog()
{
	if ( m_Log.ReleaseLogWrite() != TRUE )
		return FALSE;

	return TRUE;
}

const BOOL ServiceManager::StartServer(LPTSTR* argv)
{
	m_Log.EventLog(2, L"StartServer", L"Start...");

	if (this->InitialAllData() != TRUE) {
		return FALSE;
	}

	m_Log.EventLog(2, L"StartServer", L"Initialize : Success");


	m_hStopEvent = CreateEvent(NULL, TRUE, FALSE, L"Server Stop Event");
	m_hSendEvent = CreateEvent(NULL, TRUE, FALSE, L"Send Data Event");

	if ( WaitForSingleObject(m_hStopEvent, INFINITE) != WAIT_TIMEOUT )
	{
		SAFE_CLOSE_HANDLE(m_hStopEvent);
	}
	else
		return FALSE;

	return TRUE;
}

const BOOL ServiceManager::StopServer()
{
	if ( this->m_AcceptThread.StopThread() != TRUE )
	{
		m_Log.EventLog(2, L"StopServer", L"Release Fail : AcceptThread");
		return FALSE;
	}

	m_Log.EventLog(2, L"StopServer", L"Release OK : AcceptThread");

	if ( this->m_ControlThread.StopThread() != TRUE )
	{
		m_Log.EventLog(2, L"StopServer", L"Release Fail : ControlThread");
		return FALSE;
	}

	m_Log.EventLog(2, L"StopServer", L"Release OK : ControlThread");

	if ( this->m_WorkerThread.StopMultiThread() != TRUE )
	{
		m_Log.EventLog(2, L"StopServer", L"Release Fail : WorkerThread");
		return FALSE;
	}

	m_Log.EventLog(2, L"StopServer", L"Release OK : WorkerThread");

	if ( EndNetwork() != TRUE )
	{
		m_Log.EventLog(2, L"StopServer", L"Release Fail : END_NETWORK");
		return FALSE;
	}

	m_Log.EventLog(2, L"StopServer", L"Release OK : END_NETWORK");

	if ( this->ReleaseAllData() != TRUE )
		return FALSE;

	SetEvent(m_hStopEvent);

	return TRUE;
}

const UINT ServiceManager::_AcceptThread(LPVOID lpParam)
{
	ServiceManager* pThis = (ServiceManager*)lpParam;
	if ( !pThis )
		return FALSE;

	pThis->AcceptThread();

	return TRUE;
}

const UINT ServiceManager::_ControlThread(LPVOID lpParam)
{
	ServiceManager* pThis = (ServiceManager*)lpParam;
	if ( !pThis )
		return FALSE;

	pThis->ControlThread();

	return TRUE;
}

const UINT ServiceManager::_WorkerThread(LPVOID lpParam)
{
	ServiceManager* pThis = (ServiceManager*)lpParam;
	if ( !pThis )
		return FALSE;

	pThis->WorkerThread();

	return TRUE;
}

const UINT ServiceManager::_SendThread(LPVOID lpParam)
{
	ServiceManager* pThis = (ServiceManager*)lpParam;
	if ( !pThis )
		return FALSE;

	pThis->SendThread();

	return TRUE;
}

const BOOL ServiceManager::AcceptThread()
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
	servAddr.sin_port			= htons((u_short)m_ServerInfo.dwTCPPort);
	servAddr.sin_addr.s_addr	= htonl(INADDR_ANY);

	if ( bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR )
		return FALSE;

	if ( listen(hServSock, SOMAXCONN) == SOCKET_ERROR )
		return FALSE;

	int nSizeClntAddr = sizeof(clntAddr);

	while ( m_AcceptThread.IsRun() )
	{
		DWORD dwRecvByte = 0;
		DWORD dwFlags = 0;

		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &nSizeClntAddr);

		if ( hClntSock == INVALID_SOCKET )
		{
			if ( WSAEINTR == WSAGetLastError() )
			{
				m_Log.EventLog(2, L"AcceptThread", GetStringErrCode(WSAEINTR));
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

		SessionData* pSession = m_SessionPool.CreateSession();
		pSession->m_SocketCtx.clntSocket = hClntSock;
		pSession->SetClientAddr(&clntAddr);

		m_SessionPool.InsertSession(pSession);

		if ( m_IOCP.RegisterCilent(hClntSock, pSession) != TRUE )
		{
//			m_Log.EventLog();
			continue;
		}

		int nRet = WSARecv(hClntSock, &(pSession->m_SocketCtx.recvContext->wsaBuf), 1, &dwRecvByte, &dwFlags, &(pSession->m_SocketCtx.recvContext->overlapped), NULL);

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

const BOOL ServiceManager::ControlThread()
{
	WCHAR	szTitle[MAX_STRING] = _T("");
	WCHAR	szWatch[MAX_STRING]	= _T("");
	DWORD	dwHour = 0, dwMinute = 0, dwSecond = 0;

	HANDLE hTimeEvent = WSACreateEvent();

	while ( m_ControlThread.IsRun() )
	{
		if ( WaitForSingleObject(hTimeEvent, 999) == WAIT_TIMEOUT )	// 999 ms 를 1초로 가정하자. 왜냐하면 if 문 이하 수행처리 시간을 고려했다.
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

			wsprintf(szTitle, L"Time(%d/%d/%d) | Player(%d)", dwHour, dwMinute, dwSecond++, m_SessionPool.GetSessionSize());
			SetConsoleTitle(szTitle);
		}
	}

	SAFE_CLOSE_HANDLE(hTimeEvent);

	return TRUE;
}

/*
 *	멀티 스레드를 사용한다. 따라서 멤버 변수에 접근하면 꼭 Lock 을 걸어둔다
*/
const BOOL ServiceManager::WorkerThread()
{
	DWORD				dwSizeInOutData = 0;
	DWORD				dwResult = 0;
	SessionData*	pSession = NULL;
	PPerIoContext		pPerIoCtx = NULL;

	while ( m_WorkerThread.IsRun() )
	{
		dwSizeInOutData = 0;
		dwResult = 0;
		pSession = NULL;
		pPerIoCtx = NULL;

		if ( FALSE == GetQueuedCompletionStatus(m_IOCP.GetCompletionPort(), 
												&dwSizeInOutData, 
												(PULONG_PTR)&pSession, 
												(LPOVERLAPPED*)&pPerIoCtx, 
												INFINITE) )
			dwResult = WSAGetLastError();

		if ( dwResult == 0 )
		{
			DWORD dwError = GetLastError();

			if ( pSession == NULL )
				continue;

			if ( dwSizeInOutData == 0 )	// 클라이언트가 접속을 끊었다
			{
				static int snCount = 0;
				WCHAR szTemp[MAX_STRING] = _T("");
				wsprintf(szTemp, L"클라이언트 close 횟수 : %d", ++snCount);
				// 위에 3줄 코딩은 삭제하세요. 테스트로 남긴거예요.

				m_Log.EventLog(1, szTemp);
				m_SessionPool.DeleteSession(pSession);
				continue;
			}
			
			if ( pPerIoCtx == pSession->m_SocketCtx.recvContext )
			{
				// 보낸 패킷의 사이즈만큼 받아서 처리할 것
				PacketProcess(pSession);
				
				DWORD dwFlags = 0;	// out parameter, and I/O 처리가 끝나면 꼭 해당 소켓에 recv 를 걸어둔다.
				WSARecv(pSession->m_SocketCtx.clntSocket, &(pSession->m_SocketCtx.recvContext->wsaBuf), 1, NULL, &dwFlags, &(pSession->m_SocketCtx.recvContext->overlapped), NULL);
			}
			else if ( pPerIoCtx == pSession->m_SocketCtx.sendContext )
			{
				// WSASend 가 호출되고나서 Entry Point 가 여기로 온다. 마땅히 처리할 내용이 없다
			}
		}
		else
		{
			if ( WAIT_TIMEOUT == dwResult )
			{
			}
			else if ( ERROR_NETNAME_DELETED == dwResult ) 
			{
				m_Log.EventLog(2, L"WorkerThread", L"ERROR CODE : ERROR_NETNAME_DELETED");
				m_SessionPool.DeleteSession(pSession);
			}
			else 
			{
				m_Log.EventLog(2, L"GetQueuedCompletionStatus unknown Error [%d]", dwResult);
				m_SessionPool.DeleteSession(pSession);
			}

			continue;
		}
	}

	return TRUE;
}

const BOOL ServiceManager::SendThread()
{
	DWORD	dwSendBytes = 0;
	DWORD	dwFlags = 0;

	while ( m_SendThread.IsRun() )
	{
		DWORD dwRet = WaitForSingleObject(m_hSendEvent, INFINITE);

		if ( m_SendCtx != NULL )
		{
			int nReturn = ::WSASend(m_SendCtx->m_SocketCtx.clntSocket, &m_SendCtx->m_SocketCtx.sendContext->wsaBuf, 1, &dwSendBytes, dwFlags, &m_SendCtx->m_SocketCtx.sendContext->overlapped, NULL);

			if ( nReturn == SOCKET_ERROR )
			{
				if ( WSAGetLastError() != ERROR_IO_PENDING )
				{
					// 에러처리
				}
			}

			m_SendCtx = NULL;
		}

		ResetEvent(m_hSendEvent);
	}

	return TRUE;
}

const BOOL ServiceManager::PacketProcess(SessionData* pSession)
{
	BOOL bReturn = TRUE;

	header_special	Header;
	memcpy(&Header, pSession->m_SocketCtx.recvContext->Buffer, SIZE_HEADER);

	switch ( Header.command )
	{
	case CS_AUTH_LOGIN_REQ	:	this->RecvCS_AUTH_LOGIN_REQ(pSession);		break;
	default					:	bReturn = FALSE;							break;
	}

	return bReturn;
}