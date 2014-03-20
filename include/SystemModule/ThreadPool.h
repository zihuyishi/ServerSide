#include "..\Common\ZDef.h"
#include <iostream>
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
	typedef std::queue<HANDLE>				THREADQUEUE;
	typedef std::pair<threadTask, void*>	TASK, *PTASK, **PPTASK;
	typedef std::queue<TASK>				TASKQUEUE;
	typedef std::map<HANDLE, PPTASK>		TASKMAP;

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
		tagPoolParam(ThisType* p, PPTASK t) :
			pThis(p), pTask(t)
		{}
	} PoolParam, *PPoolParam;

	THREADQUEUE								m_threadQueue;		//可用线程队列
	THREADQUEUE								m_finishQueue;		//做完任务的线程队列
	TASKQUEUE								m_taskQueue;		//待完成的任务队列
	TASKMAP									m_taskMap;			//线程队列和任务表
	HANDLE									m_thread;			//控制线程
	ZInt									m_threadNum;		//线程池线程数
	ZBool									m_bStop;			//是否停下

	static ZInt								m_cReTry;			//Create时如创建线程失败重试的总共次数
	static TASK								m_nullTask;
public:
	~CThreadPool();
	CThreadPool();
public:
	ZBool Create(ZInt threadNum);
	void AddTask(threadTask task, void* lpParam);
private:
	static unsigned __stdcall mainThread(void* lpParam);
	static unsigned __stdcall poolThread(void* lpParam);
	static unsigned __stdcall doNothing(void* lpParam);
};

ZHTTPEND
#endif