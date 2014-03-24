#include "FunctionTEst.h"
#include "..\include\SystemModule\ThreadPool.h"
#include <iostream>
using namespace ZHttp;
unsigned __stdcall task(void* lpParam)
{
	int num = *(int*)lpParam;
	//std::cout << "num is " << num <<std::endl;
	return 0;
}
void ThreadPoolTest1()
{
	CThreadPool *threadpool = new CThreadPool();
	threadpool->Create(4);
	int *pNum = new int;
	*pNum = 1024;
	threadpool->AddTask(task, pNum);
}
void ThreadPoolTest2()
{
	CThreadPool *threadpool =  new CThreadPool();
	threadpool->Create(4);
	for (int i = 0; i < 100; i++) {
		int *num = new int;
		*num = i;
		threadpool->AddTask(task, num);
	}
}
void ThreadPoolTest()
{
	ThreadPoolTest1();
	ThreadPoolTest2();
}