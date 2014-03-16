#include "RegexTest.h"
#include "..\include\HttpModule\HttpStruct.h"
using namespace ZHttp;
void RegTest1()
{
	URI uri1;
	uri1.SetURI(R"(http://127.0.0.1/user?name=lichong&password=1234&age=24)");
}
void RegTest()
{
	RegTest1();
}