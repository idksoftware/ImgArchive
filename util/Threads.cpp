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
**	Filename	: SIAApplication.h
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
#ifdef _WIN32
#include <windows.h>
#endif
#include <stdint.h>
#include "Threads.h"

void SIASleep(int sleeptime)
{
#ifdef POSIX
    struct timespec ts;
    struct timespec tr;
	double rem;

	// Calculate no secs
	ts.tv_sec = (time_t)sleeptime / 60;

	// Calculate no nanosecs
	rem = sleeptime - ( ts.tv_sec * 60 );

	ts.tv_nsec = ((double)(rem / 60)) * 999999999;

	nanosleep(&ts, &tr);
#endif
#ifdef _WIN32
	uint32_t millis;
	millis = sleeptime;
	Sleep(millis);
#endif
	return;
}

/*
 *		WINDOWS 32 IMPLEMENTATION
 */

SIAThread::EStatus SIAThread::s_Status = SIAThread::Ok;
std::vector<SIAThread *> SIAThread::m_List;
#ifdef _MT

#include <process.h>


bool SIAThread::Start()
{
	void *l_vPtr = (void *)this;
//		IDKCreateThread((SIAThread::RunThread),l_vPtr);

	if ((m_ThreadId = _beginthread((SIAThread::RunThread), 0, l_vPtr)) < 0)
	{
		return false;
	}
	return true;
}

SIAThread::~SIAThread()
{
	if (m_pThreadFunctor)
	{
		delete m_pThreadFunctor;
	}
	std::vector<SIAThread *>::iterator itr;
	itr = std::find(m_List.begin(), m_List.end(), this);  // <algorithm>
	if (itr != m_List.end()) {
		m_List.erase(itr);
	}
	_endthread();
}
/*
 *	Create a thread
 *
 *	pFunction	=	pointer to function that is run by thread
 *	pArguments	=	pointer to argument list
 */
bool IDKCreateThread(void( __cdecl * pFunction)(void*), void* pArguments)
{
	if (_beginthread(*pFunction, 0, pArguments) < 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/*
 *	Terminate the current thread
 */
bool IDKEndThread()
{
	_endthread();

	return true;
}

/*
 *	Give up remaining time slice of current thread
 */
void IDKYieldThread()
{
	Sleep(0);
}

//
// Mutex
//

#else
/*
 *		END WINDOWS 32 IMPLEMENTATION
 */


#ifndef _POSIX
#include <pthread.h>
#endif


bool SIAThread::Start()
{
	void *l_vPtr = (void *)this;
//		IDKCreateThread((SIAThread::RunThread),l_vPtr);

	void*(*ppFunction)(void*)  = (void*(*)(void*))(SIAThread::RunThread);

	return ((m_ThreadId = pthread_create(nullptr, nullptr, ppFunction, l_vPtr))==0);
}

/*
 *	Create a thread
 *
 *	pFunction	=	pointer to function that is run by thread
 *	pArguments	=	pointer to argument list
 */
bool IDKCreateThread(void(pFunction)(void*), void* pArguments)
{
	
	void*(*ppFunction)(void*)  = (void*(*)(void*))pFunction;
	bool ret = (pthread_create(nullptr, nullptr, ppFunction, pArguments)) == 0;
	return ret;
}

SIAThread::~SIAThread()
{
	if (m_pThreadFunctor)
	{
		delete m_pThreadFunctor;

	}

	//if(m_List.Delete(this))
	//{
	//	s_Status = DeleteFailed;
	//}
	void* status = nullptr;
	pthread_exit(status);

}
/*
 *	Terminate the current thread
 */
bool IDKEndThread()
{
	void* status = nullptr;
	pthread_exit(status);
	return true; 
}


/*
 *	Give up remaining time slice of current thread
 */
/*
void IDKYieldThread()
{
#ifdef _POSIX
	sched_yield();
#else
	thr_yield();
#endif
}

SIACriticalSection::SIACriticalSection()
{
	m_Error = pthread_mutex_init(&m_section, NULL);
}

SIACriticalSection::~SIACriticalSection()
{
	pthread_mutex_destroy(&m_section);
}

bool SIACriticalSection::Enter()
{
	return (pthread_mutex_lock(&m_section)==0);
}

bool SIACriticalSection::Leave()
{
	return (pthread_mutex_unlock(&m_section)==0);
}


SIAMutex::SIAMutex()
{
	m_IsInitalised = true;
}

SIAMutex::~SIAMutex()
{

}

bool SIAMutex::Init()
{
		
	m_IsInitalised = false;
	return (pthread_mutex_init(&m_section, NULL)==0);
}

bool SIAMutex::Lock()
{
	return (pthread_mutex_lock(&m_section)==0);
}

bool SIAMutex::Unlock()
{
	return (pthread_mutex_unlock(&m_section)==0);
}
*/
#endif



