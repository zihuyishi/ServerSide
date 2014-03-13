#include "..\Common\ZDef.h"
#include <Windows.h>
#ifndef SYSTEM_IOFILE_HEADER
#define SYSTEM_IOFILE_HEADER

ZHTTPBEGIN
	
class CIOFile :
	public UnCopyable
{
public:
	CIOFile();
	
	virtual ~CIOFile();
public:
	/**
	@返回值 >=0 成功
	*/
	ZInt Create(const ZString& filePath, ZULong fileSize = 1024);
	ZInt Open(const ZString& filePath, ZBool bCreate = ZFalse);
	ZInt Read(ZULong offset, void* outBuf, ZULong length);
	ZInt Append(ZULong length);
	ZInt Write(ZULong offset, const void* data, ZULong length);
	void Close();
private:
	CIOFile(const ZString& filePath);
	void Init();
private:
	HANDLE m_hFile = INVALID_HANDLE_VALUE; 
	HANDLE m_hMapFile = nullptr;

	ZULong m_mapSize = -1;
	ZULong m_ulSysGran = 1;
};

ZHTTPEND
#endif