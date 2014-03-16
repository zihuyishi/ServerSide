#include "..\Common\ZDef.h"

#ifndef HTTP_HTTPMESSAGE_HEADER
#define HTTP_HTTPMESSAGE_HEADER
ZHTTPBEGIN
namespace HTTPMessage
{
	enum StatusCode
	{
		Ok = 200,
		Create = 201,
		NoContent = 204,
		PartialContent = 206,
		MovedPermanently = 301,
		MovedTemporarity = 302,
		BadRequest = 400,
		Forbidden = 403,
		NotFound = 404,
		NotImplemented = 501,
		BadGateway = 502,
		GatewayTimeout = 504,
	};
	enum RestMethod
	{
		GET = 0,
		POST = 1,
		PUT = 2,
		DELETE = 3,
		BAD = -1,
	};
}
ZHTTPEND
#endif