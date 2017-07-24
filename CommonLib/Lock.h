#pragma once

namespace CommonLib
{
	/*
	 *	Make a person	: �����
	 *	Make a date		: 2007.02.09
	 *	Class name		: LIB_LOCK
	 *	Class describe	: CRITICAL_SECTION �� �̿��� �����ڿ� ó�����
	*/

	class LockBase
	{
	public :

		LockBase()			{}
		virtual ~LockBase()	{}

	public :

		VOID virtual Lock() = 0;
		VOID virtual UnLock() = 0;

	};

	class CriticalLock : public LockBase
	{
	private :

		CRITICAL_SECTION	_cs;

	public :

		CriticalLock()			{	InitializeCriticalSection(&_cs);	}
		~CriticalLock()			{	DeleteCriticalSection(&_cs);		}

	public :

		inline VOID Lock()		{	EnterCriticalSection(&_cs);	}
		inline VOID UnLock()	{	LeaveCriticalSection(&_cs);	}
	};
}