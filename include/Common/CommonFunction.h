#include "ZDef.h"
#include <vector>
#include <iostream>

#ifndef ZHTTP_COMMONFUNCTION_HEADER
#define ZHTTP_COMMONFUNCTION_HEADER
ZHTTPBEGIN
namespace StringFunc
{
	std::vector<ZStringA> Split(const ZStringA& src, ZCharA div);
	std::vector<ZStringW> Split(const ZStringW& src, ZCharW div);

	ZStringA StrToUpper(const ZStringA& src);
}

ZHTTPEND
#endif