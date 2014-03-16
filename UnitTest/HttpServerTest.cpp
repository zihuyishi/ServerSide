#include "HttpServerTest.h"
#include "..\include\HttpModule\HttpStruct.h"
#include <assert.h>
using namespace ZHttp;
void URITest()
{
	URI uri1;
	uri1.SetURI(R"(http://127.0.0.1/user?name=lichong&password=1234&age=25)");
	ZStringA name = uri1.GetQuery("name");
	ZStringA password = uri1.GetQuery("password");
	ZStringA age = uri1.GetQuery("age");
	assert(name == "lichong");
	assert(password == "1234");
	assert(age == "25");

}
void HttpServerTest()
{
	URITest();
}