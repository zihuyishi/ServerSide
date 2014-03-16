#include "..\Common\ZDef.h"
#include "HTTPMessage.h"
#include <iostream>
#include <map>
#ifndef ZHTTP_HTTPSTRUCT_HEADER
#define ZHTTP_HTTPSTRUCT_HEADER

ZHTTPBEGIN
typedef std::map<ZStringA, ZStringA> QUERYTABLE;
typedef QUERYTABLE::iterator QUERYITERATOR;

class CURI
{
public:
	CURI();
	~CURI();
	CURI(const CURI& lhs);
	const CURI& operator=(const CURI& lhs);
public:
	ZInt SetURI(const ZStringA& uri);
	ZStringA GetURI() const;
	const QUERYTABLE& GetQueryTable() const;
	ZStringA GetQuery(const ZStringA& key);
	ZStringA operator[](const ZStringA& key);
private:
	ZInt ParseURI();
private:
	ZStringA m_uri;
	QUERYTABLE m_query;
};
typedef CURI URI;
using HTTPMessage::RestMethod;
typedef struct tagCRequestStruct
{
	RestMethod method;
	URI uri;
	ZStringA body;
}CRequestStruct;
CRequestStruct ParseRequest(const ZCharA* data);

ZHTTPEND

#endif