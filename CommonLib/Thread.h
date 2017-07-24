
#pragma once


#include "Common.h"
#include <vector>

namespace CommonLib
{
	enum ENUM_THREAD
	{
		THREAD_EXITTIME	= 100,	// 스레드 종료되는 시간
	};

	/*
	 *	Make a person	: 이재득
	 *	Make a date		: 2007.02.09
	 *	Class name		: LIB_THREAD
	 *	Class describe	: 스레드 클래스
	*/

	class Thread
	{
	public :	// Constructor & Destructor

		Thread();
		~Thread();

	public :	// Member public function

		virtual const UINT Run() = 0;
        const	BOOL		StartThread();
		const	BOOL		StopThread();
		const	BOOL		IsRun()			{	return _isRun;	}
		const	unsigned	GetThreadID()	{	return _threadID;	}

	protected :	// Member protected variable

		volatile BOOL	_isRun;

	private :	// Member private variable

		HANDLE			_threadHandle;
		unsigned		_threadID;

	private :	// Member private function

		static	UINT WINAPI	ThreadFunc(LPVOID lpParam);

	};

	typedef const UINT (*ThreadCallBackFunc)(LPVOID lpParam);

	class SingleThread	: public Thread
	{
	public :

		const	VOID	Initialize(const LPVOID lpParam, const ThreadCallBackFunc lpFunc)
		{
			_param	= lpParam;
			_func	= lpFunc;
		}

		const	UINT	Run()				{	return (*_func)(_param);	}
		
	private :

		LPVOID				_param;
		ThreadCallBackFunc	_func;
	};

	typedef std::vector<SingleThread*> SingleThreadVector;

	class MultiThread
	{
	public :

		MultiThread();
		~MultiThread();

	public :

		const	VOID	Initialize(const int nSize, const LPVOID lpParam, const ThreadCallBackFunc lpFunc);
		const	UINT	StartMultiThread();
		const	BOOL	StopMultiThread();
		const	BOOL	IsRun()		{	return _isRun;	}

	private :

		SingleThreadVector		_threadVec;
		volatile	BOOL		_isRun;
	};
}
