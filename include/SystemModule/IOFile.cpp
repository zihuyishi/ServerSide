#include "IOFile.h"
#include <assert.h>
ZHTTPBEGIN
//contruct function
CIOFile::CIOFile()
{
	Init();
}
CIOFile::CIOFile(const ZString& filePath)
{
	Init();
}
CIOFile::~CIOFile()
{
	Close();
}

//File method

ZInt CIOFile::Create(const ZString& filePath, ZULong fileSize)
{
	Close();
	m_hFile = CreateFile(
		filePath.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (m_hFile == INVALID_HANDLE_VALUE) {
		return -1;
	}
	m_hMapFile = CreateFileMapping(m_hFile,
		NULL, PAGE_READWRITE, 0,
		fileSize, NULL);
	if (m_hMapFile == nullptr) {
		return -1;
	}
	m_filePath = filePath;
	m_mapSize = fileSize;
	return 0;
}
ZInt CIOFile::Open(const ZString& filePath, ZBool bCreate)
{
	Close();
	ZULong openMode;
	if (bCreate) {
		openMode = OPEN_ALWAYS;
	}
	else  {
		openMode = OPEN_EXISTING;
	}
	m_hFile = ::CreateFile(
		filePath.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		openMode,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (m_hFile == INVALID_HANDLE_VALUE) {
		return -1;
	}
	ZULong m_highSize = INVALID_FILE_SIZE;	//high order of file size
	ZULong m_lowSize = INVALID_FILE_SIZE;	//low order of file size
	m_lowSize = GetFileSize(m_hFile, &m_highSize);
	if (m_lowSize == INVALID_FILE_SIZE ||
		m_highSize == INVALID_FILE_SIZE) {
		return -1;
	}

	m_hMapFile = CreateFileMapping(m_hFile,
		NULL, PAGE_READWRITE, m_highSize,
		m_lowSize, NULL);
	if (m_hMapFile == nullptr) {
		return -1;
	}
	m_filePath = filePath;
	m_mapSize = m_lowSize;
	return 0;
}
ZInt CIOFile::OpenPages(const ZString& name, ZULong fileSize)
{
	Close();
	m_hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE, NULL,
		PAGE_READWRITE, 0, fileSize, name.c_str());
	if (m_hMapFile == nullptr) {
		return -1;
	}
	ZBool bInit = (::GetLastError() != ERROR_ALREADY_EXISTS);
	return 0;
}
ZInt CIOFile::Read(ZULong offset, void* outBuf, ZULong length)
{
	assert(outBuf != nullptr);
	assert(length >= 0);
	assert(offset >= 0);
	void* lpViewAddr = nullptr;
	char* lpRet = nullptr;
	if (offset + length > m_mapSize) {
		return 0;
	}
	ZULong ulViewStart = (offset / m_ulSysGran) * m_ulSysGran;
	ZULong ulViewDelta = offset - ulViewStart;
	if (m_hMapFile != nullptr && 
		offset + length <= m_mapSize) {
		lpViewAddr = MapViewOfFile(m_hMapFile,
			FILE_MAP_ALL_ACCESS,
			0,
			ulViewStart,
			length);
		if (lpViewAddr == nullptr) {
			return 0;
		}
		else {
			lpRet = (char*)lpViewAddr + ulViewDelta;
			memcpy_s(outBuf, length, lpRet, length);
			UnmapViewOfFile(lpViewAddr);
			return length;
		}
	}
	else {
		return 0;
	}
}
ZInt CIOFile::Append(ZULong length)
{
	if (m_hFile != INVALID_HANDLE_VALUE &&
		m_hMapFile != nullptr) {
		CloseHandle(m_hMapFile);
		m_hMapFile = CreateFileMapping(m_hFile,
			NULL,
			PAGE_READWRITE,
			0,
			m_mapSize + length,
			NULL);
		if (m_hMapFile == nullptr) {
			return -1;
		}
		m_mapSize += length;
	}
	return 0;
}
ZInt CIOFile::Write(ZULong offset, const void* data, ZULong length)
{
	assert(data != nullptr);
	assert(offset >= 0);
	assert(length >= 0);
	void* lpViewAddr = nullptr;
	char* lpDst = nullptr;
	if (offset + length > m_mapSize) {
		return 0;
	}
	ZULong ulViewStart = (offset / m_ulSysGran) * m_ulSysGran;
	ZULong ulViewDelta = offset - ulViewStart;
	if (m_hMapFile != nullptr) {
		lpViewAddr = MapViewOfFile(m_hMapFile,
			FILE_MAP_ALL_ACCESS, 0,
			ulViewStart, length);
		if (lpViewAddr == 0) {
			return 0;
		}
		else {
			lpDst = (char*)lpViewAddr + ulViewDelta;
			memcpy_s(lpDst, length, data, length);
			UnmapViewOfFile(lpViewAddr);
		}
	}
	return length;
}
ZInt CIOFile::SaveAs(const ZString& filePath)
{
	if (m_hMapFile != nullptr &&
		m_hFile != INVALID_HANDLE_VALUE) {
		char* tmpBuf = new char[m_mapSize];
		ON_SCOPE_EXIT([&] {
			delete[] tmpBuf;
		});
		Read(0, tmpBuf, m_mapSize);
		ThisType tmpThis;
		tmpThis.Create(filePath.c_str(), m_mapSize);
		tmpThis.Write(0, tmpBuf, m_mapSize);
		tmpThis.Close();
	}
	return 0;
}
void CIOFile::Close()
{
	if (m_hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
	if (m_hMapFile != nullptr) {
		CloseHandle(m_hMapFile);
		m_hMapFile = nullptr;
	}
	m_filePath.clear();
	m_mapSize = -1;
}
//private method
void CIOFile::Init()
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	m_ulSysGran = sysInfo.dwAllocationGranularity;
}
ZHTTPEND