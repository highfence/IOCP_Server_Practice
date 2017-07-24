
#pragma once

#ifndef __SERVICEMAIN_H__
#define __SERVICEMAIN_H__

#include "../CommonLib/Common.h"
#include "../CommonLib/SessionPool.h"
#include "../CommonLib/LIB_THREAD.h"
#include "../CommonLib/LogSystem.h"
#include "../CommonLib/LIB_SERVERSYSTEM.h"
#include "../CommonLib/Lock.h"
#include "../NetworkLib/LIB_IOCPModule.h"

using namespace CommonLib;
using namespace NETWORKLIB;

class ServiceManager
{
public :	// Constructor & Destructor

	ServiceManager();
	~ServiceManager();

public :	// Server On and Off

	const BOOL	StartServer(LPTSTR* argv);
	const BOOL	StopServer();

private :	// Interface of Initialization

	const BOOL	InitialAllData();
	const BOOL	InitialProfile();
	const BOOL	InitialNetwork();
	const BOOL	InitialThread();
	const BOOL	InitialLog();

private :	// Interface of Release

	const BOOL	ReleaseAllData();
	const BOOL	ReleaseLog();

public :	// Log System

	LogSystem	m_Log;

private :	// Member Variables of ServiceMain

	LIB_MULTITHREAD		m_WorkerThread;
	LIB_SINGLETHREAD	m_AcceptThread;
	LIB_SINGLETHREAD	m_ControlThread;
	LIB_SINGLETHREAD	m_SendThread;
	LIB_IOCPModule		m_IOCP;
	LIB_SERVERINFO		m_ServerInfo;
	SessionPool		m_SessionPool;
	CriticalLock	m_Lock;
	SessionData*	m_SendCtx;
	HANDLE				m_hStopEvent;
	HANDLE				m_hSendEvent;

private :	// Function of Thread

	static const UINT _AcceptThread(LPVOID lpParam);
	static const UINT _ControlThread(LPVOID lpParam);
	static const UINT _WorkerThread(LPVOID lpParam);
	static const UINT _SendThread(LPVOID lpParam);
	const BOOL	AcceptThread();
	const BOOL	ControlThread();
	const BOOL	WorkerThread();
	const BOOL	SendThread();

private :	// REQ PACKET
	const BOOL	PacketProcess(SessionData* pSession);
	const BOOL	RecvCS_AUTH_LOGIN_REQ(SessionData* pSession);
private :	// ACK PACKET
	const BOOL	RecvCS_AUTH_LOGIN_ACK(SessionData* pSession);
	
};

#endif	// __SERVICEMAIN_H__