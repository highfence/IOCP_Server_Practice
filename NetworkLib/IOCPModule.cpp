#include "IOCPModule.h"

namespace NetworkLib
{
	IOCPModule::IOCPModule()
	{
		_completionPort = NULL;
	}

	IOCPModule::~IOCPModule()
	{
		SAFE_CLOSE_HANDLE(_completionPort);
	}

	const BOOL IOCPModule::StartIOCP()
	{
		_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

		if (_completionPort == NULL)
			return FALSE;

		return TRUE;
	}

	const BOOL IOCPModule::RegisterCilent(SOCKET ClientSocket, CommonLib::SessionData* Player)
	{
		HANDLE hResult = INVALID_HANDLE_VALUE;
		hResult = CreateIoCompletionPort((HANDLE)ClientSocket, _completionPort, (ULONG_PTR)Player, 0);

		if (hResult == INVALID_HANDLE_VALUE)
			return FALSE;

		return TRUE;
	}

	const HANDLE IOCPModule::GetCompletionPort()
	{
		return _completionPort;
	}
}