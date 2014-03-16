// UnitTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "FileTest.h"
#include "SocketTest.h"
#include "RegexTest.h"
#include "FunctionTEst.h"
int _tmain(int argc, _TCHAR* argv[])
{
	FunctionTest();
	RegTest();
	FileTest();
	SocketTest();
	return 0;
}

