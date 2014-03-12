#ifndef HTTP_HTTPMESSAGE_HEADER
#define HTTP_HTTPMESSAGE_HEADER
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
}

#endif