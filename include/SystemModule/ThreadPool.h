#include "..\Common\ZDef.h"
#include "..\Common\Container.h"
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <functional>
#include <process.h>
#include <Windows.h>


#ifndef ZHTTP_THREADPOOL_HEADER
#define ZHTTP_THREADPOOL_HEADER

ZHTTPBEGIN

class CThreadPool :
	public UnCopyable
{
public:
	typedef unsigned(__stdcall *threadTask)(void*);
	typedef CThreadPool						ThisType;
private:
	typedef SafeQueue<HANDLE>				THREADQUEUE;
	typedef std::vector<HANDLE>				THREADVECTOR;
	typedef std::pair<threadTask, void*>	TASK, *PTASK, **PPTASK;
	typedef SafeQueue<TASK>					TASKQUEUE;
	typedef std::map<HANDLE, PPTASK>		TASKMAP;
	typedef std::map<HANDLE, HANDLE>		SIGNEDMAP;

	typedef struct tagMainParam {
		ThisType*		pThis;
		ZInt			number;
		tagMainParam(CThreadPool::ThisType* p, ZInt n) :
			pThis(p), number(n)
		{}
	} MainParam, *PMainParam;

	typedef struct tagPoolParam {
		ThisType*		pThis;
		PPTASK			pTask;
		HANDLE			*pHandle;
		HANDLE			bUseEvent;
		tagPoolParam(ThisType* p, PPTASK t, HANDLE* h, HANDLE e) :
			pThis(p), pTask(t), 
			pHandle(h), bUseEvent(e)
		{}
	} PoolParam, *PPoolParam;

	THREADQUEUE								m_threadQueue;		//可用线程队列
	SIGNEDMAP								m_signedMap;
	THREADVECTOR							m_allThreads;
	TASKQUEUE								m_taskQueue;		//待完成的任务队列
	TASKMAP									m_taskMap;			//线程队列和任务表
	HANDLE									m_thread;			//控制线程
	HANDLE									m_hEvent;			//同步信号 
	ZInt									m_threadNum;		//线程池线程数
	ZBool									m_bStop;			//是否停下

	static ZInt								m_cReTry;			//Create时如创建线程失败重试的总共次数
	static TASK								m_nullTask;
public:
#ifndef NDEBUG
	ZInt									m_taskCount;
	ZInt									m_taskDone;
#endif
public:
	~CThreadPool();
	CThreadPool();
public:
	ZBool Create(ZInt threadNum);
	void AddTask(threadTask task, void* lpParam);
	void Stop();
	/**
	等待已经安排的所有任务做完
	*/
	void WaitAllTaskDone(ZUInt uiMS = INFINITE);
	/**
	Stop()后等待所有的线程结束
	*/
	void WaitAllThreadDone(ZUInt uiMS = INFINITE);
	void Release();
private:
	static unsigned __stdcall mainThread(void* lpParam);
	static unsigned __stdcall poolThread(void* lpParam);
	static unsigned __stdcall doNothing(void* lpParam);
};

ZHTTPEND
#endif