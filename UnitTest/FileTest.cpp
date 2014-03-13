#include "FileTest.h"
#include <tchar.h>
#include <assert.h>
using ZHttp::CIOFile;
void FileTest()
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

}