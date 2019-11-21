/***************************************************
**
**    III                DDD  KKK
**    III                DDD  KKK
**                       DDD  KKK
**    III   DDDDDDDDDDD  DDD  KKK            KKK
**    III  DDD           DDD  KKK            KKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK   KKKKKKKKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK            KKK
**    III   DDDDDDDDDDDDDDDD  KKK            KKK
**
**
**     SSS         FF
**    S           F   T
**     SSS   OO   FF  TTT W   W  AAA  R RR   EEE
**        S O  O  F   T   W W W  AAAA RR  R EEEEE
**    S   S O  O  F   T   W W W A   A R     E
**     SSS   OO  FFF   TT  W W   AAAA R      EEE
**
**
****************************************************
**
**	Filename	: Threads.h
**	Author		: I.Ferguson
**	Version		: v1.0
**	Date		: 12-05-99
**
****************************************************
**
**	Classes:
**
**	
**
****************************************************/

/** Copyright 1998 IDK Software Ltd **/

#include <vector>

#ifndef SIATHREADS_H
#define SIATHREADS_H



#ifdef _WIN32
	#include <process.h>
#else
	#include <pthread.h>
#endif

/*******************************************************
**
**	Functions
**
********************************************************/

/*
 *	Create a thread
 *
 *	pFunction	=	pointer to function that is run by thread
 *	pArguments	=	pointer to argument list
 */

class SIAThread;
class SIAThreadStatus {
public:
	typedef enum {
		InitFailed = -1,
		StartFailed = -2,
		Ok = 0
	} EStatus;
private:
	EStatus m_EStatus;
public:
	SIAThreadStatus()
	{
		m_EStatus = SIAThreadStatus::Ok;
	}

	void Status(SIAThreadStatus::EStatus aEStatus)
	{
		m_EStatus = aEStatus;
	}

	SIAThreadStatus::EStatus Status()
	{
		return m_EStatus;
	}

	SIAThreadStatus& operator = (SIAThreadStatus::EStatus aStatus)
	{
		m_EStatus = aStatus;
		return *this;
	}

	operator SIAThreadStatus::EStatus()
	{
		return m_EStatus;
	}


};

class SIAThreadFunctor {
protected:
	bool m_Abort;

	SIAThreadStatus m_Status;
public:
	void _Start()
	{
		if (!Start())
		{
			m_Status = SIAThreadStatus::StartFailed;
		}

	}
	friend class SIAThread;

public:
	SIAThreadFunctor()
	{
		m_Status = SIAThreadStatus::Ok;
		m_Abort = false;
		if (!Init())
		{
			m_Status = SIAThreadStatus::InitFailed;
		}

	};

	virtual ~SIAThreadFunctor()
	{
		void Stop();
	};

	virtual bool Init()
	{
		return true;
	}
	
	virtual bool Start() = 0;
	virtual bool Stop()
	{
		return true;
	}

	SIAThreadStatus::EStatus GetStatus()
	{
		return m_Status;
	}
};



#ifdef _WIN32
	bool SIACreateThread(void( __cdecl * pFunction)(void*), void* pArguments);
#else
	bool SIACreateThread(void(Function)(void*), void* pArguments);
#endif

/*
 *	Terminate the current thread
 */
bool SIAEndThread();

/*
 *	Give up remaining time slice of current thread
 */

void SIAYieldThread();
class SIAThread
{
	// This is the globle thread list that all threads
	// are registered.
	//

	static std::vector<SIAThread *> m_List;
public:

	typedef enum {
		InitFailed = -2,
		StartFailed = -1,
		DeleteFailed = -3,
		NoThreadFunctor = -4,
		Ok = 0
	} EStatus;
private:

	unsigned long m_ThreadId;
	static EStatus s_Status;
	bool m_Running;
	EStatus m_Status;
	SIAThreadFunctor *m_pThreadFunctor;

	static void RunThread(void *pThis)
	{
		SIAThread *l_pThis = (SIAThread *)pThis;

		l_pThis->m_pThreadFunctor->_Start();
	}


public:

	SIAThread(SIAThreadFunctor *pThreadFunctor)
	{
		m_pThreadFunctor = pThreadFunctor;
		m_Running = false;
		m_List.push_back(this);
	}

	~SIAThread();

	bool Start();

	void Set(SIAThreadFunctor *pThreadFunctor)
	{
		m_pThreadFunctor = pThreadFunctor;
	}

	SIAThreadStatus::EStatus GetStatus()
	{
		return m_pThreadFunctor->GetStatus();
	}

	unsigned long GetHandle()
	{
		return m_ThreadId;
	}

	static void TerminateAll()
	{

		for (std::vector<SIAThread *>::iterator i = m_List.begin(); i < m_List.end(); i++) {
		
			SIAThread *l_Thread = *i;
			delete l_Thread;
			
		};
		m_List.clear();
	}

protected:

	SIAThreadFunctor *GetThreadFunctor()
	{
		return m_pThreadFunctor;
	}
};

/*
class SIASleep
{
SIASleep
};
*/
/*
 *	Blocking object
 */



void SIASleep(int sleeptime);

	
#endif




