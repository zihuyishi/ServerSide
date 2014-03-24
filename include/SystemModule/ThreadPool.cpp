#include "ThreadPool.h"
#include <assert.h>
ZHTTPBEGIN

namespace {
	
	template<typename queueT>
	void InterQueuePop(std::queue<queueT>& queue, HANDLE mutex)
	{
		WaitForSingleObject(mutex, INFINITE);
		queue.pop();
		ReleaseMutex(mutex);
	}
	template<typename queueT>
	void InterQueuePush(std::queue<queueT>& queue, queueT& element, HANDLE mutex)
	{
		WaitForSingleObject(mutex, INFINITE);
		queue.push(element);
		ReleaseMutex(mutex);
	}
}

//创建线程失败后再次尝试的次数
ZInt CThreadPool::m_cReTry = 5;

CThreadPool::CThreadPool() :
m_bStop(ZTrue), m_hEvent(nullptr),
m_thread(nullptr)
{
	m_threadMutex = CreateMutex(NULL, FALSE, NULL);
	m_taskMutex = CreateMutex(NULL, FALSE, NULL);
}
CThreadPool::~CThreadPool()
{
	if (m_thread != nullptr) {
		CloseHandle(m_thread);
		m_thread = nullptr;
	}
	CloseHandle(m_threadMutex);
	CloseHandle(m_taskMutex);
}
ZBool CThreadPool::Create(ZInt threadNum)
{
	PMainParam lpParam = new MainParam(this, threadNum);
	ZUInt uiThread;
	m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_thread = (HANDLE)_beginthreadex(
		NULL, 0, &ThisType::mainThread,
		lpParam, 0, &uiThread);
	if (m_thread == nullptr) {
		return ZFalse;
	}
	else {
		WaitForSingleObject(m_hEvent, INFINITE);
		CloseHandle(m_hEvent);
		return ZTrue;
	}
}
void CThreadPool::AddTask(threadTask task, void* lpParam)
{
	assert(m_thread != nullptr);
	m_taskQueue.push(std::make_pair(task, lpParam));
}
unsigned __stdcall CThreadPool::mainThread(void* lpParam)
{
	assert(lpParam != nullptr);

	//get param
	PMainParam param = static_cast<PMainParam>(lpParam);
	ThisType* pThis = param->pThis;
	ZInt threadNum = param->number;
	delete param; param = nullptr;

	//create thread pool
	ZInt countTry = CThreadPool::m_cReTry;
	for (ZInt i = 0; i < threadNum; ++i) {
		ZUInt uiThread;
		//传入自己和每个线程的Task
		PPTASK task = new PTASK;
		*task = nullptr;
		HANDLE hThread;
		HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		PPoolParam poolParam = new PoolParam(pThis, task, &hThread, hEvent);
		hThread = (HANDLE)_beginthreadex(NULL,
			0, &ThisType::poolThread, poolParam, 
			0, &uiThread);

		if (hThread == nullptr) {
			--i; --countTry;
			if (countTry == 0) { break; }
		}
		else {
			//succeed create a thread
			assert(hThread != nullptr);
			//WaitForSingleObject(hEvent, 1000);
			WaitForSingleObject(hEvent, INFINITE);
			SuspendThread(hThread);
			pThis->m_threadQueue.push(hThread);
			pThis->m_taskMap.insert(std::make_pair(
				hThread, task));
			CloseHandle(hEvent);
		}
		
	}

	//init statue
	pThis->m_bStop = ZFalse;
	pThis->m_threadNum = threadNum + countTry - m_cReTry;  //计算实际创建的线程数
	SetEvent(pThis->m_hEvent);
	//loop
	while (!pThis->m_bStop)
	{
		//first find a task
		if (pThis->m_taskQueue.empty()) {
			continue;
		}
		TASK task = pThis->m_taskQueue.front();
		//pThis->m_taskQueue.pop();
		InterQueuePop(pThis->m_taskQueue, pThis->m_taskMutex);

		//now find a free thread
		while (pThis->m_threadQueue.empty()) {
			continue;
		}
		//get it!
		HANDLE freeHandle = pThis->m_threadQueue.front();
		//pThis->m_threadQueue.pop();
		InterQueuePop(pThis->m_threadQueue, pThis->m_threadMutex);

		//give task to thread
		auto threadTask = pThis->m_taskMap.at(freeHandle);
		assert(threadTask != nullptr);
		assert(*threadTask == nullptr);
		*threadTask = new TASK;
		(*threadTask)->operator=(task);
		ResumeThread(freeHandle);
	}

	return 0;
}

unsigned __stdcall CThreadPool::poolThread(void* lpParam)
{
	assert(lpParam != 0);
	PPoolParam poolParam = static_cast<PPoolParam>(lpParam);
	ThisType *pThis = poolParam->pThis;
	PTASK *pTask;
	pTask = poolParam->pTask;
	HANDLE hMyHandle = *(poolParam->pHandle);
	SetEvent(poolParam->bUseEvent);

	do {
		if (*pTask == nullptr) {
			continue;
		}
		else {
			(*pTask)->first((*pTask)->second);
			delete *pTask;
			*pTask = nullptr;
			//todo 处理一下，把自己push进可用的队列
			//pThis->m_threadQueue.push(hMyHandle);
			InterQueuePush(pThis->m_threadQueue, hMyHandle, pThis->m_threadMutex);
			SuspendThread(hMyHandle);
		}
	} while (1);


	return 0;
}
unsigned __stdcall CThreadPool::doNothing(void* lpParam)
{
	return 0;
}
ZHTTPEND