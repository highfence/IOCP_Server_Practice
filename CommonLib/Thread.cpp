
#include "Thread.h"
#include <process.h>

namespace CommonLib
{
	Thread::Thread()
	{
		_isRun = FALSE;
		_threadHandle = NULL;
		_threadID = 0;
	}

	Thread::~Thread()
	{
		if (_threadHandle != NULL)
			StopThread();
	}

	const BOOL Thread::StartThread()
	{
		_isRun = TRUE;

		_threadHandle = (HANDLE)::_beginthreadex(NULL, 0, ThreadFunc, (LPVOID)this, 0, &_threadID);

		if (_threadHandle == NULL)
		{
			_isRun = FALSE;
			return FALSE;
		}

		return TRUE;
	}

	const BOOL Thread::StopThread()
	{
		if (_threadHandle != NULL)
		{
			if (WaitForSingleObject(_threadHandle, THREAD_EXITTIME) == WAIT_TIMEOUT)
			{
				if (TerminateThread(_threadHandle, 0))
				{
					_isRun = FALSE;
					CloseHandle(_threadHandle);
					_threadHandle = NULL;
				}
			}
		}
		else
			return FALSE;

		return TRUE;
	}

	UINT WINAPI Thread::ThreadFunc(LPVOID lpParam)
	{
		Thread* pThis = (Thread*)lpParam;
		if (pThis)
			return pThis->Run();
		else
			return FALSE;

		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////

	MultiThread::MultiThread()
	{
		_isRun = FALSE;
	}

	MultiThread::~MultiThread()
	{
		int nSize = (int)_threadVec.size();

		for (int iter = 0; iter < nSize; iter++)
		{
			SingleThread* pThread = _threadVec[iter];
			if (pThread)
				delete pThread;
		}

		_threadVec.clear();
	}

	const VOID MultiThread::Initialize(const int nSize, const LPVOID lpParam, const ThreadCallBackFunc lpFunc)
	{
		for (int iter = 0; iter < nSize; iter++)
		{
			SingleThread* pThread = new SingleThread;
			if (pThread)
			{
				pThread->Initialize(lpParam, lpFunc);
				_threadVec.push_back(SingleThreadVector::value_type(pThread));
			}
		}
	}

	const UINT MultiThread::StartMultiThread()
	{
		_isRun = TRUE;
		int nSize = (int)_threadVec.size();

		for (int iter = 0; iter < nSize; iter++)
		{
			SingleThread* pThread = _threadVec[iter];
			if (pThread)
				pThread->StartThread();
		}

		return TRUE;
	}

	const BOOL MultiThread::StopMultiThread()
	{
		_isRun = FALSE;
		int nSize = (int)_threadVec.size();

		for (int iter = 0; iter < nSize; iter++)
		{
			SingleThread* pThread = _threadVec[iter];
			if (pThread)
			{
				if (pThread->StopThread() != TRUE)
					return FALSE;
			}
		}

		return TRUE;
	}
}