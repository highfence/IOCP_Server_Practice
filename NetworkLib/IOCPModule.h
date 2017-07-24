#pragma once

#include "../CommonLib/Common.h"
#include "../CommonLib/SessionData.h"

namespace NetworkLib
{
	using SessionData = CommonLib::SessionData;
	class IOCPModule
	{
	public :

		IOCPModule();
		~IOCPModule();

	public :

		const BOOL		StartIOCP();
		const BOOL		RegisterCilent(SOCKET ClientSocket, SessionData* Player);	// ���ø� �����ұ�?
		const HANDLE	GetCompletionPort();

	private :

		HANDLE	_completionPort;
	};
}
