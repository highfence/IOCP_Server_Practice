#pragma once

#include <deque>

#include "../CommonLib/Common.h"
#include "../CommonLib/SessionPool.h"
#include "../CommonLib/Thread.h"
#include "../CommonLib/LogSystem.h"
#include "../CommonLib/ServerSystem.h"
#include "../CommonLib/Lock.h"
#include "../NetworkLib/IOCPModule.h"

using namespace CommonLib;
using namespace NetworkLib;

class ServiceManager
{
public :	

	// Constructor & Destructor
	ServiceManager();
	~ServiceManager();

	// Server On and Off
	const BOOL	StartServer(LPTSTR* argv);
	const BOOL	StopServer();

public :	

	// Log System
	LogSystem	_log;

private :	

	// Interface of Initialization
	const BOOL	initialAllData();
	const BOOL	initialProfile();
	const BOOL	initialNetwork();
	const BOOL	initialThread();
	const BOOL	initialLog();

	// Interface of Release
	const BOOL	releaseAllData();
	const BOOL	releaseLog();

	// Function of Thread
	const BOOL	acceptThread();
	const BOOL	controlThread();
	const BOOL	workerThread();
	const BOOL	sendThread();
	const BOOL  logicThread();

	// REQ PACKET
	const BOOL	packetProcess(const int sessionId);
	const BOOL	recvCS_AUTH_LOGIN_REQ(SessionData* pSession);

	// ACK PACKET
	const BOOL	recvCS_AUTH_LOGIN_ACK(SessionData* pSession);

private :	

	// Member Variables of ServiceMain
	MultiThread		  _workerThread;
	SingleThread	  _acceptThread;
	SingleThread	  _controlThread;
	SingleThread	  _sendThread;
	SingleThread	  _logicThread;
	IOCPModule		  _iocp;
	ServerInfo		  _serverInfo;
	SessionPool		  _sessionPool;
	CriticalLock	  _packetLock;
	std::deque<char*> _sendQueue;
	std::deque<int>   _packetProcessQueue;
	HANDLE			  _stopEvent;

	// Threads
	static const UINT _AcceptThread(LPVOID lpParam);
	static const UINT _ControlThread(LPVOID lpParam);
	static const UINT _WorkerThread(LPVOID lpParam);
	static const UINT _SendThread(LPVOID lpParam);
	static const UINT _LogicThread(LPVOID lpParam);
};