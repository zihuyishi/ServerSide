#include "FunctionTEst.h"
#include "..\include\SystemModule\ThreadPool.h"
#include <iostream>
using namespace ZHttp;
int g_nums[100];
int g_num = 0;
unsigned __stdcall task(void* lpParam)
{
	int num = *(int*)lpParam;
	//std::cout << "num is " << num <<std::endl;
	g_num += num;
	return 0;
}
void ThreadPoolTest1()
{
	CThreadPool *threadpool = new CThreadPool();
	for (int i = 0; i < 4; i++) {
		int *num = new int;
		*num = 0;
		threadpool->AddTask(task, num);
	}
	threadpool->Create(2);
	threadpool->WaitAllTaskDone();
	threadpool->Stop();
	threadpool->WaitAllThreadDone();
#ifndef NDEBUG
	std::cout << "task count : " << threadpool->m_taskCount << std::endl;
	std::cout << "task done : " << threadpool->m_taskDone << std::endl;
#endif
	threadpool->Release();
}
void ThreadPoolTest2()
{
	CThreadPool *threadpool =  new CThreadPool();
	for (int i = 0; i < 100; i++) {
		int *num = new int;
		*num = i;
		threadpool->AddTask(task, num);
	}
	threadpool->Create(4);
	threadpool->WaitAllTaskDone();
	threadpool->Stop();
	threadpool->WaitAllThreadDone();
#ifndef NDEBUG
	std::cout << "g_num : " << g_num << std::endl;
	std::cout << "task count : " << threadpool->m_taskCount << std::endl;
	std::cout << "task done : " << threadpool->m_taskDone << std::endl;
#endif
	threadpool->Release();
}
void ThreadPoolTest()
{
	ThreadPoolTest1();
	ThreadPoolTest2();
}