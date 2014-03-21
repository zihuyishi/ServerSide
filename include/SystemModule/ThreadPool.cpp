#include "ThreadPool.h"
#include <assert.h>
ZHTTPBEGIN


//创建线程失败后再次尝试的次数
ZInt CThreadPool::m_cReTry = 5;

CThreadPool::CThreadPool() :
m_bStop(ZTrue)
{
}
CThreadPool::~CThreadPool()
{
	if (m_thread != nullptr) {
		CloseHandle(m_thread);
		m_thread = nullptr;
	}
}
ZBool CThreadPool::Create(ZInt threadNum)
{
	PMainParam lpParam = new MainParam(this, threadNum);
	ZUInt uiThread;
	m_thread = (HANDLE)_beginthreadex(
		NULL, 0, &ThisType::mainThread,
		lpParam, 0, &uiThread);
	if (m_thread == nullptr) {
		return ZFalse;
	}
	else {
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
		PPTASK task = nullptr;
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
			assert(hThread != nullptr);
			WaitForSingleObject(hEvent, 1000);
			pThis->m_threadQueue.push(hThread);
			pThis->m_taskMap.insert(std::make_pair(
				hThread, task));
			CloseHandle(hEvent);
		}
		
	}

	//init statue
	pThis->m_bStop = ZFalse;
	pThis->m_threadNum = threadNum + countTry - m_cReTry;  //计算实际创建的线程数

	//loop
	while (!pThis->m_bStop)
	{
		//first find a task
		if (pThis->m_taskQueue.empty()) {
			continue;
		}
		TASK task = pThis->m_taskQueue.front();
		pThis->m_taskQueue.pop();

		//now find a free thread
		while (pThis->m_threadQueue.empty()) {
			continue;
		}
		//get it!
		HANDLE freeHandle = pThis->m_threadQueue.front();
		pThis->m_threadQueue.pop();
		//give task to thread
		auto threadTask = pThis->m_taskMap.at(freeHandle);
		assert(threadTask != nullptr);
		assert(*threadTask == nullptr);
		*threadTask = new TASK;
		(*threadTask)->operator=(task);
	}

	return 0;
}

unsigned __stdcall CThreadPool::poolThread(void* lpParam)
{
	assert(lpParam != 0);
	PPoolParam poolParam = static_cast<PPoolParam>(lpParam);
	ThisType *pThis = poolParam->pThis;
	PTASK *pTask = poolParam->pTask;
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
			pThis->m_threadQueue.push(hMyHandle);
		}
	} while (1);


	return 0;
}
unsigned __stdcall CThreadPool::doNothing(void* lpParam)
{
	return 0;
}
ZHTTPEND