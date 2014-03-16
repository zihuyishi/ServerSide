#include "FunctionTEst.h"
#include <iostream>
#include <assert.h>
using namespace ZHttp::StringFunc;
void StringFunction()
{
	char str1[] = "This is a Test Function";
	auto ret1 = Split(str1, ' ');
	assert(ret1[1] == "is");
	assert(ret1[4] == "Function");
	for each (auto str in ret1)
	{
		std::cout << str.c_str() << std::endl;
	}
}

void FunctionTest()
{
	StringFunction();
}