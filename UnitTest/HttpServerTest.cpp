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
void HttpStructTest()
{
	ZCharA req1[] = "POST http://www.baidu.com/user?name=saye&password=1234 HTTP/1.1\
Accept: image/gif, image/x-xbitmap, image/jpeg\
Accept_Language: en-us\r\n\r\nHello World.\
This is just a Hello World";
	CRequestStruct request1 = ParseRequest(req1);
	assert(request1.method = RestMethod::POST);
	ZStringA str1 = request1.uri.GetQuery("password");
	assert(request1.uri.GetQuery("password") == "1234");
	assert(request1.uri.GetQuery("name") == "saye");
	assert(request1.body == "Hello World.This is just a Hello World");
}
void HttpServerTest()
{
	URITest();
	HttpStructTest();
}