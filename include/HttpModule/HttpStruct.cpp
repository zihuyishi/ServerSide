#include "HttpStruct.h"
#include "..\Common\commonRegex.h"
#include <assert.h>
#include <string.h>
#include <regex>
#include <iostream>
#include "..\Common\CommonFunction.h"

ZHTTPBEGIN
using namespace CommonDef;
using StringFunc::Split;
RestMethod StrToRestMethod(const ZStringA& method)
{
	ZStringA Method = StringFunc::StrToUpper(method);
	if (Method == "POST") {
		return RestMethod::POST;
	}
	else if (Method == "GET") {
		return RestMethod::GET;
	}
	else if (Method == "PUT") {
		return RestMethod::PUT;
	}
	else if (Method == "DELETE") {
		return RestMethod::DELE;
	}
	else {
		return RestMethod::BAD;
	}
}
//class CURI
CURI::CURI() :
m_uri("")
{

}
CURI::~CURI()
{
	
}
CURI::CURI(const CURI& lhs)
{
	this->operator=(lhs);
}
const CURI& CURI::operator=(const CURI& lhs)
{
	m_uri = lhs.m_uri;
	m_query = lhs.m_query;
	return *this;
}
auto CURI::SetURI(const ZStringA& uri)->ZInt
{
	m_uri = uri;
	ParseURI();
	return 0;
}
auto CURI::GetURI() const->ZStringA
{
	return m_uri;
}
const QUERYTABLE& CURI::GetQueryTable() const
{
	return m_query;
}
auto CURI::GetQuery(const ZStringA& key)->ZStringA
{
	return m_query[key];
}
auto CURI::operator[](const ZStringA& key)->ZStringA
{
	return GetQuery(key);
}
auto CURI::ParseURI() ->ZInt
{
	ZStringA strQuery = m_uri.substr(
		m_uri.find_first_of('?') + 1);
	auto querys = Split(strQuery, '&');
	for each (auto query in querys)
	{
		auto one = Split(query, '=');
		if (one.size() != 2) continue;
		m_query.insert(std::make_pair(one[0], one[1]));
	}
	return 0;
}
//struct CRequestStruct
CRequestStruct ParseRequest(const ZCharA* data)
{
	assert(data != nullptr);
	ZInt iStart = 0,
		iPos = 0,
		length = strlen(data);
	ZStringA temp;
	temp.assign(data);
	CRequestStruct reqRet;
	ZCharA* position = nullptr;
	//Parse Request Line
	do {
		iPos = temp.find(' ');
		if (iPos == ZStringA::npos) break;
		ZStringA method = temp.substr(0, iPos);
		reqRet.method = StrToRestMethod(method);

		iStart = iPos + 1;
		iPos = temp.find(' ', iStart);
		if (iPos == ZStringA::npos) break;
		ZStringA strUri = temp.substr(iStart, iPos - iStart);
		reqRet.uri.SetURI(strUri);

		iStart = iPos + 1;
		iPos = temp.find("\r\n\r\n", iStart);
		if (iPos == ZStringA::npos) break;
		reqRet.body = temp.substr(iPos + 4);
	} while (false);

	return reqRet;
}

ZHTTPEND