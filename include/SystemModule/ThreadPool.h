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
		HANDLE			*pHandle;
		HANDLE			bUseEvent;
		tagPoolParam(ThisType* p, PPTASK t, HANDLE* h, HANDLE e) :
			pThis(p), pTask(t), 
			pHandle(h), bUseEvent(e)
		{}
	} PoolParam, *PPoolParam;

	THREADQUEUE								m_threadQueue;		//�����̶߳���
	THREADQUEUE								m_finishQueue;		//����������̶߳���
	TASKQUEUE								m_taskQueue;		//����ɵ��������
	HANDLE									m_threadMutex;		//
	HANDLE									m_taskMutex;		//
	TASKMAP									m_taskMap;			//�̶߳��к������
	HANDLE									m_thread;			//�����߳�
	HANDLE									m_hEvent;			//ͬ���ź� 
	ZInt									m_threadNum;		//�̳߳��߳���
	ZBool									m_bStop;			//�Ƿ�ͣ��

	static ZInt								m_cReTry;			//Createʱ�紴���߳�ʧ�����Ե��ܹ�����
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