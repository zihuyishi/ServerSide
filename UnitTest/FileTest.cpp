#include "FileTest.h"
#include <tchar.h>
#include <assert.h>
using ZHttp::CIOFile;
void FileTest1()
{
	CIOFile file1;
	file1.Create(_T("filetest1.txt"), 1024);
	_TCHAR str1[] = _T("啊打发手动阀是否啊舒服撒法");
	file1.Write(0, str1, sizeof(str1));
	int iVal = 1024;
	file1.Write(512, &iVal, sizeof(iVal));
	file1.SaveAs(_T("filetest2.txt"));
	file1.Close();
	CIOFile file2;
	file2.Open(_T("filetest2.txt"));
	int iVal2 = 0;
	file2.Read(512, &iVal2, sizeof(iVal2));
	assert(iVal2 == iVal);

	wchar_t buf[1024];
	file2.Read(0, buf, sizeof(str1));
	buf[sizeof(str1)] = 0;
	assert(!_tcscmp(buf, str1));

	file2.Append(1024);
	file2.Write(1536, &iVal2, sizeof(iVal2));
	int iVal3 = 0;
	file2.Read(1536, &iVal3, sizeof(iVal3));
	assert(iVal3 == iVal2);
	file2.Close();
	DeleteFile(_T("filetest2.txt"));
	DeleteFile(_T("filetest1.txt"));
}
void FileTest2()
{
	CIOFile file1;
	file1.OpenPages(_T("filetest1"), 2048);
	ZString str1;
	ZInt iVal = 1024;
	str1.append(_T("你好"));
	file1.Write(0, str1.c_str(), str1.length() * sizeof(_TCHAR));
	file1.Write(1024, &iVal, sizeof(iVal));
	CIOFile file2;
	file2.OpenPages(_T("filetest1"), 2048);
	_TCHAR buf[2048];
	ZInt iRet;
	file2.Read(0, buf, str1.length() * sizeof(_TCHAR));
	buf[str1.size()] = 0;
	file2.Read(1024, &iRet, sizeof(iRet));
	assert(!str1.compare(buf));
	assert(iRet == iVal);
	file2.Close();
	file1.Close();
}
void FileTest()
{
	FileTest1();
	FileTest2();
}