#include "ZDef.h"
#include <queue>
#include <iostream>
#include <Windows.h>
#ifndef ZHTTP_CONTAINER_HEADER
#define ZHTTP_CONTAINER_HEADER

template <class T>
class SafeQueue
{
public:
	typedef SafeQueue	ThisType;
private:
	std::queue<T>		m_queue;
	HANDLE				m_mutex;
public:
	SafeQueue() :
		m_mutex(nullptr)
	{
		m_mutex = CreateMutex(NULL, FALSE, NULL);
	}
	~SafeQueue()
	{
		if (m_mutex != nullptr) {
			CloseHandle(m_mutex);
			m_mutex = nullptr;
		}
	}
	SafeQueue(const ThisType& src) :
		m_mutex(nullptr)
	{
		m_mutex = CreateMutex(NULL, FALSE, NULL);
		*this = src;
	}
	const ThisType& operator=(const ThisType& src) {
		m_queue = src.m_queue;
		return *this;
	}
public:
	//不要相互调用！！！
	bool empty() {
		WaitForSingleObject(m_mutex, INFINITE);
		bool bRet = m_queue.empty();
		ReleaseMutex(m_mutex);
		return bRet;
	}
	bool front(T* tRet) {
		WaitForSingleObject(m_mutex, INFINITE);
		bool bRet;
		if (!m_queue.empty()) {
			*tRet = m_queue.front();
			bRet = true;
		}
		else {
			bRet = false;
		}
		ReleaseMutex(m_mutex);
		return bRet;
	}
	void push(const T& element)
	{
		WaitForSingleObject(m_mutex, INFINITE);
		m_queue.push(element);
		ReleaseMutex(m_mutex);
	}
	void pop()
	{
		WaitForSingleObject(m_mutex, INFINITE);
		if (!m_queue.empty()) {
			m_queue.pop();
		}
		ReleaseMutex(m_mutex);
	}
	ZUInt size()
	{
		WaitForSingleObject(m_mutex, INFINITE);
		ZUInt size = m_queue.size();
		ReleaseMutex(m_mutex);
		return size;
	}
	void swap(ThisType& rhs)
	{
		WaitForSingleObject(m_mutex, INFINITE);
		m_queue.swap(rhs.m_queue);
		ReleaseMutex(m_mutex);
	}
};

#endif