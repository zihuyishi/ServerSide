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
#ifndef NDEBUG
	m_taskCount = 0;
	m_taskDone = 0;
#endif
}
CThreadPool::~CThreadPool()
{
	if (m_thread != nullptr) {
		CloseHandle(m_thread);
		m_thread = nullptr;
	}
	for each(auto hThread in m_allThreads) {
		if (hThread != nullptr) {
			HANDLE hEvent = m_signedMap.at(hThread);
			CloseHandle(hEvent);
			CloseHandle(hThread);
		}
	}
	
}
ZBool CThreadPool::Create(ZInt threadNum)
{
	m_bStop = ZFalse;
	//create thread pool
	ZInt countTry = CThreadPool::m_cReTry;
	for (ZInt i = 0; i < threadNum; ++i) {
		ZUInt uiThread;
		//传入自己和每个线程的Task
		PPTASK task = new PTASK;
		*task = nullptr;
		HANDLE hThread;
		HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		PPoolParam poolParam = new PoolParam(this, task, &hThread, hEvent);
		hThread = (HANDLE)_beginthreadex(NULL,
			0, &ThisType::poolThread, poolParam,
			0, &uiThread);

		if (hThread == nullptr) {
			--i; --countTry;
			if (countTry == 0) { break; }
		}
		else {
			//WaitForSingleObject(hEvent, 1000);
			WaitForSingleObject(hEvent, INFINITE);
			ResetEvent(hEvent);
			m_threadQueue.push(hThread);
			m_allThreads.push_back(hThread);
			m_taskMap.insert(std::make_pair(
				hThread, task));
			m_signedMap.insert(std::make_pair(hThread, hEvent));
		}

	}

	//init statue
	m_threadNum = threadNum + countTry - m_cReTry;  //计算实际创建的线程数

	//create main thread
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
	m_taskQueue.push(std::make_pair(task, lpParam));
#ifndef NDEBUG
	m_taskCount++;
#endif
}
void CThreadPool::Stop()
{
	m_bStop = ZTrue;
}
void CThreadPool::WaitAllTaskDone(ZUInt uiMS)
{
	while (!m_taskQueue.empty())
	{
	}
}
void CThreadPool::WaitAllThreadDone(ZUInt uiMS)
{
	for each(auto hThread in m_allThreads) {
		if (hThread == nullptr) continue;
		WaitForSingleObject(hThread, uiMS);
	}
}
void CThreadPool::Release() {
	delete this;
}
unsigned __stdcall CThreadPool::mainThread(void* lpParam)
{
	assert(lpParam != nullptr);

	//get param
	PMainParam param = static_cast<PMainParam>(lpParam);
	ThisType* pThis = param->pThis;
	ZInt threadNum = param->number;
	delete param; param = nullptr;

	SetEvent(pThis->m_hEvent);
	//loop
	while (!pThis->m_bStop)
	{
		//first find a task
		TASK task;
		bool bRet = pThis->m_taskQueue.front(&task);
		if (!bRet) {
			continue;
		}
		pThis->m_taskQueue.pop();

		//now find a free thread
		HANDLE freeHandle;
		while ( !pThis->m_threadQueue.front(&freeHandle) ) {
			continue;
		}
		pThis->m_threadQueue.pop();

		//give task to thread
		auto threadTask = pThis->m_taskMap.at(freeHandle);
		HANDLE threadEvent = pThis->m_signedMap.at(freeHandle);
		assert(threadEvent != nullptr);
		assert(threadTask != nullptr);
		assert(*threadTask == nullptr);
		*threadTask = new TASK(task);
		//(*threadTask)->operator=(task);
		SetEvent(threadEvent);

#ifndef NDEBUG
		pThis->m_taskDone++;
#endif
	}
	for each (auto hThread in pThis->m_allThreads) {
		if (hThread != nullptr) {
			HANDLE hEvent = pThis->m_signedMap.at(hThread);
			SetEvent(hEvent);
		}
	}
	return 0;
}

unsigned __stdcall CThreadPool::poolThread(void* lpParam)
{
	assert(lpParam != 0);
	PPoolParam poolParam = static_cast<PPoolParam>(lpParam);
	ThisType *pThis = poolParam->pThis;
	PTASK *pTask;
	assert(poolParam->pTask != nullptr);
	pTask = poolParam->pTask;
	HANDLE hMyHandle = *(poolParam->pHandle);
	HANDLE hMyEvent = poolParam->bUseEvent;
	SetEvent(hMyEvent);
	delete poolParam;

	do {
		WaitForSingleObject(hMyEvent, INFINITE);
		if (*pTask == nullptr) {
			continue;
		}
		else {
			(*pTask)->first((*pTask)->second);
			delete *pTask;
			*pTask = nullptr;
			//todo 处理一下，把自己push进线程的队列
			pThis->m_threadQueue.push(hMyHandle);
			ResetEvent(hMyEvent);
		}
	} while (!pThis->m_bStop);

	delete pTask;
	return 0;
}
unsigned __stdcall CThreadPool::doNothing(void* lpParam)
{
	return 0;
}
ZHTTPEND